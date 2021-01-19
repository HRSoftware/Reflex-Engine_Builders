#include "pch.h"

#include "Includes/AssetFactory.h"
#include "Reflex-Logging.h"


#include "Includes/MaterialBuilder.h"
#include "Includes/TextureBuilder.h"


#include "Includes/ShaderBuilder.h"
#include "Includes/ModelBuilder.h"
#include "Reflex-Core/Texture.h"
#include "Reflex-Core/Texture2D.h"
#include "Reflex-Core/Shader.h"



Reflex::AssetFactory::AssetFactory()
{
    logger = std::make_shared<Reflex::Log::Logger>("Asset-Factory");
    m_shaderBuilder = std::make_shared<Builders::ShaderBuilder>();
    m_materialBuilder = std::make_shared<Builders::MaterialBuilder>();
    m_modelBuilder = std::make_shared<Builders::ModelBuilder>();
    logger->writeNotice("Hello from Asset-Factory!");
}

std::shared_ptr<Reflex::Core::Texture> Reflex::AssetFactory::createTexture(const std::string& textureName, const std::string& texturePath,
                                                                           const std::string textureType)
{
    return
        Reflex::Builders::TextureBuilder::createTexture2D(
            textureName,
            texturePath,
            Enums::Helpers::string_ToTextureType(textureType)
        );
}

std::shared_ptr<Reflex::Core::Texture> Reflex::AssetFactory::createTexture(const std::string& textureName, const std::string& texturePath,
    const aiTextureType _textureType)
{
    return Builders::TextureBuilder::createTexture2D(
        textureName,
        texturePath,
        Enums::Helpers::aiTextureType_ToTextureType(_textureType));
}

Reflex::Core::TextureCubeMap Reflex::AssetFactory::createCubeMap(const std::string& textureName,
                                                                 const std::map<Enums::CubeFace, std::string>
                                                                 collection)
{
    return Reflex::Builders::TextureBuilder::createCubemap(textureName, collection);
}

Reflex::Core::Material Reflex::AssetFactory::createMaterial(
    const std::string& materialName, const std::unordered_map<Enums::TextureType, std::shared_ptr<Core::Texture>>& typeNameMap,
    const std::shared_ptr<Core::Shader> _shader)
{
    std::shared_ptr<Reflex::Builders::MaterialBuilder> m_materialBuilder;
    m_materialBuilder->create(0, materialName);

    for (const auto textureType : typeNameMap) {
        const Enums::TextureType type = textureType.first;

        m_materialBuilder->addTexture(type, textureType.second);
    }

    m_materialBuilder->setShader(_shader);

    return m_materialBuilder->build();
}

Reflex::Core::Model Reflex::AssetFactory::createModel(const std::string& _refName,
    const std::string& _modelPath,
    const std::string& _materialName)
{
    std::shared_ptr<Reflex::Builders::ModelBuilder> m_modelBuilder;
    m_modelBuilder->create(0, _refName);
    // m_modelBuilder->loadFromPath(_modelPath); //Move to a model Loader

    return m_modelBuilder->build();
}



Reflex::Core::GameObject Reflex::AssetFactory::createGameObject(
    const std::string& refName, std::shared_ptr<Core::Model> model, glm::vec3 _positionVec, glm::vec3 _scaleVec,
    glm::quat _orientationVec)
{
    Reflex::Core::GameObject newGameObject;
    newGameObject.setModel(model);
    newGameObject.setName(refName);
    newGameObject.setScale(_scaleVec);
    newGameObject.setPosition(_positionVec);
    newGameObject.setOrientation(_orientationVec);

    return newGameObject;
}

std::optional<Reflex::Core::Shader> Reflex::AssetFactory::createShader(
    const std::string& refName, const std::string& pathToFile, const int typeMask)
{

    Reflex::Builders::ShaderBuilder m_shaderBuilder;

    m_shaderBuilder.createNewShader(0, refName, pathToFile);

    if ((typeMask & static_cast<int>(Reflex::Enums::ShaderType::VERTEX)) == static_cast<int>(Reflex::Enums::ShaderType::VERTEX))
        m_shaderBuilder.createNewShader(Reflex::Enums::ShaderType::VERTEX, pathToFile);
    if ((typeMask & static_cast<int>(Reflex::Enums::ShaderType::FRAGMENT)) == static_cast<int>(Reflex::Enums::ShaderType::FRAGMENT))
        m_shaderBuilder.createNewShader(Reflex::Enums::ShaderType::FRAGMENT, pathToFile);
    if ((typeMask & static_cast<int>(Reflex::Enums::ShaderType::COMPUTE)) == static_cast<int>(Reflex::Enums::ShaderType::COMPUTE))
        m_shaderBuilder.createNewShader(Reflex::Enums::ShaderType::COMPUTE, pathToFile);
    if ((typeMask & static_cast<int>(Reflex::Enums::ShaderType::GEOMETRY)) == static_cast<int>(Reflex::Enums::ShaderType::GEOMETRY))
        m_shaderBuilder.createNewShader(Reflex::Enums::ShaderType::GEOMETRY, pathToFile);
    if ((typeMask & static_cast<int>(Reflex::Enums::ShaderType::TESS_CONTROL)) == static_cast<int>(Reflex::Enums::ShaderType::
        TESS_CONTROL))
        m_shaderBuilder.createNewShader(Reflex::Enums::ShaderType::TESS_CONTROL, pathToFile);
    if ((typeMask & static_cast<int>(Reflex::Enums::ShaderType::TESS_EVAL)) == static_cast<int>(Reflex::Enums::ShaderType::TESS_EVAL))
        m_shaderBuilder.createNewShader(Reflex::Enums::ShaderType::TESS_EVAL, pathToFile);


    std::optional<Core::Shader> finishedTexture = m_shaderBuilder.build();

    if (finishedTexture.has_value()) {
        return finishedTexture;
    }

    return std::nullopt;
}

