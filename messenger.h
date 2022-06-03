#ifndef MESSENGER_H
#define MESSENGER_H

#pragma once

#include <string>
#include <array>
#include <iostream>
#include <optional>
#include <boost/asio.hpp>
#include "log.h"
#include <map>

using Key = std::array<int8_t, 32>;

class Session;

class Messenger
{
public:
    Messenger( std::string, Key, std::uint16_t );
    void sendMessage( Key, std::string );
    bool sendMessage( boost::asio::ip::tcp::endpoint, std::string );
    void addUser(std::string, std::weak_ptr<Session> );
    void removeUser(std::string username);
    int getMapSize()
    {
        return m_userMap.size();
    }
    //void makeList(boost::asio::ip::tcp::socket socket, std::string s);

private:
    void async_accept();
    void formList();

private:
    std::string                                     m_userName;
    Key                                             m_shaKey;
    boost::asio::io_context                         m_io_context;
    boost::asio::ip::tcp::acceptor                  m_acceptor;
    std::optional<boost::asio::ip::tcp::socket>     m_socket;
    std::map<std::string, std::shared_ptr<Session>> m_userMap;

};

#endif // MESSENGER_H
