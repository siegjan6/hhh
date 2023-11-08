#include "StyleDialog.h"
#include "ui_StyleDialog.h"
#include <QPainter>
#include <QMouseEvent>

//! 1维 styleType
//! 2维 0-数量，1-宽度，2-高度
const qint32 styleArray[][3] = {
        {2, 96, 96},			//! stCap
        {5, 96, 96},			//! stNeedle
        {3, 96, 96},			//! stRange
        {12, 96, 96},	    //! stScaleText
        {8, 96, 96},			//! stScaleStyle
        {3, 96, 96},			//! stScaleArrangement
        {3, 96, 96},			//! stScaleBack
        {9, 96, 96},			//! stBack
        {3, 96, 96},			//! stHalfStyle
        {4, 96, 96},			//! stHalfArrangement
        {6, 62, 36},			//! stLine
        {11, 36, 36},		//! stGradient

        {9, 60, 60},			//! stRMarkerStyle
        {3, 60, 60},			//! stRMarkerArrangement
        {3, 60, 60},			//! stRNeedle
        {5, 80, 50},			//! stRRangeStyle
        {3, 120, 60},		//! stRRangeArrangement
        {3, 120, 60},		//! stRScaleText
        {9, 120, 50},		//! stRScaleStyle
        {3, 70, 70},			//! stRScaleArrangement
        {3, 120, 60},		//! stRScaleBack
        {3, 93, 75}			//! stRScaleBack
};

const qint32 interval = 10;   //! 按钮间隔像素
const qint32 maxCount = 12;  //! 最大按钮数

CStyleDialog::CStyleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CStyleDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();
    for (qint32 i = 0; i < maxCount; i++)
        m_labArray[i] = NULL;
    this->setWindowFlags(Qt::Popup);
    m_index = 0;
    m_count = 0;
    m_width = 0;
    m_height = 0;
    m_type = stCap;
}

