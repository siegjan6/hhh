/****************************************************************************
*
*  Copyright (C) 2002-2014 Helta Kft. - NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: norbert@nocisoft.com, office@nocisoft.com
*  Web: www.nocisoft.com
*
*  Created: 2014. 08. 25. (nszabo)
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/

#include "ncrdexpressionbuilderdialog.h"
#include "ui_ncrdexpressionbuilderdialog.h"
#include "ncreportitem.h"
#include "ncreportdef.h"
#include "ncrddocument.h"
#include "ncreportevaluator.h"
#include "ncrdmainwindow.h"
#include "ncrddef.h"
#include "ncreportdatasource.h"
#include "scripthighlighter.h"
#include "ncrdsqltextedit.h"

#include <QPushButton>
#include <QButtonGroup>

//! 多语言
#include "MDMultiLanguage.h"

NCRDExpressionBuilderDialog::NCRDExpressionBuilderDialog(NCRDDocument* document, const QList<int> &validTypes, int sourceType, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NCRDExpressionBuilderDialog),
    /*m_item(item),*/
    m_document(document),
    m_bg(new QButtonGroup(this))
{
    ui->setupUi(this);

    ui->editor->setFont(QFont("Courier",10));
    new ScriptHighlighter(ui->editor->document());
    ui->editor->installEventFilter(new NCRDTextEditKeyPressEater(ui->editor, "    "));

    ui->comboRole->setVisible(false);
    ui->lblRole->setVisible(false);

    QList<int> sizes;
    sizes << 200 << 400;
    ui->splitter->setSizes( sizes );

    ui->comboSt->setAvailableItems(validTypes);
    ui->comboSt->setCurrentIndex( sourceType );
    ui->comboDataSource->addItems(document->reportDef()->dataSourceIdList());
    ui->comboVar->addItems(document->reportDef()->variableIdList());
//    ui->comboPar->addItems(document->mainWindow()->dummyReportDef()->parameterIdList());
    ui->comboPar->addItems(document->mainWindow()->activeDocument()->reportDef()->parameterIdList());
    ui->comboDSFunction->addItems(NCReportEvaluator::arrayToStringList(NCReportEvaluator::availableDataSourceFunctions()));
    ui->comboValueFunction->addItems(NCReportEvaluator::arrayToStringList(NCReportEvaluator::availableValueFunctions()));

    QStringList roles;
    roles << "" << def_strMarkEditRole << def_strMarkUserRole << def_strMarkDecorationRole << def_strMarkDisplayRole << def_strMarkHeaderRole;
    roles << def_strMarkHeaderDecorationRole << def_strMarkHeaderUserRole;
    roles << def_strMarkHeaderDisplayRole << def_strMarkHeaderToolTipRole;
    ui->comboRole->addItems(roles);

    connect(ui->comboDataSource, SIGNAL(currentIndexChanged(int)), this, SLOT(updateDataSourceColumns()));
    connect(ui->comboSt, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSourceType()));
    connect(ui->btnAddDataSource,SIGNAL(clicked()), this, SLOT(addDataSource()));
    connect(ui->btnAddColumn,SIGNAL(clicked()), this, SLOT(addDataSourceColumn()));
    connect(ui->btnAddDSFunc,SIGNAL(clicked()), this, SLOT(addDataSourceFunction()));
    connect(ui->btnAddDataFunc,SIGNAL(clicked()), this, SLOT(addValueFunction()));
    connect(ui->btnAddPar,SIGNAL(clicked()), this, SLOT(addParameter()));
    connect(ui->btnAddVar,SIGNAL(clicked()), this, SLOT(addVariable()));
    connect(ui->btnReset,SIGNAL(clicked()), ui->editor, SLOT(clear()));
    if (ui->buttonBox->button(QDialogButtonBox::Reset))
        connect(ui->buttonBox->button(QDialogButtonBox::Reset), SIGNAL(clicked()), ui->editor, SLOT(clear()));

    m_bg->addButton(ui->btnPlus);
    m_bg->addButton(ui->btnMinus);
    m_bg->addButton(ui->btnLess);
    m_bg->addButton(ui->btnMulti);
    m_bg->addButton(ui->btnDiv);
    m_bg->addButton(ui->btnGreather);
    m_bg->addButton(ui->btnEqual);
    m_bg->addButton(ui->btnNotEqual);
    m_bg->addButton(ui->btnLessEq);
    m_bg->addButton(ui->btnAnd);
    m_bg->addButton(ui->btnOr);
    m_bg->addButton(ui->btnGrEq);
    m_bg->addButton(ui->btnNot);
    m_bg->addButton(ui->btnXor);
    m_bg->addButton(ui->btnCompareEqual);

    connect(m_bg,SIGNAL(buttonClicked(QAbstractButton*)),this,SLOT(onButtonClicked(QAbstractButton*)));
    connect(ui->treeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(addTreeItem(QTreeWidgetItem*,int)));

    updateDataSourceColumns();
    updateSourceType();

    ui->treeWidget->setErrorHandler(document->reportDef()->error());
    ui->treeWidget->updateDataSources(document->reportDef());
    ui->treeWidget->updateVariables(document->reportDef());
    ui->treeWidget->updateParameters(document->mainWindow()->dummyReportDef());
    ui->treeWidget->updateParameters(document->mainWindow()->activeDocument()->reportDef());

    QStringList svlist = NCReportEvaluator::availableSystemVariables();
    for (int i=0;i<svlist.count();i++) {
        ui->treeWidget->addSystemVariable(svlist.at(i));
        ui->comboVar->addItem(svlist.at(i));
    }

    //! 多语言
    CMDMultiLanguage::instance()->translateWidget(this);
}

