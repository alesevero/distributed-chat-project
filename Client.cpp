//
//  Client.cpp
//  Trabalho2
//
//  Created by Alexandre Severo Jr on 6/9/16.
//  Copyright © 2016 asj. All rights reserved.
//

#include "Client.h"

std::vector<CContact> CClient::m_vecContactList;
std::vector<CGroup> CClient::m_vecGroupList;

void * CClient::ThreadInit(void *i)
{
    ((CClient *) i)->RunThread();
    
    return NULL;
}

void CClient::RunThread()
{
    StartDataHandler();
    m_MessagePool = new CMessagePool();
    std::string strCommand;
    bool bLoop = true;
    
    while (bLoop)
    {
        clrscr();
        std::cout << "***************************************" << std::endl
                  << "*                                     *" << std::endl
                  << "* Comandos:                           *" << std::endl
                  << "*                                     *" << std::endl
                  << "*   0 - Adicionar Contato             *" << std::endl
                  << "*   1 - Criar Grupo                   *" << std::endl
                  << "*   2 - Lista Mensagens               *" << std::endl
                  << "*   3 - Enviar Mensagem               *" << std::endl
                  << "*   4 - Listar Contatos               *" << std::endl
                  << "*   5 - Sair                          *" << std::endl
                  << "*                                     *" << std::endl
                  << "***************************************" << std::endl;
                  
        strCommand = "";
        std::cout << "Escolha uma opção: " << std::endl;
        std::cin >> strCommand;
        int nCommand = 9;
        
        try
        {
            nCommand = stoi(strCommand);
        }
        catch (std::exception e)
        {
            std::cout << "Opção inválida!" << std::endl;
        }
        
        switch (nCommand)
        {
            case 0:
                clrscr();
                std::cout << "------------------------" << std::endl;
                std::cout << "|  ADICIONAR CONTATO   |" << std::endl;
                std::cout << "------------------------" << std::endl;
                AddContact();
                break;
            case 1:
                clrscr();
                std::cout << "------------------------" << std::endl;
                std::cout << "|     CRIAR GRUPO      |" << std::endl;
                std::cout << "------------------------" << std::endl;
                //CreateGroup();
                break;
            case 2:
                clrscr();
                std::cout << "------------------------" << std::endl;
                std::cout << "|      MENSAGENS       |" << std::endl;
                std::cout << "------------------------" << std::endl;
                ListMessages();
                break;
            case 3:
                clrscr();
                std::cout << "------------------------" << std::endl;
                std::cout << "|    ENVIAR MENSAGEM   |" << std::endl;
                std::cout << "------------------------" << std::endl;
                SendMessage();
                break;
            case 4:
                clrscr();
                std::cout << "------------------------" << std::endl;
                std::cout << "|  LISTA DE CONTATOS   |" << std::endl;
                std::cout << "------------------------" << std::endl;
                ListContacts();
                break;
            case 5:
                StopDataHandler();
                Exit();
                bLoop = false;
                break;
            default:
                break;
        }
        
        if (nCommand != 5)
        {
            std::cin.ignore();
            std::getline(std::cin, strCommand);
        }
    }
    
    // delete message pool pointer
    m_MessagePool = NULL;
    delete m_MessagePool;
    
    std::cout << "client finalizou" << std::endl;
}

// Main methods

// 0 - Add contact
void CClient::AddContact()
{
    std::string strName;
    std::string strIP;
    
    std::cout << "Digite o nome do contato: " << std::endl;
    std::cin >> strName;
    
    std::cout << "Digite o IP do contato (XXX.XXX.XXX.XXX): " << std::endl;
    std::cin >> strIP;
    
    SaveContact(strName, strIP);
}

void CClient::AddContact(SMessage msg)
{
    std::string str = msg.m_strSenderName;
    CContact contact(msg.m_strSenderName, msg.m_strSenderIP);
    bool bExists = ContactExists(contact);
    
    if (bExists)
        std::cout << "Contato já existe!" << std::endl;
    else
    {
        m_vecContactList.push_back(contact);
    }

}

// 4 - List contact
void CClient::ListContacts()
{
    std::cout << "CONTATOS: " << std::endl;
    for (int i = 0; i < m_vecContactList.size(); i++)
    {
        if (m_vecContactList[i].HasUnreadRecvMessages())
            std::cout << "Nome: " << m_vecContactList[i].GetName() << "(" << m_vecContactList[i].GetUnreadMsgsCount() << ")" << std::endl;
        else
            std::cout << "Nome: " << m_vecContactList[i].GetName() << std::endl;
    }
    
    std::cout << std::endl << "GRUPOS: " << std::endl;
    for (int i = 0; i < m_vecGroupList.size(); i++)
    {
        if (m_vecGroupList[i].HasUnreadRecvMessages())
            std::cout << "Nome: " << m_vecGroupList[i].GetName() << "(" << m_vecGroupList[i].GetUnreadMsgsCount() << ")" << std::endl;
        else
            std::cout << "Nome: " << m_vecGroupList[i].GetName() << std::endl;
    }
}

