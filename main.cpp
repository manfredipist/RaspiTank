#include <QApplication>
#include "PiController/picontroller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    PiController piController;
    piController.show();

    return a.exec();
}
