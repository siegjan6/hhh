/*!

\author xuk

\section 功能

demo 类，用于演示各种属性的添加方法
*/
#ifndef CTEST_H
#define CTEST_H

#include <QObject>
#include <QDate>
#include <QDateTime>
#include <QString>
#include <QKeySequence>
#include <QLocale>
#include <QPoint>
#include <QPointF>
#include <QSize>
#include <QSizeF>
#include <QRect>
#include <QRectF>
#include <QVariant>
#include <QImage>
#include <QColor>
#include <QFont>

#include "cdialog.h"
#include "cdialog_1.h"

class CTest : public QObject
{
    Q_OBJECT
    Q_ENUMS(Derection)         //! 声明枚举
    Q_ENUMS(Location)

    Q_PROPERTY(QMap translateName READ translateName)// translate
    Q_PROPERTY(QMap propertyTip READ propertyTip) // tip

    //! 分组 属性名前添加GUID_前缀 分组范围为到下一个分组前的属性都属于本组
    Q_PROPERTY(QString GUID_Base READ GUID_Base)

    Q_PROPERTY(QString address READ address)
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(double mile READ mile WRITE setMile NOTIFY mileChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString DILdemo READ DILdemo WRITE setDILdemo NOTIFY DILdemoChanged)
    Q_PROPERTY(bool male_BTN READ male WRITE setMale)
    //! 扩展属性，添加DIL前缀名，显示为属性为string，
    //! 使用setProperty或setPropertyAttribute函数设置时，名称为DILstring
    //！ Q_PROPERTY(QString DILstring READ DILstring WRITE setDILstring /*NOTIFY DILstringChanged*/)

    Q_PROPERTY(QString GUID_Pro READ GUID_Pro)
    Q_PROPERTY(QDate date READ date WRITE setDate NOTIFY dateChanged)
    Q_PROPERTY(QTime time READ time WRITE setTime NOTIFY timeChanged)
    Q_PROPERTY(QDateTime dateTime READ dateTime WRITE setDateTime NOTIFY dateTimeChanged)
    Q_PROPERTY(QKeySequence keySequence READ keySequence WRITE setKeySequence NOTIFY keySequenceChanged)  //快捷键属性
    Q_PROPERTY(QChar character READ character WRITE setCharacter NOTIFY characterChanged)
    Q_PROPERTY(QLocale locale READ locale WRITE setLocale NOTIFY localeChanged)
    Q_PROPERTY(QPoint point READ point WRITE setPoint NOTIFY pointChanged)
    Q_PROPERTY(QPointF pointF READ pointF WRITE setPointF  NOTIFY pointFChanged)
    Q_PROPERTY(QSize size READ size WRITE setSize  NOTIFY sizeChanged)
    Q_PROPERTY(QSizeF sizeF READ sizeF WRITE setSizeF  NOTIFY sizeFChanged)
    Q_PROPERTY(QRect rect READ rect WRITE setRect NOTIFY rectChanged)
    Q_PROPERTY(QRectF rectF READ rectF WRITE setRectF NOTIFY rectFChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QFont font READ font WRITE setFont NOTIFY fontChanged)

    //枚举属性
    Q_PROPERTY(Derection derection_W READ derection WRITE setDerection NOTIFY derectionChanged)
    Q_PROPERTY(Location location_W READ location WRITE setLocation NOTIFY locationChanged)

public:
    explicit CTest(QObject *parent = 0);
    ~CTest();

    /*!
     \brief Derection
            枚举类(C++11 标准)
    */
    enum class Derection{up,down,right,left};
    enum class Location{east,south,west,north};

    QString address() const;

    /*!
     \brief id property
        id 属性的read和write函数
    */
    int id() const;
    void setId(int id);

    /*!
     \brief mile property
        mile 属性的read和write函数
    */
    double mile() const;
    void setMile(double mile);

    /*!
     \brief name property
        name 属性的read和write函数
    */
    QString name() const;
    void setName(const QString &name);

    /*!
     \brief DILstring property
        DILstring 属性的read和write函数
    */
    QString DILstring() const;
    void setDILstring(const QString &name);

    /*!
     \brief date property
        date 属性的read和write函数
    */
    QDate date() const;
    void setDate(QDate date);

    /*!
     \brief time property
        time 属性的read和write函数
    */
    QTime time() const;
    void setTime(QTime time);

    /*!
     \brief dateTime property
        dateTime 属性的read和write函数
    */
    QDateTime dateTime() const;
    void  setDateTime(QDateTime dateTime);

