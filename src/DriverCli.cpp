#include "DriverCli.h"

#include <QApplication>

#include "AssertMacros.h"
#include "ColorManager.h"
#include "Logging.h"
#include "Settings.h"

namespace mms {

int DriverCli::drive(int argc, char *argv[]) {
  // Make sure that this function is called just once
  ASSERT_RUNS_JUST_ONCE();

  // Initialize Qt
  QApplication app(argc, argv);

  // Initialize singletons
  Logging::init();
  Settings::init();
  ColorManager::init();

//todo add argument handling here 

  return app.exec();
}

}  // namespace mms
