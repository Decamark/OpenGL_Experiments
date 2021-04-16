/**
 * 08: Simulation of free fall
 */

#include <functional>

#include <gle/gle.hpp>
#include <gle/export.hpp>
#include <gle/shape.hpp>
#include <gle/coord.hpp>
#include <gle/time.hpp>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>

auto* window = gle::setup(WIDTH, HEIGHT, "Practice");
Camera camera(window, glm::vec3(20.0f, 20.0f, 20.0f), glm::vec3(0.0f, 1.0f, 0.0f), -135.0f, -45.0f);

int main()
{
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  // tell GLFW to capture our mouse
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);

  Shader shader("exp/08.vs", "exp/08.fs");
  shader.use();

  gle::Cartesian coord(&shader, 50);

  gle::Cube cube(&shader);

  gle::Clock clock;
  while (!glfwWindowShouldClose(window))
  {
    camera.processInput(clock.tick());
    std::cout << clock.t << std::endl;

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), WIDTH / HEIGHT, 0.1f, 100.0f);
    shader.setMat4("view", camera.GetViewMatrix());
    shader.setMat4("projection", projection);

    std::function<void(double)> motion = [&cube](double t) {
      float h = 10.0f-9.8*t*t;
      if (h >= 0)
        cube.setPos(10.0f, h, 10.0f);
      else
        cube.setPos(10.0f, 0.0f, 10.0f);
    };
    cube.move(motion, clock.t);
    cube.draw();

    coord.draw();
    coord.draw_guide({cube.model[3][0], cube.model[3][1], cube.model[3][2]});

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}
