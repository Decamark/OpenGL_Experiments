/**
 * (Physics) 07: Time
 */

#include <functional>

#include <glab/glab.hpp>
#include <glab/shape.hpp>
#include <glab/grid.hpp>
#include <glab/time.hpp>

int main()
{
  Camera* camera = new Camera(glm::vec3(25.0f, 25.0f, 25.0f), glm::vec3(0.0f, 1.0f, 0.0f), -135.0f, -45.0f);
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1200.0f / 800.0f, 0.1f, 100.0f);

  // Initial setup
  glab::setup(/* Width */ 1200, /* Height*/ 800, "glab", camera, projection);

  glab::Grid grid(50);

  glab::Cube cube(5.0f, 5.0f, 5.0f);

  glab::Clock clock;
  while (!glfwWindowShouldClose(window))
  {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glab::processInput(clock.tick());
    std::cout << clock.t << std::endl;

    std::function<void(double)> motion =
      [&cube](double t) {
        cube.translate({0.01f, glm::sin(t)*0.1, 0.01f});
      };
    cube.move(motion, clock.t);
    cube.draw(/* needGuide = */ true);

    grid.draw();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}
