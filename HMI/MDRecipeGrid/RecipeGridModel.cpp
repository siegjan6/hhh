#include "RecipeGridModel.h"
#include "GridItems/GridBaseItem.h"
#include "ShareTool.h"
#include "RecipeGridView.h"
#include <QBuffer>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonArray>

CRecipeGridModel::CRecipeGridModel(int rows, int columns, QObject *parent)
:QStandardItemModel(rows,columns,parent)
{
    m_envrionment = NULL;
    //! 设置我们自己的item基类,给model实例化的时候使用
    m_prototype = new CGridBaseItem();
    setItemPrototype(m_prototype);

    m_mainView = static_cast<CRecipeGridView*>(parent);
}

CGridBaseItem *CRecipeGridModel::getItem(QModelIndex index, bool createIfNeeded) const
{
    if( !index.isValid() )
        return NULL;
    //! itemFromIndex()的默认实现就是有item直接返回,没有则创建一个.
    if( createIfNeeded )
        return static_cast<CGridBaseItem*>(itemFromIndex(index));
    else
        return static_cast<CGridBaseItem*>(item(index.row(), index.column()));
}

void CRecipeGridModel::setMyItem(qint32 row, qint32 col, CGridBaseItem *item)
{
    QModelIndex idx = index(row,col);
    CGridBaseItem* oldItem = getItem(idx);
    if( oldItem )
        oldItem->removeAssociateItem(this);
    setItem(row, col, item);
    item->addAssociateItem(this);
    //item->onInitialization(m_envrionment);
    showEditorByItem(idx, true);
}

void CRecipeGridModel::serialize(QJsonObject &json)
{
    QJsonArray itemArray;

    CGridBaseItem cleanItem;
    for(qint32 row = 0; row < rowCount(); ++row)
    {
        for(qint32 col = 0; col < columnCount(); ++col)
        {
            CGridBaseItem* theItem = getItem(index(row,col));
            if( theItem && theItem->compareItem(cleanItem) )
            {
                QJsonObject itemjson;
                itemjson.insert("RowIdx", row);
                itemjson.insert("ColumnIdx", col);
                itemjson.insert("ItemType", (qint32)theItem->type());
                theItem->serialize(itemjson, this);
                itemArray.append(itemjson);
            }
        }
    }
    if( itemArray.size() > 0 )
        json.insert("ItemDataArray", itemArray);
}

void CRecipeGridModel::deserialize(const QJsonObject &json)
{
    QJsonArray itemArray = json.value("ItemDataArray").toArray(QJsonArray());

    for( qint32 i = 0; i < itemArray.size(); ++i )
    {
        QJsonObject itemjson = itemArray.at(i).toObject(QJsonObject());
        if( itemjson.isEmpty() )
            continue;

        qint32 row,col;
        row = itemjson.value("RowIdx").toInt();
        col = itemjson.value("ColumnIdx").toInt();
        if( row < rowCount() && col < columnCount() )
        {
            qint32 type;
            type = itemjson.value("ItemType").toInt();
            CGridBaseItem* item = CGridBaseItem::createItem((itemType)type);
            if( item )
            {
                item->deserialize(itemjson);
                setItem(row, col, item);
            }
        }
    }
}

void CRecipeGridModel::onInitialization(IDrawEnvrionment *environment)
{
    m_envrionment = environment;

    for(qint32 row = 0; row < rowCount(); ++row)
    {
        for(qint32 col = 0; col < columnCount(); ++col)
        {
            CGridBaseItem* theItem = getItem(index(row, col));
            if( theItem  )
                theItem->onInitialization(environment);
        }
    }
}

void CRecipeGridModel::showEditorByItem(QModelIndex index, bool show)
{
    if( !index.isValid() || CShareTool::m_isStudioMode )
        return;
    CGridBaseItem* item = getItem(index);
    if(item && item->canShowPersistentEditor())
        emit showEditor(index, show);
}

