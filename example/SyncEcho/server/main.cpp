#include "SyncServer.h"
#include "SyncTcpHandler.h"

int main() {

    SyncTcpHandler handler(1000);
    SyncServer server;
    
    server.Initialize(NULL);
    server.SetServerName("SyncEchoServer")->SetHandler(&handler);
    return server.Serve(NULL);
}