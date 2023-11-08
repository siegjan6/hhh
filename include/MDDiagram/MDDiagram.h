#ifndef CMDDIAGRAM_H
#define CMDDIAGRAM_H

#include <QtCore/qglobal.h>
#include <QWidget>
#include "IDrawEnvironment.h"
class CDrawWidget;

#if defined(MDDIAGRAM_LIBRARY)
#  define MDDIAGRAM_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDIAGRAM_SHARED_EXPORT Q_DECL_IMPORT
#endif

class MDDIAGRAM_SHARED_EXPORT CMDDiagram
{
public:
    CMDDiagram();
    ~CMDDiagram();

public:
    QWidget* widget();
    CMDDiagram &operator = (const CMDDiagram & other);
    //! 序列化
public:
    virtual void serialize(QJsonObject& json);
    virtual void deserialize(const QJsonObject& json);
    //! 设置大小
    void setRect(const QRectF &value);
    //! 配置界面
    bool openDesign();
    //! 双击打开配置界面
    void onDoubleClick();
private:
    CDrawWidget *m_widget;
 public:
    //! 脚本
    void setString1(const QString &str);
    void setString2(const QString &str);
    void setString3(const QString &str);
    void setValue(const QString &str);
};

#endif // CMDDIAGRAM_H
