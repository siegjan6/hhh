#include "GridBaseItem.h"
#include "RecipeGridModel.h"
#include "RecipeGridDelegate.h"
#include <QTextStream>
#include <QDateTime>

#include "ComboBoxItem.h"
#include "RadioButtonItem.h"
#include "CheckBoxItem.h"
#include "ImageItem.h"
#include "ShareTool.h"

#include "MDMultiLanguage.h"
#include "MDJsonConvert.h"

CGridBaseItem::CGridBaseItem()
{
    setExpand(true);
    setRowHidden(0);
    m_environment = NULL;
    m_firstRole = Qt::DisplayRole;
    m_finalRole = Qt::InitialSortOrderRole;

    m_decimalCount = 3;
    m_multiLanguage = false;
    m_valueRangeCheckable = false;
    m_variableBind = false;
    m_minValue = 0;
    m_maxValue = 100;
    m_dateFormat = "yyyy-MM-dd HH:mm:ss";
    m_editable = true;
    m_validInRuntime = true;
    m_showOutRange = false;
    //! --------------------角色数据初始化---------------------------------
    //! Qt::TextAlignmentRole此角色初始值为0,而qt默认给item生成的widget中的text却是左侧居中的.
    //! 这里要给赋个初值,否则第一次设置对齐方式显示不正确.
    setTextAlignment(Qt::AlignVCenter);
    CGridBaseItem::setDefaultFrame(m_frame);
    m_dataSource = dataSourceType::normal;
    QBrush brush = background();
    if( brush.style() == Qt::NoBrush )
    {
        brush.setColor(Qt::white);
        setBackground(brush);
    }
}

CGridBaseItem::~CGridBaseItem()
{
}



CGridBaseItem *CGridBaseItem::createItem(itemType type)
{
    CGridBaseItem* item = NULL;

    switch( type )
    {
        case itemType::baseItem: item = new CGridBaseItem; break;
        case itemType::radioButton: item = new CRadioButtonItem; break;
        case itemType::checkBox: item = new CCheckBoxItem; break;
        case itemType::comboBox: item = new CComboBoxItem; break;
        case itemType::image: item = new CImageItem; break;
        default: item = NULL; break;
    }
    return item;
}

QWidget *CGridBaseItem::createEditorWidget(QWidget *parent, const QStyleOptionViewItem &option, const CRecipeGridDelegate *delegate)
{
    if( CShareTool::m_isStudioMode )
        return NULL;
    else
        return createEditor(parent,option,delegate);
}

void CGridBaseItem::onInitialization(IDrawEnvrionment *environment)
{
    m_environment = environment;

    //! 运行环境下,显示的值依赖于item的类型,一般的绑定变量的item都是实时更新变量的值
    if( !CShareTool::m_isStudioMode )
    {
        setEditable(m_editable);
        if( !m_editable )
            setForeground(QBrush(Qt::darkGray));

        //! 获取数值映射表的数据
        if( !m_valueMapName.isEmpty() )
        {
            gValueMapManager->valueList(environment, m_valueMapName, m_values);
            //gValueMapManager->valueToHashIndex(m_values, m_valueIndex);
        }
    }
    else
    {
        setEditable(true);  //!开发环境总是可以编辑的
    }
}

void CGridBaseItem::setExpand(bool expand)
{
    m_expand = expand;
    m_expand ? setIcon(QIcon(":/CollapseGroup")) : setIcon(QIcon(":/ExpandGroup"));
}

void CGridBaseItem::setRowHidden(quint32 rowHiddenNum)
{
    m_rowHiddenNum = rowHiddenNum;
    if( 0 == rowHiddenNum)
        setIcon(QIcon());
}

void CGridBaseItem::enableEdit(bool edit, QWidget *editor)
{
    if( edit ){
        setEditable(true);
        setForeground(QBrush(Qt::black));
    }
    else{
        setEditable(false);
        setForeground(QBrush(Qt::darkGray));
    }

    if(editor)
        editor->setEnabled(edit);
}

