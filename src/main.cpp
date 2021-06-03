#include <QCoreApplication>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MainWindow* main_window = new MainWindow();

    return a.exec();
}
