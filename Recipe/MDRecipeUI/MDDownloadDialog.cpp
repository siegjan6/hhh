#include "MDDownloadDialog.h"
#include "ui_MDDownloadDialog.h"
#include "MDRecipeExpert.h"
#include "MDRecipe.h"
#include <QWidget>
#include <QStringListModel>
#include <QDesktopWidget>

CMDDownloadDialog::CMDDownloadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMDDownloadDialog)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setRowCount(1);
    QStringList headers;
    headers << "ID" << "RecipeValue" << "VarValue";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->listViewTask->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_taskListModel = new QStringListModel(this);

    m_recipeExpert = NULL;
}

CMDDownloadDialog::~CMDDownloadDialog()
{
    delete ui;
}

void CMDDownloadDialog::handleDownloadTaskAdded(const QString &name, int showProcessDialog/*=1*/)
{
    setModal(false);
    if(showProcessDialog == 1)
    {
        setWindowFlags(Qt::WindowStaysOnTopHint);
        QDesktopWidget *desktop = QApplication::desktop();
        if(desktop)
            move((desktop->width() - width()) / 2, (desktop->height() - height()) / 2);
        show();
        //move(800, 400);

        if(!m_timer.isActive())
        {
            connect(&m_timer, SIGNAL(timeout()), this, SLOT(handleTaskOnTimer()));
            m_timer.start(1000);
        }
    }
    else
        hide();
}

void CMDDownloadDialog::handleBeginDownloading(const QString &name, int total)
{
    setWindowTitle(QString(QObject::tr("配方下载 - %1")).arg(name));
    ui->progressBar->setStyleSheet("QProgressBar{border: 1px solid grey;border-radius: 5px;text-align: center;}"
                           "QProgressBar::chunk{background-color: #CD96CD;width: 10px;margin: 0.5px;}");

    ui->progressBar->reset();
    ui->progressBar->setRange(0, total);
    ui->labelCompleted->setText(QString("0/%1").arg(total));
}

void CMDDownloadDialog::handleTaskOnTimer()
{
    if(m_recipeExpert->taskCount() <= 0)
    {
        hide();
        m_timer.stop();
    }
    else
    {
        QStringList taskNameList;
        m_recipeExpert->taskNameList(taskNameList);
        m_taskListModel->setStringList(taskNameList);
        ui->listViewTask->setModel(m_taskListModel);

        // 问题：刷新效率低！
        CMDRecipe remained;
        m_recipeExpert->downloadingTask(remained);
        QList<CMDRecipeItem*> itemList;
        remained.configProperty()->items()->enumItems(itemList);
        ui->tableWidget->setRowCount(remained.configProperty()->items()->count());
        int iRow = 0;
        foreach (CMDRecipeItem* item, itemList)
        {
            ui->tableWidget->setItem(iRow, 0, new QTableWidgetItem(item->id()));
            ui->tableWidget->setItem(iRow, 1, new QTableWidgetItem(item->value()));
            ui->tableWidget->setItem(iRow, 2, new QTableWidgetItem(m_recipeExpert->itemVarValue(item->id()).toString()));
            iRow ++;
        }
    }
}

void CMDDownloadDialog::handleDownloadProcess(const QString &msg, int total, int completed)
{
    ui->labelStatus->setText(msg);
    if(total > 0 && completed >= 0)
    {
        ui->progressBar->setValue(completed/total);
        ui->labelCompleted->setText(QString("%1/%2").arg(completed).arg(total));
    }
}
