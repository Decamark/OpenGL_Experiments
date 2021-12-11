#ifndef _GLE_SHAPE
#define _GLE_SHAPE

#include <iostream>
#include <functional>
#include <numeric>

#include <gle/gle.hpp>
#include <learnopengl/shader_m.h>

namespace gle
{
  std::pair<unsigned int,unsigned int>
  partition(std::vector<float>& vertices)
  {
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
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

    return {vao,vbo};
  }

  std::pair<unsigned int, unsigned int>
  partition(std::vector<float> vertices, int count, ...)
  {
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    // bind VBO to GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), &vertices[0], GL_STATIC_DRAW);

    std::vector<int> sizes;
    va_list vl;
    va_start(vl, count);
    for (int i=0; i<count; i++)
      sizes.push_back( va_arg(vl, int) );
    va_end(vl);
    int stride = std::accumulate(sizes.begin(), sizes.end(), 0);

    int offset = 0;
    for (int i=0; i<count; i++) {
      glVertexAttribPointer(i, sizes[i], GL_FLOAT, GL_FALSE, stride*sizeof(float), (void*)(offset*sizeof(float)));
      glEnableVertexAttribArray(i);
      offset += sizes[i];
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    return {vao,vbo};
  }

  unsigned int order(unsigned int vao, std::vector<unsigned int> indices)
  {
    glBindVertexArray(vao);
    unsigned int ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    glBindVertexArray(0);
    return ebo;
  }

  template<glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
  void debug(glm::vec<L,T,Q> v)
  {
    std::cout.setf(std::ios::right, std::ios::adjustfield);
    for (int i=0; i<v.length(); i++)
    {
      std::cout << std::setw(10) << std::fixed << std::setprecision(5)
                << v[i] << ' ';
      std::cout << std::endl;
    }
  }

  template<glm::length_t C, glm::length_t R, typename T, glm::qualifier Q = glm::defaultp>
  void debug(glm::mat<C,R,T,Q> m)
  {
    std::cout.setf(std::ios::right, std::ios::adjustfield);
    for (int i=0; i<m[0].length(); i++)
    {
      for (int j=0; j<m.length(); j++)
        std::cout << std::setw(10) << std::fixed << std::setprecision(5)
                  << m[j][i] << ' ';
      std::cout << std::endl;
    }
  }

  class Shape
  {
  protected:
    unsigned int       vao,       vbo, ebo;
    unsigned int guide_vao, guide_vbo;
    Shader shader, guide_shader = Shader3dColor();
  public:
    unsigned int texture;
    glm::mat4 model = glm::mat4(1.0f);

    Shape()
    {
      // shape
      shader = Shader3dColor();
      shader.use();
      shader.setMat4("model", model);

      // guide
      guide_shader.use();
      guide_shader.setMat4("model", model);
      std::vector<float> guide_vs(36, 0.0f);
      std::tie(guide_vao, guide_vbo) = partition(guide_vs, 2, 3, 3);
    }
    Shape(Shader shader) : shader(shader)
    {
      // shape
      shader.use();
      shader.setMat4("model", model);

      // guide
      guide_shader.use();
      guide_shader.setMat4("model", model);
      std::vector<float> guide_vs(36, 0.0f);
      std::tie(guide_vao, guide_vbo) = partition(guide_vs, 2, 3, 3);
    }
    Shape(std::vector<float> vertices) {}

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

    void setP(glm::mat4 projection)
    {
      shader.use();
      shader.setMat4("projection", projection);
      guide_shader.use();
      guide_shader.setMat4("projection", projection);
    }

    void setV(glm::mat4 view)
    {
      shader.use();
      shader.setMat4("view", view);
      guide_shader.use();
      guide_shader.setMat4("view", view);
    }

    void setM()
    {
      shader.use();
      shader.setMat4("model", model);
    }

    void setPos(float x, float y, float z)
    {
      model = glm::translate(glm::mat4(1.0), {x,y,z});
      shader.use();
      shader.setMat4("model", model);
    }

    void translate(glm::vec3 move)
    {
      model = glm::translate(model, move);
      shader.use();
      shader.setMat4("model", model);
    }

    void rotate(float angle, glm::vec3 axis)
    {
      model = glm::rotate(model, glm::radians(angle), axis);
      shader.use();
      shader.setMat4("model", model);
    }

    void rotateAround(float angle, glm::vec3 axis, glm::vec3 origin)
    {
      translate(-origin);
      rotate(angle, axis);
      translate(origin);
    }

    void move(std::function<void(double)> motion, double t)
    {
      motion(t);
    }

    void draw_guide()
    {
      guide_shader.use();

      std::vector<float> guide_vs(36, 0.0f);

      guide_vs[0]  = model[3][0];
      guide_vs[1]  = model[3][1];
      guide_vs[2]  = model[3][2];
      guide_vs[3]  = 1.0f;
      guide_vs[4]  = 0.0f;
      guide_vs[5]  = 0.0f;
      guide_vs[6]  = 0.0f;
      guide_vs[7]  = model[3][1];
      guide_vs[8]  = model[3][2];
      guide_vs[9]  = 1.0f;
      guide_vs[10] = 0.0f;
      guide_vs[11] = 0.0f;

      guide_vs[12] = model[3][0];
      guide_vs[13] = model[3][1];
      guide_vs[14] = model[3][2];
      guide_vs[15] = 0.0f;
      guide_vs[16] = 1.0f;
      guide_vs[17] = 0.0f;
      guide_vs[18] = model[3][0];
      guide_vs[19] = 0.0f;
      guide_vs[20] = model[3][2];
      guide_vs[21] = 0.0f;
      guide_vs[22] = 1.0f;
      guide_vs[23] = 0.0f;

      guide_vs[24] = model[3][0];
      guide_vs[25] = model[3][1];
      guide_vs[26] = model[3][2];
      guide_vs[27] = 0.0f;
      guide_vs[28] = 0.0f;
      guide_vs[29] = 1.0f;
      guide_vs[30] = model[3][0];
      guide_vs[31] = model[3][1];
      guide_vs[32] = 0.0f;
      guide_vs[33] = 0.0f;
      guide_vs[34] = 0.0f;
      guide_vs[35] = 1.0f;

      glBindBuffer(GL_ARRAY_BUFFER, guide_vbo);
      glBufferSubData(GL_ARRAY_BUFFER, 0, guide_vs.size()*sizeof(float), &guide_vs[0]);

      glBindVertexArray(guide_vao);
      glDrawArrays(GL_LINES, 0, 6);
      glBindVertexArray(0);
    }
  };

  class Line : public Shape
  {
  public:
    Line(std::vector<float> vertices)
    {
      std::tie(vao,vbo) = partition(vertices);
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
      std::tie(vao,vbo) = partition(vertices);
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

    // Tetragon(std::vector<float> vertices, const char* filepath,
    //          glm::mat4 model = glm::mat4(1.0f)) : Shape(vertices)
    // {
    //   std::tie(vao,vbo) = partition(vertices);

    //   load_texture(filepath);

    //   this->model = model;
    // }

    Tetragon(float w = 1.0f, float h = 1.0f, float x = 0.0f, float y = 0.0f, float z = 0.0f, Shader shader = Shader3dColor()) : Shape(shader)
    {
      std::vector<float> vertices = {
        w/2,  h/2, 0.0f, 1.0f, 0.0f, 0.0f,
        w/2, -h/2, 0.0f, 0.0f, 1.0f, 0.0f,
       -w/2,  h/2, 0.0f, 0.0f, 0.0f, 1.0f,
       -w/2,  h/2, 0.0f, 0.0f, 0.0f, 1.0f,
       -w/2, -h/2, 0.0f, 0.0f, 1.0f, 0.0f,
        w/2, -h/2, 0.0f, 1.0f, 0.0f, 0.0f
      };
      std::tie(vao,vbo) = partition(vertices, 2, 3, 3);
      setPos(x, y, z);
    }

    Tetragon(const char* filepath, float w = 1.0f, float h = 1.0f)
    {
      std::vector<float> vertices = {
        0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
       -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
       -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
       -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f
      };
      std::tie(vao,vbo) = partition(vertices);

      load_texture(filepath);

      this->model = model;
    }

    void draw()
    {
      shader.use();

      glBindVertexArray(vao);
      // if (texture) {
      //   glEnable(GL_BLEND);
      //   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      //   glActiveTexture(GL_TEXTURE0+texture);
      //   glBindTexture(GL_TEXTURE_2D, texture);
      // }
      glDrawArrays(GL_TRIANGLES, 0, 6);
      glBindVertexArray(0);
    }
  };

  class Cube : public Shape
  {
  public:
    float x0, y0, z0;
    Cube(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f, Shader shader = Shader3dColor()) : x0(x), y0(y), z0(z), Shape(shader)
    {
      setPos(x, y, z);
      std::vector<float> vertices = {
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
      std::tie(vao,vbo) = partition(vertices, 1, 3);
    }

    void draw()
    {
      shader.use();

      glBindVertexArray(vao);

      // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      glBindVertexArray(0);
    }
  };

  // http://www.songho.ca/opengl/gl_sphere.html
  class Sphere : public Shape
  {
  public:
    float r;
    float x, y, z;

    Sphere(/* radius */ float r, float x, float y, float z,
           int stackCount, int sectorCount,
           Shader shader = Shader3dColor()) : r(r), x(x), y(y), z(z)
    {
      const float PI = glm::pi<float>();

      // tmp vertex definition (x,y,z,s,t)
      struct Vertex
      {
        float x, y, z, s, t;
      };
      std::vector<Vertex> tmpVertices;

      float sectorStep = 2 * PI / sectorCount;
      float stackStep = PI / stackCount;
      float sectorAngle, stackAngle;

      // compute all vertices first, each vertex contains (x,y,z,s,t) except normal
      for (int i = 0; i <= stackCount; ++i)
      {
        stackAngle = PI / 2 - i * stackStep;       // starting from pi/2 to -pi/2
        float xz = r * glm::cos(stackAngle);       // r * cos(u)
        // float xy = r * glm::cos(stackAngle);       // r * cos(u)
        float y  = r * glm::sin(stackAngle);       // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= sectorCount; ++j)
        {
          sectorAngle = j * sectorStep;           // starting from 0 to 2pi

          Vertex vertex;
          vertex.x = xz * glm::sin(sectorAngle);      // x = r * cos(u) * sin(v)
          vertex.z = xz * glm::cos(sectorAngle);      // z = r * cos(u) * cos(v)
          vertex.y = y;                           // y = r * sin(u)
          vertex.s = (float)j/sectorCount;        // s
          vertex.t = (float)i/stackCount;         // t
          tmpVertices.push_back(vertex);
        }
      }

      std::vector<float> vertices;
      std::vector<unsigned int> indices;
      int i, j, k, vi1, vi2;
      unsigned int index = 0;                                  // index for vertex
      for (i = 0; i < stackCount; ++i)
      {
        vi1 = i * (sectorCount + 1);                // index of tmpVertices
        vi2 = (i + 1) * (sectorCount + 1);

        for (j = 0; j < sectorCount; ++j, ++vi1, ++vi2)
        {
          // get 4 vertices per sector
          //  v1--v3
          //  |    |
          //  v2--v4
          auto v1 = tmpVertices[vi1];
          auto v2 = tmpVertices[vi2];
          auto v3 = tmpVertices[vi1 + 1];
          auto v4 = tmpVertices[vi2 + 1];

          // if 1st stack and last stack, store only 1 triangle per sector
          // otherwise, store 2 triangles (quad) per sector
          if (i == 0) // a triangle for first stack ==========================
          {
            // put a triangle
            vertices.insert(vertices.end(), {v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v4.x, v4.y, v4.z});

            // put indices of 1 triangle
            indices.insert(indices.end(), {index, index+1, index+2});

            index += 3;     // for next
          }
          else if (i == (stackCount-1)) // a triangle for last stack =========
          {
            // put a triangle
            vertices.insert(vertices.end(), {v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z});

            // put indices of 1 triangle
            indices.insert(indices.end(), {index, index+1, index+2});

            index += 3;     // for next
          }
          else // 2 triangles for others ====================================
          {
            // put quad vertices: v1-v2-v3-v4
            vertices.insert(vertices.end(), {v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z, v4.x, v4.y, v4.z});

            // put indices of quad (2 triangles)
            indices.insert(indices.end(), {index, index+1, index+2, index+2, index+1, index+3});

            index += 4;     // for next
          }
        }
      }
      std::tie(vao,vbo) = partition(vertices, 1, 3);
      ebo = order(vao, indices);

      setPos(x, y, z);
    }

    void drawElements()
    {
      shader.use();

      glBindVertexArray(vao);

      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
      int size = 0;
      glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
      glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

      glBindVertexArray(0);
    }
  }; // class Sphere
} // namespace gle

#endif
