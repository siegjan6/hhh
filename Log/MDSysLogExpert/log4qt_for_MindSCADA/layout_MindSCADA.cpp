/******************************************************************************
 *
 * package:     Log4Qt
 * file:        simplelayout.cpp
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


#include "layout_MindSCADA.h"

#include <QtCore/QDebug>
#include "../log4qt/loggingevent.h"
#include "../log4qt/helpers/datetime.h"


namespace  Log4Qt
{
	
	
	/**************************************************************************
	 * Declarations
	 **************************************************************************/
	
	
	
	/**************************************************************************
	 * C helper functions
	 **************************************************************************/
	
	
	
	/**************************************************************************
     * Class implementation: MindSCADALayout
	 **************************************************************************/
	
	
    QString MindSCADALayout::format(const LoggingEvent &rEvent)
	{
        QString source;
        QString message;
        parseSourceAndMsg(rEvent, source, message);

        QString separator = sourceSeperator();
        int index = message.indexOf(separator, Qt::CaseInsensitive);
        if(index != -1)
        {
            source = message.left(index);
            message = message.right(message.length() - separator.length() - source.length());
        }

        QString format = "ISO8601";
        QString line = Log4Qt::DateTime::fromMilliSeconds(rEvent.timeStamp()).toString(format);
        line += QLatin1String(",,");
        line += rEvent.level().toString();
        line += QLatin1String(",,");
        line += source;
        line += QLatin1String(",,");
        line += message;
        return line + Layout::endOfLine();
	}
	
    QString MindSCADALayout::sourceSeperator()
    {
        return "::-->";
    }

    void MindSCADALayout::parseSourceAndMsg(const LoggingEvent &rEvent, QString& source, QString& message)
    {
        source = "";
        message = rEvent.message();

        QString separator = sourceSeperator();
        int index = message.indexOf(separator, Qt::CaseInsensitive);
        if(index != -1)
        {
            source = message.left(index);
            message = message.right(message.length() - separator.length() - source.length());
        }
    }

#ifndef QT_NO_DEBUG_STREAM
    QDebug MindSCADALayout::debug(QDebug &rDebug) const
	{
        rDebug.nospace() << "MindSCADALayout("
	        << "name:" << name() << " "
	        << "referencecount:" << referenceCount()
	        << ")";
	    return rDebug.space();    
	}
#endif // QT_NO_DEBUG_STREAM
	
	
	
	/**************************************************************************
	 * Implementation: Operators, Helper
	 **************************************************************************/
	
	
} // namespace Log4Qt
