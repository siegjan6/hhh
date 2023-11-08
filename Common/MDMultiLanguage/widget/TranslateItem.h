/*!
\author aGRw 2016.01.11
\section Widget控件翻译，具体翻译类
*/
#ifndef TRANSLATEITEM_H
#define TRANSLATEITEM_H

#include <QObject>
#include <QList>

class CTranslateWidgetTool;

class CTranslateItem
{
public:
    static CTranslateItem* create(QObject* translateWidget, CTranslateWidgetTool* container);
    CTranslateItem(QObject* translateWidget, CTranslateWidgetTool* container);
    virtual ~CTranslateItem();
public:
    //!翻译
    void changeLanguage();
protected:
    //!翻译m_translateWidget控件
    virtual void changeSelfLanguage(){}

protected:
    void setText(const QString& value){m_text = value;}
    QString text(){return m_text;}
    QObject* object(){return m_translateWidget;}
    QString translateText(const QString& value);
private:
    //!初始文本
    QString m_text;
    //!提示信息
    QString m_toolTip;
    QObject* m_translateWidget;
    QList<CTranslateItem*> m_list;
    CTranslateWidgetTool* m_container;
};

class CTranslateAbstractButton : public CTranslateItem
{
public:
    CTranslateAbstractButton(QObject* translateWidget, CTranslateWidgetTool* container);
    virtual ~CTranslateAbstractButton(){}
protected:
    virtual void changeSelfLanguage();
};

class CTranslateListWidget : public CTranslateItem
{
public:
    CTranslateListWidget(QObject* translateWidget, CTranslateWidgetTool* container);
    virtual ~CTranslateListWidget(){}
protected:
    virtual void changeSelfLanguage();
private:
    QStringList m_textList;
};

class CTranslateTreeWidget : public CTranslateItem
{
public:
    CTranslateTreeWidget(QObject* translateWidget, CTranslateWidgetTool* container);
    virtual ~CTranslateTreeWidget(){}
protected:
    virtual void changeSelfLanguage();
private:
    QStringList m_textList;
};

class CTranslateTableWidget : public CTranslateItem
{
public:
    CTranslateTableWidget(QObject* translateWidget, CTranslateWidgetTool* container);
    virtual ~CTranslateTableWidget(){}
protected:
    virtual void changeSelfLanguage();
private:
    QStringList m_horiList;
    QStringList m_vertList;
};

class CTranslateComboBox : public CTranslateItem
{
public:
    CTranslateComboBox(QObject* translateWidget, CTranslateWidgetTool* container);
    virtual ~CTranslateComboBox(){}
protected:
    virtual void changeSelfLanguage();
private:
    QStringList m_textList;
};

class CTranslateLineEdit : public CTranslateItem
{
public:
    CTranslateLineEdit(QObject* translateWidget, CTranslateWidgetTool* container);
    virtual ~CTranslateLineEdit(){}
protected:
    virtual void changeSelfLanguage();
};

class CTranslateTextEdit : public CTranslateItem
{
public:
    CTranslateTextEdit(QObject* translateWidget, CTranslateWidgetTool* container);
    virtual ~CTranslateTextEdit(){}
protected:
    virtual void changeSelfLanguage();
};

class CTranslateLabel : public CTranslateItem
{
public:
    CTranslateLabel(QObject* translateWidget, CTranslateWidgetTool* container);
    virtual ~CTranslateLabel(){}
protected:
    virtual void changeSelfLanguage();
};

class CTranslateGroupBox : public CTranslateItem
{
public:
    CTranslateGroupBox(QObject* translateWidget, CTranslateWidgetTool* container);
    virtual ~CTranslateGroupBox(){}
protected:
    virtual void changeSelfLanguage();
};

class CTranslateTabWidget : public CTranslateItem
{
public:
    CTranslateTabWidget(QObject* translateWidget, CTranslateWidgetTool* container);
    virtual ~CTranslateTabWidget(){}
protected:
    virtual void changeSelfLanguage();
private:
    QStringList m_textList;
};

class CTranslateDialog : public CTranslateItem
{
public:
    CTranslateDialog(QObject* translateWidget, CTranslateWidgetTool* container);
    virtual ~CTranslateDialog(){}
protected:
    virtual void changeSelfLanguage();
};

class CTranslateAction : public CTranslateItem
{
public:
    CTranslateAction(QObject* translateWidget, CTranslateWidgetTool* container);
    virtual ~CTranslateAction(){}
protected:
    virtual void changeSelfLanguage();
private:
    QString m_tipText;
    QString m_statusTip;
};

#endif // TRANSLATEITEM_H

