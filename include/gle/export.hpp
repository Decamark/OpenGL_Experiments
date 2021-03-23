#ifndef _GLE_EXPORT
#define _GLE_EXPORT

#include <gle/gle.hpp>

namespace gle
{
  void export_to_png(GLFWwindow* window, const char* path)
  {
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);
  }
}

#endif
