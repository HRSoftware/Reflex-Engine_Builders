#pragma once
#include <mutex>
#include <optional>
#include <unordered_map>
#include <Reflex-Core/GameObject.h>
#include <Reflex-Core/Material.h>
#include <Reflex-Core/Model.h>
#include <Reflex-Core/Shader.h>



#include "glm/ext.hpp"
#include <assimp\material.h>


namespace Reflex
{
    namespace Log {
        class Logger;
    }

    namespace Enums {
        enum class TextureType;
        enum class CubeFace;
    }

    namespace Core {
        struct TextureCubeMap;
        struct Texture;
    }

    namespace Builders {
        class MaterialBuilder;
        class ShaderBuilder;
        class ModelBuilder;
    }

    namespace Cache {
        struct ResourceCache;
    }

    class AssetFactory
    {
        public:
        std::shared_ptr<Reflex::Log::Logger> logger;

    public:
    explicit AssetFactory();

        static std::shared_ptr<Core::Texture> createTexture(const std::string& textureName, const std::string& texturePath,
                                                        const std::string textureType);
        static std::shared_ptr<Core::Texture> createTexture(const std::string& textureName, const std::string& texturePath,
            aiTextureType _textureType);


        static Reflex::Core::TextureCubeMap createCubeMap(const std::string& textureName,
                                                          const std::map<Enums::CubeFace, std::string> collection);

        static Core::Material createMaterial(const std::string& materialName,
            const std::unordered_map<Enums::TextureType, std::shared_ptr<Core::Texture>>& typeNameMap,
            const std::shared_ptr<Core::Shader> _shader);

        static Core::Model createModel(const std::string& refName, const std::string& path,
            const std::string& _materialName = "DefaultMaterial");

        static Core::GameObject createGameObject(const std::string& refName, std::shared_ptr<Core::Model> model,
            glm::vec3
            _positionVec = { 0.f, 0.f, 0.f },
            glm::vec3 _scaleVec = { 1.f, 1.f, 1.f },
            glm::quat _orientationVec = { 0.f, 0.f, 0.f, 0.f });

        static std::optional<Core::Shader> createShader(const std::string& refName, const std::string& pathToFile,
            const int typeMask);

        std::shared_ptr<Core::Shader> getDefaultShader() { return nullptr; };
        AssetFactory(AssetFactory& other) = delete;
        void operator=(const AssetFactory&) = delete;

        //static Reflex::AssetFactory* getInstance(std::shared_ptr<Reflex::Cache::ResourceCache>);

    protected:


        //static AssetFactory* m_pInstance;
        std::mutex m_mutex;
        std::shared_ptr<Builders::ModelBuilder> m_modelBuilder;
        std::shared_ptr<Builders::ShaderBuilder> m_shaderBuilder;
        std::shared_ptr<Builders::MaterialBuilder> m_materialBuilder;
    };
}


//inline Reflex::AssetFactory* Reflex::AssetFactory::getInstance(
//    std::shared_ptr<Reflex::Cache::ResourceCache> _resourceCache)
//{
//    std::lock_guard<std::mutex> lock(m_mutex);
//
//    if ( !m_pInstance )
//        m_pInstance = new AssetFactory(_resourceCache);
//
//    return m_pInstance;
//}
