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
#include "ncrdpreferences.h"

NCRDPreferences::NCRDPreferences()
{
    fontPointSize =10;
    gridVisible = false;
    snapToGrid = false;
    //gridX = 6;
    //gridY = 6;
    showMaximized = false;
    grid = QPoint(6,6);
    printerResolution = 0; // HIGH
    noPrinterDialog = false;
    printerDPI = -1;
    encodePwd = false;
    htmlOutputStrategy = 0;
    defaultFieldWidth = 0;
    defaultFieldHeight = 0;
}

void NCRDPreferences::read(const QSettings & settings)
{
    reportDir = settings.value("reportdir", "").toString();
    pdfDir = settings.value("pdfdir", "").toString();
    svgDir = settings.value("svgdir", "").toString();
    outputDir = settings.value("outputdir", "").toString();
    templateDir = settings.value("templatedir", "").toString();
    zintDir = settings.value("zintdir", "").toString();

    fontFamily = settings.value("defaultfontfamily", "Arial").toString();
    fontPointSize = settings.value("defaultfontpointsize", 10).toInt();
    gridVisible = settings.value("gridvisible", false).toBool();
    snapToGrid = settings.value("snaptogrid", true).toBool();
    grid.setX( settings.value("gridX", 6).toInt() );
    grid.setY( settings.value("gridY", 6).toInt() );
    showMaximized = settings.value("showmaximized", false).toBool();
    fileCounter = settings.value("filecounter", 1).toInt();
    printerResolution = settings.value("printerResolution", 0).toInt();
    noPrinterDialog = settings.value("noPrinterDialog", false).toBool();
    defaultPrinterName = settings.value("defaultPrinterName","").toString();
    printerDPI = settings.value("printerDPI","").toInt();
    encodePwd = settings.value("bpa", false).toBool();
    htmlOutputStrategy = settings.value("htmlOutputStrategy",0).toInt();
    htmlOutputCssFile = settings.value("htmlOutputCssFile","").toString();
    defaultFieldText =  settings.value("defaultFieldText","").toString();
    defaultFieldWidth =  settings.value("defaultFieldWidth",0).toInt();
    defaultFieldHeight =  settings.value("defaultFieldHeight",0).toInt();
}

void NCRDPreferences::write( QSettings & settings)
{
    settings.setValue("reportdir", reportDir );
    settings.setValue("pdfdir", pdfDir );
    settings.setValue("svgdir", svgDir );
    settings.setValue("outputdir", outputDir );
    settings.setValue("templatedir", templateDir );
    settings.setValue("zintdir", zintDir );

    settings.setValue("defaultfontfamily", fontFamily );
    settings.setValue("defaultfontpointsize", fontPointSize );
    settings.setValue("gridvisible", gridVisible );
    settings.setValue("snaptogrid", snapToGrid );
    settings.setValue("gridX", grid.x() );
    settings.setValue("gridY", grid.y() );
    settings.setValue("showmaximized", showMaximized );
    settings.setValue("filecounter", fileCounter );
    settings.setValue("printerResolution", printerResolution );
    settings.setValue("noPrinterDialog", noPrinterDialog );
    settings.setValue("defaultPrinterName", defaultPrinterName );
    settings.setValue("printerDPI", printerDPI );
    settings.setValue("bpa", encodePwd );
    settings.setValue("htmlOutputStrategy", htmlOutputStrategy );
    settings.setValue("htmlOutputCssFile", htmlOutputCssFile );
    settings.setValue("defaultFieldText",defaultFieldText);
    settings.setValue("defaultFieldWidth",defaultFieldWidth);
    settings.setValue("defaultFieldHeight",defaultFieldHeight);
}


