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

  class Triangle : Shape
  {
  public:
    Triangle(std::vector<float> vertices)
    {
      std::tie(vao,vbo) = partition(vertices);
    }

    void draw()
    {
      glBindVertexArray(vao);

      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glDrawArrays(GL_TRIANGLES, 0, 3);

      glBindVertexArray(0);
    }
  };

} // namespace gle

#endif
