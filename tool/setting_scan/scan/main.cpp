#include "common/util.h"
#include <iostream>
#include <fstream>   
#include <sstream>
#include <algorithm> 
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <gflags/gflags.h>
#include <unordered_map>
#include <map>
#include <libconfig.h++>

//#include "lib/stl_ext/order_map"

using namespace std;  

enum state {
    kNormal = 1,
    kServer = 2,
    kFake = 3,
};

enum mode {
    kSource = 1,
    kSetting = 2,
};

struct stSetting_t {
    uint32_t type;
    std::vector<string> sub_setting;
};

struct stKey_t {
    std::string key;
    int count;
    friend bool operator < (const stKey_t& a, const stKey_t& b){
        return a.count < b.count;
    }
};

int gCount = 0;

struct stKey_comp {
    bool operator() (const stKey_t& a, const stKey_t& b) const{
        return a.count < b.count;
    }
};

map<string, int>key_count;
map<stKey_t, stSetting_t> source_setting;
unordered_map<string, stSetting_t> setting_setting; 
unordered_map<string, string> type_map; 

stKey_t make_key(std::string& key) {
    stKey_t k;
    k.key = key;
    auto iter = key_count.find(key);
    if(iter != key_count.end()) {
        k.count = iter->second;
        return k;
    }
    k.count = gCount++;
    key_count[key] = k.count;
    return k;
}


int g_status;
int g_mode;
string cur_svr;
stSetting_t cur_svr_setting;

DEFINE_string(setting_cpp, "", "增加的monitor desc");
DEFINE_string(setting_h, "", "attr_define.h");
DEFINE_string(prefix, "gSetting.", "前缀");
DEFINE_string(conf_file, "", "svr.conf");

void handle_source_normal(string line) {

    std::vector<std::string> match;
    std::string pattern = FLAGS_prefix + "([a-z|A-Z|0-9|_-]+)";
    int ret = reg_match_r(line, pattern.c_str(), match);
    if(ret == 0) {
        for(uint32_t index =0 ; index < match.size(); index ++) {
            if(index % 2 == 0) {
                continue;
            }
            stSetting_t attr;
            attr.type = kNormal;
            stKey_t k = make_key(match[index]);
            source_setting[k] = attr;
        }
        return;
    }
    
    ret = reg_match(line, "//server", match);
    if(ret == 0) {
        g_status = kServer;
        return;
    }
}

void handle_source_server(string line) {

    std::vector<std::string> match;
    int ret = reg_match(line, "C([a-z|A-Z|0-9|_-]+)Init", match);
    if(ret == 0) {
        stSetting_t attr;
        attr.type = kServer;

        std::vector<std::string> sub_match;
        std::string pattern = FLAGS_prefix + "([a-z|A-Z|0-9|_-]+)";
        int ret = reg_match_r(line, pattern.c_str(), sub_match);
        if(ret == 0) {
            for(uint32_t index =0 ; index < sub_match.size(); index ++) {
                if(index % 2 == 0) {
                    continue;
                }
                attr.sub_setting.push_back(sub_match[index]);
            }
        }
        stKey_t k = make_key(match[1]);
        source_setting[k] = attr;
    }
    g_status = kNormal;
}

void handle_setting_normal(string line) {

    std::vector<std::string> match;
    int ret = reg_match(line, "LOOK_UP_VALUE\\(([a-z|A-Z|0-9|_-]+)\\)", match);
    if(ret == 0) {
        stSetting_t attr;
        attr.type = kNormal;
        setting_setting[match[1]] = attr;
        return;
    }
    ret = reg_match(line, "//server ([a-z|A-Z|0-9|_-]+)", match);
    if(ret == 0) {
        g_status = kServer;
        cur_svr = match[1];
        cur_svr_setting.type = kServer;
        cur_svr_setting.sub_setting.clear();
        return;
    }
}

void handle_setting_server(string line) {

    std::vector<std::string> match;
    int ret = reg_match(line, "LOOK_UP_VALUE\\(([a-z|A-Z|0-9|_-]+)\\)", match);
    if(ret == 0) {
        cur_svr_setting.sub_setting.push_back(match[1]);
        return;
    }
    ret = reg_match(line, "//end server", match);
    if(ret == 0) {
        setting_setting[cur_svr] = cur_svr_setting;
        g_status = kNormal;
        return;
    }
}

void handle_setting(string line) {
    switch(g_status) {
        case kNormal:
            return handle_setting_normal(line);
        case kServer:
            return handle_setting_server(line);
    }
}

void handle_source(string line) {
    switch(g_status) {
        case kNormal:
            return handle_source_normal(line);
        case kServer:
            return handle_source_server(line);
    }
}

void handle_line(string line) {
    switch(g_mode) {
        case kSource:
            return handle_source(line);
        case kSetting:
            return handle_setting(line);
    }
}

void init_type_map() {

    type_map["i"] = "int";
    type_map["ui"]  = "uint32_t";
    type_map["b"] = "bool";
    type_map["str"] = "std::string";
    type_map["ul"] = "unsigned long";
    type_map["ull"] = "unsigned long long";
}

int type_of_setting(string setting, string& type) {

    std::vector<std::string> match;
    int ret = reg_match(setting, "[a-z]+", match);
    if(ret == 0) {
        auto iter = type_map.find(match[0]);
        if(iter != type_map.end()) {
            type =  iter->second;
            return 0;
        }
    }
    return -1;
}

