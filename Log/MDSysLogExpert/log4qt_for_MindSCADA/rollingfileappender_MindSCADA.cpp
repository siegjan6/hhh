/******************************************************************************
 *
 * package:     Log4Qt
 * file:        rollingfileappender.cpp
 * created:     September 2007
 * author:      Martin Heinrich
 *
 * 
 * Copyright 2007 Martin Heinrich
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 ******************************************************************************/



/******************************************************************************
 * Dependencies
 ******************************************************************************/


#include "log4qt_for_MindSCADA/rollingfileappender_MindSCADA.h"

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QTextCodec>
#include "log4qt/helpers/optionconverter.h"
#include "log4qt/layout.h"
#include "log4qt/loggingevent.h"



namespace Log4Qt
{
	    
	
	/************************************************************************
	Declarations
	*************************************************************************/
	
	
	
	/************************************************************************
	C helper functions
	*************************************************************************/
	
	
	
	/************************************************************************
    Class implementation: MindSCADARollingFileAppender
	*************************************************************************/
	
	
    MindSCADARollingFileAppender::MindSCADARollingFileAppender(QObject *pParent) :
        RollingFileAppender(pParent),
        m_fileExt("txt")
	{
        setMaxBackupIndex(3);
        setMaximumFileSize(1*1024*1024);
	}
	
	
    MindSCADARollingFileAppender::MindSCADARollingFileAppender(Layout *pLayout,
	                                         const QString &rFileName,
	                                         QObject *pParent) :
        RollingFileAppender(pLayout, rFileName, pParent),
        m_fileExt("txt")
	{
        setMaxBackupIndex(3);
        setMaximumFileSize(1*1024*1024);
	}
	
	
    MindSCADARollingFileAppender::MindSCADARollingFileAppender(Layout *pLayout,
	                                         const QString &rFileName,
	                                         bool append,
	                                         QObject *pParent) :
        RollingFileAppender(pLayout, rFileName, append, pParent),
        m_fileExt("txt")
	{
        setMaxBackupIndex(3);
        setMaximumFileSize(1*1024*1024);
	}
	
	
    MindSCADARollingFileAppender::~MindSCADARollingFileAppender()
	{
	    close();
    }

    void MindSCADARollingFileAppender::setFileExt(const QString &fileExt)
    {
        m_fileExt = fileExt;
    }

    void MindSCADARollingFileAppender::append(const LoggingEvent &rEvent)
	{
        // Q_ASSERT_X(, "MindSCADARollingFileAppender::append()", "Lock must be held by caller")
	
	    FileAppender::append(rEvent);
        if (writer()->device()->size() > maximumFileSize())
            rollOver_MindSCADA();
	}
	
	
    void MindSCADARollingFileAppender::rollOver_MindSCADA()
	{
        // Q_ASSERT_X(, "MindSCADARollingFileAppender::rollOver_MindSCADA()", "Lock must be held by caller")
	    
        int maxBI = maxBackupIndex();
        logger()->debug("Rolling over with maxBackupIndex = %1", maxBI);
	
	    closeFile();
	    
	    QFile f;
        f.setFileName(buildFileName(maxBI));
	    if (f.exists() && !removeFile(f))
	    	return;
	    
	    QString target_file_name;
	    int i;
        for (i = maxBI - 1; i >=1; i--)
	    {
            f.setFileName(buildFileName(i));
	        if (f.exists())
	        {
                target_file_name = buildFileName(i + 1);
	            if (!renameFile(f, target_file_name))
	                return;
	        }
	    }
	
        f.setFileName(file());
        target_file_name = buildFileName(1);
	    if (!renameFile(f, target_file_name))
	        return;
	    
	    openFile();
    }

    QString MindSCADARollingFileAppender::fileNoExt()
    {
        QString rawFileName = file();
        int iIndex = rawFileName.lastIndexOf('.');
        if(iIndex != -1)
        {
            return rawFileName.left(iIndex);
        }
        else return rawFileName;
    }

    QString MindSCADARollingFileAppender::buildFileName(int iNumber)
    {
       QString basicFile = fileNoExt();
        QString target_file_name = basicFile + QLatin1Char('_') + QString::number(iNumber) + QLatin1String(".") + m_fileExt;
        return target_file_name;
    }

	/**************************************************************************
	 * Implementation: Operators, Helper
	 **************************************************************************/
	
	
} // namespace Log4Qt
