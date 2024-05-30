#ifndef IC_GEOMETRY_GENERATOR_H
#define IC_GEOMETRY_GENERATOR_H

#include <vector>


#include <Icosahedron/math/geom/Vectors.h>
#include <Icosahedron/math/geom/Polygon.h>
#include <Icosahedron/math/EarClippingTriangulation.h>

#include <Icosahedron/scene/Mesh.h>
//#include <Icosahedron/scene/3d/Mesh3D.h>

#include <Icosahedron/math/Mathf.h>


namespace ic {
    /** @brief Utilities for procedurally loading geometry. */
    namespace GeometryGenerator {
        std::vector<float> generate_regular_polygon(int sides = 3, float radius = 1.0f);
    
        std::vector<float> generate_rectangle(float width, float height);

        std::vector<float> generate_UV_rectangle(float scaleX = 1.0f, float scaleY = 1.0f);
        
        std::vector<float> generate_UV_polygon(const std::vector<float> &verts, float magnification = 1.0f);

        std::vector<float> generate_parallelipiped(float width, float height, float depth);

        std::vector<float> generate_cube(float size = 0.5f);

        std::vector<unsigned int> generate_parallelipiped_indices();
        std::vector<float> generate_UV_parallelipiped(float width = 1.0f, float height = 1.0f, float depth = 1.0f);

        std::vector<float> generate_normal_parallelipiped();

        std::vector<float> generate_UV_sphere(float radius, int latitudeLines, int longitudeLines);
    
        std::vector<float> generate_UV_sphere_normals(int latitudeLines, int longitudeLines);
    
        std::vector<float> generate_UV_sphere_UVs(int latitudeLines, int longitudeLines);
        std::vector<unsigned int> generate_UV_sphere_indices(int latitudeLines, int longitudeLines);

        std::vector<float> generate_cone(ic::Vec3f &height, float baseRadius, int baseSides);
        std::vector<unsigned int> generate_cone_indices(int baseSides, bool hasCap = true);


        // ---------------- Shorthand notations ----------------
    
        ic::Mesh generate_mesh(std::vector<float> &positions);
        ic::Mesh generate_regular_polygon_mesh(int sides = 3, float radius = 1.0f);
    
    
        ic::Mesh generate_rectangle_mesh(float width, float height, float uScale = 1.0f, float vScale = 1.0f);

        ic::Mesh generate_parallelipiped_mesh(float width, float height, float depth, float sScale = 1.0f, float tScale = 1.0f, float uScale = 1.0f);
    
        ic::Mesh generate_cube_mesh(float length, float sScale, float tScale, float uScale);
        ic::Mesh generate_cube_mesh(float length, float texCoordScale = 1.0f);
    
    
        ic::Mesh generate_UV_sphere_mesh(float radius, int latitudeLines, int longitudeLines);

        ic::Mesh generate_cone_mesh(ic::Vec3f height, float baseRadius, int baseSides, bool hasCap = true);
    };
}
#endif