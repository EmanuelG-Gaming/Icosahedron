#include <Icosahedron/graphics/gl/GLStateHandler.h>


void ic::GLStateHandler::enable_depth_testing(ic::GLDepthSettings depth) {
    enable(ic::DEPTH_TESTING);
    set_depth_testing(depth);
}
void ic::GLStateHandler::enable_blending(ic::GLBlendingSource source, ic::GLBlendingDestination dest) {
    enable(ic::BLENDING);
    set_blending(source, dest);
}
void ic::GLStateHandler::enable_face_culling(ic::GLCullSettings culling, ic::GLFaceWindingOrder winding) {
    enable(ic::FACE_CULLING);
    set_face_culling(culling, winding);
}


void ic::GLStateHandler::set_depth_testing(ic::GLDepthSettings depth) {
    glDepthFunc(depth);
}
void ic::GLStateHandler::set_blending(ic::GLBlendingSource source, ic::GLBlendingDestination dest) {
    glBlendFunc(source, dest);
}
void ic::GLStateHandler::set_face_culling(ic::GLCullSettings culling, ic::GLFaceWindingOrder winding) {
    glCullFace(culling);
    glFrontFace(winding);
}

void ic::GLStateHandler::set_viewport(int w, int h) {
    glViewport(0, 0, w, h);
}

void ic::GLStateHandler::enable(ic::GLContextSettings setting) {
    glEnable(setting);
}

void ic::GLStateHandler::disable(ic::GLContextSettings setting) {
    glDisable(setting);
}


void ic::GLStateHandler::disable_depth_testing() {
    disable(ic::DEPTH_TESTING);
}
void ic::GLStateHandler::disable_blending() {
    disable(ic::BLENDING);
}
void ic::GLStateHandler::disable_face_culling() {
    disable(ic::FACE_CULLING);
}