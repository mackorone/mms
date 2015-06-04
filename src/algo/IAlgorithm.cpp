#include "IAlgorithm.h"

#include <iostream>

IAlgorithm::IAlgorithm(sim::MouseInterface** mouse) {
    m_mouse = mouse;
}

sim::MouseInterface* IAlgorithm::M() {
    if (*m_mouse == NULL) {
        throw NULL;
    }
    return *m_mouse;
}
