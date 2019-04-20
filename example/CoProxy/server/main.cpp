#include "CoProxyServer.h"
#include "CoTcpHandler.h"

#include <core/common/Memory.h>

int main() {

    CoTcpHandler handler(1001);
    CoProxyServer server;

    server.Initialize(nullptr);
    server.SetServerName("CoProxyServer")
            ->SetHandler(std::shared_ptr<CoTcpHandler>(&handler, TITANS::stack_delete<CoTcpHandler>));
    return server.Serve(nullptr);
}