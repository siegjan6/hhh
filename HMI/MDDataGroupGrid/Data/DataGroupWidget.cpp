#include <QSqlField>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QTextStream>
#include <QDomDocument>
#include <QJsonDocument>
#include <QThread>
#include <QJsonArray>
#include "DataGroupWidget.h"
#include "ui_DataGroupWidget.h"
#include "IMDDataGroupManager.h"
#include "IMDDataItem.h"
#include "IMDDataSourceManager.h"
#include "IMDDataSource.h"
#include "IMDDataTable.h"
#include "IMDDataField.h"
#include "IMDDataArchives.h"
#include "MDDBQueryServiceFactory.h"
#include "PropertyDialog.h"
#include "CommonForm.h"
#include "AppearanceForm.h"
#include "MDMultiLanguage.h"
#include "MDDataQueryUI.h"
#include "IMDDatabaseParam.h"
#include "MDDataSourceFactory.h"
#include "MDProjectExpert/IMDProjectExpert.h"
#include <MDDBQueryEngine/MDDBQueryEngineFactory.h>
#include <MDSQLDataBaseProxy/MDDatabaseRecordset.h>
#include <QDebug>
#include <assert.h>
#include "MDDBQueryConfigure.h"
#include "MDDBQueryUI/MDDBQueryUI.h"

CDataGroupWidget::CDataGroupWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CDataGroupWidget)
{
    ui->setupUi(this);
    m_environment = NULL;
    m_isDataArchives = false;
    m_pDataGroup = NULL;
    m_isStudioMode = false;
    m_popMenu = NULL;
    m_actAcknowledge = NULL;
    m_actAllAcknowledge = NULL;
    m_actConfirm = NULL;
    m_actAllConfirm = NULL;
    m_actSuppress = NULL;
    m_actShelve = NULL;
    m_actAddComment = NULL;
    m_pDBQueryEngine = NULL;
    m_DataArchivers_Menu = NULL;
    m_query_Action = NULL;
    m_Paramenter_Action = NULL;
    m_selectrow = 0;
    m_propertyData = new CDataGroupBasicData;
    m_model = new CDataGroupModel();
    m_pQueryResult = new CMDDataArchivesDBResultNotify();
    m_version = 2;
    m_viewName = "";
    m_DBQueryPath = "";
}

CDataGroupWidget::~CDataGroupWidget()
{
    delete ui;
    if(m_propertyData)
    {
        delete m_propertyData;
        m_propertyData = NULL;
    }
    if(m_model)
    {
        delete m_model;
        m_model = NULL;
    }
    if(m_pQueryResult)
    {
        delete m_pQueryResult;
        m_pQueryResult =NULL;
    }
}

void CDataGroupWidget::onInitialization()
{
    loadFields();
    initModelAndView();
    changeProperty();
    CMDMultiLanguage::instance()->translateWidget(this);
}

void CDataGroupWidget::loadFields()
{
    if(m_propertyData->m_gridType == GridType::Alarm || m_propertyData->m_gridType == GridType::Log)
    {
        loadAlarmLogFields();
        m_model->setDatas(&m_recordset);
    }
    else if(m_propertyData->m_gridType == GridType::DataArchives)
    {
        connect(m_pQueryResult,SIGNAL(receiveDatas(CMDDatabaseRecordset*)),this, SLOT(receiveDatas(CMDDatabaseRecordset*)));
    }
}

void CDataGroupWidget::loadAlarmLogFields()
{
    if (NULL != m_pDataGroup && m_fields.count() == 0)
    {
        IMDDataItem *pItem = NULL;
        for(int i = 0; i < m_pDataGroup->itemCount(); i++)
        {
            pItem = m_pDataGroup->getItem(i);
            if(NULL == pItem)
            {
                continue;
            }
            CDataGroupField field;
            field.setName(pItem->name());
            field.setComment(pItem->description());
            field.setType(pItem->dataType());
            field.setWidth(100);
            if("StartTime" == pItem->name() || "Message" == pItem->name() ||"RecordTime" == pItem->name())
            {
                field.setCheckState(true);
            }
            else
            {
                field.setCheckState(false);
            }
            if(field.type() == MDDataType::DateTime)
            {
                field.setDataFormat("yyyy-MM-dd hh:mm:ss");
                field.setWidth(200);
            }
            else if(field.type() == MDDataType::Double)
            {
                field.setDataFormat("%2f"); //! 数据小数点后位数
            }
            else if (field.type() == MDDataType::String || field.type() == MDDataType::Int32)
            {
                field.setDataFormat("");
            }
            m_fields.append(field);
        }
    }
}