NCRDExpressionBuilderDialog::~NCRDExpressionBuilderDialog()
{
    delete ui;
}

QString NCRDExpressionBuilderDialog::expression() const
{
    return ui->editor->toPlainText();
}

void NCRDExpressionBuilderDialog::setExpression(const QString &exp)
{
    ui->editor->setPlainText(exp);
    ui->editor->selectAll();
}

void NCRDExpressionBuilderDialog::setSourceType(int stype)
{
    ui->comboSt->setCurrentIndex(stype);
}

int NCRDExpressionBuilderDialog::sourceType() const
{
    return ui->comboSt->currentIndex();
}

void NCRDExpressionBuilderDialog::updateDataSourceColumns()
{
    ui->comboColumn->clear();
    if (ui->comboDataSource->currentText().isEmpty())
        return;

    NCReportDataSource *ds = m_document->reportDef()->dataSources().value(ui->comboDataSource->currentText());
    if (ds) {
        ui->comboColumn->addItems(ds->columnNames());
    }
}

void NCRDExpressionBuilderDialog::updateSourceType()
{
    int dst = ui->comboSt->currentIndex();
    switch (dst) {
    case NCReportItem::Static:
    case NCReportItem::File:
    case NCReportItem::Url:
    case NCReportItem::InternalDocument:
        break;
    case NCReportItem::FileNameFromDataSource:
    case NCReportItem::UrlFromDataSource:
    case NCReportItem::DataSource: {
        ui->comboDataSource->setEnabled(true);
        ui->btnAddDataSource->setEnabled(true);
        ui->comboColumn->setEnabled(true);
        ui->btnAddColumn->setEnabled(true);
        ui->comboDSFunction->setEnabled(true);
        ui->btnAddDSFunc->setEnabled(true);
        ui->comboValueFunction->setEnabled(true);
        ui->btnAddDataFunc->setEnabled(true);
        ui->comboVar->setEnabled(false);
        ui->btnAddVar->setEnabled(false);
        ui->comboPar->setEnabled(false);
        ui->btnAddPar->setEnabled(false);
        break;
    }
    case NCReportItem::Parameter: {
        ui->comboDataSource->setEnabled(false);
        ui->btnAddDataSource->setEnabled(false);
        ui->comboColumn->setEnabled(false);
        ui->btnAddColumn->setEnabled(false);
        ui->comboDSFunction->setEnabled(false);
        ui->btnAddDSFunc->setEnabled(false);
        ui->comboValueFunction->setEnabled(false);
        ui->btnAddDataFunc->setEnabled(false);
        ui->comboVar->setEnabled(false);
        ui->btnAddVar->setEnabled(false);
        ui->comboPar->setEnabled(true);
        ui->btnAddPar->setEnabled(true);
        break;
    }
    case NCReportItem::Variable:
    case NCReportItem::SystemVariable: {
        ui->comboDataSource->setEnabled(false);
        ui->btnAddDataSource->setEnabled(false);
        ui->comboColumn->setEnabled(false);
        ui->btnAddColumn->setEnabled(false);
        ui->comboDSFunction->setEnabled(false);
        ui->btnAddDSFunc->setEnabled(false);
        ui->comboValueFunction->setEnabled(false);
        ui->btnAddDataFunc->setEnabled(false);
        ui->comboVar->setEnabled(true);
        ui->btnAddVar->setEnabled(true);
        ui->comboPar->setEnabled(false);
        ui->btnAddPar->setEnabled(false);
        break;
    }
    case NCReportItem::ScriptExpression: {
    case NCReportItem::Template:
        ui->comboDataSource->setEnabled(true);
        ui->btnAddDataSource->setEnabled(true);
        ui->comboColumn->setEnabled(true);
        ui->btnAddColumn->setEnabled(true);
        ui->comboDSFunction->setEnabled(true);
        ui->btnAddDSFunc->setEnabled(true);
        ui->comboValueFunction->setEnabled(true);
        ui->btnAddDataFunc->setEnabled(true);
        ui->comboVar->setEnabled(true);
        ui->btnAddVar->setEnabled(true);
        ui->comboPar->setEnabled(true);
        ui->btnAddPar->setEnabled(true);
        break;
    }

    default:
        break;
    }
    for (int i=0; i<m_bg->buttons().count(); i++)
        m_bg->buttons().at(i)->setEnabled((dst==NCReportItem::ScriptExpression || dst==NCReportItem::Template));
}

