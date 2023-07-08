#pragma once

namespace mms {

class Driver {
 public:
  Driver() = delete;
  static int drive(int argc, char *argv[]);
};

}  // namespace mms
