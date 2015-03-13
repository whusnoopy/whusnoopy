// Copyright 2011 yewen

#ifndef _BASE_LOGGING_H__
#define _BASE_LOGGING_H__

#include <string>
#include <sstream>

using namespace std;

const int MAX_LOG_FILE_SIZE = 256 * 1024;

#define LOG(level) Log(__FILE__, __LINE__, level).stream()

#define LOGF(level, format, ...) \
    Log(__FILE__, __LINE__, level).stream() << stringPrintf(format, __VA_ARGS__)

enum LOG_LEVEL {
  FATAL = 0, ERROR, WARNING, FAULT, INFO, DEBUG
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