void print_declare() {

    std::ifstream in(FLAGS_setting_h.c_str());
    std::ostringstream tmp;
    tmp << in.rdbuf();
    std::string str = tmp.str();
    in.close();

    auto pos = str.find("//@auto setting");
    pos += sizeof("//@auto setting");

    ofstream declare_file(FLAGS_setting_h.c_str());
    if (!declare_file.is_open()){
        cout<<"open file failed, "<<FLAGS_setting_h<<endl;
        return;
    }

    std::string start = str.substr(0, pos);
    std::string end = str.substr(pos, str.length());

    declare_file<<start;

    std::string type;
    for(auto iter = source_setting.begin(); iter != source_setting.end(); iter ++) {
        auto exist = setting_setting.find(iter->first.key);
        if(exist == setting_setting.end()) {
            switch(iter->second.type) {
                case kNormal:
                    if(type_of_setting(iter->first.key, type) == 0) {
                        declare_file<<"\t"<<type<<" "<<iter->first.key<<";"<<endl;
                    }
                    break;
                case kServer:
                    declare_file<<endl<<"\t//server "<<iter->first.key<<endl;
                    for(auto index = iter->second.sub_setting.begin(); index != iter->second.sub_setting.end(); index++) {
                        if(type_of_setting(*index, type) == 0) {
                            declare_file<<"\t"<<type<<" "<<*index<<";"<<endl;
                        }
                    }
                    declare_file<<"\t//end server"<<endl<<endl;
                    break;
            }
        }
    }
    declare_file<<endl<<end;
    declare_file.close();
}

void print_definite() {

    std::ifstream in(FLAGS_setting_cpp.c_str());
    std::ostringstream tmp;
    tmp << in.rdbuf();
    std::string str = tmp.str();
    in.close();

    auto pos = str.find("//@auto setting");
    pos += sizeof("//@auto setting");

    ofstream definite_file(FLAGS_setting_cpp.c_str(), ios_base::trunc | ios_base::out);
    if (!definite_file.is_open()){
        cout<<"open file failed, "<<FLAGS_setting_cpp<<endl;
        return;
    }

    std::string start = str.substr(0, pos);
    std::string end = str.substr(pos, str.length());

    definite_file<<start;

    std::string type;
    for(auto iter = source_setting.begin(); iter != source_setting.end(); iter ++) {
        auto exist = setting_setting.find(iter->first.key);
        if(exist == setting_setting.end()) { //attr_define 中不存在
            switch(iter->second.type) {
                case kNormal:
                    if(type_of_setting(iter->first.key, type) == 0) {
                        definite_file<<"\t\tLOOK_UP_VALUE("<<iter->first.key<<");"<<endl;
                    }
                    break;
                case kServer:
                    definite_file<<endl<<"\t\t//server "<<iter->first.key<<endl;
                    for(auto index = iter->second.sub_setting.begin(); index != iter->second.sub_setting.end(); index++) {
                        if(type_of_setting(*index, type) == 0) {
                            definite_file<<"\t\tLOOK_UP_VALUE("<<*index<<");"<<endl;
                        }
                    }
                    definite_file<<"\t\t//end server"<<endl<<endl;
                    break;
            }
        }
    }
    definite_file<<endl<<end;
    definite_file.close();
}

void print_conf() {

    try {
        libconfig::Config m_oCfg;
        m_oCfg.readFile(FLAGS_conf_file.c_str());
        libconfig::Setting &root = m_oCfg.getRoot();

        std::string type;
        for(auto iter = source_setting.begin(); iter != source_setting.end(); iter ++) {
            auto exist = setting_setting.find(iter->first.key);
            if(exist == setting_setting.end()) { //attr_define 中不存在
                switch(iter->second.type) {
                    case kNormal:
                        if(type_of_setting(iter->first.key, type) == 0) {
                            if(type == "int" || type == "uint32_t" || type == "unsigned long" || type == "unsigned long long") {
                                root.add(iter->first.key, libconfig::Setting::TypeInt)=0;
                            }
                            else if(type == "unsigned long long") {
                                root.add(iter->first.key, libconfig::Setting::TypeInt64)=0L;
                            }
                            else if(type == "std::string") {
                                root.add(iter->first.key, libconfig::Setting::TypeString)="";
                            }
                            else if(type == "bool") {
                                root.add(iter->first.key, libconfig::Setting::TypeBoolean)=false;
                            }
                        }
                        break;
                    case kServer:
                        for(auto index = iter->second.sub_setting.begin(); index != iter->second.sub_setting.end(); index++) {
                            if(type_of_setting(*index, type) == 0) {
                                if(type == "int" || type == "uint32_t" || type == "unsigned long") {
                                    root.add(*index, libconfig::Setting::TypeInt)=0;
                                }
                                else if(type == "unsigned long long") {
                                    root.add(*index, libconfig::Setting::TypeInt64)=0L;
                                }
                                else if(type == "std::string") {
                                    root.add(*index, libconfig::Setting::TypeString)="";
                                }
                                else if(type == "bool") {
                                    root.add(*index, libconfig::Setting::TypeBoolean)=false;
                                }
                            }
                        }
                        break;
                }
            }
        }
        m_oCfg.writeFile(FLAGS_conf_file.c_str());
        
    } catch (const libconfig::ParseException &e) {
        cout<<"parse failed." << e.what();
        return;
    } catch (const libconfig::SettingException &e) {
        cout<< "setting "<<e.what();
    } catch (const std::exception &e){
        cout<< "error=%s." << e.what();
		return;
	}
}
  
int main(int argc, char** argv)  {  

    google::ParseCommandLineFlags(&argc, &argv, true);
    g_mode = kSource;
    g_status = kNormal;

    travel_dir("./");
    g_mode = kSetting;
    g_status = kNormal;

    parse_file(FLAGS_setting_cpp.c_str());

    init_type_map();
    print_declare();
    print_definite();
    print_conf();

    return 0;  
} 