void CDataGroupWidget::receiveDatas(CMDDatabaseRecordset *pResult)
{
    m_model->setDatas(pResult);
}

void CDataGroupWidget::initModelAndView()
{
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    resetSelectFields();
    m_model->setFields(&m_selectedfields);
    if(m_environment)
    {
        m_model->setStudioMode(m_environment->isStudioMode());
    }
    ui->tableView->setSortingEnabled(true);
    QSortFilterProxyModel *sort_filter = new QSortFilterProxyModel(this);
    sort_filter->setSourceModel(m_model);
    ui->tableView->setModel(sort_filter);
    ui->tableView->scrollToBottom();
}

void CDataGroupWidget::changeProperty()
{
    resetSelectFields();
    m_model->setPropertyData(m_propertyData);

    ui->tableView->horizontalHeader()->setFixedHeight(m_propertyData->m_rowWidth);
    ui->tableView->verticalHeader()->setFixedWidth(m_propertyData->m_colHeight);
    ui->tableView->horizontalHeader()->setStyleSheet(setHeaderBackgroundColor(m_propertyData->m_rowColor));
    ui->tableView->verticalHeader()->setStyleSheet(setHeaderBackgroundColor(m_propertyData->m_colColor));
    ui->tableView->setStyleSheet(setScrollBarAndCorner(m_propertyData->m_horizontalBarHeight,m_propertyData->m_verticalBarWidth,m_propertyData->m_cornerColor));

    refreshModel();

    for(int i = 0; i < m_selectedfields.count(); i++)
    {
        ui->tableView->setColumnWidth(i,m_selectedfields.at(i).width());
    }

    if(m_propertyData->m_gridType == GridType::Alarm)
    {
        m_propertyData->m_allFieldsName.clear();

        for(int i = 0; i < m_fields.count(); i++)
        {
            m_propertyData->m_allFieldsName.append(m_fields.at(i).name());
        }
    }
}

bool CDataGroupWidget::openConfigDialog()
{
    CPropertyDialog dlg(dynamic_cast<QWidget*>(QObject::parent()));
    dlg.m_dataGroupWidget = this;
    dlg.setFields(&m_fields);
    dlg.setPropertyData(m_propertyData);
    dlg.setTitleName(m_propertyData->m_gridName);
    dlg.m_commonForm->m_isDataArchives = m_isDataArchives;

    if(m_propertyData->m_gridType == GridType::DataArchives)
    {
        dlg.m_commonForm->setDBQueryPath(m_DBQueryPath);
        dlg.m_commonForm->initViewList(m_viewName);
    }
    dlg.initialize();
    if(dlg.exec() == QDialog::Accepted)
    {
        m_viewName = dlg.getViewName();
        m_fields = *dlg.fields();
        *m_propertyData = *dlg.propertyData();
        changeProperty();
        return true;
    }
    return false;
}

void CDataGroupWidget::setGridType(GridType type)
{
    m_propertyData->m_gridType = type;
    if(type == GridType::DataArchives)
    {
        m_isDataArchives = true;
        createCustomConnection();
    }
    else
    {
        m_isDataArchives = false;
        if(type == GridType::Alarm)
        {
            createAlarmActionConnection();
        }
        if(type == GridType::Log)
        {
            createLogActionConnection();
        }
    }
}

void CDataGroupWidget::setGridName(const QString &name)
{
    m_propertyData->m_gridName = name;
}

void CDataGroupWidget::setDrawEnvrionment(IDrawEnvrionment *value)
{
    if(value)
    {
        m_environment = value;
        if(!m_environment->isStudioMode())
        {
            if(m_environment->projectExpert())
            {
                m_pDBQueryEngine = m_environment->projectExpert()->queryEngine();
            }
        }

        m_DBQueryPath = m_environment->projectPath()+"/DBQuery";
        m_isStudioMode = value->isStudioMode();

        if(value->dataArchives())
        {
            if(value->dataArchives()->dataGroupManager())
            {
                if(m_propertyData->m_gridType == GridType::Alarm)
                {
                    m_pDataGroup = value->dataArchives()->dataGroupManager()->getAlarmDataGroup();
                }
                else if(m_propertyData->m_gridType == GridType::Log)
                {
                    m_pDataGroup = value->dataArchives()->dataGroupManager()->getLogDataGroup();
                }
            }
        }
    }
}

