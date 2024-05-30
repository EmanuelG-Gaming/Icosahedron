#include <Icosahedron/util/GeometryGenerator.h>

std::vector<float> ic::GeometryGenerator::generate_regular_polygon(int sides, float radius) {
    std::vector<float> vertices;
    float angle = 0.0f;

    for (int i = 0; i < sides; i++) {
        angle += (2 * ic::Mathf::pi / sides);
        float x = ic::Mathf::cosf(angle) * radius;
        float y = ic::Mathf::sinf(angle) * radius;

        vertices.push_back(x);
        vertices.push_back(y);
    }

    return vertices;
}

std::vector<float> ic::GeometryGenerator::generate_rectangle(float width, float height) {
    std::vector<float> vertices = {
        -width, height,
        width, height,
        width, -height,
        -width, -height
    };

    return vertices;
}

std::vector<float> ic::GeometryGenerator::generate_UV_rectangle(float scaleX, float scaleY) {
    std::vector<float> textureCoords = {
        0.0f, 0.0f, 
        scaleX, 0.0f, 
        scaleX, scaleY, 
        0.0f, scaleY
    };

    return textureCoords;
}

std::vector<float> ic::GeometryGenerator::generate_UV_polygon(const std::vector<float> &verts, float magnification) {
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

std::vector<float> ic::GeometryGenerator::generate_parallelipiped(float width, float height, float depth) {
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

std::vector<float> ic::GeometryGenerator::generate_cube(float size) {
    return generate_parallelipiped(size, size, size);
}

std::vector<unsigned int> ic::GeometryGenerator::generate_parallelipiped_indices() {
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

std::vector<float> ic::GeometryGenerator::generate_UV_parallelipiped(float width, float height, float depth) {
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

std::vector<float> ic::GeometryGenerator::generate_normal_parallelipiped() {
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

std::vector<float> ic::GeometryGenerator::generate_UV_sphere(float radius, int latitudeLines, int longitudeLines) {
    std::vector<float> result;

    float latitudeSpacing = ic::Mathf::pi / (float) latitudeLines;
    float longitudeSpacing = 2 * ic::Mathf::pi / (float) longitudeLines;

    for (int i = 0; i <= latitudeLines; i++) {
        for (int j = 0; j <= longitudeLines; j++) {
            float theta = j * longitudeSpacing;
            float phi = ic::Mathf::pi * 0.5f - i * latitudeSpacing;

            result.push_back(ic::Mathf::cosf(phi) * ic::Mathf::cosf(theta) * radius);
            result.push_back(ic::Mathf::cosf(phi) * ic::Mathf::sinf(theta) * radius);
            result.push_back(ic::Mathf::sinf(phi) * radius);
        }
    }
    return result;
}

std::vector<float> ic::GeometryGenerator::generate_UV_sphere_normals(int latitudeLines, int longitudeLines) {
    std::vector<float> result;

    float latitudeSpacing = ic::Mathf::pi / (float) latitudeLines;
    float longitudeSpacing = 2 * ic::Mathf::pi / (float) longitudeLines;

    for (int i = 0; i <= latitudeLines; i++) {
        for (int j = 0; j <= longitudeLines; j++) {
            float theta = j * longitudeSpacing;
            float phi = ic::Mathf::pi * 0.5f - i * latitudeSpacing;

            result.push_back(ic::Mathf::cosf(phi) * ic::Mathf::cosf(theta));
            result.push_back(ic::Mathf::cosf(phi) * ic::Mathf::sinf(theta));
            result.push_back(ic::Mathf::sinf(phi));
        }
    }
    return result;
}

std::vector<float> ic::GeometryGenerator::generate_UV_sphere_UVs(int latitudeLines, int longitudeLines) {
    std::vector<float> result;

    float latitudeSpacing = ic::Mathf::pi / (float) latitudeLines;
    float longitudeSpacing = 2 * ic::Mathf::pi / (float) longitudeLines;

    for (int i = 0; i <= latitudeLines; i++) {
        for (int j = 0; j <= longitudeLines; j++) {
            float theta = j * longitudeSpacing;
            float phi = ic::Mathf::pi * 0.5f - i * latitudeSpacing;

            ic::Vec3f position = {
                ic::Mathf::cosf(phi) * ic::Mathf::cosf(theta),
                ic::Mathf::cosf(phi) * ic::Mathf::sinf(theta),
                ic::Mathf::sinf(phi)
            };

            result.push_back(0.5f + atan2(position.z(), position.x()) / (2 * ic::Mathf::pi));
            result.push_back(0.5f + asin(position.y()) / ic::Mathf::pi);
        }
    }
    return result;
}


std::vector<unsigned int> ic::GeometryGenerator::generate_UV_sphere_indices(int latitudeLines, int longitudeLines) {
    std::vector<unsigned int> result;

    int k1, k2;
    for (int i = 0; i < latitudeLines; i++) {
        k1 = i * (latitudeLines + 1);
        k2 = k1 + latitudeLines + 1;
        for (int j = 0; j < longitudeLines; j++, k1++, k2++) {
            if (i != 0) {
                result.push_back(k1);
                result.push_back(k2);
                result.push_back(k1 + 1);
            }
            if (i != (latitudeLines - 1)) {
                result.push_back(k1 + 1);
                result.push_back(k2); 
                result.push_back(k2 + 1);
            }
        }
    }

    return result;
}
std::vector<float> ic::GeometryGenerator::generate_cone(ic::Vec3f &height, float baseRadius, int baseSides) {
    std::vector<float> result;
    result.push_back(height.x());
    result.push_back(height.y());
    result.push_back(height.z());
    float angle = 0.01f;
    for (int i = 0; i < baseSides; i++) {
        angle += (2 * ic::Mathf::pi / baseSides);
        
        ic::Vec3f forward = { ic::Mathf::cosf(angle), 0.0f, ic::Mathf::sinf(angle) };
        ic::Vec3f baseVertex = height.crs(forward).nor();
        baseVertex = baseVertex * baseRadius;
        result.push_back(baseVertex.x());
        result.push_back(baseVertex.y());
        result.push_back(baseVertex.z());
    }
    return result;
}
std::vector<unsigned int> ic::GeometryGenerator::generate_cone_indices(int baseSides, bool hasCap) {
    std::vector<unsigned int> result;
    // Side geometry
    for (int i = 1; i < baseSides - 1; i++) {
        result.push_back(0);
        result.push_back(i);
        result.push_back(i + 1);
    }
    // Triangulate vertices at the base
    if (hasCap) {
        for (int i = 1; i < baseSides; i++) {
            result.push_back(i);
            result.push_back(i + 1);
            result.push_back(i + 2);
        }
    }
    return result;
}
// ---------------- Shorthand notations ----------------

ic::Mesh ic::GeometryGenerator::generate_mesh(std::vector<float> &positions) {
    ic::Mesh result;
    result.add_attribute(0, 2, positions);
    result.add_attribute(2, 2, generate_UV_polygon(positions));
    result.set_index_buffer(ic::EarClippingTriangulation::triangulate(positions));

    return result;
}

ic::Mesh ic::GeometryGenerator::generate_regular_polygon_mesh(int sides, float radius) {
    std::vector<float> positions = generate_regular_polygon(sides, radius);
    
    ic::Mesh result;
    result.add_attribute(0, 2, positions);
    result.add_attribute(2, 2, generate_UV_polygon(positions));
    result.set_index_buffer(ic::EarClippingTriangulation::triangulate(positions));

    return result;
}


ic::Mesh ic::GeometryGenerator::generate_rectangle_mesh(float width, float height, float uScale, float vScale) {
    ic::Mesh result;
    result.add_attribute(0, 2, generate_rectangle(width, height));
    result.add_attribute(2, 2, generate_UV_rectangle(uScale, vScale));
    result.set_index_buffer({ 0, 1, 2, 0, 2, 3 });

    return result;
}


ic::Mesh ic::GeometryGenerator::generate_parallelipiped_mesh(float width, float height, float depth, float sScale, float tScale, float uScale) {
    ic::Mesh result;
    result.add_attribute(0, 3, generate_parallelipiped(width, height, depth));
    result.add_attribute(2, 2, generate_UV_parallelipiped(sScale, tScale, uScale));
    result.add_attribute(3, 3, generate_normal_parallelipiped());
    result.set_index_buffer(generate_parallelipiped_indices());

    return result;
}

ic::Mesh ic::GeometryGenerator::generate_cube_mesh(float length, float sScale, float tScale, float uScale) {
    return generate_parallelipiped_mesh(length, length, length, sScale, tScale, uScale);
}
ic::Mesh ic::GeometryGenerator::generate_cube_mesh(float length, float texCoordScale) {
    return generate_parallelipiped_mesh(length, length, length, texCoordScale, texCoordScale, texCoordScale);
}


ic::Mesh ic::GeometryGenerator::generate_UV_sphere_mesh(float radius, int latitudeLines, int longitudeLines) {
    ic::Mesh result;
    result.add_attribute(0, 3, generate_UV_sphere(radius, latitudeLines, longitudeLines));
    result.add_attribute(2, 2, generate_UV_sphere_UVs(latitudeLines, longitudeLines));
    result.add_attribute(3, 3, generate_UV_sphere_normals(latitudeLines, longitudeLines));
    result.set_index_buffer(generate_UV_sphere_indices(latitudeLines, longitudeLines));

    return result;
}
ic::Mesh ic::GeometryGenerator::generate_cone_mesh(ic::Vec3f height, float baseRadius, int baseSides, bool hasCap) {
    ic::Mesh result;
    
    result.add_attribute(0, 3, generate_cone(height, baseRadius, baseSides));
    result.set_index_buffer(generate_cone_indices(baseSides, hasCap));
    return result;
}