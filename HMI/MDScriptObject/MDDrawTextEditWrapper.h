/*!
\author Y2hhaWhq

\section 功能

编辑框控件脚本类
*/
#ifndef MDDRAWTEXTEDITWRAPPER_H
#define MDDRAWTEXTEDITWRAPPER_H

#include "MDDrawWidgetWrapper.h"
#include <QVariant>
class CMDDrawTextEdit;

class MDHMIFORMSHARED_EXPORT CMDDrawTextEditWrapper : public CMDDrawWidgetWrapper
{
    Q_OBJECT

    Q_PROPERTY(QString text READ text WRITE setText)
    Q_PROPERTY(bool readOnly READ readOnly WRITE setReadOnly)
    Q_PROPERTY(int alignment READ alignment WRITE setAlignment)
    Q_PROPERTY(int lineWrapMode READ lineWrapMode WRITE setLineWrapMode)
    Q_PROPERTY(int borderStyle READ borderStyle WRITE setBorderStyle)
    Q_PROPERTY(bool horizontalScrollBar READ horizontalScrollBar WRITE setHorizontalScrollBar)
    Q_PROPERTY(bool verticalScrollBar READ verticalScrollBar WRITE setVerticalScrollBar)
    Q_PROPERTY(int selectStart READ selectStart WRITE setSelectStart)
    Q_PROPERTY(int selectLength READ selectLength WRITE setSelectLength)
    Q_PROPERTY(QString selectText READ selectText WRITE setSelectText)

public:
    CMDDrawTextEditWrapper(void* object, const QString& name);
    virtual ~CMDDrawTextEditWrapper(){}

//property
public Q_SLOTS:
    //! 文本
    QString text();
    void setText(const QString& str);
    //!  只读
    bool readOnly();
    void setReadOnly(bool readOnly);
    //!  对齐方式
    int alignment();
    void setAlignment(int value);
    //!  换行模式
    int lineWrapMode();
    void setLineWrapMode(int value);
    //!  边框样式
    int borderStyle();
    void setBorderStyle(int value);
    //! 水平滚动条
    void setHorizontalScrollBar(bool value);
    bool horizontalScrollBar();
    //!  垂直滚动条
    void setVerticalScrollBar(bool value);
    bool verticalScrollBar();
    //!  选中文本起始位置
    int selectStart();
    void setSelectStart(int value);
    //!  选中文本长度
    int selectLength();
    void setSelectLength(int value);
    //!  选中文本
    QString selectText();
    void setSelectText(QString &str);
public:
    CMDDrawTextEdit* object();
};

#endif // MDDRAWTEXTEDITWRAPPER_H
