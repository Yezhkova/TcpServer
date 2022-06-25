#include "messenger.h"

int main(int argc, char *argv[])
{
    Messenger messenger ( 15001 );
    std::cin.ignore( std::numeric_limits< std::streamsize >::max( ), '\n' );
}
