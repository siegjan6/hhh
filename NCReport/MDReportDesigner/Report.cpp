#include "Report.h"
#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QDebug>
#include <QProcess>
#include <QFileInfo>
#include <QString>
#include "MDMultiLanguage.h"

CReport::CReport()
{
    m_studioInterface = NULL;
    m_rootInStudio = NULL;

    m_pNCReportDirPopupMenu = NULL;
    m_NCReportDir_NewAction = NULL;
    m_pNCReportFilePopupMenu = NULL;
    m_pNCReportFile_OpenAction = NULL;
    m_pNCReportFile_DeleteAction = NULL;
    m_pNCReportFile_RenameAction = NULL;
    m_pNCRDMainWindow = NULL;
    m_pTreeWidget = NULL;
    m_ReportDirRoot = NULL;
    isexists_currentfile = false;
    m_NCReportFilePath = "";
    m_oldname = "";
    m_newname = "";
    sRootName = QString("报表");

    m_transTool = CMDMultiLanguage::createWidgetTool();
    m_transTool->init(this);
}

CReport::~CReport()
{
    unLoad();
}

/*!
\section 设置报表目录
*/
void CReport::setNCReportFilePath(const QString &NCReportFilePath)
{
    m_NCReportFilePath = NCReportFilePath;
}

/*!
\section 获取报表目录
*/
QString CReport::NCReportFilePath() const
{
    return m_NCReportFilePath;
}

/*!
\section 获取树管理器指针
*/
QTreeWidget *CReport::treeWidget()
{
    return (m_studioInterface ? m_studioInterface->treeWidget() : NULL);
}

/*!
\section 获取树根节点指针
*/
QTreeWidgetItem *CReport::treeRoot()
{
    return m_rootInStudio;
}

/*!
\section 加载项目报表树节点
*/
void CReport::load()
{
    connect(m_studioInterface->treeWidget(), &QTreeWidget::doubleClicked, this, &CReport::onMouseDbClick);
    connect(m_studioInterface->treeWidget(), &QTreeWidget::customContextMenuRequested, this, &CReport::onMouseRClick);

    buildReportDirTree(treeRoot());
}

/*!
\section 卸载项目报表树节点
*/
void CReport::unLoad()
{
    //    if(m_pNCReportDirPopupMenu)
    //    {
    //        delete m_pNCReportDirPopupMenu;
    //        m_pNCReportDirPopupMenu = NULL;
    //    }
    //    if(m_NCReportDir_NewAction)
    //    {
    //        delete m_NCReportDir_NewAction;
    //        m_NCReportDir_NewAction = NULL;
    //    }
    //    if(m_pNCReportFilePopupMenu)
    //    {
    //        delete m_pNCReportFilePopupMenu;
    //        m_pNCReportFilePopupMenu = NULL;
    //    }
    //    if(m_pNCReportFile_OpenAction)
    //    {
    //        delete m_pNCReportFile_OpenAction;
    //        m_pNCReportFile_OpenAction = NULL;
    //    }
    //    if(m_pNCReportFile_DeleteAction)
    //    {
    //        delete m_pNCReportFile_DeleteAction;
    //        m_pNCReportFile_DeleteAction = NULL;
    //    }
    //    if(m_pNCReportFile_RenameAction)
    //    {
    //        delete m_pNCReportFile_RenameAction;
    //        m_pNCReportFile_RenameAction = NULL;
    //    }
    if(m_pNCRDMainWindow)
    {
        m_pNCRDMainWindow->close();
        delete m_pNCRDMainWindow;
        m_pNCRDMainWindow = NULL;
    }
    //    if(m_pTreeWidget)
    //    {
    //        delete m_pTreeWidget;
    //        m_pTreeWidget = NULL;
    //    }
    //    if(m_pRoot)
    //    {
    //        delete m_pRoot;
    //        m_pRoot = NULL;
    //    }
    //    if(m_ReportDirRoot)
    //    {
    //        delete m_ReportDirRoot;
    //        m_ReportDirRoot = NULL;
    //    }
}

