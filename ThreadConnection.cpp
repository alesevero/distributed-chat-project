//
//  ThreadConnection.cpp
//  Trabalho2
//
//  Created by Alexandre Severo Jr on 6/8/16.
//  Copyright © 2016 asj. All rights reserved.
//

#include "ThreadConnection.h"

void * CThreadConnection::ThreadInit(void *i)
{
    ((CThreadConnection *) i)->RunThread();
    return NULL;
}

void CThreadConnection::RunThread()
{
    if (m_nConnection > 0)
    {
        SMessage msg;
        bool loop = false;
        char m[300];
        bzero(m, 301);
        while(!loop)
        {
            if (read(m_nConnection, m, 300) >= 0)
            {
                std::string strMsg(m);
                msg = ParseMessage(strMsg);
                pthread_mutex_lock(&CMessagePool::m_ptmInPoolMutex);
                m_MessagePool.InsertInMessage(msg);
                pthread_mutex_unlock(&CMessagePool::m_ptmInPoolMutex);
            }
            else
            {
                close(m_nConnection);
                break;
            }
        }
    }

    // Finalize thread on the server
    FinalizeThreadCallback(m_nThreadID);
}

SMessage CThreadConnection::ParseAddContactMessage(std::string strMsg)
{
    SMessage msg;
    
    std::string strSubMsg;
    std::stringstream lineStream(strMsg);
    
    std::getline(lineStream, strSubMsg, '#');
    std::getline(lineStream, strSubMsg, '#');
    
    msg.m_strSenderName = strSubMsg;
    msg.m_strSenderIP = m_strClientIP;
    msg.m_nMessageCode = 0;
    
    return msg;
}

SMessage CThreadConnection::ParseChatMessage(std::string strMsg)
{
    SMessage msg;
    
    std::string strSubMsg;
    std::stringstream lineStream(strMsg);
    
    std::getline(lineStream, strSubMsg, '#');
    std::getline(lineStream, strSubMsg, '#');
    
    msg.m_strMessage = strSubMsg;
    msg.m_strSenderIP = m_strClientIP;
    msg.m_nMessageCode = 2;
    
    return msg;
}

SMessage CThreadConnection::ParseMessage(std::string strMsg)
{
    SMessage msg;
    std::string strSubMsg;
    std::stringstream lineStream(strMsg);
    
    std::getline(lineStream, strSubMsg, '#');
    
    int nMessageCode = atoi(strSubMsg.c_str());
    
    switch (nMessageCode) {
        case 0:
            msg = ParseAddContactMessage(strMsg);
            break;
        case 1: // create group
            break;
        case 2:
            msg = ParseChatMessage(strMsg);
            break;
        case 3: // group message
            break;
        case 4:
            msg.m_nMessageCode = 4;
            msg.m_strSenderIP = m_strClientIP;
            break;
        default:
            break;
    }
    
    
    return msg;
}

void CThreadConnection::SetCallbackFuction(void (*callback)(int))
{
    FinalizeThreadCallback = callback;
}

void CThreadConnection::SetSocket(int con)
{
    m_nConnection = con;
}

int CThreadConnection::GetSocket()
{
    return m_nConnection;
}

void CThreadConnection::SetThreadID(int nThreadID)
{
    m_nThreadID = nThreadID;
}

void CThreadConnection::SetClientAddr(std::string IP)
{
    m_strClientIP = IP;
}