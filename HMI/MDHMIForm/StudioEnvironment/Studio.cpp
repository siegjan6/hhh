#include <QtAlgorithms>
#include <QDebug>
#include <QApplication>
#include <QVBoxLayout>
#include <QDialog>

#include "Studio.h"
#include "IHMIStudio.h"
#include "MDDrawObj.h"
#include "MDDrawWidget.h"
#include "MDDrawGroup.h"
#include "MDDrawCombine.h"
#include "MDHMIForm.h"
#include "MDPropertyDrawText.h"
#include "SelectObjectManager.h"
#include "ControlPointContainer.h"
#include "PropertySelectObject.h"
#include "LayerConfigDialog.h"
#include "HMIFormData.h"
#include "IGraphicsStorage.h"
#include "ObjectChangedCommand.h"
#include "FormCommand.h"
#include "SelectObjectCommand.h"
#include "PropertyBrowser.h"

QList<CMDDrawObj*> CStudio::m_copyList;
QString CStudio::m_copyImagePath;
//////////////////////////////////////////////////////////////////////////
CStudio::CStudio(IHMIForm *control): CEnvironment(control)
{
    m_isSelectFrame = false;
    m_rotateCursor = QCursor(QPixmap(":/rs/image/rotate.png"));
    m_isOrtho = false;
    m_isGrid = false;
    m_formScale = QPointF(1, 1);
    MoveSize = 5;

    m_create = new CCreateDrawObject(this);
    m_controlPoint = new CControlPointContainer(container());
    m_rubberBand = new QRubberBand(QRubberBand::Rectangle, window());
}
CStudio::~CStudio()
{
    delete m_controlPoint;
    delete m_create;

    qDeleteAll(m_deletedObjs);
    m_deletedObjs.clear();
    //解组后的group只需要删除自身，不需要删除子控件
    for(int i = 0; i < m_deletedGroups.count(); i++)
        dynamic_cast<CMDDrawGroup*>(m_deletedGroups.at(i))->objList()->clear();
    qDeleteAll(m_deletedGroups);
    m_deletedGroups.clear();

    for(int i = 0; i < m_deletedCombines.count(); i++)
        dynamic_cast<CMDDrawCombine*>(m_deletedCombines.at(i))->objList()->clear();
    qDeleteAll(m_deletedCombines);
    m_deletedCombines.clear();
}
//////////////////////////////////////////////////////////////////////////
void CStudio::paint(QPainter *p, const QRect& r)
{
    //background
    p->fillRect(0, 0, window()->width(), window()->height(), QColor(0x80, 0x80, 0x80));
    p->setRenderHint(QPainter::Antialiasing, true);
    qreal scaleW = formScale().x();
    qreal scaleH = formScale().y();
    QSize size = rect().size();

    p->save();
    p->setClipRect(0, 0, size.width()*scaleW, size.height()*scaleH);
    p->scale(scaleW, scaleH);
    CEnvironment::paint(p, r);
    p->restore();

    m_controlPoint->draw(p);
}
void CStudio::drawBackground(QPainter *p)
{
    CEnvironment::drawBackground(p);

    //分隔线
    QSize size = window()->rect().size();
    const int interval = 100;
    p->setPen(Qt::gray);
    for (int i = 0; i < size.width() / interval; i++)
        p->drawLine((i+1)*interval, 0, (i+1)*interval, size.height());
    for (int j = 0; j < size.height() / interval; j++)
        p->drawLine(0, (j+1)*interval, size.width(), (j+1)*interval);
}
void CStudio::objectCanSelectChanged(CMDDrawObj *obj)
{
    CEnvironment::objectCanSelectChanged(obj);

    //如果是不可操作图层，从选择列表中删除
    if (!obj->canSelect())
    {
        if (m_selectedObjs.contains(obj))
        {
            m_selectedObjs.removeOne(obj);
            m_controlPoint->changeSelectObj(&m_selectedObjs);
        }
    }
}
//////////////////////////////////////////////////////////////////////////
void CStudio::mouseMove(Qt::MouseButton button, const QPointF &location)
{
    QPointF pf = getRevertScrollAndScalePointF(location);
    if (m_create->mouseMove(button, location, pf, m_downPoint))
        return;

    CEnvironment::mouseMove(button, pf);

    //选择框
    if (m_isSelectFrame)
    {
        m_rubberBand->setGeometry(
                    QRect(m_downPoint.toPoint(), location.toPoint()).normalized());
        return;
    }

    ControlState state = ControlState::None;
    int pos = 0;
    m_controlPoint->mouseMove(pf, state, pos);

    switch (state)
    {
        case ControlState::Move:
            window()->setCursor(Qt::OpenHandCursor);
            break;
        case ControlState::Shear:
            window()->setCursor(Qt::SplitHCursor);
            break;
        case ControlState::Center:
        case ControlState::Custom:
            window()->setCursor(Qt::PointingHandCursor);
            break;
        case ControlState::Rotate:
            window()->setCursor(m_rotateCursor);
            break;
        case ControlState::FrameMove:
            window()->setCursor(Qt::SizeAllCursor);
            break;
        case ControlState::MoveNode:
        case ControlState::AddNode:
        case ControlState::DeleteNode:
            window()->setCursor(Qt::PointingHandCursor);
            break;
        case ControlState::FormWidth:
            window()->setCursor(Qt::SizeHorCursor);
            break;
        case ControlState::FormHeight:
            window()->setCursor(Qt::SizeVerCursor);
            break;
//        case ControlState::Segment:
//            break;
        default:
            window()->setCursor(Qt::ArrowCursor);
            break;
    }
}
void CStudio::mouseDown(Qt::MouseButton button, const QPointF &location)
{
    window()->setFocus();

    QPointF pf = getRevertScrollAndScalePointF(location);
    m_downPoint = location;

    if (m_create->mouseDown(button, location, pf))
        return;

    CEnvironment::mouseDown(button, pf);

    if (button != Qt::LeftButton)
        return;
    if (m_controlPoint->mouseDown(pf))
        return;

    CMDDrawObj* drawObj = NULL;
    int count = objs()->count();
    for (int i = (count -1); i >= 0; i--)
    {
        CMDDrawObj* obj = objs()->at(i);
        if (obj->canSelect(pf))
        {
            drawObj = obj;
            break;
        }
    }

    if (drawObj != NULL)
    {
        if (QApplication::keyboardModifiers() == Qt::ControlModifier)
        {
            if (m_selectedObjs.contains(drawObj))
                m_selectedObjs.removeOne(drawObj);
            else
                m_selectedObjs.append(drawObj);
        }
        else if (QApplication::keyboardModifiers() == Qt::ShiftModifier)
        {
            //最后选中的DrawObj位置移动到列表的最后
            if (m_selectedObjs.contains(drawObj))
               m_selectedObjs.removeOne(drawObj);
            m_selectedObjs.append(drawObj);
        }
        else
        {
            if (m_selectedObjs.contains(drawObj))
                m_selectedObjs.removeOne(drawObj);
            else
                m_selectedObjs.clear();
            m_selectedObjs.append(drawObj);
        }
    }
    else
    {
        m_selectedObjs.clear();
        m_isSelectFrame = true;
        m_rubberBand->setGeometry(QRect(m_downPoint.toPoint(), QSize()));
        m_rubberBand->show();
    }

    m_controlPoint->changeSelectObj(&m_selectedObjs);
    m_controlPoint->mouseDown(pf);
}
void CStudio::mouseUp(Qt::MouseButton button, const QPointF &location)
{
    QPointF pf = getRevertScrollAndScalePointF(location);
    QPointF revertDownPoint = getRevertScrollAndScalePointF(m_downPoint);
    if (m_create->mouseUp(button, location, pf, revertDownPoint))
        return;

    CEnvironment::mouseUp(button, pf);

    if (m_isSelectFrame)
    {
        m_selectedObjs.clear();

        QRectF rf = QRectF(m_downPoint, location).normalized();
        rf = getRevertScrollAndScaleRect(rf);

        for(int i = 0; i < objs()->count(); i++)
        {
            CMDDrawObj* obj = objs()->at(i);
            if (obj->canSelect(rf))
                m_selectedObjs.append(obj);
        }

        m_controlPoint->changeSelectObj(&m_selectedObjs);
        m_isSelectFrame = false;
        m_rubberBand->hide();
    }

    m_controlPoint->mouseUp(pf);
    refreshProperty();
}
void CStudio::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_Delete://delete
        deleteObj();
        container()->setModify(true);
        break;
    case Qt::Key_C://copy
        if (e->modifiers() & Qt::ControlModifier)
            copy();
        break;
    case Qt::Key_V://paste
        if (e->modifiers() & Qt::ControlModifier)
        {
            paste();
            container()->setModify(true);
        }
        break;
    case Qt::Key_S://save
        if (e->modifiers() & Qt::ControlModifier)
            window()->saveEvent();
        break;
    case Qt::Key_A://select all
        if (e->modifiers() & Qt::ControlModifier)
            selectAll();
        break;
    case Qt::Key_Up://up
        if (e->modifiers() & Qt::ShiftModifier)
            decreaseHeight();
        else
            toTop();
        container()->setModify(true);
        break;
    case Qt::Key_Down://down
        if (e->modifiers() & Qt::ShiftModifier)
            increaseHeight();
        else
            toBottom();
        container()->setModify(true);
        break;
    case Qt::Key_Left://left
        if (e->modifiers() & Qt::ShiftModifier)
            decreaseWidth();
        else
            toLeft();
        container()->setModify(true);
        break;
    case Qt::Key_Right://right
        if (e->modifiers() & Qt::ShiftModifier)
            increaseWidth();
        else
            toRight();
        container()->setModify(true);
        break;
    default:
        break;
    }
}
void CStudio::mouseDoubleClickEvent(const QPointF& point)
{
    m_create->end();

    int count = m_selectedObjs.count();
    if (count > 0)
        m_selectedObjs.at(count - 1)->loadDoubleClickEvent(point);
}
//////////////////////////////////////////////////////////////////////////
void* CStudio::property()
{
    return window()->framework()->manager()->property();
}
//////////////////////////////////////////////////////////////////////////
void CStudio::initalization()
{
    CEnvironment::initalization();
    setScrollSize();

    changeLanguage();
}
void CStudio::setFullName(const QString &value)
{
    CEnvironment::setFullName(value);

    QString str = QFileInfo(fullName()).baseName();
    window()->setWindowText(str);
}
//////////////////////////////////////////////////////////////////////////
void CStudio::invalidateObject(CMDDrawObj *obj)
{
    int w = 3 + obj->widenWidth();
    QRect rect = getScrollAndScaleRect(obj->bound()).toRect();
    rect.adjust(-w, -w, w, w);

    window()->update(rect);

    if (m_controlPoint->contains(obj))
        m_controlPoint->invalidate();
}
void CStudio::invalidateControlPoint(CControlPointContainer *control)
{
    QRect rect = control->invalidateRect().toRect();
    //rect.Offset(Container.AutoScrollPosition);
    window()->update(rect);
    rect = control->rotateCenterRect().toRect();
    //rect.Offset(Container.AutoScrollPosition);
    window()->update(rect);
}
void CStudio::timeInvalidate()
{
    QRectF rf = rectInvalidate();
    if (rf.isValid())
    {
        window()->update(rf.toAlignedRect());
        setRectInvalidate(QRectF());
    }
}
void CStudio::invalidateBatchControlPoint()
{
    CSelectObjectManager* selectObjs = m_controlPoint->selectObjs();
    selectObjs->resetProperty();
    selectObjs->invalidate();
    refreshProperty();
}
//////////////////////////////////////////////////////////////////////////
void CStudio::setRect(const QRect &value)
{
    CEnvironment::setRect(value);

    resizeEvent(rect().size());
    setScrollSize();
}
void CStudio::setFormScale(const QPointF &value)
{
    const qreal min = 0.25;
    const qreal max = 4;
    qreal x = value.x();
    qreal y = value.y();
    x = (x < min) ? min : x;
    x = (x > max) ? max : x;
    y = (y < min) ? min : y;
    y = (y > max) ? max : y;
    QPointF pf(x, y);

    if (pf != m_formScale)
    {
        m_formScale = pf;
        for(int i = 0; i < objs()->count(); i++)
        {
            CMDDrawObj* obj = objs()->at(i);
            obj->updateFormScale();
        }

        setScrollSize();
        m_controlPoint->invalidate();
        window()->update();
    }
}
//////////////////////////////////////////////////////////////////////////
void CStudio::setScrollSize()
{
    int width = rect().size().width()*formScale().x() + 5;
    int height = rect().size().height()*formScale().y() + 28;
    window()->setFixedSize(width, height);
}
//////////////////////////////////////////////////////////////////////////
void CStudio::group()
{
    int count = m_selectedObjs.count();
    if (count > 1)
    {
        //需要排序，否则绘图的先后顺序会发生改变
        QList<int> indexA = objectIndexs(&m_selectedObjs);
        QList<CMDDrawObj*> groupL;
        for(int i = 0; i < indexA.count(); i++)
            groupL.append(objs()->at(indexA.at(i)));
        CMDDrawGroup* group = new CMDDrawGroup(&groupL);
        group->initCreate();
        dynamic_cast<CObjName*>(nameManager())->createName(group, false);
        group->setLayer(defaultLayer());


        QList<CMDDrawObj*> groups;
        groups.append(group);
        QList<int> indexs;
        indexs.append(indexA.at(0));;
        m_undo.push(new CObjectChangedCommand(&groups, &indexs, this, (int)ObjectChangedId::Group));

        m_selectedObjs.clear();
        m_selectedObjs.append(group);

        m_controlPoint->changeSelectObj(&m_selectedObjs);
        window()->update();

        objectChangedEvent()->group(group);
    }
}
void CStudio::unGroup()
{
    QList<CMDDrawObj*> list;
    QList<int> indexs;
    for(int i = 0; i < m_selectedObjs.count(); i++)
    {
        CMDDrawObj* obj = m_selectedObjs.at(i);
        if (obj->type() != DrawType::Group)
            continue;
        list.append(obj);
    }

    if (!list.isEmpty())
    {
        indexs = objectIndexs(&list);
        list.clear();
        for(int i = 0; i < indexs.count(); i++)
            list.append(objs()->at(indexs.at(i)));
    }
    m_undo.push(new CObjectChangedCommand(&list, &indexs, this, (int)ObjectChangedId::UnGroup));


    if (!list.isEmpty())
    {
        CMDDrawGroup* group = dynamic_cast<CMDDrawGroup*>(list.at(0));
        //去除锁定的控件
        int count = group->objList()->count();
        for (int i = count - 1; i >= 0; i--)
        {
            if (!group->objList()->at(i)->canSelect())
                group->objList()->removeAt(i);
        }

        m_selectedObjs.clear();
        m_selectedObjs.append(*group->objList());

        m_controlPoint->changeSelectObj(&m_selectedObjs);
        window()->update();
    }

    //通知对象对话框
    for (int i = 0; i < list.count(); i++)
        objectChangedEvent()->unGroup(list.at(i));
}
void CStudio::combine()
{
    int count = m_selectedObjs.count();
    if(count < 2)
        return;

    //需要排序，否则绘图的先后顺序会发生改变
    QList<int> indexA = objectIndexs(&m_selectedObjs);
    QList<CMDDrawObj*> combineL;
    for(int i = 0; i < indexA.count(); i++)
        combineL.append(objs()->at(indexA.at(i)));
    CMDDrawCombine* combine = new CMDDrawCombine(&combineL);
    combine->initCreate();
    dynamic_cast<CObjName*>(nameManager())->createName(combine, false);
    combine->setLayer(defaultLayer());


    QList<CMDDrawObj*> combines;
    combines.append(combine);
    QList<int> indexs;
    indexs.append(indexA.at(0));
    m_undo.push(new CObjectChangedCommand(&combines, &indexs, this, (int)ObjectChangedId::Combine));

    m_selectedObjs.clear();
    m_selectedObjs.append(combine);

    m_controlPoint->changeSelectObj(&m_selectedObjs);
    window()->update();

    objectChangedEvent()->combine(combine);
}
void CStudio::unCombine()
{
    QList<CMDDrawObj*> list;
    QList<int> indexs;
    for(int i = 0; i < m_selectedObjs.count(); i++)
    {
        CMDDrawObj* obj = m_selectedObjs.at(i);
        if (obj->type() != DrawType::Combine)
            continue;
        list.append(obj);
    }

    if(list.isEmpty())
        return;

    indexs = objectIndexs(&list);
    list.clear();
    for(int i = 0; i < indexs.count(); i++)
        list.append(objs()->at(indexs.at(i)));
    m_undo.push(new CObjectChangedCommand(&list, &indexs, this, (int)ObjectChangedId::UnCombine));

    CMDDrawCombine* combine = dynamic_cast<CMDDrawCombine*>(list.at(0));
    //去除锁定的控件
    int count = combine->objList()->count();
    for (int i = count - 1; i >= 0; i--)
    {
        if (!combine->objList()->at(i)->canSelect())
            combine->objList()->removeAt(i);
    }

    m_selectedObjs.clear();
    m_selectedObjs.append(*combine->objList());

    m_controlPoint->changeSelectObj(&m_selectedObjs);
    window()->update();

    //通知对象对话框
    for (int i = 0; i < list.count(); i++)
        objectChangedEvent()->unCombine(list.at(i));
}
//////////////////////////////////////////////////////////////////////////
void CStudio::serialize(QJsonObject &json)
{
    CEnvironment::serialize(json);
}
void CStudio::deserialize(const QJsonObject &json)
{
    CEnvironment::deserialize(json);
}
//////////////////////////////////////////////////////////////////////////
void CStudio::refreshProperty()
{
    container()->framework()->manager()->refreshProperty();
    statusLabelChanged();
}
void CStudio::selectObject(QObject *obj)
{
    container()->framework()->manager()->selectObject(obj);
    if (obj != NULL && obj->inherits("CPropertyDrawObj"))
        dynamic_cast<CMDPropertyDrawObj*>(obj)->resetAttribute();

    statusLabelChanged();
    selectedObjChanged();
}
void CStudio::resetSelectObjs(bool isNullObj/* = false*/)
{
    if (isNullObj)
    {
        selectObject(NULL);
        return;
    }

    CSelectObjectManager* selectObjs = m_controlPoint->selectObjs();
    objectChangedEvent()->select(selectObjs->list());

    if (selectObjs->isEmpty()) //form
        selectObject(window());
    else if (selectObjs->isSingle())
        selectPropetyObject(selectObjs->list()->at(0));
    else
        selectObject(selectObjs->propertyDrawObj());
}
//////////////////////////////////////////////////////////////////////////
void CStudio::clearCopyList()
{
    for(int i = 0; i < m_copyList.count(); i++)
    {
        CMDDrawObj* obj = m_copyList.at(i);
        delete obj;
    }
    m_copyList.clear();
    m_copyImagePath = "";
}
void CStudio::copy()
{
    clearCopyList();
    for(int i = 0; i < m_selectedObjs.count(); i++)
    {
        CMDDrawObj* obj = m_selectedObjs.at(i);
        m_copyList.append(obj->clone());
    }

    if(m_selectedObjs.count() > 0)
        m_copyImagePath = window()->framework()->projectImagePath();
}
void CStudio::paste()
{
    bool needCopyImage = false;
    QString currentImagePath = window()->framework()->projectImagePath();
    if(m_copyImagePath.compare(currentImagePath, Qt::CaseInsensitive) != 0)
        needCopyImage = true;

    QList<CMDDrawObj*> list;
    QList<int> indexs;
    int count = objs()->count();
    for(int i = 0; i < m_copyList.count(); i++)
    {
        CMDDrawObj* obj = m_copyList.at(i);

        if(needCopyImage)
            obj->copyImage(m_copyImagePath, currentImagePath, false);
        obj->setFactRect(QRectF(obj->rect().x()+10,obj->rect().y()+10,obj->rect().width(),obj->rect().height()));
        list.append(cloneDrawObj(obj));

        indexs.append(count++);
    }

    m_undo.push(new CObjectChangedCommand(&list, &indexs, this, (int)ObjectChangedId::Add));

    objectChangedEvent()->add(&list);
}
void CStudio::deleteObj()
{
    for (int i = 0; i < m_selectedObjs.count(); i++)
    {
        m_selectedObjs.at(i)->invalidate();
        dynamic_cast<CObjName*>(nameManager())->removeName(m_selectedObjs.at(i));
    }
    QList<int> indexs = objectIndexs(&m_selectedObjs);

    m_undo.push(new CObjectChangedCommand(&m_selectedObjs, &indexs, this, (int)ObjectChangedId::Delete));

    m_controlPoint->changeSelectObj(NULL);
    objectChangedEvent()->dele(&m_selectedObjs);
    m_selectedObjs.clear();
}
void CStudio::toLeft()
{
    CSelectObjectManager* selectObjs = m_controlPoint->selectObjs();
    QRectF rf = selectObjs->rect();
    rf.moveLeft(rf.left()-1);
    m_undo.push(
        new CSelectObjectCommand(selectObjs, selectObjs->rect(), rf, (int)SelectObjectId::Rect));
}
void CStudio::toRight()
{
    CSelectObjectManager* selectObjs = m_controlPoint->selectObjs();
    QRectF rf = selectObjs->rect();
    rf.moveLeft(rf.left()+1);
    m_undo.push(
        new CSelectObjectCommand(selectObjs, selectObjs->rect(), rf, (int)SelectObjectId::Rect));
}
void CStudio::toTop()
{
    CSelectObjectManager* selectObjs = m_controlPoint->selectObjs();
    QRectF rf = selectObjs->rect();
    rf.moveTop(rf.top()-1);
    m_undo.push(
        new CSelectObjectCommand(selectObjs, selectObjs->rect(), rf, (int)SelectObjectId::Rect));
}
void CStudio::toBottom()
{
    CSelectObjectManager* selectObjs = m_controlPoint->selectObjs();
    QRectF rf = selectObjs->rect();
    rf.moveTop(rf.top()+1);
    m_undo.push(
        new CSelectObjectCommand(selectObjs, selectObjs->rect(), rf, (int)SelectObjectId::Rect));
}
void CStudio::increaseWidth()
{
    CSelectObjectManager* selectObjs = m_controlPoint->selectObjs();
    QRectF rf = selectObjs->rect();
    qreal width = rf.width() + 1;
    rf.setWidth(width);
    m_undo.push(
        new CSelectObjectCommand(selectObjs, selectObjs->rect(), rf, (int)SelectObjectId::Rect));
}
void CStudio::decreaseWidth()
{
    CSelectObjectManager* selectObjs = m_controlPoint->selectObjs();
    QRectF rf = selectObjs->rect();
    qreal width = rf.width() - 1;
    width = (width > 1) ? width : 1;
    rf.setWidth(width);
    m_undo.push(
        new CSelectObjectCommand(selectObjs, selectObjs->rect(), rf, (int)SelectObjectId::Rect));
}
void CStudio::increaseHeight()
{
    CSelectObjectManager* selectObjs = m_controlPoint->selectObjs();
    QRectF rf = selectObjs->rect();
    qreal height = rf.height() + 1;
    rf.setHeight(height);
    m_undo.push(
        new CSelectObjectCommand(selectObjs, selectObjs->rect(), rf, (int)SelectObjectId::Rect));
}
void CStudio::decreaseHeight()
{
    CSelectObjectManager* selectObjs = m_controlPoint->selectObjs();
    QRectF rf = selectObjs->rect();
    qreal height = rf.height() - 1;
    height = (height > 1) ? height : 1;
    rf.setHeight(height);
    m_undo.push(
        new CSelectObjectCommand(selectObjs, selectObjs->rect(), rf, (int)SelectObjectId::Rect));
}
CMDDrawObj *CStudio::cloneDrawObj(CMDDrawObj *orig)
{
    CMDDrawObj* newObj = orig->clone();
    newObj->setStorageImagePath("");
    newObj->setParent(container());
    dynamic_cast<CObjName*>(nameManager())->createName(newObj);
    newObj->loadInitalizationEvent();
    newObj->changeLanguage();
    newObj->invalidate();

    return newObj;
}
//////////////////////////////////////////////////////////////////////////
void CStudio::ortho()
{
    m_isOrtho = !m_isOrtho;
}
void CStudio::grid()
{
    m_isGrid = !m_isGrid;
}
void CStudio::scale(const QString& text)
{
    QString s = text;
    s = s.remove(s.length() - 1, 1);
    qreal v = s.toInt() / 100.f;
    setFormScale(QPointF(v, v));
}
void CStudio::layer()
{
    QList<QVariant> oldValue;
    QVector<bool>* oldVs = new QVector<bool>(10);
    *oldVs = *visibleLayers();
    QVector<bool>* oldLs = new QVector<bool>(10);
    *oldLs = *lockedLayers();
    oldValue.append((qulonglong)oldVs);
    oldValue.append((qulonglong)oldLs);

    CLayerConfigDialog dlg(window());
    if (dlg.exec() == QDialog::Accepted)
    {
        QList<QVariant> newValue;
        QVector<bool>* newVs = new QVector<bool>(10);
        *newVs = *visibleLayers();
        QVector<bool>* newLs = new QVector<bool>(10);
        *newLs = *lockedLayers();
        newValue.append((qulonglong)newVs);
        newValue.append((qulonglong)newLs);

        m_undo.push(new CFormCommand(dynamic_cast<CMDHMIForm*>(container()),
            oldValue, newValue, (int)FormId::Layer));

        m_controlPoint->changeSelectObj(NULL);
        window()->update();
        container()->setModify(true);
    }
    else
    {
        delete oldVs;
        delete oldLs;
    }
}
//////////////////////////////////////////////////////////////////////////
qreal CStudio::getSortValue(CMDDrawObj *obj, LayoutType type)
{
    switch (type)
    {
        case LayoutType::Left:
            return obj->bound().left();
        case LayoutType::Top:
            return obj->bound().top();
        default:
            break;
    }

    return 0;
}
void CStudio::sortList(QList<CMDDrawObj *>* sorts, CStudio::LayoutType type)
{
    int count = sorts->count();
    for (int i = 0; i < count; i++)
    {
        for (int j = i+1; j < count; j++)
        {
            if (getSortValue(sorts->at(j), type) < getSortValue(sorts->at(i), type))
            {
                CMDDrawObj* replace = sorts->at(j);
                (*sorts)[j] = sorts->at(i);
                (*sorts)[i] = replace;
            }
        }
    }
}
void CStudio::incrSpace(CStudio::LayoutType type)
{
    CMDDrawObj* last = m_controlPoint->selectObjs()->lastSelectedObj();
    int count = m_selectedObjs.count();

    QList<CMDDrawObj*> sorts(m_selectedObjs);
    sortList(&sorts, type);

    QList<QVariant> olds = objectUndoDatas(&m_selectedObjs);

    int lastIndex = sorts.indexOf(last);
    for (int i = 0; i < count; i++)
    {
        CMDDrawObj* obj = sorts.at(i);
        int index = i - lastIndex;
        QRectF rf = obj->rect();
        if (type == LayoutType::Left)
            rf.moveLeft(rf.left() + index * MoveSize);
        else if (type == LayoutType::Top)
            rf.moveTop(rf.top() + index * MoveSize);
        obj->setRect(rf);
    }

    m_controlPoint->calculateAndInvalidate();

    QList<QVariant> news = objectUndoDatas(&m_selectedObjs);;
    m_undo.push(new CMDMouseOperateCommand(olds, news, (int)MouseOperateId::Mouse));
}
void CStudio::alignLeft()
{
    QList<QVariant> olds = objectUndoDatas(&m_selectedObjs);

    CMDDrawObj* last = m_controlPoint->selectObjs()->lastSelectedObj();
    for(int i = 0; i < m_selectedObjs.count(); i++)
    {
        CMDDrawObj* obj = m_selectedObjs.at(i);

        if (obj == last)
            continue;

        QRectF rf = obj->rect();
        rf.moveLeft(last->bound().left());
        obj->setRect(rf);
    }
    m_controlPoint->calculateAndInvalidate();

    QList<QVariant> news = objectUndoDatas(&m_selectedObjs);;
    m_undo.push(new CMDMouseOperateCommand(olds, news, (int)MouseOperateId::Mouse));
}
void CStudio::alignCenter()
{
    QList<QVariant> olds = objectUndoDatas(&m_selectedObjs);

    CMDDrawObj* last = m_controlPoint->selectObjs()->lastSelectedObj();
    qreal f = last->bound().left() + last->bound().width() / 2;
    for(int i = 0; i < m_selectedObjs.count(); i++)
    {
        CMDDrawObj* obj = m_selectedObjs.at(i);

        if (obj == last)
            continue;

        QRectF rf = obj->rect();
        rf.moveLeft(f - obj->bound().width() / 2);
        obj->setRect(rf);
    }
    m_controlPoint->calculateAndInvalidate();

    QList<QVariant> news = objectUndoDatas(&m_selectedObjs);;
    m_undo.push(new CMDMouseOperateCommand(olds, news, (int)MouseOperateId::Mouse));
}
void CStudio::alignRight()
{
    QList<QVariant> olds = objectUndoDatas(&m_selectedObjs);

    CMDDrawObj* last = m_controlPoint->selectObjs()->lastSelectedObj();
    for(int i = 0; i < m_selectedObjs.count(); i++)
    {
        CMDDrawObj* obj = m_selectedObjs.at(i);

        if (obj == last)
            continue;

        QRectF rf = obj->rect();
        rf.moveLeft(last->bound().right() - obj->bound().width());
        obj->setRect(rf);
    }
    m_controlPoint->calculateAndInvalidate();

    QList<QVariant> news = objectUndoDatas(&m_selectedObjs);;
    m_undo.push(new CMDMouseOperateCommand(olds, news, (int)MouseOperateId::Mouse));
}
void CStudio::alignTop()
{
    QList<QVariant> olds = objectUndoDatas(&m_selectedObjs);

    CMDDrawObj* last = m_controlPoint->selectObjs()->lastSelectedObj();
    for(int i = 0; i < m_selectedObjs.count(); i++)
    {
        CMDDrawObj* obj = m_selectedObjs.at(i);

        if (obj == last)
            continue;

        QRectF rf = obj->rect();
        rf.moveTop(last->bound().top());
        obj->setRect(rf);
    }
    m_controlPoint->calculateAndInvalidate();

    QList<QVariant> news = objectUndoDatas(&m_selectedObjs);
    m_undo.push(new CMDMouseOperateCommand(olds, news, (int)MouseOperateId::Mouse));
}
void CStudio::alignMiddle()
{
    QList<QVariant> olds = objectUndoDatas(&m_selectedObjs);

    CMDDrawObj* last = m_controlPoint->selectObjs()->lastSelectedObj();
    qreal f = last->bound().top() + last->bound().height() / 2;
    for(int i = 0; i < m_selectedObjs.count(); i++)
    {
        CMDDrawObj* obj = m_selectedObjs.at(i);

        if (obj == last)
            continue;

        QRectF rf = obj->rect();
        rf.moveTop(f - obj->bound().height() / 2);
        obj->setRect(rf);
    }
    m_controlPoint->calculateAndInvalidate();

    QList<QVariant> news = objectUndoDatas(&m_selectedObjs);;
    m_undo.push(new CMDMouseOperateCommand(olds, news, (int)MouseOperateId::Mouse));
}

