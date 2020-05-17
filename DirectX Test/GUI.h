#pragma once

#include <QtWidgets/QApplication>

class GUI : public QObject
{
    Q_OBJECT

public:
    GUI(int** table);
    int run(QApplication* application);

private:
    QApplication* application;
    int* table;
};