// 1 - Create group
void CClient::CreateGroup()
{
    std::string strGroupName;
    std::string strName;
    std::string strIP;
    int nContactCount;
    
    // declarar grupo para adicionar os contatos
    
    std::cout << "Digite o nome do grupo: " << std::endl;
    std::cin >> strGroupName;
    
    std::cout << "Quantos contatos? " << std::endl;
    std::cin >> nContactCount;
    
    CGroup group(strGroupName);
    
    SMessage msg;
    msg.m_nMessageCode = 1; // code to add contact
    msg.m_nGroupContactCount = nContactCount;
    
    for (int j = 0; j < nContactCount; j++)
    {
        std::cout << "Digite o nome do contato " << j << ": " << std::endl;
        std::cin >> strName;
        
        CContact contact;
        contact.SetName(strName);
        if (!GetContactByName(strName, contact))
        {
            std::cout << "Digite o IP do contato (XXX.XXX.XXX.XXX): " << std::endl;
            std::cin >> strIP;
            contact = SaveContact(strName, strIP);
        }
        
        // adiciona no grupo
        group.InsertContact(&contact);
        
        // insert the ip to the message
        msg.m_vecStrDestinationIP.push_back(contact.GetIP());
        msg.m_vecStrGroupMembersName.push_back(contact.GetName());
    }
    
    m_vecGroupList.push_back(group);
    
    // Sends add contact message
    SendToQueue(msg);
}

void CClient::CreateGroup(SMessage msg)
{
    if (!GroupExists(msg.m_strGroupName))
    {
        CGroup group;
        CContact *contact = new CContact();
        GetContactByName(msg.m_strSenderName, *contact);
        group.InsertContact(contact);
        
        for (int i = 0; i < msg.m_vecStrDestinationIP.size(); i++)
        {
            contact = NULL;
            contact = new CContact(msg.m_vecStrGroupMembersName[i], msg.m_vecStrDestinationIP[i]);
            
            if (!ContactExists(*contact))
                m_vecContactList.push_back(*contact);
            
            group.InsertContact(contact);
        }
        
        m_vecGroupList.push_back(group);
    }
    
    std::cout << "Grupo com esse nome já existe!" << std::endl;
}


// Auxiliar methods
CContact CClient::SaveContact(std::string strName, std::string strIP)
{
    
    CContact contact(strName, strIP);
    bool bExists = ContactExists(contact);
    
    if (bExists)
        std::cout << "Contato já existe!" << std::endl;
    else
    {
        m_vecContactList.push_back(contact);
        
        SMessage msg;
        msg.m_nMessageCode = 0; // code to add contact
        msg.m_vecStrDestinationIP.push_back(strIP);
        
        // Sends add contact message
        SendToQueue(msg);
        std::cout << "Contato adicionado com sucesso!" << std::endl;
    }
    
    return m_vecContactList[m_vecContactList.size() - 1];
}

bool CClient::ContactExists(CContact &contact)
{
    for (int i = 0; i < m_vecContactList.size(); i++)
    {
        if ((contact.GetName().compare(m_vecContactList[i].GetName()) == 0) ||
            (contact.GetIP().compare(m_vecContactList[i].GetIP())     == 0))
        {
            contact.SetID(m_vecContactList[i].GetID());
            return true;
        }
    }
    return false;
}

bool CClient::GetContactByName(std::string strName, CContact &contact)
{
    for (int i = 0; i < m_vecContactList.size(); i++)
    {
        if (strName.compare(m_vecContactList[i].GetName()) == 0)
        {
            contact = m_vecContactList[i];
            return true;
        }
    }
    return false;
}

int CClient::GetContactByName(std::string strName)
{
    for (int i = 0; i < m_vecContactList.size(); i++)
    {
        if (strName.compare(m_vecContactList[i].GetName()) == 0)
            return i;
    }
    return (-1);
}

int CClient::GetContactByIP(std::string strIP)
{
    for (int i = 0; i < m_vecContactList.size(); i++)
    {
        if (strIP.compare(m_vecContactList[i].GetIP()) == 0)
            return i;
    }
    return (-1);
}

bool CClient::GroupExists(std::string strName)
{
    for (int i = 0; i < m_vecGroupList.size(); i++)
    {
        if (strName.compare(m_vecGroupList[i].GetName()))
            return true;
    }
    
    return false;
}

bool CClient::GetGroupByName(std::string strName, CGroup &group)
{
    for (int i = 0; i < m_vecGroupList.size(); i++)
    {
        if (strName.compare(m_vecGroupList[i].GetName()) == 0)
        {
            group = m_vecGroupList[i];
            return true;
        }
    }
    return false;
}

int CClient::GetGroupByName(std::string strName)
{
    for (int i = 0; i < m_vecGroupList.size(); i++)
    {
        if (strName.compare(m_vecGroupList[i].GetName()) == 0)
            return i;
    }
    return (-1);
}

