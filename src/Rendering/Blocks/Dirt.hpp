#pragma once
#include <Rendering/Block.hpp>

class Dirt : public Block
{
  public:
    Dirt(int cubeSize = 1, std::array<bool, 6> vFaces = {true, true, true, true, true, true},
         glm::mat4 transform = glm::mat4(1.0f))
        : Block(cubeSize, "assets/textures/dirt_block.png", vFaces, transform)
    {
    }
};
