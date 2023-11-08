#include <QtTest>
#include "FormPreviewTest.h"
#include "PreviewDialog.h"
#include "HMIFramework.h"
//////////////////////////////////////////////////////////////////////////
FormPreviewTest::FormPreviewTest()
{
    m_test = NULL;
    m_framework = NULL;
    //需要确认这个路径下有文件
    m_fullName = "/home/aGRw/project/HMITest/HMI/temp.gdi";
}
void FormPreviewTest::initTestCase()
{
    m_test = new CPreviewDialog;
    m_framework = new CHMIFramework;
    m_framework->setIsStudioMode(true);
    m_test->init(m_framework);
}
void FormPreviewTest::cleanupTestCase()
{
    delete m_test;
    delete m_framework;
}
//////////////////////////////////////////////////////////////////////////
void FormPreviewTest::loadFormCase()
{
    m_test->loadForm(m_fullName);
    QVERIFY(m_test->objectWidget()->currentObj());
}
void FormPreviewTest::mouseCase()
{
    CFormObjectContainer* container = m_test->objectWidget();
    CFormObject* form = container->currentObj();
    if(form == NULL)
        return;
    form->setRect(QRect(0, 0, 300, 200));

    QTest::mousePress(container, Qt::LeftButton, NULL, QPoint(100, 100));
    QMouseEvent e0(QEvent::MouseMove, QPoint(120, 130), Qt::LeftButton,Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(container,&e0);
    //无效，原因不明
    //QTest::mouseMove(container, QPoint(120, 130));
    QTest::mouseRelease(container, Qt::LeftButton, NULL, QPoint(120, 130));
    QVERIFY2(form->rect() == QRect(20, 30, 300, 200), "move");

    QTest::mousePress(container, Qt::LeftButton, NULL, QPoint(320, 100));
    QMouseEvent e1(QEvent::MouseMove, QPoint(340, 100), Qt::LeftButton,Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(container,&e1);
    QTest::mouseRelease(container, Qt::LeftButton, NULL, QPoint(340, 100));
    QVERIFY2(form->rect() == QRect(20, 30, 320, 200), "width");

    QTest::mousePress(container, Qt::LeftButton, NULL, QPoint(100, 230));
    QMouseEvent e2(QEvent::MouseMove, QPoint(100, 250), Qt::LeftButton,Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(container,&e2);
    QTest::mouseRelease(container, Qt::LeftButton, NULL, QPoint(100, 250));
    QVERIFY2(form->rect() == QRect(20, 30, 320, 220), "height");
}
void FormPreviewTest::keyCase()
{
    CFormObjectContainer* container = m_test->objectWidget();
    CFormObject* form = container->currentObj();
    if(form == NULL)
        return;
    form->setRect(QRect(0, 0, 300, 200));

    QTest::keyClick(container, Qt::Key_Right);
    QVERIFY2(form->rect() == QRect(1, 0, 300, 200), "left");
    QTest::keyClick(container, Qt::Key_Down);
    QVERIFY2(form->rect() == QRect(1, 1, 300, 200), "top");
    QTest::keyClick(container, Qt::Key_Right, Qt::ShiftModifier);
    QVERIFY2(form->rect() == QRect(1, 1, 301, 200), "width");
    QTest::keyClick(container, Qt::Key_Down, Qt::ShiftModifier);
    QVERIFY2(form->rect() == QRect(1, 1, 301, 201), "height");
}
void FormPreviewTest::propertyCase()
{
    CFormObjectContainer* container = m_test->objectWidget();
    CFormObject* form = container->currentObj();
    CPreviewPropertyWidget* property = m_test->property();
    if(form == NULL)
        return;

    int left = property->lineEditLeft()->text().toInt();
    int top = property->lineEditTop()->text().toInt();
    int width = property->lineEditWidth()->text().toInt();
    int height = property->lineEditHeight()->text().toInt();
    QVERIFY2(QRect(1, 1, 301, 201) == QRect(left, top, width, height), "read value");

    QTest::keyClick(property->lineEditWidth(), Qt::Key_A, Qt::ControlModifier);
    QTest::keyClicks(property->lineEditWidth(), "400");
    QTest::keyClick(property->lineEditWidth(), Qt::Key_Enter);
    QVERIFY2(form->rect() == QRect(1, 1, 400, 201), "write value");
}
void FormPreviewTest::saveFormCase()
{
    CFormObjectContainer* container = m_test->objectWidget();
    CFormObject* form = container->currentObj();
    if(form == NULL)
        return;
    form->setRect(QRect(0, 0, 300, 400));
    m_test->saveForm(form);

    container->setCurrentObj(NULL);
    container->list()->removeOne(form);
    delete form;

    m_test->loadForm(m_fullName);
    QCOMPARE(container->currentObj()->rect(), QRect(0, 0, 300, 400));
}
//////////////////////////////////////////////////////////////////////////
