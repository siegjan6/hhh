/*!
\author aGRw 2015.03.04
\section 图形窗体，数据类
*/

#ifndef HMIFORMDATA_H
#define HMIFORMDATA_H

#include <QAction>
#include <QToolBar>
#include <QLabel>

#include "IHMIFramework.h"
#include "Environment.h"
#include "Studio.h"
#include "Run.h"
#include "MDJavaScriptCodeEditor.h"

class CHMIFormData
{
public:
    CHMIFormData();
public:
    //!string
    QString sCopy;
    QString sPaste;
    QString sDelete;
    QString sGroup;
    QString sUnGroup;
    QString sCombine;
    QString sUnCombine;
    QString sGrid;
    QString sOrtho;
    QString sLayer;
    QString sAlignLeft;
    QString sAlignCenter;
    QString sAlignRight;
    QString sAlignTop;
    QString sAlignMiddle;
    QString sAlignBottom;
    QString sSameWidth;
    QString sSameHeight;
    QString sSameSize;
    QString sHSpace;
    QString sIncrHSpace;
    QString sDecrHSpace;
    QString sVSpace;
    QString sIncrVSpace;
    QString sDecrVSpace;
    QString sTop;
    QString sLast;
    QString sFront;
    QString sBack;
    QString sFlipX;
    QString sFlipY;
    QString sScale;
    QString sScript;
    QString sSaveToStorage;
    QString sSaveToStoragePath;
    QString sStorage;
    QString sFormLayout;
    QString sBatchEdit;
    QString sToolBar;

    //toolbar
    QList<QToolBar*> m_toolBarList;

    QAction* m_orthoAction;
    QAction* m_gridAction;
    QAction* m_layerAction;
    QAction* m_alignLeftAction;
    QAction* m_alignCenterAction;
    QAction* m_alignRightAction;
    QAction* m_alignTopAction;
    QAction* m_alignMiddleAction;
    QAction* m_alignBottomAction;
    QAction* m_sameWidthAction;
    QAction* m_sameHeightAction;
    QAction* m_sameSizeAction;
    QAction* m_hSpaceAction;
    QAction* m_incrHSpaceAction;
    QAction* m_decrHSpaceAction;
    QAction* m_vSpaceAction;
    QAction* m_incrVSpaceAction;
    QAction* m_decrVSpaceAction;
    QAction* m_topAction;
    QAction* m_lastAction;
    QAction* m_frontAction;
    QAction* m_backAction;
    QAction* m_flipXAction;
    QAction* m_flipYAction;
    QAction* m_groupAction;
    QAction* m_unGroupAction;
    QAction* m_combineAction;
    QAction* m_unCombineAction;
    QAction* m_scaleAction;
    QAction* m_scriptAction;
    QAction* m_saveToStorageAction;
    QAction* m_saveToStoragePathAction;
    QAction* m_batchEditAction;

    //!statusBar
    QLabel m_labelMouse;//!鼠标位置
    QLabel m_labelLocation;//!控件位置
    QLabel m_labelSize;//!控件尺寸

    //!property
    IHMIFramework* m_framework;
    CEnvironment* m_common;
    CStudio* m_studio;
    CRun* m_run;

    QMenu* m_popupMenu;
    QPoint m_mousePos;
    QList<QMenu*> m_menuList;

    CMDJavaScriptCodeEditor* m_editor;

    //!language
    ITranslateWidgetTool* m_transMenu0;
    ITranslateWidgetTool* m_transMenu1;
    ITranslateWidgetTool* m_transPopMenu;
    ITranslateWidgetTool* m_transToolBar;
};

#endif // HMIFORMDATA_H
