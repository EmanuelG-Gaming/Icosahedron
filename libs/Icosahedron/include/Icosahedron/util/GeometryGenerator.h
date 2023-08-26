#ifndef IC_GEOMETRY_GENERATOR_H
#define IC_GEOMETRY_GENERATOR_H

#include <vector>


#include <Icosahedron/math/geom/Vectors.h>
#include <Icosahedron/math/geom/Polygon.h>

#include <Icosahedron/math/Mathf.h>

namespace ic {
    /** @brief Utilities for procedurally loading geometry. */
    class GeometryGenerator {
public:
        static GeometryGenerator& get() {
            static GeometryGenerator ins;
            return ins;
        }

        std::vector<float> generate_regular_polygon(int sides = 3, float radius = 1.0f) {
            std::vector<float> vertices;
            float angle = 0.0f;

            for (int i = 0; i < sides; i++) {
                angle += (2 * ic::Mathf::get().pi / sides);
                float x = ic::Mathf::get().cosf(angle) * radius;
                float y = ic::Mathf::get().sinf(angle) * radius;

                vertices.push_back(x);
                vertices.push_back(y);
            }

            return vertices;
        }

        std::vector<float> generate_rectangle(float width, float height) {
            std::vector<float> vertices = {
                -width, height,
                width, height,
                width, -height,
                -width, -height
            };

            return vertices;
        }

        std::vector<float> generate_UV_rectangle(float scaleX = 1.0f, float scaleY = 1.0f) {
            std::vector<float> textureCoords = {
                0.0f, 0.0f, 
                scaleX, 0.0f, 
                scaleX, -scaleY, 
                0.0f, -scaleY
            };

            return textureCoords;
        }
        
        std::vector<float> generate_UV_polygon(const std::vector<float> &verts, float magnification = 1.0f) {
            float width = 0, height = 0;

            float minX = verts[0];
            float minY = verts[1];
            float maxX = verts[0];
            float maxY = verts[1];

            for (int i = 0; i < verts.size(); i += 2) {
                float dx = verts[i];
                float dy = verts[i + 1];

                minX = minX > dx ? dx : minX;
                minY = minY > dy ? dy : minY;

                maxX = maxX < dx ? dx : maxX;
                maxY = maxY < dy ? dy : maxY;
            }

            width = maxX - minX;
            height = maxY - minY;

            std::vector<float> textureCoords;
            float scale = 2.0f / magnification;
            for (int i = 0; i < verts.size(); i += 2) {
                float x = verts[i], y = verts[i + 1];

                float nx = (x / width + 1) * scale;
                float ny = (y / height + 1) * scale;

                ny *= -1;
                textureCoords.push_back(nx);
                textureCoords.push_back(ny);
            }
            return textureCoords;
        }

        std::vector<float> generate_parallelipiped(float width, float height, float depth) {
            std::vector<float> vertices = {
                // Front
                -0.5, -0.5, 0.5,
                0.5, -0.5, 0.5, 
                0.5, 0.5, 0.5,  
                -0.5, 0.5, 0.5, 

                // Top
                -0.5, 0.5, 0.5, 
                0.5, 0.5, 0.5,  
                0.5, 0.5, -0.5, 
                -0.5, 0.5, -0.5,

                // Back
                0.5, -0.5, -0.5, 
                -0.5, -0.5, -0.5,
                -0.5, 0.5, -0.5, 
                0.5, 0.5, -0.5, 
                
                // Bottom
                -0.5, -0.5, -0.5,
                0.5, -0.5, -0.5,
                0.5, -0.5, 0.5, 
                -0.5, -0.5, 0.5,

                // Left
                -0.5, -0.5, -0.5,
                -0.5, -0.5, 0.5, 
                -0.5, 0.5, 0.5,  
                -0.5, 0.5, -0.5, 

                // Right
                0.5, -0.5, 0.5, 
                0.5, -0.5, -0.5,
                0.5, 0.5, -0.5, 
                0.5, 0.5, 0.5,  
            };

            for (int i = 0; i < vertices.size(); i+=3) {
                vertices[i] *= width * 2;
                vertices[i + 1] *= height * 2;
                vertices[i + 2] *= depth * 2;
            }

            return vertices;
        }

        std::vector<float> generate_cube(float size = 0.5f) {
            return generate_parallelipiped(size, size, size);
        }

        std::vector<unsigned int> generate_parallelipiped_indices() {
            std::vector<unsigned int> indices = {
                // Front
                0, 1, 2,
                2, 3, 0,
                // Top
                4, 5, 6,
                6, 7, 4,
                // Back
                8, 9, 10,
                10, 11, 8,
                // Bottom
                12, 13, 14,
                14, 15, 12,
                // Left
                16, 17, 18,
                18, 19, 16,
                // Right
                20, 21, 22,
                22, 23, 20
            };

            return indices;
        }

        std::vector<float> generate_UV_parallelipiped(float width = 1.0f, float height = 1.0f, float depth = 1.0f) {
            return { 
                0.0, 0.0,
                width, 0.0,
                width, height,
                0.0, height,
            
                0.0, 0.0,
                width, 0.0,
                width, depth,
                0.0, depth,
                
                0.0, 0.0,
                width, 0.0,
                width, height,
                0.0, height,
                
                0.0, 0.0,
                width, 0.0,
                width, depth,
                0.0, depth,
            
                0.0, 0.0,
                depth, 0.0,
                depth, height,
                0.0, height,
                
                0.0, 0.0,
                depth, 0.0,
                depth, height,  
                0.0, height
            };
        }

        std::vector<float> generate_normal_parallelipiped() {
            return {
                0.0, 0.0, 1.0,  
                0.0, 0.0, 1.0,  
                0.0, 0.0, 1.0,  
                0.0, 0.0, 1.0,  
              
                0.0, 1.0, 0.0,  
                0.0, 1.0, 0.0, 
                0.0, 1.0, 0.0,  
                0.0, 1.0, 0.0, 
            
                0.0, 0.0, -1.0,
                0.0, 0.0, -1.0,
                0.0, 0.0, -1.0,
                0.0, 0.0, -1.0, 
            
                0.0, -1.0, 0.0,
                0.0, -1.0, 0.0,
                0.0, -1.0, 0.0, 
                0.0, -1.0, 0.0, 
            
                -1.0, 0.0, 0.0,
                -1.0, 0.0, 0.0,
                -1.0, 0.0, 0.0, 
                -1.0, 0.0, 0.0,
            
                1.0, 0.0, 0.0,  
                1.0, 0.0, 0.0, 
                1.0, 0.0, 0.0, 
                1.0, 0.0, 0.0
            };
        }

private:
        GeometryGenerator() {}
        ~GeometryGenerator() {}
public:
        GeometryGenerator(GeometryGenerator const&) = delete;
        void operator = (GeometryGenerator const&) = delete;
    };
}
#endif