#ifndef PDFVIEWERWIDGET_H
#define PDFVIEWERWIDGET_H

#include <QWidget>
#include "IDrawEnvironment.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class PdfViewerWidget;
}

class QPdfDocument;
class QPdfView;
QT_END_NAMESPACE

class PageSelector;
class ZoomSelector;


class PdfViewerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PdfViewerWidget(QWidget *parent = nullptr);
    ~PdfViewerWidget();

    void setDrawEnvrionment(IDrawEnvrionment *value);
    // mode: 0 - SinglePage, 1 - MultiPage
    void setPageMode(int mode);

public slots:
    void open(const QUrl &docLocation);

private slots:
    void bookmarkSelected(const QModelIndex &index);

    // action handlers
    void on_actionOpen_triggered();
    void on_actionQuit_triggered();
    void on_actionAbout_triggered();
    void on_actionAbout_Qt_triggered();
    void on_actionZoom_In_triggered();
    void on_actionZoom_Out_triggered();
    void on_actionPrevious_Page_triggered();
    void on_actionNext_Page_triggered();
    void on_actionContinuous_triggered();

private:
    Ui::PdfViewerWidget *ui;
    ZoomSelector *m_zoomSelector;
    PageSelector *m_pageSelector;

    QPdfDocument *m_document;

    IDrawEnvrionment* m_pEnv;
};

#endif // PDFVIEWERWIDGET_H
