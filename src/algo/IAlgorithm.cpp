#include "IAlgorithm.h"

IAlgorithm::IAlgorithm(sim::MouseInterface* mouse) {
    m_mouse = mouse;
}

sim::MouseInterface* IAlgorithm::M() {
    return m_mouse;
}
