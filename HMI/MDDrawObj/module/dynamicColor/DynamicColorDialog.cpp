#include "DynamicColorDialog.h"
#include "ui_DynamicColorDialog.h"
#include "MDMultiLanguage.h"

CDynamicColorDialog::CDynamicColorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDynamicColorDialog)
{
    m_colors = CDynamicColorManager(DynamicType::Brush);//!数据
    m_width = 110;
    m_height = 70;
    hasChangedEvent = false;//!防止在初始化时触发cellChanged

    ui->setupUi(this);

    CMDMultiLanguage::instance()->translateWidget(this);

    initTableWidget();
}
CDynamicColorDialog::~CDynamicColorDialog()
{
    delete ui;
}
//////////////////////////////////////////////////////////////////////////
void CDynamicColorDialog::on_pushButtonAdd_clicked()
{
    hasChangedEvent = false;

    QTableWidget* table = ui->tableWidget;
    int row = table->rowCount();
    table->insertRow(row);
    for (int i = 0; i < 3; i++)
        table->setItem(row, i, new QTableWidgetItem());

    CDynamicColor* color = m_colors.insert(row);
    int r = qrand() % 255;
    int g = qrand() % 255;
    int b = qrand() % 255;
    color->initColor(QColor(r, g, b));

    fillItem(color, row);

    hasChangedEvent = true;
}
void CDynamicColorDialog::on_pushButtonDel_clicked()
{
    QTableWidget* table = ui->tableWidget;
    int row = table->currentRow();
    if (row == -1)
        return;

    table->removeRow(row);
    delete m_colors.list()->takeAt(row);

    if (row >= table->rowCount())
        row = table->rowCount() - 1;
    table->setCurrentCell(row, 1);
}
void CDynamicColorDialog::on_pushButtonOk_clicked()
{
    accept();
}
void CDynamicColorDialog::on_pushButtonCancel_clicked()
{
    reject();
}
//////////////////////////////////////////////////////////////////////////
void CDynamicColorDialog::init(CDynamicColorManager *colors)
{
    m_colors = *colors;
    CMDDrawObj* obj = colors->container();

    m_colors.setContainer(colors->container());

    QTableWidget* table = ui->tableWidget;
    table->clearContents();

    int count = m_colors.list()->count();
    table->setRowCount(count);
    for (int i = 0; i < count; i++)
        for (int j = 0; j < 3; j++)
            table->setItem(i, j, new QTableWidgetItem);

    for (int i = 0; i < count; i++)
        fillItem(m_colors.list()->at(i), i);

    hasChangedEvent = true;
}
void CDynamicColorDialog::initTableWidget()
{
    QTableWidget* table = ui->tableWidget;

    table->verticalHeader()->setDefaultSectionSize(m_height);
    table->verticalHeader()->hide();
    table->horizontalHeader()->setDefaultSectionSize(m_width);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(table, &QTableWidget::cellChanged, this, &CDynamicColorDialog::cellChanged);
    connect(table, &QTableWidget::cellDoubleClicked, this, &CDynamicColorDialog::cellDoubleClicked);
}
void CDynamicColorDialog::cellChanged(int row, int col)
{
    if (!hasChangedEvent)
        return;
    if (col == 0)
        return;

    QTableWidgetItem* item = ui->tableWidget->item(row, col);
    bool ok = false;
    double df = item->text().toDouble(&ok);
    CDynamicColor* color = m_colors.list()->at(row);

    if (col == 1)//min
    {
        if (ok)
            color->setMin(df);
        else
            df = color->min();
    }
    else if (col == 2)//max
    {
        if (ok)
            color->setMax(df);
        else
            df = color->max();
    }

    item->setText(QString::number(df));
}
void CDynamicColorDialog::cellDoubleClicked(int row, int col)
{
    if (col != 0)
        return;

    m_colors.showDialog(row);
    CDynamicColor* color = m_colors.list()->at(row);
    QRectF rf(0, 0, m_width, m_height);

    QBrush brush;
    if (m_colors.type() == DynamicType::Brush)
        brush = color->brush()->brush(rf);
    else
        brush = color->pen()->brushManager().brush(rf);
    ui->tableWidget->item(row, col)->setBackground(brush);
}
void CDynamicColorDialog::fillItem(CDynamicColor* color, int row)
{
    QTableWidget* table = ui->tableWidget;
    QRectF rf(0, 0, m_width, m_height);
    QBrush brush;
    if (m_colors.type() == DynamicType::Brush)
        brush = color->brush()->brush(rf);
    else
        brush = color->pen()->brushManager().brush(rf);

    //color
    table->item(row, 0)->setBackground(brush);
    table->item(row, 0)->setFlags(Qt::ItemIsEnabled);
    //min
    table->item(row, 1)->setText(QString::number(color->min()));
    //max
    table->item(row, 2)->setText(QString::number(color->max()));
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
