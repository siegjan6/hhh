#include "WebContainer.h"
//#include "BrowserManager.h"
#include <QApplication>
//#include "ClientProxy.h"
//#include "include/cef_v8.h"
#include <QThread>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QTextEdit>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include "webobjectinstance.h"
#include <QMessageBox>
#include <QEventLoop>

#ifdef OS_LINUX
#include <X11/Xlib.h>
#endif


CWebContainer::CWebContainer(ContentType type, QString content, QWidget *parent) :
    QWebEngineView(parent),
//    m_windowHandle(NULL),
    m_firstShow(true),
    m_content(content),
//    m_browser(NULL),
    m_type(type)
{

}
CWebContainer::CWebContainer(const QString& url, QWidget *parent) :
    QWebEngineView(parent),
//    m_windowHandle(NULL),
    m_firstShow(true),
    m_content(""),
//    m_browser(NULL),
    m_type(ContentType::string)
{
    m_url=url;
}

CWebContainer::~CWebContainer()
{
    qDebug("delete webcontainer");
//    if( m_browser )
//    {
//        delete m_browser;
//        m_browser = NULL;
//    }
    emit deleteBrowser(m_browserId);
}

void CWebContainer::initObjModel(const ObjList &objs)
{
    //! 临时js对象声明
    for(int idx = 0; idx < objs.count(); idx++)
    {
        QString eachModel = "\r\n declare var " + objs[idx].first + ": " + objs[idx].second + ";";
        m_objModel += eachModel;
    }
    //! 对象列表实体
    initObjListModel(objs);
    qDebug("init model %s", qPrintable(m_objModel));
}

void CWebContainer::showContent()
{
//    if(!gBrowserManager->m_errString.isEmpty())
//    {
//        showErrorWidget();
//        return;
//    }

    if( CWebContainer::ContentType::string == m_type )
        m_editorModel = m_content;
    else
    {
        QString filePath = m_content;
        QFile file(filePath);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
        QTextStream stream(&file);
        stream.setCodec(QTextCodec::codecForName("UTF-8"));
        m_editorModel = stream.readAll();

        file.close();
    }

    showBrowser();
}

void CWebContainer::saveFile()
{
//    CefRefPtr<CefFrame> frame = (*m_browser)->GetMainFrame();
//    frame->ExecuteJavaScript("saveFile();",
//                             frame->GetURL(), 0);
calljs("var ttt=123");
    calljs("saveFile();");
}

//void CWebContainer::showMindBrowser(const QString& url)
//{
//    qDebug("will load Url");

//#ifdef OS_WIN
//    CefWindowInfo info;
//    RECT rect{0,0,width(), height()};
//    info.SetAsChild((HWND)winId(), rect);
//#else
//    cef_window_info_t windowSettings;
//    windowSettings.x= 0;
//    windowSettings.y= 0;
//    windowSettings.height = 10;
//    windowSettings.width = 10;
//    windowSettings.parent_window = NULL;
//    windowSettings.window = NULL;
//#ifndef __arm__
//    windowSettings.transparent_painting_enabled = 0;
//#endif
//    windowSettings.windowless_rendering_enabled = 0;
//    CefWindowInfo info(windowSettings);
//#endif
//    CefBrowserSettings browserSettings;

////    gClientHandler = new CClientHandler;
////    CefRefPtr<CClientHandler> pJsHandler(gClientHandler);
//    std::string std_url = url.toStdString();
//    CefRefPtr<CefBrowser> browser = CefBrowserHost::CreateBrowserSync(
//                                      info,
//                                      gClientHandler,
//                                      std_url,
//                                      browserSettings,
//            #ifdef __arm__
//                                      0,
//            #endif
//                                      0);

//   connect(this, &CWebContainer::deleteBrowser, gClientHandler->m_qtProxy, &CClientProxy::onDeleteBrowser);
//   connect(gClientHandler->m_qtProxy, &CClientProxy::deleteBrowserRef, this, &CWebContainer::onDeleteBrowserRef );
//   connect(gClientHandler->m_qtProxy, &CClientProxy::errorBrowserRef, this, &CWebContainer::onInvoke );
//   connect(gClientHandler->m_qtProxy, &CClientProxy::consoleMessageBrowserRef, this, &CWebContainer::onInvoke );


//   //! 将container添加到browserHandler中
//    m_windowHandle = browser->GetHost()->GetWindowHandle();
//    m_browserId = browser->GetIdentifier();

//    m_browser = new CefRefPtr<CefBrowser>(browser);

//    gClientHandler->addWebContainerView(m_browserId, this);

//#ifdef OS_LINUX
//    initBrowser();
//#endif
//}

