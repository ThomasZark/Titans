#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include <codec/proto/ProtoCodec.h>

using namespace TITANS::CODEC;

TEST_CASE( "CtxProtoCodec 序列化", "[CtxProtoCodec]" ) {

    CtxProtoCodec<iLiveRequest, iLiveRequest> codec;

    codec.BodyRsp().set_version(1000);
    codec.BodyRsp().set_client_type(401);

    REQUIRE( codec.BodyRsp().version() == 1000 );
    REQUIRE( codec.BodyRsp().client_type() == 401 );
    REQUIRE( codec.BodyReq().version() == 0 );
    REQUIRE( codec.BodyReq().client_type() == 0 );

    SECTION( "CtxProtoCodec 序列化" ) {
        std::string strData;
        CHECK( codec.RspPkgEncode(strData) == 0 );
        
        CHECK( codec.BodyRsp().version() == 1000 );
        CHECK( codec.BodyRsp().client_type() == 401 );
        CHECK( codec.BodyReq().version() == 0 );
        CHECK( codec.BodyReq().client_type() == 0 );
    }
    SECTION( "CtxProtoCodec 反序列化" ) {
        std::string strData;
        REQUIRE( codec.RspPkgEncode(strData) == 0 );
        std::string originalData = strData;
        
        CHECK( codec.ReqPkgDecode(strData.c_str(), strData.length(), NULL) == 0 );
        CHECK( codec.BodyRsp().version() == 1000 );
        CHECK( codec.BodyRsp().client_type() == 401 );
        CHECK( codec.BodyReq().version() == 1000 );
        CHECK( codec.BodyReq().client_type() == 401 );        
        CHECK( originalData == strData );   
    }
}