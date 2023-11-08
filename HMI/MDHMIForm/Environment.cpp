#include <QPrintDialog>
#include <QPrinterInfo>
#include <QPair>
#include <QList>
#include "Environment.h"
#include "MDDrawObj.h"
#include "MDDrawMulti.h"
#include "MDDrawWidget.h"
#include "MDHMIForm.h"
#include "IHMIStudio.h"
#include "DrawCreator.h"
#include "MDProjectConfiger/MDProjectConfiger.h"
#include "MDProjectConfiger.h"
#include "PrintConfigDialog.h"
#include "MDJsonConvert.h"
//////////////////////////////////////////////////////////////////////////
CEnvironment::CEnvironment(IHMIForm *container)
{
    Q_ASSERT(container != NULL);

    CDrawCreator::initExpression();

    m_rect = QRect(0, 0, 800, 600); //!设计尺寸,窗体最大化或全屏状态下实际尺寸和设计尺寸不一致。
    m_formType = FormType::Normal;//!窗体类型
    m_hasFrame = true; //!窗体边框
    m_fullScreen = true;//!全屏

    m_defaultLayer = 0;//!当前层
    m_layerCount = 10;
    m_visibleLayers = QVector<bool>(m_layerCount, true);//!层的可见信息
    m_lockedLayers = QVector<bool>(m_layerCount, false);//!层的锁定信息

    m_printTop = 5;
    m_printBottom = 5;
    m_printLeft = 5;
    m_printRight = 5;
    m_printHeader = 8;
    m_printFooter = 8;
    m_printDateFormat = "yyyy-MM-dd";
    m_printTimeFormat = "hh:mm:ss";

    m_openSecurity = CMDActionSP(MDActionType::window_Open);//!安全策略-打开
    m_closeSecurity = CMDActionSP(MDActionType::window_Close);//!安全策略-关闭

    m_container = container;
    m_window = dynamic_cast<CMDHMIForm*>(container);
    m_brushManager.setBrushType(CMDBrushManager::BrushType::Solid);
    m_brushManager.setColor(QColor(Qt::lightGray));

    initExpManager();

    m_nameManager.init(this);
}
CEnvironment::~CEnvironment()
{
    for(int i = 0; i < m_objs.count(); i++)
        delete m_objs.at(i);
    m_objs.clear();
}
//////////////////////////////////////////////////////////////////////////
bool CEnvironment::save()
{
    QFile file(fullName());
    if (!file.open(QIODevice::WriteOnly))
        return false;

    QJsonObject json;
    serializeHead(json);
    serialize(json);

    QJsonDocument doc;
    doc.setObject(json);
    QJsonDocument::JsonFormat format = QJsonDocument::Compact;
#ifdef QT_DEBUG
    format = QJsonDocument::Indented;
#endif
    QByteArray ba = doc.toJson(/*format*/);

    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    stream << ba;

    file.close();
    return true;
}
bool CEnvironment::load(const QString &fullName)
{
    QFile file(fullName);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    QByteArray ba = stream.readAll().toUtf8();
    file.close();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(ba, &error);
    if(error.error != QJsonParseError::NoError)
        return false;

    deserialize(doc.object());

    return true;
}
void CEnvironment::resizeEvent(const QSize& size)
{
    m_paintSize = calculactPaintSize(QRect(QPoint(0, 0), size));
    generateContent();
}
CMDDrawObj *CEnvironment::findObject(const QString &name)
{
    for(int i = 0; i < m_objs.count(); i++)
    {
        CMDDrawObj* o = m_objs.at(i);
        if(name.compare(o->name(), Qt::CaseInsensitive) == 0)
            return o;

        if(o->type() == DrawType::Group)
        {
            CMDDrawGroup* g = dynamic_cast<CMDDrawGroup*>(o);
            CMDDrawObj* child = findGroupObject(name, g);
            if(child != NULL)
                return child;
        }
    }

    return NULL;
}
void CEnvironment::setWidgetParentNull()
{
    for(int i = 0; i < objs()->count(); i++)
    {
        if(objs()->at(i)->isWidget())
        {
            CMDDrawWidget* w = dynamic_cast<CMDDrawWidget*>(objs()->at(i));
            w->widget()->setParent(NULL);
        }
    }
}
//////////////////////////////////////////////////////////////////////////
void CEnvironment::mouseMove(Qt::MouseButton button, const QPointF& location)
{
    loadMouseEvent(button, location, MouseType::Move);
}
void CEnvironment::mouseDown(Qt::MouseButton button, const QPointF& location)
{
    loadMouseEvent(button, location, MouseType::Down);
}
void CEnvironment::mouseUp(Qt::MouseButton button, const QPointF& location)
{
    loadMouseEvent(button, location, MouseType::Up);
}
void CEnvironment::initalization()
{
    m_brushManager.setProjectPath(projectPath());

    //控件初始化
    for(int i = 0; i < m_objs.count(); i++)
    {
        m_objs.at(i)->setParent(container());
        m_objs.at(i)->loadInitalizationEvent();
    }

    //action
    m_mouseDownActions.setContainer(container());
    m_mouseUpActions.setContainer(container());
    m_rightMouseDownActions.setContainer(container());
    m_rightMouseUpActions.setContainer(container());
    m_openActions.setContainer(container());
    m_closeActions.setContainer(container());

    resizeEvent(m_rect.size());

    m_nameManager.resetDict(objs());

    m_window->changeLanguage();
}
//////////////////////////////////////////////////////////////////////////
CMDDrawObj *CEnvironment::findGroupObject(const QString &name, CMDDrawGroup *parent)
{
    for(int i = 0; i < parent->objList()->count(); i++)
    {
        CMDDrawObj* o = parent->objList()->at(i);
        if(name.compare(o->name(), Qt::CaseInsensitive) == 0)
            return o;

        if(o->type() == DrawType::Group)
        {
            CMDDrawGroup* g = dynamic_cast<CMDDrawGroup*>(o);
            CMDDrawObj* child = findGroupObject(name, g);
            if(child != NULL)
                return child;
        }
    }

    return NULL;
}
//////////////////////////////////////////////////////////////////////////
void CEnvironment::setRect(const QRect &value)
{
    const int Min = 1;
    QRect v = value;
    if(v.width() < Min)
        v.setWidth(Min);
    if(v.height() < Min)
        v.setHeight(Min);

    m_rect = v;
}
void CEnvironment::setFullName(const QString &value)
{
    m_fullName = value;
}
//////////////////////////////////////////////////////////////////////////
void CEnvironment::paint(QPainter *p, const QRect& rect)
{
    drawBackground(p);
    environmentPaint(p, rect);
}
void CEnvironment::drawBackground(QPainter *p)
{
    environmentDrawBackgound(p);
}
bool CEnvironment::isInvalidateObject(CMDDrawObj *obj, const QRect& invalidateRect)
{
    qreal w = 3;
    w = qMax(w, obj->widenWidth());
    QRectF rect = getScrollAndScaleRect(obj->bound());
    rect.adjust(-w, -w, w, w);
    return rect.intersects(invalidateRect);
}
void CEnvironment::environmentDrawBackgound(QPainter *p)
{
    m_brushManager.draw(p, m_paintSize, &m_path);
}
void CEnvironment::environmentPaint(QPainter *p, const QRect& rect)
{
    p->setRenderHint(QPainter::Antialiasing, true);

    for(int i = 0; i < m_objs.count(); i++)
    {
        CMDDrawObj* obj = m_objs.at(i);
        if (obj->finalVisible() && isInvalidateObject(obj, rect))
            obj->draw(p);
    }
}
//////////////////////////////////////////////////////////////////////////
void CEnvironment::timeInvalidate()
{
    if (m_rectInvalidate.isValid())
    {
        m_window->update(m_rectInvalidate.toAlignedRect());
        m_rectInvalidate = QRectF();
    }
}
//////////////////////////////////////////////////////////////////////////
void CEnvironment::serialize(QJsonObject &json)
{
    QJsonObject js;

    js.insert("Rect", CMDJsonConvert::toJson(m_rect));
    js.insert("FormType", (int)m_formType);
    js.insert("HasFrame", m_hasFrame);
    js.insert("FullScreen", m_fullScreen);
    js.insert("Text", m_text);
    js.insert("DefaultLayer", m_defaultLayer);
    js.insert("ScriptCode", m_scriptCode);

    js.insert("PrintTop", m_printTop);
    js.insert("PrintBottom", m_printBottom);
    js.insert("PrintLeft", m_printLeft);
    js.insert("PrintRight", m_printRight);
    js.insert("PrintHeader", m_printHeader);
    js.insert("PrintFooter", m_printFooter);
    js.insert("PrintLeftHeader", m_printLeftHeader);
    js.insert("PrintMiddleHeader", m_printMiddleHeader);
    js.insert("PrintRightHeader", m_printRightHeader);
    js.insert("PrintLeftFooter", m_printLeftFooter);
    js.insert("PrintMiddleFooter", m_printMiddleFooter);
    js.insert("PrintRightFooter", m_printRightFooter);
    js.insert("PrintDateFormat", m_printDateFormat);
    js.insert("PrintTimeFormat", m_printTimeFormat);

    QJsonArray arrayVL;
    foreach(bool b, m_visibleLayers)
        arrayVL.append(b);
    js.insert("VisibleLayers", arrayVL);
    QJsonArray arrayLL;
    foreach(bool b, m_lockedLayers)
        arrayLL.append(b);
    js.insert("LockedLayers", arrayLL);

    QJsonObject jsMD;
    m_mouseDownActions.serialize(jsMD);
    js.insert("MouseDownActions", jsMD);
    QJsonObject jsMU;
    m_mouseUpActions.serialize(jsMU);
    js.insert("MouseUpActions", jsMU);
    QJsonObject jsRMD;
    m_rightMouseDownActions.serialize(jsRMD);
    js.insert("RightMouseDonwActions", jsRMD);
    QJsonObject jsRMU;
    m_rightMouseUpActions.serialize(jsRMU);
    js.insert("RightMouseUpActions", jsRMU);
    QJsonObject jsOpen;
    m_openActions.serialize(jsOpen);
    js.insert("OpenActions", jsOpen);
    QJsonObject jsClose;
    m_closeActions.serialize(jsClose);
    js.insert("CloseActions", jsClose);

    QJsonObject jsOpenSecurity;
    m_openSecurity.serialize(jsOpenSecurity);
    js.insert("OpenSecurity", jsOpenSecurity);
    QJsonObject jsCloseSecurity;
    m_closeSecurity.serialize(jsCloseSecurity);
    js.insert("CloseSecurity", jsCloseSecurity);

    QJsonObject jsBrushManager;
    m_brushManager.serialize(jsBrushManager);
    js.insert("BrushManager", jsBrushManager);
    QJsonObject jsExpManager;
    m_expManager.serialize(jsExpManager);
    js.insert("ExpManager", jsExpManager);

    QJsonArray arrayObj;
    foreach(CMDDrawObj* obj, m_objs)
    {
        QJsonObject jsObj;
        jsObj.insert("Type", (int)obj->type());
        obj->serialize(jsObj);
        arrayObj.append(jsObj);
    }
    js.insert("Objs", arrayObj);

    json.insert("Environment", js);
}
void CEnvironment::deserialize(const QJsonObject &json)
{
    QJsonObject js = json.value("Environment").toObject();

    m_rect = CMDJsonConvert::toQRect(js.value("Rect"));
    m_formType = (FormType)js.value("FormType").toInt();
    m_hasFrame = js.value("HasFrame").toBool();
    m_fullScreen = js.value("FullScreen").toBool();
    m_text = js.value("Text").toString();
    m_defaultLayer = js.value("DefaultLayer").toInt();
    m_scriptCode = js.value("ScriptCode").toString();

    m_printTop = js.value("PrintTop").toInt();
    m_printBottom = js.value("PrintBottom").toInt();
    m_printLeft = js.value("PrintLeft").toInt();
    m_printRight = js.value("PrintRight").toInt();
    m_printHeader = js.value("PrintHeader").toInt();
    m_printFooter = js.value("PrintFooter").toInt();
    m_printLeftHeader = js.value("PrintLeftHeader").toString();
    m_printMiddleHeader = js.value("PrintMiddleHeader").toString();
    m_printRightHeader = js.value("PrintRightHeader").toString();
    m_printLeftFooter = js.value("PrintLeftFooter").toString();
    m_printMiddleFooter = js.value("PrintMiddleFooter").toString();
    m_printRightFooter = js.value("PrintRightFooter").toString();
    m_printDateFormat = js.value("PrintDateFormat").toString();
    m_printTimeFormat = js.value("PrintTimeFormat").toString();

    QJsonArray arrayVL = js.value("VisibleLayers").toArray();
    int visibleCount = arrayVL.count();
    if(visibleCount > m_visibleLayers.count())
        visibleCount = m_visibleLayers.count();
    for(int i = 0; i < visibleCount; i++)
        m_visibleLayers[i] = arrayVL.at(i).toBool();
    QJsonArray arrayLL = js.value("LockedLayers").toArray();
    int lockedCount = arrayLL.count();
    if(lockedCount > m_lockedLayers.count())
        lockedCount = m_lockedLayers.count();
    for(int i = 0; i < lockedCount; i++)
        m_lockedLayers[i] = arrayLL.at(i).toBool();

    m_mouseDownActions.deserialize(js.value("MouseDownActions").toObject());
    m_mouseUpActions.deserialize(js.value("MouseUpActions").toObject());
    m_rightMouseDownActions.deserialize(js.value("RightMouseDownActions").toObject());
    m_rightMouseUpActions.deserialize(js.value("RigthMouseUpActions").toObject());
    m_openActions.deserialize(js.value("OpenActions").toObject());
    m_closeActions.deserialize(js.value("CloseActions").toObject());

    m_openSecurity.deserialize(js.value("OpenSecurity").toObject());
    m_closeSecurity.deserialize(js.value("CloseSecurity").toObject());

    m_brushManager.deserialize(js.value("BrushManager").toObject());
    m_expManager.deserialize(js.value("ExpManager").toObject());

    QJsonArray arrayObj = js.value("Objs").toArray();
    foreach(QJsonValue v, arrayObj)
    {
        QJsonObject jsObj = v.toObject();
        DrawType type = (DrawType)jsObj.value("Type").toInt();
        CMDDrawObj* obj = CDrawCreator::createDrawObj(type);
        obj->deserialize(jsObj);
        m_objs.append(obj);
    }
}
void CEnvironment::serializeHead(QJsonObject &json)
{
    QJsonObject js;
    js.insert("Version", 1);
    js.insert("Type", QString("Mind.HMI.MDHMIForm"));

    json.insert("Head", js);
}
//////////////////////////////////////////////////////////////////////////
void CEnvironment::generateContent()
{
    resetPath(m_path);
    m_brushManager.generate(m_paintSize);
}
void CEnvironment::resetPath(QPainterPath &path)
{
    path = QPainterPath();
    path.addRect(m_paintSize);
}
//////////////////////////////////////////////////////////////////////////
void CEnvironment::objectCanSelectChanged(CMDDrawObj *obj)
{
    obj->invalidate();
}
void CEnvironment::setDefaultLayer(int value)
{
    if (value >= 0 && value <= m_layerCount - 1)
        m_defaultLayer = value;
}
void CEnvironment::updateVisible()
{
    for(int i = 0; i < objs()->count(); i++)
        objs()->at(i)->updateVisible();
    window()->update();
}
void CEnvironment::updateLocked()
{
    for(int i = 0; i < objs()->count(); i++)
        objs()->at(i)->updateEnabled();
    window()->update();
}
//////////////////////////////////////////////////////////////////////////
void CEnvironment::print(bool showPrintDialog)
{
    if(showPrintDialog)
    {
        QPrinter print;
        setPrintMargins(&print);
        QPrintDialog dlg(&print, window());
        if (dlg.exec() == QDialog::Accepted)
            directPrint(&print);
    }
    else
    {
        QPrinterInfo info = QPrinterInfo::defaultPrinter();
        if(info.isNull())
            return;

        QPrinter print(info);
        setPrintMargins(&print);
        directPrint(&print);
    }
}
void CEnvironment::directPrint(QPrinter *printer)
{
    printFun(printer, m_paintSize.toRect());
}
void CEnvironment::setPrintMargins(QPrinter *printer)
{
    printer->setPageMargins(m_printLeft, m_printTop+m_printHeader,
        m_printRight, m_printBottom+m_printFooter, QPrinter::Millimeter);
}
void CEnvironment::printRegion(const QRect &rect, bool showPrintDialog, bool isFullPage)
{
    if(showPrintDialog)
    {
        QPrinter print;
        setPrintMargins(&print);
        QPrintDialog dlg(&print, window());
        if (dlg.exec() == QDialog::Accepted)
            printFun(&print, rect, isFullPage);
    }
    else
    {
        QPrinterInfo info = QPrinterInfo::defaultPrinter();
        if(info.isNull())
            return;

        QPrinter print(info);
        setPrintMargins(&print);
        printFun(&print, rect, isFullPage);
    }
}
void CEnvironment::printObject(const QString &name, bool showPrintDialog, bool isFullPage)
{
    CMDDrawObj* obj = findObject(name);
    if(obj == NULL)
        return;

    QRect r = obj->bound().toRect();
    if(showPrintDialog)
    {
        QPrinter print;
        setPrintMargins(&print);
        QPrintDialog dlg(&print, window());
        if (dlg.exec() == QDialog::Accepted)
            printFun(&print, r, isFullPage, obj);
    }
    else
    {
        QPrinterInfo info = QPrinterInfo::defaultPrinter();
        if(info.isNull())
            return;

        QPrinter print(info);
        setPrintMargins(&print);
        printFun(&print, r, isFullPage, obj);
    }
}
void CEnvironment::printConfig()
{
    CPrintConfigDialog dlg(window());
    dlg.init(this);
    if (dlg.exec() == QDialog::Accepted)
        container()->setModify(true);
}
void CEnvironment::printFun(QPrinter *printer, const QRect &rect, bool isFullPage, CMDDrawObj *obj)
{
    QPainter p(printer);
    p.setRenderHint(QPainter::Antialiasing, true);
    QRect pageRect = printer->pageRect();
    QSize size = rect.size();

    //calculate rect
    int w = pageRect.width();
    int h = pageRect.height();
    int row = size.height() / h + 1;
    int col = size.width() / w + 1;
    int pageCount = row * col;

    for (int i = 0; i < printer->numCopies(); i++)
    {
        for (int j = 0; j < pageCount; j++)
        {
            if (j != 0)
                printer->newPage();

            int x = (j % col ) * w;
            int y = (j / col) * h;

            p.resetTransform();

            //header & footer
            p.setClipRect(QRect(), Qt::NoClip);
            QRectF rectHeader = QRectF(0, -m_printHeader, w, m_printHeader);
            QRectF rectFooter = QRectF(0, h, w, m_printFooter);

            QTextOption option;
            option.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            p.drawText(rectHeader, replacePrintString(m_showPrintLeftHeader, pageCount, j+1), option);
            p.drawText(rectFooter, replacePrintString(m_showPrintLeftFooter, pageCount, j+1), option);
            option.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            p.drawText(rectHeader, replacePrintString(m_showPrintMiddleHeader, pageCount, j+1), option);
            p.drawText(rectFooter, replacePrintString(m_showPrintMiddleFooter, pageCount, j+1), option);
            option.setAlignment(Qt::AlignRight | Qt::AlignVCenter);
            p.drawText(rectHeader, replacePrintString(m_showPrintRightHeader, pageCount, j+1), option);
            p.drawText(rectFooter, replacePrintString(m_showPrintRightFooter, pageCount, j+1), option);

            //填充整个页面
            if(isFullPage)
            {
                qreal pw = w;
                qreal ph = h;
                qreal wRate = pw / size.width();
                qreal hRate = ph / size.height();
                bool isW = (wRate < hRate);
                qreal rate = isW ? wRate : hRate;
                if(isW)
                    ph = size.height() * rate;
                else
                    pw = size.width() * rate;
                QPointF pageCenter(pw / 2, ph / 2);
                QPointF rectCenter(rect.x() + rect.width()/2.0, rect.y() + rect.height()/2.0);

                p.translate(pageCenter.x(), pageCenter.y());
                p.scale(rate, rate);
                p.translate(-rectCenter.x(), -rectCenter.y());

                p.setClipRect(rect);
            }
            else
            {
                //page
                p.translate(- x - rect.x(), - y - rect.y());
                QRect r = QRect(x + rect.x(), y + rect.y() , w, h);
                p.setClipRect(rect.intersected(r));
            }

            //background
            environmentDrawBackgound(&p);

            if(obj != NULL)
                obj->draw(&p, true);
            else
            {
                for(int i = 0; i < m_objs.count(); i++)
                {
                    CMDDrawObj* o = m_objs.at(i);
                    if (o->finalVisible())
                        o->draw(&p, true);
                }
            }

            //只绘制一页
            if(isFullPage)
                break;
        }
    }
}
QString CEnvironment::replacePrintString(const QString &source, int count, int page)
{
    const QString sDate = "&Date";
    const QString sTime = "&Time";
    const QString sPage = "&Page";
    const QString sPageCount = "&Count";

    QDateTime current = QDateTime::currentDateTime();
    QString d = current.toString(m_showPrintDateFormat);
    QString t = current.toString(m_showPrintTimeFormat);
    QString p = QString::number(page);
    QString c = QString::number(count);

    QString r = "@";
    QString src = source;
    src.remove(r);

    src.replace(sDate, r, Qt::CaseSensitive);
    src.replace(r, d);
    src.replace(sTime, r, Qt::CaseSensitive);
    src.replace(r, t);
    src.replace(sPage, r, Qt::CaseSensitive);
    src.replace(r, p);
    src.replace(sPageCount, r, Qt::CaseSensitive);
    src.replace(r, c);

    return src;
}
//////////////////////////////////////////////////////////////////////////
void CEnvironment::fillObjectToScript(QList<QPair<QString, QString> > &infos)
{
    infos.append(qMakePair(QString("form"), QString("CMDHMIForm")));
    foreach(CMDDrawObj* o, m_objs)
        fillObjectToScript(o, infos);
}
void CEnvironment::fillObjectToScript(CMDDrawObj *obj, QList<QPair<QString, QString> > &infos)
{
    infos.append(qMakePair(obj->name(), obj->className()));
    if(obj->type() == DrawType::Group)
    {
        CMDDrawGroup* g = dynamic_cast<CMDDrawGroup*>(obj);
        foreach(CMDDrawObj* o, *g->objList())
            fillObjectToScript(o, infos);
    }
}
//////////////////////////////////////////////////////////////////////////
QString CEnvironment::projectPath()
{
    if(container()->framework()->projectConfiger() == NULL)
        return "";
    return container()->framework()->projectConfiger()->path();
}
bool CEnvironment::isStudioMode()
{
    return container()->framework()->isStudioMode();
}
IMDDataArchives *CEnvironment::dataArchives()
{
    if(container()->framework()->projectConfiger() == NULL)
        return NULL;
    return container()->framework()->projectConfiger()->dataArchives();
}
IMDProjectExpert *CEnvironment::projectExpert()
{
    return container()->framework()->projectExpert();
}
IStudioInterface *CEnvironment::studioProject()
{
    return container()->framework()->studioProject();
}
void CEnvironment::update(const QRect &rect)
{
    window()->update(rect);
}
//////////////////////////////////////////////////////////////////////////
QList<QStringList> CEnvironment::expNames()
{
    QList<QStringList> list;
    list << QStringList({"#layout",         QObject::tr("布局")});
    list << QStringList({"form.x",          QObject::tr("左")});
    list << QStringList({"form.y",          QObject::tr("顶")});
    list << QStringList({"form.width",      QObject::tr("宽")});
    list << QStringList({"form.height",     QObject::tr("高")});

    list << QStringList({"#appearance",         QObject::tr("外观")});
    list << QStringList({"form.visible",        QObject::tr("可见")});

    list << QStringList({"#layer",         QObject::tr("层")});
    list << QStringList({"form.visibleLayer0",  QObject::tr("0层可见")});
    list << QStringList({"form.visibleLayer1",  QObject::tr("1层可见")});
    list << QStringList({"form.visibleLayer2",  QObject::tr("2层可见")});
    list << QStringList({"form.visibleLayer3",  QObject::tr("3层可见")});
    list << QStringList({"form.visibleLayer4",  QObject::tr("4层可见")});
    list << QStringList({"form.visibleLayer5",  QObject::tr("5层可见")});
    list << QStringList({"form.visibleLayer6",  QObject::tr("6层可见")});
    list << QStringList({"form.visibleLayer7",  QObject::tr("7层可见")});
    list << QStringList({"form.visibleLayer8",  QObject::tr("8层可见")});
    list << QStringList({"form.visibleLayer9",  QObject::tr("9层可见")});
    list << QStringList({"form.lockedLayer0",   QObject::tr("0层锁定")});
    list << QStringList({"form.lockedLayer1",   QObject::tr("1层锁定")});
    list << QStringList({"form.lockedLayer2",   QObject::tr("2层锁定")});
    list << QStringList({"form.lockedLayer3",   QObject::tr("3层锁定")});
    list << QStringList({"form.lockedLayer4",   QObject::tr("4层锁定")});
    list << QStringList({"form.lockedLayer5",   QObject::tr("5层锁定")});
    list << QStringList({"form.lockedLayer6",   QObject::tr("6层锁定")});
    list << QStringList({"form.lockedLayer7",   QObject::tr("7层锁定")});
    list << QStringList({"form.lockedLayer8",   QObject::tr("8层锁定")});
    list << QStringList({"form.lockedLayer9",   QObject::tr("9层锁定")});
    return list;
}
void CEnvironment::initExpManager()
{
    m_expManager.clear();
    m_expManager.append(CMDExpConfig::expManager((int)DrawType::Form));
}
//////////////////////////////////////////////////////////////////////////
void CEnvironment::changeLanguage()
{
    CMDMultiLanguage* l = CMDMultiLanguage::instance();

    m_showPrintLeftHeader = l->value(m_printLeftHeader, false);
    m_showPrintMiddleHeader = l->value(m_printMiddleHeader, false);
    m_showPrintRightHeader = l->value(m_printRightHeader, false);
    m_showPrintLeftFooter = l->value(m_printLeftFooter, false);
    m_showPrintMiddleFooter = l->value(m_printMiddleFooter, false);
    m_showPrintRightFooter = l->value(m_printRightFooter, false);
    m_showPrintDateFormat = l->value(m_printDateFormat, false);
    m_showPrintTimeFormat = l->value(m_printTimeFormat, false);

    int count = m_objs.count();
    for(int i = 0; i < count; i++)
        m_objs.at(i)->changeLanguage();

}
//////////////////////////////////////////////////////////////////////////
QPointF CEnvironment::getRevertScrollAndScalePointF(const QPointF &point)
{
    return QPointF(point.x() / formScale().x(), point.y() / formScale().y());
}
QRectF CEnvironment::getScrollAndScaleRect(const QRectF &rect)
{
    qreal scaleW = formScale().x();
    qreal scaleH = formScale().y();
    QRectF rf = rect;
    rf.moveLeft(rect.x()*scaleW);
    rf.moveTop(rect.y()*scaleH);
    rf.setWidth(rect.width()*scaleW);
    rf.setHeight(rect.height()*scaleH);
    return rf;
}
QRectF CEnvironment::getRevertScrollAndScaleRect(const QRectF &rect)
{
    qreal scaleW = formScale().x();
    qreal scaleH = formScale().y();
    QRectF rf = rect;
    rf.moveLeft(rect.x() / scaleW);
    rf.moveTop(rect.y() / scaleH);
    rf.setWidth(rect.width() / scaleW);
    rf.setHeight(rect.height() / scaleH);
    return rf;
}
//////////////////////////////////////////////////////////////////////////





