#pragma once

#include <QMap>
#include <QPair>

#include "BufferInterface.h"
#include "Color.h"
#include "Tile.h"

namespace mms {

class TileGraphic {

public:

    TileGraphic();
    TileGraphic(
        const Tile* tile,
        BufferInterface* bufferInterface);

    void setColor(const Color color);
    void declareWall(Direction direction, bool isWall);
    void undeclareWall(Direction direction);
    void setText(const QString& text);

    // TODO: MACK - rename these to "reload" or something
    void drawPolygons() const;
    void drawTextures();

    // TODO: MACK - do I need these anymore?
    // TODO: MACK - rename these to "refresh" or something
    void updateColor() const;
    void updateWalls() const;
    void updateText() const;
    
private:

    // Input and output objects
    const Tile* m_tile;
    BufferInterface* m_bufferInterface;

    // Visual state
    Color m_color;
    QMap<Direction, bool> m_declaredWalls;
    QString m_text;

    // Helper functions
    void updateWall(Direction direction) const;
    QPair<Color, float> deduceWallColorAndAlpha(Direction direction) const;
};

} // namespace mms
