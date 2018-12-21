#include "util.h"

#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <sys/stat.h>
#include <string.h>

using namespace std;

void UperString(std::string& name)
{
    if (islower(name[0])) {
        char c = toupper(name[0]);
        name[0] = c;
    }

    for (size_t i = 1; i < name.size(); i++) {
        if (isupper(name[i])) {
            name.insert(i, "_");
            i++;
        }
    }

    for (size_t i = 1; i < name.size(); i++) {
        if (islower(name[i])) {
            char c = toupper(name[i]);
            name[i] = c;
        }
    }

    return;
}

string GetClassName(const string& name)
{
    string result;
    result.resize(name.size());
    result[0] = islower(name[0]) ? toupper(name[0]) : name[0];

    size_t j = 1;
    for (size_t i = 1; i < name.size(); i++) {
        if (name[i] == '_') {
            if (i < (name.size() - 1)) {
                result[j++] = toupper(name[++i]);
            }
        } else {
            result[j] = name[i];
            j++;
        }
    }
    return result.substr(0, j);
}

void ConvertPackage(string& package)
{
    string tmp;
    for (size_t i = 0; i < package.size(); i++)
    {
        if (package[i] != '.')
        {   
            tmp += package[i];
            continue;
        }
        tmp += "::";
    }
    package = tmp; 
    return;
}

int strcmpIgnoreCase(const char *a,const char *b)
{
  while( *a && *b )
  {
      if (0 == isalpha(*a))
      {
          ++a;
          continue;
      }

      if (0 == isalpha(*b))
      {
          ++b;
          continue;
      }

      int r=tolower(*a)-tolower(*b);
      if( r )
          return r;
      ++a;
      ++b;
  }

  while(*a)
  {
      if (0 != isalpha(*a))
      {
          return 1;
      }
      ++a;
  }

  while(*b)
  {
      if (0 != isalpha(*b))
      {
          return 1;
      }
      ++b;
  }

  return 0;
#if 0
  if (0 == isalpha(*a) && 0 == isalpha(*b))
  {
      return 0;
  }

  return tolower(*a)-tolower(*b);
#endif
}

void split(const string& str, const string& separator, std::vector<string>& result)
{
    size_t start = 0;
    size_t end = 0;

    while ((end = str.find(separator, start) ) != string::npos) {
        string tmp = str.substr(start, end - start);
        if (!tmp.empty()) {
            result.push_back(str.substr(start, end - start));
        }
        start = end + 1;
    }

    string last = str.substr(start);
    if (!last.empty()) {
        result.push_back(last);
    }

    return;
}

bool CreateDir(const char* dir_name)
{
    std::vector<string> dirs;
    split(dir_name, "/", dirs);

    string dir = "/";
    for (size_t i = 0; i < dirs.size(); i++) {
        dir += dirs[i];
        dir += "/";
        int status = access(dir.c_str(), F_OK);
        if (0 != status) {
            if ((ENOTDIR != errno) && (ENOENT != errno)) {
                cout << "access dir failed,err=" << errno << ",errmsg=" << strerror(errno) << endl;
                return false;
            }

            status = mkdir(dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IWOTH);
            if (0 != status) {
                cout << "mkdir failed,err=" << errno << ",errmsg=" << strerror(errno) << endl;
                return false;
            }
        }
    }

    return true;
}

