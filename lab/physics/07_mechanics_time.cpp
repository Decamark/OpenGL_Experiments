/**
 * (Physics) 07: Time
 */

#include <functional>

#include <glab/glab.hpp>
#include <glab/shape.hpp>
#include <glab/grid.hpp>
#include <glab/time.hpp>

#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>

auto* window = glab::setup(1200, 800, "glab");
Camera camera(window, glm::vec3(25.0f, 25.0f, 25.0f), glm::vec3(0.0f, 1.0f, 0.0f), -135.0f, -45.0f);

int main()
{
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1200.0f / 800.0f, 0.1f, 100.0f);

  glab::Grid grid(50);
  grid.setP(projection);

  glab::Cube cube(5.0f, 5.0f, 5.0f);
  cube.setP(projection);

  glab::Clock clock;
  while (!glfwWindowShouldClose(window))
  {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.processInput(clock.tick());
    std::cout << clock.t << std::endl;

    cube.setV(camera.GetViewMatrix());
    std::function<void(double)> motion =
      [&cube](double t) {
        cube.translate({0.01f, glm::sin(t)*0.1, 0.01f});
      };
    cube.move(motion, clock.t);
    cube.draw();
    cube.drawGuide();

    grid.setV(camera.GetViewMatrix());
    grid.draw();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}