void CGridBaseItem::updateValidUIState()
{
    if( !isValidInRuntime() )
    {
        QString show = m_varName + QObject::tr("(InValid)");
        setText(show);
        setForeground(QBrush(Qt::gray));
        setEditable(false);
    }
    else
    {
        setForeground(QBrush(Qt::black));
        setEditable(m_editable);
    }
}

void CGridBaseItem::setDefaultFrame(frameData &frame)
{
    frame.pen.setWidth(1);
    frame.pen.setStyle(Qt::SolidLine);
    frame.pen.setJoinStyle(Qt::MiterJoin);
    frame.type = frameType::noframe;
}

CGridBaseItem::CGridBaseItem(const CGridBaseItem &srcitem) :QStandardItem(srcitem)
{
    *this = srcitem;
    //! 成组的数据成员复位
    setExpand(true);
    setRowHidden(0);
}

void CGridBaseItem::copyMembers(const CGridBaseItem *srcitem, bool clearGroupInfo)
{
    if( this == srcitem )
        return;

    *this = *srcitem;
    //! 成组的数据成员复位
    if(clearGroupInfo)
    {
        setExpand(true);
        setRowHidden(0);
    }
}

qint32 CGridBaseItem::indexOfValue(QVariant value)
{
    if( 0 == m_values.size() || !value.isValid() )
        return -1;


    bool ok;
    double tmpd;

    if( m_valueIndex.size() )
    {
        switch (value.type())
        {
            case QVariant::DateTime:
            case QVariant::Date:
            case QVariant::Time:
                return m_valueIndex.value(value.toDateTime().toString(m_dateFormat), -1);
            case QMetaType::Int:
            case QMetaType::LongLong:
            case QMetaType::Double:
            case QMetaType::UInt:
            case QMetaType::ULongLong:
                tmpd = value.toDouble(&ok);
                if( ok )
                    return m_valueIndex.value(tmpd, -1);
                else
                    return -1;
            case QMetaType::QString:
            default:
                return m_valueIndex.value(value.toString(), -1);
        }
    }

    for(qint32 i = 0; i < m_values.size(); ++i)
    {
        QString valuestr = m_values[i].first;
        switch (value.type())
        {
            case QVariant::DateTime:
            case QVariant::Date:
            case QVariant::Time:
                if(!valuestr.isEmpty())
                    m_valueIndex.insert(valuestr, i);
                break;
            case QMetaType::Int:
            case QMetaType::LongLong:
            case QMetaType::Double:
            case QMetaType::UInt:
            case QMetaType::ULongLong:
                tmpd = valuestr.toDouble(&ok);
                if( ok )
                    m_valueIndex.insert(tmpd, i);
                else
                {
                    qDebug("convert to double failed! type=%d", value.type());
                    Q_ASSERT(ok);
                }
                break;
            case QMetaType::QString:
            default:
                if(!valuestr.isEmpty())
                    m_valueIndex.insert(valuestr, i);
                break;
        }
    }

    if( m_valueIndex.size() )
        return indexOfValue(value);
    else
        return -1;
}

QString CGridBaseItem::stringValue(QVariant value)
{
    switch (value.type())
    {
    case QVariant::DateTime:
    case QVariant::Date:
    case QVariant::Time:
        return value.toDateTime().toString(m_dateFormat);
    case QMetaType::Int:
    case QMetaType::LongLong:
    case QMetaType::Double:
    case QMetaType::UInt:
    case QMetaType::ULongLong:
        return QString::number(value.toDouble(), 'f', m_decimalCount);
    case QMetaType::QString:
    default:
        return value.toString();
    }

    return value.toString();
}

CGridBaseItem *CGridBaseItem::clone() const
{
    return new CGridBaseItem(*this);
}

CGridBaseItem *CGridBaseItem::myClone(bool clearGroupInfo)
{
    bool ex = expand();
    qint32 rows = rowHidden();

    CGridBaseItem *gridItem = clone();

    if(!clearGroupInfo)
    {
        gridItem->setExpand(ex);
        gridItem->setRowHidden(rows);
    }

    return gridItem;
}

