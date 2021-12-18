#ifndef _GLAB_GRID
#define _GLAB_GRID

#include <glab/shape.hpp>
#include <learnopengl/shader_m.h>

namespace glab
{
  class Grid : public Shape
  {
  private:
    float l, m, n;
  public:
    Grid(float d, Shader shader = Shader3dColor()) : Grid(d, d, d, shader)
    {}

    Grid(float l, float m, float n, Shader shader = Shader3dColor()) : l(l), m(m), n(n), Shape(shader)
    {
      primitive = GL_LINES;

      std::vector<float> vs;
      // xy-plane
      for (float y=0.0f; y<=m; y++) {
        vs.push_back(0.0f);
        vs.push_back(y);
        vs.push_back(0.0f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(l);
        vs.push_back(y);
        vs.push_back(0.0f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
      }
      for (float x=0.0f; x<=l; x++) {
        vs.push_back(x);
        vs.push_back(0.0f);
        vs.push_back(0.0f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(x);
        vs.push_back(m);
        vs.push_back(0.0f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
      }
      // yz-plane
      for (float y=0.0f; y<=m; y++) {
        vs.push_back(0.0f);
        vs.push_back(y);
        vs.push_back(0.0f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(0.0f);
        vs.push_back(y);
        vs.push_back(n);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
      }
      for (float z=0.0f; z<=n; z++) {
        vs.push_back(0.0f);
        vs.push_back(0.0f);
        vs.push_back(z);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(0.0f);
        vs.push_back(m);
        vs.push_back(z);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
      }
      // zx-plane
      for (float z=0.0f; z<=n; z++) {
        vs.push_back(0.0f);
        vs.push_back(0.0f);
        vs.push_back(z);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(l);
        vs.push_back(0.0f);
        vs.push_back(z);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
      }
      for (float x=0.0f; x<=l; x++) {
        vs.push_back(x);
        vs.push_back(0.0f);
        vs.push_back(0.0f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(x);
        vs.push_back(0.0f);
        vs.push_back(n);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
      }
      std::tie(vao,vbo) = partition(vs, 2, 3, 3);

      std::vector<unsigned int> indices;
      for (unsigned int i=0; i<4*(l+m+n)+12; i++)
        indices.push_back(i);
      ebo = order(vao, indices);
    }

    // void draw()
    // {
    //   shader.use();
    //   glBindVertexArray(vao);
    //   // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //   glDrawArrays(GL_LINES, 0, 4*(l+m+n)+12);
    //   glBindVertexArray(0);
    // }
  };

  // class GridA
  // {
  // private:
  //   Shader shader;
  //   Tetragon tetra;
  // public:
  //   GridA(Shader shader = Shader3dColor()) : shader(shader), tetra(Tetragon("resources/coord_tex.png"))
  //   {
  //     shader.setInt("texture0", tetra.texture);
  //   }

  //   void draw()
  //   {
  //     tetra.model = glm::mat4(1.0f);
  //     tetra.translate({0.5, 0.5, 0.0});
  //     for (int x=0; x<10; x++)
  //     {
  //       for (int y=0; y<10; y++)
  //       {
  //         shader.setMat4("model", tetra.model);
  //         tetra.draw();
  //         tetra.translate({0.0, 1.0, 0.0});
  //       }
  //       tetra.translate({1.0, -10.0, 0.0});
  //     }

  //     tetra.model = glm::mat4(1.0f);
  //     tetra.translate({0.5, 0.0, 0.5});
  //     tetra.rotate(90.0f, {1.0, 0.0, 0.0});
  //     for (int x=0; x<10; x++)
  //     {
  //       for (int z=0; z<10; z++)
  //       {
  //         shader.setMat4("model", tetra.model);
  //         tetra.draw();
  //         tetra.translate({0.0, 1.0, 0.0});
  //       }
  //       tetra.translate({1.0, -10.0, 0.0});
  //     }

  //     tetra.model = glm::mat4(1.0f);
  //     tetra.translate({0.0, 0.5, 0.5});
  //     tetra.rotate(90.0f, {0.0, 1.0, 0.0});
  //     for (int x=0; x<10; x++)
  //     {
  //       for (int y=0; y<10; y++)
  //       {
  //         shader.setMat4("model", tetra.model);
  //         tetra.draw();
  //         tetra.translate({0.0, 1.0, 0.0});
  //       }
  //       tetra.translate({-1.0, -10.0, 0.0});
  //     }
  //   }
  // };
}

#endif
