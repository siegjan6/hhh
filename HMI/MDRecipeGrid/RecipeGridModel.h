/*!
author Y2hvdWps 2016.08.29
section 配方表格模型类
*/
#ifndef MDRECIPEGRIDMODEL_H
#define MDRECIPEGRIDMODEL_H

#include <QSet>
#include <QStandardItemModel>
#include "IDrawEnvironment.h"
#include "GridItems/GridBaseItem.h"

#define InvalidVarIndex -1

class CRecipeGridView;

class CRecipeGridModel : public QStandardItemModel
{

    friend class CGridBaseItem;

    Q_OBJECT

public:
    explicit CRecipeGridModel(int rows, int columns, QObject * parent = 0);
    virtual ~CRecipeGridModel(){}

    CGridBaseItem* getItem(QModelIndex index, bool createIfNeeded = false) const;
    void setMyItem(qint32 row, qint32 col, CGridBaseItem* item);
    /*!
    \section  序列化,json格式接口
    */
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

    void onInitialization(IDrawEnvrionment *environment);    //! 反序列化后的一些更新操作
    void showEditorByItem(QModelIndex index, bool show);
    /*!
    \section  变量相关
    */
    void getExpressionList(QList<int>& indexs, QList<QString>& exps);
    void setValue(const qint32 index, QVariant variant);
    QVariant variableValue(const QString varname);

    /*!
    \section  多语言
    */
    void changeLanguage();
    void collectWords();
    bool multiLanguage() const;
    /*!
    section 获取item的默认角色数据
    */
    QColor defaultBackgroundColor() const;
    QFont defaultFont() const;
    Qt::Alignment defaultTextAlign() const;
    dataSourceType defaultDataSource() const;
    qint32 defaultDecimalCount() const;
    QString defaultDateFormat() const;
public:
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    //! 设置表格的第一列/第一行
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

public:
    IDrawEnvrionment* m_envrionment;
    CRecipeGridView* m_mainView;
private:
    CGridBaseItem* m_prototype;

    /*!
    \section  变量容器
    */
    enum VarPurpose{
        dataSource = 1 << 0,
        checkRange = 1 << 1
    };

    typedef QPair<qint32,qint32> IndexPair;
    typedef QSet<IndexPair> ItemSet;
    typedef struct varinfo{
        QString name;
        VarPurpose purpose;
        QVariant value;
        ItemSet items;
    }VarInfo;
    typedef QList<varinfo> VarValues;
    typedef QMap<QString, qint32> VarIndexs;
    VarValues m_varList; //! 变量数据源列表
    VarIndexs m_varIndexs;  //! 变量索引表

    void updateVarInfo(QString varName, VarPurpose purpose);    //! 添加/更新变量相关信息
    void addAssociateItem(QString varName, const IndexPair& index);    //! 添加跟变量关联的item
    void removeAssociateItem(QString varName, const IndexPair& index);    //! 添加跟变量关联的item
signals:
    void showEditor(QModelIndex index, bool show);
public slots:

};

#endif // MDRECIPEGRIDMODEL_H
