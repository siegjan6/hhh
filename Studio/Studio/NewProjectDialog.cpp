#include <QMessageBox>
#include <QTranslator>
#include <QDateTime>
#include <qglobal.h>

#include "NewProjectDialog.h"
#include "ui_NewProjectDialog.h"
#include "Utility.h"

CNewProjectDialog::CNewProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewProjectDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowCloseButtonHint);

    QString path = "/";
#ifdef Q_OS_WIN
    path = "C:" + path;
#endif
    ui->ProjectPathEdit->setText(path);
}
CNewProjectDialog::~CNewProjectDialog()
{
    delete ui;
}
void CNewProjectDialog::on_OkButton_clicked()
{
    QString name = ui->ProjectNameEdit->text().trimmed();
    QString path = ui->ProjectPathEdit->text().trimmed();
    if (name.isEmpty())
    {
        QMessageBox::warning(this,tr("警告"),tr("请输入项目名称"));
        ui->ProjectNameEdit->setFocus();
        return;
    }
    if (path.isEmpty())
    {
        QMessageBox::warning(this,tr("警告"),tr("请输入项目路径"));
        ui->ProjectNameEdit->setFocus();
        return;
    }

    m_description = ui->CommentEdit->toPlainText();
    if(!path.endsWith("/"))
        path = path + "/";
    m_projectPath = path + name;
    m_projectName = name;
    if(name.endsWith(CMDProjectConfiger::projectFileExt()))
        m_projectName = name.left(name.length()-5);

    accept();
}
void CNewProjectDialog::on_CancelButton_clicked()
{
    reject();
}
void CNewProjectDialog::on_BrowseButton_clicked()
{
    QString directory = QFileDialog::getExistingDirectory(this,tr("目录"),ui->ProjectPathEdit->text(),
        QFileDialog::ShowDirsOnly  | QFileDialog::DontResolveSymlinks);

    if(!directory.isEmpty())
        ui->ProjectPathEdit->setText(directory);
}
