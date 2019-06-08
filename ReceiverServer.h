//
//  ReceiverServer.hpp
//  Trabalho2
//
//  Created by Alexandre Severo Jr on 6/4/16.
//  Copyright © 2016 asj. All rights reserved.
//

#ifndef ReceiverServer_h
#define ReceiverServer_h

#include <vector>
#include <arpa/inet.h>
#include "ThreadConnection.h"
#include "MessagePool.h"

class CReceiverServer
{
private:
    
    // Socket info
    
    int m_nPID;
    int m_nPort;
    int m_nListenFd;
    int m_nConnection;
    
    socklen_t m_slSocketLength;
    
    struct sockaddr_in m_saServerAdd;
    struct sockaddr_in m_saClientAdd;
    
    /////////////////////////////////////////////////
    // variaveis para usar com socket
    int pId, listenFd;
    socklen_t len; //store size of the address
    bool loop = false;
    struct sockaddr_in svrAdd, clntAdd;
    /////////////////////////////////////////////////
    
    static std::vector<CThreadConnection> m_vecConnections;
    static std::vector<pthread_t> m_vecThreads;
    
    // Methods
    void RunThread();
    
public:
    
    CReceiverServer();
    CReceiverServer(int nPort);
    
    static void FinalizeThread(int nThreadId);
    
    static void * ThreadInit(void *i);
    
    bool ConfigureSocket();
};

#endif /* ReceiverServer_h */
