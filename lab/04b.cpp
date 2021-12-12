/**
 * 04: Draw a line
 */

#include <gle/gle.hpp>
#include <gle/export.hpp>
#include <gle/shape.hpp>
#include <learnopengl/shader_m.h>
#include <unistd.h>

int main()
{
  const int WIDTH = 800, HEIGHT = 800;
  auto* window = gle::setup(WIDTH, HEIGHT, "Practice");

  Shader shader("exp/04.vs", "exp/04.fs");
  shader.use();

  gle::Tetragon tetra(
    { -0.95,  0.95, 0.0,
       0.95,  0.95, 0.0,
       0.95, -0.95, 0.0,
       0.95, -0.95, 0.0,
      -0.95, -0.95, 0.0,
      -0.95,  0.95, 0.0
    });

  // configure global opengl state
  // -----------------------------
  // glEnable(GL_DEPTH_TEST);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  tetra.draw();
  // glFlush();
    // tri.draw();

  // usleep(1000000);

  glfwSwapBuffers(window);

  gle::export_to_png(window, "./coord_tex.png");
}
