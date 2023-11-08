#include "CalendarForm.h"
#include "ui_CalendarForm.h"

CCalendarForm::CCalendarForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CCalendarForm)
{
    ui->setupUi(this);

    m_button = ui->todayButton;
    m_label = ui->todayLabel;
    m_calendar = ui->calendarWidget;

    m_label->setText(QDate::currentDate().toString("yyyy/MM/dd"));
}

CCalendarForm::~CCalendarForm()
{
    delete ui;
}

QCalendarWidget *CCalendarForm::calendar() const
{
    return m_calendar;
}

void CCalendarForm::on_todayButton_clicked()
{
    m_calendar->setSelectedDate(QDate::currentDate());
}
