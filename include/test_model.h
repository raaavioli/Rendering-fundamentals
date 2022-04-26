#pragma once
// Defines a simple test model: The Cornel Box

#include <glm/glm.hpp>
#include <vector>

#include "triangle.h"

/**
* Loads the Cornell Box.It is scaled to fill the volume:
* -1 <= x <= +1
* -1 <= y <= +1
* -1 <= z <= +1
*/
void LoadTestModel(std::vector<Triangle>& triangles);