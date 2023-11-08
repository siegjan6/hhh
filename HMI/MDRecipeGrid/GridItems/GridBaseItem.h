/*!
author Y2hvdWps 2016.08.29
section 单元格模型基类
*/
#ifndef GRIDBASEITEM_H
#define GRIDBASEITEM_H
#include<QPen>
#include <QStandardItem>
#include <QStyleOptionViewItem>
#include <QJsonObject>
#include <QJsonArray>

#include "IDrawEnvironment.h"
#include "ValueMap/ValueMapManager.h"

class CRecipeGridModel;

enum itemType
{
    baseItem = QStandardItem::UserType + 1,
    radioButton,
    checkBox,
    comboBox,
    image
};
enum dataSourceType
{
    normal = 0, //! 常规=普通字符串
    realTimeVariable,   //! 实时变量
    recipeVariable      //! 配方变量
};
enum frameType{
    noframe = 0,
    leftframe = 1 << 0,
    topframe = 1 << 1,
    rightframe = 1 << 2,
    bottomframe = 1 << 3,
    allframe = leftframe | topframe | rightframe | bottomframe
};

struct frameData{
    frameType type;
    QPen pen;
};

class CRecipeGridDelegate;


class CGridBaseItem : public QStandardItem
{
    friend class CItemCfgDialog;
    friend class CMulitItemCfgDialog;
    friend class CRecipeGridModel;
public:
    CGridBaseItem();
    virtual ~CGridBaseItem();

    static CGridBaseItem *createItem(itemType type);
    QWidget *createEditorWidget(QWidget *parent, const QStyleOptionViewItem &option, const CRecipeGridDelegate* delegate);
    virtual bool needCustomPaint(QPainter *, const QStyleOptionViewItem &) {return false;}
    virtual bool needCustomSetModelData(QWidget *, QAbstractItemModel *, const QModelIndex &) {return false;}
    virtual bool canShowPersistentEditor() {return false;}
    virtual void onInitialization(IDrawEnvrionment *environment);    //! 反序列化后的一些更新操作
    virtual void enableEdit(bool edit, QWidget* editor);
    /*!
    section 必须重载的接口,model会使用这些接口
    */
    virtual int	type() const {return itemType::baseItem;}
    virtual CGridBaseItem *clone() const;
    CGridBaseItem *myClone(bool clearGroupInfo=true);
public:
    bool isValueOutOfRange(QString inputString); //! 数据是否超限
    bool compareItem(const CGridBaseItem& item); //!是否是有效的item,用于序列化
    void addAssociateItem(CRecipeGridModel* model);    //! 添加跟变量关联的item
    void removeAssociateItem(CRecipeGridModel* model);    //! 添加跟变量关联的item

    virtual void serialize(QJsonObject& json, CRecipeGridModel* mainModel);
    virtual void deserialize(const QJsonObject& json);
    /*!
    \section  多语言
    */
    void changeLanguage();
    void collectWords();
    virtual bool canTranslation();
    /*!
    成组/解组相关
    */
    bool expand() const {return m_expand;}
    void setExpand(bool expand); //! 成组后展开/收缩
    quint32 rowHidden() const {return m_rowHiddenNum;}
    void setRowHidden(quint32 rowHiddenNum); //! 设置成组/解组后隐藏的行数

    frameData frame() const {return m_frame;}
    void setFrame(const frameData& frame) { m_frame = frame; }

    dataSourceType dataSource() const {return m_dataSource; }
    void setDataSource(dataSourceType type) { m_dataSource = type; }

    QString recipeName() const {return m_recipeName;}

    qint32 decimalCount() const {return m_decimalCount;}
    QString dateFormat() const {return m_dateFormat;}
    QString stringOutRange() const {return m_strOutRange;}
    bool showWhenOutRange() const {return m_showOutRange;}
    bool valueRangeCheckable() const {return m_valueRangeCheckable;}
    bool variableBind() const {return m_variableBind;}
    QString minVarBinded() const {return m_minVariable;}
    QString maxVarBinded() const {return m_maxVariable;}

    QString variableName() const {return m_varName;}
    //! 运行环境下,记录是否是有效的变量
    bool isValidInRuntime() const {return m_validInRuntime;}
    void setValidInRuntime(bool valid) { m_validInRuntime = valid; }
    virtual void updateValidUIState(); //! 有效/无效时ui上的变化
    virtual bool updateShowString(QVariant value, QWidget* editor=NULL);  //! 用于运行环境下,更新显示字符串
    static void setDefaultFrame(frameData& frame);

protected:
    CGridBaseItem(const CGridBaseItem& srcitem);
    CGridBaseItem &operator=(const CGridBaseItem&) = default;
    void copyMembers(const CGridBaseItem* srcitem, bool clearGroupInfo = true);
    virtual QWidget *createEditor(QWidget */*parent*/, const QStyleOptionViewItem &/*option*/, const CRecipeGridDelegate*){return NULL;}  //! 不同子类的items去创建自己的editor
    qint32 indexOfValue(QVariant value);
    QString stringValue(QVariant value);
protected://var
    IDrawEnvrionment *m_environment;
    bool m_expand;  //! 展开/收缩标志
    quint32 m_rowHiddenNum;  //! 成组后隐藏的行数

    frameData m_frame;  //! 边框参数
    dataSourceType m_dataSource;   //!数据源类型
    qint32 m_decimalCount;
    bool m_multiLanguage;   //! 是否支持多语言
    QString m_valueMapName; //! 数值映射表名称
    QString m_dateFormat;   //! 日期格式
    bool m_editable;        //! 是否可编辑
    /*!
    数值检查相关
    */
    bool m_valueRangeCheckable;   //! 是否检查数值范围
    bool m_variableBind;    //!是否为最大最小值绑定变量
    bool m_showOutRange;    //! 超限时,是否显示提示框
    qreal m_minValue;   //! 最小值
    qreal m_maxValue;   //! 最大值
    QString m_minVariable;  //!最小值绑定的变量,为空则不绑定变量
    QString m_maxVariable;  //!最大值绑定的变量,为空则不绑定变量
    QString m_strOutRange;  //! 超限时候的提示信息

    Qt::ItemDataRole m_firstRole;   //!Qt首位角色
    Qt::ItemDataRole m_finalRole;   //!Qt末尾角色
    QString m_varName;  //! 实时变量名称
    QString m_recipeName;   //!配方名称
    bool m_validInRuntime;  //! 运行环境下,是否是有效的变量

    QString m_originText;   //!最后一次更新到多语言字典的原始字符串
    CValueMapManager::Values m_values;  //! 数值映射表数据容器list
    CValueMapManager::ValueIndex m_valueIndex; //! 数值映射表的hash版本
};

#endif // GRIDBASEITEM_H
