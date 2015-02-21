#pragma once

namespace sim {

class Time {

public:
    virtual ~Time() = 0;
    float getSeconds() const;
    float getMilliseconds() const;
    float getMicroseconds() const;

protected:
    Time();
    float m_seconds;

};

} // namespace sim
