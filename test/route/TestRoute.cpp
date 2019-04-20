#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include <core/route/IPRoute.h>

using namespace TITANS::ROUTE;

TEST_CASE( "IPRoute", "[Route]" ) {

    struct stRoute_t route;
    route.ip = "";
    route.port = 0;

    REQUIRE(route.ip == "");
    REQUIRE(route.port == 0);

    SECTION( "IPRoute GetRoute" ) {

        IPRoute iproute;
        iproute.SetRouteKey("127.0.0.0:10086");
        REQUIRE( iproute.GetRoute(route) == ROUTE_SUCC);
        REQUIRE( route.ip == "127.0.0.0");
        REQUIRE( route.port == 10086);
    }

    SECTION( "IPRoute GetRoute Key Excption" ) {

        IPRoute iproute;
        iproute.SetRouteKey("127.0.0.0,10086");
        REQUIRE( iproute.GetRoute(route) == ROUTE_KEY_ERROR);
    }

    SECTION( "IPRoute GetRoute port Exception" ) {

        IPRoute iproute;
        iproute.SetRouteKey("127.0.0.0:");
        REQUIRE( iproute.GetRoute(route) == ROUTE_SUCC);
        REQUIRE( route.ip == "127.0.0.0");
        REQUIRE( route.port == 0);
    }
}