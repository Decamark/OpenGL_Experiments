/**
 * 05: Cartesian coordinate system
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

  // Shader shader("exp/05_coord.vs", "exp/05_coord.gs", "exp/05_coord.fs");
  Shader shader("exp/05_coord.vs", "exp/05_coord.fs");
  shader.use();

  gle::Cartesian coord(&shader, 20);

  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);

  while (!glfwWindowShouldClose(window))
  {
    camera.processInput();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), WIDTH / HEIGHT, 0.1f, 100.0f);
    shader.setMat4("model", model);
    shader.setMat4("view", camera.GetViewMatrix());
    shader.setMat4("projection", projection);

    coord.draw();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}
