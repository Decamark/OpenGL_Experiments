/**
 * (Physics) 09: Camera
 */

#include <functional>

#include <glab/glab.hpp>
#include <glab/shape.hpp>
#include <glab/grid.hpp>
#include <glab/time.hpp>

int main()
{
  // Initial setup
  Camera* camera0 = new Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f);
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 2.0f, 100.0f);
  glab::setup(/* Width */ 800, /* Height*/ 800, "glab", camera0, projection);

  Camera* camera1 = new Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
  camera.push_back(camera1);

  glab::Grid grid(100.0f);

  glab::Cube cube(0.0f, 0.0f, -5.0f, 1.0f);

  glab::Clock clock;
  while (!glfwWindowShouldClose(window))
  {
    // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    CAMERA_SLOT_NO = 0; // This leads to camera0's view matrix updated
    glab::processInput(clock.tick());

    grid.draw();

    // ex 1) This is the same as looking towards -z at (0,0,0)
    // Compare e with camera[1]'s view
    glm::mat4 e = glm::mat4(1.0f);
    std::cout << "[identity]" << std::endl;
    glab::debug(e);
    std::cout << "[view]" << std::endl;
    glab::debug(camera[1]->GetViewMatrix());

    CAMERA_SLOT_NO = 1;
    cube.draw(/* needGuide = */ true);
    CAMERA_SLOT_NO = 0;

    // ex 2) About projection matrix
    // http://www.songho.ca/opengl/gl_projectionmatrix.html
    // https://github.com/g-truc/glm/blob/b3f87720261d623986f164b2a7f6a0a938430271/glm/ext/matrix_clip_space.inl#L248-L262
    std::cout << "[projection]" << std::endl;
    glab::debug(projection);

    // ex 3) The clip space is left-handed (as gl_Position is). That is,
    //   y z
    // x_|/
    //
    // So glm::perspective assumes the right-handed coordinates and produces the left-handed coordinates
    std::cout << "[Check that xz-axes are swithed by moving around]" << std::endl;
    glab::debug(projection * camera[0]->GetViewMatrix() * cube.model * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f));

    // ex 4) Camera is directed to the x-axis at first
    // This is because a vector (cos(theta), 0) is along the x-axis
    std::cout << "[Camera's direction]" << std::endl;
    glab::debug(camera[0]->GetViewMatrix());

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}
