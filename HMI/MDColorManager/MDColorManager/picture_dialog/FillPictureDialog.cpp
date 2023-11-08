#include <QFileDialog>
#include <QPainter>
#include <QMessageBox>
#include <QFile>
#include "FillPictureDialog.h"
#include "ui_FillPictureDialog.h"
#include "MDMultiLanguage.h"
#include <QDebug>
#include <QButtonGroup>
CFillPictureDialog::CFillPictureDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CFillPictureDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();
    QButtonGroup *group1 = new QButtonGroup(this);
    group1->addButton(ui->tileRadioButton);
    group1->addButton(ui->centerRadioButton);
    group1->addButton(ui->stretchRadioButton);
    ui->tileRadioButton->setChecked(true);
    connect(ui->pathLineEdit,SIGNAL(textChanged(QString)),this,SLOT(resetPath(QString)));

    QStringList types;
    types << "QLineEdit";
    CMDMultiLanguage::instance()->translateWidget(this, &types);
}

CFillPictureDialog::~CFillPictureDialog()
{
    delete ui;
}

void CFillPictureDialog::inintDialog()
{
    m_pathName = m_projectPath+m_imageName;
    ui->pathLineEdit->setText(m_imageName);
    m_image.load(m_pathName);
    switch(m_position)
    {
    case 0:
    {
        ui->tileRadioButton->setChecked(true);
        on_tileRadioButton_clicked();
    }

        break;
    case 1:
    {
        ui->centerRadioButton->setChecked(true);
        on_centerRadioButton_clicked();
    }

        break;
    case 2:
    {
        ui->stretchRadioButton->setChecked(true);
        on_stretchRadioButton_clicked();
    }
        break;
    default:
        break;
    }
}

void CFillPictureDialog::setCSDisable()
{
    ui->centerRadioButton->setDisabled(true);
    ui->stretchRadioButton->setDisabled(true);
}

void CFillPictureDialog::tileSetting(QPixmap pix, bool fillBack)
{
    ui->previewLabel->clear();
    ui->previewLabel->setStyleSheet("background-image:none");
    QPalette palette;
    palette.setBrush(QPalette::Background,pix);
    ui->previewLabel->setPalette(palette);
    ui->previewLabel->setAutoFillBackground(fillBack);
}

//void CFillPictureDialog::createFile(const QString &str)
//{
//    QDir *temp = new QDir;
//    bool exist = temp->exists(str);
//    if(!exist)
//    {
//        temp->mkdir(str);
//    }
//    delete temp;
//}

bool CFillPictureDialog::copyFileToPath(QString sourceDir, QString toDir)
{
    toDir.replace("\\","/");
     if (sourceDir == toDir)
     {
       return true;
     }
     if (!QFile::exists(sourceDir))
     {
       return false;
     }
     QDir *createfile = new QDir;
     bool exist = createfile->exists(toDir);
     if (exist)
     {
         QMessageBox box ;
         box.setIcon(QMessageBox::Warning);
         box.setText(tr("文件已存在是否替换?"));
         box.setWindowTitle(tr("保存图片"));
         box.setStandardButtons(QMessageBox::Yes|QMessageBox::No);

         CMDMultiLanguage::instance()->translateWidget(&box);
         if(box.exec() == QMessageBox::Yes)
         {
              createfile->remove(toDir);
         }
     }

     if(!QFile::copy(sourceDir, toDir))
     {
       return false;
     }
     return true;
}

void CFillPictureDialog::paintEvent(QPaintEvent *)
{
    QPainter paint(this);
    paint.drawRect(7,5,375,170);

    if(m_imageName.isEmpty()|| ui->pathLineEdit->text().isEmpty())
    {
        paint.setPen(Qt::NoPen);
        paint.setBrush(QColor(0,0,0,200));
        paint.drawRect(ui->previewLabel->x(),ui->previewLabel->y(),
                        ui->previewLabel->width(),ui->previewLabel->height());
    }
}

