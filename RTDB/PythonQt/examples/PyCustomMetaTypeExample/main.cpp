/*
 *
 *  Copyright (C) 2010 MeVis Medical Solutions AG All Rights Reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  Further, this software is distributed without any warranty that it is
 *  free of the rightful claim of any third person regarding infringement
 *  or the like.  Any license provided herein, whether implied or
 *  otherwise, applies only to this software file.  Patent licenses, if
 *  any, provided herein do not apply to combinations of this program with
 *  other software, or any other product whatsoever.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Contact information: MeVis Medical Solutions AG, Universitaetsallee 29,
 *  28359 Bremen, Germany or:
 *
 *  http://www.mevis.de
 *
 */

//----------------------------------------------------------------------------------
/*!
// \file    PyGuiExample.cpp
// \author  Florian Link
// \author  Last changed by $Author: florian $
// \date    2007-04
*/
//----------------------------------------------------------------------------------

#include "PythonQt.h"
#include "gui/PythonQtScriptingConsole.h"
#include "CustomObject.h"

#include <QApplication>

int main( int argc, char **argv )
{
  QApplication qapp(argc, argv);

  PythonQt::init(PythonQt::IgnoreSiteModule | PythonQt::RedirectStdOut);

  PythonQtObjectPtr  mainContext = PythonQt::self()->getMainModule();
  //PythonQtScriptingConsole console(NULL, mainContext);

  // register the type with QMetaType
  qRegisterMetaType<CustomObject>("CustomObject");
  // add a wrapper object for the new variant type
  PythonQt::self()->registerCPPClass("CustomObject","","example", PythonQtCreateObject<CustomObjectWrapper>);


//  mainContext.evalScript("import sys");
//  mainContext.evalScript("sys.path.append('C:\Python27\Lib\site-packages\redis_py_cluster-1.1.0-py2.7.egg')");
//  mainContext.evalScript("sys.path.append('C:\Python27\Lib\site-packages')");
 // mainContext.evalScript("sys.path.append('C:\Python27\Lib\redis-2.10.5-py2.7.egg')");
 // mainContext.evalScript("from rediscluster import StrictRedisCluster");
  //mainContext.evalScript("sys.setdefaultencoding('utf-8')");
  mainContext.evalFile(":example.py");
  Q_ASSERT(!mainContext.isNull());
  mainContext.evalScript("startup_nodes = [{'host': '192.167.8.13', 'port': '6379'}]");
  PythonQtObjectPtr rcc = mainContext.evalScript("MyRedisClient(host='192.167.8.13',port=6379,decode_responses=True)\n",Py_eval_input);
//  PythonQtObjectPtr rcc = mainContext.evalScript("MyRedisClient(host='192.168.10.99',port=7000)\n",Py_eval_input);
  if(!rcc.isNull())
  {
      //QVariant v = rc.call("get", QVariantList());
      rcc.call("onInit",QVariantList()<< "192.167.8.13" << 6379);
      QVariant v = rcc.call("get", QVariantList() << "a" << "b");
      int m = 9;
  }

//  console.appendCommandPrompt();
 // console.show();

  return qapp.exec();
}