void CDataGroupWidget::setNotify(bool notify)
{
    if(notify)
    {
        dataChangedEvent();
    }
}

void CDataGroupWidget::dataChangedEvent()
{
    if(!m_isStudioMode)
    {
        if(m_environment->projectExpert())
        {
            m_recordset.clear();

            if(m_propertyData->m_gridType == GridType::Alarm)
            {
                m_environment->projectExpert()->alarmRecordSet(&m_recordset);
            }
            else if(m_propertyData->m_gridType == GridType::Log)
            {
                m_environment->projectExpert()->logRecordSet(&m_recordset);
            }
        }
        refreshModel();
    }
}

void CDataGroupWidget::query()
{
    if(!m_pDBQueryEngine)
        return;

    QString str = "";
    bool b = m_pDBQueryEngine->queryView(m_viewName, m_pQueryResult,str);
    assert(b);
    if(b)
    {
        qDebug()<<"从数据库查询字段成功！";
    }
    refreshModel();
}

void CDataGroupWidget::resetSelectFields()
{
    m_selectedfields.clear();
    for(int i = 0; i < m_fields.count(); i++)
    {
        if(m_fields.at(i).checkState())
        {
            m_selectedfields.append(m_fields.at(i));
        }
    }
}

void CDataGroupWidget::refreshModel()
{
    m_model->resetModel();
    ui->tableView->scrollToBottom();
}

void CDataGroupWidget::setQueryEngine(IMDDBQueryEngine *pEngine)
{
    m_pDBQueryEngine = pEngine;
}

void CDataGroupWidget::createAlarmActionConnection()
{
    m_popMenu = new QMenu(ui->tableView);
    m_actAcknowledge = new QAction(tr("知悉"), m_popMenu);
    m_actAllAcknowledge = new QAction(tr("知悉所有"), m_popMenu);
    m_actConfirm = new QAction(tr("确认"), m_popMenu);
    m_actAllConfirm = new QAction(tr("确认所有"), m_popMenu);
    m_actSuppress = new QAction(tr("抑制"), m_popMenu);
    m_actShelve = new QAction(tr("搁置"), m_popMenu);
    m_actAddComment = new QAction(tr("备注"), m_popMenu);
    m_popMenu->addAction(m_actAcknowledge);
    m_popMenu->addAction(m_actAllAcknowledge);
    m_popMenu->addAction(m_actConfirm);
    m_popMenu->addAction(m_actAllConfirm);
    m_popMenu->addAction(m_actAddComment);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect (ui->tableView,&QTableView::customContextMenuRequested,this,&CDataGroupWidget::slotMoveMenu);
    connect (m_actAcknowledge,&QAction::triggered,this,&CDataGroupWidget::slotActionAcknowledge);
    connect (m_actAllAcknowledge,&QAction::triggered,this,&CDataGroupWidget::slotActionAllAcknowledge);
    connect (m_actConfirm,&QAction::triggered,this,&CDataGroupWidget::slotActionConfirm);
    connect (m_actAllConfirm,&QAction::triggered,this,&CDataGroupWidget::slotActionAllConfirm);
    connect (m_actSuppress,&QAction::triggered,this,&CDataGroupWidget::slotActionSuppress);
    connect (m_actShelve,&QAction::triggered,this,&CDataGroupWidget::slotActionShelve);
    connect (m_actAddComment,&QAction::triggered,this,&CDataGroupWidget::slotActionAddAlarmComment);
}

QString CDataGroupWidget::recordIDToName(const QString& rid)
{
    int index = rid.indexOf("_");
    if(-1 == index)
        return rid;

    return rid.left(index);
}

void CDataGroupWidget::slotMoveMenu(QPoint pos)
{
    QModelIndex index = ui->tableView->indexAt(pos);
    if(!index.isValid())
    {
        return;
    }
    m_selectrow = index.row();
    if(!m_environment->isStudioMode())
    {
        if(m_environment->projectExpert())
        {
            QString aName = recordIDToName(m_recordset.value(m_selectrow,"ID").toString());
            bool ar = m_environment->projectExpert()->acknowledgeRequired(aName);
            m_actAcknowledge->setVisible(ar);
            m_actAllAcknowledge->setVisible(ar);

            ar = m_environment->projectExpert()->confirmRequired(aName);
            m_actConfirm->setVisible(ar);
            m_actAllConfirm->setVisible(ar);
        }
    }
    m_popMenu->exec(QCursor::pos());
}

