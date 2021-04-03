#version 330 core

float r = 200.0;

layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

void main()
{
    vec4 v0 = gl_in[0].gl_Position;
    // gl_Position = v0;
    // EmitVertex();
    vec4 v1 = gl_in[1].gl_Position;
    // gl_Position = v1;
    // EmitVertex();

    vec4 vs[4];

    // xy-plane
    if (v0.z == 0 && v1.z == 0)
    {
      // horizontal
      if (v0.y == v1.y) {
        vs[0] = vec4(v0.x, v0.y+r, v0.z, v0.w);
        vs[1] = vec4(v0.x, v0.y-r, v0.z, v0.w);
        vs[2] = vec4(v1.x, v1.y+r, v1.z, v1.w);
        vs[3] = vec4(v1.x, v1.y-r, v1.z, v1.w);
      }
      // vertical
      if (v0.x == v1.x) {
        vs[0] = vec4(v0.x+r, v0.y, v0.z, v0.w);
        vs[1] = vec4(v0.x-r, v0.y, v0.z, v0.w);
        vs[2] = vec4(v1.x+r, v1.y, v1.z, v1.w);
        vs[3] = vec4(v1.x-r, v1.y, v1.z, v1.w);
      }
    }

    // yz-plane
    if (v0.x == 0 && v1.x == 0)
    {
      // horizontal
      if (v0.y == v1.y) {
        vs[0] = vec4(v0.x, v0.y+r, v0.z, 1.0);
        vs[1] = vec4(v0.x, v0.y-r, v0.z, 1.0);
        vs[2] = vec4(v1.x, v1.y+r, v1.z, 1.0);
        vs[3] = vec4(v1.x, v1.y-r, v1.z, 1.0);
      }
      // vertical
      if (v0.z == v1.z) {
        vs[0] = vec4(v0.x, v0.y, v0.z+r, 1.0);
        vs[1] = vec4(v0.x, v0.y, v0.z-r, 1.0);
        vs[2] = vec4(v1.x, v1.y, v1.z+r, 1.0);
        vs[3] = vec4(v1.x, v1.y, v1.z-r, 1.0);
      }
    }

    // zx-plane
    if (v0.y == 0 && v1.y == 0)
    {
      // horizontal
      if (v0.z == v1.z) {
        vs[0] = vec4(v0.x, v0.y, v0.z+r, 1.0);
        vs[1] = vec4(v0.x, v0.y, v0.z-r, 1.0);
        vs[2] = vec4(v1.x, v1.y, v1.z+r, 1.0);
        vs[3] = vec4(v1.x, v1.y, v1.z-r, 1.0);
      }
      // vertical
      if (v0.x == v1.x) {
        vs[0] = vec4(v0.x+r, v0.y, v0.z, 1.0);
        vs[1] = vec4(v0.x-r, v0.y, v0.z, 1.0);
        vs[2] = vec4(v1.x+r, v1.y, v1.z, 1.0);
        vs[3] = vec4(v1.x-r, v1.y, v1.z, 1.0);
      }
    }

    for (int i=0; i<3; i++) {
      gl_Position = vs[i];
      EmitVertex();
    }
    // // gl_Position = vs[0];
    // // EmitVertex();
    // // gl_Position = vs[1];
    // // EmitVertex();
    // // gl_Position = vs[2];
    // // EmitVertex();
    // // gl_Position = vs[2];
    // // EmitVertex();
    // // gl_Position = vs[3];
    // // EmitVertex();
    // // gl_Position = vs[1];
    // // EmitVertex();
    EndPrimitive();
}
