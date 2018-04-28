#include "FontImage.h"

#include "Assert.h"
#include "Resources.h"
#include "SettingsMisc.h"
#include "SimUtilities.h"

namespace mms {

FontImage* FontImage::INSTANCE = nullptr;

void FontImage::init() {
    ASSERT_TR(INSTANCE == nullptr);
    INSTANCE = new FontImage();
}

FontImage* FontImage::get() {
    ASSERT_FA(INSTANCE == nullptr);
    return INSTANCE;
}

QString FontImage::imageFilePath() {
    return m_imageFilePath;
}

QMap<QChar, QPair<double, double>> FontImage::positions() {
    return m_positions;
}

FontImage::FontImage() {

    // Initialize the font image path if necessary
    QString comboBoxValue = SettingsMisc::getFontImagePathComboBoxValue();
    if (SettingsMisc::getFontImagePathComboBoxValue().isEmpty()) {
        int index = 0;
        QStringList fonts = Resources::getFonts();
        for (int i = 0; i < fonts.size(); i += 1) {
            QString font = fonts.at(i);
            if (font.contains("Unispace-Bold.png")) {
                index = i;
                break;
            }
        }
        SettingsMisc::setFontImagePathComboBoxValue(fonts.at(index));
        SettingsMisc::setFontImagePathComboBoxSelected(true);
    }

    // Read the font image path
    m_imageFilePath = SettingsMisc::getFontImagePathLineEditValue();
    if (SettingsMisc::getFontImagePathComboBoxSelected()) {
        m_imageFilePath = SettingsMisc::getFontImagePathComboBoxValue();
    }

    // These values must perfectly reflect the font image being used,
    // or else the wrong characters will be displayed on the tiles
    QString fontImageChars =
        " !\"#$%&'()*+,-./0123456789:;<=>?"
        "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
        "`abcdefghijklmnopqrstuvwxyz{|}~";

    // Create a map of the font image chars (allowable tile text chars) to
    // their horizontal position in the png image (a fraction from 0.0 to 1.0)
    int size = fontImageChars.size();
    for (int i = 0; i < size; i += 1) {
        double start = static_cast<double>(i + 0) / static_cast<double>(size);
        double end   = static_cast<double>(i + 1) / static_cast<double>(size);
        m_positions.insert(fontImageChars.at(i), {start, end});
    }
}

} // namespace mms
