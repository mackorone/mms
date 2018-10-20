#pragma once

#include <QString>

#include "Color.h"

namespace mms {

class ColorManager {

public:

    static void init();
    static ColorManager* get();
    
    Color getTileBaseColor();
    Color getTileWallColor();
    Color getTileCornerColor();
    Color getUndeclaredWallColor();
    Color getUndeclaredNoWallColor();
    Color getIncorrectlyDeclaredWallColor();
    Color getIncorrectlyDeclaredNoWallColor();
    Color getMouseBodyColor();
    Color getMouseWheelColor();

private:

    static const QString GROUP;
    static const QString KEY_TILE_BASE_COLOR;
    static const QString KEY_TILE_WALL_COLOR;
    static const QString KEY_TILE_CORNER_COLOR;
    static const QString KEY_UNDECLARED_WALL_COLOR;
    static const QString KEY_UNDECLARED_NO_WALL_COLOR;
    static const QString KEY_INCORRECTLY_DECLARED_WALL_COLOR;
    static const QString KEY_INCORRECTLY_DECLARED_NO_WALL_COLOR;
    static const QString KEY_MOUSE_BODY_COLOR;
    static const QString KEY_MOUSE_WHEEL_COLOR;

    ColorManager();
    static ColorManager* INSTANCE;

    Color m_tileBaseColor;
    Color m_tileWallColor;
    Color m_tileCornerColor;
    Color m_undeclaredWallColor;
    Color m_undeclaredNoWallColor;
    Color m_incorrectlyDeclaredWallColor;
    Color m_incorrectlyDeclaredNoWallColor;
    Color m_mouseBodyColor;
    Color m_mouseWheelColor;

    static Color getColor(const QString& key, Color defaultColor);
    static void setColor(const QString& key, Color color);
    static QString getValue(const QString& key);
    static void setValue(const QString& key, const QString& value);

};

} // namespace mms
