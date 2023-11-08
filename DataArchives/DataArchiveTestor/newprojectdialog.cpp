#include "newprojectdialog.h"
#include "ui_newprojectdialog.h"
#include <QFileDialog>
#include <QDir>

NewProjectDialog::NewProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewProjectDialog)
{
    ui->setupUi(this);
}

NewProjectDialog::~NewProjectDialog()
{
    delete ui;
}

QString NewProjectDialog::GetProjectPath()
{
    return m_ProjectPath;
}

void NewProjectDialog::on_browserBtn_clicked()
{
    QString fullPath = QFileDialog::getExistingDirectory(this, tr("浏览"),
                                                    "",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    if(fullPath.isEmpty())
        return ;

    ui->pathLineEdit->setText(fullPath);
}

void NewProjectDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->button(QDialogButtonBox::Ok) != button)
        return ;

    if(ui->pathLineEdit->text().isEmpty() || ui->nameLineEdit->text().isEmpty())
        return ;

    m_ProjectPath = ui->pathLineEdit->text() +"/"+ ui->nameLineEdit->text();
    QDir dir(m_ProjectPath);
    if(!dir.exists())
    {
        if(!dir.mkdir(m_ProjectPath))
        {
            m_ProjectPath.clear();
            return ; //TODO:Log
        }
    }
}
