// Copyright 2011 yewen

#include <errno.h>
#include <syslog.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

#include "base/flags.h"
#include "base/util.h"

#include "base/logging.h"

DECLARE_FLAGS(string, root_dir);

DECLARE_FLAGS(string, process_name);

// log level controller, log level less than log_level will be written
DEFINE_OPTIONAL_FLAGS(int, log_level, 4,
                      "If true, DEBUG logs will also written");

// Also log to stderr or not
DEFINE_OPTIONAL_FLAGS(bool, logtostderr, false,
                      "If true, all logs will also written to stderr");

const string LOG_LEVEL_NAME[] = {
  "FATAL", "ERROR", "WARNING", "FAULT", "INFO", "DEBUG"
};

class LogFile {
  public :
    ~LogFile() {
      if (file_)
        fclose(file_);
    }

    void create() {
      string dir = FLAGS_root_dir + "/log";
      if (mkdir(dir.c_str(), 0777) < 0 && errno != EEXIST) {
        openlog("Flood Judge Client", 0, LOG_USER);
        syslog(LOG_ERR, "Fail to create dir %s", dir.c_str());
        return;
      }
      string filename = 
               stringPrintf("%s/%s.%s.%d.log",
                            dir.c_str(),
                            FLAGS_process_name.c_str(),
                            getLocalTimeAsString("%Y%m%d-%H%M%S").c_str(),
                            getpid());
      file_ = fopen(filename.c_str(), "w");
      if (file_ == NULL) {
        openlog("Flood Judge Client", 0, LOG_USER);
        syslog(LOG_ERR, "Fail to create file %s", filename.c_str());
      }
      filesize_ = 0;
    }

    void write(const string& log_message) {
      if (file_ == NULL)
        this->create();
      if (file_ || FLAGS_logtostderr) {
        if (file_) {
          filesize_ += log_message.size();
          if (filesize_ > MAX_LOG_FILE_SIZE) {
            if (file_) {
              fclose(file_);
              file_ = NULL;
            }
            this->create();
            filesize_ += log_message.size();
          }
          fprintf(file_, "%s", log_message.c_str());
          fflush(file_);
        }
        if (FLAGS_logtostderr) {
          fprintf(stderr, "%s", log_message.c_str());
          fflush(stderr);
        }
      }
    }

  private :
    FILE* file_;
    int filesize_;
} logFile;

Log::Log(const char* filename, int line_number, int level) :
  level_(level) {
  log_stream_ << getLocalTimeAsString("%Y-%m-%d %H:%M:%S")
              << " [" << LOG_LEVEL_NAME[level] << "] "
              << filename << ":" << line_number << ": ";
  if (level == FATAL) {
    log_stream_ << strerror(errno) << ". ";
  }
}

Log::~Log() {
  log_stream_ << endl;
  if (level_ > FLAGS_log_level)
    log_stream_.flush();
  else
    logFile.write(log_stream_.str());
}

