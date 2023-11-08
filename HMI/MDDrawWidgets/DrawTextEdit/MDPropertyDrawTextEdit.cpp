#include "MDPropertyDrawTextEdit.h"
#include "MDDrawTextEdit.h"
#include "DrawTextEditData.h"
#include "MDDrawTextEditFormatDialog.h"
#include "undo/DrawTextEditCommand.h"
#include "CDrawTextEditTextDialog.h"

/*!
\section 属性函数
*/
void CMDPropertyDrawTextEdit::setHorizontalScrollBar(bool value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchHorizontalScrollBar(value);
    else
        undo()->push(new DrawTextEditCommand(
                         widget(),widget()->horizontalScrollBar(),value,(int)DrawTextEditId::HScrollBar));

    setModify(true);
}

bool CMDPropertyDrawTextEdit::horizontalScrollBar()
{
    if(widget())
    {
        bool  horizontalScrollBar = widget()->horizontalScrollBar();
        return horizontalScrollBar;
    }
    return false;
}

void CMDPropertyDrawTextEdit::setVerticalScrollBar(bool value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchVerticalScrollBar(value);
    else
        undo()->push(new DrawTextEditCommand(
                         widget(),widget()->verticalScrollBar(),value,(int)DrawTextEditId::VScrollBar));

    setModify(true);
}

bool CMDPropertyDrawTextEdit::verticalScrollBar()
{
    if(widget())
    {
        bool  verticalScrollBar = widget()->verticalScrollBar();
        return verticalScrollBar;
    }
    return false;
}

void CMDPropertyDrawTextEdit::setBorderStyle(PropertyBorderStyle value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchBorderStyle(value);
    else
    {
        QFrame::Shape factValue = QFrame::NoFrame;
        switch (value)
        {
        case PropertyBorderStyle::NoFrame:
            factValue = QFrame::NoFrame;
            break;
        case PropertyBorderStyle::StyledPanel:
            factValue = QFrame::StyledPanel;
            break;
        case PropertyBorderStyle::WinPanel:
            factValue = QFrame::WinPanel;
            break;
        default:
            break;
        }
        undo()->push(new DrawTextEditCommand(
                         widget(),(int)widget()->borderStyle(),(int)factValue,(int)DrawTextEditId::BorderStyle));
    }

    setModify(true);
}

CMDPropertyDrawTextEdit::PropertyBorderStyle CMDPropertyDrawTextEdit::borderStyle()
{
    if(widget())
    {
        switch (widget()->borderStyle())
        {
        case QFrame::NoFrame:
            return PropertyBorderStyle::NoFrame;
            break;
        case QFrame::StyledPanel:
            return PropertyBorderStyle::StyledPanel;
            break;
        case QFrame::WinPanel:
            return PropertyBorderStyle::WinPanel;
            break;
        default:
            break;
        }
    }
    return PropertyBorderStyle::NoFrame;
}

void CMDPropertyDrawTextEdit::setLineWrap(bool value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchLineWrap(value);
    else
    {
        QTextEdit::LineWrapMode factValue = QTextEdit::WidgetWidth;

        switch(value)
        {
        case false:
            factValue = QTextEdit::NoWrap;
            break;
        case true:
            factValue = QTextEdit::WidgetWidth;
            break;
        default:
            break;
        }
        undo()->push(new DrawTextEditCommand(
                         widget(),widget()->lineWrapMode(),factValue,(int)DrawTextEditId::LineWrap));
    }

    setModify(true);
}

bool CMDPropertyDrawTextEdit::lineWrap()
{
    if(widget())
        return widget()->lineWrapMode();
    return true;
}

void CMDPropertyDrawTextEdit::setReadOnly(bool value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchReadOnly(value);
    else
        undo()->push(new DrawTextEditCommand(
                         widget(),widget()->readOnly(),value,(int)DrawTextEditId::ReadOnly));

    setModify(true);
}

