/**
 * 05: Cartesian coordinate system
 */

#include <gle/gle.hpp>
#include <gle/export.hpp>
#include <gle/shape.hpp>
#include <learnopengl/shader_m.h>
#include <unistd.h>

int main()
{
  const float WIDTH = 800, HEIGHT = 800;
  auto* window = gle::setup(WIDTH, HEIGHT, "Practice");

  Shader shader("exp/05_coord.vs", "exp/05_coord.fs");
  shader.use();

  gle::Tetragon tetra(
    // positions
    // {  1.0f,  1.0f, 0.0f,
    //    1.0f,  0.0f, 0.0f,
    //    0.0f,  1.0f, 0.0f,
    //    0.0f,  1.0f, 0.0f,
    //    0.0f,  0.0f, 0.0f,
    //    1.0f,  0.0f, 0.0f
    // },
    {  0.5f,  0.5f, 0.0f,
       0.5f, -0.5f, 0.0f,
      -0.5f,  0.5f, 0.0f,
      -0.5f,  0.5f, 0.0f,
      -0.5f, -0.5f, 0.0f,
       0.5f, -0.5f, 0.0f
    },
    {  1.0f, 1.0f,
       1.0f, 0.0f,
       0.0f, 1.0f,
       0.0f, 1.0f,
       0.0f, 0.0f,
       1.0f, 0.0f
    },
    "resources/coord_tex.png");
  shader.setInt("texture0", tetra.texture);

  // configure global opengl state
  // -----------------------------
  // glEnable(GL_DEPTH_TEST);

  // float lframe = glfwGetTime();
  // float delta = 0.0f;
  // float v = 2.0f;
  glm::mat4 model = glm::mat4(1.0f);
  while (!glfwWindowShouldClose(window))
  {

    // float cframe = glfwGetTime();
    // delta = cframe - lframe;
    // lframe = cframe;

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 model      = glm::mat4(1.0f);
    glm::mat4 view       = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), WIDTH / HEIGHT, 0.1f, 100.0f);
    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    tetra.draw();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}
