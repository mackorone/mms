#include "Driver.h"

#include <QApplication>

#include "FontImage.h"
#include "Logging.h"
#include "Screen.h"
#include "Settings.h"
#include "SimTime.h"
#include "State.h"
#include "Model.h"
#include "Window.h"

namespace mms {

int Driver::drive(int argc, char* argv[]) {

    // Make sure that this function is called just once
    ASSERT_RUNS_JUST_ONCE();

    // Initialize Qt
    QApplication app(argc, argv);

    // Initialize the Time object
    SimTime::init();

    // Initialize the Screen object
    Screen::init();

    // Initiliaze the Logging object
    Logging::init();

    // Initialize the Settings object
    Settings::init();

    P(); // Initialize the Param object
    S(); // Initialize the State object

    // Initialize the FontImage object
    FontImage::init(P()->tileTextFontImage());

    // Create the main window
    Window window;
    window.show();

    // Start the event loop
    return app.exec();
}

} // namespace mms
