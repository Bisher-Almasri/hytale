#pragma once
#include <Rendering/Block.hpp>

class Grass : public Block
{
  public:
    Grass(int cubeSize = 1, std::array<bool, 6> vFaces = {true, true, true, true, true, true},
          glm::mat4 transform = glm::mat4(1.0f))
        : Block(cubeSize, "assets/textures/grass_block.png", vFaces, transform)
    {
    }
};
