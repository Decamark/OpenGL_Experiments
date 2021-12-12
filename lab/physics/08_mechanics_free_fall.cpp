/**
 * (Physics) 08: Simulation of free fall
 */

#include <functional>

#include <glab/glab.hpp>
#include <glab/shape.hpp>
#include <glab/grid.hpp>
#include <glab/time.hpp>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>

auto* window = glab::setup(1200, 800, "glab");
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

  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1200.0f / 800.0f, 0.1f, 100.0f);

  glab::Grid grid(100.0f);
  grid.setP(projection);

  glab::Cube cube(10.0f, 100.0f, 10.0f, 5.0f);
  cube.setP(projection);

  glab::Cube cube2(10.0f, 100.0f, 10.0f, 5.0f);
  cube2.setP(projection);

  glab::Clock clock;
  while (!glfwWindowShouldClose(window))
  {
    camera.processInput(clock.tick());
    std::cout << clock.t << std::endl;

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    grid.setV(camera.GetViewMatrix());
    grid.draw();

    cube.setV(camera.GetViewMatrix());
    std::function<void(double)> motion = [&cube](double t) {
      float h = cube.y0-9.8/2*t*t;
      if (h >= 0)
        cube.setPos(cube.x0,    h, cube.z0);
      else
        cube.setPos(cube.x0, 0.0f, cube.z0);
    };
    cube.move(motion, clock.t);
    cube.draw();
    cube.drawGuide();

    cube2.setV(camera.GetViewMatrix());
    cube2.draw();
    cube2.drawGuide();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}
