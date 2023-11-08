/*!
\author aGRw 2016.01.07
\section 多语言，工具栏控件
*/
#ifndef LANGUAGETOOLWIDGET_H
#define LANGUAGETOOLWIDGET_H

#include <QWidget>
#include "ITranslateWidgetTool.h"

class CDictionaryManager;

namespace Ui {
class CLanguageToolWidget;
}

class CLanguageToolWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CLanguageToolWidget(QWidget *parent = 0);
    virtual ~CLanguageToolWidget();
private slots:
    void on_comboBoxSystem_currentIndexChanged(int index);
    void on_comboBoxProject_currentIndexChanged(int index);
    void on_pushButton_clicked();
private:
    Ui::CLanguageToolWidget *ui;

private:
    bool m_isValid;//!comboBox事件触发是否有效
    CDictionaryManager *m_manager;
    ITranslateWidgetTool* m_transTool;
public:
    void init(CDictionaryManager *manager);
    void resetWidget();
    void changeLanguage();
private:
    void setCurrentLanguage(int selectIndex, bool isSystem = true);
    int getFactIndex(int selectIndex);
    int getSelectIndex(int factIndex);
};

#endif // LANGUAGETOOLWIDGET_H
