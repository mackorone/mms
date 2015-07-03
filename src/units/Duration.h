#pragma once

namespace sim {

class Duration {

public:
    virtual ~Duration() = 0;
    float getSeconds() const;
    float getMilliseconds() const;
    float getMicroseconds() const;
    bool operator<(const Duration& duration) const;

protected:
    Duration();
    float m_seconds;

};

} // namespace sim
