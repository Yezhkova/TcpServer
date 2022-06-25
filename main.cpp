#include "messenger.h"

int main()
{
    Messenger messenger ( 15001 );
    messenger.waitServerEnd();
}
