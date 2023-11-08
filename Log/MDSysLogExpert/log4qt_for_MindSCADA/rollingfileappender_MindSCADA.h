/*!

\author dGFuZ3p3

\section 功能 - MindSCADA 滚动文件追加器，将日志记录输出到指定的文件，并实现滚动记录，具体功能如下：

、指定主文件名；
、主文件大小超出最大值，实现滚动记录，参考 rollOver_MindSCADA() 及相关函数；
、该类扩展了 log4qt 库的追加器种类。

\section 线程特性

此类的函数都是线程安全的。

\section 第三方代码

框架代码从 log4qt 库复制，做了小修改。

*/

#ifndef LOG4QT_ROLINGFILEAPPENDER_FOR_MINDSCADA_H
#define LOG4QT_ROLINGFILEAPPENDER_FOR_MINDSCADA_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include "log4qt/rollingfileappender.h"


/******************************************************************************
 * Declarations
 ******************************************************************************/

namespace Log4Qt
{
	
	/*!
	 * \brief The class RollingFileAppender extends FileAppender to backup 
	 *        the log files when they reach a certain size.
	 * 
	 * \note All the functions declared in this class are thread-safe.
	 *    
	 * \note The ownership and lifetime of objects of this class are managed. 
	 *       See \ref Ownership "Object ownership" for more details.
	 */
    class MindSCADARollingFileAppender : public RollingFileAppender
	{
	    Q_OBJECT
	
	    /*!
	     * The property holds the maximum backup count used by the appender.
	     *
	     * The default is 1.
	     *
	     * \sa maxBackupIndex(), setMaxBackupIndex()
	     */
	    Q_PROPERTY(int maxBackupIndex READ maxBackupIndex WRITE setMaxBackupIndex)
	    
        /*!
         * The property holds the maximum file size used by the appender.
         *
         * The default is 10 MB (10 * 1024 * 1024).
         *
         * \sa maximumFileSize(), setMaximumFileSize()
         */
        Q_PROPERTY(qint64 maximumFileSize READ maximumFileSize WRITE setMaximumFileSize)
        
        /*!
         * The property sets the maximum file size from a string value.
         *
         * \sa setMaxFileSize(), maximumFileSize()
         */
        Q_PROPERTY(QString maxFileSize WRITE setMaxFileSize)
        
	public:
        MindSCADARollingFileAppender(QObject *pParent = 0);
        MindSCADARollingFileAppender(Layout *pLayout,
	                        const QString &rFileName,
	                        QObject *pParent = 0);
        MindSCADARollingFileAppender(Layout *pLayout,
	                        const QString &rFileName,
	                        bool append,
	                        QObject *pParent = 0);
        virtual ~MindSCADARollingFileAppender();
	private:
        MindSCADARollingFileAppender(const MindSCADARollingFileAppender &rOther); // Not implemented
        MindSCADARollingFileAppender &operator=(const MindSCADARollingFileAppender &rOther); // Not implemented
    public:
        void setFileExt(const QString &fileExt);
	protected:
	    virtual void append(const LoggingEvent &rEvent);
	private:
        void rollOver_MindSCADA();
        QString fileNoExt();
        QString buildFileName(int iNumber);
    private:
        QString m_fileExt;
	};
	
	
	/**************************************************************************
	 * Operators, Helper
	 **************************************************************************/
	
	
	/**************************************************************************
	 * Inline
	 **************************************************************************/

} // namespace Log4Qt


// Q_DECLARE_TYPEINFO(Log4Qt::MindSCADARollingFileAppender, Q_COMPLEX_TYPE); // Use default


#endif // LOG4QT_ROLINGFILEAPPENDER_FOR_MINDSCADA_H
