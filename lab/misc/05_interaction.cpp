/**
 * (Misc) 05: Interaction
 */

#include <glab/glab.hpp>
#include <glab/shape.hpp>
#include <glab/grid.hpp>
#include <glab/time.hpp>

#include <learnopengl/camera.h>

glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);

// Convert a coordinate in screen space to the one in eye space
std::pair<double,double> screen2eye(double xpos, double ypos)
{
  // The height and the width of the near plane
  float nh = 2 * 0.1 * glm::tan(glm::radians(45.0f/2));
  float nw = 1200.0f/800.0f * nh;

  float xe = (xpos - 1200/2) * nw/1200.0f;
  float ye = (800/2 - ypos) * nh/800.0f;
  return {xe, ye};
}

// Joey calculates yaw and pitch in the "screen space" to implement Camera.
// ref: https://learnopengl.com/Getting-started/Camera
// This works fine if the cursor's position doesn't matter
// but I wanted to judge the intersection between an object and the cursor
// and with Joey's method, it looks weird because the point of the Front vector
// isn't covered with the cursor!
// So, I needed to handle the cursor as if it's in the "eye space" to make it look more natural.
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
  auto [xe,ye] = screen2eye(xpos, ypos);
  Front = {xe, ye, -0.1f};
}

int main()
{
  glab::initWindow(1200, 800, "glab", false, false, true, true);
  glfwSetCursorPosCallback(window, cursor_position_callback);

  Camera camera0 = Camera(glm::vec3(0.0f, 0.0f, 15.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1200.0f / 800.0f, 0.1f, 100.0f);
  camera0 |= projection;
  camera  |= camera0;

  glab::Cube cube(0.0f, 0.0f, 0.0f, 5.0f);

  glab::clock.start();
  while (!glfwWindowShouldClose(window))
  {
    glab::clock.tick();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glab::Line ray((*camera).Position+glm::vec3(0.0f, 0.0f, -0.1f), {1.0f, 0.0f, 0.0f},
                   (*camera).Position+Front,                        {0.0f, 1.0f, 0.0f});
    ray.draw();

    if (cube.intersect((*camera).Position, Front))
      cube.rasterization = GL_FILL;
    else
      cube.rasterization = GL_LINE;
    cube.draw();

    glfwSwapBuffers(window);
    glfwPollEvents();
    glab::getKeyWASD();
  }
}
