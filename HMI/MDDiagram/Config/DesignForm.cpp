#include <QPainter>
#include <QColorDialog>
#include <QFontDialog>
#include "DesignForm.h"
#include "ui_DesignForm.h"
#include "MDMultiLanguage.h"

#include "DrawWidget.h"

CDesignForm::CDesignForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDesignForm)
{
    ui->setupUi(this);
    setWindowTitle(tr("属性配置"));
    ui->listWidget->setCurrentRow(0);
    m_designData = new CDrawData;
//    ui->applyBtn->setEnabled(false);
    QObject::connect(ui->listWidget,SIGNAL(itemClicked(QListWidgetItem*)),
                     this,SLOT(setStackWidget(QListWidgetItem*)));
    QStringList types;
    types << "QLineEdit";
    CMDMultiLanguage::instance()->translateWidget(this, &types);
}

CDesignForm::~CDesignForm()
{
    delete ui;
}

void CDesignForm::inintDesign()
{
    setButtonColor(ui->backColorBtn,m_designData->m_backColor);
    setButtonColor(ui->interColorBtn,m_designData->m_interColor);
    setButtonColor(ui->lineColorBtn,m_designData->m_lineColor);
    setButtonColor(ui->describeColorBtn,m_designData->m_stringColor);
    setDiagramStyle(m_designData->m_diagramStyle);

    if(m_designData->m_setYValue == 0)
    {
        ui->maxValueEdit->setDisabled(true);
        ui->minValueEdit->setDisabled(true);
    }

    setStrPosition(m_designData->m_stringPosition);
    setStrArrange(m_designData->m_stringArrange);
    ui->clientEdgeCheck->setChecked(m_designData->m_isSetEdge);
    ui->comboBox->setCurrentIndex(m_designData->m_typePosition);
    ui->diagramCategoryBox->setCurrentIndex(m_designData->m_category);
    ui->toolBarBox->setCurrentIndex(m_designData->m_toolPosition);
    ui->setYcheckBox->setChecked(m_designData->m_setYValue);

    QString str;
    ui->describeEdit->setText(m_designData->m_stringText);
//    str.setNum(m_designData->m_typeDecimals);
//    ui->valueDecimalEdit->setText(str);
    str.setNum(m_designData->m_maxValue);
    ui->maxValueEdit->setText(str);
    str.setNum(m_designData->m_minValue);
    ui->minValueEdit->setText(str);
    str.setNum(m_designData->m_mainScaleNum);
    ui->mainScaleEdit->setText(str);
    str.setNum(m_designData->m_subScaleNum);
    ui->subScaleEdit->setText(str);
    str.setNum(m_designData->m_decimals);
    ui->scaleDecimalEdit->setText(str);

    ui->XDescribeEdit->setText(m_designData->m_string1);
    ui->setText2Edit->setText(m_designData->m_string2);
    ui->setText3Edit->setText(m_designData->m_string3);
    ui->setValueEdit->setText(m_designData->m_valueString);
    diagramCategorySelect(m_designData->m_category);

    connect(ui->comboBox,SIGNAL(currentIndexChanged(qint32)),this,SLOT(positionSelect(qint32)));
    connect(ui->toolBarBox,SIGNAL(currentIndexChanged(qint32)),this,SLOT(showToolBar(qint32)));
    connect(ui->clientEdgeCheck,SIGNAL(stateChanged(qint32)),this,SLOT(clientEdge(qint32)));
    connect(ui->setYcheckBox,SIGNAL(stateChanged(qint32)),this,SLOT(isSetYvalue(qint32)));
    connect(ui->diagramCategoryBox,SIGNAL(currentIndexChanged(qint32)),this,SLOT(diagramCategorySelect(qint32)));
}

void CDesignForm::setDesignData(CDrawData *data)
{
    *m_designData = *data;
}

CDrawData *CDesignForm::getData()
{
    return m_designData;
}