void CWebContainer::showBrowser()
{

    qDebug("will load Url");
    QWebEnginePage* p = page();
    if(p) {
        p->setWebChannel(&m_webChannel);
        m_webChannel.registerObject(QStringLiteral("mdmodel"), WebObjectInstance::instance());
        connect(WebObjectInstance::instance(),SIGNAL(signalInvokeFromJs(int)),this,SLOT(onInvokeFromJs(int)),Qt::QueuedConnection);
        connect(WebObjectInstance::instance(),SIGNAL(signalWriteFile(QString)),this,SLOT(onWriteFile(QString)),Qt::QueuedConnection);
        connect(WebObjectInstance::instance(),SIGNAL(signalSetModify(bool)),this,SLOT(onSetModify(bool)),Qt::QueuedConnection);

        QUrl qurl = QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/script/editor.html");
        setUrl(qurl);

        //QMessageBox::information(this, "showBrowser", "registerObject");
        connect(p, &QWebEnginePage::loadStarted, this, &CWebContainer::onPageLoadStarted);
        connect(p, &QWebEnginePage::loadFinished, this, &CWebContainer::onPageLoadFinished);
        //p->runJavaScript("window.objJsonModel=\""+m_strObjJson+"\";", 0);
        //p->runJavaScript("window.eventJsonModel=\""+m_strObjJson+"\";", 0);
    }


//#ifdef OS_WIN
//    CefWindowInfo info;
//    RECT rect{0,0,width(), height()};
//    info.SetAsChild((HWND)winId(), rect);
//#else
//    cef_window_info_t windowSettings;
//    windowSettings.x= 0;
//    windowSettings.y= 0;
//    windowSettings.height = 10;
//    windowSettings.width = 10;
//    windowSettings.parent_window = NULL;
//    windowSettings.window = NULL;
//#ifndef __arm__
//    windowSettings.transparent_painting_enabled = 0;
//#endif
//    windowSettings.windowless_rendering_enabled = 0;
//    CefWindowInfo info(windowSettings);
//#endif
//    CefBrowserSettings browserSettings;

//    QString qurl = "file://" + gBrowserManager->m_dirpath + "/script/editor.html";
//    std::string url;
//    //url = "http://www.baidu.com";
//    url = qurl.toStdString();

////    gClientHandler = new CClientHandler;
////    CefRefPtr<CClientHandler> pJsHandler(gClientHandler);

//    CefRefPtr<CefBrowser> browser = CefBrowserHost::CreateBrowserSync(
//                                      info,
//                                    gClientHandler,
//                                    url,
//                                    browserSettings,
//            #ifdef __arm__
//                                      0,
//            #endif
//                                      0);

//    connect(this, &CWebContainer::deleteBrowser, gClientHandler->m_qtProxy, &CClientProxy::onDeleteBrowser);
//    connect(gClientHandler->m_qtProxy, &CClientProxy::deleteBrowserRef, this, &CWebContainer::onDeleteBrowserRef );
//    //! 将container添加到browserHandler中
//    m_windowHandle = browser->GetHost()->GetWindowHandle();
//    m_browserId = browser->GetIdentifier();
//    m_browser = new CefRefPtr<CefBrowser>(browser);

//    gClientHandler->addWebContainerView(m_browserId, this);



//#ifdef OS_LINUX
//    initBrowser();
//#endif

}

