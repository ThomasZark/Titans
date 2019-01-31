#include "SyncServer.h"

#include <iostream>

SyncServer::SyncServer() {

}

SyncServer::~SyncServer() {

}

int SyncServer::Initialize(struct stBaseSvrContext* pContext) {

    std::cout<<GetServerName()<<" start!"<<std::endl;
}

int SyncServer::Finalize(struct stBaseSvrContext* pContext) {

    std::cout<<GetServerName()<<" end!"<<std::endl;
}

int SyncServer::Serve(struct stBaseSvrContext* pContext) {

    return GetHandler()->HandleProcess(NULL);
}