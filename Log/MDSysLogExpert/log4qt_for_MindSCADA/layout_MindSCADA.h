/*!

\author dGFuZ3p3

\section 功能

MindSCADA 日志记录布局类，按照 MindSCADA 的要求输出格式化的日志记录；
该类扩展了 log4qt 库的日志布局种类。

\section 线程特性

此类的函数都是线程安全的。

\section 第三方代码

框架代码从 log4qt 库复制，做了小修改。

*/

#ifndef LOG4QT_FOR_MINDSCADA_LAYOUT_H
#define LOG4QT_FOR_MINDSCADA_LAYOUT_H



/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include "../log4qt/layout.h"


/******************************************************************************
 * Declarations
 ******************************************************************************/


namespace Log4Qt
{
	
	/*!
     * \brief The class MindSCADALayout outputs the level and message of a logging
	 *        event.
	 * 
	 * \note The ownership and lifetime of objects of this class are managed. 
	 *       See \ref Ownership "Object ownership" for more details.
	 */
     class MindSCADALayout : public Layout
	{
	    Q_OBJECT
	
	public:
        MindSCADALayout(QObject *pParent = 0);
        // virtual ~NetSCADALayout(); // Use compiler default
	private:
        MindSCADALayout(const MindSCADALayout &rOther); // Not implemented
        MindSCADALayout &operator=(const MindSCADALayout &rOther); // Not implemented
	        
	public:
        //! 按照 MindSCADA 的要求输出格式化的日志记录
        QString format(const LoggingEvent &rEvent) override;
        static QString sourceSeperator();
        //! 解析 rEvent.message 到 source 和 message
        static void parseSourceAndMsg(const LoggingEvent &rEvent, QString& source, QString& message);
	protected:
	
#ifndef QT_NO_DEBUG_STREAM
	    /*!
	     * Writes all object member variables to the given debug stream
	     * \a rDebug and returns the stream.
	     *
	     * <tt>
         * %MindSCADALayout(name:"SL" referencecount:1)
	     * </tt>
	     * \sa QDebug, operator<<(QDebug debug, const LogObject &rLogObject)
	     */
        QDebug debug(QDebug &rDebug) const override;
#endif // QT_NO_DEBUG_STREAM
	};
	
	
	/**************************************************************************
	 * Operators, Helper
	 **************************************************************************/
	
	
	/**************************************************************************
	 * Inline
	 **************************************************************************/
	
    inline MindSCADALayout::MindSCADALayout(QObject *pParent) :
	    Layout(pParent)
	{}
	
	
} // namespace Log4Qt
	
	
// Q_DECLARE_TYPEINFO(Log4Qt::MindSCADALayout, Q_COMPLEX_TYPE); // Use default
	
	
#endif // LOG4QT_FOR_MINDSCADA_LAYOUT_H
