#pragma once

#include <QString>

namespace mms {

class SettingsMisc {

public:

    SettingsMisc() = delete;

    static QString getRecentMazeAlgo();
    static void setRecentMazeAlgo(const QString& name);

    static QString getRecentMouseAlgo();
    static void setRecentMouseAlgo(const QString& name);

    static int getRecentWindowWidth();
    static void setRecentWindowWidth(int width);

    static int getRecentWindowHeight();
    static void setRecentWindowHeight(int height);

    // Font image path
    static QString getFontImagePathComboBoxValue();
    static QString getFontImagePathLineEditValue();
    static bool getFontImagePathComboBoxSelected();
    static void setFontImagePathComboBoxValue(QString value);
    static void setFontImagePathLineEditValue(QString value);
    static void setFontImagePathComboBoxSelected(bool selected);

private:

    static const QString GROUP;
    static const QString KEY_RECENT_MAZE_ALGO;
    static const QString KEY_RECENT_MOUSE_ALGO;
    static const QString KEY_RECENT_WINDOW_WIDTH;
    static const QString KEY_RECENT_WINDOW_HEIGHT;

    static const QString KEY_FONT_IMAGE_PATH_LINE_EDIT_VALUE;
    static const QString KEY_FONT_IMAGE_PATH_COMBO_BOX_VALUE;
    static const QString KEY_FONT_IMAGE_PATH_COMBO_BOX_SELECTED;

    static int getNumber(QString key, int defaultValue);
    static QString getValue(const QString& key);
    static void setValue(const QString& key, const QString& value);

};

} 
