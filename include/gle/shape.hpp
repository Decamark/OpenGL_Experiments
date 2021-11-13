#ifndef _GLE_SHAPE
#define _GLE_SHAPE

#include <iostream>
#include <functional>

#include <gle/gle.hpp>
#include <gle/shader.hpp>
#include <learnopengl/shader_m.h>

namespace gle
{
  class Shape
  {
  protected:
    unsigned int vao, vbo;
    std::vector<float> vertices;
  public:
    Shader* shader;
    unsigned int texture;
    // unsigned int guide_vao;
    glm::mat4 model;

    Shape() {}
    Shape(Shader* shader) : shader(shader) {}
    Shape(std::vector<float> vertices) : vertices(vertices) {}

    void partition()
    {
      glGenVertexArrays(1, &vao);
      glBindVertexArray(vao);

      glGenBuffers(1, &vbo);
      // bind VBO to GL_ARRAY_BUFFER
      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), &vertices[0], GL_STATIC_DRAW);

      // format current VAO with currently bound VBO
      switch (vertices.size()) {
      case (3+2)*6: /* Tetragon (x,y,z,s,t) */
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);
        break;

      case 3*3:   /* Triangle (x,y,z) */
      case 3*6:   /* Tetragon (x,y,z) */
      case 3*6*6: /* Cube     (x,y,z) */
      default:
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
        break;
      }

      glBindBuffer(GL_ARRAY_BUFFER, 0);

      glBindVertexArray(0);
    }

    void load_texture(const char* filepath)
    {
      unsigned int texture;
      glGenTextures(1, &texture);
      glBindTexture(GL_TEXTURE_2D, texture);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      int width, height, nr_channels;
      stbi_set_flip_vertically_on_load(true);
      unsigned char* data = stbi_load(filepath, &width, &height, &nr_channels, 0);
      if (data)
      {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        // textures.push_back(texture);
      }
      else
        std::cout << "Failed to load texture" << std::endl;
      stbi_image_free(data);
    }

    void setPos(float x, float y, float z)
    {
      model = glm::translate(glm::mat4(1.0), {x,y,z});
    }

    void translate(glm::vec3 move)
    {
      model = glm::translate(model, move);
    }

    void rotate(float angle, glm::vec3 axis)
    {
      model = glm::rotate(model, glm::radians(angle), axis);
    }

    void move(std::function<void(double)> motion, double t)
    {
      motion(t);
    }

    virtual void draw() = 0;

    void debug()
    {
      for (int i=0; i<4; i++)
      {
        for (int j=0; j<4; j++)
          std::cout << model[j][i] << ' ';
        std::cout << std::endl;
      }
    }
  };

  class Line : public Shape
  {
  public:
    Line(std::vector<float> vertices)
    {
      partition();
    }

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
    Triangle(std::vector<float> vertices)
    {
      partition();
    }

    void draw()
    {
      glBindVertexArray(vao);

      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glDrawArrays(GL_TRIANGLES, 0, 3);
    }
  };

  class Tetragon : public Shape
  {
  public:
    Tetragon(std::vector<float> vertices) : Shape(vertices) {}

    Tetragon(std::vector<float> vertices, const char* filepath,
             glm::mat4 model = glm::mat4(1.0f)) : Shape(vertices)
    {
      partition();

      load_texture(filepath);

      this->model = model;
    }

    Tetragon(const char* filepath, float w = 1.0f, float h = 1.0f)
    {
      vertices = {
        0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
       -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
       -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
       -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f
      };
      partition();

      load_texture(filepath);

      this->model = model;
    }

    void draw()
    {
      glBindVertexArray(vao);
      if (texture) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glActiveTexture(GL_TEXTURE0+texture);
        glBindTexture(GL_TEXTURE_2D, texture);
      }
      glDrawArrays(GL_TRIANGLES, 0, 6);
    }
  };

  class Cube : public Shape
  {
  public:
    Cube(Shader* shader = &GLE_SHADER_3D_COLOR, float w = 1.0f) : Shape(shader)
    {
      vertices = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
      };
      partition();
      model = glm::mat4(1.0f);
    }

    void draw()
    {
      shader->use();
      shader->setMat4("model", model);

      glBindVertexArray(vao);

      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      glBindVertexArray(0);
    }
  };
}

#endif