/*!
\section 鼠标右击事件处理函数
*/
void CReport::onMouseRClick(const QPoint &pos)
{
    QTreeWidgetItem *pItem = m_studioInterface->treeWidget()->itemAt(pos);

    if(isItemInChildren(pItem,treeRoot()))
    {
        // NCReport报表目录节点
        QTreeWidgetItem *p_Item = treeWidget()->currentItem();

        if(p_Item == m_ReportDirRoot )
        {
            ShowNCReportDirMenu(pos);
        }
        else if(p_Item->data(0,Qt::DisplayRole).toString().endsWith(".ncr",Qt::CaseSensitive) || p_Item->data(0,Qt::DisplayRole).toString().endsWith(".xml",Qt::CaseSensitive))
        {
            ShowNCReportFileMenu(pos);
        }
        else
        {
            return;
        }
    }
}

/*!
\section 鼠标双击事件处理函数
*/
void CReport::onMouseDbClick()
{
    QTreeWidgetItem* node = selectNode();
    if (node == NULL)
        return;

    if(m_pNCRDMainWindow)
    {
        if(m_studioInterface)
        {
            m_studioInterface->setActiveForm(m_pNCRDMainWindow);
        }
        m_pNCRDMainWindow->setActive(true);
    }
    else
    {
        m_pNCRDMainWindow = new NCRDMainWindow;
        if(m_pNCRDMainWindow)
        {
            m_pNCRDMainWindow->setProjectConfiger(m_studioInterface->projectConfiger());
            QString viewPath = m_studioInterface->projectConfiger()->path()+"/DBQuery";
            m_pNCRDMainWindow->setViewPath(viewPath);
            m_pNCRDMainWindow->setReport(this);
            if(m_studioInterface)
            {
                m_studioInterface->openMDIWindow(m_pNCRDMainWindow);
            }
        }
    }


    if(isItemInChildren(node,treeRoot()))
    {// NCReport报表目录节点
        OpenCurrentNCRreportFile();
    }
}

/*!
\section　项目窗体关闭事件处理函数
*/
void CReport::closeEvent(CBaseForm *pForm)
{
    if(m_pNCRDMainWindow != NULL && m_pNCRDMainWindow == pForm)
    {
        delete m_pNCRDMainWindow;
        m_pNCRDMainWindow = NULL;
    }
}

QTreeWidgetItem *CReport::selectNode()
{
    if (treeWidget() != NULL)
    {
        QTreeWidgetItem* node = treeWidget()->currentItem();
        if(isItemInChildren(node,treeRoot()))
        {
            return  node;
        }
    }

    return NULL;
}

bool CReport::isReportNode(QTreeWidgetItem *node)
{
    return (QString::compare(node->data(0, Qt::UserRole).toString(), sRootName) == 0 || QString::compare(node->parent()->data(0, Qt::UserRole).toString(), sRootName) == 0);
}

void CReport::clearNode(QTreeWidgetItem *node)
{
    for (int i = 0; i < node->childCount(); i++)
    {
        QTreeWidgetItem* child = node->child(i);
        clearNode(child);
        delete child;
    }
    node->takeChildren();
}

/*!
\section　鼠标右键菜单栏新建报表文件槽函数
*/
void CReport::slotitemActiveNewNCReportFile()
{
    NewNCRreportFile();
}

/*!
\section　鼠标右键菜单栏打开报表文件槽函数
*/
void CReport::slotitemActiveOpenNCReportFile()
{
    OpenCurrentNCRreportFile();
}

/*!
\section　鼠标右键菜单栏删除报表文件槽函数
*/
void CReport::slotitemActiveDeleteNCReportFile()
{
    DeleteCurrentNCRreportFile();
}

/*
 * 功能：
 *     鼠标右键菜单栏重命名报表文件槽函数
 * 参数：
 *     无
 * 返回值：
 * 	   无
 */
void CReport::slotitemActiveRenameNCReportFile()
{
    RenameCurrentNCRreportFile();
}

/*!
\section　关闭报表设计器窗体槽函数
*/
void CReport::slotCloseNCReportWindow()
{
    if(m_pNCRDMainWindow != NULL)
    {
        m_pNCRDMainWindow->close();
        delete m_pNCRDMainWindow;
        m_pNCRDMainWindow = NULL;
    }
}

