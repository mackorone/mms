#include "Driver.h"

#include <QApplication>

#include "ColorManager.h"
#include "FontImage.h"
#include "Logging.h"
#include "Screen.h"
#include "Settings.h"
#include "SimTime.h"
#include "Model.h"
#include "Window.h"

namespace mms {

int Driver::drive(int argc, char* argv[]) {

    // Make sure that this function is called just once
    ASSERT_RUNS_JUST_ONCE();

    // Initialize Qt
    QApplication app(argc, argv);

    // Initialize logging
    Logging::init();

    // Initialize settings file
    Settings::init();

    // Initialize other singletons
    ColorManager::init();
    Screen::init();
    SimTime::init();

    // Initialize the Param object
    P();

    // Initialize the FontImage object
    FontImage::init(P()->tileTextFontImage());

    // Create the main window
    Window window;
    window.show();

    // Start the event loop
    return app.exec();
}

} // namespace mms
