#ifndef _GLE_EXPORT
#define _GLE_EXPORT

#include <gle/gle.hpp>

namespace gle
{
  void export_to_png(GLFWwindow* window, const char* filepath)
  {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    GLsizei nr_channels = 4;
    GLsizei stride = nr_channels * width;
    stride += (stride % 4) ? (4 - stride % 4) : 0;
    GLsizei bufsize = stride * height;
    std::vector<char> buffer(bufsize);
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer.data());
    stbi_flip_vertically_on_write(true);
    stbi_write_png(filepath, width, height, nr_channels, buffer.data(), stride);
  }
}

#endif
