/**
 * 06: Coordinate's guide
 */

#include <gle/gle.hpp>
#include <gle/export.hpp>
#include <gle/shape.hpp>
#include <gle/coord.hpp>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <unistd.h>
#include <functional>

auto* window = gle::setup(WIDTH, HEIGHT, "Practice");
Camera camera(window, glm::vec3(5.0f, 5.0f, 10.0f));

int main()
{
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  Shader s_coord("exp/06_coord.vs", "exp/06_coord.fs");
  s_coord.use();

  gle::Cartesian coord(&s_coord);

  gle::Cube cube(&s_coord);
  cube.translate({5.0f, 5.0f, 5.0f});

  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);

  while (!glfwWindowShouldClose(window))
  {
    camera.processInput();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), WIDTH / HEIGHT, 0.1f, 100.0f);
    s_coord.setMat4("model", glm::mat4(1.0f));
    s_coord.setMat4("view", camera.GetViewMatrix());
    s_coord.setMat4("projection", projection);

    cube.draw();

    coord.draw();
    coord.draw_guide({cube.model[3][0], cube.model[3][1], cube.model[3][2]});

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}