void CRecipeGridModel::getExpressionList(QList<int> &indexs, QList<QString> &exps)
{
    m_varIndexs.clear();
    m_varList.clear();

    for(qint32 row = 0; row < rowCount(); ++row)
    {
        for(qint32 col = 0; col < columnCount(); ++col)
        {
            CGridBaseItem* theItem = getItem(index(row,col));
            if(!theItem)
                continue;
            if( dataSourceType::normal == theItem->dataSource() )
                continue;

            IndexPair itemidx(row, col);
            QString varName = theItem->variableName();
            updateVarInfo(varName, VarPurpose::dataSource);
            addAssociateItem(varName, itemidx);
            //! 超限检查功能的最大最小值，如果绑定了变量，也要添加进去
            if( theItem->valueRangeCheckable() && theItem->variableBind())
            {
                QString minVar = theItem->minVarBinded();
                updateVarInfo(minVar, VarPurpose::checkRange);
                addAssociateItem(minVar, itemidx);

                QString maxVar = theItem->maxVarBinded();
                updateVarInfo(maxVar, VarPurpose::checkRange);
                addAssociateItem(maxVar, itemidx);
            }
        }
    }
    //! 向上层提交变量列表
    for( qint32 idx = 0 ; idx < m_varList.size(); idx++ )
    {
        indexs.append(idx);
        exps.append(m_varList.at(idx).name);
    }

}

void CRecipeGridModel::updateVarInfo(QString varName, VarPurpose purpose)
{
    if( varName.isEmpty() )
        return;

    qint32 varConter = m_varList.size();
    qint32 varidx = m_varIndexs.value(varName, InvalidVarIndex);
    if( varidx == InvalidVarIndex )
    {
        VarInfo info;
        info.name = varName;
        info.purpose = purpose;
        m_varList.append(info);
        m_varIndexs.insert(varName, varConter);
        return;
    }

    //! 更新变量信息
    if( varidx >=0 && varidx < m_varList.size() )
    {
        VarInfo& varinfo = m_varList[varidx];
        //! 目前也就更新下变量的目的属性
        varinfo.purpose = VarPurpose(varinfo.purpose | purpose);
    }


    //if(isNewVariable)
        //qDebug("submit new var=%s %d %d", qPrintable(varName), row, col);

    return;

}

void CRecipeGridModel::addAssociateItem(QString varName, const CRecipeGridModel::IndexPair &index)
{
    if( varName.isEmpty() )
        return;

    qint32 varidx = m_varIndexs.value(varName, InvalidVarIndex);
    if( varidx < 0 || varidx >= m_varList.size() )
        return;

    m_varList[varidx].items.insert(index);

    //qDebug("add item var=%s row=%d col=%d  total=%d", qPrintable(varName), index.first, index.second, m_varList[varidx].items.size());
}

void CRecipeGridModel::removeAssociateItem(QString varName, const CRecipeGridModel::IndexPair &index)
{
    if( varName.isEmpty() )
        return;

    qint32 varidx = m_varIndexs.value(varName, InvalidVarIndex);
    if( varidx < 0 || varidx >= m_varList.size() )
        return;

    m_varList[varidx].items.remove(index);

    //qDebug("remove item var=%s row=%d col=%d  total=%d", qPrintable(varName), index.first, index.second, m_varList[varidx].items.size());
}

void CRecipeGridModel::setValue(const qint32 index, QVariant variant)
{
    if( index < 0 || index >= m_varList.size() )
        return;

    VarInfo& sinfo = m_varList[index];
    sinfo.value = variant;

    if( sinfo.purpose & VarPurpose::dataSource )
    {
        foreach( const IndexPair& itempair, sinfo.items )
        {
            QModelIndex itemIndex = this->index(itempair.first, itempair.second);
            CGridBaseItem* theItem = getItem(itemIndex);
            if( !theItem )
                continue;

            //qDebug("set Value %d %d", iteminfo.row, iteminfo.column);

            QWidget* widget = m_mainView->indexWidget(theItem->index());
            if( theItem->updateShowString(variant, widget) )
                m_mainView->update(itemIndex);
        }
    }

}

