#include "base/flags.h"
#include <iostream>

using namespace std;

DEFINE_FLAGS(int, int, "");
DEFINE_FLAGS(bool, bool, "");
DEFINE_FLAGS(string, string, "");

DEFINE_OPTIONAL_FLAGS(int, oint, 4, "");
DEFINE_OPTIONAL_FLAGS(bool, obool, true, "");
DEFINE_OPTIONAL_FLAGS(string, ostring, "Hello world", "");

int main(int argc, char* argv[]) {
  if (parseFlags(argc, argv)) {
    cout << "Cannot parse flags!" << endl;
    return 1;
  }

  cout << FLAGS_int << endl;
  cout << FLAGS_bool << endl;
  cout << FLAGS_string << endl;
  
  cout << FLAGS_oint << endl;
  cout << FLAGS_obool << endl;
  cout << FLAGS_ostring << endl;

  return 0;
}

