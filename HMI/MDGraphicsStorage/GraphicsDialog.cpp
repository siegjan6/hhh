#include <QSplitter>
#include <QLayout>
#include <QFileInfo>
#include <QDir>
#include <QJsonDocument>
#include <QTextStream>
#include "GraphicsDialog.h"
#include "ui_GraphicsDialog.h"
//////////////////////////////////////////////////////////////////////////
CGraphicsDialog::CGraphicsDialog(const QString &imagePath, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CGraphicsDialog)
{
    m_tree = NULL;//!目录控件
    m_widget = NULL;//!控件
    m_scroll = NULL;
    m_container = NULL;
    m_canEvent = false;//!事件是否生效
    m_imagePath = imagePath;


    ui->setupUi(this);
    setWindowFlags(Qt::Widget);//add maxbutton

    init();

    m_transTool = CMDMultiLanguage::createWidgetTool();
    m_transTool->init(this);
}
CGraphicsDialog::~CGraphicsDialog()
{
    delete ui;
    delete m_transTool;
}
//////////////////////////////////////////////////////////////////////////
void CGraphicsDialog::saveObjDialog(QList<CMDDrawObj *>* objs)
{
    m_selectPathDialog->reset();
    m_selectPathDialog->changeLanguage();
    if (m_selectPathDialog->exec() == QDialog::Accepted)
    {
        m_savePath = m_selectPathDialog->selectPath();
        saveObjToPath(objs);
    }
}
void CGraphicsDialog::saveObjToPath(QList<CMDDrawObj *> *objs)
{
    for(int i = 0; i < objs->count(); i++)
    {
        CMDDrawObj* obj = objs->at(i);

        //文件是否存在;存在，修改文件名
        QString name = obj->name();
        QFileInfo info(m_savePath + "/" + name + ".gra");
        if (info.exists())
        {
            int count = name.count();
            int numCount = 0;
            for (int i = count - 1; i >= 0; i--)
            {
                int c = name.at(i).digitValue();
                if (c >= 0 && c <= 9)
                    numCount++;
                else
                    break;
            }

            QString baseName = name.left(count - numCount);
            QString strNum;
            int num = 0;
            if (numCount > 0)
            {
                strNum = name.right(numCount);
                num = strNum.toInt();
            }

            while (info.exists())
            {
                num++;
                strNum.setNum(num);
                name = m_savePath + "/" + baseName + strNum + ".gra";
                info.setFile(name);
            }
        }
        else
            name = m_savePath + "/" + name + ".gra";

        QFile file(name);
        if (!file.open(QIODevice::WriteOnly))
            continue;

        //copy image
        QString storagePath = obj->parent()->framework()->storageImagePath();
        QString projectPath = obj->parent()->framework()->projectImagePath();
        obj->copyImage(projectPath, storagePath, true);

        QJsonObject json;
        serialize(json, obj);

        QJsonDocument doc;
        doc.setObject(json);

        QJsonDocument::JsonFormat format = QJsonDocument::Compact;
#ifdef QT_DEBUG
        format = QJsonDocument::Indented;
#endif
        QByteArray ba = doc.toJson(/*format*/);

        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        stream << ba;

        file.close();
    }
}
void CGraphicsDialog::showDialog(IHMIForm *container)
{
    m_container = container;

    setParent(dynamic_cast<QWidget*>(container));
    setWindowFlags(Qt::Dialog);
    reset();
    changeLanguage();
    exec();
}
//////////////////////////////////////////////////////////////////////////
bool CGraphicsDialog::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_scroll)
    {
        switch (event->type()) {
        case QEvent::Resize:{
            QResizeEvent* e = static_cast<QResizeEvent*>(event);
            m_widget->calculateSize(e->size().width(), e->size().height());
        }
            break;
        default:
            break;
        }
    }

    return QObject::eventFilter(watched, event);
}
//////////////////////////////////////////////////////////////////////////
void CGraphicsDialog::init()
{
    QString path = QCoreApplication::applicationDirPath();
    m_basePath = path + "/graphics";
    m_savePath = m_basePath;

    QDir dir(m_basePath);
    if (!dir.exists())
        dir.mkdir(m_basePath);

    QSplitter* splitter = new QSplitter(this);
    m_scroll = new QScrollArea(this);
    m_tree = new QTreeWidget(this);
    m_widget = new CObjectWidget(m_imagePath, this);
    m_selectPathDialog = new CSelectPathDialog(this);

    m_scroll->setWidget(m_widget);
    splitter->addWidget(m_tree);
    splitter->addWidget(m_scroll);
    splitter->setStretchFactor(1, 1);
    layout()->addWidget(splitter);

    connect(&m_node, &CStorageFileNode::currentPathChanged, this, &CGraphicsDialog::currentPathChanged);
    m_scroll->installEventFilter(this);
    m_scroll->setMouseTracking(true);

    m_selectPathDialog->init(m_basePath);
}
void CGraphicsDialog::reset()
{
    m_canEvent = false;
    m_node.init(m_tree, m_basePath);
    m_canEvent = true;

    m_tree->setCurrentItem(m_tree->topLevelItem(0));
}
void CGraphicsDialog::serialize(QJsonObject &json, CMDDrawObj *obj)
{
    json.insert("Type", (int)obj->type());
    obj->serialize(json);
}
void CGraphicsDialog::currentPathChanged(const QString &path)
{
    if (!m_canEvent)
        return;

    m_widget->changePath(path, m_container);
}
//////////////////////////////////////////////////////////////////////////
void CGraphicsDialog::changeLanguage()
{
    m_transTool->changeLanguage();
    m_node.changeLanguage();
}
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
