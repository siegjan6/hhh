#ifndef CALENDARFORM_H
#define CALENDARFORM_H

#include <QWidget>
#include <QPushButton>
#include <QCalendarWidget>
#include <QLabel>
namespace Ui {
class CCalendarForm;
}

class CCalendarForm : public QWidget
{
    Q_OBJECT

public:
    explicit CCalendarForm(QWidget *parent = 0);
    ~CCalendarForm();

public:
    QCalendarWidget *calendar() const;

public:
    QPushButton* m_button;
    QLabel* m_label;
    QCalendarWidget* m_calendar;


private slots:
    void on_todayButton_clicked();

private:
    Ui::CCalendarForm *ui;
};

#endif // CALENDARFORM_H
