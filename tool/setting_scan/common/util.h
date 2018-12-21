#ifndef _COMMON_UTIL_H_
#define _COMMON_UTIL_H_

#include <vector>
#include <string>

void parse_file( const char* file_name);
  
void travel_dir( const char * dir_name );  

int reg_match(std::string tnow_url, const char* strPattern, std::vector<std::string>& match);

int reg_match_r(std::string tnow_url, const char* strPattern, std::vector<std::string>& match);

#endif //_COMMON_UTIL_H_