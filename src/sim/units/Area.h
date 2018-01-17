#pragma once

namespace mms {

class Area {

public:

    Area();
    static Area MetersSquared(double metersSquared);

    double getMetersSquared() const;

    Area operator*(double factor) const;
    Area operator+(const Area& area) const;
    Area operator-(const Area& area) const;

private:

    double m_metersSquared;
    Area(double metersSquared);

};

} // namespace mms
