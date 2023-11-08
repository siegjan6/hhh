/*!

\author eHVr

\section 功能
   属性管理器。
*/
#ifndef PROPERTYBROWSER_H
#define PROPERTYBROWSER_H

#include <QWidget>

#include <QtCore/qglobal.h>
#include <QTreeWidgetItem>


#if defined(MDPROPERTYBROWSER_LIBRARY)
#  define QT_QTPROPERTYBROWSER_EXPORT Q_DECL_EXPORT
#else
#  define QT_QTPROPERTYBROWSER_EXPORT Q_DECL_IMPORT
#endif

class QtBrowserItem;

class CPropertyBrowserPrivate;
class QT_QTPROPERTYBROWSER_EXPORT CPropertyBrowser : public QWidget
{
    Q_OBJECT
public:

    CPropertyBrowser(QWidget *parent = 0);
    ~CPropertyBrowser();

public:
    void setObject(QObject *object);
    QObject *object() const;

public:
    /*!
     \brief setPropertyAttribute
            设置指定property的attribute属性值
     \em    setPropertyAttribute(QString("mile"),QString("minimum"),1);
            mile类型为int型，设置minimum后，最小值为1.
     \param propertyName
     \param attribute
     \param value

            Property Type	Attribute Name	Attribute Type
            int             minimum         QVariant::Int
                            maximum         QVariant::Int
                            singleStep      QVariant::Int

            double         minimum          QVariant::Double
                           maximum          QVariant::Double
                           singleStep       QVariant::Double
                           decimals         QVariant::Int

            QString        regExp           QVariant::RegExp
            QDate          minimum          QVariant::Date
                           maximum          QVariant::Date

            QPointF       decimals          QVariant::Int
            QSize         minimum           QVariant::Size
                          maximum           QVariant::Size

            QSizeF         minimum           QVariant::SizeF
                           maximum           QVariant::SizeF
                           decimals          QVariant::Int

            QRect         constraint         QVariant::Rect
            QRectF        constraint         QVariant::RectF
                          decimals           QVariant::Int
            enum          enumNames          QVariant::StringList
                          enumIcons          QMap<int,QIcon>
            flag          flagNames          QVariant::StringList
    */
    void setPropertyAttribute(const QString &propertyName, const QString &attribute, const QVariant &value);
public:
    //! 设置属性值
    void setPropertyFont(const QString &propertyName, const QFont &propertyValue);
    void setPropertyPixmap(const QString &propertyName, const QPixmap &propertyPixmap);

    //! 设置多个属性行界面隐藏
    void setPropertyHide(const QStringList &list,bool isHide);
    //! 设置单一属性行界面隐藏
    void setSinglePropertyHide(const QString &str,bool isHide);

    //! 只读属性设置背景色
    void setReadOnlyBackColor(const QColor &color);
    //! 设置属性的tip
    void setPropertyTipMap(const QMap<QString, QVariant> &propertyTip);
    //! 指定属性的背景色
    void setPropertyBackColor(const QString &name,const QColor &color);

    //! 设置属性只读
    void setPropertyReadOnly(bool isReadOnly = true, const QStringList &strList ={});
    //! 动态添加枚举
    void resetEnums(const QString &name,const QStringList &list);
    //! 获取当前属性名称
    QString getCurrentPropertyName() const;
    //! 属性框全展开
    void setExpandAll();

public:
    void updateProperty();//!  更新属性窗体
private:
    CPropertyBrowserPrivate *d_ptr;
    Q_DECLARE_PRIVATE(CPropertyBrowser)
    Q_DISABLE_COPY(CPropertyBrowser)
    Q_PRIVATE_SLOT(d_func(), void slotValueChanged(QtProperty *, const QVariant &))
private:
    void collaps(QtBrowserItem* item);
    void expand(QtBrowserItem* item);
    void expandFunction(QtBrowserItem* item, bool isExpand);
    void setCurrentPropertyTip(QtBrowserItem* item); //!设置tip信息

signals:
    void propertyFinish();

};
#endif
 /*
  *
setObject
   |---saveExpandedState
   |
addClassProperties
   |---classPropertyNotExist
   |                   |--- "GUID_"/ "EX_GUID_"/"N_GUID_" -----translate
   |                   |---addNormalProperty ---- translate
   |---updateClassProperties
   |
addPropertiesToBrowser
   |
restoreExpandedState
   |---initExpandedState
   |---restoreExpandedState

*/
