//
//  MessagePool.hpp
//  Trabalho2
//
//  Created by Alexandre Severo Jr on 6/9/16.
//  Copyright © 2016 asj. All rights reserved.
//

#ifndef MessagePool_h
#define MessagePool_h

#include <queue>
#include <iostream>
#include <pthread.h>
#include "Message.h"

class CMessagePool
{
public:
 
    CMessagePool()
    {
        /*
        m_ptmOutPoolMutex = PTHREAD_MUTEX_INITIALIZER;
        m_ptmInPoolMutex = PTHREAD_MUTEX_INITIALIZER;
        */
        pthread_mutex_init(&m_ptmOutPoolMutex, NULL);
        pthread_mutex_init(&m_ptmInPoolMutex, NULL);
    }
    
    static pthread_mutex_t m_ptmOutPoolMutex;
    static pthread_mutex_t m_ptmInPoolMutex;
    
    static std::queue<SMessage> m_qInMessages;
    static std::queue<SMessage> m_qOutMessages;
    
    static void InsertOutMessage(SMessage msg);
    static void InsertInMessage(SMessage msg);
    
    static SMessage GetOutMessage();
    static SMessage GetInMessage();
    
    static bool HasOutMessages();
    static bool HasInMessages();
    
};

#endif /* MessagePool_h */
