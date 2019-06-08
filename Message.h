//
//  Message.h
//  Trabalho2
//
//  Created by Alexandre Severo Jr on 6/5/16.
//  Copyright © 2016 asj. All rights reserved.
//

#ifndef Message_h
#define Message_h

#include <string>
#include <vector>


struct SMessage
{
    std::string m_strMessage;
    std::vector<std::string> m_vecStrDestinationIP;
    std::vector<std::string> m_vecStrGroupMembersName;
    std::string m_strSenderIP;
    std::string m_strSenderName;
    std::string m_strGroupName;
    
    bool m_bFromGroup;
    bool m_bMessageReaded;
    
    int m_nMessageCode;
    int m_nGroupContactCount;
    
    /*
     *  m_nMessageCode:
     *      0 - AddContact
     *      1 - CreateGroup
     *      2 - Chat message
     *      3 - Group message
     *      4 - Message read
     */
};

#endif /* Message_h */
