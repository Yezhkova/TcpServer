#include <QCoreApplication>
#include "messenger.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::array<int8_t, 32> key1 = {1,2,3,4,5,6,7,8,9,10};
    Messenger messenger ( "server", key1, 15001 );
    return a.exec();
}
