/****************************************************************************
*
*  Copyright (C) 2002-2008 Helta Kft. / NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: nszabo@helta.hu, info@nocisoft.com
*  Web: www.nocisoft.com
*
*  This file is part of the NCReport reporting software
*
*  Licensees holding a valid NCReport License Agreement may use this
*  file in accordance with the rights, responsibilities, and obligations
*  contained therein. Please consult your licensing agreement or contact
*  nszabo@helta.hu if any conditions of this licensing are not clear
*  to you.
*
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/
#ifndef NCRDPREFERENCES_H
#define NCRDPREFERENCES_H

#include <QSettings>
#include <QPoint>

/*!
Preference class for designer application preferences
*/

/*!

\section 功能

用户偏好设置管理类
【构造函数中存储用户偏好设置参数信息】
＃＃＃在代码中暂时屏蔽了此项菜单栏功能，屏蔽菜单栏中文件首选项功能＃＃＃
*/
class NCRDPreferences
{
public:
    NCRDPreferences();

    QString reportDir, pdfDir, svgDir, outputDir, templateDir, zintDir;
    QString fontFamily;
    int fontPointSize;
    bool gridVisible;
    bool snapToGrid;
    //int gridX;
    //int gridY;
    QPoint grid;
    bool showMaximized;
    int fileCounter;
    bool noPrinterDialog;
    QString defaultPrinterName;
    int printerResolution;
    int printerDPI;
    bool encodePwd;
    int htmlOutputStrategy;
    QString htmlOutputCssFile;
    QString defaultFieldText;
    int defaultFieldWidth;
    int defaultFieldHeight;

    void read( const QSettings& );
    void write( QSettings& );
};

#endif
