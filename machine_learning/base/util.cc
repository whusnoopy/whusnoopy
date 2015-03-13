// Copyright 2011 yewen

#include <string>

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>

#include "base/logging.h"
#include "base/util.h"

ssize_t socket_read(int communicate_socket,
                    void* buffer,
                    size_t buffer_length) {
  char* cs = static_cast<char*>(buffer);
  while (buffer_length > 0) {
    ssize_t receive_length = read(communicate_socket, cs, buffer_length);
    if (receive_length == -1) {
      if (errno == EINTR) {
        continue;
      }
      LOG(FATAL) << "Fail to read from socket: " << communicate_socket;
      return -1;
    } else if (receive_length == 0) {
      break;
    }
    cs += receive_length;
    buffer_length -= receive_length;
  }
  return cs - static_cast<char*>(buffer);
}

int socket_write(int communicate_socket,
                 const void* buffer,
                 size_t buffer_length) {
  const char* cs = static_cast<const char*>(buffer);
  while (buffer_length > 0) {
    int sent_length = write(communicate_socket, cs, buffer_length);
    if (sent_length == -1) {
      if (errno == EINTR) {
        continue;
      }
      LOG(FATAL) << "Fail to write to socket: " << communicate_socket;
      return -1;
    }
    cs += sent_length;
    buffer_length -= sent_length;
  }
  return 0;
}

string stringPrintf(const char* format, ...) {
  va_list args;
  char buffer[1024];
  va_start(args, format);
  vsnprintf(buffer, sizeof(buffer) - 1, format, args);
  va_end(args);
  return buffer;
}

string getLocalTimeAsString(const char* format) {
  time_t t = time(NULL);
  struct tm tm;
  localtime_r(&t, &tm);
  char buf[1024];
  strftime(buf, sizeof(buf), format, &tm);
  return buf;
}

int mkdirRecursive(const string& path, mode_t mode) {
  if (path[0] != '/') {
    LOG(ERROR) << "Path must be absolute path";
    return -1;
  }

  if (access(path.c_str(), R_OK | W_OK | X_OK | F_OK) == 0) {
    LOG(DEBUG) << path << " exsisted";
    return 0;
  }

  string up_level_path = path.substr(0, path.rfind('/'));
  if (up_level_path.length() == 0) {
    LOG(ERROR) << "Already on the top level";
    return -1;
  }

  LOG(DEBUG) << path << " up level " << up_level_path;
  if (mkdirRecursive(up_level_path, mode) == -1)
    return -1;
  
  mkdir(path.c_str(), mode);
  LOG(DEBUG) << "Make dir " << path << " successful";

  return 0;
}

char* getToken(char* &buf, const char split_char) {
    char* ret = buf;
    while (*buf != '\0' && *buf != '\n' && *buf != split_char) {
        buf++;
    }
    *buf = '\0';
    buf++;
    return ret;
}