    /*!
     \brief keySequence property
        keySequence 属性的read和write函数
    */
    QKeySequence keySequence() const;
    void setKeySequence(QKeySequence keySequence);

    /*!
     \brief character property
        character 属性的read和write函数
    */
    QChar character() const;
    void setCharacter(QChar character);

    /*!
     \brief locale property
        locale 属性的read和write函数
    */
    QLocale locale() const;
    void setLocale(QLocale locale);

    /*!
     \brief point property
        point 属性的read和write函数
    */
    QPoint point() const;
    void setPoint(QPoint point);

    /*!
     \brief pointF property
        pointF 属性的read和write函数
    */
    QPointF pointF() const;
    void setPointF(QPointF pointF);

    /*!
     \brief size property
        size 属性的read和write函数
    */
    QSize size() const;
    void setSize(QSize size);

    /*!
     \brief sizeF property
        sizeF 属性的read和write函数
    */
    QSizeF sizeF() const;
    void setSizeF(QSizeF sizeF);

    /*!
     \brief rect property
        rect 属性的read和write函数
    */
    QRect rect() const;
    void setRect(QRect rect);

    /*!
     \brief rectF property
        rectF 属性的read和write函数
    */
    QRectF rectF() const;
    void setRectF(QRectF rectF);

    /*!
     \brief color property
        color 属性的read和write函数
    */
    QColor color() const;
    void setColor(const QColor &color);

    /*!
     \brief font property
        font 属性的read和write函数
    */
    QFont font() const;
    void setFont(const QFont &font);

    /*!
     \brief derection property
        derection 属性的read和write函数
    */
    Derection derection() const;
    void setDerection(const Derection &derection);

    Location location() const;
    void setLocation(const Location &location);


    /*!
     \brief group property
        分组 属性的read函数
    */
    QString GUID_Base() const;
    QString GUID_secBase() const;
    QString GUID_Pro() const;

    QString DILdemo() const;
    void setDILdemo(const QString &demo);

    bool male();
    void setMale(bool isMan);


    QMap<QString,QVariant> translateName() const;
    QMap<QString, QVariant> propertyTip() const;
private:
    /*!
     \brief initGroup
        初始化函数，用于初始化属性值，初始化扩展属性的信号槽链接，构造函数中调用
    */
    void initGroup();

signals:

    void idChanged(QVariant);
    void mileChanged(double);
    void nameChanged(QVariant);
    void DILdemoChanged(QVariant);
    void dateChanged(QDate);
    void timeChanged(QTime);
    void dateTimeChanged(QDateTime);
    void keySequenceChanged(QKeySequence);
    void characterChanged(QChar);
    void localeChanged(QLocale);
    void pointChanged(QPoint);
    void pointFChanged(QPointF);
    void sizeChanged(QSize);
    void sizeFChanged(QSizeF);
    void rectChanged(QRect);
    void rectFChanged(QVariant);
    void colorChanged(QVariant);
    void fontChanged(QFont);
    void DILstringChanged(QVariant);
    void derectionChanged(Derection);
    void locationChanged(Location);


private:
    QString      m_address;
    int          m_id;               //!  id属性
    double       m_mile;             //!  mile属性

    QString      m_name;             //!  name属性
    QString      m_string;
    QDate        m_date;             //!  date属性
    QTime        m_time;             //!  time属性
    QDateTime    m_dateTime;         //!  datetime属性
    QKeySequence m_keySequence;      //!  快捷键属性
    QChar        m_character;        //!  char属性
    QLocale      m_locale;           //!  locale属性
    QPoint       m_point;            //!  point属性
    QPointF      m_pointF;           //!  pointF属性
    QSize        m_size;             //!  size属性
    QSizeF       m_sizeF;            //!  sizeF属性
    QRect        m_rect;             //!  rect属性
    QRectF       m_rectF;            //!  rectF属性
    QColor       m_color;            //!  color属性
    QFont        m_font;             //!  font属性

    Derection    m_derection;        //!  derection属性
    Location     m_location;         //!  location属性

    QString      m_GUID_Base;        //!  GUID_Base属性
    QString      m_secBase;
    QString      m_GUID_Pro;         //!  GUID_Pro属性
    CDialog      *m_dialog;          //!  扩展属性调用的对话框

    bool         m_male;

    QString     m_demo;
    CDialog_1   *m_dialog_1;

    QMap<QString,QVariant>       m_translateName;
    QMap<QString,QVariant>        m_propertyTip;

};

#endif // CTEST_H
