#include "messenger.h"
#include "session.h"

// server part

Messenger::Messenger( std::string userName, Key shaKey, std::uint16_t port )
        :
        m_userName( userName ),
        m_shaKey( shaKey ),
        m_io_context( ),
        m_acceptor( m_io_context, boost::asio::ip::tcp::endpoint( boost::asio::ip::tcp::v4( ), port ))
{
    Log( ).info( "Messenger " + userName + " created" );
    async_accept( ); // Doesn't block
    Log( ).info( "Messenger server waiting for connections" );
    std::thread ServerThread( [ & ]
                              {
                                  m_io_context.run( );
                              } );
    ServerThread.detach( );
}

void Messenger::addUser(std::string usernameToAdd, std::weak_ptr<Session> session) {
//    if(m_userMap.find(username) != m_userMap.end()) {
//        // window
//        return 0;
//    }
    m_userMap[usernameToAdd] = session.lock();
    formList();
}

void Messenger::removeUser(std::string usernameToRemove)
{
    m_userMap.erase(usernameToRemove);
    LOG(usernameToRemove << " erased. map size now = " << m_userMap.size()) // DEBUG
    formList();
}

void Messenger::formList()
{
    std::string message = "list\n";
    for(auto & [username, session] : m_userMap)
    {
        message += username + ';';
    }
    message += '\n';
    sendCommandToAll(message);
}

void Messenger::writeToAll(std::string sender, std::string textOfMessage, std::weak_ptr<Session> session) // maybe shared
{
    std::string message = "msg\n"+sender+"\n"+textOfMessage+char(EOF);
    sendCommandToAll(message);
}


void Messenger::sendCommandToAll(std::string message)
{
    for(auto & [username, session] : m_userMap)
    {
        boost::asio::async_write( session->getSocket(), boost::asio::buffer( message ),
                                  []( boost::system::error_code error, std::size_t bytes_transferred ) // passed a & to access s
        {
            if(error)
            {
                LOG( "Async_write error " << error.message( ));
            }
            else
            {
                std::cout << "bytes transferred: "<< bytes_transferred << " bytes\n"; // DEBUG

            }
        } );
    }
}

void Messenger::async_accept( )
{
    m_socket.emplace( m_io_context );
    m_acceptor.async_accept( *m_socket, [ this ]( boost::system::error_code error )
    {
        boost::asio::socket_base::keep_alive option(true);
        m_socket->set_option(option);
        std::make_shared<Session>( std::move( *m_socket ), *this)->readAndExecCommand( );
        async_accept( );
    } );
};
