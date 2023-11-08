#ifndef MDBATCHMODIFYWIDGET_H
#define MDBATCHMODIFYWIDGET_H
#include <QObject>
#include <QWidget>
#include <QDockWidget>

namespace Ui {
class CMDBatchModifyWidget;
}

class CMDBatchModifyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CMDBatchModifyWidget(QWidget *parent = 0);
    ~CMDBatchModifyWidget();

private:
    Ui::CMDBatchModifyWidget *ui;

public:
    void setObjectModify(QObject* empty);
    void initUI();

private:
    void createDock();//! 创建Dock窗体

private slots:
    void closeWindow();
    void setModify();

private:
    QObject           *m_object;
};

#endif // MDBATCHMODIFYWIDGET_H