void CStudio::alignBottom()
{
    QList<QVariant> olds = objectUndoDatas(&m_selectedObjs);

    CMDDrawObj* last = m_controlPoint->selectObjs()->lastSelectedObj();
    for(int i = 0; i < m_selectedObjs.count(); i++)
    {
        CMDDrawObj* obj = m_selectedObjs.at(i);

        if (obj == last)
            continue;

        QRectF rf = obj->rect();
        rf.moveTop(last->bound().bottom() - obj->bound().height());
        obj->setRect(rf);
    }
    m_controlPoint->calculateAndInvalidate();

    QList<QVariant> news = objectUndoDatas(&m_selectedObjs);;
    m_undo.push(new CMDMouseOperateCommand(olds, news, (int)MouseOperateId::Mouse));
}
void CStudio::sameWidth()
{
    QList<QVariant> olds = objectUndoDatas(&m_selectedObjs);

    CMDDrawObj* last = m_controlPoint->selectObjs()->lastSelectedObj();
    qreal f = last->bound().width();
    for(int i = 0; i < m_selectedObjs.count(); i++)
    {
        CMDDrawObj* obj = m_selectedObjs.at(i);

        if (obj == last)
            continue;

        QRectF rf = obj->rect();
        rf.setWidth(f);
        obj->setRect(rf);
    }
    m_controlPoint->calculateAndInvalidate();

    QList<QVariant> news = objectUndoDatas(&m_selectedObjs);;
    m_undo.push(new CMDMouseOperateCommand(olds, news, (int)MouseOperateId::Mouse));
}
void CStudio::sameHeight()
{
    QList<QVariant> olds = objectUndoDatas(&m_selectedObjs);

    CMDDrawObj* last = m_controlPoint->selectObjs()->lastSelectedObj();
    qreal f = last->bound().height();
    for(int i = 0; i < m_selectedObjs.count(); i++)
    {
        CMDDrawObj* obj = m_selectedObjs.at(i);

        if (obj == last)
            continue;

        QRectF rf = obj->rect();
        rf.setHeight(f);
        obj->setRect(rf);
    }
    m_controlPoint->calculateAndInvalidate();

    QList<QVariant> news = objectUndoDatas(&m_selectedObjs);;
    m_undo.push(new CMDMouseOperateCommand(olds, news, (int)MouseOperateId::Mouse));
}
void CStudio::sameSize()
{
    QList<QVariant> olds = objectUndoDatas(&m_selectedObjs);

    CMDDrawObj* last = m_controlPoint->selectObjs()->lastSelectedObj();
    for(int i = 0; i < m_selectedObjs.count(); i++)
    {
        CMDDrawObj* obj = m_selectedObjs.at(i);

        if (obj == last)
            continue;

        QRectF rf = obj->rect();
        rf.setSize(last->bound().size());
        obj->setRect(rf);
    }
    m_controlPoint->calculateAndInvalidate();

    QList<QVariant> news = objectUndoDatas(&m_selectedObjs);;
    m_undo.push(new CMDMouseOperateCommand(olds, news, (int)MouseOperateId::Mouse));
}
void CStudio::hSpace()
{
    int count = m_selectedObjs.count();
    qreal left = m_selectedObjs.at(0)->bound().left();
    qreal right = m_selectedObjs.at(0)->bound().right();
    qreal width = m_selectedObjs.at(0)->bound().width();
    for (int i = 1; i < count; i++)
    {
        QRectF rf = m_selectedObjs.at(i)->bound();
        if (left > rf.left())
            left = rf.left();
        if (right < rf.right())
            right = rf.right();
        width += rf.width();
    }
    qreal finalWdith = width > (right - left) ? width : right - left;
    qreal interval = (finalWdith - width) / (count - 1);
    QList<CMDDrawObj*> sorts(m_selectedObjs);
    sortList(&sorts, LayoutType::Left);

    QList<QVariant> olds = objectUndoDatas(&m_selectedObjs);

    qreal current = left;
    for (int i = 0; i < count; i++)
    {
        CMDDrawObj* obj = sorts.at(i);
        QString name = obj->name();


        QRectF rf = obj->rect();
        rf.moveLeft(rf.left() + current - obj->bound().x());
        obj->setRect(rf);
        current += obj->bound().width() + interval;
    }

    m_controlPoint->calculateAndInvalidate();

    QList<QVariant> news = objectUndoDatas(&m_selectedObjs);;
    m_undo.push(new CMDMouseOperateCommand(olds, news, (int)MouseOperateId::Mouse));
}
void CStudio::incrHSpace()
{
    incrSpace(LayoutType::Left);
}
void CStudio::decrHSpace()
{
    CMDDrawObj* last = m_controlPoint->selectObjs()->lastSelectedObj();
    int count = m_selectedObjs.count();
    qreal standard = last->bound().left();

    QList<CMDDrawObj*> sorts(m_selectedObjs);
    sortList(&sorts, LayoutType::Left);

    QList<QVariant> olds = objectUndoDatas(&m_selectedObjs);

    int lastIndex = sorts.indexOf(last);
    for (int i = 0; i < count; i++)
    {
        CMDDrawObj* obj = sorts.at(i);
        int index = i - lastIndex;
        QRectF rf = obj->rect();
        if (qAbs(standard - rf.x()) > qAbs(index * MoveSize))
            rf.moveLeft(rf.left() - index * MoveSize);
        else
            rf.moveLeft(standard);
        obj->setRect(rf);
    }
    m_controlPoint->calculateAndInvalidate();

    QList<QVariant> news = objectUndoDatas(&m_selectedObjs);;
    m_undo.push(new CMDMouseOperateCommand(olds, news, (int)MouseOperateId::Mouse));
}
void CStudio::vSpace()
{
    int count = m_selectedObjs.count();
    qreal top = m_selectedObjs.at(0)->bound().top();
    qreal bottom = m_selectedObjs.at(0)->bound().bottom();
    qreal height = m_selectedObjs.at(0)->bound().height();
    for (int i = 1; i < count; i++)
    {
        QRectF rf = m_selectedObjs.at(i)->bound();
        if (top > rf.top())
            top = rf.top();
        if (bottom < rf.bottom())
            bottom = rf.bottom();
        height += rf.height();
    }
    qreal finalHeight = height > (bottom - top) ? height : bottom - top;
    qreal interval = (finalHeight - height) / (count - 1);
    QList<CMDDrawObj*> sorts(m_selectedObjs);
    sortList(&sorts, LayoutType::Top);

    QList<QVariant> olds = objectUndoDatas(&m_selectedObjs);

    qreal current = top;
    for (int i = 0; i < count; i++)
    {
        CMDDrawObj* obj = sorts.at(i);
        QRectF rf = obj->rect();
        rf.moveTop(rf.top() + current - obj->bound().y());
        obj->setRect(rf);
        current += obj->bound().height() + interval;
    }

    m_controlPoint->calculateAndInvalidate();

    QList<QVariant> news = objectUndoDatas(&m_selectedObjs);;
    m_undo.push(new CMDMouseOperateCommand(olds, news, (int)MouseOperateId::Mouse));
}
void CStudio::incrVSpace()
{
    incrSpace(LayoutType::Top);
}
void CStudio::decrVSpace()
{
    CMDDrawObj* last = m_controlPoint->selectObjs()->lastSelectedObj();
    int count = m_selectedObjs.count();
    qreal standard = last->bound().top();

    QList<CMDDrawObj*> sorts(m_selectedObjs);
    sortList(&sorts, LayoutType::Top);

    QList<QVariant> olds = objectUndoDatas(&m_selectedObjs);

    int lastIndex = sorts.indexOf(last);
    for (int i = 0; i < count; i++)
    {
        CMDDrawObj* obj = sorts.at(i);
        int index = i - lastIndex;
        QRectF rf = obj->rect();
        if (qAbs(standard - rf.y()) > qAbs(index * MoveSize))
            rf.moveTop(rf.top() - index * MoveSize);
        else
            rf.moveTop(standard);
        obj->setRect(rf);
    }
    m_controlPoint->calculateAndInvalidate();

    QList<QVariant> news = objectUndoDatas(&m_selectedObjs);;
    m_undo.push(new CMDMouseOperateCommand(olds, news, (int)MouseOperateId::Mouse));
}
//////////////////////////////////////////////////////////////////////////
void CStudio::sequenceFun(CStudio::SequenceType type)
{
    CMDDrawObj* obj = m_selectedObjs.at(0);
    int count = objs()->count();
    int i = objs()->indexOf(obj);
    QList<int> indexs;

    switch(type)
    {
    case SequenceType::Top:
        indexs.append(i);
        indexs.append(count - 1);
        if (indexs.at(0) == (count - 1))
            return;
        break;
    case SequenceType::Last:
        indexs.append(i);
        indexs.append(0);
        if (indexs.at(0) == 0)
            return;
        break;
    case SequenceType::Front:
        indexs.append(i);
        indexs.append(i+1);
        if (indexs.at(0) == (count - 1))
            return;
        break;
    case SequenceType::Back:
        indexs.append(i);
        indexs.append(i-1);
        if (indexs.at(0) == 0)
            return;
        break;
    default:
        break;
    }

    m_undo.push(new CObjectChangedCommand(&m_selectedObjs, &indexs, this, (int)ObjectChangedId::Index));

    obj->invalidate();
    objectChangedEvent()->back(obj);
    container()->setModify(true);

    switch(type)
    {
    case SequenceType::Top:
        objectChangedEvent()->top(obj);
        break;
    case SequenceType::Last:
        objectChangedEvent()->last(obj);
        break;
    case SequenceType::Front:
        objectChangedEvent()->front(obj);
        break;
    case SequenceType::Back:
        objectChangedEvent()->back(obj);
        break;
    default:
        break;
    }
}
void CStudio::flipX()
{
    QList<QVariant> olds = objectUndoDatas(m_controlPoint->selectObjs()->list());

    m_controlPoint->selectObjs()->flipX();

    QList<QVariant> news = objectUndoDatas(m_controlPoint->selectObjs()->list());
    m_undo.push(new CMDMouseOperateCommand(olds, news, (int)MouseOperateId::Mouse));

    refreshProperty();
}
void CStudio::flipY()
{
    QList<QVariant> olds = objectUndoDatas(m_controlPoint->selectObjs()->list());

    m_controlPoint->selectObjs()->flipY();

    QList<QVariant> news = objectUndoDatas(m_controlPoint->selectObjs()->list());
    m_undo.push(new CMDMouseOperateCommand(olds, news, (int)MouseOperateId::Mouse));

    refreshProperty();
}
//////////////////////////////////////////////////////////////////////////
void CStudio::setCreateObjectState(DrawType type)
{
    m_create->end();
    m_create->begin(type);
    container()->setModify(true);
}
//////////////////////////////////////////////////////////////////////////
CMDDrawObj *CStudio::studioCreateDrawObj(DrawType type, const QRectF &rf)
{
    QRectF rect = rf;
    const int limit = 5;
    const int v = 100;
    if (rf.width() < limit || rf.height() < limit)
    {
        rect.setWidth(v);
        rect.setHeight(v);
    }

    CMDDrawObj* obj = CDrawCreator::createDrawObj((DrawType)type);
    if (obj == NULL)
        return NULL;

    QList<CMDDrawObj*> list;
    list.append(obj);
    QList<int> indexs;
    indexs.append(objs()->count());
    m_undo.push(new CObjectChangedCommand(&list, &indexs, this, (int)ObjectChangedId::Add));

    obj->setParent(container());
    dynamic_cast<CObjName*>(nameManager())->createName(obj);
    obj->setRect(rect);
    obj->loadInitalizationEvent();
    obj->changeLanguage();
    obj->setLayer(defaultLayer());
    obj->invalidate();

    objectChangedEvent()->add(&list);

    return obj;
}
void CStudio::statusLabelChanged()
{
    QPoint point;
    QSize size;
    CSelectObjectManager* objs = m_controlPoint->selectObjs();

    if (objs->isEmpty()) //form
    {
        point = window()->rect().topLeft();
        size = window()->rect().size();
    }
    else//drawobj
    {
        point = objs->rect().topLeft().toPoint();
        size = objs->rect().size().toSize();
    }

    QString strX;
    strX.sprintf("%6d", point.x());
    QString strY;
    strY.sprintf("%6d", point.y());
    QString strLocation = strX + "," + strY;
    strX.sprintf("%6d", size.width());
    strY.sprintf("%6d", size.height());
    QString strSize = strX + "x" + strY;
    window()->setStatusBarText(strLocation, strSize);
}
void CStudio::selectedObjChanged()
{
    window()->setToolBarEnabled();
}
void CStudio::itemSelectObject(QList<CMDDrawObj *> *objs)
{
    m_selectedObjs.clear();
    m_selectedObjs.append(*objs);
    m_controlPoint->changeSelectObj(&m_selectedObjs);
}
void CStudio::selectPropetyObject(CMDDrawObj *obj)
{
    selectObject(obj->propertyDrawObj());
}
void CStudio::setLastSelectedObject(CMDDrawObj *obj)
{
    controlPoint()->selectObjs()->setLastSelectedObj(obj);
}
void CStudio::addToObjectTree(QList<CMDDrawObj *> *objs)
{
    objectChangedEvent()->add(objs);
}
//////////////////////////////////////////////////////////////////////////
IObjectChangedEvent *CStudio::objectChangedEvent()
{
    return container()->framework()->manager()->objectChangedEvent();
}
void CStudio::selectAll()
{
    m_selectedObjs.clear();
    m_selectedObjs.append(*objs());
    m_controlPoint->changeSelectObj(&m_selectedObjs);
}
//////////////////////////////////////////////////////////////////////////
void CStudio::pasteObject(CMDDrawObj *obj)
{
    //copy image
    QString storagePath = window()->framework()->storageImagePath();
    QString projectPath = window()->framework()->projectImagePath();
    obj->copyImage(storagePath, projectPath, false);

    QList<CMDDrawObj*> list;
    obj->setFactRect(QRectF(obj->rect().x()+10,obj->rect().y()+10,obj->rect().width(),obj->rect().height()));
    list.append(cloneDrawObj(obj));
    QList<int> indexs;
    indexs.append(objs()->count());

    m_undo.push(new CObjectChangedCommand(&list, &indexs, this, (int)ObjectChangedId::Add));

    objectChangedEvent()->add(&list);
    container()->setModify(true);
}
//////////////////////////////////////////////////////////////////////////
void CStudio::saveToStorage()
{
    QList<CMDDrawObj*> objs;
    for(int i = 0; i < m_selectedObjs.count(); i++)
        objs.append(cloneDrawObj(m_selectedObjs.at(i)));

    if (objs.count() > 0)
        container()->framework()->manager()->graphicsStorage()->saveObjDialog(&objs);

    qDeleteAll(objs.begin(), objs.end());
}
void CStudio::saveToStoragePath()
{
    QList<CMDDrawObj*> objs;
    for(int i = 0; i < m_selectedObjs.count(); i++)
        objs.append(cloneDrawObj(m_selectedObjs.at(i)));

    if (objs.count() > 0)
        container()->framework()->manager()->graphicsStorage()->saveObjToPath(&objs);

    qDeleteAll(objs.begin(), objs.end());
}
void CStudio::storage()
{
    container()->framework()->manager()->graphicsStorage()->showDialog(container());
}
//////////////////////////////////////////////////////////////////////////
void CStudio::undoRefresh()
{
    m_selectedObjs.clear();
    m_controlPoint->changeSelectObj(&m_selectedObjs);

    objectChangedEvent()->reset();
    dynamic_cast<CObjName*>(nameManager())->resetDict(objs());

    refreshProperty();
    window()->update();
    container()->setModify(true);
}
void CStudio::undoAdd(QList<CMDDrawObj *> *addObjs, QList<int>* indexs)
{
    int count = addObjs->count();
    for (int i = 0; i < count; i++)
    {
        CMDDrawObj* o = addObjs->at(i);
        objs()->insert(indexs->at(i), addObjs->at(i));
        m_deletedObjs.removeOne(o);
        o->undoAdd();
    }
}
void CStudio::undoDelete(QList<CMDDrawObj *> *delObjs, QList<int> *indexs)
{
    for(int i = 0; i < delObjs->count(); i++)
    {
        CMDDrawObj* o = delObjs->at(i);
        objs()->removeOne(o);
        o->undoDelete();
    }

    m_deletedObjs.append(*delObjs);
}
void CStudio::undoGroup(QList<CMDDrawObj *> *groupObjs, QList<int> *indexs)
{
    int count = groupObjs->count();
    for (int i = 0; i < count; i++)
    {
        CMDDrawGroup* g = dynamic_cast<CMDDrawGroup*>(groupObjs->at(i));
        g->loadInitalizationEvent();

        for(int j = 0; j < g->objList()->count(); j++)
            objs()->removeOne(g->objList()->at(j));

        objs()->insert(indexs->at(i), g);

        m_deletedGroups.removeOne(g);
    }
}
void CStudio::undoUnGroup(QList<CMDDrawObj *> *unGroupObjs, QList<int> *indexs)
{
    int count = unGroupObjs->count();
    for (int i = count - 1; i >= 0; i--)
    {
        CMDDrawGroup* g = dynamic_cast<CMDDrawGroup*>(unGroupObjs->at(i));
        int index = indexs->at(i);

        g->unMulti();
        objs()->removeOne(g);
        for(int j = 0; j < g->objList()->count(); j++)
            objs()->insert(index++, g->objList()->at(j));
    }
    m_deletedGroups.append(*unGroupObjs);
}
void CStudio::undoCombine(QList<CMDDrawObj *> *combineObjs, QList<int> *indexs)
{
    int count = combineObjs->count();
    for (int i = 0; i < count; i++)
    {
        CMDDrawCombine* o = dynamic_cast<CMDDrawCombine*>(combineObjs->at(i));
        o->loadInitalizationEvent();

        for(int j = 0; j < o->objList()->count(); j++)
            objs()->removeOne(o->objList()->at(j));

        objs()->insert(indexs->at(i), o);
        m_deletedCombines.removeOne(o);
    }
}
void CStudio::undoUnCombine(QList<CMDDrawObj *> *unCombineObjs, QList<int> *indexs)
{
    int count = unCombineObjs->count();
    for (int i = count - 1; i >= 0; i--)
    {
        CMDDrawCombine* o = dynamic_cast<CMDDrawCombine*>(unCombineObjs->at(i));
        int index = indexs->at(i);

        o->unMulti();
        objs()->removeOne(o);
        for(int j = 0; j < o->objList()->count(); j++)
            objs()->insert(index++, o->objList()->at(j));
    }
    m_deletedCombines.append(*unCombineObjs);
}
void CStudio::undoLayer(QVector<bool> *visibles, QVector<bool> *lockeds)
{
    *visibleLayers() = *visibles;
    *lockedLayers() = *lockeds;

    for(int i = 0; i < objs()->count(); i++)
        objs()->at(i)->updateVisible();
}
void CStudio::undoBrush(CMDBrushManager *value)
{
    *brush() = *value;
    generateContent();
}
void CStudio::undoIndex(CMDDrawObj* obj, int newIndex)
{
    objs()->removeOne(obj);
    objs()->insert(newIndex, obj);

    if (newIndex == objs()->count() - 1)
        obj->top();
    else if (newIndex == 0)
        obj->last();
}
void CStudio::undoAction(int id, CMDHMIActionManager *action)
{
    CMDHMIActionManager* manager = NULL;
    switch (id)
    {
    case (int)FormId::MouseDownAction:
        manager = mouseDownActions();
        break;
    case (int)FormId::MouseUpAction:
        manager = mouseUpActions();
        break;
    case (int)FormId::RightMosueDownAction:
        manager = rightMouseDownActions();
        break;
    case (int)FormId::RightMouseUpAction:
        manager = rightMouseUpActions();
        break;
    case (int)FormId::OpenAction:
        manager = openActions();
        break;
    case (int)FormId::CloseAction:
        manager = closeActions();
        break;
    default:
        break;
    }

    QList<QString>* list = manager->list();
    list->clear();
    list->append(*action->list());
}
void CStudio::undoText(const QString &value)
{
    setText(value);
}
void CStudio::undoFormType(int type, bool hasFrame, bool fullScreen)
{
    setFormType((FormType)type);
    setHasFrame(hasFrame);
    setFullScreen(fullScreen);
}
void CStudio::undoSecurity(CMDActionSP *open, CMDActionSP *close)
{
    setOpenSecurity(open);
    setCloseSecurity(close);
}
/////////////////////////////////////////////////////////////////////
QList<int> CStudio::objectIndexs(QList<CMDDrawObj *>* objects)
{
    QList<int> indexS;
    for(int i = 0; i < objects->count(); i++)
        indexS.append(objs()->indexOf(objects->at(i)));
    qSort(indexS.begin(), indexS.end());
    return indexS;
}
QList<QVariant> CStudio::objectUndoDatas(QList<CMDDrawObj *> *objects)
{
    QList<QVariant> values;
    for(int i = 0; i < objects->count(); i++)
        values.append((qulonglong)objects->at(i)->fillUndoData(false));
    return values;
}
//////////////////////////////////////////////////////////////////////////
void CStudio::collectWords()
{
    CMDMultiLanguage* l = CMDMultiLanguage::instance();
    l->addWord(text(), false);
    l->addWord(m_printLeftHeader, false);
    l->addWord(m_printMiddleHeader, false);
    l->addWord(m_printRightHeader, false);
    l->addWord(m_printLeftFooter, false);
    l->addWord(m_printMiddleFooter, false);
    l->addWord(m_printRightFooter, false);
    l->addWord(m_printDateFormat, false);
    l->addWord(m_printTimeFormat, false);

    int count = objs()->count();
    for(int i = 0; i < count; i++)
        objs()->at(i)->collectWords();
}
//////////////////////////////////////////////////////////////////////////
void CStudio::drawPreview(QPainter *p)
{
    drawBackground(p);

    for(int i = 0; i < objs()->count(); i++)
    {
        CMDDrawObj* obj = objs()->at(i);
        if (obj->finalVisible())
            obj->draw(p, true);
    }
}
void CStudio::formLayout()
{
    container()->framework()->manager()->initFormPreview(container()->framework());
    container()->framework()->manager()->formPreviewDialog()->exec();
}
//////////////////////////////////////////////////////////////////////////
void CStudio::batchEdit()
{
    QList<CMDDrawObj*>* objs = &m_selectedObjs;
    if(objs->count() < 2)
        return;

    QString name = basicClassName(objs);
    CMDPropertyDrawObj* property = objs->at(0)->createPropertyDrawObj(name);
    property->appendBatchObjs(objs);
    CPropertyBrowser* browser = new CPropertyBrowser;
    browser->setObject(property);
    property->resetBatchProperty(browser);
    browser->setExpandAll();
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setMargin(1);
    layout->addWidget(browser);
    QDialog dlg;
    dlg.setLayout(layout);
    dlg.setWindowTitle(QObject::tr("批量编辑"));
    dlg.setFixedSize(340, 650);
    dlg.exec();

    delete property;
}
QString CStudio::basicClassName(QList<CMDDrawObj *> *objs)
{
    int level = 100;
    foreach(CMDDrawObj* o, *objs)
    {
        int count = o->classInfo()->count();
        level = qMin(level, count);
    }

    while(level >= 1)
    {
        int count = objs->count() - 1;
        for(int i = 0; i < count; i++)
        {
            QString s1 = objs->at(i)->classInfo()->at(level - 1);
            QString s2 = objs->at(i + 1)->classInfo()->at(level - 1);

            if(QString::compare(s1, s2, Qt::CaseInsensitive) != 0)
            {
                level--;
                break;
            }

            if(i == (count - 1))
                return objs->at(0)->classInfo()->at(level - 1);
        }
    }

    return "";
}
//////////////////////////////////////////////////////////////////////////

