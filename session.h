#ifndef SESSION_H
#define SESSION_H


#pragma once

#include <iostream>
#include <optional>
#include <boost/asio.hpp>
#include <memory>

class Messenger;

class Session : public std::enable_shared_from_this<Session>
{
public:

    Session(boost::asio::ip::tcp::socket&&, Messenger&);

    ~Session()
    {
        std::cout << "Session killed\n";
    };

    void readAndExecCommand();

    boost::asio::ip::tcp::socket& getSocket()
    {
        return m_socket;
    }

private:

    std::string readFromBuffer      (
                                        boost::asio::streambuf& streambuf,
                                        size_t bytesTransferred
                                    );


private:

    boost::asio::ip::tcp::socket    m_socket;
    std::string                     m_username;
    boost::asio::streambuf          streambuf;
    Messenger&                      m_messenger;

};
#endif // SESSION_H
