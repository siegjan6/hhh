#include "contentqsplitter.h"
#include <QTreeWidgetItem>

static QPlainTextEdit *  pOutputWidget = 0;

QPlainTextEdit *  GetOutputWindow()
{
    return pOutputWidget;
}

static void SetSplitterLROrTBScale(QSplitter *splitter, int ltScale, int tbScale, int totalSize)
{
  QList<int> spliterSize;
  int ltSize = totalSize / (ltScale + tbScale);
  spliterSize << ltScale * ltSize << tbScale*ltSize;
  splitter->setSizes(spliterSize);
}

ContentQSplitter::ContentQSplitter(QWidget *parent) :
    QSplitter(parent)
{
    setWindowState(Qt::WindowMaximized);//窗体最大化
    m_pLeftDisplayTreeWidget = new MainTreeWidget();
    SplitWidget();
    m_pLeftDisplayTreeWidget->setContainerWidget(m_pTabWidget);
}

void ContentQSplitter::ReloadUI()
{
    m_pLeftDisplayTreeWidget->clear();
    m_pTabWidget->clear();
    m_pOutputWidget->clear();

    m_pLeftDisplayTreeWidget->Reload();
}

//在主窗体show之后调用 否则不能按照预计值调整splitter子窗体大小
void ContentQSplitter::DisplayReSize()
{
    //设置左右窗体的大小比例1:5
    SetSplitterLROrTBScale(this, 1, 5, this->width());
    SetSplitterLROrTBScale(m_pRightDisplayWidget, 3, 1, m_pRightDisplayWidget->height());
}

void ContentQSplitter::SplitWidget()
{
    //m_pMainSplitter = new QSplitter(Qt::Horizontal, this);

    m_pRightDisplayWidget = new QSplitter(Qt::Vertical, this);
    //this->setHandleWidth(10);
    //this->setStyleSheet("QSplitter::handle { background-color: blue }");
    this->addWidget(m_pLeftDisplayTreeWidget);
    this->addWidget(m_pRightDisplayWidget);

    m_pTabWidget = new QTabWidget(m_pRightDisplayWidget);
    m_pOutputWidget = new QPlainTextEdit(m_pRightDisplayWidget);

    m_pTabWidget->setStyleSheet("QTabWidget{ background-color: blue }");
    //m_pTabWidget->set
    m_pRightDisplayWidget->addWidget(m_pTabWidget);
    m_pRightDisplayWidget->addWidget(m_pOutputWidget);
    pOutputWidget = m_pOutputWidget;
}
