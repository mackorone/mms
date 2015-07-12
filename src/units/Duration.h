#pragma once

namespace sim {

class Duration {

public:
    virtual ~Duration() = 0;
    double getSeconds() const;
    double getMilliseconds() const;
    double getMicroseconds() const;
    bool operator<(const Duration& duration) const;

protected:
    Duration();
    double m_seconds;

};

} // namespace sim
