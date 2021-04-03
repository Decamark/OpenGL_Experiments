#ifndef _GLE_SHAPE
#define _GLE_SHAPE

#include <gle/gle.hpp>

namespace gle
{
  class Shape
  {
  public:
    unsigned int vao;
    unsigned int vbo[3];
    glm::mat4 model;

    void bind(std::vector<float> vertices, int size = 3, int i = 0)
    {
      glGenBuffers(1, &vbo[i]);
      glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
      glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), &vertices[0], GL_STATIC_DRAW);

      glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, size * sizeof(float), (void*)0);
      glEnableVertexAttribArray(i);
    }

    void set_pos(glm::vec3 pos)
    {
      model = glm::translate(glm::mat4(1.0), pos);
    }

    void translate(glm::vec3 move)
    {
      model = glm::translate(model, move);
    }

    void rotate(float angle, glm::vec3 axis)
    {
      model = glm::rotate(model, glm::radians(angle), axis);
    }

    virtual void draw() = 0;
    void draw_guide()
    {
      
    }

    void debug()
    {
      for (int i=0; i<4; i++)
      {
        for (int j=0; j<4; j++)
        {
          std::cout << model[j][i] << ' ';
        }
        std::cout << std::endl;
      }
    }
  };

  class Line : public Shape
  {
  public:
    Line(std::vector<float> vertices)
    {
      glGenVertexArrays(1, &vao);
      glBindVertexArray(vao);
      bind(vertices);
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
      glGenVertexArrays(1, &vao);
      glBindVertexArray(vao);
      bind(vertices);
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
    unsigned int texture;

    void bind_texture(const char* filepath)
    {
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
      }
      else
      {
        std::cout << "Failed to load texture" << std::endl;
      }
      stbi_image_free(data);
    }

    Tetragon(std::vector<float> vertices)
    {
      glGenVertexArrays(1, &vao);
      glBindVertexArray(vao);
      bind(vertices);
    }

    Tetragon(std::vector<float> vertices, std::vector<float> texpos, const char* filepath,
             glm::mat4 model = glm::mat4(1.0f))
    {
      glGenVertexArrays(1, &vao);
      glBindVertexArray(vao);
      bind(vertices);
      bind(texpos, 2, 1);

      bind_texture(filepath);

      this->model = model;
    }

    Tetragon(const char* filepath, float w = 1.0f, float h = 1.0f)
    {
      std::vector<float> vertices =
        {  0.5f*w,  0.5f*h, 0.0f,
           0.5f*w, -0.5f*h, 0.0f,
          -0.5f*w,  0.5f*h, 0.0f,
          -0.5f*w,  0.5f*h, 0.0f,
          -0.5f*w, -0.5f*h, 0.0f,
           0.5f*w, -0.5f*h, 0.0f
        };
      std::vector<float> texpos =
        { 1.0f, 1.0f,
          1.0f, 0.0f,
          0.0f, 1.0f,
          0.0f, 1.0f,
          0.0f, 0.0f,
          1.0f, 0.0f
        };

      glGenVertexArrays(1, &vao);
      glBindVertexArray(vao);
      bind(vertices);
      bind(texpos, 2, 1);

      bind_texture(filepath);

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
}

#endif
