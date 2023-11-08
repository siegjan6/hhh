
#include "PaintButton.h"
#include "StyleDialog.h"
#include "ColorDialog.h"

CPaintButton::CPaintButton()
{
    m_button = NULL;
    m_parentDialog = NULL;
    m_backColor = QColor(255,255,255,255);
    m_style = stCap;
    m_type = ptColor;
    m_index = 0;
}
CPaintButton::~CPaintButton()
{
}
void CPaintButton::buttonClicked()
{
    switch(m_type) {
    case ptImage:
    {
        CStyleDialog sDlg;
        sDlg.setStyle(m_style);
        sDlg.m_index = m_index;
        sDlg.move(800,300);
        sDlg.show();
        if (sDlg.exec() == QDialog::Accepted)
        {
            setIndex(sDlg.m_index);
            selectItem();
        }
    }
        break;
    case ptColor:
    {
        CColorDialog cDlg(m_parentDialog);
        cDlg.setColor(m_backColor);
        cDlg.show();
        if (cDlg.exec() == QDialog::Accepted)
        {
            setColor(cDlg.m_color);
            selectItem();
        }
    }
        break;
    default:
        break;
    }

}

void CPaintButton::setButtonColor(QPushButton *button, QColor color,paintType type)
{
    m_button = button;
    setColor(color);
    setType(type);
    selectItem();
}

void CPaintButton::setButtonImage(QPushButton *button, styleType style, qint32 index, paintType type)
{
    m_button = button;
    setType(type);
    setIndex(index);
    setStyle(style);
    selectItem();
}

void CPaintButton::setIndex(qint32 index)
{
    m_index = index;
}

qint32 CPaintButton::getIndex()
{
    return m_index;
}

QColor CPaintButton::getColor()
{
    return m_backColor;
}

void CPaintButton::setParentDialog(QDialog *dialog)
{
    m_parentDialog = dialog;
}

void CPaintButton::setStyle(styleType style)
{
    m_style = style;
}

void CPaintButton::setType(paintType type)
{
    m_type = type;
}

void CPaintButton::setColor(QColor color)
{
    m_backColor = color;
}


void CPaintButton::selectItem()
{
        switch (m_type) {
        case ptColor:
        {
            QString styleSheet = QString("background-color:rgb(%1,%2,%3,%4);")
                    .arg(m_backColor.red())
                    .arg(m_backColor.green())
                    .arg(m_backColor.blue())
                    .arg(m_backColor.alpha());
            this->m_button->setStyleSheet(styleSheet);
        }
            break;
        case ptImage:
        {
            QString bmpStr = "";

                switch(m_style) {
                case stCap:
                    bmpStr = ":/picture/image/cap";
                    break;
                case stNeedle:
                    bmpStr = ":/picture/image/needle";
                    break;
                case stRange:
                    bmpStr = ":/picture/image/range";
                    break;
                case stScaleText:
                    bmpStr = ":/picture/image/scaletext";
                    break;
                case stScaleStyle:
                    bmpStr = ":/picture/image/scalestyle";
                    break;
                case stScaleArrangement:
                    bmpStr = ":/picture/image/scalearrange";
                    break;
                case stScaleBack:
                    bmpStr = ":/picture/image/scaleback";
                    break;
                case stBack:
                    bmpStr = ":/picture/image/back";
                    break;
                case stHalfStyle:
                    bmpStr = ":/picture/image/halfstyle";
                    break;
                case stHalfArrangement:
                    bmpStr = ":/picture/image/halfarrange";
                    break;
                case stLine:
                    bmpStr = ":/picture/image/line";
                    break;
                case stGradient:
                    bmpStr = ":/picture/image/grad";
                    break;
                case stRMarkerStyle:
                    bmpStr = ":/picture/image/rmarkerstyle";
                    break;
                case stRMarkerArrangement:
                    bmpStr = ":/picture/image/rmarkerarrange";
                    break;
                case stRNeedle:
                    bmpStr = ":/picture/image/rneedle";
                    break;
                case stRRangeStyle:
                    bmpStr = ":/picture/image/rrangestyle";
                    break;
                case stRRangeArrangement:
                    bmpStr =  ":/picture/image/rrangearrange";
                case stRScaleBack:
                    bmpStr = ":/picture/image/rrangearrange";
                    break;
                case stRScaleText:
                    bmpStr = ":/picture/image/rscaletext";
                    break;
                case stRScaleStyle:
                    bmpStr = ":/picture/image/rscalestyle";
                    break;
                case stRScaleArrangement:
                    bmpStr = ":/picture/image/rscalearrange";
                    break;
                case stTemperature:
                    bmpStr = ":/picture/image/tem";
                    break;
                case stTemHelp:
                    bmpStr = ":/picture/image/temhelp";
                    break;
                default:
                    break;
                }
                QString strI = QString::number(m_index);
                QPixmap icon(bmpStr+ strI+".bmp");
                if(m_style == stTemHelp)
                {
                    icon = QPixmap(bmpStr+".bmp");
                }
                this->m_button->resize(icon.width(),icon.height());
                this->m_button->setIconSize(icon.size());
                this->m_button->setIcon(icon);
        }
            break;
        default:
            break;
        }
}



