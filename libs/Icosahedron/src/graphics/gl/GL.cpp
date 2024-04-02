#include <Icosahedron/graphics/gl/GL.h>


void ic::GL::enable_depth_testing(ic::GLDepthSettings depth) {
    enable(ic::DEPTH_TESTING);
    set_depth_testing(depth);
}
void ic::GL::enable_blending(ic::GLBlendingSource source, ic::GLBlendingDestination dest) {
    enable(ic::BLENDING);
    set_blending(source, dest);
}
void ic::GL::enable_face_culling(ic::GLCullSettings culling, ic::GLFaceWindingOrder winding) {
    enable(ic::FACE_CULLING);
    set_face_culling(culling, winding);
}


void ic::GL::set_depth_testing(ic::GLDepthSettings depth) {
    glDepthFunc(depth);
}
void ic::GL::set_blending(ic::GLBlendingSource source, ic::GLBlendingDestination dest) {
    glBlendFunc(source, dest);
}
void ic::GL::set_face_culling(ic::GLCullSettings culling, ic::GLFaceWindingOrder winding) {
    glCullFace(culling);
    glFrontFace(winding);
}

void ic::GL::set_viewport(int w, int h) {
    glViewport(0, 0, w, h);
}

void ic::GL::enable(ic::GLContextSettings setting) {
    glEnable(setting);
}

void ic::GL::disable(ic::GLContextSettings setting) {
    glDisable(setting);
}


void ic::GL::disable_depth_testing() {
    disable(ic::DEPTH_TESTING);
}
void ic::GL::disable_blending() {
    disable(ic::BLENDING);
}
void ic::GL::disable_face_culling() {
    disable(ic::FACE_CULLING);
}


void ic::GL::clear_color() {
    clear_color(0.0f, 0.0f, 0.0f);
}

void ic::GL::clear_color(float r, float g, float b) {
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ic::GL::clear_color(const ic::Color &color) {
    float r = color.r / 255.0f;
    float g = color.g / 255.0f;
    float b = color.b / 255.0f;
    
    clear_color(r, g, b);
}