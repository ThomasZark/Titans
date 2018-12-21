#ifndef _UTIL_H_
#define _UTIL_H_

#include <string>
#include <vector>
#include <sstream>

void UperString(std::string& name);

std::string GetClassName(const std::string& name);

void ConvertPackage(std::string& package);

int strcmpIgnoreCase(const char *a,const char *b);

bool CreateDir(const char* dir_name);

void split(const std::string& str, const std::string& separator, std::vector<std::string>& result);

template <class T>
std::string ToStr(const T& t)
{
    std::stringstream stream;
    stream << t;
    return stream.str();
}

#endif //_UTIL_H_