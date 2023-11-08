/*!
\author emhhbmdsZQ==
\section  paintButton类,对配置界面上pushButton的操作在此类中进行.
*/
#ifndef PAINTBUTTON_H
#define PAINTBUTTON_H
#include <QColor>
#include <QPushButton>
#include "DrawDefine.h"

class CPaintButton
{
public:
    CPaintButton();
    ~CPaintButton();
public:
    QPushButton* m_button;
    QDialog* m_parentDialog;
    // !按钮背景色
    QColor m_backColor;
    //!按钮上贴图类型
    styleType m_style;
    paintType m_type;
    qint32 m_index;
public:
     void buttonClicked();
     void setButtonColor(QPushButton* button, QColor color, paintType type);
     void setButtonImage(QPushButton*button, styleType style, qint32 index, paintType type);
private:
     void setIndex(qint32 index);
     void setStyle(styleType style);
     void setType(paintType type);
     void setColor(QColor color);
 public:
         void selectItem();
          qint32 getIndex();
          QColor getColor();
          void setParentDialog(QDialog* dialog);
};

#endif // PAINTBUTTON_H
