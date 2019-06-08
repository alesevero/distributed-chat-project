//
//  main.cpp
//  Trabalho2
//
//  Created by Alexandre Severo Jr on 6/4/16.
//  Copyright © 2016 asj. All rights reserved.
//

#include "SenderServer.h"
#include "ReceiverServer.h"
#include "Client.h"

#include <iostream>

int main(int argc, const char * argv[]) {
    
    /*if (argc < 4)
    {
        std::cout << "./trab <nome> <seu ip> <porta>" << std::endl;
        return -1;
    }
    
    int nPort = atoi(argv[3]);
    std::string strIP(argv[2]);
    std::string strName(argv[1]);*/
    
    CReceiverServer *rserver = new CReceiverServer(7200);
    CSenderServer *sserver = new CSenderServer(7200, "Alexandre", "localhost");
    CClient *client = new CClient();
    
    // threads do servidor
    pthread_t ptReceiverServerThread;
    pthread_t ptSenderServerThread;
    pthread_t ptClientThread;
    
    // inicia os servidores
    pthread_create(&ptReceiverServerThread, NULL, rserver->ThreadInit, rserver);
    pthread_create(&ptSenderServerThread, NULL, sserver->ThreadInit, sserver);
    pthread_create(&ptClientThread, NULL, client->ThreadInit, client);
    
    // join das threads dos servidores
    pthread_join(ptClientThread, NULL);
    pthread_join(ptSenderServerThread, NULL);
    pthread_cancel(ptReceiverServerThread);
    
    return 0;
}
