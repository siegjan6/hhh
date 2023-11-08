/*!
\author bWVpeGw=

\section 功能

编辑框控件按属性类
*/
#ifndef MDPROPERTYDRAWTEXTEDIT_H
#define MDPROPERTYDRAWTEXTEDIT_H

#include "MDPropertyDrawObj.h"
#include "MDDrawTextEdit.h"
#include "MDPropertyDrawBasicWidget.h"
class MDDRAWWIDGETSSHARED_EXPORT CMDPropertyDrawTextEdit : public CMDPropertyDrawBasicWidget
{
    Q_OBJECT

    Q_ENUMS(PropertyAlignment)
    Q_ENUMS(PropertyBorderStyle)

    Q_PROPERTY(QString GUID_Action)
    Q_PROPERTY(bool readOnly READ readOnly WRITE setReadOnly)
    Q_PROPERTY(bool writeVarEnable READ writeVarEnable WRITE setWriteVarEnable)
    Q_PROPERTY(bool enterSetVarEnable READ enterSetVarEnable WRITE setEnterSetVarEnable)
    Q_PROPERTY(QString GUID_Appearance)
    Q_PROPERTY(bool lineWrap READ lineWrap WRITE setLineWrap)
    Q_PROPERTY(PropertyBorderStyle borderStyle READ borderStyle WRITE setBorderStyle)
    Q_PROPERTY(bool horizontalScrollBar READ horizontalScrollBar WRITE setHorizontalScrollBar)
    Q_PROPERTY(bool verticalScrollBar READ verticalScrollBar WRITE setVerticalScrollBar)
    Q_PROPERTY(PropertyAlignment alignment READ alignment WRITE setAlignment)
    Q_PROPERTY(QString DILtext READ DILtext WRITE setDILtext)
    Q_PROPERTY(bool hasPassword READ hasPassword WRITE setHasPassword)
    Q_PROPERTY(QString GUID_Variable)
    Q_PROPERTY(QString DILtextFormat READ textFormat WRITE setTextFormat)


public:
    //! 对齐方式
    enum class PropertyAlignment {AlignLeft,AlignCenter,AlignRight};
    //! 边框样式
    enum class PropertyBorderStyle {NoFrame,StyledPanel,WinPanel};

/*!
\section 属性函数
*/
public:
    //! 水平滚动条
    void setHorizontalScrollBar(bool value);
    bool horizontalScrollBar();
    //! 垂直滚动条
    void setVerticalScrollBar(bool value);
    bool verticalScrollBar();
    //! 边框样式
    void setBorderStyle(PropertyBorderStyle value);
    PropertyBorderStyle borderStyle();
    //! 换行模式
    void setLineWrap(bool value);
    bool lineWrap();
    //! 只读
    void setReadOnly(bool value);
    bool readOnly();
    //! 文本对齐方式
    void setAlignment(PropertyAlignment value);
    PropertyAlignment alignment();
    //! 文本
    void setDILtext(const QString& value);
    QString DILtext();
    //! 字符串格式
    void setTextFormat(const QString& value);
    QString textFormat();
    //! 控制点，是否可写入变量
    void setWriteVarEnable(bool enable);
    bool writeVarEnable();
    //! 回车赋值
    void setEnterSetVarEnable(bool enable);
    bool enterSetVarEnable();
    //! 是否使用密码
    void setHasPassword(bool enable);
    bool hasPassword();

public:
    CMDPropertyDrawTextEdit(CMDDrawObj *parent);
    virtual ~CMDPropertyDrawTextEdit(){}

protected:
    CMDDrawTextEdit *widget();
/*!
\section 设置属性值
*/
public:
    void resetAttribute();

/*!
\section batch edit
*/
private:
    void setBatchHorizontalScrollBar(bool value);
    void setBatchVerticalScrollBar(bool value);
    void setBatchBorderStyle(PropertyBorderStyle value);
    void setBatchLineWrap(bool value);
    void setBatchReadOnly(bool value);
    void setBatchAlignment(PropertyAlignment value);
    void setBatchDILText(const QString& value);
    void setBatchTextFormat(QJsonObject *value);
    void setBatchWriteVarEnable(bool value);
    void setBatchEnterSetVarEnable(bool value);
    void setBatchHasPassword(bool value);

private:
    //! 换行符
    QString breakString() const
    {
#ifdef Q_OS_LINUX
        return {"\n"};
#elif defined(Q_OS_WIN)
        return {"\n"};
#endif
    }
};

#endif // MDPROPERTYDRAWTEXTEDIT_H

