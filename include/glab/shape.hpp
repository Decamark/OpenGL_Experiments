#ifndef _GLAB_SHAPE
#define _GLAB_SHAPE

#include <iostream>
#include <functional>
#include <numeric>

#include <glab/glab.hpp>

#include <learnopengl/shader_m.h>

namespace glab
{
  template <typename T>
  void operator+=(std::vector<T>& v1, std::vector<T>& v2)
  {
    v1.insert(v1.end(), v2.begin(), v2.end());
  }

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

  unsigned int loadTexture(const char* filepath)
  {
    unsigned int texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0); // [NOTICE] Using the texture unit "GL_TEXTURE0"
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
      std::cout << "Failed to load texture" << std::endl;
    stbi_image_free(data);
    return texture;
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
    GLenum mode = GL_TRIANGLES;
    GLenum rasterization = GL_FILL;
    std::vector<unsigned int> textures;
    glm::mat4 model = glm::mat4(1.0f);

    // Children with the constructor which doesn't hold initializer lists will call this
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

    // FIXME: Remove me later
    Shape(std::vector<float> vertices) {}

    // Set "projection" matrix
    void setP(glm::mat4 projection)
    {
      shader.use();
      shader.setMat4("projection", projection);
      guide_shader.use();
      guide_shader.setMat4("projection", projection);
    }

    // Set "view" matrix
    void setV(glm::mat4 view)
    {
      shader.use();
      shader.setMat4("view", view);
      guide_shader.use();
      guide_shader.setMat4("view", view);
    }

    // Set "model" matrix
    void setM()
    {
      shader.use();
      shader.setMat4("model", model);
    }

    // Set the absolute position in the world
    void setPos(float x, float y, float z)
    {
      model = glm::translate(glm::mat4(1.0), {x,y,z});
      shader.use();
      shader.setMat4("model", model);
    }

    // Move from the current position
    void translate(glm::vec3 move)
    {
      model = glm::translate(model, move);
      shader.use();
      shader.setMat4("model", model);
    }

    // Rotate at the current position
    void rotate(float angle, glm::vec3 axis)
    {
      model = glm::rotate(model, glm::radians(angle), axis);
      shader.use();
      shader.setMat4("model", model);
    }

    // Rotate around the center
    void rotateAround(float angle, glm::vec3 axis, glm::vec3 center)
    {
      translate(-center);
      rotate(angle, axis);
      translate(center);
    }

    void move(std::function<void(double)> motion, double t)
    {
      motion(t);
    }

    // Check if this shape intersects with a ray (Ordinally using Camera's "Front" matrix)
    // [CAUTION] Works only with GL_TRIANGLES
    // https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection
    //
    // a, b, c: vertex
    // o: ray's origin
    // od: ray's vector
    bool intersect(/* ray's origin */ glm::vec3 o, /* ray's vector */ glm::vec3 od)
    {
      glBindVertexArray(vao);
      int stride; // The number of elements a vertex holds
      glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);
      stride /= sizeof(float);

      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      float* vertices = (float*) glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
      unsigned int* indices = (unsigned int*) glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_ONLY);
      int size = 0; // The number of indices
      glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
      size /= sizeof(float);

      bool res = false;
      for (int i=0; i < size; i+=3) {
        // This is a's local coordinate
        glm::vec4 a_ = { vertices[ stride*indices[i] ],   vertices[ stride*indices[i]+1 ], vertices[ stride*indices[i]+2 ],     1.0f };
        // This is the world coordinate
        glm::vec3 a(model * a_);
        glm::vec4 b_ = { vertices[ stride*indices[i+1] ], vertices[ stride*indices[i+1]+1 ], vertices[ stride*indices[i+1]+2 ], 1.0f };
        glm::vec3 b(model * b_);
        glm::vec4 c_ = { vertices[ stride*indices[i+2] ], vertices[ stride*indices[i+2]+1 ], vertices[ stride*indices[i+2]+2 ], 1.0f };
        glm::vec3 c(model * c_);

        glm::vec3 ab = b - a;
        glm::vec3 ac = c - a;
        glm::vec3 p  = glm::cross(od, ac);
        float det    = glm::dot(ab, p);
        float invDet = 1 / det;

        const float EPSILON = 0.0000001;
        if (std::fabs(det) < EPSILON) {
          res |= false;
          continue;
        }

        glm::vec3 ao = o - a;
        float u = glm::dot(ao, p) * invDet;
        if (u < 0 || u > 1) {
          res |= false;
          continue;
        }

        glm::vec3 q = glm::cross(ao, ab);
        float v = glm::dot(od, q) * invDet;
        if (v < 0 || u + v > 1) {
          res |= false;
          continue;
        }

        float t = glm::dot(ac, q) * invDet;
        res |= true;
        break;
      }

      glUnmapBuffer(GL_ARRAY_BUFFER);
      glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
      return res;
    }

    void draw()
    {
      drawElements(mode);
    }

    void drawElements(GLenum mode)
    {
      shader.use();

      glBindVertexArray(vao);

      glPolygonMode(GL_FRONT_AND_BACK, rasterization);

      // The number of the vertices
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
      int size = 0;
      glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

      // [FIXME] This snippet doesn't consider the case where a different number of vertices are used on each face
      // ex) Archimedian solid
      if (not textures.empty()) {
        for (int i=0; i<textures.size(); i++) {
          glEnable(GL_BLEND);
          glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
          glBindTexture(GL_TEXTURE_2D, textures[i]);
          glDrawRangeElements(mode, size/sizeof(float)/textures.size()*i, size/sizeof(float)/textures.size()*(i+1)-1, size/sizeof(float)/textures.size(), GL_UNSIGNED_INT, 0);
        }
      }
      else
        glDrawElements(mode, size/sizeof(float), GL_UNSIGNED_INT, 0);

      glBindVertexArray(0);
    }

    [[deprecated("Use drawElements() instead")]]
    void drawArrays(GLenum mode, /* The number of the vertices */ int size)
    {
      shader.use();

      glBindVertexArray(vao);

      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      // glDrawArrays(GL_LINES, 0, size);
      glDrawArrays(mode, 0, size);

      glBindVertexArray(0);
    }

    void drawGuide()
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
  }; // class Shape
} // namespace glab

#include <glab/shape/line.hpp>
#include <glab/shape/polygon.hpp>
#include <glab/shape/polyhedron.hpp>

#endif