void CDesignForm::setDiagramStyle(qint32 style)
{
    switch(style)
    {
    case 1:
        ui->histogramBtn->setDown(true);
        ui->histogram2Btn->setDown(false);
        ui->histogram3DBtn->setDown(false);
        ui->histogram3D2Btn->setDown(false);
        break;
    case 2:
        ui->histogramBtn->setDown(false);
        ui->histogram2Btn->setDown(true);
        ui->histogram3DBtn->setDown(false);
        ui->histogram3D2Btn->setDown(false);
        break;
    case 3:
        ui->histogramBtn->setDown(false);
        ui->histogram2Btn->setDown(false);
        ui->histogram3DBtn->setDown(true);
        ui->histogram3D2Btn->setDown(false);
        break;
    case 4:
        ui->histogramBtn->setDown(false);
        ui->histogram2Btn->setDown(false);
        ui->histogram3DBtn->setDown(false);
        ui->histogram3D2Btn->setDown(true);
        break;
    default:
        break;
    }
}
void CDesignForm::setStrPosition(qint32 position)
{
    switch(position)
    {
    case 1:
        ui->topBtn->setDown(true);
        break;
    case 2:
        ui->bottomBtn->setDown(true);
        break;
    default:
        break;
    }
}

void CDesignForm::setStrArrange(qint32 arrange)
{
    switch(arrange)
    {
    case 1:
        ui->leftBtn->setDown(true);
        break;
    case 2:
        ui->middleBtn->setDown(true);
        break;
    case 3:
        ui->rightBtn->setDown(true);
        break;
    default:
        break;
    }
}

void CDesignForm::setButtonColor(QPushButton *button, QColor color)
{
    button->setStyleSheet(QString("background-color:rgb(%1,%2,%3,%4);")
                             .arg(color.red())
                             .arg(color.green())
                             .arg(color.blue())
                             .arg(color.alpha()));
}

void CDesignForm::positionSelect(qint32 index)
{
    m_designData->m_typePosition = index;
}

void CDesignForm::showToolBar(qint32 index)
{
    m_designData->m_toolPosition = index;
}

void CDesignForm::clientEdge(qint32 isVisible)
{
    m_designData->m_isSetEdge = isVisible;
}

void CDesignForm::isSetYvalue(qint32 num)
{
    m_designData->m_setYValue = num;
    if(num == 0)
    {
        ui->maxValueEdit->setDisabled(true);
        ui->minValueEdit->setDisabled(true);
    }
    else
    {
        ui->maxValueEdit->setDisabled(false);
        ui->minValueEdit->setDisabled(false);
    }
}

void CDesignForm::diagramCategorySelect(qint32 num)
{
    m_designData->m_category = num;
    if(num == 0)
    {
        ui->XDescribeEdit->setDisabled(false);
        ui->setText2Edit->setDisabled(true);
        ui->setText3Edit->setDisabled(true);
    }
    else if(num == 1)
    {
        ui->XDescribeEdit->setDisabled(false);
        ui->setText2Edit->setDisabled(false);
        ui->setText3Edit->setDisabled(true);
    }
    else
    {
        ui->XDescribeEdit->setDisabled(false);
        ui->setText2Edit->setDisabled(false);
        ui->setText3Edit->setDisabled(false);
    }
}

void CDesignForm::setStackWidget(QListWidgetItem *item)
{
    qint32 num = ui->listWidget->row(item);
    ui->stackedWidget->setCurrentIndex(num);
}

void CDesignForm::on_backColorBtn_clicked()
{
    QColor c = QColorDialog::getColor(m_designData->m_backColor,this);
    if(c.isValid())
    {
        m_designData->m_backColor = c;
        setButtonColor(ui->backColorBtn,c);
    }
}

