#include "SyncServer.h"

#include <iostream>

SyncServer::SyncServer() {

}

SyncServer::~SyncServer() {

}

int SyncServer::Initialize(std::shared_ptr<TITANS::SERVER::stBaseSvrContext> pContext) {

    std::cout<<GetServerName()<<" start!"<<std::endl;
    return 0;
}

void SyncServer::Finalize(std::shared_ptr<TITANS::SERVER::stBaseSvrContext> pContext) {

    std::cout<<GetServerName()<<" end!"<<std::endl;
}

int SyncServer::Serve(std::shared_ptr<TITANS::SERVER::stBaseSvrContext> pContext) {

    return GetHandler()->HandleProcess(nullptr);
}