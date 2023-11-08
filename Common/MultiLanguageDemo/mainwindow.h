#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ILanguageObject.h"
#include "MDMultiLanguage.h"
#include "ITranslateWidgetTool.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public ILanguageObject
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

private:
    IDictionaryManager* m_dictManager;
    ITranslateWidgetTool* m_tool;
    QString m_labelText;

public:
    void changeLanguage();
    void collectWords();

};



#endif // MAINWINDOW_H
