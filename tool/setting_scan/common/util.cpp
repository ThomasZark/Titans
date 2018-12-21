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

#include "util.h"

using namespace std;  

extern void handle_line(string line);

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
        cout<<"dir_name is not a valid directory !"<<dir_name<<endl;  
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
                //cout<<m_match<<endl;
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

int reg_match_r(std::string tnow_url, const char* strPattern, std::vector<std::string>& match) {

    int start = 0;
    int length = tnow_url.length();
    string line = tnow_url;
    while(true) {
        int ret = reg_match(line, strPattern, match);
        if(ret != 0) {
            return match.size() > 0 ? 0: -1;
        }
        auto pos = line.find(match[match.size()-1]);
        start = pos + match[match.size()-1].length();
        length = line.length() - start;
        line = line.substr(start, length);
    }
}