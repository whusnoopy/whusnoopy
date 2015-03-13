// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#include <iostream>
#include <stdlib.h>
#include <string.h>

#include "base/flags.h"

vector<FlagsInfo*>* flags_list;

FlagsInfo::FlagsInfo(string type,
                     string name,
                     string default_value,
                     string description,
                     bool optional,
                     void* reference)
  : type_(type),
    name_(name),
    default_value_(default_value),
    description_(description),
    optional_(optional),
    reference_(reference) {
  bool accepted_type = false;
  if (type == "int")
    accepted_type = true;
  else if (type == "string")
    accepted_type = true;
  else if (type == "bool")
    accepted_type = true;
  if (!accepted_type) {
    cerr << "Unsupported type: " << type << endl
         << "There is only int, string or bool be supported" << endl;
    exit(1);
  }
  if (flags_list == NULL) {
    flags_list = new vector<FlagsInfo*>;
  }
  flags_list->push_back(this);
}

void FlagsInfo::print() {
  cout << string(2, ' ') << "--" << name_ << " : " << type_;
  if (optional_)
    cout << " (Optional)";
  cout << endl << string(6, ' ') << "default value: " << default_value_ << endl
       << string(6, ' ') << "description: " << description_ << endl;
}

bool FlagsInfo::assign(const string& value) {
  char* cs;
  if (type_ == "int") {
    if (value.empty())
      return false;
    *(int*)reference_ = strtol(value.c_str(), &cs, 10);
    if (cs != value.c_str() + value.size())
      return false;
  } else if (type_ == "string") {
    *(string*)reference_ = value;
  } else if (type_ == "bool") {
    string s;
    for (int i = 0; i < value.size(); ++i)
      s += tolower(value[i]);
    if (s == "true")
      *(bool*)reference_ = true;
    else if (s == "false")
      *(bool*)reference_ = false;
    else
      return false;
  } else {
    return false;
  }
  return true;
}

static void printUsage() {
  cout << "Usage: judge [options]" << endl;
  for (int i = 0; i < flags_list->size(); ++i) {
    (*flags_list)[i]->print();
  }
}

// Extracts paramenter values from the passed-in arguments.
int parseFlags(int argc, char* argv[]) {
  if (flags_list == NULL)
    flags_list = new vector<FlagsInfo*>;
  vector<bool> assigned(flags_list->size(), false);

  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-' && argv[i][1] == '-') {
      char* cs = argv[i] + 2;
      while (*cs && *cs != '=')
        ++cs;
      string name(argv[i] + 2, cs - argv[i] - 2);
      bool found = false;
      for (int j = 0; j < flags_list->size(); ++j) {
        if ((*flags_list)[j]->name() == name) {
          if (*cs) {
            if (!(*flags_list)[j]->assign(cs + 1)) {
              cerr << "Invalid value for flag " << name << endl;
              printUsage();
              return -1;
            }
          } else {
            if ((*flags_list)[j]->type() != "bool") {
              cerr << "Missing value for flag " << name << endl;
              printUsage();
              return -1;
            }
            (*flags_list)[j]->assign("true");
          }
          found = true;
          assigned[j] = true;
          break;
        }
      }
      if (!found) {
        cerr << "Invalid flag " << name << endl;
        printUsage();
        return -1;
      }
    }
  }

  // Check optional flags
  for (int i = 0; i < flags_list->size(); ++i) {
    if (!assigned[i] && !(*flags_list)[i]->optional()) {
      cerr << "Missing flag " << (*flags_list)[i]->name() << endl;
      printUsage();
      return -1;
    }
  }
  
  // Parse process_name for log file
  for (int i = 0; i < flags_list->size(); ++i) {
    if ((*flags_list)[i]->name() == "process_name") {
      char* cs = argv[0];
      while (strstr(cs, "/"))
        cs = strstr(cs, "/") + 1;
      (*flags_list)[i]->assign(cs);
    }
  }
  return 0;
}

DEFINE_FLAGS(string, root_dir, "The root work directory");

DEFINE_OPTIONAL_FLAGS(string, process_name, "",
                      "The process name to be logged, it will automaticly "
                      "generated");

