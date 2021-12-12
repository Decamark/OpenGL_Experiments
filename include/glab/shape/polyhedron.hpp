#ifndef _GLAB_SHAPE_POLYHEDRON
#define _GLAB_SHAPE_POLYHEDRON

#include <glab/shape.hpp>

namespace glab
{
  class Cube : public Shape
  {
  public:
    float x0, y0, z0;

    //   v4--v6
    //  /    /|
    // v0--v2 v7
    // |   | /
    // v1--v3
    Cube(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f) : x0(x), y0(y), z0(z)
    {
      mode = GL_TRIANGLES;

      std::vector<float> vertices = {
        -w/2,  w/2,  w/2, 1.0f, 0.0f, 0.0f,
        -w/2, -w/2,  w/2, 0.0f, 1.0f, 0.0f,
         w/2,  w/2,  w/2, 0.0f, 0.0f, 1.0f,
         w/2, -w/2,  w/2, 1.0f, 1.0f, 0.0f,
        -w/2,  w/2, -w/2, 0.0f, 1.0f, 1.0f,
        -w/2, -w/2, -w/2, 1.0f, 0.0f, 1.0f,
         w/2,  w/2, -w/2, 1.0f, 0.0f, 0.0f,
         w/2, -w/2, -w/2, 0.0f, 1.0f, 0.0f
      };
      std::tie(vao,vbo) = partition(vertices, 2, 3, 3);
      std::vector<unsigned int> indices = {
        4, 0, 6, 6, 0, 2, // v4-v0-v2-v6
        0, 1, 2, 2, 1, 3, // v0-v1-v3-v2
        2, 3, 6, 6, 3, 7, // v2-v3-v7-v6
        6, 7, 4, 4, 7, 5, // v6-v7-v5-v4
        4, 5, 0, 0, 5, 1, // v4-v5-v1-v0
        1, 3, 5, 7, 5, 3  // v1-v5-v7-v3
      };
      ebo = order(vao, indices);

      setPos(x, y, z);
    }
  }; // class Cube

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
      // The primitive to use when drawing
      mode = GL_TRIANGLES;
      rasterization = GL_LINE;

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
  }; // class Sphere
} // namespace glab

#endif
