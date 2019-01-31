#include "SyncServer.h"
#include "SyncTcpHandler.h"

int main() {

    SyncTcpHandler handler(1000);
    SyncServer server;

    server.SetServerName("SyncEchoServer")->SetHandler(&handler);
    return server.Serve();
}