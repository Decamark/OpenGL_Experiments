#ifndef _GLAB_SHAPE_LINE
#define _GLAB_SHAPE_LINE

#include <glab/shape.hpp>

namespace glab
{
  class Line : public Shape
  {
  public:
    // Expects:
    // { x1, y1, z1, r1, g1, b1,
    //   x2, y2, z2, r2, g2, b2 }
    Line(std::vector<float> vertices)
    {
      mode = GL_LINES;

      std::tie(vao,vbo) = partition(vertices, 2, 3, 3);
      ebo = order(vao, {0, 1});
    }

    // p1--p2
    Line(std::vector<float> p1, std::vector<float> /* Color */ c1,
         std::vector<float> p2, std::vector<float> /* Color */ c2)
    {
      mode = GL_LINES;

      std::vector<float> vertices;
      vertices += p1;
      vertices += c1;
      vertices += p2;
      vertices += c2;
      std::tie(vao,vbo) = partition(vertices, 2, 3, 3);
      ebo = order(vao, {0, 1});
    }

    // p1--p2
    Line(glm::vec3 p1, glm::vec3 /* Color */ c1,
         glm::vec3 p2, glm::vec3 /* Color */ c2)
    {
      mode = GL_LINES;

      std::vector<float> vertices;
      vertices.push_back(p1.x); vertices.push_back(p1.y); vertices.push_back(p1.z);
      vertices.push_back(c1.r); vertices.push_back(c1.g); vertices.push_back(c1.b);
      vertices.push_back(p2.x); vertices.push_back(p2.y); vertices.push_back(p2.z);
      vertices.push_back(c2.r); vertices.push_back(c2.g); vertices.push_back(c2.b);
      std::tie(vao,vbo) = partition(vertices, 2, 3, 3);
      ebo = order(vao, {0, 1});
    }
  }; // class Line
} // namespace glab

#endif
