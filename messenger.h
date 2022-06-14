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
    void addUser(std::string, std::weak_ptr<Session> );
    void removeUser(std::string username);
    int getMapSize()
    {
        return m_userMap.size();
    }
    void writeToAll(std::string sender, std::string textOfMessage, std::weak_ptr<Session> session);
    bool contains(std::string username)
    {
        return  m_userMap.count(username);
    }

private:
    void async_accept();
    void formList(); 
    void sendCommandToAll(std::string message);

private:
    std::string                                     m_userName;
    Key                                             m_shaKey;
    boost::asio::io_context                         m_io_context;
    boost::asio::ip::tcp::acceptor                  m_acceptor;
    std::optional<boost::asio::ip::tcp::socket>     m_socket;
    std::map<std::string, std::shared_ptr<Session>> m_userMap;

};

#endif // MESSENGER_H
