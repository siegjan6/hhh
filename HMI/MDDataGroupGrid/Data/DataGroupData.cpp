#include "DataGroupData.h"
#include "../include/MDUtility/MDJsonConvert.h"
#include <QJsonArray>

CDataGroupBasicData::CDataGroupBasicData()
{
    m_rowFont = QFont("Normal", 10);
    m_colFont = QFont("Normal", 10);
    m_dataFont = QFont("Normal", 10);

    m_colColor= Qt::lightGray;
    m_colForeColor = Qt::black;
    m_rowColor = Qt::lightGray;
    m_rowForeColor = Qt::black;
    m_tabColor = Qt::white;
    m_dataColor = Qt::black;
    m_cornerColor = Qt::gray;

    m_rowWidth = 30;
    m_colHeight = 40;
    m_horizontalBarHeight = 25;
    m_verticalBarWidth = 25;

    m_frontColorField = "ForeColor";
    m_backColorField = "BackColor";
    m_isFieldColor = true;

    m_version = 2;
}

CDataGroupBasicData &CDataGroupBasicData::operator =(const CDataGroupBasicData &other)
{
    this->m_version = other.m_version;
    this->m_rowFont = other.m_rowFont;
    this->m_colFont = other.m_colFont;
    this->m_dataFont = other.m_dataFont;
    this->m_rowColor = other.m_rowColor;
    this->m_colColor = other.m_colColor;
    this->m_dataColor = other.m_dataColor;
    this->m_tabColor = other.m_tabColor;
    this->m_rowWidth = other.m_rowWidth;
    this->m_colHeight = other.m_colHeight;
    this->m_horizontalBarHeight = other.m_horizontalBarHeight;
    this->m_verticalBarWidth = other.m_verticalBarWidth;
    this->m_frontColorField = other.m_frontColorField;
    this->m_backColorField = other.m_backColorField;
    this->m_isFieldColor = other.m_isFieldColor;
    this->m_allFieldsName = other.m_allFieldsName;
    this->m_gridType = other.m_gridType;
    this->m_gridName = other.m_gridName;
    this->m_colForeColor = other.m_colForeColor;
    this->m_rowForeColor = other.m_rowForeColor;
    this->m_cornerColor = other.m_cornerColor;
    return *this;
}

void CDataGroupBasicData::serialize(QDataStream &stream, bool isOrigVersion)
{
    if(!isOrigVersion)
        m_version = 2;
    if(m_version == 2)
        serialize_1(stream);
}

void CDataGroupBasicData::deserialize(QDataStream &stream)
{
    stream >> m_version;
    if(m_version >= 1)
    {
        stream >> m_rowFont >> m_colFont >> m_dataFont;
        stream >> m_rowColor >> m_colColor >> m_dataColor >> m_tabColor;
        stream >> m_rowWidth >> m_colHeight >> m_horizontalBarHeight >> m_verticalBarWidth;
        stream >> m_frontColorField >> m_backColorField >> m_isFieldColor;
        stream >> m_gridName;

        qint32 nameCount = 0;
        stream >> nameCount;
        for(int i = 0; i < nameCount; i++)
        {
            QString allname = "";
            stream >> allname;
            m_allFieldsName.append(allname);
        }
    }
    if(m_version >= 2){
        stream >> m_colForeColor >> m_rowForeColor >> m_cornerColor;
    }
}

void CDataGroupBasicData::serialize_1(QDataStream &stream)
{
    stream << m_version;
    stream << m_rowFont << m_colFont << m_dataFont;
    stream << m_rowColor << m_colColor << m_dataColor << m_tabColor;
    stream << m_rowWidth << m_colHeight << m_horizontalBarHeight << m_verticalBarWidth;
    stream << m_frontColorField << m_backColorField << m_isFieldColor;
    stream << m_gridName;

    qint32 nameCount = m_allFieldsName.count();
    stream << nameCount;
    for(int i = 0; i < nameCount; i++)
    {
        QString allname = m_allFieldsName.at(i);
        stream << allname;
    }
    stream << m_colForeColor << m_rowForeColor << m_cornerColor;
}