bool CGridBaseItem::isValueOutOfRange(QString inputString)
{
    if( inputString.isEmpty() || dataSourceType::normal == dataSource() || !m_valueRangeCheckable)
        return false;

    bool ok = false;
    qreal number = inputString.toDouble(&ok);
    if( !ok )
        return false;

    bool getMinSuccess = false, getMaxSuccess = false;
    qreal minValue;
    qreal maxValue;
    if(m_variableBind)
    {
        CRecipeGridModel* model = static_cast<CRecipeGridModel*>(this->model());
        bool ok;
        qreal tmpReal;
        if( !m_minVariable.isEmpty() )
        {
            QVariant minVar = model->variableValue(m_minVariable);
            if( minVar.isValid() )
            {
                tmpReal = minVar.toReal(&ok);
                if(ok)
                {
                    minValue = tmpReal;
                    getMinSuccess = true;
                }
            }
        }
        if( !m_maxVariable.isEmpty() )
        {
            QVariant maxVar = model->variableValue(m_maxVariable);
            if( maxVar.isValid() )
            {
                tmpReal = maxVar.toReal(&ok);
                if(ok)
                {
                    maxValue = tmpReal;
                    getMaxSuccess = true;
                }
            }
        }
    }
    else
    {
        getMinSuccess = true;
        getMaxSuccess = true;
        minValue = m_minValue;
        maxValue = m_maxValue;
    }

    if( getMinSuccess && number < minValue)
        return true;
    if( getMaxSuccess && number > maxValue )
        return true;

    return false;
}

bool CGridBaseItem::updateShowString(QVariant value, QWidget *editor)
{

    Q_UNUSED(editor);
    setText(stringValue(value));

    return false;
}

bool CGridBaseItem::compareItem(const CGridBaseItem &item)
{
    //! Qt默认定义的角色
    for( qint32 roleIndex = m_firstRole; roleIndex <= m_finalRole; ++roleIndex )
    {
        QVariant var1 = item.data(roleIndex);
        QVariant var2 = data(roleIndex);
        if( !var2.isValid() && var2.isNull() )
            continue;
        if( var2 != var1 )
        {
            QVariant::Type type = var2.type();
            if( QVariant::Type::String == type && var2.toString() == var1.toString() )
                continue;

            return true;
        }
    }

    //if( item.dataSource() != dataSource() )
       // return true;

    if( frame().type != frameType::noframe )
        return true;

    if( item.rowHidden() != rowHidden() )
        return true;
    //! 成员变量
    if( m_editable != item.m_editable )
        return true;

    return false;
}

void CGridBaseItem::addAssociateItem(CRecipeGridModel *model)
{
    if(CShareTool::m_isStudioMode)
        return;
    CRecipeGridModel::IndexPair pair(row(), column());
    model->addAssociateItem(variableName(), pair);
    if( valueRangeCheckable() && variableBind())
    {
        model->addAssociateItem(minVarBinded(), pair);
        model->addAssociateItem(maxVarBinded(), pair);
    }
}

void CGridBaseItem::removeAssociateItem(CRecipeGridModel *model)
{
    //qDebug("will delete itemobj!!!");
    if(CShareTool::m_isStudioMode)
        return;

   //qDebug("will remove connection!!!");
    CRecipeGridModel::IndexPair pair(row(), column());
    model->removeAssociateItem(variableName(), pair);
    if( valueRangeCheckable() && variableBind())
    {
        model->removeAssociateItem(minVarBinded(), pair);
        model->removeAssociateItem(maxVarBinded(), pair);
    }
}

