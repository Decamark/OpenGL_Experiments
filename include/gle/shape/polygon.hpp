#ifndef _GLE_SHAPE_POLYGON
#define _GLE_SHAPE_POLYGON

#include <gle/shape.hpp>

namespace gle
{
  class Line : public Shape
  {
  public:
    // Expects:
    // { x1, y1, z1, r1, g1, b1,
    //   x2, y2, z2, r2, g2, b2 }
    Line(std::vector<float> vertices)
    {
      std::tie(vao,vbo) = partition(vertices, 2, 3, 3);
      ebo = order(vao, {0, 1});
    }

    // p1--p2
    Line(std::vector<float> p1, std::vector<float> /* Color */ c1,
         std::vector<float> p2, std::vector<float> /* Color */ c2)
    {
      std::vector<float> vertices;
      vertices += p1;
      vertices += c1;
      vertices += p2;
      vertices += c2;
      std::tie(vao,vbo) = partition(vertices, 2, 3, 3);
      ebo = order(vao, {0, 1});
    }
  };

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
  };

} // namespace gle

#endif