void CFillPictureDialog::resetPath(const QString &str)
{
    m_image.load(m_projectPath/*+"/"*/+str);
    switch(m_position)
    {
    case 0:
    {
        ui->tileRadioButton->setChecked(true);
        tileSetting(QPixmap::fromImage(m_image),true);
    }
        break;
    case 1:
    {
        ui->centerRadioButton->setChecked(true);
        ui->previewLabel->clear();
        ui->previewLabel->setStyleSheet("background-image:none");
        ui->previewLabel->setPixmap(QPixmap::fromImage(m_image));
        ui->previewLabel->setAlignment(Qt::AlignCenter);
    }
        break;
    case 2:
    {
        ui->stretchRadioButton->setChecked(true);
        m_image=m_image.scaled(ui->previewLabel->size(),   Qt::IgnoreAspectRatio);
        ui->previewLabel->setPixmap(QPixmap::fromImage(m_image));
    }
        break;
    default:
        break;
    }
}

void CFillPictureDialog::on_choseBtn_clicked()
{
    QString fileFull, fileName, filePath;
    QFileInfo fileInfo;
    QFileDialog fileDialog(this);
    if(fileDialog.exec() == QFileDialog::Accepted)
    {
        fileFull = fileDialog.selectedFiles().at(0);
        fileInfo = QFileInfo(fileFull);
        fileName = fileInfo.fileName();
        filePath = fileInfo.absolutePath();
        ui->pathLineEdit->setText(fileName);
//        m_pathName = filePath+"\/"+fileName;
        m_pathName = fileFull;
        m_image.load(m_pathName);
        QString styeString;
        styeString = "background-image:url("+m_pathName+")";
        ui->previewLabel->setStyleSheet(styeString);
        QString projectPath = m_projectPath+fileName;

        copyFileToPath(m_pathName,projectPath);
        setImageName(fileName);
        m_pathName = projectPath;
        if(m_position == 1)
        {
            ui->previewLabel->clear();
            ui->previewLabel->setStyleSheet("background-image:none");
            QPalette palette;
            palette.setBrush(QPalette::Window, Qt::NoBrush);
            ui->previewLabel->setPalette(palette);
            ui->previewLabel->setPixmap(QPixmap::fromImage(m_image));
            ui->previewLabel->setAlignment(Qt::AlignCenter);
        }
    }
}

void CFillPictureDialog::on_tileRadioButton_clicked()
{
    m_position = 0;
    ui->previewLabel->clear();
    ui->previewLabel->setStyleSheet("background-image:none");
    tileSetting(QPixmap::fromImage(m_image),true);
}

void CFillPictureDialog::on_centerRadioButton_clicked()
{
    m_position = 1;
    ui->previewLabel->clear();
    ui->previewLabel->setStyleSheet("background-image:none");
    QPalette palette;
    palette.setBrush(QPalette::Window, Qt::NoBrush);
    ui->previewLabel->setPalette(palette);
    ui->previewLabel->setPixmap(QPixmap::fromImage(m_image));
    ui->previewLabel->setAlignment(Qt::AlignCenter);
}

void CFillPictureDialog::on_stretchRadioButton_clicked()
{
    m_position = 2;
    ui->previewLabel->clear();
    ui->previewLabel->setStyleSheet("background-image:none");
    QImage* imgScaled = new QImage;
    *imgScaled=m_image.scaled(ui->previewLabel->size(), Qt::IgnoreAspectRatio);
    ui->previewLabel->setPixmap(QPixmap::fromImage(*imgScaled));
    delete imgScaled;
}

void CFillPictureDialog::setImageName(const QString &imageName)
{
    m_imageName = imageName;
}

QString CFillPictureDialog::imageName()
{
    return m_imageName;
}

void CFillPictureDialog::setProjectPath(const QString &proPath)
{
    m_projectPath = proPath;
}

QString CFillPictureDialog::projectPath() const
{
    return m_projectPath;
}

