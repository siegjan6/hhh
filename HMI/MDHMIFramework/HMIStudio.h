/*!
\author aGRw 2014.11.13
\section 图形编辑环境管理类
*/
#ifndef HMISTUDIO_H
#define HMISTUDIO_H

#include <QObject>
#include <QList>

#include "HMIFramework.h"
#include "IHMIStudio.h"
#include "ObjectTreeDialog.h"
#include "MDGraphicsStorage.h"
#include "MDHMIForm.h"
#include "ToolKitDialog.h"
#include "PreviewDialog.h"

class CFileNode;


class CHMIStudio : public IHMIStudio
{
public:
    CHMIStudio();
    ~CHMIStudio();
/*!
\section private function
*/
private:
   void clear();
/*!
\section propertyGrid
*/
public:
   //!设置属性框选择控件
   void selectObject(QObject* obj);
   //!属性框刷新
   void refreshProperty();
public:
   CPropertyBrowser* property(){return m_property;}
private:
   //!属性框
   CPropertyBrowser* m_property;
/*!
\section framework
*/
public:
   CHMIFramework* open();
/*!
\section fileNode
*/
public:
   CFileNode* fileNode(){return m_fileNode;}
private:
   CFileNode* m_fileNode;
/*!
\section objectTree
*/
public:
   QWidget* objectTreeDialog(){return &m_objectTreeDialog;}
   void setObjectTree(IHMIForm* form);
   IObjectChangedEvent* objectChangedEvent(){return &m_objectTreeDialog;}
private:
   CObjectTreeDialog m_objectTreeDialog;
/*!
\section graphics storage
*/
public:
   IGraphicsStorage* graphicsStorage();
private:
   IGraphicsStorage* m_storage;//!图库
/*!
\section tool kit
*/
public:
    void setCurrentForm(IHMIForm* value){m_currentForm = value;}
    IHMIForm* currentForm(){return m_currentForm;}
    QWidget* toolKitDialog(){return &m_toolKitDialog;}
private:
    IHMIForm* m_currentForm;
    CToolKitDialog m_toolKitDialog;
private:

public:
    //!开始创建
    void setCreateState(DrawType type);
    //!结束创建
    void endCreate();
    //!回到指针
    void resetToolKit();
/*!
\section language
*/
public:
    void changeLanguage();
/*!
\section image path
*/
public:
    QString storageImagePath(){return m_storageImagePath;}
private:
    QString m_storageImagePath;//!图库图片目录
private:
    void initStorageImagePath();
/*!
\section form preview
*/
public:
    void initFormPreview(IHMIFramework* framework);
    QDialog* formPreviewDialog(){return &m_formPreviewDialog;}
private:
    CPreviewDialog m_formPreviewDialog;//!布局预览对话框

};

#endif // HMISTUDIO_H