void CWebContainer::initObjListModel(const ObjList &objs)
{
    //!初始化treeGrid的顶级节点
    QJsonArray topLevelNodes;

    QJsonObject json;
    json.insert("id", "local");
    json.insert("name", tr("本地脚本对象"));
    QJsonArray children;
    for(int idx = 0; idx < objs.count(); idx++)
    {
        QString objName = objs[idx].first;
        QString objClass = objs[idx].second;
        QJsonObject oneObj;
        oneObj.insert("id", objName);
        oneObj.insert("name", objName);
        oneObj.insert("class", objClass);
        children.append(oneObj);
    }
    json.insert("children", children);
    topLevelNodes.append(json);

    QJsonObject json_global;
//    json_global.insert("id", "global");
//    json_global.insert("name", tr("全局脚本对象"));
//    QJsonArray children_global;
////    for(int idx = 0; idx < objs.count(); idx++)
////    {
////        QString objName = objs[idx].first;
////        QString objClass = objs[idx].second;
////        QJsonObject oneObj;
////        oneObj.insert("id", objName);
////        oneObj.insert("name", objName);
////        oneObj.insert("class", objClass);
////        children.append(oneObj);
////    }
//    QJsonObject Obj1;
//    Obj1.insert("id", "g_Project");
//    Obj1.insert("name", "g_Project");
//    Obj1.insert("class", "CProjectMaster");
//    children_global.append(Obj1);

//    QJsonObject Obj2;
//    Obj2.insert("id", "g_Variable");
//    Obj2.insert("name", "g_Variable");
//    Obj2.insert("class", "CVariableMaster");
//    children_global.append(Obj2);

//    QJsonObject Obj3;
//    Obj3.insert("id", "g_DataSource");
//    Obj3.insert("name", "g_DataSource");
//    Obj3.insert("class", "CDataSourceMaster");
//    children_global.append(Obj3);

//    QJsonObject Obj4;
//    Obj4.insert("id", "g_DataQuery");
//    Obj4.insert("name", "g_DataQuery");
//    Obj4.insert("class", "CDataQueryMaster");
//    children_global.append(Obj4);

//    QJsonObject Obj5;
//    Obj5.insert("id", "g_Window");
//    Obj5.insert("name", "g_Window");
//    Obj5.insert("class", "CWindowMaster");
//    children_global.append(Obj5);

//    QJsonObject Obj6;
//    Obj6.insert("id", "g_Authorize");
//    Obj6.insert("name", "g_Authorize");
//    Obj6.insert("class", "CAuthMaster");
//    children_global.append(Obj6);

//    QJsonObject Obj7;
//    Obj7.insert("id", "g_Recipe");
//    Obj7.insert("name", "g_Recipe");
//    Obj7.insert("class", "CRecipeMaster");
//    children_global.append(Obj7);

//    json_global.insert("children", children_global);


    json_global = initGlobalObjListModel();
    topLevelNodes.append(json_global);

    QJsonDocument doc(topLevelNodes);
    m_strObjJson = doc.toJson();
}

QJsonObject CWebContainer::initGlobalObjListModel()
{
    QList<QPair<QString, QString> > globalObj;
    globalObj.append(qMakePair(QString("g_Project"), QString("CProjectMaster")));
    globalObj.append(qMakePair(QString("g_Variable"), QString("CVariableMaster")));
    globalObj.append(qMakePair(QString("g_DataSource"), QString("CDataSourceMaster")));
//    globalObj.append(qMakePair(QString("g_DataQuery"), QString("CDataQueryMaster")));
    globalObj.append(qMakePair(QString("g_DbQuery"), QString("CDbQueryMaster")));
    globalObj.append(qMakePair(QString("g_Window"), QString("CWindowMaster")));
    globalObj.append(qMakePair(QString("g_Authorize"), QString("CAuthMaster")));
    globalObj.append(qMakePair(QString("g_Recipe"), QString("CRecipeMaster")));

    QJsonObject json_global;
    json_global.insert("id", "global");
    json_global.insert("name", tr("全局脚本对象"));
    QJsonArray children;
    for(int idx = 0; idx < globalObj.count(); idx++)
    {
        QString objName = globalObj[idx].first;
        QString objClass = globalObj[idx].second;
        QJsonObject oneObj;
        oneObj.insert("id", objName);
        oneObj.insert("name", objName);
        oneObj.insert("class", objClass);
        children.append(oneObj);

        QString eachModel = "\r\n declare var " + globalObj[idx].first + ": " + globalObj[idx].second + ";";
        m_objModel += eachModel;
    }
    json_global.insert("children", children);
    return json_global;
}

void CWebContainer::showErrorWidget()
{
    QTextEdit* text = new QTextEdit(this);
    text->setAlignment(Qt::AlignCenter);
    text->setReadOnly(true);
    QFont font = text->font();
    font.setPointSize(16);
    text->setFont(font);
//    text->insertPlainText(gBrowserManager->m_errString);
    QLayout* layer = layout();
    if( !layer )
    {
        layer = new QHBoxLayout;
        setLayout(layer);
    }

    layer->addWidget(text);

    text->show();
}

void CWebContainer::resizeEvent(QResizeEvent *event)
{
    QSize newsize = event->size();

#ifdef Q_OS_WIN
    //! window 发送WM_SIZE消息
//    if(m_windowHandle)
//    {
//        MoveWindow(m_windowHandle, 0, 0, newsize.width(), newsize.height(), TRUE);
//    }
#else
#endif

}


bool CWebContainer::eventFilter(QObject *obj, QEvent *event)
{
#ifdef OS_LINUX
    QString name = obj->metaObject()->className();
    if( event->type() == QEvent::Show )
    {
        //qDebug("will show web %s", qPrintable(name));
    }
    //if( event->type() != QEvent::Paint )
        //qDebug("filterevent %d  %s", event->type(), qPrintable(name));
    if( event->type() == QEvent::WindowDeactivate && m_firstShow)
    {
        initSize();
    }
#endif

    return QWidget::eventFilter(obj, event);
}


//void CWebContainer::initBrowser()
//{
//    QWindow* window = NULL;
//    window = QWindow::fromWinId((WId)m_windowHandle);
//    QWidget* widget = NULL;
//    widget = QWidget::createWindowContainer(window, this);
//    widget->setParent(this);
//    setLayout(new QVBoxLayout());

