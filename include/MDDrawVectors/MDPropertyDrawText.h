/*!
\author aGRw
\section 文本控件属性类
*/
#ifndef MDPROPERTYDRAWTEXT_H
#define MDPROPERTYDRAWTEXT_H

#include "MDPropertyDrawVector.h"
#include "MDDrawText.h"

class MDDRAWVECTORSSHARED_EXPORT CMDPropertyDrawText : public CMDPropertyDrawVector
{
public:
    Q_OBJECT

    Q_ENUMS(PropertyHAlignment)
    Q_ENUMS(PropertyVAlignment)

    Q_PROPERTY(QString GUID_Appearance)
    Q_PROPERTY(QString DILtextBrush READ textBrush WRITE setTextBrush)
    Q_PROPERTY(QFont font READ font WRITE setFont)
    Q_PROPERTY(PropertyHAlignment hAlignment READ hAlignment WRITE setHAlignment)
    Q_PROPERTY(PropertyVAlignment vAlignment READ vAlignment WRITE setVAlignment)
    Q_PROPERTY(bool lineWrap READ lineWrap WRITE setLineWrap)
    Q_PROPERTY(QString text READ text WRITE setText)
    Q_PROPERTY(QString DILtextFormat READ textFormat WRITE setTextFormat)
//////////////////////////////////////////////////////////////////////////
public:
    CMDPropertyDrawText(CMDDrawObj* parent);
    virtual ~CMDPropertyDrawText(){}
/*!
\section enum
*/
public:
    enum class PropertyHAlignment
    {Left = 0x0001,
        Center = 0x0004,
        Right = 0x0002};
    enum class PropertyVAlignment
    {Top = 0x0020,
        Middle = 0x0080,
        Bottom = 0x0040};
/*!
\section property
*/
public:
    void setText(const QString& value);
    QString text();
    void setTextBrush(const QString& value);
    QString textBrush();
    void setFont(const QFont& value);
    QFont font();
    void setHAlignment(PropertyHAlignment value);
    PropertyHAlignment hAlignment();
    void setVAlignment(PropertyVAlignment value);
    PropertyVAlignment vAlignment();
    void setLineWrap(bool value);
    bool lineWrap();
    void setTextFormat(const QString& value);
    QString textFormat();
/*!
\section private function
*/
private:
    CMDDrawText* object();
/*!
\section batch edit
*/
private:
    void setBatchText(const QString& value);
    void setBatchTextBrush(CMDBrushManager* value);
    void setBatchFont(const QFont& value);
    void setBatchHAlignment(PropertyHAlignment value);
    void setBatchVAlignment(PropertyVAlignment value);
    void setBatchLineWrap(bool value);
    void setBatchTextFormat(QJsonObject* value);
};

#endif // MDPROPERTYDRAWTEXT_H
