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
    float d;
    std::vector<float> vertices;
    unsigned int vao;
    unsigned int vbo;
  public:
    Cartesian(Shader* shader, float d = 10) : shader(shader), d(d)
    {
      // xy-plane
      for (float y=0.0f; y<=d; y++) {
        vertices.push_back(0.0f);
        vertices.push_back(y);
        vertices.push_back(0.0f);
        vertices.push_back(d);
        vertices.push_back(y);
        vertices.push_back(0.0f);
      }
      for (float x=0.0f; x<=d; x++) {
        vertices.push_back(x);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(x);
        vertices.push_back(d);
        vertices.push_back(0.0f);
      }
      // yz-plane
      for (float y=0.0f; y<=d; y++) {
        vertices.push_back(0.0f);
        vertices.push_back(y);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(y);
        vertices.push_back(d);
      }
      for (float z=0.0f; z<=d; z++) {
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(z);
        vertices.push_back(0.0f);
        vertices.push_back(d);
        vertices.push_back(z);
      }
      // zx-plane
      for (float z=0.0f; z<=d; z++) {
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(z);
        vertices.push_back(d);
        vertices.push_back(0.0f);
        vertices.push_back(z);
      }
      for (float x=0.0f; x<=d; x++) {
        vertices.push_back(x);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(x);
        vertices.push_back(0.0f);
        vertices.push_back(d);
      }

      glGenVertexArrays(1, &vao);
      glBindVertexArray(vao);

      glGenBuffers(1, &vbo);
      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), &vertices[0], GL_STATIC_DRAW);

      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);
    }

    void draw()
    {
      glBindVertexArray(vao);
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glDrawArrays(GL_LINES, 0, 12*(d+1));
    }
  };

  class CartesianA
  {
  private:
    Shader* shader;
    gle::Tetragon tetra;
  public:
    CartesianA(Shader* shader) : shader(shader), tetra(gle::Tetragon("resources/coord_tex.png"))
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
