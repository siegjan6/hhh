#ifndef WEBCONTAINER_H
#define WEBCONTAINER_H

#include <QWindow>
#include <QLayout>
#include <QWidget>
#include <QResizeEvent>
#include <QWebEngineView>
#include <QWebChannel>
#include <QJsonObject>

typedef QList< QPair<QString, QString> > ObjList;

class CWebContainer : public QWebEngineView
{
    Q_OBJECT
public:
    enum class ContentType{
        file = 0,
        string
    };

    explicit CWebContainer(ContentType type, QString content, QWidget *parent = 0);
    explicit CWebContainer(const QString& url, QWidget *parent = 0);
    virtual ~CWebContainer();

    void initObjModel(const ObjList &objs);
    void showContent();
    void saveFile();
    void initSize();
    void calljs(const QString& js);
    QVariant getFromJs(const QString& js);
public:
    int m_browserId;
//    CefWindowHandle m_windowHandle;
    bool m_firstShow;
    QString m_editorModel;
    QString m_objModel;
    QString m_strObjJson;
    QString m_content;
    QString m_url;
//    void showMindBrowser(const QString& url);
protected:
    virtual void resizeEvent(QResizeEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);
private:
//    CefRefPtr<CefBrowser>* m_browser;
    ContentType m_type;
    QWebChannel m_webChannel;
private:
    //void initBrowser();
    void showBrowser();
    void showErrorWidget();
    void initObjListModel(const ObjList &objs);    //!对象列表,用于侧边栏的对象一览
    QJsonObject initGlobalObjListModel(); //!全局对象列表,用于侧边栏的对象一览
signals:
    void deleteBrowser(int browserId);
    void setModify(bool modify);
    void saveFileFinished();
    void invoke(QString content);
public slots:
    void onDeleteBrowserRef();
    void onWriteFile(QString content);
    void onSetZoomLevel(double level);
    void onSetModify(bool modify);
    void onInvoke(QString content);
    void onPageLoadStarted();
    void onPageLoadFinished(bool ok);

    // for js invoke qt
    void onInvokeFromJs(int mode);
};

#endif // WEBCONTAINER_H
