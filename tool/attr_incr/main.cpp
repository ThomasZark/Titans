#include <iostream>
#include <fstream>  
#include <sstream>
#include <regex.h>
#include <algorithm> 
#include <stdio.h>  
#include <unistd.h>  
#include <dirent.h>  
#include <stdlib.h>  
#include <sys/stat.h>   
#include <string.h>  
#include <gflags/gflags.h>
#include <unordered_map>

using namespace std;  

enum state {
    kNormal = 1,
    kServer = 2,
    kFake = 3,
    kMsg = 4,
    kSubcmd = 5,
};

enum mode {
    kSource = 1,
    kAttr = 2,
    kDesc = 3,
};

struct stAttr_t {
    uint32_t type;
    std::string desc;
};

unordered_map<string, stAttr_t> source_attr;
unordered_map<string, stAttr_t> attr_attr;

int g_status;
int g_mode;

ofstream g_desc;

DEFINE_string(desc_file, "", "增加的monitor desc");
DEFINE_string(cur_file, "", "attr_define.h");

int reg_match(string tnow_url, const char* strPattern, std::vector<std::string>& match) { 

    char acErrBuf[256];
    int iRet=-1;
    regex_t tReg;    //定义一个正则实例
    const char *pStrBuf = tnow_url.c_str();   //定义待匹配串
    const size_t dwMatch = 5;    //定义匹配结果最大允许数        //表示允许几个匹配
    regmatch_t atMatch[5];   //定义匹配结果在待匹配串中的下标范围    //存储匹配串位置的数组
    //数组0单元存放主正则表达式位置，后边的单元依次存放子正则表达式位置

    //REG_ICASE 匹配字母时忽略大小写。
    iRet =regcomp(&tReg, strPattern, REG_EXTENDED);    //编译正则模式串
    if(iRet != 0) 
    {
        regerror(iRet, &tReg, acErrBuf, sizeof(acErrBuf));
        cout<<"reg_match Regex Error="<<acErrBuf<<",pattern="<<strPattern<<endl;
        return -1;
    }
    else
    {
        iRet = regexec(&tReg, pStrBuf, dwMatch, atMatch, 0); //匹配他
        if (iRet == REG_NOMATCH)
        { //如果没匹配上
            //cout<<"reg_match not match Error"<<",pattern="<<strPattern<<endl;
            return -1;
        }
        else if (iRet == REG_NOERROR)
        {  //如果匹配上了
            for(uint32_t i = 0; i< dwMatch && atMatch[i].rm_so != -1; i++) {
                std::string m_match = tnow_url.substr(atMatch[i].rm_so, (atMatch[i].rm_eo - atMatch[i].rm_so));
                match.push_back(m_match);
            }
            return 0;
        }
        else
        {
            return -1;
        }
        regfree(&tReg);  //释放正则表达式
    }
}

int match_desc(string tnow_url,std::string& match) {

    auto iter = tnow_url.find("//");
    if(iter == string::npos) {
        return -1;
    }
    if(iter == tnow_url.length() -2) {
        return -1;
    }
    match = tnow_url.substr(iter + 2, tnow_url.length());
    return 0;

}

void handle_source_normal(string line) {

    std::vector<std::string> match;
    int ret = reg_match(line, "Attr_API\\(([a-z|A-Z|0-9|_-]+),", match);
    if(ret == 0) {
        std::string desc;
        stAttr_t attr;
        attr.desc = match[1];
        ret = match_desc(line, desc);
        if(ret == 0) {
    	    attr.desc = desc;
        }
        attr.type = kNormal;
        source_attr[match[1]] = attr;
        return;
    }
    
    ret = reg_match(line, "//server", match);
    if(ret == 0) {
        g_status = kServer;
        return;
    }

    ret = reg_match(line, "//IliveMsgFactory", match);
    if(ret == 0) {
        g_status = kMsg;
        return;
    }

    ret = reg_match(line, "//LogicMonitor", match);
    if(ret == 0) {
        g_status = kSubcmd;
        return;
    }
}

void handle_source_server(string line) {

    std::vector<std::string> match;
    int ret = reg_match(line, "MONITOR_([a-z|A-Z|0-9|_-]+)", match);
    if(ret == 0) {
        stAttr_t attr;
        attr.type = kServer;
        source_attr[match[0]] = attr;
    }
    g_status = kNormal;
}

void handle_source_msg(string line) {

    std::vector<std::string> match;
    int ret = reg_match(line, "MONITOR_([a-z|A-Z|0-9|_-]+)", match);
    if(ret == 0) {
        stAttr_t attr;
        attr.type = kMsg;
        source_attr[match[0]] = attr;
    }
    g_status = kNormal;
}