//    layout()->setMargin(0);
//    layout()->addWidget(widget);

//    widget->installEventFilter(this);
//}

void CWebContainer::onDeleteBrowserRef()
{
    qDebug("will delete ref222");
//    if( m_browser )
//    {
//        delete m_browser;
//        m_browser = NULL;
//    }
}

void CWebContainer::onWriteFile(QString content)
{
    //qDebug("did writeFile!~~ %s", qPrintable(content));

    if( CWebContainer::ContentType::string == m_type )
        m_editorModel = content;
    else{
        QString filePath = m_content;
//        CJsonFile sFile;
//        sFile.init(filePath);
//        sFile.writeFileBytes(content.toUtf8());
    }

    emit saveFileFinished();
}

void CWebContainer::onSetZoomLevel(double level)
{
    // 0.25 - 5.0
    double zoom = this->zoomFactor();

    if( level == 0 )
        this->setZoomFactor(0);
    else
        this->setZoomFactor(zoom+level);
}

void CWebContainer::onSetModify(bool modify)
{
    emit setModify(modify);
}

void CWebContainer::onInvoke(QString content)
{
    emit invoke(content);
}

void CWebContainer::initSize()
{
    QWidget* parent = parentWidget();

    if(!parent)
        return;

    QSize parentSize = parent->size();
    resize(parentSize  - QSize(1,1));
    resize(parentSize);
}

void CWebContainer::calljs(const QString& js)
{
    QWebEnginePage* p = page();
    if(p) {
        p->runJavaScript(js, [&](const QVariant &v) {
            qDebug() << "runJavaScript result:" <<v.toString();
//            QMessageBox::information(0, "runJavaScript result:", v.toString());
        });
    }
}

QVariant CWebContainer::getFromJs(const QString& js)
{
    QVariant ret;
    QWebEnginePage* p = page();
    if(p) {
        QEventLoop loop;
        p->runJavaScript(js, [&](const QVariant &v) {
            qDebug() << "runJavaScript result:" <<v.toString();
//            QMessageBox::information(0, "runJavaScript result:", v.toString());
            ret = v;
            loop.quit();
        });
        loop.exec();
    }
    return ret;
}


void CWebContainer::onPageLoadStarted()
{
    //QMessageBox::information(this,"onPageLoadStarted",m_strObjJson);
    //qDebug() << "window.objJsonModel=\""+m_strObjJson+"\";";
    //calljs("window.objJsonModel="+m_strObjJson);
}

void CWebContainer::onPageLoadFinished(bool ok)
{
//    QMessageBox::information(this,"onPageLoadFinished",m_strObjJson);
//    calljs("window.objJsonModel="+m_strObjJson);
    //QMessageBox::information(this,"onPageLoadFinished",QString("var data1 = %1 ; var data2 = %2 ; initModel(data1, data2);").arg(m_strObjJson).arg(m_strObjJson));
    QFile eventdefine_configFile(QCoreApplication::applicationDirPath() + "/script/eventdefine.json");
    if(!eventdefine_configFile.exists())
        return;

    eventdefine_configFile.open(QIODevice::ReadOnly);
    QByteArray ba1;
    QTextStream stream1(&eventdefine_configFile);
    stream1.setCodec("UTF-8");
    ba1 = stream1.readAll().toUtf8();
    eventdefine_configFile.close();

    QJsonParseError error1;
    QJsonDocument doc1 = QJsonDocument::fromJson(ba1, &error1);
    if(error1.error != QJsonParseError::NoError)
        return;
    QString strEventJson = doc1.toJson();


    QFile jsdefine_configFile(QCoreApplication::applicationDirPath() + "/script/define.d.ts");

    if(!jsdefine_configFile.exists())
        return;

    jsdefine_configFile.open(QIODevice::ReadOnly);
    QByteArray ba2;
    QTextStream stream2(&jsdefine_configFile);
    stream2.setCodec("UTF-8");
    ba2 = stream2.readAll().toUtf8();
    jsdefine_configFile.close();

    QString strJsDefine = ba2;
    strJsDefine += m_objModel;

    calljs(QString("var obj_data1 = %1 ; var evt_data2 = %2 ; var jsdefine_data3 = `%3` ; var code_data4 = `%4` ; initModel(obj_data1, evt_data2, jsdefine_data3, code_data4) ;").arg(m_strObjJson).arg(strEventJson).arg(strJsDefine).arg(m_editorModel));
}


void CWebContainer::onInvokeFromJs(int mode)
{
    if(mode == 0) {
        // need to set objJsonModel
        calljs("var objJsonModel=123");
    }
}
