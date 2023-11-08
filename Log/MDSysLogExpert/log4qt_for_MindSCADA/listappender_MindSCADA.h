/*!

\author dGFuZ3p3

\section 功能 - MindSCADA 内存链表追加器，将日志记录输出到内存链表，具体功能如下：

、保留最近的指定数量的日志记录，参考 setMaxCount；
、管理超出链表最大长度的日志记录，将这些记录转交给垃圾处理器，
    参考 mDeleteList 及相关函数；
、该类扩展了 log4qt 库的追加器种类。

\section 线程特性

此类的函数都是线程安全的。

\section 第三方代码

框架代码从 log4qt 库复制，做了小修改。

*/

#ifndef LOG4QT_LISTAPPENDER_MINDSCADA_LAYOUT_H
#define LOG4QT_LISTAPPENDER_MINDSCADA_LAYOUT_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include "log4qt/appenderskeleton.h"

#include <QtCore/QList>
#include <QtCore/QLinkedList>
#include <QtCore/QMutex>
#include "log4qt/loggingevent.h"

#include "../../../include/MDSysLogExpert/MDSysLogData.h"

/******************************************************************************
 * Declarations
 ******************************************************************************/

namespace Log4Qt
{
	/*!
     * \brief The class MindSCADAListAppender appends logging events to a list for later
	 *        processing.
	 * 
	 * \note All the functions declared in this class are thread-safe.
	 * 
	 * \note The ownership and lifetime of objects of this class are managed. 
	 *       See \ref Ownership "Object ownership" for more details.
	 */
    class MindSCADAListAppender : public AppenderSkeleton
	{
	    Q_OBJECT
	    
	    /*!
	     * The property holds, if the Appender is used by a configurator.
	     *
	     * The default value is false for not being a configurator list.
	     *
	     * \sa configuratorList(), setConfiguratorList()
	     */
	    Q_PROPERTY(bool configuratorList READ configuratorList WRITE setConfiguratorList)
	    
	    /*!
	     * The property holds the maximum count used by the appender.
	     *
	     * The default maximum count is -1 for unlimited.
	     *
	     * \sa maxCount(), setMaxCount()
	     */
	    Q_PROPERTY(int maxCount READ maxCount WRITE setMaxCount)
	    
	public:
        MindSCADAListAppender(QObject *pParent = 0);
        virtual ~MindSCADAListAppender();
	private:
        MindSCADAListAppender(const MindSCADAListAppender &rOther); // Not implemented
        MindSCADAListAppender &operator=(const MindSCADAListAppender &rOther); // Not implemented
	        
	public:
		/*!
		 * Returns true, if the appender is used by a configurator. Otherweise it returns 
		 * false.
		 * 
		 * \sa setConfiguratorList()
		 */
		bool configuratorList() const;
		
        QList<CMDSysLogData*>* list();
        int count() const;
        CMDSysLogData* at(int index) const;
	    int maxCount() const;
	    
		/*!
		 * Sets that the appender is used by a configurator. If set to true, the appender
		 * will not be removed from a Logger when Logger::removeAllAppenders()is called.
		 * This way the appender can collect events raised during the configuration process.
		 * 
		 * \sa configuratorList(), BasicConfigurator, PropertyConfigurator, 
		 *     ConfiguratorHelper::configureError()
		 */
	    void setConfiguratorList(bool isConfiguratorList);
	    
	    void setMaxCount(int n);
	    
        void clearList();
	    virtual bool requiresLayout() const;
	    
	protected:
	    virtual void append(const LoggingEvent &rEvent);
	    
#ifndef QT_NO_DEBUG_STREAM
	    /*!
	     * Writes all object member variables to the given debug stream 
	     * \a rDebug and returns the stream.
	     *
	     * <tt>
         * %MindSCADAListAppender(name:"LA" count:1 filter:0x41fa488 isactive:true
	     *               isclosed:false maxcount:170 referencecount:1 
	     *               threshold:"TRACE_SET")  
	     * </tt>
	     * \sa QDebug, operator<<(QDebug debug, const LogObject &rLogObject)
	     */
	    virtual QDebug debug(QDebug &rDebug) const;
#endif // QT_NO_DEBUG_STREAM
	
	    /*!
	     * Ensures that the count of events is less or equal then the maxium 
	     * count. If the list contains too many items, items are deleted from 
	     * the begin of the list.
	     */
	    void ensureMaxCount();
    private:
        volatile int mMaxCount;
	private:
		volatile bool mConfiguratorList; 
        QList<CMDSysLogData*> mList;
	};
	

	/**************************************************************************
	 * Operators, Helper
	 **************************************************************************/
	
	
	/**************************************************************************
	 * Inline
	 **************************************************************************/
	
    inline bool MindSCADAListAppender::configuratorList() const
	{   // QMutexLocker locker(&mObjectGuard); // Read/Write of int is safe
		return mConfiguratorList;   }

    inline int MindSCADAListAppender::maxCount() const
	{   return mMaxCount;    }
	
    inline void MindSCADAListAppender::setConfiguratorList(bool isConfiguratorList)
	{   // QMutexLocker locker(&mObjectGuard); // Read/Write of int is safe
		mConfiguratorList = isConfiguratorList;   }

    inline bool MindSCADAListAppender::requiresLayout() const
	{   return false;    }
	
	
} // namespace Log4Qt


// Q_DECLARE_TYPEINFO(Log4Qt::MindSCADAListAppender, Q_COMPLEX_TYPE); // Use default


#endif // LOG4QT_LISTAPPENDER_MINDSCADA_LAYOUT_H
