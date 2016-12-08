#include "Lens.h"

#include "FontImage.h"
#include "Param.h"

namespace mms {

Lens::Lens(Model* model) {

    // Initialize the buffer and graphics objects
    m_bufferInterface = new BufferInterface(
        {model->getMaze()->getWidth(), model->getMaze()->getHeight()},
        &m_graphicCpuBuffer,
        &m_textureCpuBuffer
    );
    m_mazeGraphic = new MazeGraphic(model->getMaze(), m_bufferInterface);
    m_mouseGraphic = new MouseGraphic(model->getMouse(), m_bufferInterface);

    // TODO: MACK
    m_mazeGraphic->drawPolygons();

    // TODO: MACK - get these defaults from somewhere
    initTileGraphicText(2, 4);
}

MazeGraphic* Lens::getMazeGraphic() {
    return m_mazeGraphic;
}

MouseGraphic* Lens::getMouseGraphic() {
    return m_mouseGraphic;
}

void Lens::initTileGraphicText(int numRows, int numCols) {

    // Initialze the tile text in the buffer class, do caching for speed improvement
    m_bufferInterface->initTileGraphicText(
        Meters(P()->wallLength()),
        Meters(P()->wallWidth()),
        {numRows, numCols},
        P()->tileTextBorderFraction(),
        STRING_TO_TILE_TEXT_ALIGNMENT.value(P()->tileTextAlignment()));

    // TODO: MACK - this is kind of confusing
    // - I should insert and then update (no draw method)
    m_textureCpuBuffer.clear();
    m_mazeGraphic->drawTextures();
}

QVector<TriangleGraphic>* Lens::getGraphicCpuBuffer() {
    return &m_graphicCpuBuffer;
}

QVector<TriangleTexture>* Lens::getTextureCpuBuffer() {
    return &m_textureCpuBuffer;
}

} // namespace mms
