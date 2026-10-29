#include "ui/forms/mainwindow.hpp"

#include <QApplication>

#include "Core/Core.hpp"

int main(int argc, char *argv[])
{
    auto &core = Core::get();
    core.init();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return QApplication::exec();
}
