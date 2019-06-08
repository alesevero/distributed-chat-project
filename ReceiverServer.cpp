//
//  ReceiverServer.cpp
//  Trabalho2
//
//  Created by Alexandre Severo Jr on 6/4/16.
//  Copyright © 2016 asj. All rights reserved.
//

#include "ReceiverServer.h"

std::vector<pthread_t> CReceiverServer::m_vecThreads;
std::vector<CThreadConnection> CReceiverServer::m_vecConnections;

CReceiverServer::CReceiverServer(int nPort)
{
    m_nPort = nPort;
}

void * CReceiverServer::ThreadInit(void *i)
{
    ((CReceiverServer *) i)->RunThread();
    return NULL;
}

void CReceiverServer::RunThread()
{
    ConfigureSocket();
    
    while (1)
    {
        //std::cout << "Listening" << std::endl;
        
        //this is where client connects. svr will hang in this mode until client conn
        m_nConnection = accept(listenFd, (struct sockaddr *)&clntAdd, &len);
        
        // create thread to receive messages
        CThreadConnection *tdConnection = new CThreadConnection();
        tdConnection->SetSocket(m_nConnection);
        tdConnection->SetCallbackFuction(this->FinalizeThread); // set the callback function to end the thread
        tdConnection->SetThreadID((int)m_vecConnections.size());
        std::string ip = inet_ntoa(clntAdd.sin_addr);
        tdConnection->SetClientAddr(ip);
        
        pthread_t thread;
        pthread_create(&thread, NULL, tdConnection->ThreadInit, tdConnection);
        
        m_vecThreads.push_back(thread);
        m_vecConnections.push_back(*tdConnection);
    }
    
    // aguarda por uma conexão
    // le a mensagem
    // envia para o pool de mensagens de entrada
}

void CReceiverServer::FinalizeThread(int nThreadID)
{
    if ((nThreadID < m_vecThreads.size()) && (nThreadID < m_vecConnections.size()))
    {
        close(m_vecConnections[nThreadID].GetSocket());
        pthread_t thread = m_vecThreads[nThreadID];
        pthread_cancel(thread);
        std::cout << "finalizou" << std::endl;
    }
}

bool CReceiverServer::ConfigureSocket()
{
    bool bResult = true;
    
    listenFd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(listenFd < 0)
    {
        std::cerr << "Cannot open socket" << std::endl;
        bResult = false;
    }
    
    bzero((char*) &svrAdd, sizeof(svrAdd));
    
    svrAdd.sin_family = AF_INET;
    svrAdd.sin_addr.s_addr = INADDR_ANY;
    svrAdd.sin_port = htons(m_nPort);
    
    //bind socket
    if (bind(listenFd, (struct sockaddr *)&svrAdd, sizeof(svrAdd)) < 0)
    {
        std::cerr << "Cannot bind" << std::endl;
        bResult = false;
    }
    
    listen(listenFd, 5);
    
    len = sizeof(clntAdd);
    
    return bResult;
}
