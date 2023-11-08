#include <QMessageBox>
#include <QTranslator>
#include "Ccommon.h"
#include "MDMultiLanguage.h"

Ccommon::Ccommon()
{
}
Ccommon::~Ccommon()
{
}

void Ccommon::setSlider(QSlider *slider, qint32 value)
{
    slider->setValue(value);
}

void Ccommon::setEdit(QLineEdit *lineedit, qreal value)
{
    QString str;
    str.setNum(value);
    lineedit->setText(str);
}

void Ccommon::setEdit(QLineEdit *lineedit, qint32 value)
{
    QString str;
    str.setNum(value);
    lineedit->setText(str);
}

void Ccommon::editDoubleWarning(QLineEdit *edit, QString WarningStr, qreal max, qreal min)
{
    if(edit->text().toDouble()>max||edit->text().toDouble()<min)
    {
        //        QMessageBox msgBox(QMessageBox::Warning,QObject::tr( "警告"),
        //                             WarningStr);
        //         msgBox.addButton(QObject::tr("确定"), QMessageBox::AcceptRole);
        //         msgBox.exec();
        //         edit->setFocus();
        QMessageBox box ;
        box.setIcon(QMessageBox::Warning);
        box.setText(WarningStr);
        box.setWindowTitle(QObject::tr("警告"));
        QPushButton* button = new QPushButton;
        button->setText(QObject::tr("确定"));
        box.addButton(button,QMessageBox::AcceptRole);
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();
        edit->setFocus();
        delete button;
    }
}
void Ccommon::editIntWarning(QLineEdit* edit, QString WarningStr,qint32 max,qint32 min)
{
    if(edit->text().toInt()>max||edit->text().toInt()<min)
    {
//        QMessageBox msgBox(QMessageBox::Warning, QObject::tr("警告"),
//                             WarningStr);
//         msgBox.addButton(QObject::tr("确定"), QMessageBox::AcceptRole);
//         msgBox.exec();
//         edit->setFocus();
        QMessageBox box ;
        box.setIcon(QMessageBox::Warning);
        box.setText(WarningStr);
        box.setWindowTitle(QObject::tr("警告"));
        QPushButton* button = new QPushButton;
        button->setText(QObject::tr("确定"));
        box.addButton(button,QMessageBox::AcceptRole);
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();
        edit->setFocus();
        delete button;
    }
}


