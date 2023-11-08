#include "pdfviewerwidget.h"
#include "ui_pdfviewerwidget.h"

#include "pageselector.h"
#include "zoomselector.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QPdfBookmarkModel>
#include <QPdfDocument>
#include <QPdfPageNavigation>
#include <QtMath>
#include <QDebug>

const qreal zoomMultiplier = qSqrt(2.0);

PdfViewerWidget::PdfViewerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PdfViewerWidget)
    , m_zoomSelector(new ZoomSelector(this))
    , m_pageSelector(new PageSelector(this))
    , m_document(new QPdfDocument(this))
{
    ui->setupUi(this);

    m_zoomSelector->setMaximumWidth(150);
    ui->mainToolBar->insertWidget(ui->actionZoom_In, m_zoomSelector);

    m_pageSelector->setMaximumWidth(150);
    ui->mainToolBar->addWidget(m_pageSelector);

    m_pageSelector->setPageNavigation(ui->pdfView->pageNavigation());

    connect(m_zoomSelector, &ZoomSelector::zoomModeChanged, ui->pdfView, &QPdfView::setZoomMode);
    connect(m_zoomSelector, &ZoomSelector::zoomFactorChanged, ui->pdfView, &QPdfView::setZoomFactor);
    m_zoomSelector->reset();

    QPdfBookmarkModel *bookmarkModel = new QPdfBookmarkModel(this);
    bookmarkModel->setDocument(m_document);

    ui->bookmarkView->setModel(bookmarkModel);
    connect(ui->bookmarkView, SIGNAL(activated(QModelIndex)), this, SLOT(bookmarkSelected(QModelIndex)));

    ui->tabWidget->setTabEnabled(1, false); // disable 'Pages' tab for now
    ui->tabWidget->setVisible(false);
    ui->menuBar->setVisible(false);
    ui->statusBar->setVisible(false);

    ui->pdfView->setDocument(m_document);

    connect(ui->pdfView, &QPdfView::zoomFactorChanged,
            m_zoomSelector, &ZoomSelector::setZoomFactor);
}

PdfViewerWidget::~PdfViewerWidget()
{
    delete ui;
}

void PdfViewerWidget::setDrawEnvrionment(IDrawEnvrionment *value)
{
    m_pEnv = value;
}

void PdfViewerWidget::open(const QUrl &docLocation)
{
    if (docLocation.isLocalFile()) {
        m_document->load(docLocation.toLocalFile());
        const auto documentTitle = m_document->metaData(QPdfDocument::Title).toString();
        setWindowTitle(!documentTitle.isEmpty() ? documentTitle : QStringLiteral("PDF Viewer"));
    } else {
        qDebug() << docLocation << "is not a valid local file";
        QMessageBox::critical(this, tr("Failed to open"), tr("%1 is not a valid local file").arg(docLocation.toString()));
    }
    qDebug() << docLocation;
}

void PdfViewerWidget::bookmarkSelected(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    const int page = index.data(QPdfBookmarkModel::PageNumberRole).toInt();
    ui->pdfView->pageNavigation()->setCurrentPage(page);
}

void PdfViewerWidget::on_actionOpen_triggered()
{
    QUrl toOpen = QFileDialog::getOpenFileUrl(this, tr("Choose a PDF"), QUrl(), "Portable Documents (*.pdf)");
    if (toOpen.isValid())
        open(toOpen);
}

void PdfViewerWidget::on_actionQuit_triggered()
{
    QApplication::quit();
}

void PdfViewerWidget::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About PdfViewer"),
        tr("An example using QPdfDocument"));
}

void PdfViewerWidget::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this);
}

void PdfViewerWidget::on_actionZoom_In_triggered()
{
    ui->pdfView->setZoomFactor(ui->pdfView->zoomFactor() * zoomMultiplier);
}

void PdfViewerWidget::on_actionZoom_Out_triggered()
{
    ui->pdfView->setZoomFactor(ui->pdfView->zoomFactor() / zoomMultiplier);
}

void PdfViewerWidget::on_actionPrevious_Page_triggered()
{
    ui->pdfView->pageNavigation()->goToPreviousPage();
}

void PdfViewerWidget::on_actionNext_Page_triggered()
{
    ui->pdfView->pageNavigation()->goToNextPage();
}

void PdfViewerWidget::on_actionContinuous_triggered()
{
    ui->pdfView->setPageMode(ui->actionContinuous->isChecked() ? QPdfView::MultiPage : QPdfView::SinglePage);
}

void PdfViewerWidget::setPageMode(int mode)
{
    ui->pdfView->setPageMode((QPdfView::PageMode)mode);
}
