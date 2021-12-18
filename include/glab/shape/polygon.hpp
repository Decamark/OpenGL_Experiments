#ifndef _GLAB_SHAPE_POLYGON
#define _GLAB_SHAPE_POLYGON

#include <glab/shape.hpp>

namespace glab
{
  class Triangle : public Shape
  {
  public:
    // Expects:
    // { x1, y1, z1, r1, g1, b1,
    //   x2, y2, z2, r2, g2, b2,
    //   x3, y3, z3, r3, g3, b3 }
    Triangle(std::vector<float> vertices)
    {
      mode = GL_TRIANGLES;

      std::tie(vao,vbo) = partition(vertices, 2, 3, 3);
      ebo = order(vao, {0, 1, 2});
    }

    // Equilateral triangle with length "l" each
    Triangle(float x, float y, float z, float l = 1.0f)
    {
      mode = GL_TRIANGLES;

      std::vector<float> vertices = {
        0.0f,  l/2, 0.0f, 1.0f, 0.0f, 0.0f,
        -l/2, -l/2, 0.0f, 0.0f, 1.0f, 0.0f,
         l/2, -l/2, 0.0f, 0.0f, 0.0f, 1.0f
      };
      std::tie(vao,vbo) = partition(vertices, 2, 3, 3);
      ebo = order(vao, {0, 1, 2});

      setPos(x, y, z);
    }
  }; // class Triangle

  class Tetragon : public Shape
  {
  public:
    //  v0--v2
    //  |    |
    //  v1--v3
    //
    // Expects:
    // { x0, y0, z0, r0, g0, b0,
    //   x1, y1, z1, r1, g1, b1,
    //   x2, y2, z2, r2, g2, b2,
    //   x3, y3, z3, r3, g3, b3 }
    Tetragon(std::vector<float> vertices)
    {
      mode = GL_TRIANGLES;

      std::tie(vao,vbo) = partition(vertices, 2, 3, 3);
      ebo = order(vao, {0, 1, 2, 2, 1, 3});
    }

    Tetragon(float w = 1.0f, float h = 1.0f, float x = 0.0f, float y = 0.0f, float z = 0.0f)
    {
      mode = GL_TRIANGLES;

      std::vector<float> vertices = {
        -w/2,  h/2, 0.0f, 1.0f, 0.0f, 0.0f,
        -w/2, -h/2, 0.0f, 0.0f, 1.0f, 0.0f,
         w/2,  h/2, 0.0f, 0.0f, 0.0f, 1.0f,
         w/2, -h/2, 0.0f, 1.0f, 0.0f, 0.0f
      };
      std::tie(vao,vbo) = partition(vertices, 2, 3, 3);
      ebo = order(vao, {0, 1, 2, 2, 1, 3});

      setPos(x, y, z);
    }

    // With a texture
    Tetragon(float w, float h, float x, float y, float z, const char* filepath) : Shape(Shader3dTexture())
    {
      mode = GL_TRIANGLES;

      std::vector<float> vertices = {
        -w/2,  h/2, 0.0f, 0.0f, 1.0f,
        -w/2, -h/2, 0.0f, 0.0f, 0.0f,
         w/2,  h/2, 0.0f, 1.0f, 1.0f,
         w/2, -h/2, 0.0f, 1.0f, 0.0f
      };
      std::tie(vao,vbo) = partition(vertices, 2, 3, 2);
      ebo = order(vao, {0, 1, 2, 2, 1, 3});

      textures.push_back( loadTexture(filepath) );

      setPos(x, y, z);
    }
  }; // class Tetragon
} // namespace glab

#endif
