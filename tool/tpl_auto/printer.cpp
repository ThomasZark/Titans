#include "printer.h"
#include "common.h"
#include "util.h"

#include <iostream>
#include <fstream>

using namespace ctemplate;
using namespace google::protobuf;
using namespace google::protobuf::compiler;
using namespace std;

void Print(std::string source_file, TemplateDictionary& dict, std::string out_file)
{
    std::string output;
    ctemplate::ExpandTemplate(source_file , ctemplate::DO_NOT_STRIP, &dict, &output);
    ofstream of(out_file);
    of << output;
    of.close();
}