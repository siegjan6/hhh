#include <QCoreApplication>
#include <thread>
#include <QThread>
std::thread t1;

void Test();
static void thread_task() {
    QThread::sleep(2);
    Test();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::thread t1(thread_task);
    return a.exec();
}
