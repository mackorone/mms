#pragma once

namespace mms {

class Duration {

public:

    Duration();
    static Duration Seconds(double seconds);
    static Duration Milliseconds(double milliseconds);
    static Duration Microseconds(double microseconds);

    double getSeconds() const;
    double getMilliseconds() const;
    double getMicroseconds() const;

    Duration operator*(double factor) const;
    Duration operator+(const Duration& other) const;
    Duration operator-(const Duration& other) const;
    bool operator<(const Duration& other) const;
    void operator+=(const Duration& other);

private:

    double m_seconds;
    Duration(double seconds);

};

} 
