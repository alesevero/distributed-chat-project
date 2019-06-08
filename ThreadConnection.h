//
//  ThreadConnection.h
//  Trabalho2
//
//  Created by Alexandre Severo Jr on 6/8/16.
//  Copyright © 2016 asj. All rights reserved.
//

#ifndef ThreadConnection_h
#define ThreadConnection_h

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "Message.h"
#include "MessagePool.h"

class CThreadConnection
{
private:
    
    typedef void (*Callback)(int);
    Callback FinalizeThreadCallback;
    
    CMessagePool m_MessagePool;
    
    int m_nConnection; // socket
    std::string m_strClientIP;
    
    int m_nThreadID;
    
    SMessage ParseMessage(std::string strMsg);
    SMessage ParseAddContactMessage(std::string strMsg);
    SMessage ParseChatMessage(std::string strMsg);
    SMessage ParseCreateGroupMessage(std::string strMsg);
    SMessage ParseGroupChatMessage(std::string strMsg);
    
public:
    
    static void * ThreadInit(void *i);
    void RunThread();
    
    void SetSocket(int con);
    int GetSocket();
    
    void SetThreadID(int nThreadID);
    
    void SetCallbackFuction(void (*callback)(int));
    
    void SetClientAddr(std::string IP);
    
};

#endif /* ThreadConnection_h */
