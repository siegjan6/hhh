#ifndef TESTDATAEDITOR_H
#define TESTDATAEDITOR_H

#include <QWidget>
#include <QString>

namespace Ui {
class TestDataEditor;
}

class TestDataEditor : public QWidget
{
    Q_OBJECT

public:
    explicit TestDataEditor(QWidget *parent = 0);
    ~TestDataEditor();
    void SetTestDataFileName(QString testDataFileName);
private slots:
    void on_saveBtn_clicked();

private:
    Ui::TestDataEditor *ui;
    QString m_TestDataFileName;
};

#endif // TESTDATAEDITOR_H
