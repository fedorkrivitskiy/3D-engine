#include "ui/forms/MainWindow.hpp"

#include <QApplication>

#include "Core/Core.hpp"

int main(int argc, char *argv[])
{
    auto &core = Core::get();
    core.init();

    QApplication application(argc, argv);
    MainWindow window;
    window.show();
    return QApplication::exec();
}
