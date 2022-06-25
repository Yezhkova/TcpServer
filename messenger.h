#ifndef MESSENGER_H
#define MESSENGER_H

#pragma once
#include <string>
#include <iostream>
#include <optional>
#include <boost/asio.hpp>
#include "log.h"
#include <map>

class Session;

class Messenger
{
public:
    Messenger( std::uint16_t );
    void addUser(std::string, std::weak_ptr<Session> );
    void removeUser(std::string username);
    int getMapSize()
    {
        return m_userMap.size();
    }
    void writeToAll(std::string sender, std::string textOfMessage);
    bool contains(std::string username)
    {
        return  m_userMap.count(username);
    }
    void waitServerEnd()
    {
        if(m_mainThread.joinable())
         m_mainThread.join();
    }

private:
    void async_accept();
    void formList(); 
    void sendCommandToAll(std::string message);

private:
    boost::asio::io_context                         m_io_context;
    boost::asio::ip::tcp::acceptor                  m_acceptor;
    std::optional<boost::asio::ip::tcp::socket>     m_socket;
    std::map<std::string, std::shared_ptr<Session>> m_userMap;
    std::thread                                     m_mainThread;

};

#endif // MESSENGER_H
