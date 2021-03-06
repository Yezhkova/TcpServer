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
    boost::asio::async_read_until(m_socket, streambuf, '\n', [self = shared_from_this(), this]
                                                          (boost::system::error_code error, std::size_t bytes_transferred)
    {
        if(error)
        {
            LOG("Closing client: " << error.message())
            m_messenger.removeUser(m_username);
            return;
        }

        std::string command = readFromBuffer(streambuf, bytes_transferred);
        LOG("Command == " << command << "/end of command") // DEBUG

        if (command == "init")
        {
            // read usrname;
            boost::asio::async_read_until(self->m_socket, self->streambuf, '\n', [self, this]
                                                                  (boost::system::error_code error, std::size_t bytes_transferred)
            {
                if(error)
                {
                    LOG("Closing client: " << error.message())
                    m_messenger.removeUser(m_username);
                    return;
                }
                std::string username = readFromBuffer(self->streambuf, bytes_transferred);
                if(!m_messenger.contains(username))
                {
                    m_username = username;
                    LOG("Name = " << username << "/end of name") // DEBUG
                    self->m_messenger.addUser(username, self);
                    LOG("Now " << self->m_messenger.getMapSize() << " users") // DEBUG
                    self->streambuf.consume(bytes_transferred);
                    readAndExecCommand();
                }
                else
                {
                    m_socket.close();
                    self->streambuf.consume(bytes_transferred);
                }
                //self->streambuf.consume(bytes_transferred);
            });

        }

        else if (command == "msg")
        {
            boost::asio::async_read_until(self->m_socket, self->streambuf, EOF, [self, this]
                                                                  (boost::system::error_code error, std::size_t bytes_transferred)
            {
                if(error)
                {
                    LOG("Closing client: " << error.message())
                    m_messenger.removeUser(m_username);
                    return;
                }
                std::string textOfMessage= readFromBuffer(self->streambuf, bytes_transferred);
                LOG(m_username << " says " << textOfMessage << '\n')
                self->m_messenger.writeToAll(m_username, textOfMessage);
                self->streambuf.consume(bytes_transferred);
                readAndExecCommand();
            });
        }
    });
}

std::string Session::readFromBuffer(boost::asio::streambuf& streambuf, size_t bytesTransferred)
{
    std::string s{
        buffers_begin(streambuf.data()),
        buffers_begin(streambuf.data()) + bytesTransferred - 1
    };
    streambuf.consume(bytesTransferred);
    return s;
}