void CGridBaseItem::serialize(QJsonObject &json, CRecipeGridModel* mainModel)
{
    const CGridBaseItem* prototype = static_cast<const CGridBaseItem*>(mainModel->itemPrototype());
    Q_ASSERT(prototype);
    //! 成员变量
    if( prototype->m_dataSource != m_dataSource )
        json.insert("DataSource", (qint32)m_dataSource);
    if( prototype->m_recipeName != m_recipeName )
        json.insert("RecipeName", m_recipeName);
    if( prototype->m_frame.type != m_frame.type )
        json.insert("FrameType", (qint32)m_frame.type);
    if( m_frame.type != frameType::noframe && prototype->m_frame.pen != m_frame.pen )
        json.insert("FramePen", CMDJsonConvert::toJson(m_frame.pen));
    if( prototype->m_decimalCount != m_decimalCount )
        json.insert("DecimalCount", m_decimalCount);
    if( prototype->m_dateFormat != m_dateFormat )
        json.insert("DateFormat", m_dateFormat);
    if( prototype->m_editable != m_editable )
        json.insert("Editable", m_editable);
    if( prototype->m_multiLanguage != m_multiLanguage )
        json.insert("MultiLanguage", m_multiLanguage);
    if( prototype->m_valueMapName != m_valueMapName )
        json.insert("ValueMapName", m_valueMapName);
    if( text().isEmpty() )
        m_originText = "";
    if( prototype->m_originText != m_originText )
        json.insert("OriginText", m_originText);
    //! 检查数值范围相关
    if( prototype->m_valueRangeCheckable != m_valueRangeCheckable )
        json.insert("VRCheckable", m_valueRangeCheckable);
    if( prototype->m_variableBind != m_variableBind )
        json.insert("VariableBind", m_variableBind);
    if( prototype->m_showOutRange != m_showOutRange )
        json.insert("ShowOutRange", m_showOutRange);
    if( prototype->m_minValue != m_minValue )
        json.insert("MinValue", m_minValue);
    if( prototype->m_maxValue != m_maxValue )
        json.insert("MaxValue", m_maxValue);
    if( prototype->m_minVariable != m_minVariable )
        json.insert("MinVariable", m_minVariable);
    if( prototype->m_maxVariable != m_maxVariable )
        json.insert("MaxVariable", m_maxVariable);
    if( prototype->m_strOutRange != m_strOutRange )
        json.insert("StrOutRange", m_strOutRange);
    //! 成组功能
    if( prototype->m_rowHiddenNum != m_rowHiddenNum )
        json.insert("RowHiddenNum", (qint32)m_rowHiddenNum);
    if( prototype->m_expand != m_expand )
        json.insert("Expand", m_expand);
    //!----------------保存各种角色数据---------------------------

    if( prototype->text() != text() )
        json.insert("DisplayRole", text());

    QFont thefont = font();
    if( prototype->font() != thefont )
        json.insert("FontRole", CMDJsonConvert::toJson(thefont));

    QBrush thebrush = background();
    if( prototype->background() != thebrush )
        json.insert("BackgroundRole",  CMDJsonConvert::toJson(thebrush));

    if( prototype->textAlignment() != textAlignment() )
    json.insert("TextAlignmentRole", (qint32)textAlignment());
}

