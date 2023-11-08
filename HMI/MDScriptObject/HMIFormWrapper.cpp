#include <QApplication>
#include "HMIFormWrapper.h"
#include "MDHMIForm.h"
#include "Run.h"

/*event
onLoad()
onUnload()
onMouseUp(x, y)
onMouseDown(x, y)
onMouseMove(x, y)
onEnter()
onLeave()
onGetFocus()
onLostFocus()
onShow()
onHide()
*/

//////////////////////////////////////////////////////////////////////////
CHMIFormWrapper::CHMIFormWrapper(void *object, const QString &name)
    :CMDObjectWrapper(object, name)
{
}
//////////////////////////////////////////////////////////////////////////
void CHMIFormWrapper::setLeft(int value)
{
    QRect r = object()->rect();
    r.moveLeft(value);
    object()->setRect(r);
}
int CHMIFormWrapper::left()
{
    return object()->rect().left();
}
void CHMIFormWrapper::setTop(int value)
{
    QRect r = object()->rect();
    r.moveTop(value);
    object()->setRect(r);
}
int CHMIFormWrapper::top()
{
    return object()->rect().top();
}
void CHMIFormWrapper::setWidth(int value)
{
    QRect r = object()->rect();
    r.moveTop(value);
    object()->setRect(r);
}
int CHMIFormWrapper::width()
{
    return object()->rect().width();
}
void CHMIFormWrapper::setHeight(int value)
{
    QRect r = object()->rect();
    r.moveTop(value);
    object()->setRect(r);
}
int CHMIFormWrapper::height()
{
    return object()->rect().width();
}
void CHMIFormWrapper::setVisible(bool value)
{
    if (value)
        object()->show();
    else
        dynamic_cast<QWidget*>(object()->parent())->hide();
}
bool CHMIFormWrapper::visible()
{
    return object()->isVisible();
}
void CHMIFormWrapper::setLayerVisible(int layer, bool value)
{
    if(layer < 0 || layer > 9)
        return;

    CEnvironment* e = dynamic_cast<CEnvironment*>(object()->common());
    (*(e->visibleLayers()))[layer] = value;
    e->updateVisible();
}
bool CHMIFormWrapper::layerVisible(int layer)
{
    if(layer < 0 || layer > 9)
        return false;

    return dynamic_cast<CEnvironment*>(object()->common())->visibleLayers()->at(layer);
}
void CHMIFormWrapper::setLayerLocked(int layer, bool value)
{
    if(layer < 0 || layer > 9)
        return;

    CEnvironment* e = dynamic_cast<CEnvironment*>(object()->common());
    (*(e->lockedLayers()))[layer] = value;
    e->updateLocked();
}
bool CHMIFormWrapper::layerLocked(int layer)
{
    if(layer < 0 || layer > 9)
        return false;

    return dynamic_cast<CEnvironment*>(object()->common())->lockedLayers()->at(layer);
}
//////////////////////////////////////////////////////////////////////////
void CHMIFormWrapper::close()
{
    QApplication::postEvent(object()->parent(), new QCloseEvent);
    //object()->closeForm();
}
void CHMIFormWrapper::hide()
{
    object()->hide();
    object()->hideForm();
    setVisible(false);
}
void CHMIFormWrapper::move(int x, int y)
{
    object()->parentWidget()->move(x, y);
}
void CHMIFormWrapper::print(bool showPrintDialog)
{
    object()->print(showPrintDialog);
}
void CHMIFormWrapper::printPreview()
{
    object()->printPreview();
}
void CHMIFormWrapper::printRegion(int x, int y, int w, int h, bool showPrintDialog, bool isFullPage)
{
    QRect r(x, y, w, h);
    r = r.normalized();

    object()->printRegion(r, showPrintDialog, isFullPage);
}
void CHMIFormWrapper::printObject(const QString &name, bool showPrintDialog, bool isFullPage)
{
    object()->printObject(name, showPrintDialog, isFullPage);
}
bool CHMIFormWrapper::createObject(const QString &origName, const QString &newName)
{
    QString orig = origName.trimmed();
    QString n = newName.trimmed();

    return dynamic_cast<CRun*>(object()->run())->createObject(orig, n);
}
//////////////////////////////////////////////////////////////////////////
CMDHMIForm *CHMIFormWrapper::object()
{
    return reinterpret_cast<CMDHMIForm*>(MDObject());
}

