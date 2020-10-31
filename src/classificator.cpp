#include "version.h"
#include <iostream>
#include <map>

#include <dlib/clustering.h>
#include <dlib/svm_threaded.h>

int main() {
  std::cout << "Current classificator version: " << version() << std::endl;
  return 0;
}
