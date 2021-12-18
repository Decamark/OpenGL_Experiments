/**
 * (Math) 11: Detect intersections between a ray and a triangle
 */

#include <functional>

#include <glab/glab.hpp>
#include <glab/shape.hpp>
#include <glab/grid.hpp>
#include <glab/time.hpp>

#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>

auto* window = glab::setup(1200, 800, "glab");
Camera camera(window, glm::vec3(15.0f, 25.0f, 35.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -25.0f);

int main()
{
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  // tell GLFW to hide our mouse
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);

  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1200.0f / 800.0f, 0.1f, 100.0f);

  glab::Grid grid(100.0f);
  grid.setP(projection);

  glab::Triangle tri(10, 10, 10, 5);
  tri.setP(projection);

  glab::Cube cube(20, 20, 20, 5);
  cube.setP(projection);

  glab::Clock clock;
  while (!glfwWindowShouldClose(window))
  {
    float dt = clock.tick();
    camera.processInput(dt);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Slightly sliding the camera's position
    glab::Line ray(camera.Position+glm::vec3(0.1f, 0.1f, 0.1f), {1.0f, 0.0f, 0.0f}, camera.Position+camera.Front, {0.0f, 1.0f, 0.0f});
    ray.setP(projection);
    ray.setV(camera.GetViewMatrix());
    ray.draw();

    tri.setV(camera.GetViewMatrix());
    if (tri.intersect(camera.Position, camera.Front))
      tri.rasterization = GL_FILL;
    else
      tri.rasterization = GL_LINE;
    
    tri.draw();
    tri.drawGuide();

    cube.setV(camera.GetViewMatrix());
    if (cube.intersect(camera.Position, camera.Front))
      cube.rasterization = GL_FILL;
    else
      cube.rasterization = GL_LINE;
    
    cube.draw();
    cube.drawGuide();

    grid.setV(camera.GetViewMatrix());
    grid.draw();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}
