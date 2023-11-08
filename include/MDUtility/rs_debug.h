
#ifndef RS_DEBUG_H
#define RS_DEBUG_H

#include "Utility.h"

#include <iosfwd>
#ifdef __hpux
#include <sys/_size_t.h>
#endif

class QString;

/** print out a debug header*/
#define DEBUG_HEADER debugHeader(__FILE__, __func__, __LINE__);
void debugHeader(char const* file, char const* func, int line);
#define RS_DEBUG RS_Debug::instance()
#define RS_DEBUG_VERBOSE DEBUG_HEADER \
	RS_Debug::instance()

/**
 * Debugging facilities.
 *
 * @author Andrew Mustun
 */
class UTILITYSHARED_EXPORT RS_Debug {

public:
    /**
     * Enum for debug levels. Only messages of the current
     * or a higher level are printed.
     * <ul>
     *  <li>D_NOTHING:  nothing
     *  <li>D_CRITICAL: critical messages
     *  <li>D_ERROR:    errors
     *  <li>D_WARNING:  warnings
     *  <li>D_NOTICE:   notes
     *  <li>D_INFORMATIONAL: infos
     *  <li>D_DEBUGGING: very verbose
     * </ul>
     */
    enum RS_DebugLevel { D_NOTHING,
                         D_CRITICAL,
                         D_ERROR,
                         D_WARNING,
                         D_NOTICE,
                         D_INFORMATIONAL,
                         D_DEBUGGING };

private:
    RS_Debug();
	RS_Debug(const RS_Debug&)=delete;
	RS_Debug& operator = (const RS_Debug&)=delete;
	RS_Debug(RS_Debug&&)=delete;
	RS_Debug& operator = (RS_Debug&&)=delete;

public:
    static RS_Debug* instance();

    static void deleteInstance();
    void setLevel(RS_DebugLevel level);
    RS_DebugLevel getLevel();
    void print(RS_DebugLevel level, const char* format ...);
    void print(const char* format ...);
    void printUnicode(const QString& text);
    void timestamp();
    void setStream(FILE* s) {
        stream = s;
    }

private:
    static RS_Debug* uniqueInstance;

    RS_DebugLevel debugLevel;
    FILE* stream;
};

#endif
// EOF
