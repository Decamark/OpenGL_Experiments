#ifndef _GLAB_SHAPE_POLYHEDRON
#define _GLAB_SHAPE_POLYHEDRON

#include <glab/shape.hpp>

namespace glab
{
  class Cube : public Shape
  {
  public:
    float x0, y0, z0;

    //   v4--v6
    //  /    /|
    // v0--v2 v7
    // |   | /
    // v1--v3
    Cube(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f) : x0(x), y0(y), z0(z)
    {
      mode = GL_TRIANGLES;

      std::vector<float> vertices = {
        -w/2,  w/2,  w/2, 1.0f, 0.0f, 0.0f,
        -w/2, -w/2,  w/2, 0.0f, 1.0f, 0.0f,
         w/2,  w/2,  w/2, 0.0f, 0.0f, 1.0f,
         w/2, -w/2,  w/2, 1.0f, 1.0f, 0.0f,
        -w/2,  w/2, -w/2, 0.0f, 1.0f, 1.0f,
        -w/2, -w/2, -w/2, 1.0f, 0.0f, 1.0f,
         w/2,  w/2, -w/2, 1.0f, 0.0f, 0.0f,
         w/2, -w/2, -w/2, 0.0f, 1.0f, 0.0f
      };
      std::tie(vao,vbo) = partition(vertices, 2, 3, 3);
      std::vector<unsigned int> indices = {
        4, 0, 6, 6, 0, 2, // v4-v0-v2-v6
        0, 1, 2, 2, 1, 3, // v0-v1-v3-v2
        2, 3, 6, 6, 3, 7, // v2-v3-v7-v6
        6, 7, 4, 4, 7, 5, // v6-v7-v5-v4
        4, 5, 0, 0, 5, 1, // v4-v5-v1-v0
        1, 3, 5, 7, 5, 3  // v1-v5-v7-v3
      };
      ebo = order(vao, indices);

      setPos(x, y, z);
    }
  }; // class Cube
} // namespace glab

#endif
