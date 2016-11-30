#pragma once

namespace mms {

class Screen {

public:
    static void init();
    static Screen* get();
    double pixelsPerMeter();

private:
    Screen();
    static Screen* INSTANCE;
    double m_pixelsPerMeter;

};

} // namespace mms
