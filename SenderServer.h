//
//  SenderServer.hpp
//  Trabalho2
//
//  Created by Alexandre Severo Jr on 6/4/16.
//  Copyright © 2016 asj. All rights reserved.
//

#ifndef SenderServer_h
#define SenderServer_h

#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/msg.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <vector>

#include "MessagePool.h"

class CSenderServer
{
private:
    
    CMessagePool *m_MessagePool;

    int m_nPort;
    int m_nListenFd;
    
    bool m_bOpenConnection;
    
    std::string m_strMyIP;
    std::string m_strMyName;
    
    struct sockaddr_in m_saServerAdd;
    struct hostent *m_heServer;
    
    pthread_t m_ptServerThread;
    
    // Methods
    void RunThread();
    bool ConfigureSocket(std::string strIP);
    
    bool GetMessageFromQueue(SMessage &msg);
    std::string FormatMessage(SMessage msg);
    
public:
    
    CSenderServer();
    CSenderServer(int nPort, std::string strName, std::string strMyIP);
    
    static void * ThreadInit(void *i);
};

#endif /* SenderServer_h */
