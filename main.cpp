#include <QApplication>

#include "overlay.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Overlay overlay;
    overlay.show();
    return app.exec();
}
