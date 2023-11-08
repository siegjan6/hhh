#include "NewViewDialog.h"
#include "ui_NewViewDialog.h"
#include "MDMultiLanguage.h"
#include <QMessageBox>
#include <QDir>
#include <QDebug>

NewViewDialog::NewViewDialog(QString title, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewViewDialog),
    m_windowTitle(title),
    m_viewName(""),
    m_view(new DBQ::View),
    m_studioInterface(NULL),
    isexists_currentfile(false)
{
    ui->setupUi(this);
    this->setWindowTitle(m_windowTitle);
    QWidget::setTabOrder(ui->lineEdit_viewName,ui->pbt_Confirm);
    QWidget::setTabOrder(ui->pbt_Confirm, ui->pbt_Cancel);
    QWidget::setTabOrder(ui->pbt_Cancel, ui->lineEdit_viewName);
}

NewViewDialog::~NewViewDialog()
{
    delete ui;
    if(m_view)
    {
        delete m_view;
        m_view = NULL;
    }
}

QString NewViewDialog::getViewName()
{
    return m_viewName;
}

void NewViewDialog::on_pbt_Confirm_clicked()
{
    m_viewName = ui->lineEdit_viewName->text();

    QRegExp newView_reg_exp;
    newView_reg_exp.setPattern("^[a-zA-Z0-9_\u4e00-\u9fa5\\w]{1,30}$");
    QRegExpValidator p_newView_validator(newView_reg_exp);

    if(p_newView_validator.regExp().exactMatch(m_viewName))
    {
        QString strError = "";

        if (m_viewName.isEmpty())
        {
            strError = "名称不能为空";
            QMessageBox box;
            box.setText(strError);
            box.setWindowTitle(tr("警告!"));
            box.setStandardButtons(QMessageBox::Ok);
            CMDMultiLanguage::instance()->translateWidget(&box);
            box.exec();
            ui->lineEdit_viewName->setFocus();
            return;
        }
        else
        {
            QString m_ViewFilePath = "";
            CMDProjectConfiger *pProjectConfiger = m_studioInterface->projectConfiger();
            if(pProjectConfiger)
            {
                m_ViewFilePath = pProjectConfiger->path()+"/"+"DBQuery";
                if(hascurrentXMLFile(m_viewName,m_ViewFilePath))
                {
                    QString str1 = QString(tr("视图文件"));
                    QString str2 = m_viewName;
                    QString str3 = QString(tr("已存在!"));
                    QString strError = str1 + str2 + str3;
                    QMessageBox box;
                    strError = CMDMultiLanguage::instance()->value(str1)+str2+CMDMultiLanguage::instance()->value(str3);
                    box.setText(strError);
                    box.setWindowTitle(tr("警告!"));
                    box.setStandardButtons(QMessageBox::Ok);
                    CMDMultiLanguage::instance()->translateWidget(&box);
                    box.exec();
                    ui->lineEdit_viewName->setFocus();
                    this->close();
                    return;
                }
                else
                {
                    QString errOut = "";
                    m_ViewFilePath =m_ViewFilePath + "/view_" + m_viewName + ".json";
                    m_view->viewName = m_viewName;
                    DBQ::View_Write(m_ViewFilePath,DBQ::FILE_TYPE_JSON,m_view,errOut);
                }
            }
        }

        this->accept();
    }
    else
    {
        QString strError = QString("视图文件名不正确!");
        QMessageBox box;
        box.setText(strError);
        box.setWindowTitle(tr("警告!"));
        box.setStandardButtons(QMessageBox::Ok);
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();
        //////////////////////////////////////////////////////////////////////

        ui->lineEdit_viewName->setFocus();
        this->close();
        return;
    }
}

void NewViewDialog::on_pbt_Cancel_clicked()
{
    this->reject();
}

bool NewViewDialog::hascurrentXMLFile(QString xmlName, QString Path)
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
IStudioInterface *NewViewDialog::studioInterface() const
{
    return m_studioInterface;
}

void NewViewDialog::setStudioInterface(IStudioInterface *studioInterface)
{
    m_studioInterface = studioInterface;
}


DBQ::View *NewViewDialog::getView() const
{
    return m_view;
}

void NewViewDialog::setView(DBQ::View *view)
{
    m_view = view;
}