void handle_source_subcmd(string line) {

    std::vector<std::string> match;
    int ret = reg_match(line, "MONITOR_([a-z|A-Z|0-9|_-]+)", match);
    if(ret == 0) {
        stAttr_t attr;
        attr.type = kSubcmd;
        source_attr[match[0]] = attr;
    }
    g_status = kNormal;
}

void handle_attr(string line) {

    std::vector<std::string> match;
    int ret = reg_match(line, "#define ([a-z|A-Z|0-9|_-]+)([^0-9]+)([0-9]+)", match);
    if(ret == 0) {
        stAttr_t attr;
        attr.type = strtoul(match[3].c_str(), NULL, 10);
        attr.type = attr.type < 1000 ? kFake : kNormal;
        attr_attr[match[1]] = attr;
    }
}

void handle_desc(string line) {

    std::vector<std::string> match;
    int ret = reg_match(line, "#define ([a-z|A-Z|0-9|_-]+)([^0-9]+)([0-9]+)", match);
    if(ret == 0) {
        int attr;
        attr = strtoul(match[3].c_str(), NULL, 10);
        if(attr < 1000) {
            std::string desc;
            g_desc<<"0|"<<match[1]<<"|";
            ret = match_desc(line, desc);
            if(ret == 0) {
                g_desc<<desc<<endl;
            }
            else {
                g_desc<<match[1]<<endl;
            }
        }
    }  
}

void handle_source(string line) {
    switch(g_status) {
        case kNormal:
            return handle_source_normal(line);
        case kServer:
            return handle_source_server(line);
        case kMsg:
            return handle_source_msg(line);
        case kSubcmd:
            return handle_source_subcmd(line);
    }
}

void handle_line(string line) {
    switch(g_mode) {
        case kSource:
            return handle_source(line);
        case kAttr:
            return handle_attr(line);
        case kDesc:
            return handle_desc(line);
    }
}

void parse_file( const char* file_name) {

    if( NULL == file_name )  {  
        cout<<" file_name is null ! "<<file_name<<endl;  
        return;  
    }      
    cout<<"parse_file "<<file_name<<endl;

    std::ifstream fin(file_name, std::ios::in);
    char line[1024]={0};
    while(fin.getline(line, sizeof(line))) {
        handle_line(line);
    }
    fin.clear();
    fin.close();
}
  
void travel_dir( const char * dir_name ) {  
    
    if( NULL == dir_name )  {  
        cout<<" dir_name is null ! "<<endl;  
        return;  
    }  

    struct stat s;  
    lstat( dir_name , &s );  
    if( ! S_ISDIR( s.st_mode ) ) {  
        cout<<"dir_name is not a valid directory !"<<endl;  
        return;  
    }  
      
    struct dirent * filename;    // return value for readdir()  
    DIR * dir;                   // return value for opendir()  
    dir = opendir( dir_name );  
    if( NULL == dir ) {  
        cout<<"Can not open dir "<<dir_name<<endl;  
        return;  
    }   
      
    while( ( filename = readdir(dir) ) != NULL )  {  
        if( strcmp( filename->d_name , "." ) == 0 ||    
            strcmp( filename->d_name , "..") == 0 ) {  
            continue;  
        }
        else if(filename->d_type == 8) {    ///file
            stringstream ss;
            ss<<dir_name<<"/"<<filename->d_name;
            parse_file(ss.str().c_str());
        }
        else if(filename->d_type == 4) {    ///dir
            stringstream ss;
            ss<<dir_name<<"/"<<filename->d_name;
            travel_dir(ss.str().c_str());
        }
        else {
           cout<<"wrong type"<<filename->d_name <<endl;
        }
    }  
}   

void print_desc() {

    g_desc.open(FLAGS_desc_file.c_str());
        g_desc<<"#ATTR_ID|MICRO|DESC"<<endl;
	g_mode = kDesc;
    parse_file(FLAGS_cur_file.c_str());

    g_desc.close();
}