bool CMDPropertyDrawTextEdit::readOnly()
{
    if(widget())
        return widget()->readOnly();
    return false;
}

void CMDPropertyDrawTextEdit::setAlignment(PropertyAlignment value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchAlignment(value);
    else
    {
        Qt::Alignment factValue = Qt::AlignLeft;
        switch (value)
        {
        case PropertyAlignment::AlignLeft:
            factValue = Qt::AlignLeft;
            break;
        case PropertyAlignment::AlignRight:
            factValue = Qt::AlignRight;
            break;
        case PropertyAlignment::AlignCenter:
            factValue = Qt::AlignCenter;
            break;
        default:
            break;
        }
        undo()->push(new DrawTextEditCommand(
                         widget(),(int)widget()->alignment(),(int)factValue,(int)DrawTextEditId::Alignment));
    }

    setModify(true);
}

CMDPropertyDrawTextEdit::PropertyAlignment CMDPropertyDrawTextEdit::alignment()
{
    if(widget())
    {
        switch (widget()->alignment())
        {
        case Qt::AlignLeft:
            return PropertyAlignment::AlignLeft;
            break;
        case Qt::AlignRight:
            return PropertyAlignment::AlignRight;
            break;
        case Qt::AlignCenter:
            return PropertyAlignment::AlignCenter;
            break;
        default:
            break;
        }
    }
    return PropertyAlignment::AlignLeft;
}

void CMDPropertyDrawTextEdit::setDILtext(const QString &value)
{
    Q_UNUSED(value);

    if(!widget())
        return;

    CDrawTextEditTextDialog dialog(dynamic_cast<QWidget*>(QObject::parent()));
    dialog.setItems(widget()->text());
    if(dialog.exec() == QDialog::Accepted)
    {
        QString text = dialog.items();
        if(isBatch())
            setBatchDILText(text);
        else
            undo()->push(new DrawTextEditCommand(
                             widget(),widget()->text(),text,(int)DrawTextEditId::Text));
    }
    setModify(true);
}

QString CMDPropertyDrawTextEdit::DILtext()
{
    QString showText = "";
    if(widget())
    {
        QStringList items = widget()->text().split(breakString(),QString::SkipEmptyParts);
        for(int i = 0; i < items.count();i++)
        {
            if(items.at(i) == breakString())
            {
                items.removeAt(i);
            }
        }
        if(items.count() > 0)
        {
            showText = items.at(0);
        }
        else
        {
           showText = widget()->text();
        }
    }
    return showText;
}

void CMDPropertyDrawTextEdit::setTextFormat(const QString &value)
{
    Q_UNUSED(value);
    MDDrawTextEditFormatDialog dlg(container());
    dlg.init(widget());
    if (dlg.exec() == QDialog::Accepted)
    {
        QJsonObject* v = dlg.value();
        if(isBatch())
            setBatchTextFormat(v);
        else
        {
            widget()->setDecimal(v->value("Decimal").toInt());
            widget()->setDateFormat(v->value("DateFormat").toString());
            widget()->setVarMaxLength(v->value("MaxLength").toInt());
        }

        setModify(true);
    }
}

QString CMDPropertyDrawTextEdit::textFormat()
{
    return widget()->textFormatString();
}

void CMDPropertyDrawTextEdit::setWriteVarEnable(bool enable)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchWriteVarEnable(enable);
    else
        undo()->push(new DrawTextEditCommand(
                         widget(),widget()->writeVarEnable(),enable,(int)DrawTextEditId::WriteVarEnable));

    setModify(true);
}

bool CMDPropertyDrawTextEdit::writeVarEnable()
{
    if(widget())
        return widget()->writeVarEnable();
    return false;
}

void CMDPropertyDrawTextEdit::setEnterSetVarEnable(bool enable)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchEnterSetVarEnable(enable);
    else
        undo()->push(new DrawTextEditCommand(
                         widget(),widget()->enterSetVarEnable(),enable,(int)DrawTextEditId::EnterSetVarEnable));

    setModify(true);
}

