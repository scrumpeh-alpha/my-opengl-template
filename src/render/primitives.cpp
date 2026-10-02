#include "primitives.h"

Mesh Primitives::makeCube(std::vector<Texture> textures) {
    return Mesh(
        std::vector<Vertex>(VERTICES.begin(), VERTICES.end()),
        std::vector<uint32_t>(INDICES.begin(), INDICES.end()),
        std::move(textures)
    );
}
