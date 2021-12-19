/**
 * (Physics) 08: Simulation of free fall
 */

#include <functional>

#include <glab/glab.hpp>
#include <glab/shape.hpp>
#include <glab/grid.hpp>
#include <glab/time.hpp>

int main()
{
  Camera* camera = new Camera(glm::vec3(20.0f, 20.0f, 20.0f), glm::vec3(0.0f, 1.0f, 0.0f), -135.0f, -45.0f);
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1200.0f / 800.0f, 0.1f, 100.0f);

  // Initial setup
  glab::setup(/* Width */ 1200, /* Height*/ 800, "glab", camera, projection);

  glab::Grid grid(100.0f);

  glab::Cube  cube(10.0f, 100.0f, 10.0f, 5.0f);
  glab::Cube cube2(10.0f, 100.0f, 10.0f, 5.0f);

  glab::Clock clock;
  while (!glfwWindowShouldClose(window))
  {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glab::processInput(clock.tick());
    std::cout << clock.t << std::endl;

    std::function<void(double)> motion = [&cube](double t) {
      float h = cube.y0-9.8/2*t*t;
      if (h >= 0)
        cube.setPos(cube.x0,    h, cube.z0);
      else
        cube.setPos(cube.x0, 0.0f, cube.z0);
    };
    cube.move(motion, clock.t);
    cube.draw(/* needGuide = */ true);

    cube2.draw(/* needGuide = */ true);

    grid.draw();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}
