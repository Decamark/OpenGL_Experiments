/**
 * (Math > Geometry) Rotations
 */

#include <functional>

#include <glm/gtx/euler_angles.hpp>

#include <glab/glab.hpp>
#include <glab/shape.hpp>
#include <glab/grid.hpp>
#include <glab/time.hpp>

int main()
{
  glab::initWindow(/* Width */ 1200, /* Height*/ 800, "glab");

  Camera camera0 = Camera(glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(0.0f, 1.0f, 0.0f), -135.0f, -30.0f);
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1200.0f / 800.0f, 0.1f, 100.0f);
  camera0 |= projection;
  camera  |= camera0;

  glab::Grid grid(100.0f);

  glab::Cube cube1(15.0f, 15.0f, 15.0f, 10.0f);
  glab::Cube cube2(30.0f, 15.0f, 15.0f, 10.0f);
  glab::Cube cube3(45.0f, 15.0f, 15.0f, 10.0f);
  glab::Cube cube4(60.0f, 15.0f, 15.0f, 10.0f);
  glab::Cube cube5(75.0f, 15.0f, 15.0f, 10.0f);
  glab::Cube cube6(90.0f, 15.0f, 15.0f, 10.0f);
  cube6.rotation *= glm::eulerAngleY(glm::radians(90.0f));

  glab::clock.start();
  while (!glfwWindowShouldClose(window))
  {
    float dt = glab::clock.tick();
    float  t = glab::clock.t;

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    grid.draw();

    // Rotate on the spot
    cube1.rotate(90.0f*dt, {0.0f, 1.0f, 0.0f});
    cube1.draw(/* needGuide */ true);

    // Rotate around (0.0f, 1.0f, 1.0f) with cube2.Position+(5.0f, 5.0f, 5.0f) as the origin
    cube2.rotateAround(90.0f*dt, {0.0f, 1.0f, 1.0f}, {5.0f, 5.0f, 5.0f});
    cube2.draw(/* needGuide */ true);

    // Rotate around z-axis. All of the GLM's matrices are inversed
    // so I decided to take the inverse of the normal definition because it's easier to understand.
    glm::mat4 rotX = glm::inverse(glm::mat4(
        1,                                0,                                 0, 0,
        0, glm::cos(glm::radians(90.0f*dt)), -glm::sin(glm::radians(90.0f*dt)), 0,
        0, glm::sin(glm::radians(90.0f*dt)),  glm::cos(glm::radians(90.0f*dt)), 0,
        0,                                0,                                 0, 1
    ));

    glm::mat4 rotY = glm::inverse(glm::mat4(
         glm::cos(glm::radians(90.0f*dt)), 0, glm::sin(glm::radians(90.0f*dt)), 0,
                                        0, 1,                                0, 0,
        -glm::sin(glm::radians(90.0f*dt)), 0, glm::cos(glm::radians(90.0f*dt)), 0,
                                        0, 0,                                0, 1
    ));

    glm::mat4 rotZ = glm::inverse(glm::mat4(
       glm::cos(glm::radians(90.0f*dt)), -glm::sin(glm::radians(90.0f*dt)), 0, 0,
       glm::sin(glm::radians(90.0f*dt)),  glm::cos(glm::radians(90.0f*dt)), 0, 0,
                                      0,                                 0, 1, 0,
                                      0,                                 0, 0, 1
    ));

    // The effect of transformations propagates from the left.
    // So this rotates around the x-axis first and in the view of the orientation after the rotation,
    // apply the rotation around the z-axis.
    // This is what Shape::rotate() does.
    cube3.rotation = cube3.rotation * rotX * rotY * rotZ;
    cube3.draw(/* needGuide */ true);

    cube4.rotate(90.0f*dt, {1.0f, 0.0f, 0.0f});
    cube4.rotate(90.0f*dt, {0.0f, 1.0f, 0.0f});
    cube4.rotate(90.0f*dt, {0.0f, 0.0f, 1.0f});
    cube4.draw(/* needGuide */ true);

    // Apply rotations in the reversed order
    cube5.rotation = cube5.rotation * rotZ * rotY * rotX;
    cube5.draw(/* needGuide */ true);

    // Gimbal lock
    // Rotations around x-axis and z-axis become identical
    static float  yaw = 0.0f;
    static float roll = 0.0f;
    cube6.rotation = glm::eulerAngleX(glm::radians(yaw)) * glm::eulerAngleY(glm::radians(90.0f)) * glm::eulerAngleZ(glm::radians(roll));
    cube6.draw(/* needGuide */ true);
     yaw += 90.0f*dt;
    roll += 90.0f*dt;

    glfwSwapBuffers(window);
    glfwPollEvents();
    glab::getKeyWASD();
  }
}
