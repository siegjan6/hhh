#include "HMIFormData.h"


CHMIFormData::CHMIFormData()
{
    sCopy = QObject::tr("复制");
    sPaste = QObject::tr("粘帖");
    sDelete = QObject::tr("删除");
    sGroup = QObject::tr("成组");
    sUnGroup = QObject::tr("解组");
    sCombine = QObject::tr("联合");
    sUnCombine = QObject::tr("解除联合");
    sGrid = QObject::tr("网格模式");
    sOrtho = QObject::tr("正交模式");
    sLayer = QObject::tr("图层配置");
    sAlignLeft = QObject::tr("左对齐");
    sAlignCenter = QObject::tr("垂直居中");
    sAlignRight = QObject::tr("右对齐");
    sAlignTop = QObject::tr("上对齐");
    sAlignMiddle = QObject::tr("水平居中");
    sAlignBottom = QObject::tr("下对齐");
    sSameWidth = QObject::tr("等宽");
    sSameHeight = QObject::tr("等高");
    sSameSize = QObject::tr("等尺寸");
    sHSpace = QObject::tr("水平等距");
    sIncrHSpace = QObject::tr("扩大水平距离");
    sDecrHSpace = QObject::tr("缩小水平距离");
    sVSpace = QObject::tr("垂直等距");
    sIncrVSpace = QObject::tr("扩大垂直距离");
    sDecrVSpace = QObject::tr("缩小垂直距离");
    sTop = QObject::tr("最前");
    sLast = QObject::tr("最后");
    sFront = QObject::tr("置前一层");
    sBack = QObject::tr("置后一层");
    sFlipX = QObject::tr("水平翻转");
    sFlipY = QObject::tr("垂直翻转");
    sScale = QObject::tr("窗体缩放");
    sScript = QObject::tr("脚本");
    sSaveToStorage = QObject::tr("保存到图库...");
    sSaveToStoragePath = QObject::tr("保存到上次目录");
    sStorage = QObject::tr("图库");
    sFormLayout = QObject::tr("布局预览");
    sBatchEdit = QObject::tr("批量编辑");
    sToolBar = QObject::tr("图形");


    m_orthoAction = NULL;
    m_gridAction = NULL;
    m_layerAction = NULL;
    m_alignLeftAction = NULL;
    m_alignCenterAction = NULL;
    m_alignRightAction = NULL;
    m_alignTopAction = NULL;
    m_alignMiddleAction = NULL;
    m_alignBottomAction = NULL;
    m_sameWidthAction = NULL;
    m_sameHeightAction = NULL;
    m_sameSizeAction = NULL;
    m_hSpaceAction = NULL;
    m_incrHSpaceAction = NULL;
    m_decrHSpaceAction = NULL;
    m_vSpaceAction = NULL;
    m_incrVSpaceAction = NULL;
    m_decrVSpaceAction = NULL;
    m_topAction = NULL;
    m_lastAction = NULL;
    m_frontAction = NULL;
    m_backAction = NULL;
    m_flipXAction = NULL;
    m_flipYAction = NULL;
    m_groupAction = NULL;
    m_unGroupAction = NULL;
    m_combineAction = NULL;
    m_unCombineAction = NULL;
    m_scaleAction = NULL;
    m_scriptAction = NULL;
    m_saveToStorageAction = NULL;
    m_saveToStoragePathAction = NULL;
    m_batchEditAction = NULL;


    //!property
    m_framework = NULL;
    m_common = NULL;
    m_studio = NULL;
    m_run = NULL;

    m_popupMenu = NULL;


    m_editor = NULL;

    m_transMenu0 = NULL;
    m_transMenu1 = NULL;
    m_transPopMenu = NULL;
    m_transToolBar = NULL;
}