void CClient::ListMessages()
{
    std::string strContactName;
    std::cout << "Digite o nome do contato: " << std::endl;
    std::cin >> strContactName;
    
    CContact contact;
    //CGroup group;
    std::vector<std::string> vecMessages;
    int ID = 0;
    if ((ID = GetContactByName(strContactName)) >= 0)
    {
        vecMessages = m_vecContactList[ID].ListMessages();
        for (int i = 0; i < vecMessages.size(); i++)
            std::cout << vecMessages[i] << std::endl;
        
        SMessage msg;
        msg.m_nMessageCode = 4;
        msg.m_vecStrDestinationIP.push_back(m_vecContactList[ID].GetIP());
        
        SendToQueue(msg);
        
    }
    /*else if ((ID = GetGroupByName(strContactName)) >= 0)
    {
        vecMessages = m_vecGroupList[ID].GetMessages();
        for (int i = 0; i < vecMessages.size(); i++)
            std::cout << vecMessages[i] << std::endl;
    }*/
    else
        std::cout << "Contato/grupo não existe!" << std::endl;
}

void CClient::SendMessage()
{
    std::string strMessage;
    std::string strName;
    bool bLoop = true;
    while (bLoop)
    {
        strName = "";
        std::cout << "Digite nome do usuário (exit para sair): " << std::endl;
        std::cin >> strName;
        
        CContact contact;
        CGroup group;
        int ID = 0;
        if ((ID = GetContactByName(strName)) >= 0) // add if pra ver se existe
        {
            while (bLoop)
            {
                strMessage = "";
                std::cout << ">> ";
                //std::cin.ignore();
                std::getline(std::cin, strMessage);
                
                if (strMessage.compare("exit") == 0)
                {
                    bLoop = false;
                    break;
                }
                else if (strMessage.compare("") != 0)
                {
                    SMessage msg;
                    msg.m_strMessage = strMessage;
                    
                    msg.m_nMessageCode = 2; // messagem from contact
                    msg.m_vecStrDestinationIP.push_back(contact.GetIP());
                    m_vecContactList[ID].InsertSentMessage(msg.m_strMessage);
                    
                    SendToQueue(msg);
                }
            }
        }
        else if ((ID = GetGroupByName(strName)) >= 0)
        {
            while (bLoop)
            {
                strMessage = "";
                std::cout << ">> ";
                std::cin.ignore(200, '\n');
                std::getline(std::cin, strMessage);
                
                if (strMessage.compare("exit") == 0)
                {
                    bLoop = false;
                    break;
                }
                
                SMessage msg;
                msg.m_strMessage = strMessage;
                msg.m_nMessageCode = 3; // message from group
                msg.m_vecStrDestinationIP = group.GetIP();
                m_vecGroupList[ID].InsertSentMessage(msg.m_strMessage);
                
                SendToQueue(msg);
            }
        }
    }
}

void CClient::SendToQueue(SMessage msg)
{
    pthread_mutex_lock(&CMessagePool::m_ptmOutPoolMutex);
    m_MessagePool->InsertOutMessage(msg);
    pthread_mutex_unlock(&CMessagePool::m_ptmOutPoolMutex);
}

void CClient::Exit()
{
    SMessage msg;
    msg.m_nMessageCode = 5;
    SendToQueue(msg);
}

void CClient::StartDataHandler()
{
    pthread_create(&m_ptRecvDataHandler, NULL, this->DataHandlerThreadInit, this);
}

void CClient::StopDataHandler()
{
    pthread_cancel(m_ptRecvDataHandler);
}

void * CClient::DataHandlerThreadInit(void * i)
{
    ((CClient *) i)->RunRecvThread();
    return NULL;
}

void CClient::RunRecvThread()
{
    bool bLoop = true;
    while (bLoop)
    {
        if (CMessagePool::HasInMessages())
        {
            SMessage msg;
            pthread_mutex_lock(&CMessagePool::m_ptmInPoolMutex);
            msg = m_MessagePool->GetInMessage();
            pthread_mutex_unlock(&CMessagePool::m_ptmInPoolMutex);
            
            switch (msg.m_nMessageCode) {
                case 0:
                    AddContact(msg);
                    break;
                case 1:
                    //CreateGroup(msg);// create group
                    break;
                case 2: // chat message
                    InsertRecvChatMessage(msg);
                    break;
                case 3: // message from group
                    break;
                case 4: ReadChatMessage(msg);
                    break;
                default:
                    break;
            }
            
        }
    }
}

void CClient::ReadChatMessage(SMessage msg)
{
    int ID;
    
    if ((ID = GetContactByIP(msg.m_strSenderIP)) >= 0)
        m_vecContactList[ID].UpdateMessages();

}

void CClient::InsertRecvChatMessage(SMessage msg)
{
    for (int i = 0; i < m_vecContactList.size(); i++)
    {
        if (msg.m_strSenderIP.compare(m_vecContactList[i].GetIP()) == 0)
        {
            m_vecContactList[i].InsertRecvMessage(msg.m_strMessage);
            break;
        }
    }
}








