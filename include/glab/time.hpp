#ifndef _GLAB_TIME
#define _GLAB_TIME

namespace glab
{
  class Clock
  {
  public:
    float t, dt;
    void start() { glfwSetTime(0.0f); t = glfwGetTime(); }
    float tick()
    {
      float ct = glfwGetTime();
      dt = ct - t;
       t = ct;
      return dt;
    }
  };
  Clock clock;
}

#endif
