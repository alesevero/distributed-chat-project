//
//  MessagePool.cpp
//  Trabalho2
//
//  Created by Alexandre Severo Jr on 6/9/16.
//  Copyright © 2016 asj. All rights reserved.
//

#include "MessagePool.h"

// static declaration
std::queue<SMessage> CMessagePool::m_qInMessages;
std::queue<SMessage> CMessagePool::m_qOutMessages;

pthread_mutex_t CMessagePool::m_ptmOutPoolMutex;
pthread_mutex_t CMessagePool::m_ptmInPoolMutex;


// Methods
void CMessagePool::InsertOutMessage(SMessage msg)
{
    m_qOutMessages.push(msg);
}

void CMessagePool::InsertInMessage(SMessage msg)
{
    m_qInMessages.push(msg);
}

SMessage CMessagePool::GetOutMessage()
{
    SMessage msg = m_qOutMessages.front();
    m_qOutMessages.pop();
    return msg;
}

SMessage CMessagePool::GetInMessage()
{
    SMessage msg = m_qInMessages.front();
    m_qInMessages.pop();
    return msg;
}

bool CMessagePool::HasOutMessages()
{
    return (m_qOutMessages.size() < 1) ? false : true;
}

bool CMessagePool::HasInMessages()
{
    return (m_qInMessages.size() < 1) ? false : true;
}
