#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include <core/codec/ProtoCodec.h>
#include "Echo.pb.h"

using namespace TITANS::CODEC;
using namespace Echo;

TEST_CASE( "ProtoCodec", "[Codec]" ) {

    ProtoCodec<EchoReq, EchoRsp> echoCodec;
    echoCodec.BodyReq().set_appid(1);
    echoCodec.BodyReq().set_data("Hello");
    echoCodec.BodyRsp().set_retcode(2);
    echoCodec.BodyRsp().set_errmsg("world");

    REQUIRE(echoCodec.BodyReq().appid() == 1);
    REQUIRE(echoCodec.BodyReq().data() == "Hello");
    REQUIRE(echoCodec.BodyRsp().retcode() == 2);
    REQUIRE(echoCodec.BodyRsp().errmsg() == "world");

    SECTION( "ProtoCodec 序列化" ) {
        std::string strReq;
        std::string strRsp;

        REQUIRE(echoCodec.ReqPkgEncode(strReq) == 0);
        REQUIRE(echoCodec.RspPkgEncode(strRsp) == 0);
        REQUIRE(echoCodec.BodyReq().appid() == 1);
        REQUIRE(echoCodec.BodyReq().data() == "Hello");
        REQUIRE(echoCodec.BodyRsp().retcode() == 2);
        REQUIRE(echoCodec.BodyRsp().errmsg() == "world");       

        ProtoCodec<EchoReq, EchoRsp> echoCodec2;
        CHECK_FALSE(echoCodec2.BodyReq().has_appid());
        CHECK_FALSE(echoCodec2.BodyReq().has_data());
        CHECK_FALSE(echoCodec2.BodyRsp().has_retcode());
        CHECK_FALSE(echoCodec2.BodyRsp().has_errmsg());

        REQUIRE(echoCodec2.ReqPkgDecode(strReq.c_str(), strReq.size(), nullptr) == 0);
        REQUIRE(echoCodec2.RspPkgDecode(strRsp.c_str(), strRsp.size(), nullptr) == 0);
        REQUIRE(echoCodec2.BodyReq().appid() == 1);
        REQUIRE(echoCodec2.BodyReq().data() == "Hello");
        REQUIRE(echoCodec2.BodyRsp().retcode() == 2);
        REQUIRE(echoCodec2.BodyRsp().errmsg() == "world");          
    }    
}