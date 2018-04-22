#pragma once

#include <QString>

#include "Color.h"

namespace mms {

class ColorManager {

public:

    static void init();
    static ColorManager* get();
    
    Color getTileBaseColor();
    void setTileBaseColor(Color color);

    Color getTileWallColor();
    void setTileWallColor(Color color);

    Color getTileCornerColor();
    void setTileCornerColor(Color color);

    Color getTileFogColor();
    void setTileFogColor(Color color);

    double getTileFogAlpha();
    void setTileFogAlpha(double alpha);

    Color getUndeclaredWallColor();
    void setUndeclaredWallColor(Color color);

    Color getUndeclaredNoWallColor();
    void setUndeclaredNoWallColor(Color color);

    Color getIncorrectlyDeclaredWallColor();
    void setIncorrectlyDeclaredWallColor(Color color);

    Color getIncorrectlyDeclaredNoWallColor();
    void setIncorrectlyDeclaredNoWallColor(Color color);

    Color getDistanceCorrectTileBaseColor();
    void setDistanceCorrectTileBaseColor(Color color);

    Color getMouseBodyColor();
    void setMouseBodyColor(Color color);

    Color getMouseCenterOfMassColor();
    void setMouseCenterOfMassColor(Color color);

    Color getMouseWheelColor();
    void setMouseWheelColor(Color color);

    Color getMouseSensorColor();
    void setMouseSensorColor(Color color);

    Color getMouseVisionColor();
    void setMouseVisionColor(Color color);

private:

    ColorManager();
    static ColorManager* INSTANCE;

    Color m_tileBaseColor;
    Color m_tileWallColor;
    Color m_tileCornerColor;
    Color m_tileFogColor;
    double m_tileFogAlpha;
    Color m_undeclaredWallColor;
    Color m_undeclaredNoWallColor;
    Color m_incorrectlyDeclaredWallColor;
    Color m_incorrectlyDeclaredNoWallColor;
    Color m_distanceCorrectTileBaseColor;
    Color m_mouseBodyColor;
    Color m_mouseCenterOfMassColor;
    Color m_mouseWheelColor;
    Color m_mouseSensorColor;
    Color m_mouseVisionColor;

    static const QString GROUP;
    static const QString KEY_TILE_BASE_COLOR;
    static const QString KEY_TILE_WALL_COLOR;
    static const QString KEY_TILE_CORNER_COLOR;
    static const QString KEY_TILE_FOG_COLOR;
    static const QString KEY_TILE_FOG_ALPHA;
    static const QString KEY_UNDECLARED_WALL_COLOR;
    static const QString KEY_UNDECLARED_NO_WALL_COLOR;
    static const QString KEY_INCORRECTLY_DECLARED_WALL_COLOR;
    static const QString KEY_INCORRECTLY_DECLARED_NO_WALL_COLOR;
    static const QString KEY_DISTANCE_CORRECT_TILE_BASE_COLOR;
    static const QString KEY_MOUSE_BODY_COLOR;
    static const QString KEY_MOUSE_CENTER_OF_MASS_COLOR;
    static const QString KEY_MOUSE_WHEEL_COLOR;
    static const QString KEY_MOUSE_SENSOR_COLOR;
    static const QString KEY_MOUSE_VISION_COLOR;

    static Color getColor(const QString& key, Color defaultColor);
    static void setColor(const QString& key, Color color);
    static QString getValue(const QString& key);
    static void setValue(const QString& key, const QString& value);

};

} // namespace mms
