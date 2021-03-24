#ifndef _GLE_LINE
#define _GLE_LINE

#include <gle/gle.hpp>

namespace gle
{
  class Shape
  {
  public:
    unsigned int vao;
    unsigned int vbo[3];
    void bind(std::vector<float> vertices, int size = 3, int i = 0)
    {
      glGenBuffers(1, &vbo[i]);
      glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
      glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), &vertices[0], GL_STATIC_DRAW);

      glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, size * sizeof(float), (void*)0);
      glEnableVertexAttribArray(i);
    }
    virtual void draw() = 0;
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

  class Tetragon : Shape
  {
  public:
    unsigned int texture;

    Tetragon(std::vector<float> vertices, std::vector<float> texpos, const char* filepath)
    {
      glGenVertexArrays(1, &vao);
      glBindVertexArray(vao);
      bind(vertices);
      bind(texpos, 2, 1);

      glGenTextures(1, &texture);
      glBindTexture(GL_TEXTURE_2D, texture);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      int width, height, nr_channels;
      unsigned char* data = stbi_load(filepath, &width, &height, &nr_channels, 0);
      if (data)
      {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
      }
      else
      {
        std::cout << "Failed to load texture" << std::endl;
      }
      stbi_image_free(data);
    }

    void draw()
    {
      glBindVertexArray(vao);
      glActiveTexture(GL_TEXTURE0+texture);
      glBindTexture(GL_TEXTURE_2D, texture);
      glDrawArrays(GL_TRIANGLES, 0, 6);
    }
  };
}

#endif
