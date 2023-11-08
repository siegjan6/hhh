/*!
\author Y2hhaWhq

\section 功能

控件脚本基类
*/
#ifndef MDDRAWWIDGETWRAPPER_H
#define MDDRAWWIDGETWRAPPER_H

#include <QVariant>
#include <QRgb>
#include "MDDrawObjWrapper.h"

class CMDDrawBasicWidget;


class MDHMIFORMSHARED_EXPORT CMDDrawWidgetWrapper : public CMDDrawObjWrapper
{
    Q_OBJECT

    Q_PROPERTY(QString fontName READ fontName WRITE setFontName)
    Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize)
    Q_PROPERTY(bool fontBold READ fontBold WRITE setFontBold)
    Q_PROPERTY(bool fontItalic READ fontItalic WRITE setFontItalic)
    Q_PROPERTY(bool fontUnderline READ fontUnderline WRITE setFontUnderline)
    Q_PROPERTY(bool fontStrikeOut READ fontStrikeOut WRITE setFontStrikeOut)
    Q_PROPERTY(int backColor READ backColor WRITE setBackColor)
    Q_PROPERTY(int textColor READ textColor WRITE setTextColor)

public:
    CMDDrawWidgetWrapper(void* Object, const QString& name);
    virtual ~CMDDrawWidgetWrapper(){}

//property
public Q_SLOTS:
    //!  字体名称
    QString fontName();
    void  setFontName(const QString &name);
    //!  字体大小
    int fontSize();
    void setFontSize(int size);
    //!  粗体
    bool fontBold();
    void setFontBold(bool bold);
    //!  斜体
    bool fontItalic();
    void setFontItalic(bool italic);
    //!  下划线
    bool fontUnderline();
    void setFontUnderline(bool underLine);
    //!  删除线
    bool fontStrikeOut();
    void setFontStrikeOut (bool strikeOut);
    //!  背景色
    int backColor();
    void setBackColor(int rgba);
    //!  字体色
    int textColor();
    void setTextColor(int rgba);
public:
    CMDDrawBasicWidget* object();
};

#endif // MDDRAWWIDGETWRAPPER_H