void CDesignForm::on_interColorBtn_clicked()
{
    QColor c = QColorDialog::getColor(m_designData->m_interColor,this);
    if(c.isValid())
    {
        m_designData->m_interColor = c;
        setButtonColor(ui->interColorBtn,c);
    }
}
void CDesignForm::on_OKBtn_clicked()
{
        QString str = "";
        m_designData->m_stringText = ui->describeEdit->text();
        if(!ui->maxValueEdit->text().isEmpty())
        {
            str = ui->maxValueEdit->text();
            m_designData->m_maxValue = str.toLongLong();
        }
        if(!ui->minValueEdit->text().isEmpty())
        {
            str = ui->minValueEdit->text();
            m_designData->m_minValue = str.toLongLong();
        }
        if(!ui->mainScaleEdit->text().isEmpty())
        {
            str = ui->mainScaleEdit->text();
            if(str.toLongLong() >= 1)
            {
                m_designData->m_mainScaleNum = str.toLongLong();
            }

        }
        if(!ui->subScaleEdit->text().isEmpty())
        {
            str = ui->subScaleEdit->text();
            if(str.toLongLong() >=0)
            {
                m_designData->m_subScaleNum = str.toLongLong();
            }
        }
        if(!ui->scaleDecimalEdit->text().isEmpty())
        {
            str = ui->scaleDecimalEdit->text();
            if(str.toLongLong() >= 0)
            {
                m_designData->m_decimals = str.toLongLong();
            }
        }

        if(!ui->XDescribeEdit->toPlainText().isEmpty())
        {
            m_designData->m_string1 = ui->XDescribeEdit->toPlainText();
        }
        if(!ui->setText2Edit->toPlainText().isEmpty())
        {
            m_designData->m_string2 = ui->setText2Edit->toPlainText();
        }
        if(!ui->setText3Edit->toPlainText().isEmpty())
        {
            m_designData->m_string3 = ui->setText3Edit->toPlainText();
        }
        if(!ui->setValueEdit->toPlainText().isEmpty())
        {
            m_designData->m_valueString = ui->setValueEdit->toPlainText();
        }
        accept();
}

void CDesignForm::on_cancelBtn_clicked()
{
    reject();
}

void CDesignForm::on_applyBtn_clicked()
{
        QString str = "";
        m_designData->m_stringText = ui->describeEdit->text();
        if(!ui->maxValueEdit->text().isEmpty())
        {
            str = ui->maxValueEdit->text();
            m_designData->m_maxValue = str.toLongLong();
        }
        if(!ui->minValueEdit->text().isEmpty())
        {
            str = ui->minValueEdit->text();
            m_designData->m_minValue = str.toLongLong();
        }
        if(!ui->mainScaleEdit->text().isEmpty())
        {
            str = ui->mainScaleEdit->text();
            if(str.toLongLong() >= 1)
            {
                m_designData->m_mainScaleNum = str.toLongLong();
            }

        }
        if(!ui->subScaleEdit->text().isEmpty())
        {
            str = ui->subScaleEdit->text();
            if(str.toLongLong() >=0)
            {
                m_designData->m_subScaleNum = str.toLongLong();
            }
        }
        if(!ui->scaleDecimalEdit->text().isEmpty())
        {
            str = ui->scaleDecimalEdit->text();
            if(str.toLongLong() >= 0)
            {
                m_designData->m_decimals = str.toLongLong();
            }
        }

        if(!ui->XDescribeEdit->toPlainText().isEmpty())
        {
            m_designData->m_string1 = ui->XDescribeEdit->toPlainText();
        }
        if(!ui->setText2Edit->toPlainText().isEmpty())
        {
            m_designData->m_string2 = ui->setText2Edit->toPlainText();
        }
        if(!ui->setText3Edit->toPlainText().isEmpty())
        {
            m_designData->m_string3 = ui->setText3Edit->toPlainText();
        }
        if(!ui->setValueEdit->toPlainText().isEmpty())
        {
            m_designData->m_valueString = ui->setValueEdit->toPlainText();
        }

        *m_drawWidget->m_drawData = *getData();
        m_drawWidget->setValue(getData()->m_valueString);
        m_drawWidget->setString1(getData()->m_string1);
        m_drawWidget->setString2(getData()->m_string2);
        m_drawWidget->setString3(getData()->m_string3);
}

