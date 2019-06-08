//
//  Group.hpp
//  Trabalho2
//
//  Created by Alexandre Severo Jr on 6/15/16.
//  Copyright © 2016 asj. All rights reserved.
//

#ifndef Group_h
#define Group_h

#include <stdio.h>
#include <vector>
#include <string>
#include "Contact.h"

class CGroup
{
private:
    
    std::vector<CContact*> m_vecContactList;
    
    std::vector<std::string> m_vecMessages;
    
    std::string m_strName;

    bool m_bHasUnreadSentMessages;
    bool m_bHasUnreadRecvMessages;
    
    int m_nUnreadMessageCount;
    
    pthread_mutex_t m_ptmVecMutex;
    
public:
    
    CGroup() {}
    CGroup(std::string strName);
    
    void SetName(std::string strName);
    void SetUnreadMessages(bool bHasNewMessages);
    
    void InsertSentMessage(std::string msg);
    
    std::string GetName();
    bool HasUnreadRecvMessages();
    int GetUnreadMsgsCount();
    
    std::vector<std::string> GetIP();
    
    std::vector<std::string> GetMessages();
    
    void InsertContact(CContact *contact);
    
};


#endif /* Group_h */
