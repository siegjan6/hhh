/*!
\author aGRw 2015.07.27
\section 图库对话框
*/
#ifndef GRAPHICSDIALOG_H
#define GRAPHICSDIALOG_H

#include <QDialog>
#include <QTreeWidget>
#include <QScrollArea>
#include <QJsonObject>
#include "ObjectWidget.h"
#include "StorageFileNode.h"
#include "MDDrawObj.h"
#include "IStudio.h"
#include "IGraphicsStorage.h"
#include "SelectPathDialog.h"

namespace Ui {
class CGraphicsDialog;
}

class CGraphicsDialog : public QDialog, public IGraphicsStorage
{
    Q_OBJECT
public:
    CGraphicsDialog(const QString& imagePath, QWidget *parent = 0);
    ~CGraphicsDialog();
private:
    Ui::CGraphicsDialog *ui;
//////////////////////////////////////////////////////////////////////////
public:
    QString imagePath(){return m_imagePath;}
private:
    QString m_imagePath;//!图片目录
//////////////////////////////////////////////////////////////////////////
private:
    QString m_savePath;//!保存目录
    QTreeWidget* m_tree;//!目录控件
    CObjectWidget* m_widget;//!控件
    QScrollArea* m_scroll;
    QString m_basePath;//!图库目录
    CStorageFileNode m_node;//!文件管理节点
    IHMIForm* m_container;
    CSelectPathDialog* m_selectPathDialog;
    bool m_canEvent;//!事件是否生效
////////////////////////////////////////////////////////////////////////////
public:
    //!保存控件
    void saveObjDialog(QList<CMDDrawObj*>* objs);
    //!保存控件到默认目录
    void saveObjToPath(QList<CMDDrawObj*>* objs);
    //!打开对话框
    void showDialog(IHMIForm* container);
////////////////////////////////////////////////////////////////////////////
protected:
    bool eventFilter(QObject *watched, QEvent *event);
////////////////////////////////////////////////////////////////////////////
private:
    void init();
    void reset();
    void serialize(QJsonObject& json, CMDDrawObj* obj);
    void currentPathChanged(const QString& path);
/*!
\section language
*/
public:
    void changeLanguage();
private:
    ITranslateWidgetTool* m_transTool;

};

#endif // GRAPHICSDIALOG_H
