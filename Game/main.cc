#include <engine.hh>
#include <QApplication>


int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(offlinedata);
    Engine w;

    return a.exec();
}
