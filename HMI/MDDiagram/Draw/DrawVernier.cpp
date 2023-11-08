#include "DrawVernier.h"
#include "ui_DrawVernier.h"
#include <QPainter>

CDrawVernier::CDrawVernier(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDrawVernier)
{
    ui->setupUi(this);
    m_vernierData = NULL;
    width = 0;
    height = 0;
    m_rectNum = 0;
//    setWindowFlags( Qt::FramelessWindowHint );
//    setAttribute(Qt::WA_TranslucentBackground, true);
    setFocusPolicy(Qt::NoFocus);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    setWindowOpacity(0.5);
    this->close();
}

CDrawVernier::~CDrawVernier()
{
    delete ui;
}

void CDrawVernier::setVernierData(CDrawData *vernierData)
{
    m_vernierData = vernierData;
}

void CDrawVernier::setNum(qint32 num)
{
    m_rectNum = num;
}

void CDrawVernier::setValueList(QList<qreal> valueList)
{
    m_valueList = valueList;
}

void CDrawVernier::setStringList1(QList<QString> stringList)
{
    m_stringList1 = stringList;
}

void CDrawVernier::setStringList2(QList<QString> stringList)
{
    m_stringList2 = stringList;
}

void CDrawVernier::setStringList3(QList<QString> stringList)
{
    m_stringList3 = stringList;
}

void CDrawVernier::paintEvent(QPaintEvent *)
{
    QPainter *paint = new QPainter(this);
    paint->setRenderHint(QPainter::Antialiasing, true);
    paint->setBrush(QColor(240,240,240,255));
    QRect rect(0,0,getWidth(),getheight());
    paint->drawRoundedRect(rect,10,10);
    paint->setPen(QColor(0,0,0,255));
    QFont font;
    font.setPointSize(12);
    paint->setFont(font);
    drawValue(paint);
    delete paint;
}

void CDrawVernier::drawValue(QPainter *painter)
{
    if(m_vernierData->m_category == 0) //!一分组各数据值
    {
        if(!m_vernierData->m_stringText.isEmpty())
        {
           painter->drawText(10,20,m_vernierData->m_stringText);
           QString str = m_stringList1.at(m_rectNum);
           str.append(":");
           QString str1 = str+QString("%1").arg(m_valueList.at(m_rectNum));
           painter->drawText(10,20+25,str1);
        }
        else
        {
            QString str = m_stringList1.at(m_rectNum);
            str.append(":");
            QString str1 = str+QString("%1").arg(m_valueList.at(m_rectNum));
            painter->drawText(10,20,str1);
        }
    }
    else if(m_vernierData->m_category == 1) //! 二分组各数据值
    {
        QString str = m_stringList1.at(m_rectNum);
        painter->drawText(10,20,str);

        qint32 count = m_stringList2.count();//每组数据的个数
        QList<qreal>valueList;

        for(qint32 j =m_rectNum*count;j<count*(m_rectNum+1);j++)
        {
            valueList.append(m_valueList.at(j));
        }
        for(qint32 i = 0;i<m_stringList2.count();i++)
        {
            QString str1 = m_stringList2.at(i);
            str1.append(":");
            str1 += QString("%1").arg(valueList.at(i));
            painter->drawText(10,20+25*(i+1),str1);
        }
    }
}

qint32 CDrawVernier::getWidth()
{
    QFont font;
    font.setPointSize(12);
    font.setBold(true);
    QFontMetrics fm(font);
    if(m_vernierData->m_category == 0)
    {
        if(!m_vernierData->m_stringText.isEmpty())
        {
            qint32 width1 = fm.width(m_vernierData->m_stringText);
            qint32 width2 = fm.width(m_stringList1.at(m_rectNum));
            width = (width1>width2?width1:width2) +20;
        }
        else
        {
            QString str = m_stringList1.at(m_rectNum);
            str.append(":");
            QString str1 = str+QString("%1").arg(m_valueList.at(m_rectNum));
            width = fm.width(str1)+20;
        }
    }
    else
    {
        qint32 count = m_stringList1.count()*m_stringList2.count();//! 求取最长数据值
        qint32 *valueWidth = new qint32[count-1];
//        qint32 valueWidth[count-1];
        for (qint32 i = 0; i < count; i++)
        {
            QString str = QString("%1").arg(m_valueList.at(i));
            qint32 width1 = fm.width(str);
            valueWidth[i] = width1;
        }
        qint32 valueStringWidth =valueWidth[0];
        for(qint32 i =0;i<count;i++)
        {
            for (qint32 j=i+1;j<count;j++)
            {
                if (valueWidth[j]<=valueWidth[i])
                {
                    valueStringWidth=valueWidth[i];
                    valueWidth[i]=valueWidth[j];
                    valueWidth[j]=valueStringWidth;
                }
            }
        }

        qint32 count1 = m_stringList1.count();  //! 求取最长的字符串1的值
        qint32 *string1 = new qint32[count1];
//        qint32 string1[count1];
        for (qint32 i = 0; i < count1; i++)
        {
            qint32 width1 = fm.width(m_stringList1.at(i));
            string1[i] = width1;
        }
        qint32 String1Width = string1[0];
        for(qint32 i =0;i<count1;i++)
        {
            for (qint32 j=i+1;j<count1;j++)
            {
                if (string1[j]<=string1[i])
                {
                    String1Width=string1[i];
                    string1[i]=string1[j];
                    string1[j]=String1Width;
                }
            }
        }
        qint32 count2 = m_stringList2.count();//! 求取最长的字符串2的值
        qint32 *string2 = new qint32[count2];
//        qint32 string2[count2];
        for (qint32 i = 0; i < count2; i++)
        {
            qint32 width2 = fm.width(m_stringList2.at(i));
            string2[i] = width2;
        }
        qint32 String2Width;
        for(qint32 i =0;i<count2;i++)
        {
            for (qint32 j=i+1;j<count2;j++)
            {
                if (string2[j]<=string2[i])
                {
                    String2Width=string2[i];
                    string2[i]=string2[j];
                    string2[j]=String2Width;
                }
            }
        }
        QString string = QString(":");  //! ":"的长度
        qint32 widthString = fm.width(string);
        width = (String1Width>=(String2Width+valueStringWidth+widthString)?
                     String1Width:(String2Width+valueStringWidth+widthString))+15;
    }

    return width;
}

qint32 CDrawVernier::getheight()
{
    if(m_vernierData->m_category == 0)
    {
        if(!m_vernierData->m_stringText.isEmpty())
        {
           height = 55;
        }
        else
        {
            height = 30;
        }
    }
    else
    {
        height = 25*m_stringList2.count()+30;
    }
    return height;
}


