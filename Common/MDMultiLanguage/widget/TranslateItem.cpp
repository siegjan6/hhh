#include <QAbstractButton>
#include <QListWidget>
#include <QTreeWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QGroupBox>
#include <QTabWidget>
#include <QAction>
#include <QTableWidgetItem>
#include <QHeaderView>
#include "TranslateItem.h"
#include "MDMultiLanguage.h"
#include "TranslateWidgetTool.h"
////////////////////////////////////////////////////////////
CTranslateItem *CTranslateItem::create(QObject *translateWidget, CTranslateWidgetTool *container)
{
    if(container->isSeveredObject(translateWidget))
        return NULL;

    CTranslateItem* item = NULL;
    if(container->isValidObject(translateWidget))
    {
        if(translateWidget->inherits("QAbstractButton"))
            item = new CTranslateAbstractButton(translateWidget, container);
        else if(translateWidget->inherits("QListWidget"))
            item = new CTranslateListWidget(translateWidget, container);
        else if(translateWidget->inherits("QTreeWidget"))
            item = new CTranslateTreeWidget(translateWidget, container);
        else if(translateWidget->inherits("QTableWidget"))
            item = new CTranslateTableWidget(translateWidget, container);
        else if(translateWidget->inherits("QComboBox"))
            item = new CTranslateComboBox(translateWidget, container);
        else if(translateWidget->inherits("QLineEdit"))
            item = new CTranslateLineEdit(translateWidget, container);
        else if(translateWidget->inherits("QTextEdit"))
            item = new CTranslateTextEdit(translateWidget, container);
        else if(translateWidget->inherits("QLabel"))
            item = new CTranslateLabel(translateWidget, container);
        else if(translateWidget->inherits("QGroupBox"))
            item = new CTranslateGroupBox(translateWidget, container);
        else if(translateWidget->inherits("QTabWidget"))
            item = new CTranslateTabWidget(translateWidget, container);
        else if(translateWidget->inherits("QDialog")
                || translateWidget->inherits("QMainWindow")
                || translateWidget->inherits("QDockWidget"))
            item = new CTranslateDialog(translateWidget, container);
        else if(translateWidget->inherits("QAction"))
            item = new CTranslateAction(translateWidget, container);
        else
            item = new CTranslateItem(translateWidget, container);
    }
    else
        item = new CTranslateItem(translateWidget, container);

    return item;
}
CTranslateItem::CTranslateItem(QObject *translateWidget, CTranslateWidgetTool *container)
{
    m_translateWidget = translateWidget;
    m_container = container;

    QObjectList l = m_translateWidget->children();
    int count = l.count();
    for(int i = 0; i < count; i++)
    {
        CTranslateItem* item = CTranslateItem::create(l.at(i), m_container);
        if(item != NULL)
            m_list.append(item);
    }

    QWidget* w = qobject_cast<QWidget*>(object());
    if(w != NULL)
        m_toolTip = w->toolTip();
}
CTranslateItem::~CTranslateItem()
{
    qDeleteAll(m_list.begin(), m_list.end());
    m_list.clear();
}
void CTranslateItem::changeLanguage()
{
    changeSelfLanguage();

    int count = m_list.count();
    for(int i = 0; i < count; i++)
        m_list.at(i)->changeLanguage();

    if(!m_toolTip.isEmpty())
    {
        QWidget* w = qobject_cast<QWidget*>(object());
        if(w != NULL)
            w->setToolTip(translateText(m_toolTip));
    }
}
QString CTranslateItem::translateText(const QString &value)
{
    return CMDMultiLanguage::instance()->value(value, m_container->isSystem());
}
////////////////////////////////////////////////////////////
CTranslateAbstractButton::CTranslateAbstractButton(QObject *translateWidget, CTranslateWidgetTool *container)
    : CTranslateItem(translateWidget, container)
{
    setText(qobject_cast<QAbstractButton*>(object())->text());
}
void CTranslateAbstractButton::changeSelfLanguage()
{
    QAbstractButton* w = qobject_cast<QAbstractButton*>(object());
    w->setText(translateText(text()));
}
////////////////////////////////////////////////////////////
CTranslateListWidget::CTranslateListWidget(QObject *translateWidget, CTranslateWidgetTool* container)
    : CTranslateItem(translateWidget, container)
{
    QListWidget* w = qobject_cast<QListWidget*>(object());
    int count = w->count();
    for(int i = 0; i < count; i++)
        m_textList.append(w->item(i)->text());
}
void CTranslateListWidget::changeSelfLanguage()
{
    QListWidget* w = qobject_cast<QListWidget*>(object());
    int count = m_textList.count();
    for(int i = 0; i < count; i++)
        w->item(i)->setText(translateText(m_textList.at(i)));
}
////////////////////////////////////////////////////////////
CTranslateTreeWidget::CTranslateTreeWidget(QObject *translateWidget, CTranslateWidgetTool* container)
    : CTranslateItem(translateWidget, container)
{
    QTreeWidget* w = qobject_cast<QTreeWidget*>(object());
    int count = w->headerItem()->columnCount();
    for(int i = 0; i < count; i++)
        m_textList.append(w->headerItem()->text(i));
}
void CTranslateTreeWidget::changeSelfLanguage()
{
    QTreeWidget* w = qobject_cast<QTreeWidget*>(object());
    int count = m_textList.count();
    for(int i = 0; i < count; i++)
        w->headerItem()->setText(i, translateText(m_textList.at(i)));
}
////////////////////////////////////////////////////////////
CTranslateTableWidget::CTranslateTableWidget(QObject *translateWidget, CTranslateWidgetTool* container)
    : CTranslateItem(translateWidget, container)
{
    QTableWidget* w = qobject_cast<QTableWidget*>(object());
    int count = w->horizontalHeader()->count();
    for(int i = 0; i < count; i++)
    {
        QTableWidgetItem* item = w->horizontalHeaderItem(i);
        if(item != NULL)
            m_horiList.append(item->text());
    }
    count = w->verticalHeader()->count();
    for(int i = 0; i < count; i++)
    {
        QTableWidgetItem* item = w->verticalHeaderItem(i);
        if(item != NULL)
            m_vertList.append(item->text());
    }
}
void CTranslateTableWidget::changeSelfLanguage()
{
    QTableWidget* w = qobject_cast<QTableWidget*>(object());
    int count = m_horiList.count();
    for(int i = 0; i < count; i++)
        w->horizontalHeaderItem(i)->setText(translateText(m_horiList.at(i)));
    count = m_vertList.count();
    for(int i = 0; i < count; i++)
        w->verticalHeaderItem(i)->setText(translateText(m_vertList.at(i)));
}
////////////////////////////////////////////////////////////
CTranslateComboBox::CTranslateComboBox(QObject *translateWidget, CTranslateWidgetTool* container)
    : CTranslateItem(translateWidget, container)
{
    QComboBox* w = qobject_cast<QComboBox*>(object());
    int count = w->count();
    for(int i = 0; i < count; i++)
        m_textList.append(w->itemText(i));
}
void CTranslateComboBox::changeSelfLanguage()
{
    QComboBox* w = qobject_cast<QComboBox*>(object());
    int count = m_textList.count();
    for(int i = 0; i < count; i++)
        w->setItemText(i, translateText(m_textList.at(i)));
}
////////////////////////////////////////////////////////////
CTranslateLineEdit::CTranslateLineEdit(QObject *translateWidget, CTranslateWidgetTool* container)
    : CTranslateItem(translateWidget, container)
{
    QLineEdit* w = qobject_cast<QLineEdit*>(object());
    setText(w->text());
}
void CTranslateLineEdit::changeSelfLanguage()
{
    QLineEdit* w = qobject_cast<QLineEdit*>(object());
    w->setText(translateText(text()));
}
////////////////////////////////////////////////////////////
CTranslateTextEdit::CTranslateTextEdit(QObject *translateWidget, CTranslateWidgetTool* container)
    : CTranslateItem(translateWidget, container)
{
    QTextEdit* w = qobject_cast<QTextEdit*>(object());
    setText(w->toPlainText());
}
void CTranslateTextEdit::changeSelfLanguage()
{
    QTextEdit* w = qobject_cast<QTextEdit*>(object());
    w->setText(translateText(text()));
}
////////////////////////////////////////////////////////////
CTranslateLabel::CTranslateLabel(QObject *translateWidget, CTranslateWidgetTool* container)
    : CTranslateItem(translateWidget, container)
{
    QLabel* w = qobject_cast<QLabel*>(object());
    setText(w->text());
}
void CTranslateLabel::changeSelfLanguage()
{
    QLabel* w = qobject_cast<QLabel*>(object());
    w->setText(translateText(text()));
}
////////////////////////////////////////////////////////////
CTranslateGroupBox::CTranslateGroupBox(QObject *translateWidget, CTranslateWidgetTool* container)
    : CTranslateItem(translateWidget, container)
{
    QGroupBox* w = qobject_cast<QGroupBox*>(object());
    setText(w->title());
}
void CTranslateGroupBox::changeSelfLanguage()
{
    QGroupBox* w = qobject_cast<QGroupBox*>(object());
    w->setTitle(translateText(text()));
}
////////////////////////////////////////////////////////////
CTranslateTabWidget::CTranslateTabWidget(QObject *translateWidget, CTranslateWidgetTool* container)
    : CTranslateItem(translateWidget, container)
{
    QTabWidget* w = qobject_cast<QTabWidget*>(object());
    int count = w->count();
    for(int i = 0; i < count; i++)
        m_textList.append(w->tabText(i));
}
void CTranslateTabWidget::changeSelfLanguage()
{
    QTabWidget* w = qobject_cast<QTabWidget*>(object());
    int count = m_textList.count();
    for(int i = 0; i < count; i++)
        w->setTabText(i, translateText(m_textList.at(i)));
}
////////////////////////////////////////////////////////////
CTranslateDialog::CTranslateDialog(QObject *translateWidget, CTranslateWidgetTool* container)
    : CTranslateItem(translateWidget, container)
{
    QWidget* w = qobject_cast<QWidget*>(object());
    setText(w->windowTitle());
}
void CTranslateDialog::changeSelfLanguage()
{
    QWidget* w = qobject_cast<QWidget*>(object());
    w->setWindowTitle(translateText(text()));
}
////////////////////////////////////////////////////////////
CTranslateAction::CTranslateAction(QObject *translateWidget, CTranslateWidgetTool* container)
    : CTranslateItem(translateWidget, container)
{
    QAction* w = qobject_cast<QAction*>(object());
    setText(w->text());
    m_tipText = w->toolTip();
    m_statusTip = w->statusTip();
}
void CTranslateAction::changeSelfLanguage()
{
    QAction* w = qobject_cast<QAction*>(object());
    w->setText(translateText(text()));
    w->setToolTip(translateText(m_tipText));
    w->setStatusTip(translateText(m_statusTip));
}
////////////////////////////////////////////////////////////