bool CMDPropertyDrawTextEdit::enterSetVarEnable()
{
    if(widget())
        return widget()->enterSetVarEnable();
    return false;
}

void CMDPropertyDrawTextEdit::setHasPassword(bool enable)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchHasPassword(enable);
    else
        undo()->push(new DrawTextEditCommand(
                         widget(),widget()->hasPassword(),enable,(int)DrawTextEditId::HasPassword));

    setModify(true);
}

bool CMDPropertyDrawTextEdit::hasPassword()
{
    if(widget())
        return widget()->hasPassword();
    return false;
}

CMDPropertyDrawTextEdit::CMDPropertyDrawTextEdit(CMDDrawObj *parent) :
    CMDPropertyDrawBasicWidget(parent)
{
    insertTranslateMap("readOnly",tr("只读"));
    insertTranslateMap("hideSelection",tr("隐藏选定"));
    insertTranslateMap("writeVarEnable",tr("可写入变量"));
    insertTranslateMap("enterSetVarEnable",tr("回车赋值"));
    insertTranslateMap("hasPassword",tr("使用密码"));
    insertTranslateMap("lineWrap",tr("换行"));
    insertTranslateMap("borderStyle",tr("边框样式"));
    insertTranslateMap("horizontalScrollBar",tr("水平滚动条"));
    insertTranslateMap("verticalScrollBar",tr("垂直滚动条"));
    insertTranslateMap("alignment",tr("文本对齐"));
    insertTranslateMap("text",tr("文本"));
    insertTranslateMap("textFormat",tr("变量格式"));
    insertTranslateMap("AlignLeft",tr("左对齐"));
    insertTranslateMap("AlignRight",tr("右对齐"));
    insertTranslateMap("AlignCenter",tr("居中对齐"));
    insertTranslateMap("NoScrollBar",tr("无"));
    insertTranslateMap("Vertical",tr("垂直"));
    insertTranslateMap("Horizontal",tr("水平"));
    insertTranslateMap("BothScrollBar",tr("垂直水平"));
    insertTranslateMap("NoFrame",tr("无"));
    insertTranslateMap("StyledPanel",tr("平面"));
    insertTranslateMap("WinPanel",tr("立体"));
}

CMDDrawTextEdit *CMDPropertyDrawTextEdit::widget()
{
    return dynamic_cast<CMDDrawTextEdit*>(m_object);
}
/*!
\section 设置属性值
*/
void CMDPropertyDrawTextEdit::resetAttribute()
{
    CMDPropertyDrawObj::resetAttribute();
}

//////////////////////////////////////////////////////////////////////////
void CMDPropertyDrawTextEdit::setBatchHorizontalScrollBar(bool value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawTextEdit* v = dynamic_cast<CMDDrawTextEdit*>(o);
        undo()->push(new DrawTextEditCommand(
                         v,v->horizontalScrollBar(),value,(int)DrawTextEditId::HScrollBar));
    }

    undo()->endMacro();
}

void CMDPropertyDrawTextEdit::setBatchVerticalScrollBar(bool value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawTextEdit* v = dynamic_cast<CMDDrawTextEdit*>(o);

        undo()->push(new DrawTextEditCommand(
                         v,v->verticalScrollBar(),value,(int)DrawTextEditId::VScrollBar));
    }

    undo()->endMacro();
}

void CMDPropertyDrawTextEdit::setBatchBorderStyle(CMDPropertyDrawTextEdit::PropertyBorderStyle value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawTextEdit* v = dynamic_cast<CMDDrawTextEdit*>(o);

        QFrame::Shape factValue = QFrame::NoFrame;
        switch (value)
        {
        case PropertyBorderStyle::NoFrame:
            factValue = QFrame::NoFrame;
            break;
        case PropertyBorderStyle::StyledPanel:
            factValue = QFrame::StyledPanel;
            break;
        case PropertyBorderStyle::WinPanel:
            factValue = QFrame::WinPanel;
            break;
        default:
            break;
        }

        undo()->push(new DrawTextEditCommand(
                         v,(int)v->borderStyle(),(int)factValue,(int)DrawTextEditId::BorderStyle));
    }

    undo()->endMacro();
}

