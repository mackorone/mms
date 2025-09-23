#pragma once

namespace mms {

class DriverCli {
 public:
  DriverCli() = delete;
  static int drive(int argc, char *argv[]);
};

}