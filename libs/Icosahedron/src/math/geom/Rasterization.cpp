#include <Icosahedron/math/geom/Rasterization.h>

namespace ic::Rasterization {
    namespace {
        void fill_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, const std::function<void(int, int)> &callback) {
            float inverseSlope1 = (x1 - x0) / (float) (y1 - y0);
            float inverseSlope2 = (x2 - x0) / (float) (y2 - y0);
        
            float currentX1 = x0, currentX2 = x0;
            
            for (int scanY = y0; scanY <= y1; scanY++) {
                float cx1 = currentX1;
                float cx2 = currentX2;
                if (cx2 < cx1) std::swap(cx1, cx2);
                
                // Draw horizontal line
                for (int i = (int) cx1; i <= (int) cx2; i++) {
                    callback(i, scanY);
                }
               
                currentX1 += inverseSlope1;
                currentX2 += inverseSlope2;
            }
        }
    
        void fill_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, const std::function<void(int, int)> &callback) {
            float inverseSlope1 = (x2 - x0) / (float) (y2 - y0);
            float inverseSlope2 = (x2 - x1) / (float) (y2 - y1);
        
            float currentX1 = x2, currentX2 = x2;
            
            for (int scanY = y2; scanY > y0; scanY--) {
                float cx1 = currentX1;
                float cx2 = currentX2;
                if (cx2 < cx1) std::swap(cx1, cx2);
                
                // Draw horizontal line
                for (int i = (int) cx1; i <= (int) cx2; i++) {
                    callback(i, scanY);
                }
        
                currentX1 -= inverseSlope1;
                currentX2 -= inverseSlope2;
            }
        }
    }
}

void ic::Rasterization::line(int x0, int y0, int x1, int y1, const std::function<void(int, int)> &callback) {
    int dx = abs(x1 - x0);
    int dy = -abs(y1 - y0);
    int displacementX = x0 < x1 ? 1 : -1;
    int displacementY = y0 < y1 ? 1 : -1;

    int error = dx + dy;

    while (true) {
        callback(x0, y0);
        if (x0 == x1 && y0 == y1) break;

        int twoTimesError = 2 * error;
        if (twoTimesError >= dy) {
            if (x0 == x1) break;

            error += dy;
            x0 += displacementX;
        }

        if (twoTimesError <= dx) {
            if (y0 == y1) break;

            error += dx;
            y0 += displacementY;
        }
    }
}

void ic::Rasterization::triangle(int x0, int y0, int x1, int y1, int x2, int y2, const std::function<void(int, int)> &with) {
    int ry0 = y0, ry1 = y1, ry2 = y2;
    int rx0 = x0, rx1 = x1, rx2 = x2;

    // Do vertex y sorting
    if (ry1 < ry0) { 
        std::swap(ry0, ry1); 
        std::swap(rx0, rx1); 
    }
    if (ry2 < ry0) { 
        std::swap(ry0, ry2); 
        std::swap(rx0, rx2);
    }
    if (ry2 < ry1) { 
        std::swap(ry1, ry2); 
        std::swap(rx1, rx2);
    }


    if (ry1 == ry2) {
        fill_bottom_triangle(rx0, ry0, rx1, ry1, rx2, ry2, with);
    } else if (ry0 == ry1) {
        fill_top_triangle(rx0, ry0, rx1, ry1, rx2, ry2, with);
    } else {
        ic::Vec2i v = { rx0 + (int) (((float) (ry1 - ry0) / (float)(ry2 - ry0)) * (rx2 - rx0)), ry1 };

        fill_bottom_triangle(rx0, ry0, rx1, ry1, v.x(), v.y(), with);
        fill_top_triangle(rx1, ry1, v.x(), v.y(), rx2, ry2, with);
    }
}         