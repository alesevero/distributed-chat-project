//
//  SenderServer.cpp
//  Trabalho2
//
//  Created by Alexandre Severo Jr on 6/4/16.
//  Copyright © 2016 asj. All rights reserved.
//

#include "SenderServer.h"

CSenderServer::CSenderServer(int nPort, std::string strName, std::string strMyIP)
{
    m_nPort = nPort;
    m_strMyIP = strMyIP;
    m_strMyName = strName;
}

bool CSenderServer::ConfigureSocket(std::string strIP)
{
    if (m_bOpenConnection)
        return true;
    else
    {
        bool bConnected = false;
        
        m_nListenFd = socket(AF_INET, SOCK_STREAM, 0);
        
        if(m_nListenFd < 0)
            std::cerr << "Cannot open socket" << std::endl;
        else
        {
            m_heServer = gethostbyname(strIP.c_str());
            bzero((char *) &m_saServerAdd, sizeof(m_saServerAdd));
            m_saServerAdd.sin_family = AF_INET;
            bcopy((char *) m_heServer -> h_addr, (char *) &m_saServerAdd.sin_addr.s_addr, m_heServer -> h_length);
            m_saServerAdd.sin_port = htons(m_nPort);
            
            if (connect(m_nListenFd,(struct sockaddr *) &m_saServerAdd, sizeof(m_saServerAdd)) < 0)
                std::cerr << "Cannot connect!" << std::endl;
            else
                bConnected = true;
        }
        
        m_bOpenConnection = true;
        return bConnected;
    }
}

void * CSenderServer::ThreadInit(void *i)
{
    ((CSenderServer *) i)->RunThread();
    return NULL;
}

void CSenderServer::RunThread()
{
    // method for the thread
    m_MessagePool = new CMessagePool();
    SMessage msg;

    m_bOpenConnection = false;
    for (;;)
    {
        if (GetMessageFromQueue(msg))
        {
            if (msg.m_nMessageCode == 5)
                break; // encerra a thread
            
            msg.m_strSenderName = m_strMyName;
            std::string strMsg = FormatMessage(msg);
            // send the message to all recipients
            for (int i = 0; i < msg.m_vecStrDestinationIP.size(); i++)
            {
                if (ConfigureSocket(msg.m_vecStrDestinationIP[i]))
                    write(m_nListenFd, strMsg.c_str(), sizeof(strMsg));
                else
                {
                    // TODO: SALVAR NO ARQUIVO
                }
            }
        }
    }
    std::cout << "sender server finalizou" << std::endl;
}

bool CSenderServer::GetMessageFromQueue(SMessage &msg)
{
    bool bHasMessage = false;
    // Get message from the out pool, locking its access
    pthread_mutex_lock(&CMessagePool::m_ptmOutPoolMutex);
    if (m_MessagePool->HasOutMessages())
    {
        msg = m_MessagePool->GetOutMessage();
        bHasMessage = true;
    }
    pthread_mutex_unlock(&CMessagePool::m_ptmOutPoolMutex);
    
    return bHasMessage;
}

std::string CSenderServer::FormatMessage(SMessage msg)
{
    std::string strMsg;
    
    switch (msg.m_nMessageCode) {
        case 0:
            strMsg = "0#" + m_strMyName;
            break;
        case 1:
            // create group
            break;
        case 2:
            strMsg = "2#" + msg.m_strMessage;
            break;
        case 3:
            // group message
            break;
        case 4:
            strMsg = "4#";
            break;
            
        default:
            break;
    }
    
    return strMsg;
}










