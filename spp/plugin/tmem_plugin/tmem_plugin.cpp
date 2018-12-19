#include <syncincl.h>

#include <Attr_API.h>
#include <core/plugin/PluginMgr.h>
#include <spp/plugin/tmem_plugin/tmem_plugin.h>

using namespace google::protobuf;
using namespace TITANS::PLUGIN;

TmemPlugin::TmemPlugin() {

}

TmemPlugin::~TmemPlugin() {

}

int TmemPlugin::Initialize(void* arg1, void* arg2) {

	const char* etc = static_cast<char*>(arg1);
	CServerBase* base = static_cast<CServerBase*>(arg2);
    if (base->servertype() == SERVER_TYPE_WORKER) {
        if(!LoadConfig(etc)) {
        	return -10002;
    	}
    }
	_ssLog << "TmemPlugin init succ" << std::endl;
    return 0;  
}

void TmemPlugin::TmemLatencyReport(uint32_t timespan) {
    timespan < 10 ? Monitor(MONITOR_TMEM_SPAN_0_10, 1) ://Tmem延时10ms以内
    timespan < 20 ? Monitor(MONITOR_TMEM_SPAN_10_20, 1) ://Tmem延时10-20ms
    timespan < 50 ? Monitor(MONITOR_TMEM_SPAN_20_50, 1) ://Tmem延时20-50ms
    timespan < 100 ? Monitor(MONITOR_TMEM_SPAN_50_100, 1) ://Tmem延时50-100ms
    timespan < 200 ? Monitor(MONITOR_TMEM_SPAN_100_200, 1) ://Tmem延时100-200ms
    timespan < 500 ? Monitor(MONITOR_TMEM_SPAN_200_500, 1) ://Tmem延时200-500ms
    timespan < 1000 ? Monitor(MONITOR_TMEM_SPAN_500_1000, 1) ://Tmem延时500-1000ms
    timespan < 2000 ? Monitor(MONITOR_TMEM_SPAN_1000_2000, 1) ://Tmem延时1000-2000ms
    timespan < 5000 ? Monitor(MONITOR_TMEM_SPAN_2000_5000, 1) : //Tmem延时2000-5000ms
    Monitor(MONITOR_TMEM_SPAN_MORE_5000, 1);//Tmem延时大于5000ms
}

int TmemPlugin::GetTmemData(MtTmemClient* mt_client, const string& key, Message* message, int* cas) {
    Monitor(MONITOR_TMEM_GET, 1); //Tmem_Get请求
    string data;
    int err = mt_client->Get(key, data, *cas);
    TmemLatencyReport(mt_client->time_cost());
    if (MtTmemClient::TMEM_MT_EC_KEY_NOT_EXIST == err) {
        Monitor(MONITOR_TMEM_GET_NOT_EXIST, 1); //Tmem_Get数据不存在
        SF_LOG(LOG_ERROR, "tmem key is not exist|key=%s|ret=%d|", key.c_str(), err);
        return EC_TMEM_KEY_NOT_EXIST;
    }
    if (0 != err) {
        Monitor(MONITOR_TMEM_GET_ERROR, 1); //Tmem_Get错误
        SF_LOG(LOG_ERROR, "get data failed|key=%s|ret=%d|err_msg=%s|", key.c_str(), err, mt_client->get_errmsg().c_str()); 
        return EC_TMEM_GET_ERROR;
    }
    if (data.empty()) {
        Monitor(MONITOR_TMEM_GET_DATA_EMPTY, 1); //Tmem_Get数据为空
        SF_LOG(LOG_ERROR, "tmem data is empty|key=%s|", key.c_str());
        return EC_TMEM_DATA_EMPTY;
    }
    if (!message->ParseFromString(data)){
        Monitor(MONITOR_TMEM_DECODE_ERROR, 1); //Tmem_decode错误
        SF_LOG(LOG_ERROR, "parse tmem data failed|key=%s|err=%s|", key.c_str(), message->InitializationErrorString().c_str());
        return EC_TMEM_DECODE_ERROR;
    }
    Monitor(MONITOR_TMEM_GET_SUCC, 1); //Tmem_Get成功
    return 0;
} 

int TmemPlugin::SetTmemData(MtTmemClient* mt_client, const string& key, const Message& message, int* cas, int expire) {
    Monitor(MONITOR_TMEM_SET, 1); //Tmem_Set请求
    string data;
    if (!message.SerializeToString(&data))
    {
        Monitor(MONITOR_TMEM_ENCODE_FAILED, 1);//Tmem_Set_encode错误
        SF_LOG(LOG_ERROR, "serializetoString failed|key=%s|", key.c_str());
        return EC_TMEM_MAS_ERROR;
    }

    int err = mt_client->CasSet(key, data, *cas, expire);
    TmemLatencyReport(mt_client->time_cost());
    if (0 == err)
    {
        Monitor(MONITOR_TMEM_SET_SUCC, 1); //Tmem_Set成功
        SF_LOG(LOG_DEBUG, "set tmem info success|key=%s|", key.c_str());
        return 0;
    }

    if (-13104 != err)
    {
        Monitor(MONITOR_TMEM_SET_ERROR, 1);//Tmem_Set错误
        SF_LOG(LOG_ERROR, "set tmem failed|key=%s|err=%d|err_msg=%s|", key.c_str(), err, mt_client->get_errmsg().c_str());
        return EC_TMEM_SET_ERROR;
    }

    Monitor(MONITOR_TMEM_CAS_CONFLICT, 1);//Tmem_Set_Cas冲突
    SF_LOG(LOG_ERROR, "cas conflict|key=%s|", key.c_str());
    return EC_TMEM_CAS_CONFLICT;
}

int TmemPlugin::DelTmemData(MtTmemClient* mt_client, const string& key) {
    Monitor(MONITOR_TMEM_DEL, 1); //Tmem_Del请求
    int err = mt_client->Del(key);
    TmemLatencyReport(mt_client->time_cost());
    if (0 == err){
        Monitor(MONITOR_TMEM_DEL_SUCC, 1); //Tmem_Del成功
        SF_LOG(LOG_DEBUG, "del tmem info success|key=%s|", key.c_str());
        return 0;
    }

    if (-13105 != err){
        Monitor(MONITOR_TMEM_DEL_ERROR, 1);//Tmem_Del错误
        SF_LOG(LOG_ERROR, "del tmem failed|key=%s|err=%d|", key.c_str(), err);
        return EC_TMEM_DEL_ERROR;
    }

    Monitor(MONITOR_TMEM_DEL_SUCC, 1); 
    SF_LOG(LOG_DEBUG, "del key not exist|key=%s|", key.c_str());
    return 0;
}

void TmemPlugin::Monitor(uint32_t attr, uint32_t time) {

	Attr_API(iMonitor + attr, time);
}

bool TmemPlugin::LoadParamConfig(const libconfig::Setting & sSetting) {

	LookupValueWithDefault(sSetting, "iMonitor", iMonitor, 33643018);
    return true;
}

REGIST_PLUGIN(TmemPlugin)