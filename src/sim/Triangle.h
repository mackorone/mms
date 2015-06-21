#pragma once

#include <Cartesian.h>

namespace sim {

class Triangle {

public:
    Triangle();
    Triangle(const Cartesian& p1, const Cartesian& p2, const Cartesian& p3);
    Cartesian getP1() const;
    Cartesian getP2() const;
    Cartesian getP3() const;

private:
    Cartesian m_p1;
    Cartesian m_p2;
    Cartesian m_p3;
};

} // namespace sim
