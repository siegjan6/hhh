/*!
\author emhhbmdsZQ==
\section  common类供所有配置类调用其静态函数和变量。
*/
#ifndef CCOMMON_H
#define CCOMMON_H
#include <QString>
#include <QColor>
#include <QSlider>
#include <QLineEdit>
#include <QPushButton>
class Ccommon
{
public:
    Ccommon();
    ~Ccommon();
public:
    static void setSlider(QSlider* slider, qint32 value);
    static void setEdit(QLineEdit* lineedit,qreal value);
    static void setEdit(QLineEdit* lineedit,qint32 value);
    static void editIntWarning(QLineEdit* edit, QString WarningStr, qint32 max, qint32 min);
    static void editDoubleWarning(QLineEdit* edit, QString WarningStr, qreal max, qreal min);
};

#endif // CCOMMON_H
