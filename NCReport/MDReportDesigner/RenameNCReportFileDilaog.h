#ifndef RENAMENCREPORTFILEDILAOG_H
#define RENAMENCREPORTFILEDILAOG_H

#include <QDialog>
#include <QMessageBox>
#include <Report.h>
#include "MDProjectConfiger.h"
#include "IStudioInterface.h"

namespace Ui {
class RenameNCReportFileDilaog;
}

class CRenameNCReportFileDilaog : public QDialog
{
    Q_OBJECT

public:
    explicit CRenameNCReportFileDilaog(QWidget *parent = 0);
    ~CRenameNCReportFileDilaog();
    IStudioInterface *m_studioInterface;
public:
    //! 报表文件名
    void setFilename(const QString &filename);
    QString filename() const;

/*!
\section 信号与槽函数，槽函数主要是响应确认与取消按钮相关操作
*/
private slots:
    void on_OkButton_clicked();
    void on_CancelButton_clicked();

private:
    /*!
     判断当前文件目录下是否含有与所要重命名的报表文件名相同的文件

     \param    1. QString xmlName 需要新建的报表文件名
                      2. QString Path  报表文件的存储目录
     \returns   bool
     */
    bool hascurrentXMLFile(QString xmlName,QString Path);

private:
    Ui::RenameNCReportFileDilaog *ui;

    CReport * m_report;
    bool isexists_currentfile;

private:
    QString m_filename;
};

#endif // RENAMENCREPORTFILEDILAOG_H