/*!
\section　构建报表文件目录
*/
void CReport::buildReportDirTree(QTreeWidgetItem *pTreeRoot)
{
    clearNode(pTreeRoot);
    pTreeRoot->setText(0, sRootName);
    pTreeRoot->setData(0, Qt::UserRole, QVariant(sRootName));
    if(pTreeRoot->data(0,Qt::DisplayRole).toString() == sRootName)
    {
        LoadReportDirTree(pTreeRoot);
        m_ReportDirRoot = pTreeRoot;
    }
}

/*!
\section　加载报表树节点
*/
void CReport::LoadReportDirTree(QTreeWidgetItem *pRoot)
{
    CMDProjectConfiger *pProjectConfiger = m_studioInterface->projectConfiger();
    if(pRoot && pProjectConfiger)
    {
        m_NCReportFilePath = pProjectConfiger->path()+"/"+"Report";
        QDir dir(m_NCReportFilePath);
        if (!dir.exists())
        {
            if (!QDir::root().mkpath(m_NCReportFilePath))
            {
                qDebug()<<"Dir create failed";
                return;
            }
        }
        m_filelist = AddReportDirallfile(pRoot,m_NCReportFilePath);
    }
}

/*!
\section　遍历查找报表目录下报表文件
*/
QFileInfoList CReport::AddReportDirallfile(QTreeWidgetItem *root,QString path)
{
    /*添加path路径文件*/
    QDir dir(path);
    // 遍历各级子目录
    QDir dir_file(path);
    // 遍历子目录中所有文件
    dir_file.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks );
    // 获取当前所有文件
    //dir_file.setSorting(QDir::Time /*| QDir::Reversed*/);
    QFileInfoList list_file = dir_file.entryInfoList();
    for (int i = 0; i < list_file.size(); ++i)
    {
        //将当前目录中所有文件添加到treewidget中
        QFileInfo fileInfo = list_file.at(i);
        if(fileInfo.completeSuffix() == "ncr" || fileInfo.completeSuffix() == "xml")
        {
            QString name2=fileInfo.fileName();
            QTreeWidgetItem* child = new QTreeWidgetItem(QStringList()<<name2);
            child->setIcon(0,QIcon(":/images/file.png"));;
            child->setCheckState(1, Qt::Checked);
            root->addChild(child);
        }
    }

    QFileInfoList file_list=dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    // 获取当前所有目录
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    // 自动递归添加各目录到上一级目录
    for(int i = 0; i != folder_list.size(); i++)
    {
        // 获取路径
        QString namepath = folder_list.at(i).absoluteFilePath();
        QFileInfo folderinfo= folder_list.at(i);
        // 获取目录名
        QString name=folderinfo.fileName();
        QTreeWidgetItem* childroot = new QTreeWidgetItem(QStringList()<<name);
        childroot->setIcon(0,QIcon(":/images/folder.png"));
        childroot->setCheckState(1, Qt::Checked);
        // 将当前目录添加成path的子项
        root->addChild(childroot);
        // 进行递归
        QFileInfoList child_file_list = AddReportDirallfile(childroot,namepath);
        file_list.append(child_file_list);
        file_list.append(name);
    }
    return file_list;
}

/*!
\section　 判断hItem 节点是否为htrDestParent子节点?(包括自己本身)
*/
bool CReport::isItemInChildren(QTreeWidgetItem *hItem,QTreeWidgetItem *htrDestParent)
{
    if(hItem && htrDestParent)
    {
        do
        {
            if (hItem == htrDestParent)
                break;
        }
        while ((hItem = hItem->parent())!= NULL);
        return (hItem != NULL);
    }
    return false;
}

/*!
\section　 鼠标右键单击显示操作报表目录菜单
*/
void CReport::ShowNCReportDirMenu(const QPoint &pos)
{
    Q_UNUSED(pos);
    if(!m_pNCReportDirPopupMenu)
    {
        m_pNCReportDirPopupMenu = new QMenu(tr("新建"),treeWidget());

        CMDMultiLanguage::instance()->translateWidget(m_pNCReportDirPopupMenu);

        if(!m_NCReportDir_NewAction)
        {
            m_NCReportDir_NewAction = new QAction(tr("新建报表"),m_pNCReportDirPopupMenu);

            CMDMultiLanguage::instance()->translateWidget(m_NCReportDir_NewAction);

            connect(m_NCReportDir_NewAction,SIGNAL(triggered()),this,SLOT(slotitemActiveNewNCReportFile()));
        }
        // 添加报表右键编辑菜单
        m_pNCReportDirPopupMenu->addAction(m_NCReportDir_NewAction);
    }
    if(m_pNCReportDirPopupMenu)
    {
        m_pNCReportDirPopupMenu->exec(QCursor::pos());
    }
}

