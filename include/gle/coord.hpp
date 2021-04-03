#ifndef _GLE_COORD
#define _GLE_COORD

#include <gle/shape.hpp>
#include <learnopengl/shader_m.h>

namespace gle
{
  class Cartesian
  {
  private:
    Shader* shader;
    gle::Tetragon tetra;
  public:
    Cartesian(Shader* shader) : shader(shader), tetra(gle::Tetragon("resources/coord_tex.png"))
    {
      shader->setInt("texture0", tetra.texture);
    }

    void draw()
    {
      tetra.model = glm::mat4(1.0f);
      tetra.translate({0.5, 0.5, 0.0});
      for (int x=0; x<10; x++)
      {
        for (int y=0; y<10; y++)
        {
          shader->setMat4("model", tetra.model);
          tetra.draw();
          tetra.translate({0.0, 1.0, 0.0});
        }
        tetra.translate({1.0, -10.0, 0.0});
      }

      tetra.model = glm::mat4(1.0f);
      tetra.translate({0.5, 0.0, 0.5});
      tetra.rotate(90.0f, {1.0, 0.0, 0.0});
      for (int x=0; x<10; x++)
      {
        for (int z=0; z<10; z++)
        {
          shader->setMat4("model", tetra.model);
          tetra.draw();
          tetra.translate({0.0, 1.0, 0.0});
        }
        tetra.translate({1.0, -10.0, 0.0});
      }

      tetra.model = glm::mat4(1.0f);
      tetra.translate({0.0, 0.5, 0.5});
      tetra.rotate(90.0f, {0.0, 1.0, 0.0});
      for (int x=0; x<10; x++)
      {
        for (int y=0; y<10; y++)
        {
          shader->setMat4("model", tetra.model);
          tetra.draw();
          tetra.translate({0.0, 1.0, 0.0});
        }
        tetra.translate({-1.0, -10.0, 0.0});
      }
    }
  };
}

#endif
