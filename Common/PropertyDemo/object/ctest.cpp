#include "ctest.h"
#include <QDebug>


CTest::CTest(QObject *parent) :
    QObject(parent),
    m_dialog(0),
    m_dialog_1(0)
{
    initGroup();
}

CTest::~CTest()
{
    delete m_dialog;
    delete m_dialog_1;
}

QString CTest::address() const
{
    return m_address;
}

int CTest::id() const
{
    return m_id;
}

void CTest::setId(int id)
{
    if(m_id != id){
        m_id = id;
        emit idChanged(id);
    }
}

double CTest:: mile() const
{
    return m_mile;
}
void CTest::setMile(double mile)
{
    if(mile != m_mile){
        m_mile = mile;
        emit mileChanged(mile);
    }
}

QString CTest::name() const
{
    return m_name;
}

void CTest::setName(const QString &name)
{

    if(m_name != name)
    {
        m_name = name;
        emit nameChanged(m_name);
    }
}



QDate CTest::date() const
{
    return m_date;
}
void CTest::setDate(QDate date)
{
    if(date != m_date){
        m_date = date;
        emit dateChanged(date);
    }
}

QTime CTest::time() const
{
    return m_time;
}
void CTest::setTime(QTime time)
{
    if(time != m_time){
        m_time = time;
        emit timeChanged(time);
    }
}

QDateTime CTest::dateTime() const
{
    return m_dateTime;
}
void  CTest::setDateTime(QDateTime dateTime)
{
   if(dateTime != m_dateTime){
        m_dateTime = dateTime;
        emit dateTimeChanged(dateTime);
    }
}

QKeySequence CTest::keySequence() const
{
    return m_keySequence;
}
void CTest::setKeySequence(QKeySequence keySequence)
{
   if(keySequence != m_keySequence){
        m_keySequence = keySequence;
        emit keySequenceChanged(keySequence);
    }
}

QChar CTest::character() const
{
    return m_character;
}
void CTest::setCharacter(QChar character)
{
   if(character != m_character){
        m_character = character;
        emit characterChanged(character);
    }
}

QLocale CTest::locale() const
{
    return m_locale;
}
void CTest::setLocale(QLocale locale)
{
     if(locale != m_locale){
        m_locale= locale;
        emit localeChanged(locale);
    }
}

QPoint CTest::point() const
{
    return m_point;
}
void CTest::setPoint(QPoint point)
{
     if(point != m_point){
        m_point = point;
        emit pointChanged(point);
    }
}

QPointF CTest::pointF() const
{
    return m_pointF;
}

void CTest::setPointF(QPointF pointF)
{
    if(pointF != m_pointF){
        m_pointF= pointF;
        emit pointFChanged(pointF);
    }
}

QSize CTest::size() const
{
    return m_size;
}
void CTest::setSize(QSize size)
{
     if(size != m_size){
        m_size = size;
        emit sizeChanged(size);
    }
}

QSizeF CTest::sizeF() const
{
    return m_sizeF;
}
void CTest::setSizeF(QSizeF sizeF)
{    
    if(sizeF != m_sizeF){
        m_sizeF = sizeF;
        emit sizeFChanged(sizeF);
    }
}

QRect CTest::rect() const
{
    return m_rect;
}
void CTest::setRect(QRect rect)
{
    if(rect != m_rect){
        m_rect = rect;
        emit rectChanged(rect);
    }
}

QRectF CTest::rectF() const
{
    return m_rectF;
}
void CTest::setRectF(QRectF rectF)
{
    if(rectF != m_rectF){
        m_rectF = rectF;
        emit rectFChanged(rectF);
    }
}
QString CTest::GUID_Base() const
{
    return m_GUID_Base;
}

QString CTest::GUID_secBase() const
{
    return m_secBase;
}

QString CTest::GUID_Pro() const
{
    return m_GUID_Pro;
}

void CTest::initGroup()
{
    //属性初始化
//    m_name = "hello world";
    m_id = 5;
    m_mile = 12;

    m_translateName.insert(QString("id"),QString("编号"));
    m_translateName.insert(QString("GUID_Base"),QString("基础属性"));// group name
    m_translateName.insert(QString("name"),QString("姓名"));

    m_translateName.insert(QString("address"),QString("地址"));
    m_translateName.insert(QString("mile"),QString("英里"));
    m_translateName.insert(QString("demo"),QString("示例"));
    m_translateName.insert(QString("location"),QString("位置"));
    //将扩展属性的信号与 valueChanged信号链接
//    connect(this,SIGNAL(DILstringChanged(QVariant)),this,SIGNAL(valueChanged(QVariant)));
//    connect(this,SIGNAL(DILdemoChanged(QVariant)),this,SIGNAL(valueChanged(QVariant)));
    m_translateName.insert(QString("derection"),QString("方位"));

    m_propertyTip.insert("address","地址属性tip已设置！");
    m_propertyTip.insert("id","编号属性tip已设置！");
    m_propertyTip.insert("name","姓名属性tip已设置！");
    m_propertyTip.insert("GUID_Base","基础属性tip已设置！");
}
QString CTest::DILstring() const
{
     return m_string;
}

void CTest::setDILstring(const QString &name)
{
    Q_UNUSED(name);

    if(!m_dialog)
    {
        m_dialog = new CDialog;
        connect(m_dialog,&CDialog::changeValue,
                this,[=](QString name){m_string = name;
            emit DILstringChanged(name);});
        //             connect(m_dialog_1,SIGNAL(changeIcon(QPixmap)),this,SIGNAL(iconChanged(QPixmap)));
    }
    m_dialog->exec();
}
QString CTest::DILdemo() const
{
    return m_demo;
}

void CTest::setDILdemo(const QString &demo)
{
    Q_UNUSED(demo);
    if(!m_dialog_1){
        m_dialog_1 = new CDialog_1;
        connect(m_dialog_1,&CDialog_1::changeValue,
                [=](QString name){m_demo = name;emit DILdemoChanged(name);});
    }
    m_dialog_1->exec();
}

bool CTest::male()
{
    return m_male;
}

void CTest::setMale(bool isMan)
{
    if(isMan != m_male)
        m_male = isMan;
}

QMap<QString, QVariant> CTest::translateName() const
{
    return m_translateName;
}

QMap<QString, QVariant> CTest::propertyTip() const
{
    return m_propertyTip;
}

QFont CTest::font() const
{
    return m_font;
}

void CTest::setFont(const QFont &font)
{
    if(m_font != font){
        m_font = font;
        emit fontChanged(font);
    }
}

CTest::Derection CTest::derection() const
{
    return m_derection;
}

CTest::Location CTest::location() const
{
    return m_location;
}

void CTest::setLocation(const Location &location)
{
    if(m_location != location)
    {
        m_location = location;
        emit locationChanged(m_location);
    }
}

void CTest::setDerection(const Derection &derection)
{
    if(m_derection != derection)
    {
        m_derection = derection;
        emit derectionChanged(m_derection);
    }
}


QColor CTest::color() const
{
    return m_color;
}

void CTest::setColor(const QColor &color)
{
    if(m_color != color)
    {
        m_color = color;
        emit colorChanged(m_color);
    }
}




