//
//  Contact.h
//  Trabalho2
//
//  Created by Alexandre Severo Jr on 6/11/16.
//  Copyright © 2016 asj. All rights reserved.
//

#ifndef Contact_h
#define Contact_h

#include <string>
#include <iostream>
#include <vector>


class CContact
{
private:
    
    std::string m_strName;
    std::string m_strIP;
    
    int m_nContactID;
    
    std::vector<std::string> m_vecStrMessages;
    std::vector<std::string> m_vecStrUnreadSentMessages;
    std::vector<std::string> m_vecStrUnreadRecvMessages;
    
    pthread_mutex_t m_ptmVecMutex;
    
public:
    
    CContact() {}
    CContact(std::string strName, std::string strIP);
    
    std::string GetName();
    std::string GetIP();
    
    bool HasUnreadRecvMessages();
    bool HasUnreadSentMessages();
    
    void IncrementUnreadMsgs();
    int GetUnreadMsgsCount();
    int GetID();
    
    void SetName(std::string strName);
    void SetIP(std::string strIP);
    void SetID(int nID);
    
    void InsertSentMessage(std::string msg);
    void InsertRecvMessage(std::string msg);
    
    void UpdateMessages();
    void ReadMessages();
    std::vector<std::string> ListMessages();
    std::vector<std::string> GetRecvMessages();
    std::vector<std::string> GetSentMessages();
    std::vector<std::string> GetMessages();
};

#endif /* Contact_h */
