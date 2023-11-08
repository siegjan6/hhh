#ifndef NCRDBORDERPANEL_H
#define NCRDBORDERPANEL_H

#include <QWidget>
#include <QColorDialog>
#include <QTranslator>
#include "ui_borderpanel.h"

class NCReportItem;
class QLineEdit;
class NCRDDocument;
class NCReportLabelItem;

/*!

\section 功能

高级选项管理类
【管理border条件，选项等配置】

*/
class NCRDBorderPanel : public QWidget, public Ui::NCRDBorderPanelUI
{
  Q_OBJECT
public:
    NCRDBorderPanel(QWidget* parent = 0 );
    ~NCRDBorderPanel();

    void setOptions(NCReportLabelItem* item , NCRDDocument *document);
    void getOptions( NCReportLabelItem* item );

public slots:
    void slotSetBorderColor();

private:
    NCReportLabelItem* m_item;
    NCRDDocument* m_document;
    QColor m_borderColor;


    void openEditDialog( QLineEdit* editor );

    inline void setButtonColor(const QColor &color, QPushButton &button)
    {
        button.setStyleSheet(QString("background-color: rgb(%1, %2, %3)")
                             .arg(color.red())
                             .arg(color.green())
                             .arg(color.blue()));
        m_borderColor = color;
    }

    inline int selectColor(QColor &initialColor, QPushButton &colorButton,QWidget *parent = 0)
    {
        QColorDialog dialog(parent);
        dialog.setCurrentColor(initialColor);
        int ret = dialog.exec();
        if(ret == QDialog::Accepted){
            QColor color = dialog.selectedColor();
            if(color.isValid()){
                setButtonColor(color,colorButton);
                initialColor = color;

                return QDialog::Accepted;
            }
        }
        return QDialog::Rejected;
    }

};

#endif