/*!
\section　 鼠标右键单击显示操作报表文件菜单
*/
void CReport::ShowNCReportFileMenu(const QPoint &pos)
{
    Q_UNUSED(pos);
    if(!m_pNCReportFilePopupMenu)
    {
        m_pNCReportFilePopupMenu = new QMenu(tr("新建"),treeWidget());

        if(!m_pNCReportFile_OpenAction)
        {
            m_pNCReportFile_OpenAction = new QAction(tr("打开"),m_pNCReportFilePopupMenu);

            CMDMultiLanguage::instance()->translateWidget(m_pNCReportFile_OpenAction);

            connect(m_pNCReportFile_OpenAction,SIGNAL(triggered()),this,SLOT(slotitemActiveOpenNCReportFile()));
        }
        if(!m_pNCReportFile_DeleteAction)
        {
            m_pNCReportFile_DeleteAction = new QAction(tr("删除"),m_pNCReportFilePopupMenu);

            CMDMultiLanguage::instance()->translateWidget(m_pNCReportFile_DeleteAction);

            connect(m_pNCReportFile_DeleteAction,SIGNAL(triggered()),this,SLOT(slotitemActiveDeleteNCReportFile()));
        }
        if(!m_pNCReportFile_RenameAction)
        {
            m_pNCReportFile_RenameAction = new QAction(tr("重命名"),m_pNCReportFilePopupMenu);

            CMDMultiLanguage::instance()->translateWidget(m_pNCReportFile_RenameAction);

            connect(m_pNCReportFile_RenameAction,SIGNAL(triggered()),this,SLOT(slotitemActiveRenameNCReportFile()));
        }
        // 添加报表文件右键编辑菜单
        m_pNCReportFilePopupMenu->addAction(m_pNCReportFile_OpenAction);
        m_pNCReportFilePopupMenu->addAction(m_pNCReportFile_DeleteAction);
        m_pNCReportFilePopupMenu->addAction(m_pNCReportFile_RenameAction);
    }
    if(m_pNCReportFilePopupMenu)
    {
        m_pNCReportFilePopupMenu->exec(QCursor::pos());
    }
}

/*!
\section　 报表目录节点下新建报表文件函数
*/
void CReport::NewNCRreportFile()
{
    QString absoluteFilePath = "";
    CMDProjectConfiger *pProjectConfiger = m_studioInterface->projectConfiger();
    if(pProjectConfiger)
    {
        QTreeWidget *pQTreeWidget = treeWidget();
        if(pQTreeWidget)
        {
            QTreeWidgetItem *pItem = pQTreeWidget->currentItem();
            if(pItem)
            {
                CNewNCReportFileDialog dlg(treeWidget());
                dlg.m_studioInterface = m_studioInterface;
                if(dlg.exec() == QDialog::Accepted)
                {
                    QString filename = dlg.filename();
                    absoluteFilePath = NCReportFilePath() + "/" + filename;
                    QTreeWidgetItem *pItem = new QTreeWidgetItem(QStringList(filename));
                    pItem->setIcon(0,QIcon(":/images/file.png"));
                    pItem->setCheckState(1, Qt::Checked);
                    m_ReportDirRoot->addChild(pItem);
                    if(m_pNCRDMainWindow)
                    {
                        m_pNCRDMainWindow->slotnewNCReportFile(absoluteFilePath);
                    }
                    else
                    {
                        m_pNCRDMainWindow = new NCRDMainWindow;
                        if(m_pNCRDMainWindow)
                        {
                            m_pNCRDMainWindow->setProjectConfiger(m_studioInterface->projectConfiger());
                            //<修复新建报表后 在视图数据源获取不到视图列表的BUG>
                            QString viewPath = m_studioInterface->projectConfiger()->path()+"/DBQuery";
                            m_pNCRDMainWindow->setViewPath(viewPath);
                            //</修复新建报表后 在视图数据源获取不到视图列表的BUG>

                            m_pNCRDMainWindow->setReport(this);
                            if(m_studioInterface)
                            {
                                m_studioInterface->openMDIWindow(m_pNCRDMainWindow);
                                m_pNCRDMainWindow->slotnewNCReportFile(absoluteFilePath);
                            }
                        }
                    }
                }
            }
        }
    }
}