void CGridBaseItem::deserialize(const QJsonObject &json)
{
    QJsonValue jvalue;
    //! 成员变量
    jvalue = json.value("DataSource");
    if( jvalue.type() != QJsonValue::Undefined )
        m_dataSource = (dataSourceType)jvalue.toInt();
    jvalue = json.value("RecipeName");
    if( jvalue.type() != QJsonValue::Undefined )
        m_recipeName = jvalue.toString();
    jvalue = json.value("FrameType");
    if( jvalue.type() != QJsonValue::Undefined )
        m_frame.type = (frameType)jvalue.toInt();
    jvalue = json.value("FramePen");
    if( jvalue.type() != QJsonValue::Undefined )
        m_frame.pen = CMDJsonConvert::toQPen(jvalue);
    jvalue = json.value("DecimalCount");
    if( jvalue.type() != QJsonValue::Undefined )
        m_decimalCount = jvalue.toInt();
    jvalue = json.value("DateFormat");
    if( jvalue.type() != QJsonValue::Undefined )
        m_dateFormat = jvalue.toString();
    jvalue = json.value("Editable");
    if( jvalue.type() != QJsonValue::Undefined )
        m_editable = jvalue.toBool();
    jvalue = json.value("MultiLanguage");
    if( jvalue.type() != QJsonValue::Undefined )
        m_multiLanguage = jvalue.toBool();
    jvalue = json.value("ValueMapName");
    if( jvalue.type() != QJsonValue::Undefined )
        m_valueMapName = jvalue.toString();
    jvalue = json.value("OriginText");
    if( jvalue.type() != QJsonValue::Undefined )
        m_originText = jvalue.toString();
    //! 检查数值范围相关
    jvalue = json.value("VRCheckable");
    if( jvalue.type() != QJsonValue::Undefined )
        m_valueRangeCheckable = jvalue.toBool();
    jvalue = json.value("VariableBind");
    if( jvalue.type() != QJsonValue::Undefined )
        m_variableBind = jvalue.toBool();
    jvalue = json.value("ShowOutRange");
    if( jvalue.type() != QJsonValue::Undefined )
        m_showOutRange = jvalue.toBool();
    jvalue = json.value("MinValue");
    if( jvalue.type() != QJsonValue::Undefined )
        m_minValue = jvalue.toDouble();
    jvalue = json.value("MaxValue");
    if( jvalue.type() != QJsonValue::Undefined )
        m_maxValue = jvalue.toDouble();
    jvalue = json.value("MinVariable");
    if( jvalue.type() != QJsonValue::Undefined )
        m_minVariable = jvalue.toString();
    jvalue = json.value("MaxVariable");
    if( jvalue.type() != QJsonValue::Undefined )
        m_maxVariable = jvalue.toString();
    jvalue = json.value("StrOutRange");
    if( jvalue.type() != QJsonValue::Undefined )
        m_strOutRange = jvalue.toString();
    //! 成组功能
    jvalue = json.value("RowHiddenNum");
    if( jvalue.type() != QJsonValue::Undefined )
        m_rowHiddenNum = jvalue.toInt();
    jvalue = json.value("Expand");
    if( jvalue.type() != QJsonValue::Undefined )
        m_expand = jvalue.toBool();
    if( m_rowHiddenNum == 0 )
        m_expand = true;
    //!----------------保存各种角色数据---------------------------
    jvalue = json.value("DisplayRole");
    if( jvalue.type() != QJsonValue::Undefined )
    {
        QString text = jvalue.toString();
        setData(text, Qt::DisplayRole);
        setData(text, Qt::EditRole);
        m_varName = text;
    }
    jvalue = json.value("FontRole");
    if( jvalue.type() != QJsonValue::Undefined )
        setFont(CMDJsonConvert::toQFont(jvalue));
    jvalue = json.value("BackgroundRole");
    if( jvalue.type() != QJsonValue::Undefined )
        setBackground(CMDJsonConvert::toQBrush(jvalue));
    jvalue = json.value("TextAlignmentRole");
    if( jvalue.type() != QJsonValue::Undefined )
        setTextAlignment((Qt::Alignment)jvalue.toInt());
}

void CGridBaseItem::changeLanguage()
{
    if( !canTranslation() )
        return;

    QString show = CMDMultiLanguage::instance()->value(m_originText,false);
    if(!show.isEmpty())
    {
        setData(show, Qt::DisplayRole);
        setData(show, Qt::EditRole);
    }

}

void CGridBaseItem::collectWords()
{
    if( !canTranslation() )
        return;

    m_originText = text();
    CMDMultiLanguage::instance()->addWord(m_originText,false);
}

bool CGridBaseItem::canTranslation()
{
    if( m_multiLanguage && dataSourceType::normal == dataSource() && !text().isEmpty() )
    {
        //! 理论上item可以不归属于任何的model,所以这里还是要做一下判断
        CRecipeGridModel* mainModel = qobject_cast<CRecipeGridModel*>(model());
        if( mainModel && mainModel->multiLanguage() )
            return true;
        else
            return false;
    }

    return false;
}

