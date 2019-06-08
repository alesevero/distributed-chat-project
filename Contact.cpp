//
//  Contact.cpp
//  Trabalho2
//
//  Created by Alexandre Severo Jr on 6/11/16.
//  Copyright © 2016 asj. All rights reserved.
//

#include "Contact.h"

CContact::CContact(std::string strName, std::string strIP)
{
    m_strIP = strIP;
    m_strName = strName;
    
    pthread_mutex_init(&m_ptmVecMutex, NULL);
}

std::vector<std::string> CContact::ListMessages()
{
    std::vector<std::string> vecMsg;
    pthread_mutex_lock(&m_ptmVecMutex);
    if (m_vecStrMessages.size() > 0)
        for (int i = 0; i < m_vecStrMessages.size(); i++)
        {
            std::string str = m_vecStrMessages[i];
            if (std::string::npos != str.find("eu"))
                str += " (l)";
            
            vecMsg.push_back(str);
        }
    if (m_vecStrUnreadSentMessages.size() > 0)
        for (int i = 0; i < m_vecStrUnreadSentMessages.size(); i++)
            vecMsg.push_back(m_vecStrUnreadSentMessages[i] + " (nl)");
    
    if (m_vecStrUnreadRecvMessages.size() > 0)
        for (int i = 0; i < m_vecStrUnreadRecvMessages.size(); i++)
        {
            vecMsg.push_back(m_vecStrUnreadRecvMessages[i]);
        }
    
    pthread_mutex_unlock(&m_ptmVecMutex);
    
    ReadMessages();
    
    return vecMsg;
}

void CContact::UpdateMessages()
{
    pthread_mutex_lock(&m_ptmVecMutex);
    if (m_vecStrUnreadSentMessages.size() > 0)
        for (int i = 0; i < m_vecStrUnreadSentMessages.size(); i++)
            m_vecStrMessages.push_back(m_vecStrUnreadSentMessages[i]);
    
    m_vecStrUnreadSentMessages.clear();
    
    pthread_mutex_unlock(&m_ptmVecMutex);
}

void CContact::ReadMessages()
{
    pthread_mutex_lock(&m_ptmVecMutex);
    
    if (m_vecStrUnreadRecvMessages.size() > 0)
        for (int i = 0; i < m_vecStrUnreadRecvMessages.size(); i++)
            m_vecStrMessages.push_back(m_vecStrUnreadRecvMessages[i]);
    
    m_vecStrUnreadRecvMessages.clear();
    pthread_mutex_unlock(&m_ptmVecMutex);
}

void CContact::InsertSentMessage(std::string msg)
{
    pthread_mutex_lock(&m_ptmVecMutex);
    m_vecStrUnreadSentMessages.push_back("eu: " + msg);
    pthread_mutex_unlock(&m_ptmVecMutex);
}

void CContact::InsertRecvMessage(std::string msg)
{
    pthread_mutex_lock(&m_ptmVecMutex);
    m_vecStrUnreadRecvMessages.push_back(m_strName + ": " + msg);
    pthread_mutex_unlock(&m_ptmVecMutex);
}

std::string CContact::GetName()
{
    return m_strName;
}

std::string CContact::GetIP()
{
    return m_strIP;
}

int CContact::GetID()
{
    return m_nContactID;
}

bool CContact::HasUnreadRecvMessages()
{
    pthread_mutex_lock(&m_ptmVecMutex);
    bool bHas = m_vecStrUnreadRecvMessages.size() > 0;
    pthread_mutex_unlock(&m_ptmVecMutex);
    
    return bHas;
}

bool CContact::HasUnreadSentMessages()
{
    pthread_mutex_lock(&m_ptmVecMutex);
    bool bHas = m_vecStrUnreadSentMessages.size() > 0;
    pthread_mutex_unlock(&m_ptmVecMutex);
    
    return bHas;
}

int CContact::GetUnreadMsgsCount()
{
    pthread_mutex_lock(&m_ptmVecMutex);
    int count = (int)m_vecStrUnreadRecvMessages.size();
    pthread_mutex_unlock(&m_ptmVecMutex);
    
    return count;
}

void CContact::SetName(std::string strName)
{
    m_strName = strName;
}

void CContact::SetIP(std::string strIP)
{
    m_strIP = strIP;
}

void CContact::SetID(int nID)
{
    m_nContactID = nID;
}