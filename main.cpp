#include <QCoreApplication>
#include "messenger.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Messenger messenger ( 15001 );
    return a.exec();
}