void CMDPropertyDrawTextEdit::setBatchLineWrap(bool value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawTextEdit* v = dynamic_cast<CMDDrawTextEdit*>(o);

        QTextEdit::LineWrapMode factValue = QTextEdit::WidgetWidth;
        switch(value)
        {
        case false:
            factValue = QTextEdit::NoWrap;
            break;
        case true:
            factValue = QTextEdit::WidgetWidth;
            break;
        default:
            break;
        }

        undo()->push(new DrawTextEditCommand(
                         v,v->lineWrapMode(),factValue,(int)DrawTextEditId::LineWrap));
    }

    undo()->endMacro();
}

void CMDPropertyDrawTextEdit::setBatchReadOnly(bool value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawTextEdit* v = dynamic_cast<CMDDrawTextEdit*>(o);
        undo()->push(new DrawTextEditCommand(
                         v,v->readOnly(),value,(int)DrawTextEditId::ReadOnly));
    }

    undo()->endMacro();
}

void CMDPropertyDrawTextEdit::setBatchAlignment(CMDPropertyDrawTextEdit::PropertyAlignment value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawTextEdit* v = dynamic_cast<CMDDrawTextEdit*>(o);

        Qt::Alignment factValue = Qt::AlignLeft;
        switch (value)
        {
        case PropertyAlignment::AlignLeft:
            factValue = Qt::AlignLeft;
            break;
        case PropertyAlignment::AlignRight:
            factValue = Qt::AlignRight;
            break;
        case PropertyAlignment::AlignCenter:
            factValue = Qt::AlignCenter;
            break;
        default:
            break;
        }
        undo()->push(new DrawTextEditCommand(
                         v,(int)v->alignment(),(int)factValue,(int)DrawTextEditId::Alignment));
    }

    undo()->endMacro();
}

void CMDPropertyDrawTextEdit::setBatchDILText(const QString &value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawTextEdit* v = dynamic_cast<CMDDrawTextEdit*>(o);
        undo()->push(new DrawTextEditCommand(
                         v,v->text(),value,(int)DrawTextEditId::Text));
    }

    undo()->endMacro();
}

void CMDPropertyDrawTextEdit::setBatchTextFormat(QJsonObject *value)
{
    int decimal = value->value("Decimal").toInt();
    QString dateFormat = value->value("DateFormat").toString();
    int maxLength = value->value("VarMaxLength").toInt();

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawTextEdit* v = dynamic_cast<CMDDrawTextEdit*>(o);
        v->setDecimal(decimal);
        v->setDateFormat(dateFormat);
        v->setVarMaxLength(maxLength);
    }
}

void CMDPropertyDrawTextEdit::setBatchWriteVarEnable(bool value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawTextEdit* v = dynamic_cast<CMDDrawTextEdit*>(o);
        undo()->push(new DrawTextEditCommand(
                         v,v->writeVarEnable(),value,(int)DrawTextEditId::WriteVarEnable));
    }

    undo()->endMacro();
}

void CMDPropertyDrawTextEdit::setBatchEnterSetVarEnable(bool value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawTextEdit* v = dynamic_cast<CMDDrawTextEdit*>(o);
        undo()->push(new DrawTextEditCommand(
                         v,v->enterSetVarEnable(),value,(int)DrawTextEditId::EnterSetVarEnable));
    }

    undo()->endMacro();
}

void CMDPropertyDrawTextEdit::setBatchHasPassword(bool value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawTextEdit* v = dynamic_cast<CMDDrawTextEdit*>(o);
        undo()->push(new DrawTextEditCommand(
                         v,v->hasPassword(),value,(int)DrawTextEditId::HasPassword));
    }

    undo()->endMacro();
}
//////////////////////////////////////////////////////////////////////////
