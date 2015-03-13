// Copyright 2011 yewen

#ifndef _BASE_UTIL_H__
#define _BASE_UTIL_H__

#include <string>
#include <sstream>
#include <sys/stat.h>

using namespace std;

ssize_t socket_read(int communicate_socket,
                    void* buffer,
                    size_t buffer_length);

int socket_write(int communicate_socket,
                 const void* buffer,
                 size_t buffer_length);

static inline int sendReply(int communicate_socket, char reply_message) {
  return socket_write(communicate_socket, &reply_message, 1);
}

string stringPrintf(const char* format, ...);

template <class T>
inline string toString(T obj) {
  ostringstream os;
  os << obj;
  return os.str();
}

string getLocalTimeAsString(const char* format);

int mkdirRecursive(const string& path, mode_t mode);

char* getToken(char* &buf, const char split_char = '\t');

#endif

