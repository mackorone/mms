#include "FontImage.h"

#include <QFile>

#include "Assert.h"
#include "Logging.h"
#include "Resources.h"
#include "SimUtilities.h"

namespace mms {

FontImage* FontImage::INSTANCE = nullptr;

void FontImage::init(const QString& imageFilePath) {
    ASSERT_TR(INSTANCE == nullptr);
    INSTANCE = new FontImage(imageFilePath);
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

FontImage::FontImage(const QString& imageFilePath) :
    m_imageFilePath(imageFilePath) {

    // Ensure the m_imageFilePath exists
    if (!QFile::exists(m_imageFilePath)) {
        qCritical() << "Could not find font image file" << m_imageFilePath;
        // TODO: upforgrabs
        // Don't quit here - instead, just disable tile text
        SimUtilities::quit();
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
