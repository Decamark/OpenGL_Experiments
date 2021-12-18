/**
 * (Math) 01: Draw a line
 */

#include <glab/glab.hpp>
#include <glab/shape.hpp>
#include <glab/grid.hpp>
#include <glab/time.hpp>

int main()
{
  Camera* camera = new Camera(glm::vec3(8.0f, 12.0f, 15.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -30.0f);
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1200.0f / 800.0f, 0.1f, 100.0f);

  // Initial setup
  glab::setup(/* Width */ 1200, /* Height*/ 800, "glab", camera, projection);

  glab::Grid grid(100.0f);

  glab::Line l(std::vector<float>({5, 5, 5}), {1, 0, 0}, {10, 10, 10}, {0, 1, 0});

  glab::Clock clock;
  while (!glfwWindowShouldClose(window))
  {
    float dt = clock.tick();
    glab::processInput(dt);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    grid.draw();

    l.draw(/* needGuide = */ true);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}
