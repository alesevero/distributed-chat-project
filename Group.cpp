//
//  Group.cpp
//  Trabalho2
//
//  Created by Alexandre Severo Jr on 6/15/16.
//  Copyright © 2016 asj. All rights reserved.
//

#include "Group.h"


CGroup::CGroup(std::string strName)
{
    m_strName = strName;
    m_bHasUnreadRecvMessages = false;
    m_bHasUnreadSentMessages = false;
    m_nUnreadMessageCount = 0;
}

void CGroup::InsertContact(CContact *contact)
{
    m_vecContactList.push_back(contact);
}

void CGroup::SetName(std::string strName)
{
    m_strName = strName;
}

void CGroup::SetUnreadMessages(bool bHasNewMessages)
{
    m_bHasUnreadRecvMessages = bHasNewMessages;
}

std::string CGroup::GetName()
{
    return m_strName;
}

bool CGroup::HasUnreadRecvMessages()
{
    return m_bHasUnreadRecvMessages;
}

int CGroup::GetUnreadMsgsCount()
{
    return m_nUnreadMessageCount;
}

std::vector<std::string> CGroup::GetMessages()
{
    pthread_mutex_lock(&m_ptmVecMutex);
    std::vector<std::string> vecMsg = m_vecMessages;
    pthread_mutex_unlock(&m_ptmVecMutex);
    
    return vecMsg;
}

std::vector<std::string> CGroup::GetIP()
{
    std::vector<std::string> vecIP;
    for (int i = 0; i < m_vecContactList.size(); i++)
        vecIP.push_back(m_vecContactList[i]->GetIP());
    
    return vecIP;
}

void CGroup::InsertSentMessage(std::string msg)
{
    pthread_mutex_lock(&m_ptmVecMutex);
    m_vecMessages.push_back("eu: " + msg);
    pthread_mutex_unlock(&m_ptmVecMutex);
}









