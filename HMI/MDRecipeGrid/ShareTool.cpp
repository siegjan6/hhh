#include "ShareTool.h"
#include <QPainter>
#include <QLabel>
#include <QLineEdit>
#include <QFileDialog>
#include <QTextStream>


#include "MDRecipeGrid.h"

#include "../include/interface/Studio/IStudioInterface.h"
#include "../include/MDRTDBManagerUI/IMDRTDBManagerUI.h"

#include "ValueMap/ValueMapManager.h"

bool CShareTool::m_isStudioMode = true;

CShareTool::CShareTool()
{
}

bool CShareTool::lessThanRow(const QModelIndex &index1, const QModelIndex &index2)
{
    if( index1.row() < index2.row() )
        return true;
    else if( index1.row() == index2.row() && index1.column() < index2.column() )
        return true;
    else
        return false;
}

bool CShareTool::lessThanColumn(const QModelIndex &index1, const QModelIndex &index2)
{
    if( index1.column() < index2.column() )
        return true;
    else if( index1.column() == index2.column() && index1.row() < index2.row() )
        return true;
    else
        return false;
}

bool CShareTool::isRegion(const QModelIndexList &indexList)
{
    qint32 firstRow = indexList.first().row();
    qint32 lastRow = indexList.last().row();
    qint32 firstColumn = indexList.first().column();
    qint32 lastColumn = indexList.last().column();

    if( indexList.size() != (lastRow-firstRow+1) * (lastColumn - firstColumn+1) || 0 == indexList.size() )
        return false;

    qint32 cnter = 0;
    for( qint32 row = firstRow; row <= lastRow; row++ )
    {
       for( qint32 col = firstColumn; col <= lastColumn; col++ )
       {
           if( indexList.at(cnter).row() != row || indexList.at(cnter).column() != col )
               return false;
           cnter++;
       }
    }

    return true;
}

void CShareTool::initPenStyleComboBox(QComboBox *combobox, Qt::PenStyle initStyle)
{
    QSize size = combobox->iconSize();
    combobox->setIconSize(QSize(combobox->size().width(), size.height()));
    for (qint32 i = Qt::SolidLine; i <= Qt::DashDotDotLine; ++i)
    {
        QPixmap map(combobox->iconSize());
        map.fill(Qt::transparent);
        QPainter painter(&map);
        QPen pen((Qt::PenStyle)i);
        pen.setWidth(2);
        painter.setPen(pen);
        painter.drawLine(QPoint(0,map.height()/2), QPoint(map.width(),map.height()/2));

        combobox->addItem(map, "");
    }

    combobox->setCurrentIndex(initStyle - Qt::SolidLine);
}

void CShareTool::initItemTypeComboBox(QComboBox *combobox, itemType initType)
{
    combobox->addItem(QObject::tr("编辑框"), itemType::baseItem);
    combobox->addItem(QObject::tr("单选按钮"), itemType::radioButton);
    combobox->addItem(QObject::tr("复选按钮"), itemType::checkBox);
    combobox->addItem(QObject::tr("列表框"), itemType::comboBox);
    combobox->addItem(QObject::tr("图像"), itemType::image);

    combobox->setCurrentIndex(initType - itemType::baseItem);
}

void CShareTool::initDataSourceTypeComboBox(QComboBox *combobox, dataSourceType initType)
{
    combobox->addItem(QObject::tr("常规"), dataSourceType::normal);
    combobox->addItem(QObject::tr("实时变量"), dataSourceType::realTimeVariable);
    combobox->addItem(QObject::tr("配方变量"), dataSourceType::recipeVariable);

    combobox->setCurrentIndex(initType - dataSourceType::normal);
}

void CShareTool::initValueMapsComboBox(QComboBox *combobox, IDrawEnvrionment *environment, QString initMapName)
{
    QList<QString> names;
    gValueMapManager->valueMapNames(environment, names);
    qint32 initIndex = 0;
    qint32 cnter = 1;
    combobox->addItem("");
    foreach(QString mapName, names)
    {
        if( 0 == initMapName.compare(mapName) )
            initIndex = cnter;
        combobox->addItem(mapName);

        cnter++;
    }

    combobox->setCurrentIndex(initIndex);
}

void CShareTool::setDrawEnvrionment(IDrawEnvrionment *value)
{
    m_isStudioMode = value->isStudioMode();
}

bool CShareTool::selectVariableUI(IDrawEnvrionment *environment, QString &varName)
{
    //! 调用变量对话框
    IMDRTDBManagerUI* varUI = environment->studioProject()->rtdbManagerUI();

    QStringList varNames;
    bool result = varUI->selectVariables(varNames);
    varName = result && varNames.count() > 0 ? varNames.at(0) : "";
    return result;
}

QString CShareTool::setlectImageUI(IDrawEnvrionment *environment, QWidget *parent)
{
    QString projectPath = environment->projectPath() + "/image";
    QString filter = "Images  files(*.png *.bmp *.jpg *.ico)";
    //! 插入图片操作,确认图片存放机制后再补充
    QFileDialog fileDlg(parent, QString(), projectPath, filter);
    fileDlg.setFileMode(QFileDialog::ExistingFile);
    CShareTool::translateWidget(&fileDlg);
    if( fileDlg.exec() != QDialog::Accepted || fileDlg.selectedFiles().count() != 1)
        return "";
    QString totalPath = fileDlg.selectedFiles().at(0);
    if( totalPath.isEmpty() )
        return "";

    QString fileName = totalPath.right(totalPath.count()-totalPath.lastIndexOf("/")-1);
    QString imagePath = projectPath + "/" + fileName;
    if( imagePath.compare(totalPath) )
    {
        bool copy = QFile::copy(totalPath, imagePath);
        if( !copy )
        {
            QString err = QObject::tr("图片复制到工程目录中失败!\r\n文件可能已经存在!\r\npath = ") + imagePath;
            trWarningMessageBox(err, parent);
            return "";
        }
    }

    return fileName;
}

void CShareTool::translateWidget(QObject *obj)
{
    CMDMultiLanguage::instance()->translateWidget(obj);
}

void CShareTool::trWarningMessageBox(const QString & comment, QWidget *parent)
{
    QMessageBox dialog(parent);
    dialog.setWindowTitle(QObject::tr("警告"));
    dialog.setText(comment);
    dialog.setStandardButtons(QMessageBox::Ok);
    dialog.setIcon(QMessageBox::Warning);
    translateWidget(&dialog);
    dialog.exec();
    return;
}

void CShareTool::installDialogTranslator(QDialog *dialog,
                                        const QStringList & exs,
                                        const QStringList &severedNames,
                                        ITranslateWidgetTool *tool)
{
    if(!tool)
        tool = CMDMultiLanguage::createWidgetTool();
    if(tool){
        tool->setExceptionTypes(exs);
        tool->setSeveredNames(severedNames);
        tool->init(dialog);
        tool->changeLanguage();
    }
}

void CShareTool::resetIndex(qint32& startRow, qint32& startCol, qint32& endRow, qint32& endCol)
{
    qint32 srow,erow,scol,ecol;
    if( startRow < endRow ){
        srow = startRow;
        erow = endRow;
    }
    else{
        srow = endRow;
        erow = startRow;
    }
    if( startCol < endCol ){
        scol = startCol;
        ecol = endCol;
    }
    else{
        scol = endCol;
        ecol = startCol;
    }
    if( srow < 0 )  srow = 0;
    if( scol < 0 )  scol = 0;

    startRow = srow;
    endRow = erow;
    startCol = scol;
    endCol = ecol;
}

