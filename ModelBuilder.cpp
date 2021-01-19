#include "pch.h"
#pragma once


#include "Includes/ModelBuilder.h"
#include "Reflex-Core/Vertex.h"

//
//void handleMaterial(const rapidjson::GenericValue<rapidjson::UTF8<>>::Object& getObject, const rapidjson::UTF8<>::Ch* getString);
//
//
//std::shared_ptr<Reflex::Core::Mesh> Reflex::Builders::ModelBuilder::handleMesh(
//    const rapidjson::GenericMember<rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<>>& mesh)
//{
//    std::vector <Reflex::Core::  Vertex > vertexVec;
//    std::vector<unsigned> indicies;
//
//    for ( auto& vertex  : mesh.value.FindMember("Verticies")->value.GetArray() )
//    {
//        Reflex::Core::Vertex newVertex;
//        int i = 0;
//        for (auto& axis : vertex.FindMember("Position")->value.GetArray())
//        {
//            newVertex.Position[i++] = axis.GetFloat();
//        }
//
//        i = 0;
//        for (auto& axis : vertex.FindMember("Normals")->value.GetArray())
//        {
//            newVertex.Normal[i++] = axis.GetFloat();
//        }
//
//        i = 0;
//        for (auto& axis : vertex.FindMember("TexCoords")->value.GetArray())
//        {
//            newVertex.TexCoords[i++] = axis.GetFloat();
//        }
//
//        i = 0;
//        for (auto& axis : vertex.FindMember("Tangents")->value.GetArray())
//        {
//            newVertex.Tangent[i++] = axis.GetFloat();
//        }
//
//        i = 0;
//        for (auto& axis : vertex.FindMember("Bitangents")->value.GetArray())
//        {
//            newVertex.Bitangent[i++] = axis.GetFloat();
//        }
//        vertexVec.push_back(newVertex);
//    }
//
//    for ( auto& index : mesh.value.FindMember("Indices")->value.GetArray())
//        indicies.push_back(index.GetInt());
//
//    auto materialName = mesh.value.FindMember("Material")->value.GetString();
//
//
//
//
//    return std::make_shared<Mesh>( mesh.name.GetString(), 
//        vertexVec, 
//        indicies,  
//        materialCache.getResource(materialName));
//
//
//}
//
//void Reflex::Builders::ModelBuilder::handleTextures(rapidjson::GenericArray<false, rapidjson::GenericValue<rapidjson::UTF8<>>> textureArray)
//{
//    texture Resource manager 
//    for ( auto& textureObject : textureArray )
//    {
//        auto textureName = textureObject.GetString();
//        auto texture = textureCache.getResource(textureName);
//
//        if (texture == nullptr)
//        {
//            Reflex::Core::Texture newTexture = TextureLoader::createTexture(textureName, Reflex::Enums::TextureType::UNKNOWN);
//            textureCache.addResource(textureName, newTexture);
//        }
//    }
//}
//
//std::shared_ptr<Material> Reflex::Builders::ModelBuilder::handleMaterial(const rapidjson::GenericValue<rapidjson::UTF8<>>::Object& materialObject, std::string materialName)
//{
//
//    Core::Material newMaterial;
//
//    const auto materialShader = materialObject.FindMember("Shader")->value.GetString();
//
//    newMaterial.m_shader = m_resourceCache->getShader(materialShader, true);;
//
//
//    for (auto& textureObject : materialObject.FindMember("Textures")->value.GetObject()) //for each textureObject
//    {
//        auto textureName = textureObject.value.FindMember("Name")->value.GetString();
//        std::shared_ptr<Reflex::Core::Texture> _texture = m_resourceCache->getTexture(textureName);
//
//        if (_texture == nullptr) {
//
//            _texture = m_.createNewResource(textureName);
//        }
//        int t = static_cast<int>(_texture->_textureType);
//        newMaterial.textures.at(t) = _texture;
//    };
//
//
//    return m_resourceCache->m_MaterialCache.addResource(materialName, newMaterial);
//}

Reflex::Builders::ModelBuilder& Reflex::Builders::ModelBuilder::create(int id, const std::string& newModelName)
{
    resetBuilder();
    m_modelName = newModelName;
    return *this;
}

Reflex::Builders::ModelBuilder& Reflex::Builders::ModelBuilder::addMesh(std::vector<std::shared_ptr<Core::Mesh>> meshes)
{
    m_meshes = meshes;
    return *this;
}

Reflex::Core::Model Reflex::Builders::ModelBuilder::build()
{
    Core::Model newModel;
    newModel.modelName = m_modelName;
    newModel.meshes = m_meshes;

    resetBuilder();
    return newModel;
}

void Reflex::Builders::ModelBuilder::resetBuilder()
{
    m_modelName.clear();
    m_meshes.clear();
    m_directory.clear();
    m_fullFileName.clear();
}