void CDesignForm::on_lineColorBtn_clicked()
{
    QColor c = QColorDialog::getColor(m_designData->m_lineColor,this);
    if(c.isValid())
    {
        m_designData->m_lineColor = c;
        setButtonColor(ui->lineColorBtn,c);
    }
}

void CDesignForm::on_diagramFontBtn_clicked()
{
    QFontDialog fontDialog(this);
    fontDialog.setCurrentFont(m_designData->m_diagramFont);
    fontDialog.show();
    if(fontDialog.exec() != QDialog::Accepted)
          return;
    QFont font = fontDialog.selectedFont();
    m_designData->m_diagramFont = font;
}

void CDesignForm::on_valueFontBtn_clicked()
{
    QFontDialog fontDialog(this);
    fontDialog.setCurrentFont(m_designData->m_typeValueFont);
    fontDialog.show();
    if(fontDialog.exec() != QDialog::Accepted)
          return;
    QFont font = fontDialog.selectedFont();
    m_designData->m_typeValueFont = font;

}

void CDesignForm::on_discribeFontBtn_clicked()
{
    QFontDialog fontDialog(this);
    fontDialog.setCurrentFont(m_designData->m_stringFont);
    fontDialog.show();
    if(fontDialog.exec() != QDialog::Accepted)
          return;
    QFont font = fontDialog.selectedFont();
    m_designData->m_stringFont = font;
}

void CDesignForm::on_topBtn_clicked()
{
    m_designData->m_stringPosition = 1;
    ui->topBtn->setDown(true);
    ui->bottomBtn->setDown(false);
}

void CDesignForm::on_bottomBtn_clicked()
{
    m_designData->m_stringPosition = 2;
    ui->topBtn->setDown(false);
    ui->bottomBtn->setDown(true);

}

void CDesignForm::on_leftBtn_clicked()
{
    m_designData->m_stringArrange = 1;
    ui->leftBtn->setDown(true);
    ui->middleBtn->setDown(false);
    ui->rightBtn->setDown(false);
}

void CDesignForm::on_middleBtn_clicked()
{
    m_designData->m_stringArrange = 2;
    ui->leftBtn->setDown(false);
    ui->middleBtn->setDown(true);
    ui->rightBtn->setDown(false);
}

void CDesignForm::on_rightBtn_clicked()
{
    m_designData->m_stringArrange = 3;
    ui->leftBtn->setDown(false);
    ui->middleBtn->setDown(false);
    ui->rightBtn->setDown(true);
}

void CDesignForm::on_histogramBtn_clicked()
{
    m_designData->m_diagramStyle = 1;
    ui->histogramBtn->setDown(true);
    ui->histogram2Btn->setDown(false);
    ui->histogram3DBtn->setDown(false);
    ui->histogram3D2Btn->setDown(false);
}

void CDesignForm::on_histogram2Btn_clicked()
{
    m_designData->m_diagramStyle = 2;
    ui->histogramBtn->setDown(false);
    ui->histogram2Btn->setDown(true);
    ui->histogram3DBtn->setDown(false);
    ui->histogram3D2Btn->setDown(false);
}

void CDesignForm::on_describeColorBtn_clicked()
{
    QColor c = QColorDialog::getColor(m_designData->m_stringColor,this);
    if(c.isValid())
    {
        m_designData->m_stringColor = c;
        setButtonColor(ui->describeColorBtn,c);
    }
}

void CDesignForm::on_histogram3DBtn_clicked()
{
    m_designData->m_diagramStyle = 3;
    ui->histogramBtn->setDown(false);
    ui->histogram2Btn->setDown(false);
    ui->histogram3DBtn->setDown(true);
    ui->histogram3D2Btn->setDown(false);
}

void CDesignForm::on_histogram3D2Btn_clicked()
{
    m_designData->m_diagramStyle = 4;
    ui->histogramBtn->setDown(false);
    ui->histogram2Btn->setDown(false);
    ui->histogram3DBtn->setDown(false);
    ui->histogram3D2Btn->setDown(true);
}
