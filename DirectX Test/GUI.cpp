#include "GUI.h"

GUI::GUI(int** table)
{
    application = new QApplication{ argc, argv };
    application->setApplicationDisplayName("Statue Hall Inventory");
    QDebug() << "Hello from Qt";
}

int GUI::run(QApplication* application)
{
    application->show();
    return application->run();
}
