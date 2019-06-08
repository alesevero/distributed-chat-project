//
//  Client.h
//  Trabalho2
//
//  Created by Alexandre Severo Jr on 6/9/16.
//  Copyright © 2016 asj. All rights reserved.
//

#ifndef Client_h
#define Client_h

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include "Group.h"
#include "Contact.h"
#include "MessagePool.h"
#include "conio.h"


class CClient
{
private:
    
    static std::vector<CContact> m_vecContactList;
    static std::vector<CGroup> m_vecGroupList;
    
    CMessagePool *m_MessagePool;
    
    // thread to handle the data receiver from the server
    pthread_t m_ptRecvDataHandler;
    
    /*
     * Thread methods
     */
    void RunThread();
    void StartDataHandler();
    void StopDataHandler();
    void RunRecvThread();
    static void * DataHandlerThreadInit(void *i);
    
    /*
     * Main methods
     */
    void SendToQueue(SMessage msg);
    void SendMessage();
    void AddContact();
    void AddContact(SMessage msg);
    void CreateGroup();
    void CreateGroup(SMessage msg);
    void ListContacts();
    void ListMessages();
    void Exit();

    /*
     * Contact methods
     */
    CContact SaveContact(std::string strName, std::string strIP);
    
    bool ContactExists(CContact &contact);
    bool GetContactByName(std::string strName, CContact &contact);
    
    int GetContactByName(std::string strName);
    int GetContactByIP(std::string strIP);
    
    void ReadChatMessage(SMessage msg);
    void InsertRecvChatMessage(SMessage msg);
    
    /*
     * Group methods
     */
    bool GroupExists(std::string strName);
    bool GetGroupByName(std::string strName, CGroup &group);
    int GetGroupByName(std::string strName);
    
    
public:
    
    static void * ThreadInit(void *i);
};

#endif /* Client_h */