void NCRDExpressionBuilderDialog::addDataSource()
{
    addSymbol(ui->comboDataSource->currentText(), NCReportItem::DataSource);
}

void NCRDExpressionBuilderDialog::addDataSourceColumn()
{
    addSymbol(ui->comboDataSource->currentText()+"."+ui->comboColumn->currentText(), NCReportItem::DataSource);
}

void NCRDExpressionBuilderDialog::addDataSourceFunction()
{
    addSymbol(ui->comboDataSource->currentText()+"."+ui->comboDSFunction->currentText()+"()", NCReportItem::DataSource);
}

void NCRDExpressionBuilderDialog::addValueFunction()
{
    QString arguments;
    int argnum = NCReportEvaluator::availableValueFunctionArguments()[ui->comboValueFunction->currentIndex()];
    switch (argnum)
    {
    case 0: return;
    case 1: arguments = "(n)"; break;
    case 2: arguments = "(n,m)"; break;
    }
    addSymbol(ui->comboDataSource->currentText()+"."+ui->comboColumn->currentText()+"."
              +ui->comboValueFunction->currentText()+arguments, NCReportItem::DataSource);
}

void NCRDExpressionBuilderDialog::addVariable()
{
    addSymbol(ui->comboVar->currentText(), NCReportItem::Variable);
}

void NCRDExpressionBuilderDialog::addParameter()
{
    addSymbol(ui->comboPar->currentText(), NCReportItem::Parameter);
}

void NCRDExpressionBuilderDialog::addTreeItem(QTreeWidgetItem* item, int column)
{
    Q_UNUSED(column)
    NCRDDataSourceTree::ItemDescriptor d = ui->treeWidget->parseItem(item);
    if (d.itemSourceType>0)
        addSymbol(d.id, d.itemSourceType);

//    if (item->type()==NCRDDataSourceTree::DataSourceColumn)
//        addSymbol(item->parent()->text(column)+"."+item->text(column), NCReportItem::DataSource);
//    else if (item->type()==NCRDDataSourceTree::Variable)
//        addSymbol(item->text(column), NCReportItem::Variable);
//    else if (item->type()==NCRDDataSourceTree::SysVar)
//        addSymbol(item->text(column), NCReportItem::Variable);
//    else if (item->type()==NCRDDataSourceTree::Parameter)
//        addSymbol(item->text(column), NCReportItem::Parameter);
}

void NCRDExpressionBuilderDialog::onButtonClicked(QAbstractButton *btn)
{
    QString s;
    if (btn==ui->btnPlus) s = "+";
    else if (btn==ui->btnMinus) s = "-";
    else if (btn==ui->btnLess) s = "<";
    else if (btn==ui->btnMulti) s = "*";
    else if (btn==ui->btnDiv) s = "/";
    else if (btn==ui->btnGreather) s = ">";
    else if (btn==ui->btnEqual) s = "=";
    else if (btn==ui->btnNotEqual) s = "!=";
    else if (btn==ui->btnLessEq) s = "<=";
    else if (btn==ui->btnAnd) s = "&";
    else if (btn==ui->btnOr) s = "|";
    else if (btn==ui->btnGrEq) s = ">=";
    else if (btn==ui->btnNot) s = "!";
    else if (btn==ui->btnXor) s = "^";
    else if (btn==ui->btnCompareEqual) s = "==";

    ui->editor->insertPlainText(s.prepend(" ").append(" "));
    ui->editor->setFocus();
}

bool NCRDExpressionBuilderDialog::isComplexExpression() const
{
    switch (ui->comboSt->currentIndex()) {
    case NCReportItem::ScriptExpression:
    case NCReportItem::Template:
        return true;
    default:
        break;
    }
    return false;
}

void NCRDExpressionBuilderDialog::addSymbol(const QString &exp, int dstype)
{
    QString s(exp);
    NCReportItem::SourceType type = (NCReportItem::SourceType)dstype;
    if (isComplexExpression())
        NCReportEvaluator::tokenizeKeyWord(s, type );
    else
        ui->editor->clear();

    ui->editor->insertPlainText(s);
    ui->editor->setFocus();
}
