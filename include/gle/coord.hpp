#ifndef _GLE_COORD
#define _GLE_COORD

#include <gle/shape.hpp>
#include <gle/shader.hpp>
#include <learnopengl/shader_m.h>

namespace gle
{
  class Cartesian
  {
  private:
    Shader* shader;
    float d;
    std::vector<float> guide_vs;
    unsigned int vao,       vbo;
    unsigned int guide_vao, guide_vbo;
    glm::mat4 model;
  public:
    Cartesian(float d = 10, Shader* shader = &GLE_SHADER_3D_COLOR) : d(d), shader(shader)
    {
      std::vector<float> vs;
      // xy-plane
      for (float y=0.0f; y<=d; y++) {
        vs.push_back(0.0f);
        vs.push_back(y);
        vs.push_back(0.0f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(d);
        vs.push_back(y);
        vs.push_back(0.0f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
      }
      for (float x=0.0f; x<=d; x++) {
        vs.push_back(x);
        vs.push_back(0.0f);
        vs.push_back(0.0f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(x);
        vs.push_back(d);
        vs.push_back(0.0f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
      }
      // yz-plane
      for (float y=0.0f; y<=d; y++) {
        vs.push_back(0.0f);
        vs.push_back(y);
        vs.push_back(0.0f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(0.0f);
        vs.push_back(y);
        vs.push_back(d);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
      }
      for (float z=0.0f; z<=d; z++) {
        vs.push_back(0.0f);
        vs.push_back(0.0f);
        vs.push_back(z);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(0.0f);
        vs.push_back(d);
        vs.push_back(z);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
      }
      // zx-plane
      for (float z=0.0f; z<=d; z++) {
        vs.push_back(0.0f);
        vs.push_back(0.0f);
        vs.push_back(z);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(d);
        vs.push_back(0.0f);
        vs.push_back(z);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
      }
      for (float x=0.0f; x<=d; x++) {
        vs.push_back(x);
        vs.push_back(0.0f);
        vs.push_back(0.0f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(x);
        vs.push_back(0.0f);
        vs.push_back(d);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
        vs.push_back(0.4f);
      }

      std::tie(vao,vbo) = partition(vs, 2, 3, 3);

      // guide
      guide_vs = std::vector(36, 0.0f);
      glGenVertexArrays(1, &guide_vao);
      glBindVertexArray(guide_vao);

      glGenBuffers(1, &guide_vbo);
      glBindBuffer(GL_ARRAY_BUFFER, guide_vbo);
      glBufferData(GL_ARRAY_BUFFER, guide_vs.size()*sizeof(float), &guide_vs[0], GL_DYNAMIC_DRAW);

      /* (x,y,z) */
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);
      /* (r,g,b) */
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
      glEnableVertexAttribArray(1);

      model = glm::mat4(1.0f);
    }

    void draw()
    {
      shader->use();
      shader->setMat4("model", model);
      glBindVertexArray(vao);
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glDrawArrays(GL_LINES, 0, 12*(d+1));
    }

    void draw_guide(glm::vec3 pos)
    {
      shader->use();
      shader->setMat4("model", model);

      guide_vs[0]  = pos.x;
      guide_vs[1]  = pos.y;
      guide_vs[2]  = pos.z;
      guide_vs[3]  = 1.0f;
      guide_vs[4]  = 0.0f;
      guide_vs[5]  = 0.0f;
      guide_vs[6]  = 0.0f;
      guide_vs[7]  = pos.y;
      guide_vs[8]  = pos.z;
      guide_vs[9]  = 1.0f;
      guide_vs[10] = 0.0f;
      guide_vs[11] = 0.0f;
      guide_vs[12] = pos.x;
      guide_vs[13] = pos.y;
      guide_vs[14] = pos.z;
      guide_vs[15] = 0.0f;
      guide_vs[16] = 1.0f;
      guide_vs[17] = 0.0f;
      guide_vs[18] = pos.x;
      guide_vs[19] = 0.0f;
      guide_vs[20] = pos.z;
      guide_vs[21] = 0.0f;
      guide_vs[22] = 1.0f;
      guide_vs[23] = 0.0f;
      guide_vs[24] = pos.x;
      guide_vs[25] = pos.y;
      guide_vs[26] = pos.z;
      guide_vs[27] = 0.0f;
      guide_vs[28] = 0.0f;
      guide_vs[29] = 1.0f;
      guide_vs[30] = pos.x;
      guide_vs[31] = pos.y;
      guide_vs[32] = 0.0f;
      guide_vs[33] = 0.0f;
      guide_vs[34] = 0.0f;
      guide_vs[35] = 1.0f;

      glBindBuffer(GL_ARRAY_BUFFER, guide_vbo);
      glBufferSubData(GL_ARRAY_BUFFER, 0, guide_vs.size()*sizeof(float), &guide_vs[0]);

      glBindVertexArray(guide_vao);
      glDrawArrays(GL_LINES, 0, 18);
    }
  };

  class CartesianA
  {
  private:
    Shader* shader;
    gle::Tetragon tetra;
  public:
    CartesianA(Shader* shader = &GLE_SHADER_3D_COLOR) : shader(shader), tetra(gle::Tetragon("resources/coord_tex.png"))
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