void CDataGroupBasicData::serialize(QJsonObject &json)
{
    json.insert("RowFont",CMDJsonConvert::toJson(m_rowFont));
    json.insert("ColFont",CMDJsonConvert::toJson(m_colFont));
    json.insert("DataFont",CMDJsonConvert::toJson(m_dataFont));;
    json.insert("RowColor",(qreal)m_rowColor.rgba());
    json.insert("ColColor",(qreal)m_colColor.rgba());
    json.insert("DataColor",(qreal)m_dataColor.rgba());
    json.insert("TabColor",(qreal)m_tabColor.rgba());
    json.insert("ColForeColor",(qreal)m_colForeColor.rgba());
    json.insert("RowForeColor",(qreal)m_rowForeColor.rgba());
    json.insert("CornerColor",(qreal)m_cornerColor.rgba());
    json.insert("RowWidth",m_rowWidth);
    json.insert("ColHeight",m_colHeight);
    json.insert("HorizontalBarHeight",m_horizontalBarHeight);
    json.insert("VerticalBarWidth",m_verticalBarWidth);
    json.insert("FrontColorField",m_frontColorField);
    json.insert("BackColorField",m_backColorField);
    json.insert("IsFieldColor",m_isFieldColor);
    json.insert("GridName",m_gridName);
    json.insert("NameCount",m_allFieldsName.count());

    QJsonArray array;
    for(int i = 0; i < m_allFieldsName.count(); i++)
    {
        array.append(m_allFieldsName[i]);
    }
    json.insert("AllFieldsName", array);
}

void CDataGroupBasicData::deserialize(const QJsonObject &json)
{
    m_rowFont = CMDJsonConvert::toQFont(json.value("RowFont"));
    m_colFont = CMDJsonConvert::toQFont(json.value("ColFont"));
    m_dataFont = CMDJsonConvert::toQFont(json.value("DataFont"));
    m_rowColor = QColor::fromRgba(json.value("RowColor").toDouble());
    m_colColor = QColor::fromRgba(json.value("ColColor").toDouble());
    m_dataColor = QColor::fromRgba(json.value("DataColor").toDouble());
    m_tabColor = QColor::fromRgba(json.value("TabColor").toDouble());
    m_colForeColor = QColor::fromRgba(json.value("ColForeColor").toDouble());
    m_rowForeColor = QColor::fromRgba(json.value("RowForeColor").toDouble());
    m_cornerColor = QColor::fromRgba(json.value("CornerColor").toDouble());
    m_rowWidth = json.value("RowWidth").toInt();
    m_colHeight = json.value("ColHeight").toInt();
    m_horizontalBarHeight = json.value("HorizontalBarHeight").toInt();
    m_verticalBarWidth = json.value("VerticalBarWidth").toInt();
    m_frontColorField = json.value("FrontColorField").toString();
    m_backColorField = json.value("BackColorField").toString();
    m_isFieldColor = json.value("IsFieldColor").toBool();
    m_gridName = json.value("GridName").toString();

    int count = json.value("NameCount").toInt();
    m_allFieldsName.clear();
    QJsonArray array = json.value("AllFieldsName").toArray();
    for(int i = 0; i < count; i++)
    {
        m_allFieldsName.append(array.at(i).toString());
    }
}

void CDataGroupField::serialize(QJsonObject &json)
{
    json.insert("Name",m_name);
    json.insert("Comment",m_comment);
    json.insert("Type",(int)m_type);
    json.insert("Width",m_width);
    json.insert("CheckState",m_checkState);
    json.insert("DataFormat",m_dataFormat);
}

void CDataGroupField::deserialize(const QJsonObject &json)
{
    m_name = json.value("Name").toString();
    m_comment = json.value("Comment").toString();
    m_type = (MDDataType)json.value("Type").toInt();
    m_width = json.value("Width").toInt();
    m_checkState = json.value("CheckState").toBool();
    m_dataFormat = json.value("DataFormat").toString();
}
