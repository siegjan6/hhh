/**************************************************************************
 *
 *	project				 : VisiScript
 *
 *	copyright            : (C) 2014 by Michael Neuroth
 *
 ***************************************************************************/
/*********************************************************************************
 *																				 *
 * This file is part of the VisiScript package (a frontend for script languages) *
 *																				 *
 * Copyright (C) 2014 by Michael Neuroth.										 *
 *                                                                               *
 * This program is free software; you can redistribute it and/or modify			 *
 * it under the terms of the GNU General Public License as published by    		 *
 * the Free Software Foundation; either version 2 of the License, or             *
 * (at your option) any later version.                                           *
 *                                                                               *
 ********************************************************************************/

#ifndef CONFIGURE_H
#define CONFIGURE_H

const int c_iComboBoxHeight = 96;

#define _INIT_OUTPUT_RATIO          30

#define DEMO_JS                     "demo.js"
#define DEMOOBJ_JS                  "demoobj.js"
#define BENCHMARK_JS                "benchmark.js"

#define _QSCRIPT                    "QScript"

#define _MNEUROTH_DE                "mneuroth.de"

#if defined(Q_OS_ANDROID)
#define FONT_NAME                   "Droid sans mono"
#define _WITH_MOBILE_FILEDIALOG
#define _SHOW_LINENUMBERS           1
#define FILES_DIR                   "/data/data/de.mneuroth.visiscript/files/"
#define SDCARD_DIRECTORY            "/"     // "/sdcard"
#elif defined(Q_OS_WIN)
#define FONT_NAME                   "fixed"
#undef _WITH_MOBILE_FILEDIALOG
#define _SHOW_LINENUMBERS           1
#define FILES_DIR                   ".\\files"
#define SDCARD_DIRECTORY            "/"     // "/sdcard"
#else
#define FONT_NAME                   "fixed"
#undef _WITH_MOBILE_FILEDIALOG
#define _SHOW_LINENUMBERS           1
#define FILES_DIR                   "./"
#define SDCARD_DIRECTORY            "/"     // "/sdcard"
#endif

#define _with_thread_search

// remove qsintilla.lib from *.pro file if QScintilla is not used !
#define _WITH_QSCINTILLA

#undef _WITH_LISP

#define LANG_UNDEFINED -1
#define LANG_NONE 		1
#define LANG_MIMDCRIPT	2
#define LANG_CPP		3
#define LANG_CSHARP		4
#define LANG_JAVA		5
#define LANG_JAVASCRIPT	6
#define LANG_D          7
#define LANG_PYTHON		8
#define LANG_RUBY		9
#define LANG_PERL		10
#define LANG_PHP		11
#define LANG_LUA		12
#define LANG_LISP		13
#define LANG_TCL		14
#define LANG_IDL		15
#define LANG_SQL		16
#define LANG_HTML		17
#define LANG_CSS		18
#define LANG_XML        19
#define LANG_TEX		20
#define LANG_VHDL       21
#define LANG_MATLAB		22
#define LANG_OCTAVE		23
#define LANG_MAKEFILE	24
#define LANG_DIFF       25
#define LANG_SCHEME     26
#define LANG_PASCAL     27
#define LANG_FORTRAN    28
#define LANG_HASKELL    29
#define LANG_BATCH		30
//#define LANG_POV
//#define LANG_VERILOG
//#define LANG_AVS
#define LANG_BASH		31
#define LANG_YAML       32
#define LANG_PO         33
#define LANG_PROPERTIES 34
#define LANG_CMAKE      35
#define LANG_SPICE      36
#define LANG_POSTSCRIPT 37
#define LANG_COFFEESCRIPT    38
#define LANG_MAX_ID		LANG_COFFEESCRIPT+1


#if defined(Q_OS_WIN)
#define EOL_MODE        QsciScintilla::EolWindows
#elif defined(Q_OS_LINUX) || defined(Q_OS_ANDROID)
#define EOL_MODE        QsciScintilla::EolUnix
#elif defined(Q_OS_MAC)
#define EOL_MODE        QsciScintilla::EolMac
#else
#define EOL_MODE        QsciScintilla::EolUnix
#endif

#define _USER_EVENT_MSG			 QEvent::User+1
#define _USER_EVENT_DONE		 QEvent::User+2
#define _USER_EVENT_CHECK_RELOAD QEvent::User+3

#endif // CONFIGURE_H
