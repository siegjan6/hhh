/*!

\author eHVr

\section 功能

    界面功能按钮.
    重载了 << >> 运算符用于序列化，实现克隆函数
    \a buttonPos
    \a isIcon
    \a buttonName
    \a buttonVisible
*/
#ifndef CTOOLBUTTON_H
#define CTOOLBUTTON_H

#include <QPushButton>
#include <QSize>

class CToolButton : public QPushButton
{
    Q_OBJECT
public:
    explicit CToolButton(QWidget *parent = 0);
    ~CToolButton(){}

    qint32 buttonPos() const;
    void setButtonPos(const qint32 &buttonPos);

    bool isIcon() const;
    void setIsIcon(bool isIcon);

    QString buttonName() const;
    void setButtonName(const QString &buttonName);

    void setButtonVisible(bool visible);
    bool buttonVisible() const;
public:
    CToolButton *clone();
private:
    bool     m_isIcon ;

    qint32   m_buttonPos;
    QString  m_buttonName;
    bool     m_visible;

    //! 序列化和反序列化
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
};

#endif // CTOOLBUTTON_H
