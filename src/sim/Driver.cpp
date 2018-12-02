#include "Driver.h"

#include <QApplication>

#include "Assert.h"
#include "FontImage.h"
#include "Logging.h"
#include "Settings.h"
#include "SimTime.h"
#include "Window.h"

namespace mms {

int Driver::drive(int argc, char* argv[]) {

    // Make sure that this function is called just once
    ASSERT_RUNS_JUST_ONCE();

    // Initialize Qt
    QApplication app(argc, argv);

    // Initialize singletons
    Logging::init();
    Settings::init();
    FontImage::init();
    SimTime::init();

    // Create the main window
    Window window;
    window.show();

    // Start the event loop
    return app.exec();
}

} 
