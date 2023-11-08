/*!
author Y2hvdWps 2016.08.29
section 配方表格视图类
*/
#ifndef MDRECIPEGRIDVIEW_H
#define MDRECIPEGRIDVIEW_H

#include <QTableView>
#include <QContextMenuEvent>
#include <QMenu>
#include <QPrinter>
#include "RecipeGridModel.h"
#include "RecipeGridDelegate.h"
#include "GridItems/GridBaseItem.h"
#include "ConfigDialogs/PrintConfigDialog.h"
#include "MDMultiLanguage.h"


#define sLeftButtonClicked      "onLClicked"
#define sRightButtonClicked     "onRClicked"
#define sEditFinish             "onEditFinish"

class IHMIForm;
class CRowFrozenView;
class CColumnFrozenView;
class CFixedFrozenView;
class CPrintCloneView;
class CMDRecipeGrid;

enum class viewType{
    mainView=0,
    rowFrozenView,
    columnFrozenView,
    fixedFrozenView,
    printView
};

typedef struct propertypara
{
    bool showGrid;
    bool highLight;
    bool multiLanguage;
    bool softKeyboard;
    bool clickEdit;
    qint32 verticalHeaderWidth;
    qint32 horizontalHeaderHeight;
}PropertyPara;

class CRecipeGridView : public QTableView
{
    friend class CAbstractFrozenView;
    friend class CPrintCloneView;
    friend class CRecipeGridDelegate;
    friend class CMDRecipeGrid;
    Q_OBJECT

public:
    explicit CRecipeGridView(QWidget *parent = 0);
    CRecipeGridView(const CRecipeGridView&) = default;
    virtual ~CRecipeGridView();

    virtual viewType tableViewType(){return viewType::mainView;}
    virtual void init(qint32 rowNum=-1, qint32 columnNum=-1); //! 部分初始化操作,与子类实现差异化
    bool isItemFrozen(const QModelIndex& index) const;    //! item是否被固定
    virtual qint32 frozenRowCount(){return m_rowFrozen;}
    virtual qint32 frozenColumnCount(){return m_columnFrozen;}
    void onInitialization(IDrawEnvrionment *environment, IHMIForm *mainform, QString jsObjName);    //! 反序列化后,调整更新视图元素
    bool multiLanguage() const {return m_propertyPara.multiLanguage;}

protected:
    /*!
    事件处理qDebug("combo edited row=%d col=%d  index=%d", index.row(), index.column(), combobox->currentIndex());
    */
    virtual void paintEvent(QPaintEvent *event);
    virtual void contextMenuEvent(QContextMenuEvent * event);
    virtual void resizeEvent(QResizeEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

protected:
    void commonInit();   //! 和子类们共同的初始化操作
    bool isItemMerged(const QModelIndex& index) const;    //! 是否是合并的单元格
    virtual QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers);
    virtual void scrollTo (const QModelIndex & index, ScrollHint hint = EnsureVisible);
    virtual void setSelection(const QRect & rect, QItemSelectionModel::SelectionFlags flags);


private:
    void initTranslator(); //! 初始化多语言翻译tool
    void initHeader();  //! 初始化横纵表头
    void initMenu();    //! 初始化右键菜单
    void showAllEditor();    //! 在运行环境下，所有特殊类型的item显示为相应的QWiget
    /*!
    获取设置合并过的单元格信息,用于序列化
    */
    typedef QPair<qint32,qint32> IndexPair;
    typedef QPair<qint32,qint32> SpanPair;
    typedef QMap<IndexPair, SpanPair> SpanMap;
    void spanMap(SpanMap & spanMap);
    void setSpanForItems(const SpanMap& spanMap);
    void mySetSpan(int row, int column, int rowSpan, int columnSpan);
    /*!
    get/set每个行列的高宽度,用于序列化
    */
    typedef QMap<qint32, qint32> SectionSizeMap;
    void diffSectionSize(Qt::Orientation orientation, SectionSizeMap& sections);    //! 获取行列宽度和默认值不一样的行列
    void refreshGroups();   //!重新将所有成组的items打开/收缩一次.用于反序列化.将来可能用于之类view
    /*!
    \section  序列化
    */
#define MagicNumber 0x5AA5
#define InvalidVersion      0
#define FirstReadVersion    1
#define WriteVersion FirstReadVersion
    qint32 m_readVersion;
    qint32 m_writeVersion;
public:
    //! json格式接口
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

