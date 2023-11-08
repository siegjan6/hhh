#include "MDDrawMulti.h"
#include "DrawMultiData.h"
#include "MDCalculation.h"
#include "MDUndoData.h"
#include "MDDrawGroup.h"
#include "MDDrawCombine.h"

//////////////////////////////////////////////////////////////////////////
CMDDrawMulti::CMDDrawMulti(QList<CMDDrawObj *> *list/* = NULL*/)
{
    m_multi = new CDrawMultiData;

    if (list == NULL)
        return;
    Q_ASSERT(list->count() != 0);

    m_multi->m_objList.append(*list);
    for(int i = 0; i < m_multi->m_objList.count(); i++)
        m_multi->m_objList.at(i)->setMultiParent(this);
    setParent(m_multi->m_objList.at(0)->parent());
}
CMDDrawMulti::~CMDDrawMulti()
{
    clear();
    delete m_multi;
}
void CMDDrawMulti::clear()
{
    qDeleteAll(m_multi->m_objList);
    m_multi->m_objList.clear();
}
//////////////////////////////////////////////////////////////////////////
QPainterPath *CMDDrawMulti::matrixPath()
{
    return isVector() ? CMDDrawVector::matrixPath() : basePath();
}
QPainterPath *CMDDrawMulti::path()
{
    return isVector() ? CMDDrawVector::path() : basePath();
}
QTransform *CMDDrawMulti::matrix()
{
    return isVector() ? CMDDrawVector::matrix() : NULL;
}
QList<CMDDrawObj*> *CMDDrawMulti::objList()
{
    return &m_multi->m_objList;
}
CMDDrawGroup *CMDDrawMulti::toGroup()
{
    if(type() == DrawType::Group)
        return dynamic_cast<CMDDrawGroup*>(this);

    return NULL;
}
CMDDrawCombine *CMDDrawMulti::toCombine()
{
    if(type() == DrawType::Combine)
        return dynamic_cast<CMDDrawCombine*>(this);

    return NULL;
}
int CMDDrawMulti::baseVersion()
{
    return m_multi->m_baseVersion;
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawMulti::unMulti()
{
    for(int i = 0; i < m_multi->m_objList.count(); i++)
        m_multi->m_objList.at(i)->unMultiOper();
    clearData();
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawMulti::mouseMove(const QPointF &point)
{
    for(int i = 0; i < m_multi->m_objList.count(); i++)
        m_multi->m_objList.at(i)->mouseMove(point);

    loadGeneratePathEvent();
}
void CMDDrawMulti::mouseFrameMove(const QPointF &point, int pos)
{
    //正交模式
    qreal orthoTan = 0;
    if (parent()->studio()->isOrtho())
        orthoTan = CMDCalculation::calcOrthoTan(dataBk()->data()->rect(), pos);

    qreal xOff = 0, yOff = 0, wOff = 0, hOff = 0;
    CMDCalculation::frameOffset(dataBk(), point, pos, xOff, yOff, wOff, hOff, orthoTan);
    childObjBoundMove(xOff, yOff, wOff, hOff);

    if (isVector())
        frameMove(xOff, yOff, wOff, hOff);
    else
        loadGeneratePathEvent();
}
void CMDDrawMulti::boundMove(qreal xOff, qreal yOff, qreal wOff, qreal hOff)
{
    QRectF rect = isVector() ?
                baseBoundMove(xOff, yOff, wOff, hOff) :
                CMDCalculation::offsetRect(dataBk()->data()->rect(), xOff, yOff, wOff, hOff);

    setFactRect(rect);
}
void CMDDrawMulti::multiRotate(qreal angle, const QPointF &center)
{
    QRectF rect = baseMultiRotate(angle, center);

    qreal xOff = rect.x() - dataBk()->data()->rect().x();
    qreal yOff = rect.y() - dataBk()->data()->rect().y();
    for(int i = 0; i < m_multi->m_objList.count(); i++)
    {
        CMDDrawObj* obj = m_multi->m_objList.at(i);
        obj->setFactRect(CMDCalculation::offsetRect(obj->dataBk()->data()->rect(), xOff, yOff, 0, 0));
    }

    loadGeneratePathEvent();
}
void CMDDrawMulti::loadInitalizationEvent()
{
    for(int i = 0; i < m_multi->m_objList.count(); i++)
    {
        CMDDrawObj* obj = m_multi->m_objList.at(i);
        obj->setMultiParent(this);
        obj->loadInitalizationEvent();
    }

    CMDDrawVector::loadInitalizationEvent();
}
void CMDDrawMulti::setFactRect(const QRectF &rect)
{
    qreal xOff = rect.x() - dataBk()->data()->rect().x();
    qreal yOff = rect.y() - dataBk()->data()->rect().y();
    qreal wOff = rect.width() - dataBk()->data()->rect().width();
    qreal hOff = rect.height() - dataBk()->data()->rect().height();
    childObjBoundMove(xOff, yOff, wOff, hOff);

    loadGeneratePathEvent();
}
void CMDDrawMulti::generateMatrix()
{
    if (!isVector())
        return;

    CMDDrawVector::generateMatrix();
    for(int i = 0; i < m_multi->m_objList.count(); i++)
    {
        CMDDrawObj* obj = m_multi->m_objList.at(i);
        obj->generateMatrix();
    }
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawMulti::childObjBoundMove(qreal xOff, qreal yOff, qreal wOff, qreal hOff)
{
    QRectF rf = dataBk()->data()->rect();
    qreal xOffRate = wOff / rf.width();
    qreal yOffRate = hOff / rf.height();

    for(int i = 0; i < m_multi->m_objList.count(); i++)
    {
        CMDDrawObj* obj = m_multi->m_objList.at(i);

        QRectF ojbRect = obj->dataBk()->matrixBound();
        qreal objX = xOff + (ojbRect.x() - rf.x()) * xOffRate;
        qreal objW = ojbRect.width() * xOffRate;
        qreal objY = yOff + (ojbRect.y() - rf.y()) * yOffRate;
        qreal objH = ojbRect.height() * yOffRate;

        obj->boundMove(objX, objY, objW, objH);
    }
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawMulti::serialize(QJsonObject &json)
{
    CMDDrawVector::serialize(json);

    QJsonObject js;
    m_multi->serialize(js);
    json.insert("DrawMulti", js);
}
void CMDDrawMulti::deserialize(const QJsonObject &json)
{
    clear();
    CMDDrawVector::deserialize(json);

    QJsonObject js = json.value("DrawMulti").toObject();
    m_multi->deserialize(js);
}
CMDDrawObj *CMDDrawMulti::clone()
{
    CMDDrawMulti* obj = dynamic_cast<CMDDrawMulti*>(CMDDrawVector::clone());
    *obj->m_multi = *m_multi;

    return obj;
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawMulti::setParent(IHMIForm *value)
{
    CMDDrawVector::setParent(value);
    foreach(CMDDrawObj* obj, m_multi->m_objList)
        obj->setParent(value);
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawMulti::backupData()
{
    for(int i = 0; i < m_multi->m_objList.count(); i++)
    {
        CMDDrawObj* obj = m_multi->m_objList.at(i);
        obj->backupData();
    }

    CMDDrawVector::backupData();
}
void CMDDrawMulti::backupMousePos(const QPointF &point)
{
    for(int i = 0; i < m_multi->m_objList.count(); i++)
    {
        CMDDrawObj* obj = m_multi->m_objList.at(i);
        obj->backupMousePos(point);
    }

    CMDDrawVector::backupMousePos(point);
}
//////////////////////////////////////////////////////////////////////////
IUndoDataObj *CMDDrawMulti::createUndoData()
{
    return new CMDUndoDataMulti;
}
bool CMDDrawMulti::revertUndoData(IUndoDataObj *value)
{
    bool hasChange = CMDDrawVector::revertUndoData(value);

    if (hasChange)
    {
        CMDUndoDataMulti* u = dynamic_cast<CMDUndoDataMulti*>(value);
        for(int i = 0; i < u->list()->count(); i++)
        {
            IUndoDataObj* d = u->list()->at(i);

            d->obj()->revertUndoData(d);
            d->obj()->loadGeneratePathEvent();
        }
    }

    return hasChange;
}
IUndoDataObj *CMDDrawMulti::fillUndoData(bool isOld)
{
    CMDUndoDataMulti* u = dynamic_cast<CMDUndoDataMulti*>(CMDDrawVector::fillUndoData(isOld));
    for(int i = 0; i < m_multi->m_objList.count(); i++)
    {
        CMDDrawObj* obj = m_multi->m_objList.at(i);
        u->list()->append(obj->fillUndoData(isOld));
    }

    return u;
}
void CMDDrawMulti::undoAdd()
{
    for(int i = 0; i < m_multi->m_objList.count(); i++)
    {
        CMDDrawObj* obj = m_multi->m_objList.at(i);
        obj->undoAdd();
    }
}
void CMDDrawMulti::undoDelete()
{
    for(int i = 0; i < m_multi->m_objList.count(); i++)
    {
        CMDDrawObj* obj = m_multi->m_objList.at(i);
        obj->undoDelete();
    }
}
void CMDDrawMulti::clearData()
{
    dataVector()->setRotateAngle(0);
    dataVector()->setShear(0);
    dataVector()->setRotatePoint(QPointF(0.5, 0.5));
    dataVector()->setXScale(1);
    dataVector()->setYScale(1);
    dataVector()->setScalePoint(QPointF(0.5, 0.5));
    dataVector()->setIsFlipX(false);
    dataVector()->setIsFlipY(false);
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawMulti::changeLanguage()
{
    for(int i = 0; i < m_multi->m_objList.count(); i++)
        m_multi->m_objList.at(i)->changeLanguage();

    CMDDrawVector::changeLanguage();
}
void CMDDrawMulti::collectWords()
{
    for(int i = 0; i < m_multi->m_objList.count(); i++)
        m_multi->m_objList.at(i)->collectWords();

    CMDDrawVector::collectWords();
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawMulti::getBrushes(QList<CMDBrushManager *> &list)
{
    CMDDrawVector::getBrushes(list);

    for(int i = 0; i < m_multi->m_objList.count(); i++)
        m_multi->m_objList.at(i)->getBrushes(list);
}
void CMDDrawMulti::getPens(QList<CMDPenManager *> &list)
{
    CMDDrawVector::getPens(list);

    for(int i = 0; i < m_multi->m_objList.count(); i++)
        m_multi->m_objList.at(i)->getPens(list);
}
void CMDDrawMulti::resetImagePath(const QString &path)
{
    CMDDrawVector::resetImagePath(path);

    for(int i = 0; i < m_multi->m_objList.count(); i++)
        m_multi->m_objList.at(i)->resetImagePath(path);
}
void CMDDrawMulti::copyImage(const QString &sourcePath, const QString &targetPath, bool needRename)
{
    CMDDrawVector::copyImage(sourcePath, targetPath, needRename);

    for(int i = 0; i < m_multi->m_objList.count(); i++)
        m_multi->m_objList.at(i)->copyImage(sourcePath, targetPath, needRename);
}
//////////////////////////////////////////////////////////////////////////



