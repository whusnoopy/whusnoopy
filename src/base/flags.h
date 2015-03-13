// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#ifndef _BASE_FLAGS_H__
#define _BASE_FLAGS_H__

#include <string>
#include <vector>

using namespace std;

class FlagsInfo {
  public :
    FlagsInfo(string type,
              string name,
              string default_value,
              string description,
              bool optional,
              void* reference);
    void print();
    bool assign(const string& value);

    const string& type() { return type_; }
    const string& name() { return name_; }
    const string& default_value() { return default_value_; }
    bool optional() { return optional_; }

  private :
    string type_;
    string name_;
    string default_value_;
    string description_;
    bool optional_;
    void* reference_;
};

#define DEFINE_FLAGS(type, name, description) \
  type FLAGS_##name; \
  FlagsInfo _flags_##name(#type, #name, "", description, false, \
                          &FLAGS_##name)

#define DEFINE_OPTIONAL_FLAGS(type, name, default_value, description) \
  type FLAGS_##name = default_value; \
  FlagsInfo _flags_##name(#type, #name, #default_value, description, true, \
                          &FLAGS_##name)

#define DECLARE_FLAGS(type, name) \
  extern type FLAGS_##name

// Extracts paramenter values from the passed-in arguments.
int parseFlags(int argc, char* argv[]);

#endif

