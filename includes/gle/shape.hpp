#ifndef _GLE_LINE
#define _GLE_LINE

#include <gle/gle.hpp>

namespace gle
{
  class Shape
  {
  public:
    unsigned int vao;
    unsigned int vbo;
    void bindn(std::vector<float> vertices, int n = 3)
    {
      glGenBuffers(1, &vbo);
      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), &vertices[0], GL_STATIC_DRAW);

      glGenVertexArrays(1, &vao);
      glBindVertexArray(vao);

      glVertexAttribPointer(0, n, GL_FLOAT, GL_FALSE, n * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);
    }
    virtual void draw() = 0;
  };

  class Line : public Shape
  {
  public:
    Line(std::vector<float> vertices) { bindn(vertices); }

    void draw()
    {
      glBindVertexArray(vao);
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glDrawArrays(GL_LINES, 0, 2);
    }
  };

  class Triangle : Shape
  {
  public:
    Triangle(std::vector<float> vertices) { bindn(vertices); }

    void draw()
    {
      glBindVertexArray(vao);
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glDrawArrays(GL_TRIANGLES, 0, 3);
    }
  };
}

#endif
