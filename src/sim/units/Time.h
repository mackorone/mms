#pragma once

namespace sim {

class Time {

public:
    virtual ~Time();
    float getSeconds() const;
    float getMilliseconds() const;

protected:
    Time(); // Abstract class
    float m_seconds;

};

} // namespace sim