CStyleDialog::~CStyleDialog()
{
    for (qint32 i = 0; i < maxCount; i++)
        {
            if (m_labArray[i] != NULL)
                delete m_labArray[i];
        }
    delete ui;
}
/*!
 \section  根据风格窗体的类型显示出该风格的所有类别。
*/
void CStyleDialog::showStyle()
{
     styleType type = getStyle();
     m_count = styleArray[type][0];
     m_width = (m_count < 3) ? m_count : 3;
     m_width = m_width * (styleArray[type][1] + interval) + interval;
     m_height = (m_count - 1) / 3 + 1;
     m_height = m_height * (styleArray[type][2] + interval) + interval;

     this->resize(m_width, m_height);

     for (qint32 i = 0; i < m_count; i++)
      {
         qint32 x = i % 3;
         x = (styleArray[type][1] + interval) * x + interval;
         qint32 y = i / 3;
         y = (styleArray[type][2] + interval) * y + interval;

         QLabel* lab = new QLabel(this);
         m_labArray[i] = lab;
         lab->move(x,y);

         QString strI = QString::number(i);
         switch(m_type)
         {
         case stCap:
         {
             this->move(730,330);
             QPixmap pix(":/picture/image/bcap"+ strI +".bmp");
             m_labArray[i]->resize(pix.width(),pix.height());
             m_labArray[i]->setPixmap(pix);
         }
             break;
         case stNeedle:
         {
             this->move(680,300);
             QPixmap pix(":/picture/image/bneedle"+ strI +".bmp");
             m_labArray[i]->resize(pix.width(),pix.height());
             m_labArray[i]->setPixmap(pix);
         }
             break;
         case stRange:
         {
             this->move(680,300);
             QPixmap pix(":/picture/image/brange"+ strI +".bmp");
             m_labArray[i]->resize(pix.width(),pix.height());
             m_labArray[i]->setPixmap(pix);
         }
             break;
         case stScaleText:
         {
             this->move(690,230);
             QPixmap pix(":/picture/image/bscaletext" + strI +".bmp");
             m_labArray[i]->resize(pix.width(),pix.height());

             m_labArray[i]->setPixmap(pix);
         }
             break;
         case stScaleStyle:
         {
             this->move(700,250);
             QPixmap pix(":/picture/image/bscalestyle" + strI +".bmp");
             m_labArray[i]->resize(pix.width(),pix.height());

             m_labArray[i]->setPixmap(pix);
         }
             break;
         case stScaleArrangement:
         {
             this->move(750,300);
             QPixmap pix(":/picture/image/bscalearrange" + strI +".bmp");
             m_labArray[i]->resize(pix.width(),pix.height());
             m_labArray[i]->setPixmap(pix);
         }
             break;
         case stScaleBack:
         {
             this->move(680,300);
             QPixmap pix(":/picture/image/bscaleback" + strI +".bmp");
             m_labArray[i]->resize(pix.width(),pix.height());
             m_labArray[i]->setPixmap(pix);
         }
             break;
         case stBack:
         {
             this->move(700,250);
             QPixmap pix(":/picture/image/bback" + strI +".bmp");
             m_labArray[i]->resize(pix.width(),pix.height());
             m_labArray[i]->setPixmap(pix);
         }
             break;
         case stHalfStyle:
         {
             this->move(700,250);
             QPixmap pix(":/picture/image/bhalfstyle" + strI +".bmp");
             m_labArray[i]->resize(pix.width(),pix.height());
             m_labArray[i]->setPixmap(pix);

         }
             break;
         case stHalfArrangement:
         {
             this->move(750,300);
             QPixmap pix(":/picture/image/bhalfarrange" + strI +".bmp");
             m_labArray[i]->resize(pix.width(),pix.height());
             m_labArray[i]->setPixmap(pix);
         }
             break;
         case stLine:
         {
             QPixmap pix(":/picture/image/bline" + strI + ".bmp");
             m_labArray[i]->resize(pix.width(),pix.height());
             m_labArray[i]->setPixmap(pix);
         }
             break;
         case stGradient:
         {
             QPixmap pix(":/picture/image/bgrad" +strI+".bmp");
             m_labArray[i]->resize(pix.width(),pix.height());
             m_labArray[i]->setPixmap(pix);
         }
             break;
         case stRMarkerStyle:
         {
             QPixmap pix(":/picture/image/brmarkerstyle" +strI+ ".bmp");
             m_labArray[i]->resize(pix.width(),pix.height());
             m_labArray[i]->setPixmap(pix);
         }
             break;
         case stRMarkerArrangement:
         {
             QPixmap pix(":/picture/image/brmarkerarrange" +strI+ ".bmp");
             m_labArray[i]->resize(pix.width(),pix.height());
             m_labArray[i]->setPixmap(pix);
         }
             break;
         case stRNeedle:
         {
             QPixmap pix(":/picture/image/brneedle" +strI+ ".bmp");
             m_labArray[i]->resize(pix.width(),pix.height());
             m_labArray[i]->setPixmap(pix);
         }
             break;
         case stRRangeStyle:
         {
             QPixmap pix(":/picture/image/brrangestyle" +strI+ ".bmp");
             m_labArray[i]->resize(pix.width(),pix.height());
             m_labArray[i]->setPixmap(pix);
         }
             break;
         case stRRangeArrangement:
         {
             QPixmap pix(":/picture/image/brrangearrange" +strI+ ".bmp");
             m_labArray[i]->resize(pix.width(),pix.height());
             m_labArray[i]->setPixmap(pix);
         }
             break;
         case stRScaleText:
         {
             QPixmap pix(":/picture/image/brscaletext" +strI+ ".bmp");
             m_labArray[i]->resize(pix.width(),pix.height());
             m_labArray[i]->setPixmap(pix);
         }
             break;
        case stRScaleStyle:
         {
             QPixmap pix(":/picture/image/brscalestyle" +strI+ ".bmp");
             m_labArray[i]->resize(pix.width(),pix.height());
             m_labArray[i]->setPixmap(pix);
         }
             break;
         case stRScaleArrangement:
         {
             QPixmap pix(":/picture/image/brscalearrange" +strI+ ".bmp");
             m_labArray[i]->resize(pix.width(),pix.height());
             m_labArray[i]->setPixmap(pix);
         }
             break;
         case stRScaleBack:
         {
             QPixmap pix(":/picture/image/brangearrange" +strI+ ".bmp");
             m_labArray[i]->resize(pix.width(),pix.height());
             m_labArray[i]->setPixmap(pix);
         }
             break;
         case stTemperature:
         {
             this->move(730,330);
             QPixmap icon(":/picture/image/tem" +strI+".bmp");
             m_labArray[i]->resize(icon.width(),icon.height());
             m_labArray[i]->setPixmap(icon);
         }
             break;
         case stTemHelp:
             break;
         default:
             break;
         }
      }
}

void CStyleDialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(QColor(110,110,110,255));
    //!  绘制背景
    QRect rect = QRect(0,0,m_width,m_height);
    painter.drawRect(rect);

    styleType type = getStyle();
    qint32 x = m_index % 3;
    x = (styleArray[type][1] + interval) * x + interval;
    qint32 y = m_index / 3;
    y = (styleArray[type][2] + interval) * y + interval;
    QRect r(x, y, styleArray[type][1], styleArray[type][2]);

    //!  绘制选中框
    QPen pen;
    pen.setColor(QColor(0,0,0,255));
    pen.setWidth(6);
    painter.setPen(pen);
    painter.drawRect(r);
}
/*!
 \section 确定选择的风格。
*/
void CStyleDialog::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        QPoint p = event->pos();
        this->buttonDown(p);
    }
}

void CStyleDialog::buttonDown(QPoint point)
{
    QPoint p(point.x(), point.y());

   //!  是否关闭窗口
    if (p.x() < 0 || p.x() > m_width || p.y() < 0 || p.y() > m_height)
    {
       this->close();
       return;
    }

    styleType type = getStyle();
   //!  是否选中按钮
    for (qint32 i = 0; i < m_count; i++)
    {
       qint32 x = i % 3;
       x = (styleArray[type][1] + interval) * x + interval;
       qint32 y = i / 3;
       y = (styleArray[type][2] + interval) * y + interval;
       QRect r(x, y, styleArray[type][1], styleArray[type][2]);

       if (r.contains(p.x(),p.y()) == true)
         {
            m_index = i;
            accept();
            return;
         }
    }
}

void CStyleDialog::setStyle(styleType type)
{
    m_type = type;
    this->showStyle();
}

styleType CStyleDialog::getStyle()
{
    return m_type;
}

