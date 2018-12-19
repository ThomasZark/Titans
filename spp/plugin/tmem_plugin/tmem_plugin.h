#ifndef _TITANS_SPP_PLUGIN_TMEM_PLUGIN_TMEM_PLUGIN_H_
#define _TITANS_SPP_PLUGIN_TMEM_PLUGIN_TMEM_PLUGIN_H_

#include <core/common/Singleton.h>
#include <spp/plugin/TmplPlugin.h>
#include <mt_client/tmem_mt_client.h>
#include <google/protobuf/message.h>

namespace TITANS {

namespace PLUGIN {

enum EC_TMEM_RETERN_CODE
{
    EC_TMEM_GET_ERROR = 1,
    EC_TMEM_KEY_NOT_EXIST = 2,
    EC_TMEM_DECODE_ERROR = 3,
    EC_TMEM_CAS_CONFLICT = 4,
    EC_TMEM_DATA_EMPTY = 5,
    EC_TMEM_SET_ERROR = 6,
    EC_TMEM_MAS_ERROR = 7,
    EC_TMEM_DEL_ERROR = 8
};

class TmemPlugin :public TmplPlugin {

public:
    TmemPlugin();
    
    virtual ~TmemPlugin();

    virtual int Initialize(void* arg1, void* arg2);

    bool LoadParamConfig(const libconfig::Setting & sSetting);

    int GetTmemData(MtTmemClient* mt_client, const string& key, google::protobuf::Message* message, int* cas);
    
    int SetTmemData(MtTmemClient* mt_client, const string& key, const google::protobuf::Message& message, int* cas, int expire = 0); 
    
    int DelTmemData(MtTmemClient* mt_client, const string& key);
    
    void TmemLatencyReport(uint32_t timespan);

    void Monitor(uint32_t attr, uint32_t time);

protected:
    int iMonitor;

    enum Attr_Type {
        MONITOR_TMEM_GET = 0,	                // MONITOR_TMEM_GET
        MONITOR_TMEM_GET_SUCC = 1,	            // MONITOR_TMEM_GET_SUCC
        MONITOR_TMEM_GET_ERROR = 2,	            // TMEM_GET_ERROR
        MONITOR_TMEM_GET_NOT_EXIST = 3,	        // MONITOR_TMEM_GET_NOT_EXIST
        MONITOR_TMEM_GET_DATA_EMPTY = 4,	    // MONITOR_TMEM_GET_DATA_EMPTY
        MONITOR_TMEM_SET = 5,	                // MONITOR_TMEM_SET
        MONITOR_TMEM_SET_SUCC = 6,	            // MONITOR_TMEM_SET_SUCC
        MONITOR_TMEM_SET_ERROR = 7,             // MONITOR_TMEM_SET_ERROR
        MONITOR_TMEM_CAS_CONFLICT = 8,	        // TMEM_CAS冲突
        MONITOR_CAS_RETRY_FAILED = 9,	        // TMEM重试后仍失败
        MONITOR_TMEM_DEL = 10,	                // MONITOR_TMEM_DEL
        MONITOR_TMEM_DEL_SUCC = 11,	            // MONITOR_TMEM_DEL_SUCC
        MONITOR_TMEM_DEL_ERROR = 12,	        // TMEM_DEL_ERROR
        MONITOR_TMEM_DECODE_ERROR = 13,         // TMEM_DECODE_ERROR
        MONITOR_TMEM_ENCODE_FAILED = 14,        // TMEM_ENCODE失败
        MONITOR_TMEM_SPAN_0_10 = 15,	        // TMEM_SPAN_0_10
        MONITOR_TMEM_SPAN_10_20 = 16,	        // TMEM_SPAN_10_20
        MONITOR_TMEM_SPAN_20_50 = 17,	        // TMEM_SPAN_20_50
        MONITOR_TMEM_SPAN_50_100 = 18,	        // TMEM_SPAN_50_100
        MONITOR_TMEM_SPAN_100_200 = 19,	        // TMEM_SPAN_100_200
        MONITOR_TMEM_SPAN_200_500 = 20,	        // TMEM_SPAN_200_500
        MONITOR_TMEM_SPAN_500_1000 = 21,	    // TMEM_SPAN_500_1000
        MONITOR_TMEM_SPAN_1000_2000 = 22,	    // TMEM_SPAN_1000_2000
        MONITOR_TMEM_SPAN_2000_5000 = 23,	    // TMEM_SPAN_2000_5000
        MONITOR_TMEM_SPAN_MORE_5000 = 24,	    // TMEM_SPAN_MORE_5000     
    };
};

typedef Singleton<TmemPlugin> sTmemPlugin;

}//namespace PLUGIN

}//namespace TITANS

#endif //_TITANS_SPP_PLUGIN_TMEM_PLUGIN_TMEM_PLUGIN_H_