void CDataGroupWidget::slotActionAcknowledge()
{
    if(!m_environment->isStudioMode())
    {
        if(m_environment->projectExpert())
        {
            m_environment->projectExpert()->acknowledgeAlarm(m_recordset.value(m_selectrow,"ID").toString(), "", 1);
        }
    }
}

void CDataGroupWidget::slotActionAllAcknowledge()
{
    if(!m_environment->isStudioMode())
    {
        if(m_environment->projectExpert())
        {
            m_environment->projectExpert()->acknowledgeAllAlarm();
        }
    }
}

void CDataGroupWidget::slotActionConfirm()
{
    if(!m_environment->isStudioMode())
    {
        if(m_environment->projectExpert())
        {
            m_environment->projectExpert()->confirmAlarm(m_recordset.value(m_selectrow,"ID").toString(), "", 1);
        }
    }
}

void CDataGroupWidget::slotActionAllConfirm()
{
    if(!m_environment->isStudioMode())
    {
        if(m_environment->projectExpert())
        {
            m_environment->projectExpert()->confirmAllAlarm();
        }
    }
}

void CDataGroupWidget::slotActionSuppress()
{
    if(!m_environment->isStudioMode())
    {
        if(m_environment->projectExpert())
        {
            m_environment->projectExpert()->suppressAlarm(m_recordset.value(m_selectrow,"ID").toString());
        }
    }
}
void CDataGroupWidget::slotActionShelve()
{
    if(!m_environment->isStudioMode())
    {
        if(m_environment->projectExpert())
        {
            m_environment->projectExpert()->shelveAlarm(m_recordset.value(m_selectrow,"ID").toString());
        }
    }
}
void CDataGroupWidget::slotActionAddAlarmComment()
{
    if(!m_environment->isStudioMode())
    {
        if(m_environment->projectExpert())
        {
            QString comment;
            //TODO: 通过弹出注释框获取用户对报警的注释
            m_environment->projectExpert()->addAlarmComment(m_recordset.value(m_selectrow,"ID").toString(), comment);
        }
    }
}

void CDataGroupWidget::createLogActionConnection()
{
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView,&QTableView::customContextMenuRequested,this,&CDataGroupWidget::slotLogMenu);
}

void CDataGroupWidget::slotLogMenu(QPoint p)
{
    enum {DataSource = 0,Filter,Export,Rejected};

    QPoint point = mapToGlobal(p);
    if(!m_environment->isStudioMode())
    {
        MDDataQueryUI dialog(this);
        dialog.createActionFlag((int)Export);

        if(dialog.exec(point) == (int)Export)
        {
            //! 文件导出
            enum{csv=0,xml,json};
            int type = dialog.exportType();
            switch(type)
            {
            case (int)csv:
                exportToCsv();
                break;
            case (int)xml:
                exportToXml();
                break;
            case (int)json:
                exportToJson();
                break;
            default:
                break;
            }
        }
    }

}

void CDataGroupWidget::slotActionCsv()
{
    exportToCsv();
}

void CDataGroupWidget::slotActionXml()
{
    exportToXml();
}

void CDataGroupWidget::slotActionJson()
{
    exportToJson();
}

void CDataGroupWidget::createCustomConnection()
{
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView,&QTableView::customContextMenuRequested,this,&CDataGroupWidget::slotCustomMenu);
}

void CDataGroupWidget::slotCustomMenu(const QPoint &pos)
{
    if(!m_DataArchivers_Menu)
    {
        m_DataArchivers_Menu = new QMenu(tr("新建"),ui->tableView);

        if(!m_query_Action)
        {
            m_query_Action = new QAction(tr("查询"),ui->tableView);
            connect(m_query_Action,SIGNAL(triggered()),this,SLOT(slot_query_Action()));
            m_DataArchivers_Menu->addAction(m_query_Action);
        }
        if(!m_Paramenter_Action)
        {
            m_Paramenter_Action = new QAction(tr("参数配置"),ui->tableView);
            connect(m_Paramenter_Action,SIGNAL(triggered()),this,SLOT(slot_paramenter_Action()));
            m_DataArchivers_Menu->addAction(m_Paramenter_Action);
        }
    }
    m_DataArchivers_Menu->exec(ui->tableView->mapToGlobal(pos));
}

