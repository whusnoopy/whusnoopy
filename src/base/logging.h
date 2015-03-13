// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#ifndef _BASE_LOGGING_H__
#define _BASE_LOGGING_H__

#include <string>
#include <sstream>

using namespace std;

const int MAX_LOG_FILE_SIZE = 256 * 1024;

#define LOG(level) Log(__FILE__, __LINE__, level).stream()

enum LOG_LEVEL {
  SYS_ERROR = 0, ERROR, WARNING, FAULT, INFO, DEBUG
};

class Log {
  public :
    Log(const char* filename, int line_number, int level);
    ~Log();

    ostream& stream() { return log_stream_; }

  private :
    int level_;
    ostringstream log_stream_;
};

#endif

