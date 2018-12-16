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

    void setWall(Direction direction);
    void clearWall(Direction direction);

    void setColor(const Color color);
    void clearColor();

    void setText(const QString& text);
    void clearText();

    // TODO: MACK - rename these to "reload" or something
    void drawPolygons() const;
    void drawTextures();

private:

    // Input and output objects
    const Tile* m_tile;
    BufferInterface* m_bufferInterface;

    // Visual state
    QMap<Direction, bool> m_walls;
    Color m_color;
    QString m_text;

    // Helper functions
    // TODO: MACK - do I need these anymore?
    // TODO: MACK - rename these to "refresh" or something
    void updateWall(Direction direction) const;
    void updateColor() const;
    void updateText() const;
    
    unsigned char getWallAlpha(Direction direction) const;
};

} 