void CDataGroupWidget::slot_query_Action()
{
    query();
}

void CDataGroupWidget::slot_paramenter_Action()
{
    if(!m_pDBQueryEngine)
        return;

    DBQUI::CallStandardRumtimeParmatersConfigUI(m_pDBQueryEngine);
}

void CDataGroupWidget::exportToCsv()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Csv file"),
                                                    QDir::currentPath(),
                                                    tr("Csv Files (*.csv)"));
    if(fileName.isEmpty())
    {
        return;
    }
    if(QFileInfo(fileName).suffix().isEmpty())  //! 若后缀为空自动添加后缀
    {
        fileName.append(".csv");
    }
    QFile csvfile(fileName);
    if(!csvfile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }
    QTextStream out(&csvfile);
    QString textData;
    QString textHeader;
    int rows = m_model->rowCount(QModelIndex());
    int columns = m_model->columnCount(QModelIndex());
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns; j++)
        {
            textData += m_model->data(m_model->index(i,j),0).toString();
            textData += ", ";           //! for .csv file format
        }
        textData += "\n";               //! (optional: for new line segmentation)
    }
    for(int a = 0; a < columns; a++)
    {
        textHeader += m_model->headerData(a,Qt::Horizontal,Qt::DisplayRole).toString();
        textHeader +=", ";
    }
    textHeader+="\n";
    out << textHeader;
    out << textData;
    QMessageBox::information(this,
                             tr("导出数据成功"),
                             tr("信息已保存在%1！").arg(fileName),
                             tr("确定"));
    csvfile.close();
}

void CDataGroupWidget::exportToXml()
{
    QString fileName = QFileDialog::getSaveFileName(
                this,
                tr("Xml file"),
                QDir::currentPath(),
                tr("Xml Files (*.xml)") );
    if(fileName.isEmpty())
    {
        return;
    }
    if(!fileName.isNull())
    {
        if (QFileInfo(fileName).suffix().isEmpty()) //! 若后缀为空自动添加后缀
        {
            fileName.append(".xml");
        }
        QFile::remove(fileName);
        QFile xmlfile(fileName);
        xmlfile.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream outPut(&xmlfile);
        int rows = m_model->rowCount(QModelIndex());
        int columns = m_model->columnCount(QModelIndex());
        //! QDomDocument
        QDomDocument xmlDocument;
        QDomNode instruction = xmlDocument.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
        xmlDocument.appendChild(instruction);
        QDomElement digioElement = xmlDocument.createElement("DataArchives");
        for(int i = 0; i < rows; i++)
        {
            QDomElement ioElement = xmlDocument.createElement("line"+QString::number(i));
            digioElement.appendChild(ioElement);
            for(int j = 0; j < columns; j++)
            {
                QDomElement pinElement = xmlDocument.createElement(m_model->headerData(j,Qt::Horizontal,Qt::DisplayRole).toString());
                pinElement.appendChild( xmlDocument.createTextNode(m_model->data(m_model->index(i,j),0).toString()));
                ioElement.appendChild(pinElement);
            }
            xmlDocument.appendChild(digioElement);
        }
        xmlDocument.save(outPut,4);
        QMessageBox::information(this,
                                 tr("导出数据成功"),
                                 tr("信息已保存在%1！").arg(fileName),
                                 tr("确定"));
        xmlfile.close();
    }
}

void CDataGroupWidget::exportToJson()
{
    QString fileName = QFileDialog::getSaveFileName(
                this,
                tr("Json file"),
                tr("untitled.json"),
                tr("Text Files (*.json)"));
    if(fileName.isEmpty())
    {
        return;
    }
    if(QFileInfo(fileName).suffix().isEmpty()) //! 若后缀为空自动添加后缀
    {
        fileName.append(".json");
    }
    QVariantMap nameMap;
    QList<QVariant> nameList;
    QVariantMap rowMap;
    QVariantMap data;
    int rows = m_model->rowCount(QModelIndex());
    int columns = m_model->columnCount(QModelIndex());
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns; j++)
        {
            if(!m_selectedfields.at(j).comment().isEmpty())
            {
                data.insert(m_selectedfields.at(j).comment(),m_model->data(m_model->index(i,j),Qt::DisplayRole));
            }
            else
            {
                data.insert(m_selectedfields.at(j).name(),m_model->data(m_model->index(i,j),Qt::DisplayRole));
            }
        }
        QChar ch = '0';
        rowMap.insert(QString("Line %1").arg(i+1,4,10,ch),data);
    }
    nameList.append(rowMap);
    nameMap.insert("日志",nameList);
    QFile jsonFile(fileName);
    jsonFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream outPut(&jsonFile);
    QJsonDocument jsonDocument = QJsonDocument::fromVariant(nameMap);
    outPut << jsonDocument.toJson();
    QMessageBox::information(this,
                             tr("导出数据成功"),
                             tr("信息已保存在%1！").arg(fileName),
                             tr("确定"));
    jsonFile.close();
}

