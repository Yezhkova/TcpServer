#include "session.h"
#include "messenger.h"
#include "log.h"
#include <string>

Session::Session(boost::asio::ip::tcp::socket&& socket, Messenger& messenger)
: m_socket(std::move(socket)), m_messenger(messenger)
{
    boost::system::error_code error;
    boost::asio::ip::tcp::endpoint socket_remote_endpoint = m_socket.remote_endpoint(error);
    if(error){
        LOG( "Endpoint error " << error.message() )
    }
    LOG( "Received connection from " << socket_remote_endpoint )
}

void Session::readAndExecCommand()
{
    //std::cout << "Im reading\n"; // DEBUG
    boost::asio::async_read_until(m_socket, streambuf, '\n', [self = shared_from_this(), this]
                                                          (boost::system::error_code error, std::size_t bytes_transferred)
    {
        //std::string command( (std::istreambuf_iterator<char>(&self->streambuf)), std::istreambuf_iterator<char>() );
        if(error)
        {
            LOG("Closing client: "<<error.message())
            m_messenger.removeUser(m_username);
            return;
        }

        std::string command = readFromBuffer("\n", streambuf, bytes_transferred);
        std::cout << "Command == " << command << "/end of command\n"; // DEBUG

        if(command == "init")
        {
            // read usrname;
            boost::asio::async_read_until(self->m_socket, self->streambuf, '\n', [self, this]
                                                                  (boost::system::error_code error, std::size_t bytes_transferred)
            {
                std::string username = readFromBuffer("\n", self->streambuf, bytes_transferred);
                m_username = username;
                std::cout << "Name = " << username << "/end of name\n"; // DEBUG
                self->m_messenger.addUser(username, self);
                std::cout << "Now " << self->m_messenger.getMapSize() << " users\n"; // DEBUG
                self->streambuf.consume(bytes_transferred);
                //LOG("I have just consumed")
                readAndExecCommand();
            });

        }

        else if(command == "msg")
        {
            // read receiver name;
            std::cout <<"YEAH!\n";
        }
    });
}

std::string Session::readFromBuffer(const std::string delimiter, boost::asio::streambuf& streambuf, size_t bytesTransferred)
{
    std::string s{
        buffers_begin(streambuf.data()),
        buffers_begin(streambuf.data()) + bytesTransferred - delimiter.size()
    };
    streambuf.consume(bytesTransferred);
    return s;
}
