#include <Icosahedron/util/Noise.h>


namespace ic::Noise { namespace {
    void load_permutation_lookup() {
        memset(permutations, 0, sizeof(permutations));

        for (int i = 0; i < 256; i++) {
            permutations[i] = i;
        }

        // Shuffle element indices
        for (int i = 256 - 1; i > 0; i--) {
            int index = rand() % (256 - 1);
            uint8_t temporary = permutations[i];

            permutations[i] = permutations[index];
            permutations[index] = temporary;
        }

        for (int i = 0; i < 256; i++) {
            permutations[i + 256 - 1] = permutations[i];
        }
    }

    ic::Vec2f get_2D_vector_from_permutation(uint8_t value) {
        uint8_t a = value & 3;
        switch (a) {
            case 0:  return {  1.0f, 1.0f  };
            case 1:  return { -1.0f, 1.0f  };
            case 2:  return { -1.0f, -1.0f };
            default: return {  1.0f, -1.0f };
        }
    }

    float get_value_from_permutation(uint8_t value) {
        uint8_t a = value & 3;

        if (a == 0) {
            return 1.0f;
        } else if (a == 1) {
            return -1.0f;
        } else if (a == 2) {
            return -1.0f;
        }

        return 1.0f;
    }
}}


void ic::Noise::init() {
    if (loaded) {
        printf("Noise library was already loaded!\n");
        return;
    }
    load_permutation_lookup();
    loaded = true;
}


float ic::Noise::perlin_1D(float position, bool normalized) {
    uint8_t np = (uint8_t) floor(position) & 255;
    int p = np + 1;
    float dp = position - (float) np;
    
    uint8_t vCorner1 = permutations[permutations[p]];
    uint8_t vCorner2 = permutations[permutations[np]];
    
    float corner1 = dp - 1.0f;
    float corner2 = dp;
    float perm1 = get_value_from_permutation(vCorner1);
    float perm2 = get_value_from_permutation(vCorner2);
    
    float dot1 = corner1 * perm1;
    float dot2 = corner2 * perm2;
    
    float fx = ic::Interpolation::smoothstep(dp);
    
    float result = ic::Mathf::interpolate(dot1, dot2, fx);
    if (normalized) {
        result = (result + 1.0f) * 0.5f;
    }
    return result;
}


float ic::Noise::perlin_2D(float x, float y, bool normalized) {
    uint8_t nx = (uint8_t) floor(x) & 255;
    uint8_t ny = (uint8_t) floor(y) & 255;
    int px = nx + 1;
    int py = ny + 1;

    float dx = x - (float) nx;
    float dy = y - (float) ny;
    
    uint8_t vCorner1 = permutations[permutations[px] + py];
    uint8_t vCorner2 = permutations[permutations[nx] + py];
    uint8_t vCorner3 = permutations[permutations[px] + ny];
    uint8_t vCorner4 = permutations[permutations[nx] + ny];
    
    ic::Vec2f corner1 = { dx - 1.0f, dy - 1.0f };
    ic::Vec2f corner2 = { dx       , dy - 1.0f };
    ic::Vec2f corner3 = { dx - 1.0f, dy        };
    ic::Vec2f corner4 = { dx       , dy        };
    
    ic::Vec2f perm1 = get_2D_vector_from_permutation(vCorner1);
    ic::Vec2f perm2 = get_2D_vector_from_permutation(vCorner2);
    ic::Vec2f perm3 = get_2D_vector_from_permutation(vCorner3);
    ic::Vec2f perm4 = get_2D_vector_from_permutation(vCorner4);
    
    float dot1 = corner1.dot(perm1);
    float dot2 = corner2.dot(perm2);
    float dot3 = corner3.dot(perm3);
    float dot4 = corner4.dot(perm4);
    float fx = ic::Interpolation::smoothstep(dx);
    float fy = ic::Interpolation::smoothstep(dy);
    
    float result = ic::Mathf::interpolate(
        ic::Mathf::interpolate(dot4, dot2, fy),
        ic::Mathf::interpolate(dot3, dot1, fy),
        fx
    );
    if (normalized) {
        result = (result + 1.0f) * 0.5f;
    }
    return result;
}