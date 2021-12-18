#ifndef _GLAB_SHAPE_POLYGON
#define _GLAB_SHAPE_POLYGON

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
  };
} // namespace glab

#endif
