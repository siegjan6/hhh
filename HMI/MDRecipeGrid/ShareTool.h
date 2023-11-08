/*!
author Y2hvdWps 2016.09.09
section 提供可共享的方法的工具类
*/
#ifndef SHARETOOL_H
#define SHARETOOL_H
#include <QModelIndex>
#include <QComboBox>
#include <QMessageBox>
#include <QDialog>

#include "GridItems/GridBaseItem.h"
#include "IDrawEnvironment.h"
#include "MDMultiLanguage.h"


#define RegExp_RealNumber "^-?\\d+(\\.\\d+)?$"

class CShareTool
{
    CShareTool();
public:
    static bool m_isStudioMode;
public:
    static bool lessThanRow(const QModelIndex& index1, const QModelIndex& index2);
    static bool lessThanColumn(const QModelIndex& index1, const QModelIndex& index2);
    static bool isRegion(const QModelIndexList& indexList);

    static void initPenStyleComboBox(QComboBox* combobox, Qt::PenStyle initStyle = Qt::SolidLine);
    static void initItemTypeComboBox(QComboBox* combobox, itemType initType = itemType::baseItem);
    static void initDataSourceTypeComboBox(QComboBox* combobox, dataSourceType initType = dataSourceType::normal);
    static void initValueMapsComboBox(QComboBox* combobox, IDrawEnvrionment *environment, QString initMapName= "");

    static void setDrawEnvrionment(IDrawEnvrionment *value);
    static bool selectVariableUI(IDrawEnvrionment *environment, QString& varName);
    static QString setlectImageUI(IDrawEnvrionment *environment, QWidget* parent = 0);
    static void translateWidget(QObject* obj);
    static void trWarningMessageBox(const QString & comment,QWidget *parent = 0);
    static void installDialogTranslator(QDialog *dialog,
                                        const QStringList & exs = QStringList("QLineEdit"),
                                        const QStringList &severedNames = QStringList(),
                                        ITranslateWidgetTool *tool = NULL);
    static void resetIndex(qint32 &startRow, qint32 &startCol, qint32 &endRow, qint32 &endCol);
};

#endif // SHARETOOL_H
