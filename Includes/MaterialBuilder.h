#pragma once
#include <Reflex-Core/Material.h>

//fwd decl
namespace Reflex {
    namespace Core {
        struct Shader;
        struct Texture;
    }
    namespace Enums {
        enum class TextureType;
    }
}



namespace Reflex::Builders
{
        class MaterialBuilder
        {

        public:
            MaterialBuilder(){}

            ~MaterialBuilder() = default;

           MaterialBuilder& reset();
           MaterialBuilder& create(int id, const std::string& name = "default");


           MaterialBuilder& addTextures(std::vector<std::shared_ptr<Core::Texture>>& _textures);
           MaterialBuilder& addTexture(std::shared_ptr<Reflex::Core::Texture> texture);
            //void void AIMaterial(aiMaterial* mat, const std::string& directory);
           MaterialBuilder& setShader(std::shared_ptr<Core::Shader> _shader);
           MaterialBuilder& addTextures(std::unordered_map<Enums::TextureType, std::shared_ptr<Core::Texture>>& _textures)
           {
               for (auto texture : _textures)
                   textures.insert_or_assign(texture.first, texture.second);
           }
                MaterialBuilder& addTexture(Reflex::Enums::TextureType _textureType, std::shared_ptr<Core::Texture>_texture)
            {
                textures.insert_or_assign(_textureType, _texture);
            }
             Core::Material build();

        private:
             int resID = -1;
             std::string materialName{};
             std::unordered_map<Enums::TextureType, std::shared_ptr<Core::Texture>> textures{};
             std::shared_ptr<Core::Shader> shader;
        };

    }


//namespace Reflex {
//    namespace Factory {
//
//        class MaterialFactory
//        {
//            public:
//            explicit MaterialFactory(std::shared_ptr<Cache::ResourceCache> cache);
//
//
//            private:
//            Builders::MaterialBuilder builder;
//        };
//
//    }
//}
