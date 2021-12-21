/**
 * (Math) 11: Detect intersections between a ray and a triangle
 */

#include <functional>

#include <glab/glab.hpp>
#include <glab/shape.hpp>
#include <glab/grid.hpp>
#include <glab/time.hpp>

int main()
{
  Camera camera0 = Camera(glm::vec3(15.0f, 25.0f, 35.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -25.0f);
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1200.0f / 800.0f, 0.1f, 100.0f);
  camera0 |= projection;
  camera  |= camera0;

  glab::initWindow(/* Width */ 1200, /* Height*/ 800, "glab");

  glab::Grid grid(100.0f);

  glab::Triangle tri(10, 10, 10, 5);

  glab::Cube cube(20, 20, 20, 5);

  glab::Clock clock;
  while (!glfwWindowShouldClose(window))
  {
    float dt = clock.tick();
    glab::processInput(dt);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Slightly sliding the camera's position
    glab::Line ray((*camera).Position+glm::vec3(0.0f, -0.05f, 0.0f), {1.0f, 0.0f, 0.0f}, (*camera).Position+(*camera).Front, {0.0f, 1.0f, 0.0f});
    ray.draw();

    if (tri.intersect((*camera).Position, (*camera).Front))
      tri.rasterization = GL_FILL;
    else
      tri.rasterization = GL_LINE;
    
    tri.draw(/* needGuide = */ true);

    if (cube.intersect((*camera).Position, (*camera).Front))
      cube.rasterization = GL_FILL;
    else
      cube.rasterization = GL_LINE;
    
    cube.draw(/* needGuide = */ true);

    grid.draw();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}
