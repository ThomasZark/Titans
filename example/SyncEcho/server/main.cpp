#include "SyncServer.h"
#include "SyncTcpHandler.h"

#include <core/common/Memory.h>

int main() {

    SyncTcpHandler handler(1000);
    SyncServer server;

    server.Initialize(nullptr);
    server.SetServerName("SyncEchoServer")->SetHandler(std::shared_ptr<SyncTcpHandler>(&handler, TITANS::stack_delete<SyncTcpHandler>));
    return server.Serve(nullptr);
}