void append_fake_monitor() {

    std::ifstream in(FLAGS_cur_file.c_str());
    std::ostringstream tmp;
    tmp << in.rdbuf();
    std::string str = tmp.str();
    in.close();

    auto pos = str.find("#endif");

    ofstream attr_file(FLAGS_cur_file.c_str(), ios_base::trunc | ios_base::out);
    if (!attr_file.is_open()){
        cout<<"open file failed, "<<FLAGS_cur_file<<endl;
        return;
    }

    std::string start = str.substr(0, pos);
    std::string end = str.substr(pos, str.length());

    attr_file<<start;

    for(auto iter = source_attr.begin(); iter != source_attr.end(); iter ++) {
        auto exist = attr_attr.find(iter->first);
        if(exist == attr_attr.end()) { //attr_define 中不存在
            if(iter->second.type == kServer) {
                attr_file<<"#define "<<iter->first<<" 0 //"<<iter->first<<"_ENCODE"<<endl;
                attr_file<<"#define "<<iter->first<<"_L5 0 //"<<iter->first<<"_L5"<<endl;
                attr_file<<"#define "<<iter->first<<"_NET 0 //"<<iter->first<<"_NET"<<endl;
                attr_file<<"#define "<<iter->first<<"_DECODE 0 //"<<iter->first<<"_DECODE"<<endl;
                attr_file<<"#define "<<iter->first<<"_TIME_0_10 0 //"<<iter->first<<"_TIME_0_10"<<endl;
                attr_file<<"#define "<<iter->first<<"_TIME_10_50 0 //"<<iter->first<<"_TIME_10_50"<<endl;
                attr_file<<"#define "<<iter->first<<"_TIME_50_100 0 //"<<iter->first<<"_TIME_50_100"<<endl;
                attr_file<<"#define "<<iter->first<<"_TIME_100_500 0 //"<<iter->first<<"_TIME_100_500"<<endl;
                attr_file<<"#define "<<iter->first<<"_TIME_500_1000 0 //"<<iter->first<<"_TIME_500_1000"<<endl;
                attr_file<<"#define "<<iter->first<<"_TIME_1000_2000 0 //"<<iter->first<<"_TIME_1000_2000"<<endl;
                attr_file<<"#define "<<iter->first<<"_TIME_2000_5000 0 //"<<iter->first<<"_TIME_2000_5000"<<endl;
                attr_file<<"#define "<<iter->first<<"_TIME_MORE_5000 0 //"<<iter->first<<"_TIME_MORE_5000"<<endl;
            }
        }
    }
    for(auto iter = source_attr.begin(); iter != source_attr.end(); iter ++) {
        auto exist = attr_attr.find(iter->first);
        if(exist == attr_attr.end()) { //attr_define 中不存在
            if(iter->second.type == kSubcmd) {
                attr_file<<"#define "<<iter->first<<" 0 //"<<iter->first<<"_SPAN_0_10"<<endl;
                attr_file<<"#define "<<iter->first<<"_SPAN_10_20 0 //"<<iter->first<<"_SPAN_10_20"<<endl;
                attr_file<<"#define "<<iter->first<<"_SPAN_20_50 0 //"<<iter->first<<"_SPAN_20_50"<<endl;
                attr_file<<"#define "<<iter->first<<"_SPAN_50_100 0 //"<<iter->first<<"_SPAN_50_100"<<endl;
                attr_file<<"#define "<<iter->first<<"_SPAN_100_200 0 //"<<iter->first<<"_SPAN_100_200"<<endl;
                attr_file<<"#define "<<iter->first<<"_SPAN_200_500 0 //"<<iter->first<<"_SPAN_200_500"<<endl;
                attr_file<<"#define "<<iter->first<<"_SPAN_500_1000 0 //"<<iter->first<<"_SPAN_500_1000"<<endl;
                attr_file<<"#define "<<iter->first<<"_SPAN_1000_2000 0 //"<<iter->first<<"_SPAN_1000_2000"<<endl;
                attr_file<<"#define "<<iter->first<<"_SPAN_SPAN_2000_5000 0 //"<<iter->first<<"_SPAN_SPAN_2000_5000"<<endl;
                attr_file<<"#define "<<iter->first<<"_SPAN_SPAN_MORE_5000 0 //"<<iter->first<<"_SPAN_SPAN_MORE_5000"<<endl;
            }
        }
    }
    for(auto iter = source_attr.begin(); iter != source_attr.end(); iter ++) {
        auto exist = attr_attr.find(iter->first);
        if(exist == attr_attr.end()) { //attr_define 中不存在
            switch(iter->second.type) {
                case kNormal:
                    attr_file<<"#define "<<iter->first<<" 0 //"<<iter->second.desc<<endl;
                    break;
                case kServer:
                    break;
            }
        }
    }
    attr_file<<endl<<end;
    attr_file.close();
}
  
int main(int argc, char** argv)  {  

    google::ParseCommandLineFlags(&argc, &argv, true);
    g_mode = kSource;
    g_status = kNormal;

    travel_dir("./");
    g_mode = kAttr;

    parse_file(FLAGS_cur_file.c_str());

    append_fake_monitor();
    print_desc(); //生成批量申请文件，会覆盖之前

    return 0;  
} 
