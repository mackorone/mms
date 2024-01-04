#include "DriverCli.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QProcess>

#include "AssertMacros.h"
#include "ColorManager.h"
#include "Logging.h"
#include "Settings.h"
#include "Window.h"
#include "SettingsMouseAlgos.h"

namespace mms {

int DriverCli::drive(int argc, char *argv[]) {
  // Make sure that this function is called just once
  ASSERT_RUNS_JUST_ONCE();

  // Initialize Qt
  QApplication app(argc, argv);

  QCommandLineParser parser;
  parser.setApplicationDescription("Test helper");
  parser.addHelpOption();

  QCommandLineOption mazeFileOption(QStringList() << "m" << "mazeFile");
  mazeFileOption.setValueName("mazePath");
  parser.addOption(mazeFileOption);

  QCommandLineOption runCommandOption(QStringList() << "r" << "runCommand");
  runCommandOption.setValueName("runCommand");
  parser.addOption(runCommandOption);

  QCommandLineOption directoryCommandOption(QStringList() << "d" << "runDirectory");
  directoryCommandOption.setValueName("runDirectory");
  parser.addOption(directoryCommandOption);

  QCommandLineOption buildCommandOption(QStringList() << "b" << "buildCommand");
  buildCommandOption.setValueName("buildCommand");
  parser.addOption(buildCommandOption);

  parser.process(app);

  // Initialize singletons
  Logging::init();
  Settings::init();
  ColorManager::init();

  if(!parser.isSet(mazeFileOption)){
      return -1;
  }

  if(!parser.isSet(runCommandOption)){
       return -2;
  }

  if(!parser.isSet(directoryCommandOption)){
       return -3;
  }

  QString mazePath = parser.values(mazeFileOption).first();
  QString runCommand = parser.values(runCommandOption).first();
  QString directory = parser.values(directoryCommandOption).first();

  SettingsMouseAlgos::removeall();
  SettingsMouseAlgos::add("temp", directory, "test", runCommand);

  Window window;
  window.onMazeFileComboBoxChanged(mazePath);
  window.startRun();

  return app.exec();
}

}  // namespace mms
