/*!

\author eHVr

\section 功能
    界面功能函数类

    包含：MessageBox弹窗，按钮颜色设置，颜色对话框调用，字体对话框调用。整合了多语言功能
*/
#ifndef CFORMTOOLFUNC_H
#define CFORMTOOLFUNC_H
#include <QMessageBox>
#include <QColor>
#include <QColorDialog>
#include <QFontDialog>
#include <QPushButton>

#include "CTranslator.h"

namespace FormToolFunc {

inline int  showMessage(const QString & comment,QWidget *parent = 0,
            QMessageBox::StandardButtons buttons = QMessageBox::Ok)
{
    QMessageBox dialog(parent);
    dialog.setWindowTitle(QObject::tr("警告"));
    dialog.setText(comment);
    dialog.setStandardButtons(buttons);
    Translator::installTranslatorWidget(&dialog);

    return dialog.exec();
}

inline void setButtonColor(const QColor &color, QPushButton &button)
{
    button.setStyleSheet(QString("background-color: rgb(%1, %2, %3)")
                         .arg(color.red())
                         .arg(color.green())
                         .arg(color.blue()));
}

inline int selectColor(QColor &initialColor, QPushButton &colorButton,QWidget *parent = 0)
{
    QColorDialog dialog(parent);
    dialog.setCurrentColor(initialColor);
    Translator::installTranslatorWidget(&dialog);
    int ret = dialog.exec();
    if(ret == QDialog::Accepted){
        QColor color = dialog.selectedColor();
        if(color.isValid()){
            setButtonColor(color,colorButton);
            initialColor = color;

            return QDialog::Accepted;
        }
    }
    return QDialog::Rejected;
}

inline int selectFont(QFont & initialFont,QWidget *parent = 0)
{
    QFontDialog dialog(initialFont,parent);
    Translator::installTranslatorWidget(&dialog);

    int ret = dialog.exec();
    if(ret == QDialog::Accepted)
    {
        initialFont = dialog.selectedFont();
    }
    return ret;
}
}
#endif // CFORMTOOLFUNC_H
