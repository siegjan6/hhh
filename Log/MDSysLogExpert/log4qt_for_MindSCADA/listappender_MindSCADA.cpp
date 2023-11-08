/******************************************************************************
 *
 * package:     Log4Qt
 * file:        listappender.cpp
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


#include "log4qt_for_MindSCADA/listappender_MindSCADA.h"
#include "../MDSysLoggerImp.h"
#include <QtCore/QDebug>

namespace Log4Qt
{
	    
	
	/**************************************************************************
	 * Declarations
	 **************************************************************************/
	
	
	
	/**************************************************************************
	 * C helper functions
	 **************************************************************************/
	
	
	
	/**************************************************************************
     * Class implementation: MindSCADAListAppender
	 **************************************************************************/
	
	
    MindSCADAListAppender::MindSCADAListAppender(QObject *pParent) :
	    AppenderSkeleton(pParent),
	    mConfiguratorList(false),
	    mList(),
        mMaxCount(2048)
	{

	}
	
	
    MindSCADAListAppender::~MindSCADAListAppender()
	{
        clearList();
	}
	
	
    QList<CMDSysLogData*>* MindSCADAListAppender::list()
	{
	    QMutexLocker locker(&mObjectGuard);
	
        return &mList;
	}
	
    int MindSCADAListAppender::count() const
    {
        QMutexLocker locker(&mObjectGuard);

        return mList.count();
    }

    CMDSysLogData* MindSCADAListAppender::at(int index) const
    {
        QMutexLocker locker(&mObjectGuard);

        return mList.at(index);
    }

    void MindSCADAListAppender::setMaxCount(int n)
	{
	    QMutexLocker locker(&mObjectGuard);
	    
	    if (n < 0)
	    {
	        logger()->warn("Attempt to set maximum count for appender '%1' to %2. Using zero instead", name(), n);
	        n = 0;
	    }
	    mMaxCount = n;
	    ensureMaxCount();
	}
	
	
    void MindSCADAListAppender::clearList()
	{
	    QMutexLocker locker(&mObjectGuard);
	    
        qDeleteAll(mList.begin(), mList.end());
        mList.clear();
	}
	
	
    // bool MindSCADAListAppender::requiresLayout() const;

    void MindSCADAListAppender::append(const LoggingEvent &rEvent)
	{
        // Q_ASSERT_X(, "MindSCADAListAppender::append()", "Lock must be held by caller")

        CMDSysLogData* recycleLog =  NULL;
        if(mList.size() >= mMaxCount)
            recycleLog = mList.takeLast();
        CMDSysLogData* newLog = CMDSysLoggerImp::helper_newLogData(rEvent, recycleLog);
        if(!newLog)
            return;
        mList.prepend(newLog); // 最新的日志被添加到队列头部
    }

#ifndef QT_NO_DEBUG_STREAM
    QDebug MindSCADAListAppender::debug(QDebug &rDebug) const
	{
        rDebug.nospace() << "MindSCADAListAppender("
	        << "name:" << name() << " "
            << "count:" <<  count() << " "
	        << "filter:" << firstFilter() << " "
	        << "isactive:" << isActive() << " "
	        << "isclosed:" << isClosed() << " "
	        << "maxcount:" <<  maxCount() << " "
	        << "referencecount:" << referenceCount() << " "
	        << "threshold:" << threshold().toString()
	        << ")";
	    return rDebug.space();    
	}
#endif // QT_NO_DEBUG_STREAM
	
	
    void MindSCADAListAppender::ensureMaxCount()
	{
        // Q_ASSERT_X(, "MindSCADAListAppender::ensureMaxCount()", "Lock must be held by caller")
	
	    if (mMaxCount <= 0)
	        return;
	    
        while(mList.size() > mMaxCount)
            delete mList.takeLast();
	}

	/**************************************************************************
	 * Implementation: Operators, Helper
	 **************************************************************************/
} // namespace Log4Qt
