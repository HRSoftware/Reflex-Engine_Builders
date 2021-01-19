#include "pch.h"
#include "Includes/MaterialBuilder.h"

#include "Reflex-Core/Cache.h"
#include "Reflex-Core/Texture.h"
#include <Reflex-Logging.h>


Reflex::Builders::MaterialBuilder& Reflex::Builders::MaterialBuilder::reset()
{
    resID = -1;
    materialName.clear();
    textures.clear();
    shader = nullptr;
    return *this;
}

Reflex::Builders::MaterialBuilder& Reflex::Builders::MaterialBuilder::create(int id, const std::string& name)
{
    reset();
    resID = id;
    materialName = name;
    return *this;
}

Reflex::Builders::MaterialBuilder& Reflex::Builders::MaterialBuilder::setShader(std::shared_ptr<Core::Shader> _shader)
{
    if(!_shader)
    {
       // Log::writeError("MaterialBuilder - Cannot set empty shader");
    }
    else 
    {
        shader = _shader;
    }
    return *this;
}

Reflex::Builders::MaterialBuilder& Reflex::Builders::MaterialBuilder::addTextures(std::vector<std::shared_ptr<Core::Texture>>& _textures)
{
    for (auto texture : _textures)
        textures.insert_or_assign(texture->getTextureType(), texture);
    return *this;
}

Reflex::Builders::MaterialBuilder& Reflex::Builders::MaterialBuilder::addTexture(std::shared_ptr<Core::Texture> texture)
{
    textures.insert_or_assign(texture->getTextureType(), texture);
    return *this;
}

//void Reflex::Builders::MaterialBuilder::loadTexturesFromAIMaterial(aiMaterial* _material, const std::string& directory)
//{
//    
//    for ( auto texture : TextureLoader::loadTexturesFromAIMaterial(_material, directory))
//    {
//        textures.insert_or_assign(texture.first, std::make_shared<Core::Texture>(texture.second));
//    }
//}

Reflex::Core::Material Reflex::Builders::MaterialBuilder::build()
{
    Core::Material newMaterial;
    newMaterial.m_name = materialName;
    newMaterial.m_textures.swap(textures);
    newMaterial.m_shader = shader;

    return newMaterial;
}

//Reflex::Factory::MaterialFactory::MaterialFactory(std::shared_ptr<Cache::ResourceCache> cache): builder(cache) {}
