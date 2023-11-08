#include <QFrame>
#include "MDDrawTextEdit.h"
#include "DrawTextEditData.h"
#include "MDDrawTexEditEvent.h"
#include "MDPropertyDrawTextEdit.h"
#include "MDMultiLanguage.h"
#include <QRegExpValidator>
#include <QRegExp>
#include <QChar>

CMDDrawTextEdit::CMDDrawTextEdit()
{
    m_data = new CDrawTextEditData;
    m_data->setTextEdit(new QTextEdit());
    m_data->setEvent(new CMDDrawTexEditEvent());

    setWidget(m_data->textEdit(), m_data->event());
    cursor = QTextCursor(m_data->textEdit()->document());
    //CMDMultiLanguage::instance()->translateWidget(this);
}
CMDDrawTextEdit::~CMDDrawTextEdit()
{
    delete m_data;
}
CDrawTextEditData *CMDDrawTextEdit::data() const
{
    return m_data;
}

QTextEdit *CMDDrawTextEdit::widget()
{
    return m_data->textEdit();
}

void CMDDrawTextEdit::setHorizontalScrollBar(bool value)
{
    m_data->setHorizontalScrollBar(value);

    if(value)
    {
        widget()->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
    else
    {
        widget()->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
}

bool CMDDrawTextEdit::horizontalScrollBar()
{
    return m_data->horizontalScrollBar();
}

void CMDDrawTextEdit::setVerticalScrollBar(bool value)
{
    m_data->setVerticalScrollBar(value);

    if(value)
    {
        widget()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
    else
    {
        widget()->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
}

bool CMDDrawTextEdit::verticalScrollBar()
{
    return m_data->verticalScrollBar();
}

void CMDDrawTextEdit::setBorderStyle(QFrame::Shape value)
{
    m_data->setBorderStyle(value);
    m_data->textEdit()->setFrameShape(value);
}
QFrame::Shape CMDDrawTextEdit::borderStyle()
{
    return (QFrame::Shape)m_data->borderStyle();
}

void CMDDrawTextEdit::setLineWrapMode(QTextEdit::LineWrapMode value)
{
    m_data->setLineWrapMode(value);
    m_data->textEdit()->setLineWrapMode(value);
}

QTextEdit::LineWrapMode CMDDrawTextEdit::lineWrapMode()
{
    return m_data->lineWrapMode();
}

void CMDDrawTextEdit::setReadOnly(bool value)
{
    m_data->setReadOnly(value);
    m_data->textEdit()->setReadOnly(value);
}
bool CMDDrawTextEdit::readOnly()
{
    return m_data->readOnly();
}
void CMDDrawTextEdit::setAlignment(Qt::Alignment value)
{
    m_data->setAlignment(value);
    m_data->textEdit()->document()->setDefaultTextOption(QTextOption(value));
}

Qt::Alignment CMDDrawTextEdit::alignment()
{
    return m_data->alignment();
}

void CMDDrawTextEdit::setText(const QString &value)
{
    m_data->setText(value);

    if(hasPassword())
    {
        handleHasPassword();
    }
    else
    {
        m_data->textEdit()->setText(value);
    }
}
QString CMDDrawTextEdit::text()
{
    return m_data->text();
}

void CMDDrawTextEdit::setSelectStart(int value)
{
    m_data->setSelectStart(value);
    cursor.setPosition(value, QTextCursor::MoveAnchor);
}

int CMDDrawTextEdit::selectStart()
{
    return cursor.selectionStart();
}

void CMDDrawTextEdit::setSelectLength(int value)
{
    m_data->setSelectLength(value);
    cursor.setPosition(m_data->selectStart() + value, QTextCursor::KeepAnchor);
}
int CMDDrawTextEdit::selectLength()
{
    return m_data->selectLength();
}

void CMDDrawTextEdit::setSelectText(QString& value)
{
    m_data->setSelectText(value);
}
QString CMDDrawTextEdit::selectText()
{
    return cursor.selectedText();
}

void CMDDrawTextEdit::setVarMaxLength(int value)
{
    if(value >= 0 && value <= 99999)
        m_data->setVarMaxLength(value);
}

int CMDDrawTextEdit::varMaxLength()
{
    return m_data->VarMaxLength();
}

void CMDDrawTextEdit::setDecimal(int value)
{
    if (value >= 0 && value <= 10)
        m_data->setDecimal(value);
}

int CMDDrawTextEdit::decimal()
{
    return m_data->decimal();
}

void CMDDrawTextEdit::setDateFormat(const QString &value)
{
    m_data->setDateFormat(value);
    //m_data->m_showDateFormat = language()->value(m_data->m_dateFormat, false);
}

QString CMDDrawTextEdit::dateFormat()
{
    return m_data->dateFormat();
}

void CMDDrawTextEdit::setWriteVarEnable(bool enable)
{
    m_data->setWriteVarEnable(enable);
}

bool CMDDrawTextEdit::writeVarEnable()
{
    return m_data->writeVarEnable();
}

void CMDDrawTextEdit::setEnterSetVarEnable(bool enable)
{
    m_data->setEnterSetVarEnable(enable);
}

bool CMDDrawTextEdit::enterSetVarEnable()
{
    return m_data->enterSetVarEnable();
}

void CMDDrawTextEdit::setHasPassword(bool enable)
{
    m_data->setHasPassword(enable);
    setText(m_data->text());
}

bool CMDDrawTextEdit::hasPassword()
{
    return m_data->hasPassword();
}

QString CMDDrawTextEdit::textFormatString()
{
    return QString("%1,%2,%3").arg(varMaxLength()).arg(decimal()).arg(dateFormat());
}

/*!
\section 控件对象属性
*/
DrawType CMDDrawTextEdit::type()
{
    return DrawType::TextEdit;
}

CMDPropertyDrawObj *CMDDrawTextEdit::createPropertyDrawObj()
{
    return new CMDPropertyDrawTextEdit(this);
}

void CMDDrawTextEdit::onInitialization()
{
    CMDDrawBasicWidget::onInitialization();
    if(!widget())
        return;

    if(m_data->horizontalScrollBar())
    {
        m_data->textEdit()->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
    else
    {
        m_data->textEdit()->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }

    if(m_data->verticalScrollBar())
    {
        m_data->textEdit()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
    else
    {
        m_data->textEdit()->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }

    m_data->textEdit()->setLineWrapMode((QTextEdit::LineWrapMode)m_data->lineWrapMode());
    m_data->textEdit()->setReadOnly(m_data->readOnly());
    m_data->textEdit()->setText((QString)m_data->text());
    m_data->textEdit()->selectAll();
    m_data->textEdit()->document()->setDefaultTextOption(QTextOption((Qt::Alignment)m_data->alignment()));
    m_data->textEdit()->setFrameShape((QFrame::Shape)m_data->borderStyle());

    QTextCursor cur = m_data->textEdit()->textCursor();
    cur.setPosition(0, QTextCursor::MoveAnchor);
    m_data->textEdit()->setTextCursor(cur);

    if(!hasPassword())
    {
        setText(m_data->text());
    }
    else
    {
        handleHasPassword();
    }
}

CMDDrawObj *CMDDrawTextEdit::createSelf()
{
    return new CMDDrawTextEdit();
}
/*!
\section 序列化
*/
CMDDrawObj *CMDDrawTextEdit::clone()
{
    CMDDrawTextEdit* obj = static_cast<CMDDrawTextEdit*>(CMDDrawBasicWidget::clone());
    *obj->m_data = *m_data;
    return obj;
}

void CMDDrawTextEdit::serialize(QJsonObject &json)
{
    CMDDrawBasicWidget::serialize(json);

    QJsonObject js;
    m_data->serialize(js);
    json.insert("DrawTextEdit",js);
}

void CMDDrawTextEdit::deserialize(const QJsonObject &json)
{
    CMDDrawBasicWidget::deserialize(json);

    QJsonObject js = json.value("DrawTextEdit").toObject();
    m_data->deserialize(js);
}

QList<QStringList> CMDDrawTextEdit::expNames()
{
    QList<QStringList> list;
    list << QStringList({"#appearance"});
    list << QStringList({"drawtextedit.text",       QObject::tr("文本")});
    return list;
}
/*!
\section  ===== 绑定变量=====
*/
void CMDDrawTextEdit::onDataChanged(CMDExpInfo *exp)
{
    switch (exp->index())
    {
    case 0:     //! text
        setText(exp->stringValue(
                    m_data->decimal(), m_data->dateFormat(), m_data->VarMaxLength()));
        break;
    default:
        break;
    }
}

void CMDDrawTextEdit::writeVariable()
{   
    QString textExpression = expManager()->list()->last()->lowerExpression();
    if (textExpression.count() == 0)
        return;

    parent()->framework()->setVariableValue(textExpression, widget()->toPlainText());

    setText(parent()->framework()->variableValue(textExpression).toString());
}

void CMDDrawTextEdit::handleHasPassword()
{
    QString rep = getMaskString(m_data->text().length());
    widget()->setText(rep);
}

QString CMDDrawTextEdit::getMaskString(int count)
{
    QString mask = "";
    if(count>0)
    {
        for(int i=0;i<count;i++)
            mask += "●";
    }
    return mask;
}

void CMDDrawTextEdit::handleEventWriteVarNoPassword()
{
    setText(widget()->toPlainText());
    widget()->moveCursor(QTextCursor::End);
    writeVariable();
}

//////////////////////////////////////////////////////////////////////////
CMDPropertyDrawObj *CMDDrawTextEdit::createPropertyDrawObj(const QString &className)
{
    if(className.compare("DrawTextEdit", Qt::CaseInsensitive) == 0)
        return new CMDPropertyDrawTextEdit(this);

    return CMDDrawBasicWidget::createPropertyDrawObj(className);
}

void CMDDrawTextEdit::initClassInfo(QStringList &classInfo)
{
    CMDDrawBasicWidget::initClassInfo(classInfo);
    classInfo.append("DrawTextEdit");
}
//////////////////////////////////////////////////////////////////////////