/*!
\section　 打开当前节点报表文件函数
*/
void CReport::OpenCurrentNCRreportFile()
{
    QString absolutefilepath = "";

    QTreeWidget *pQTreeWidget = treeWidget();
    if(pQTreeWidget)
    {
        QTreeWidgetItem *p_Item = pQTreeWidget->currentItem();

        if(p_Item->data(0,Qt::DisplayRole).toString().endsWith(".ncr",Qt::CaseSensitive) || p_Item->data(0,Qt::DisplayRole).toString().endsWith(".xml",Qt::CaseSensitive))
        {
            QString currentitem_name = p_Item->data(0,Qt::DisplayRole).toString();
            qDebug()<<"p_Item          "<<currentitem_name;
            absolutefilepath = m_NCReportFilePath + "/" + currentitem_name;
            qDebug()<<"absolutfilepath          "<<absolutefilepath;

            if(m_pNCRDMainWindow)
            {
                m_pNCRDMainWindow->slotopenNCReportFile(absolutefilepath);
            }
        }
    }
}

/*!
\section　 删除当前节点报表文件函数
*/
void CReport::DeleteCurrentNCRreportFile()
{
    QString absolutefilepath = "";
    QTreeWidget *pQTreeWidget = treeWidget();
    if(pQTreeWidget)
    {
        QTreeWidgetItem *p_Item = pQTreeWidget->currentItem(); // 获得当前节点

        if(p_Item->data(0,Qt::DisplayRole).toString().endsWith(".ncr",Qt::CaseSensitive) || p_Item->data(0,Qt::DisplayRole).toString().endsWith(".xml",Qt::CaseSensitive))
        {
            QString currentitem_name = p_Item->data(0,Qt::DisplayRole).toString();

            absolutefilepath = m_NCReportFilePath + "/" + currentitem_name;

            QString content=p_Item->text(0);//获取当前行的名字
            QString str1 = QString(tr("确认要删除报表"));
            QString str2 = content;
            QString str3 = QString(tr("吗?"));
            QString questionString = str1 + str2 + str3;
#if 0
            QString questionString = QString(tr("确认要删除报表%1吗？")).arg(content);
            QMessageBox::StandardButton rb = QMessageBox::question(treeWidget(),  tr("删除!"), tr("确认要删除报表%1吗？").arg(content), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
#endif
            //////////////////////////////////////////////////////////////
            QMessageBox box;
            questionString = CMDMultiLanguage::instance()->value(str1)+str2+CMDMultiLanguage::instance()->value(str3);
            box.setText(questionString);
            box.setWindowTitle(tr("询问!"));
            box.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
            CMDMultiLanguage::instance()->translateWidget(&box);
            //////////////////////////////////////////////////////////////

            if(box.exec() == QMessageBox::Yes)
            {

                QFileInfo fileinfo;
                fileinfo.setFile(m_NCReportFilePath,currentitem_name);
                if(!fileinfo.exists())
                {
                    return;
                }
                QFile file;
                file.setFileName(absolutefilepath);

                if(file.isOpen())
                {
                    QString strError = QString(tr("报表文件已经打开,禁止删除!"));
#if 0
                    QMessageBox::warning(treeWidget(), tr("警告!"), tr(" 报表文件已经打开,禁止删除!"), QMessageBox::Yes);
#endif
                    //////////////////////////////////////////////////////////////
                    QMessageBox box;
                    box.setText(strError);
                    box.setWindowTitle(tr("警告!"));
                    box.setStandardButtons(QMessageBox::Ok);
                    CMDMultiLanguage::instance()->translateWidget(&box);
                    box.exec();
                    //////////////////////////////////////////////////////////////


                    return;
                }

                delete p_Item->parent()->takeChild(pQTreeWidget->currentIndex().row());
                if(m_pNCRDMainWindow)
                {
                    m_pNCRDMainWindow->CloseAllSubWindow();
                }
                if(file.remove(absolutefilepath))
                {
                    qDebug()<<"remove XMLfile succed!";
                }
                else
                {
                    qDebug()<<"remove XMLfile failed!";
                }
                return;
            }
        }
    }
}

void CReport::RenameCurrentNCRreportFile()
{
    QString oldabsolutefilepath = "";
    QString newabsolutefilepath = "";

    CMDProjectConfiger *pProjectConfiger = m_studioInterface->projectConfiger();
    if(pProjectConfiger)
    {
        QTreeWidget *pQTreeWidget = treeWidget();
        if(pQTreeWidget)
        {
            QTreeWidgetItem *p_Item = pQTreeWidget->currentItem();
            m_oldname = p_Item->text(0);

            if(p_Item)
            {
                CRenameNCReportFileDilaog dlg;
                dlg.m_studioInterface = m_studioInterface;
                if(dlg.exec() == QDialog::Accepted)
                {
                    m_newname = dlg.filename();


                    oldabsolutefilepath = m_NCReportFilePath + QString("/") + m_oldname;
                    newabsolutefilepath = m_NCReportFilePath + QString("/") + m_newname;

                    if(m_newname != m_oldname)
                    {
                        p_Item->setText(0,m_newname);

                        if(m_pNCRDMainWindow)
                        {
                            m_pNCRDMainWindow->CloseAllSubWindow();
                        }

                        if(QFile::rename(oldabsolutefilepath,newabsolutefilepath))
                        {
                            qDebug()<<"rename xmlfile succeed!";
                        }
#ifdef Q_OS_LINUX
                        //        QString rename_string ="";
                        //        rename_string = QString("%1 %2 %3").arg("mv").arg(oldabsolutefilepath).arg(newabsolutefilepath);
                        //        QProcess::execute(rename_string);
#endif
                    }
                    else
                    {
                        p_Item->setText(0,m_oldname);
                        QString strError = QString(tr("重命名报表文件名格式错误!\n报表%1重命名失败!")).arg(m_oldname);
#if 0
                        QMessageBox *message = new QMessageBox();
                        message->warning(treeWidget(),tr("警告!"),strError);
                        message->addButton(tr("确认"),QMessageBox::YesRole);
#endif
                        //////////////////////////////////////////////////////////////
                        QMessageBox box;
                        box.setText(strError);
                        box.setWindowTitle(tr("警告!"));
                        box.setStandardButtons(QMessageBox::Ok);
                        CMDMultiLanguage::instance()->translateWidget(&box);
                        box.exec();
                        //////////////////////////////////////////////////////////////

                        return;
                    }

                }
            }
        }
    }
}

bool CReport::hascurrentXMLFile(QString xmlName, QString Path)
{
    QDir dir(Path);
    if (!dir.exists())
    {
        return false;
    }
    dir.setFilter(QDir::Dirs|QDir::Files);
    dir.setSorting(QDir::Time |QDir::Reversed);
    // 排序方式 修改时间从小到大
    QFileInfoList list = dir.entryInfoList();
    int i=0;
    do{ QFileInfo fileInfo = list.at(i);
        if(fileInfo.fileName() == "." )
        {
            i++;
            continue;
        }
        bool bisFile=fileInfo.isFile();
        if(bisFile)
        {
            QString currentFileName=fileInfo.fileName();
            if(currentFileName == xmlName)
            {
                isexists_currentfile = true;
                qDebug()<<"isexists_currentfile = true;";
            }
        }
        i++;
    }
    while(i<list.size());
    return isexists_currentfile;
}

void CReport::changeLanguage()
{
    if(m_ReportDirRoot)
    {
        QString strText = tr("报表");
        m_ReportDirRoot->setText(0,CMDMultiLanguage::instance()->value(strText));
    }
    if(m_transTool)
    {
        m_transTool->changeLanguage();
    }
    if(m_pNCRDMainWindow)
    {
        m_pNCRDMainWindow->changeLanguage();
    }
}
