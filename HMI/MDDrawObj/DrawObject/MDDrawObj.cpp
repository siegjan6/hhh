#include <QDebug>

#include "MDDrawObj.h"
#include "MDCalculation.h"
#include "MDDrawMulti.h"
#include "MDDrawGroup.h"
#include "IHMIForm.h"
#include "MDParameter.h"
#include "DrawObjData.h"
#include "MDExpInfo.h"
#include "MDPropertyDrawObj.h"
#include "MDUndoData.h"
#include "BindVarDialog.h"

//////////////////////////////////////////////////////////////////////////
CMDDrawObj::CMDDrawObj()
{
    m_object = new CDrawObjData();

    setFactRect(QRectF(400, 100, 200, 100));
}
CMDDrawObj::~CMDDrawObj()
{
    if (m_object->m_property != NULL)
        delete m_object->m_property;

    delete m_object;
}
void CMDDrawObj::initCreate()
{
    initClassInfo(m_object->m_classInfo);
    initExpManager();
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawObj::setFactRect(const QRectF &rf)
{
    setNewRect(rf);
    loadGeneratePathEvent();
}
void CMDDrawObj::setRect(const QRectF& rf)
{
    backupData();
    setFactRect(rf);
}
QRectF CMDDrawObj::rect()
{
    return m_object->m_data.rect();
}
void CMDDrawObj::setMultiParent(CMDDrawMulti *parent)
{
    m_object->m_multiParent = parent;
}
CMDDrawMulti *CMDDrawObj::multiParent()
{
    return m_object->m_multiParent;
}
CMDDrawGroup *CMDDrawObj::groupParent()
{
    return (multiParent()) ? multiParent()->toGroup() : NULL;
}
qreal CMDDrawObj::widenWidth()
{
    return 3;
}
void CMDDrawObj::setBound(const QRectF &rf)
{
    m_object->m_bound = rf;
}
QRectF CMDDrawObj::bound()
{
    return m_object->m_bound;
}
void CMDDrawObj::setParent(IHMIForm *parent)
{
    m_object->m_parent = parent;
}
IHMIForm *CMDDrawObj::parent()
{
    return m_object->m_parent;
}
QPainterPath *CMDDrawObj::basePath()
{
    return &m_object->m_basePath;
}
QPainterPath *CMDDrawObj::matrixPath()
{
    return &m_object->m_basePath;
}
QPainterPath *CMDDrawObj::path()
{
    return &m_object->m_basePath;
}
void CMDDrawObj::setName(const QString &value)
{
    //需要验证名称有效
    QString name = value.trimmed();
    if (!name.isEmpty())
    {
        if (name.compare(m_object->m_name, Qt::CaseInsensitive) == 0)
            return;
        if (parent()->common()->nameManager()->containsName(name))
            return;

        m_object->m_name = name;
        parent()->common()->nameManager()->addName(name);
    }
}
QString CMDDrawObj::name()
{
    return m_object->m_name;
}
QRectF CMDDrawObj::newRect()
{
    return m_object->m_newRect;
}
void CMDDrawObj::setNewRect( const QRectF& rf )
{
    m_object->m_newRect = rf;
    m_object->m_newRect = CMDCalculation::limitRect(m_object->m_newRect);
}
void CMDDrawObj::setDirectRect(const QRectF& rf)
{
    setNewRect(rf);
    m_object->m_data.setRect(newRect());
}
IDataObj *CMDDrawObj::data()
{
    return &m_object->m_data;
}
OrthoMode CMDDrawObj::orthoMode()
{
    return OrthoMode::Square;
}
bool CMDDrawObj::isStudioMode()
{
    if (parent() != NULL)
        return parent()->framework()->isStudioMode();
    return true;
}
void CMDDrawObj::setSupportGroupEvent(bool value)
{
    m_object->m_supportGroupEvent = value;
}
bool CMDDrawObj::supportGroupEvent()
{
    return m_object->m_supportGroupEvent;
}
void CMDDrawObj::setShowSelectedFrame(bool value)
{
    m_object->m_showSelectedFrame = value;
}
bool CMDDrawObj::showSelectedFrame()
{
    return m_object->m_showSelectedFrame;
}
void CMDDrawObj::setToolTip(const QString &value)
{
    m_object->m_toolTip = value;
}
QString CMDDrawObj::toolTip()
{
    return m_object->m_toolTip;
}
void CMDDrawObj::setSupportHidedEvent(bool value)
{
   m_object->m_supportHidedEvent = value;
}
bool CMDDrawObj::supportHidedEvent()
{
    return m_object->m_supportHidedEvent;
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawObj::draw(QPainter* p, bool isPrinting/* = false*/)
{
    if (isPrinting)
        onPrint(p);
    else
        onPaint(p);
}
void CMDDrawObj::mouseFrameMove(const QPointF &point, int pos)
{
    //正交模式,仅在studio模式下
    qreal orthoTan = 0;
    if (isStudioMode())
    {
        if (parent()->studio()->isOrtho())
            orthoTan = CMDCalculation::calcOrthoTan(dataBk()->data()->rect(), pos);
    }

    qreal x = 0, y = 0, width = 0, height = 0;
    CMDCalculation::frameOffset(dataBk(), point, pos, x, y, width, height, orthoTan);
    frameMove(x, y, width, height);
}
bool CMDDrawObj::isVisible(const QPointF& point)
{
    return m_object->m_data.rect().contains(point);
}
void CMDDrawObj::boundMove(qreal xOff, qreal yOff, qreal wOff, qreal hOff)
{
    frameMove(xOff, yOff, wOff, hOff);
}
void CMDDrawObj::mouseDown(const QPointF& point)
{
    backupMousePos(point);
    backupData();
}
void CMDDrawObj::mouseMove(const QPointF& point)
{
    QRectF rf = CMDCalculation::offsetRect(m_object->m_dataBk.data()->rect(),
                                           point.x() - m_object->m_dataBk.mousePos().x(), point.y() - m_object->m_dataBk.mousePos().y(), 0, 0);
    setFactRect(rf);
}
//////////////////////////////////////////////////////////////////////////
QObject *CMDDrawObj::propertyDrawObj()
{
    if (m_object->m_property == NULL)
        m_object->m_property = createPropertyDrawObj();
    return m_object->m_property;
}
CMDPropertyDrawObj *CMDDrawObj::createPropertyDrawObj()
{
    return new CMDPropertyDrawObj(this);
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawObj::frameMove( qreal xOff, qreal yOff, qreal wOff, qreal hOff )
{
    setFactRect(CMDCalculation::offsetRect(m_object->m_dataBk.data()->rect(), xOff, yOff, wOff, hOff));
}
IDrawEnvrionment *CMDDrawObj::environment()
{
    if (parent() == NULL)
        return NULL;

    return dynamic_cast<IDrawEnvrionment*>(parent()->common());
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawObj::invalidate()
{
    if (parent() == NULL)
        return;

    parent()->common()->invalidateObject(this);
}
void CMDDrawObj::generatePath()
{
    setDirectRect(newRect());

    m_object->m_basePath = QPainterPath();
    onGeneratePath();
}
void CMDDrawObj::generateBound()
{
    setBound(m_object->m_data.rect());
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawObj::serialize(QJsonObject &json)
{
    QJsonObject js;
    m_object->serialize(js);
    json.insert("DrawObj", js);
}
void CMDDrawObj::deserialize(const QJsonObject &json)
{
    QJsonObject js = json.value("DrawObj").toObject();
    m_object->deserialize(js);
}
CMDDrawObj* CMDDrawObj::clone()
{
    CMDDrawObj* obj = createSelf();
    obj->initCreate();
    obj->m_object->m_expManager.clear();

    *obj->m_object = *m_object;
    obj->m_object->m_expManager.list()->clear();
    obj->m_object->m_expManager.append(&m_object->m_expManager);

    return obj;
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawObj::unMultiOper()
{
    setMultiParent(NULL);
}
void CMDDrawObj::generateGroupPath()
{
    CMDDrawGroup* group = groupParent();
    while(group != NULL)
    {
        group->loadGeneratePathEvent();
        group = group->groupParent();
    }
}
ICalcData *CMDDrawObj::dataBk()
{
    return &m_object->m_dataBk;
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawObj::backupData()
{
    m_object->m_dataBk.data()->setRect(rect());
    m_object->m_dataBk.setBound(rect());
    m_object->m_dataBk.setMatrixBound(rect());
}
void CMDDrawObj::backupMousePos(const QPointF& point)
{
    m_object->m_dataBk.setMousePos(point);
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawObj::loadSizeEvent()
{
    if (m_object->m_rectBk.topLeft() != m_object->m_data.rect().topLeft())
        onLocationChanged();
    if (m_object->m_rectBk.size() != m_object->m_data.rect().size())
        onSizeChanged();
    m_object->m_rectBk = m_object->m_data.rect();
}
void CMDDrawObj::loadGeneratePathEvent()
{
    invalidate();

    generatePath();
    generateMatrix();
    generateBound();
    generateContent();

    invalidate();

    loadSizeEvent();
}
void CMDDrawObj::loadInitalizationEvent()
{
    m_object->m_newRect = m_object->m_data.rect();

    generatePath();
    generateMatrix();
    generateBound();
    initContent();
    backupData();

    loadSizeEvent();
    onInitialization();
}
void CMDDrawObj::loadMouseEnterEvent()
{
    onMouseEnter();
}
void CMDDrawObj::loadMouseLeaveEvent()
{
    onMouseLeave();
}
void CMDDrawObj::loadDoubleClickEvent(const QPointF &point)
{
    onDoubleClick();
}
void CMDDrawObj::onMouseEnter()
{
    //qDebug() << name() + "  enter";
}
void CMDDrawObj::onMouseLeave()
{
    //qDebug() << name() + "  leave";
}
void CMDDrawObj::onPrint(QPainter *p)
{
    onPaint(p);
}
void CMDDrawObj::onGeneratePath()
{
    m_object->m_basePath.addRect(rect());
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawObj::setVisible(bool value)
{
    m_object->m_visible = value;
    parent()->common()->objectCanSelectChanged(this);
    updateVisible();
}
bool CMDDrawObj::visible()
{
    return m_object->m_visible;
}
bool CMDDrawObj::finalVisible()
{
    //如果是成组控件，取父控件的属性
    if (visible() && groupParent())
        return groupParent()->finalVisible();

    if (parent() == NULL)
        return visible();

    return parent()->common()->visibleLayers()->at(m_object->m_layer) && visible();
}
void CMDDrawObj::setLayer(int layer)
{
    if (layer < 0 || layer > 9)
        return;

    m_object->m_layer = layer;
    parent()->common()->objectCanSelectChanged(this);
    updateVisible();
}
int CMDDrawObj::layer()
{
    return m_object->m_layer;
}
void CMDDrawObj::setLocked(bool value)
{
    m_object->m_locked = value;
    parent()->common()->objectCanSelectChanged(this);
}
bool CMDDrawObj::locked()
{
    return m_object->m_locked;
}
bool CMDDrawObj::finalLocked()
{
    //如果是成组控件，取父控件的属性
    if (!locked() && groupParent())
        return groupParent()->finalLocked();

    if (parent() == NULL)
        return locked();

    return parent()->common()->lockedLayers()->at(m_object->m_layer) || locked();
}
bool CMDDrawObj::canSelect()
{
    bool visible = finalVisible();
    if(!isStudioMode())
        visible = finalVisible() | supportHidedEvent();

    return (visible && !finalLocked());
}
bool CMDDrawObj::canSelect(const QPointF& point)
{
    if (canSelect())
        return isVisible(point);

    return false;
}
bool CMDDrawObj::canSelect(const QRectF& rect)
{
    if (canSelect())
        return rect.intersects(m_object->m_data.rect());

    return false;
}


//////////////////////////////////////////////////////////////////////////
QList<QStringList> CMDDrawObj::expNames()
{
    //组格式
    //#name<<showText<<comment
    //表达式格式
    //name<<showText<<scriptName<<comment
    //隐藏格式
    //$name

    QList<QStringList> list;
    list << QStringList({"#layout",         QObject::tr("布局")});
    list << QStringList({"drawobj.x",       QObject::tr("左")});
    list << QStringList({"drawobj.y",       QObject::tr("顶")});
    list << QStringList({"drawobj.width",   QObject::tr("宽")});
    list << QStringList({"drawobj.height",  QObject::tr("高")});

    list << QStringList({"#appearance",     QObject::tr("外观")});
    list << QStringList({"drawobj.visible", QObject::tr("可见")});
    return list;
}
QDialog *CMDDrawObj::externalBindVarDialog(QWidget* parent, QList<CMDExpInfo *> *list, IDrawEnvrionment *envir, const QString &selectName)
{
    return new CBindVarDialog(list, envir, selectName, parent);
}
void CMDDrawObj::analyseExternalExpression(CMDExpManager *exp, IHMIFramework* framework)
{
    QMap<QString, CMDParameter*>* dict = CMDExpConfig::paraDict();
    for(int i = 0; i < exp->list()->count(); i++)
    {
        CMDExpInfo* value = exp->list()->at(i);
        value->parameters()->clear();
        QString s = value->lowerExpression();
        if (s.isEmpty())
            continue;

        QList<QString> varNames = value->getReferencedVarList();
        for(int i = 0; i < varNames.count(); i++)
        {
            QString name = varNames.at(i);
            CMDParameter* p = NULL;

            if (dict->contains(name))
                p = dict->value(name);
            else
            {
                p = new CMDParameter(name);
                //初始化的时候获取变量值
                p->setValue(framework->variableValue(p->name()));
                dict->insert(name, p);
            }
            p->list()->append(value);
            value->parameters()->append(p);
        }

        value->onDataChanged();
    }
}
CMDExpManager *CMDDrawObj::expManager()
{
    return &m_object->m_expManager;
}
void CMDDrawObj::initExpManager()
{
    m_object->m_expManager.clear();
    m_object->m_expManager.append(CMDExpConfig::expManager((int)type()));
}
void CMDDrawObj::replaceCustomProperty()
{
    if (groupParent() == NULL)
        return;
    for(int i = 0; i < expManager()->list()->count(); i++)
    {
        CMDExpInfo* value = expManager()->list()->at(i);
        QString name = value->expression();
        replaceCustomString(name);
        value->setExpression(name);
    }
}
void CMDDrawObj::analyseExpression()
{
    addAddtionalExpression();

    expManager()->setContainer(this);

    for(int i = 0; i < expManager()->list()->count(); i++)
    {
        CMDExpInfo* value = expManager()->list()->at(i);
        analyseSingleExpression(value);
    }
}
void CMDDrawObj::loadDataChangedEvent(CMDExpInfo *exp)
{
    if(exp->type() == (int)DrawType::Obj)
    {
        switch (exp->index())
        {
        case 0:     //x
        {
            QRectF rf = rect();
            rf.moveLeft(exp->decimalValue());
            setDynamicRect(rf);
        }
            break;
        case 1:     //y
        {
            QRectF rf = rect();
            rf.moveTop(exp->decimalValue());
            setDynamicRect(rf);
        }
            break;
        case 2:     //width
        {
            QRectF rf = rect();
            rf.setWidth(exp->decimalValue());
            setDynamicRect(rf);
        }
            break;
        case 3:     //height
        {
            QRectF rf = rect();
            rf.setHeight(exp->decimalValue());
            setDynamicRect(rf);
        }
            break;
        case 4://visible
            setVisible(exp->decimalValue() != 0);
            break;
        default:
            break;
        }
    }

    if(exp->type() == (int)type())
        onDataChanged(exp);
}
void CMDDrawObj::addExpressionToManager(const QList<QString> &exps, QList<int> *indexs)
{
    if(indexs != NULL)
        Q_ASSERT(indexs->count() == exps.count());

    for (int i = 0; i < exps.count(); i++)
    {
        int index = i;
        if(indexs != NULL)
            index = indexs->at(i);
        m_object->m_expManager.append(new CMDExpInfo(exps.at(i), (int)type(), index));
    }
}
void CMDDrawObj::replaceCustomString(QString &name)
{
    //获取@前缀的变量并将其替换
    while (name.contains("@"))
    {
        int index = name.indexOf("@", 0);
        int count = 0;
        int i = index + 1;

        while (i < name.count())
        {
            QChar c = name.at(i++);
            if ((c >= '0' && c <= '9')
                || (c >= 'A' && c <= 'z')
                || (c == '_'))
                count++;
            else
                break;
        }
        QString custom = name.mid(index + 1, count);
        name.remove(index, count + 1);

        if (count == 0)
            continue;

        bool hasFind = false;
        CMDDrawGroup* group = groupParent();
        while (group != NULL)
        {
            QList<CMDCustomProperty>* list = group->customPropertys();
            for (int i = 0; i < list->count(); i++)
            {
                CMDCustomProperty p = list->at(i);

                if (custom.compare(p.name(), Qt::CaseInsensitive) == 0)
                {
                    name.insert(index, "("+p.expression()+")");//加个(),预防操作符优先级问题
                    hasFind = true;
                    break;
                }
            }

            if (hasFind)
                break;
            group = group->groupParent();
        }
    }
}
void CMDDrawObj::analyseSingleExpression(CMDExpInfo* exp)
{
    QMap<QString, CMDParameter*>* dict = CMDExpConfig::paraDict();
    exp->parameters()->clear();
    QString s = exp->lowerExpression();
    if (s.isEmpty())
        return;

    QList<QString> varNames = exp->getReferencedVarList();
    for(int i = 0; i < varNames.count(); i++)
    {
        QString name = varNames.at(i);
        CMDParameter* p = NULL;

        if (dict->contains(name))
            p = dict->value(name);
        else
        {
            p = new CMDParameter(name);
            //初始化的时候获取变量值
            p->setValue(parent()->framework()->variableValue(p->name()));
            dict->insert(name, p);
        }
        p->list()->append(exp);
        exp->parameters()->append(p);
    }

    exp->onDataChanged();
}
//////////////////////////////////////////////////////////////////////////
ICalcData *CMDDrawObj::createCalcData()
{
    return new CCalcData();
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawObj::setDynamicRect(const QRectF &value)
{
    setRect(value);
    generateGroupPath();
}
//////////////////////////////////////////////////////////////////////////
IUndoDataObj* CMDDrawObj::createUndoData()
{
    return new CMDUndoDataObj;
}
bool CMDDrawObj::revertUndoData(IUndoDataObj *value)
{
    bool hasChange = false;
    CMDUndoDataObj* u = dynamic_cast<CMDUndoDataObj*>(value);
    if (!dynamic_cast<CDataObj*>(u->data())->equal(&m_object->m_data))
    {
        hasChange = true;
        setNewRect(u->data()->rect());
    }

    return hasChange;
}
IUndoDataObj* CMDDrawObj::fillUndoData(bool isOld)
{
    CMDUndoDataObj* u = dynamic_cast<CMDUndoDataObj*>(createUndoData());
    u->setObj(this);
    if (isOld)
        u->setData(dynamic_cast<CDataObj*>(dataBk()->data()));
    else
        u->setData(&m_object->m_data);

    return u;
}
void CMDDrawObj::undoOperate(IUndoDataObj *value)
{
    if (revertUndoData(value))
        loadGeneratePathEvent();
}
bool CMDDrawObj::undoEqual()
{
    return dynamic_cast<CDataObj*>(dataBk()->data())->equal(&m_object->m_data);
}
//////////////////////////////////////////////////////////////////////////
CMDMultiLanguage *CMDDrawObj::language()
{
    return CMDMultiLanguage::instance();
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawObj::setStorageImagePath(const QString &value)
{
    m_object->m_storageImagePath = value;
}
QString CMDDrawObj::getImagePath()
{
    if(!m_object->m_storageImagePath.isEmpty())
        return m_object->m_storageImagePath;

    if(parent() != NULL)
        return parent()->framework()->projectImagePath();

    return "";
}
void CMDDrawObj::resetImagePath(const QString &path)
{
    QList<CMDBrushManager *> brushes;
    getBrushes(brushes);
    for(int i = 0; i < brushes.count(); i++)
        brushes.at(i)->setProjectPath(path);

    QList<CMDPenManager *> pens;
    getPens(pens);
    for(int i = 0; i < pens.count(); i++)
        pens.at(i)->setProjectPath(path);
}
void CMDDrawObj::copyImage(const QString &sourcePath, const QString &targetPath, bool needRename)
{
    QList<CMDBrushManager *> brushes;
    getBrushes(brushes);
    for(int i = 0; i < brushes.count(); i++)
        brushes.at(i)->copyImage(sourcePath, targetPath, needRename);

    QList<CMDPenManager *> pens;
    getPens(pens);
    for(int i = 0; i < pens.count(); i++)
        pens.at(i)->copyImage(sourcePath, targetPath, needRename);
}
void CMDDrawObj::getBrushes(QList<CMDBrushManager *> &list)
{
    list.clear();
}
void CMDDrawObj::getPens(QList<CMDPenManager *> &list)
{
    list.clear();
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawObj::setScriptExpression(const QString &name, const QString &expression)
{
    //替换自定义字符串
    QString exp = expression;
    replaceCustomString(exp);

    QList<CMDExpInfo*>* list = expManager()->list();
    for(int i = 0; i < list->count(); i++)
    {
        CMDExpInfo* p = list->at(i);
        if(p->name().compare(name, Qt::CaseInsensitive) == 0)
        {
            p->setExpression(expression);
            analyseSingleExpression(p);
            return;
        }
    }
}
//////////////////////////////////////////////////////////////////////////
QStringList *CMDDrawObj::classInfo()
{
    return &m_object->m_classInfo;
}
CMDPropertyDrawObj *CMDDrawObj::createPropertyDrawObj(const QString &className)
{
    if(className.compare("DrawObj", Qt::CaseInsensitive) == 0)
        return new CMDPropertyDrawObj(this);

    return NULL;
}
void CMDDrawObj::initClassInfo(QStringList &classInfo)
{
    classInfo.clear();
    classInfo.append("DrawObj");
}
//////////////////////////////////////////////////////////////////////////










