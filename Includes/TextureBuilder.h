
#pragma once
#include <unordered_map>
#include <memory>
#include <map>
#include "glm/vec2.hpp"
#include "Reflex-Core/TextureCubeMap.h"


struct aiMaterial;
enum aiTextureType;
struct aiString;

namespace Reflex {
    namespace Core {
        struct Texture2D;
        struct Texture;
    }

    namespace Enums {
        enum class CubeFace;
        enum class TextureType;
    }
}

namespace Reflex::Builders
{
    struct TextureBuilder
    {
        TextureBuilder(){};
         static std::unordered_map<Enums::TextureType, std::shared_ptr<Reflex::Core::Texture>> loadTexturesFromAIMaterial(aiMaterial* mat, const std::string& directory);

         static int createTextureIDFromFile(const char* path, const std::string& directory, bool gamma, glm::vec2&);
         static std::shared_ptr<Reflex::Core::Texture2D> createTexture2D(const std::string& name, const std::string& path, aiTextureType type);
         static std::shared_ptr<Reflex::Core::Texture2D> createTexture2D(const std::string& name, const std::string& path, Reflex::Enums::TextureType type);
         static Reflex::Core::TextureCubeMap createCubemap(const std::string& cubemapName,
                                                  const std::map<Enums::CubeFace, std::string>& mapFaceToPath);

         static std::shared_ptr<Core::Texture> loadSingleTextureFromMaterial(aiString str, aiTextureType type, const std::string& directory);
    };
}