QString CDataGroupWidget::setHeaderBackgroundColor(QColor &color)
{
    QString headerBackgroundStyle = QString("QHeaderView::section {background-color:rgb(%1,%2,%3)}")
            .arg(color.red())
            .arg(color.green())
            .arg(color.blue());
    return headerBackgroundStyle;
}

QString CDataGroupWidget::setScrollBarAndCorner(int horizontal, int vertical, QColor corner)
{
    QString scrollBar = QString("QScrollBar:horizontal {height: %1px} QScrollBar:vertical {width: %2px} QTableCornerButton::section {background-color:rgb(%3,%4,%5,%6)}")
            .arg(horizontal)
            .arg(vertical)
            .arg(corner.red())
            .arg(corner.green())
            .arg(corner.blue())
            .arg(corner.alpha());
    return scrollBar;
}

CDataGroupWidget &CDataGroupWidget::operator =(const CDataGroupWidget &other)
{
    this->m_version = other.m_version;
    this->m_fields = other.m_fields;
    this->m_viewName = other.m_viewName;
    this->m_DBQueryPath = other.m_DBQueryPath;
    *this->m_propertyData = *other.m_propertyData;
    return *this;
}

void CDataGroupWidget::serialize(QDataStream &stream, bool isOrigVersion)
{
    if(!isOrigVersion)
        m_version = 2;
    if(m_version == 2)
        serialize_1(stream);
}

void CDataGroupWidget::deserialize(QDataStream &stream)
{
    stream>>m_version;

    if(m_version >= 1)
    {
        qint32 count = 0;
        stream >> count;
        for(int i = 0; i < count; i++)
        {
            CDataGroupField f;
            QString comment = "";
            stream >> comment;
            f.setComment(comment);
            QString name = "";
            stream >> name;
            f.setName(name);
            qint32 width = 0;
            stream >> width;
            f.setWidth(width);
            qint32 type;
            stream >> type;
            f.setType(MDDataType(type));
            bool visible = true;
            stream >>visible;
            f.setCheckState(visible);
            QString dataFormat = "";
            stream >> dataFormat;
            f.setDataFormat(dataFormat);
            m_fields.append(f);
        }
        m_propertyData->deserialize(stream);
    }
}

void CDataGroupWidget::serialize_1(QDataStream &stream)
{
    stream << m_version;

    qint32 count  = m_fields.count();
    stream << count;
    for(int i = 0; i < count; i++)
    {
        QString comment = m_fields.at(i).comment();
        stream << comment;
        QString name  = m_fields.at(i).name();
        stream << name;
        qint32 width = m_fields.at(i).width();
        stream << width;
        qint32 type = qint32(m_fields.at(i).type());
        stream << type;
        bool checkState = m_fields.at(i).checkState();
        stream << checkState;
        QString dataFormat = m_fields.at(i).dataFormat();
        stream << dataFormat;
    }
    m_propertyData->serialize_1(stream);
}

void CDataGroupWidget::serialize(QJsonObject &json)
{
    json.insert("viewName",m_viewName);
    QJsonArray array;
    int count = fields()->count();
    for(int i = 0; i < count; i++)
    {
        QJsonObject itemJson;
        CDataGroupField field = fields()->at(i);
        field.serialize(itemJson);
        array.append(itemJson);
    }
    json.insert("FieldsArray",array);
    m_propertyData->serialize(json);
}

void CDataGroupWidget::deserialize(const QJsonObject &json)
{
    m_viewName = json.value("viewName").toString();
    QJsonArray array = json.value("FieldsArray").toArray();
    int count =array.count();
    for(int i = 0; i < count; i++)
    {
        QJsonObject itemJson = array.at(i).toObject();
        CDataGroupField field;
        field.deserialize(itemJson);
        m_fields.append(field);
    }

    m_propertyData->deserialize(json);
}
