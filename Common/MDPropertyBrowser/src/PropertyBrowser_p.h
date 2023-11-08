/*!
\author eHVr
\section 功能
    导出类的私有类，包含属性和各种操作
\author Y2hhaWhq
\section  功能
     属性分组的选择性展开，状态存储，下拉项目的翻译，以及对象切换时保持相同选择项
*/
#ifndef  PROPERTYBROWSER_P_H
#define PROPERTYBROWSER_P_H

#include "../../include/MDPropertyBrowser/PropertyBrowser.h"
#include "qtpropertybrowser.h"
#include "qtvariantproperty.h"
#include "qtgroupboxpropertybrowser.h"
#include "qttreepropertybrowser.h"
#include "VariantManager.h"
#include "VariantFactory.h"

#include <QMetaObject>
#include <QMetaProperty>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QTextEdit>

#if QT_VERSION >= 0x040400
QT_BEGIN_NAMESPACE
#endif
const QStringList stringList;
class CPropertyBrowserPrivate
{
    CPropertyBrowser *q_ptr;
    Q_DECLARE_PUBLIC(CPropertyBrowser)
public:
    //预览元对象
    void previewClassProperties(const QMetaObject *metaObject);
    //获取需要隐藏的属性名称
    void getHideClassProperties(const QMetaObject *metaObject);
    //! 通过给定的元对象，获取其属性和值。
    void addClassProperties(const QMetaObject *metaObject);
    void updateClassProperties(const QMetaObject *metaObject, bool recursive);

    //! 将属性添加到Browser中，并显示
    void addPropertiesToBrowser();
    void classPropertyNotExist(const QMetaObject *metaObject, QString classProperty);
    void addNormalProperty(int type, QMetaProperty &metaProperty, QtVariantProperty *&subProperty, const QString &propertyName,const QString &markText);

    //! 扩展状态。
    void restoreExpandedState();
    void setProperty(const QString &name, const QVariant &value);
    //! 设置单个属性行界面隐藏
    void setPropertyHide(const QStringList &list, bool isHide);
    //! 设置属性行界面隐藏
    void setSinglePropertyHide(const QString &str, bool isHide);
    //! 只读属性设置背景色
    void setReadOnlyBackColor(const QColor &color);

    //! 指定属性的背景色
    void setPropertyBackColor(const QString &name,const QColor &color);
    //! 设置属性只读
    void setPropertyReadOnly(bool isReadOnly = true, const QStringList &strList = stringList);
    //! 动态添加枚举
    void resetEnums(const QString &name,const QStringList &list);
    //! 获取当前枚举值
    QString getCurrentEnumText(const QString &name);
    //! 属性框全展开
    void setExpandAll();
    //! 设置属性的tip
    void setPropertyTipMap(const QMap<QString, QVariant> &propertyTip);

    //! test
    void setWhatsThis(const QString &name);

    void setCurrentPropertyName(const QString &str);
    QString currentPropertyName() const;


public:
    void slotValueChanged(QtProperty *property, const QVariant &value);

public:
    int enumToInt(const QMetaEnum &metaEnum, int enumValue) const;
    int intToEnum(const QMetaEnum &metaEnum, int intValue) const;
    int flagToInt(const QMetaEnum &metaEnum, int flagValue) const;
    int intToFlag(const QMetaEnum &metaEnum, int intValue) const;
    bool isSubValue(int value, int subValue) const;
    bool isPowerOf2(int value) const;

    QString findTranslate(const QString &) const; //! 翻译
    QString translateByMutli(const QString &)const;
private:
    QObject                  *m_object;

    QMap<const QMetaObject *, QString> m_classToProperty;
    QMap<QString,QtProperty *>         m_propertyMap;
    QList<QtProperty *>         m_topLevelProperties;  //！ 顶层属性

    QMap<QtProperty *, int>            m_propertyToIndex;  //！ 属性索引
    QMap<const QMetaObject *, QMap<int, QtVariantProperty *> > m_classToIndexToProperty;
    QMap<QtProperty *, bool>    m_propertyToExpanded;  //! 属性扩展状态
    QMap<QtProperty *, bool>    m_propertyToVisible;  //! 属性显示状态
    QtBrowserItem*              m_currentItem;  //！ 当前选择属性
    QMap<QString, int> m_hideDict;//需要隐藏的属性名称

    /*QtAbstractPropertyBrowser*/ QtTreePropertyBrowser   *m_browser;
    QtVariantPropertyManager    *m_manager;
    QtVariantPropertyManager    *m_readOnlyManager;

    CVariantFactory             *m_factory;
    QMap<int, QtProperty*>       m_indexToProperty;
    QTextEdit                   *m_edit;
    QMap<QString,QVariant>        m_propertyToTip;
    QString                      m_currentPropertyName;

private:
    QMap<QString, bool> m_expandDict;//展开状态
    bool m_needSaveExpandState;//是否保存展开状态
private:
    void ExpandFunction(QtBrowserItem* item, bool isExpand);
    void initExpand();
    void setCurrentPropertyTip(QtBrowserItem* item); //!设置tip信息

public:
    QMap<QString,QVariant>             m_translateName;


};

#if QT_VERSION >= 0x040400
QT_END_NAMESPACE
#endif
#endif // PROPERTYBROWSER_P_H
