#ifndef DATAGROUPFRAMEWORK_H
#define DATAGROUPFRAMEWORK_H

/*!

\author bHVvY2hhbw==

\section 功能
1.数据组DMI主窗体，继承类CBaseForm。
2.负责数据组相关界面配置呈现。

*/

#include "BaseForm.h"

namespace Ui {
class DBstudioFramework;
}

class IMDRTDBManagerUI;
class CDataArchivesUI;
class IMDDataArchives;
class IMDDataGroup;
class CMDDataGroupTableWidget;
class IMDDataGroupManager;

class CDataGroupFramework : public CBaseForm
{
public:
    CDataGroupFramework(QWidget *parent = 0);
    ~CDataGroupFramework();

    /*!
    \section 派生实现保存功能
    */
public:
    virtual bool save();

    /*!
    \section 配置数据组呈现需要的相关属性
    */
public:
    void setDataGrpProperty(IMDRTDBManagerUI *pRTDBManagerUI, IMDDataArchives *pDataArchives, IMDDataGroup *pDataGrp);

    void setDataArchivesUI(CDataArchivesUI *pDataArchivesUI);
    inline CDataArchivesUI *getDataArchivesUI(){return m_pDataArchivesUI;}

    void changeLanguage(); //! 语言切换

    /*!
    \section 派生实现窗体关闭
    */
protected:
    void closeEvent(QCloseEvent *e);

private:
    Ui::DBstudioFramework *ui                    ; //! 窗体UI类
    CDataArchivesUI *m_pDataArchivesUI           ; //! 数据归档界面指针
    CMDDataGroupTableWidget *m_pDataGrpDlg       ; //! 数据组界面各配置界面类
};

#endif // DATAGROUPFRAMEWORK_H