    void setModify(bool modify);
    CRecipeGridView &operator=(CRecipeGridView& other);
    /*!
    \section  变量相关
    */
    void getExpressionList(QList<int>& indexs, QList<QString>& exps);
    void setValue(const qint32 index, QVariant variant);
    /*!
    \section  多语言
    */
    void changeLanguage();
    void collectWords();

public:
    IDrawEnvrionment *m_environment;
protected://var
    CRecipeGridModel* m_model;
    CRecipeGridDelegate* m_delegate;
    frameData m_frozenFrame;
    PrintPara m_printPara;             //! 打印设置参数
    PropertyPara m_propertyPara;       //! 表格基本属性
    void setDefaultProperty(PropertyPara& property);          //! 表格基本属性赋初值
    void updateProperty();              //! 更新各个属性并生效
private://var
    QMenu* m_menu;
    IHMIForm* m_mainForm;
    QAction* m_enableMultiLanguage;
    QAction* m_disableMultiLanguage;
    ITranslateWidgetTool* m_tool;
    QPoint m_clickPosition;            //! 记录单击item的坐标,用于点击编辑功能
    QList<QAction*> m_actionList;       //! 菜单中主要功能的aciton列表
    QModelIndexList m_copyIndexs;       //! 复制区域的临时索引列表
    QList<CGridBaseItem*> m_copyItems;    //! 复制区域的item列表
    void invokeFrozenViewSameAction(const QModelIndex& index);     //! 触发固定行列子views们的相同index的action
    void createForzenViews();   //!创建固定行列子views
    /*!
    固定行列功能
    */
    CFixedFrozenView *m_fixedFrozenView;
    CRowFrozenView *m_rowFrozenView;
    CColumnFrozenView *m_columnFrozenView;
    qint32 m_rowFrozen;
    qint32 m_columnFrozen;
    void mySectionHidden(Qt::Orientation orientation, int index, bool hide);

signals:
    void updateFrozenViewGeometry();
    void invokeAction(qint32 index);
    void sectionHiddenChanged(Qt::Orientation orientation, int index, bool hide);
    void showGridChanged(bool show);
    void setSpanChanged(int row, int column, int rowSpan, int columnSpan);
    void showEditorFromFrozenView(const QModelIndex & index);

protected slots:
    void triggerAction(qint32 index);   //! 配合invokeAction信号，让frozenViews触发action
    void showEditorChanged(QModelIndex index, bool show);   //! 关闭/显示特殊item的editor部件
    void closeEditor(QWidget * editor, QAbstractItemDelegate::EndEditHint hint);
private slots:
    void horizontalSectionResized(int logicalIndex, int /*oldSize*/, int newSize);
    void verticalSectionResized(int logicalIndex, int /*oldSize*/, int newSize);

    void clickedItem(const QModelIndex & index);

    /*!
    右键菜单:主要功能
    */
    void configItem();                  //! 设置单元格
    void configMulitItems();            //! 设置多个单元格
    void setRecipeName();               //! 设置配方名称
    void setDataSource();               //! 设置数据源
    void setItemFont();                 //! 字体
    void setItemBackground();           //! 背景
    void setItemFrame();                //! 边框
    void setItemAlign();                //! 对齐方式
    void mergeItem();                   //! 合并单元格
    void splitItem();                   //! 拆分单元格

    void setGroup();                    //! 成组
    virtual void expandCollapseGroup(const QModelIndex& index, bool manualExpand=false);   //!展开/收缩组
    void splitGroup();                  //! 解组

    void configRecipeTemplate();        //! 设置配方模板

    void addRowColumn();                //! 添加行/列
    void deleteRowColumn();             //! 删除行/列
    void insertRowColumn();             //! 插入行/列
    void frozenRowColumn();             //! 固定行列
    void setRowHightcolumnWidth();      //! 行高列宽

    void enableMultiLanguage() {m_propertyPara.multiLanguage = true; setModify(true);}     //! 启用多语言
    void disableMultiLanguage() {m_propertyPara.multiLanguage = false; setModify(true);}   //! 禁用多语言

    void printConfig();                 //! 打印配置
    void printPreview();                //! 预览
    void print();                       //! 打印

    void copyRegion();                  //! 复制区域
    void pasteRegion();                 //! 粘贴
    void replacePasteRegion();          //! 替换粘贴
    void clearText();                   //! 清除文本
    void setTableViewProperty();        //! 属性


private:
    //! 供自身和脚本使用
    void didCopyRegion(const QModelIndexList& selectList);
    void didReplacePasteRegion(qint32 curRow, qint32 curColumn, QString strSrc, QString strDes);
    /*!
    \section  脚本接口函数相关
    */
public:
    QString m_jsObjName;
public slots:
    void itemEditFinishEvent();
    void itemClickEvent(QPoint pt, QMouseEvent* event);
private:
    void redrawAll();
    void redrawCell(qint32 row, qint32 col);
    void redrawRange(qint32 startRow, qint32 startCol, qint32 endRow, qint32 endCol);
    qint32 getNumberRows();
    void setNumberRows(qint32 count);
    void insertRows(qint32 startRow, qint32 count);
    void deleteRows(qint32 startRow, qint32 count);
    qint32 getRowHeight(qint32 row);
    void setRowHeightJs(qint32 row, qint32 height);
    qint32 getNumberCols();
    void setNumberCols(qint32 count);
    void insertCols(qint32 startCol, qint32 count);
    void deleteCols(qint32 startCol, qint32 count);
    qint32 getColWidth(qint32 col);
    void setColWidth(qint32 col, qint32 width);
    void gotoRow(qint32 row);
    void gotoCol(qint32 col);
    QString getCellText(qint32 row, qint32 col);
    void setCellText(qint32 row, qint32 col, QString text);
    void enableRegionEdit(qint32 startRow, qint32 startCol, qint32 endRow, qint32 endCol, bool enableEdit);
    void setBackgroundColor(qint32 startRow, qint32 startCol, qint32 endRow, qint32 endCol, qint32 red, qint32 green, qint32 blue);
    void copyAndReplaceRegion(qint32 startRow, qint32 startCol, qint32 endRow, qint32 endCol, qint32 toRow, qint32 toCol, QString strSrc, QString strDes);
    void getRangeRecipeName(qint32 startRow, qint32 startCol, qint32 endRow, qint32 endCol, QStringList& nameArray);
};

#endif // MDRECIPEGRIDVIEW_H