QVariant CRecipeGridModel::variableValue(const QString varname)
{
    qint32 index = m_varIndexs.value(varname, InvalidVarIndex);
    if( index >=0 && index < m_varList.size() )
        return m_varList.at(index).value;

    return QVariant();
}

void CRecipeGridModel::changeLanguage()
{
    for(qint32 row = 0; row < rowCount(); ++row)
    {
        for(qint32 col = 0; col < columnCount(); ++col)
        {
            CGridBaseItem* theitem = getItem(index(row,col));
            if( theitem )
                theitem->changeLanguage();

        }
    }
}

void CRecipeGridModel::collectWords()
{
    for(qint32 row = 0; row < rowCount(); ++row)
    {
        for(qint32 col = 0; col < columnCount(); ++col)
        {
            CGridBaseItem* theitem = getItem(index(row,col));
            if( theitem )
                theitem->collectWords();

        }
    }
}

bool CRecipeGridModel::multiLanguage() const
{
    return m_mainView->multiLanguage();
}

QColor CRecipeGridModel::defaultBackgroundColor() const
{
    QBrush brush = m_prototype->background();
    //! 默认值是不填充的brush,此时其颜色是黑色.这里直接返回白色更合理
    if( Qt::NoBrush == brush.style() )
        return Qt::white;
    return m_prototype->background().color();
}

QFont CRecipeGridModel::defaultFont() const
{
    return m_prototype->font();
}

Qt::Alignment CRecipeGridModel::defaultTextAlign() const
{
    return m_prototype->textAlignment();
}

dataSourceType CRecipeGridModel::defaultDataSource() const
{
    return m_prototype->dataSource();
}

qint32 CRecipeGridModel::defaultDecimalCount() const
{
    return m_prototype->decimalCount();
}

QString CRecipeGridModel::defaultDateFormat() const
{
    return m_prototype->dateFormat();
}

bool CRecipeGridModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    if( Qt::EditRole == role)
    {
        CGridBaseItem* theitem = getItem(index);
        QString input = value.toString();
        //! 如果之前该index并没有item,如果写入的数据还是空,则不创建tiem实例.直接退出.
        if( NULL == theitem && input.isEmpty() )
            return false;

        if( NULL == theitem )
            theitem = getItem(index, true);

        Q_ASSERT(theitem);

        //! 做超限检查
        if( theitem->isValueOutOfRange(input))
        {
            if( theitem->showWhenOutRange() )
                CShareTool::trWarningMessageBox(theitem->stringOutRange(), m_mainView);

            return false;
        }
        QVariant oldValue = theitem->data(role);
        if( oldValue == value )
            return false;
        //qDebug("setData %s", qPrintable(input));
        theitem->setData(value, role);
        if( dataSourceType::normal != theitem->dataSource() && !CShareTool::m_isStudioMode)
        {
            //! Y2hvdWps null 运行环境下，直接对变量赋值
        }

        m_mainView->setModify(true);
        return true;
    }

    return QStandardItemModel::setData(index,value,role);
}


QVariant CRecipeGridModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    //! 列表头位数字递增,使用默认行为
    if( Qt::Vertical == orientation )
        return QStandardItemModel::headerData(section, orientation, role);
    //! 表格的表头暂时只能显示,其他的按默认处理
    if (role != Qt::DisplayRole)
        return QStandardItemModel::headerData(section, orientation, role);

    //! 设置横表头为字母
    qint32 shang;
    QString str = "";
    while( (shang = section / 26) > 0 )
    {
        str.insert(0, 'A' + section%26);
        section = shang;
    };

    str.insert(0, 'A' + section%26);

    return str;
}




