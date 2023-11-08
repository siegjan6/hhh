#ifndef SCRIPTEDITOR_H
#define SCRIPTEDITOR_H

#include <QWidget>
#include <QString>

namespace Ui {
class ScriptEditor;
}

class ScriptEditor : public QWidget
{
    Q_OBJECT

public:
    explicit ScriptEditor(QWidget *parent = 0);
    ~ScriptEditor();
    void SetCaseName(QString caseName);
private slots:
    void on_saveBtn_clicked();

    void on_runBtn_clicked();

private:
    Ui::ScriptEditor *ui;
    QString m_CaseName;
};

#endif // SCRIPTEDITOR_H
