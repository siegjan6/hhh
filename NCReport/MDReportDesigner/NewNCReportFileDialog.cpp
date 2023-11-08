#include "NewNCReportFileDialog.h"
#include "ui_newncreportfiledialog.h"
#include <QDebug>

#include "MDMultiLanguage.h"

CNewNCReportFileDialog::CNewNCReportFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewNCReportFileDilaog)
{
    m_filename = "";
    m_studioInterface = NULL;
    m_report = NULL;
    isexists_currentfile = false;

    ui->setupUi(this);
    this->setWindowTitle(tr("新建报表"));
    QWidget::setTabOrder(ui->lineEdit,ui->OkButton);
    QWidget::setTabOrder(ui->OkButton, ui->CancelButton);
    QWidget::setTabOrder(ui->CancelButton, ui->lineEdit);

    CMDMultiLanguage::instance()->translateWidget(this);
}

CNewNCReportFileDialog::~CNewNCReportFileDialog()
{
    delete ui;
//    if(m_projectConfiger)
//    {
//        delete m_projectConfiger;
//        m_projectConfiger = NULL;
//    }
//    if(m_report)
//    {
//        delete m_report;
//        m_report = NULL;
//    }
}

/*!
\section 获取报表文件名
*/
QString CNewNCReportFileDialog::filename() const
{
    return m_filename;
}

/*!
\section 设置报表文件名
*/
void CNewNCReportFileDialog::setFilename(const QString &filename)
{
    m_filename = ui->lineEdit->text();
    m_filename = filename;
}

/*!
\section 槽函数，确认输入新建报表文件的文件名
*/
void CNewNCReportFileDialog::on_OkButton_clicked()
{
    m_filename = ui->lineEdit->text();

//    QRegExp newreport_reg_exp("[A-Za-z0-9_]{1,30}");
    QRegExp newreport_reg_exp;
    newreport_reg_exp.setPattern("^[a-zA-Z0-9_\u4e00-\u9fa5\\w]{1,30}$");
    QRegExpValidator p_newreport_validator(newreport_reg_exp);

      if(p_newreport_validator.regExp().exactMatch(m_filename))
      {
          if(!m_filename.endsWith(".XML",Qt::CaseInsensitive) || m_filename.endsWith(".NCR",Qt::CaseInsensitive))
          {
              m_filename += ".xml";
          }
          QString strError = "";

          if (m_filename.isEmpty())
          {
              strError = "名称不能为空";
#if 0
              QMessageBox *message = new QMessageBox(this);
              message->warning(this,tr("警告!"),strError);
              message->addButton(tr("确认"),QMessageBox::YesRole);        
#endif
              //////////////////////////////////////////////////////////////////////
              QMessageBox box;
              box.setText(strError);
              box.setWindowTitle(tr("警告!"));
              box.setStandardButtons(QMessageBox::Ok);
              CMDMultiLanguage::instance()->translateWidget(&box);
              box.exec();
              //////////////////////////////////////////////////////////////////////

              ui->lineEdit->setFocus();
              return;
          }
          else
          {
              QString m_NCReportFilePath = "";
              CMDProjectConfiger *pProjectConfiger = m_studioInterface->projectConfiger();
              m_report = new CReport;
              if(pProjectConfiger)
              {
                  m_NCReportFilePath = pProjectConfiger->path()+"/"+"Report";
                  if(hascurrentXMLFile(m_filename,m_NCReportFilePath))
                  {
                      QString str1 = QString(tr("对象"));
                      QString str2 = m_filename;
                      QString str3 = QString(tr("已存在!"));
                       QString strError = str1 + str2 + str3;

#if 0
                       strError = QString("对象%1已存在").arg(m_filename);
                      QMessageBox *message = new QMessageBox(this);
                      message->warning(this,tr("警告!"),strError);
                      message->addButton(tr("确认"),QMessageBox::YesRole);
#endif
                      //////////////////////////////////////////////////////////////////////
                      QMessageBox box;
                      strError = CMDMultiLanguage::instance()->value(str1)+str2+CMDMultiLanguage::instance()->value(str3);
                      box.setText(strError);
                      box.setWindowTitle(tr("警告!"));
                      box.setStandardButtons(QMessageBox::Ok);
                      CMDMultiLanguage::instance()->translateWidget(&box);
                      box.exec();
                      //////////////////////////////////////////////////////////////////////

                      ui->lineEdit->setFocus();
                      this->close();
                      return;
                  }
              }
          }
          QDialog::accept();
      }
      else
      {
          QString strError = QString("报表文件名不正确!");
#if 0
          QMessageBox *message = new QMessageBox(this);
          message->warning(this,tr("警告!"),strError);
          message->addButton(tr("确认"),QMessageBox::YesRole);
#endif
          //////////////////////////////////////////////////////////////////////
          QMessageBox box;
          box.setText(strError);
          box.setWindowTitle(tr("警告!"));
          box.setStandardButtons(QMessageBox::Ok);
          CMDMultiLanguage::instance()->translateWidget(&box);
          box.exec();
          //////////////////////////////////////////////////////////////////////

          ui->lineEdit->setFocus();
          this->close();
          return;
      }
}

/*!
\section 槽函数，取消输入新建报表文件的文件名
*/
void CNewNCReportFileDialog::on_CancelButton_clicked()
{
    QDialog::reject();
}

/*!
 \section　判断当前文件目录下是否含有与所要新建的报表文件名相同的文件
 */
bool CNewNCReportFileDialog::hascurrentXMLFile(QString xmlName,QString Path)
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
