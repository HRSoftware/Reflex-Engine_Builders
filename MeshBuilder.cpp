#include "pch.h"
#pragma once
#include "Includes/MeshBuilder.h"
#include "Includes/AssetFactory.h"

#include "Reflex-Core/Vertex.h"

#include "Reflex-Logging.h"

#include "Reflex-Core/ResourceCache.h"
#include "Includes/TextureBuilder.h"
#include "Includes/ModelBuilder.h"


Reflex::Builders::MeshBuilder& Reflex::Builders::MeshBuilder::createMesh(int id, const std::string& name, std::shared_ptr<Core::Material> material, std::string parentMeshName = "")
{
    m_meshName = name;
    m_childMeshes.clear();
    m_parentMeshName = std::move(parentMeshName);
    m_material = material;

    return *this;
}

void Reflex::Builders::MeshBuilder::processNode(aiNode* node, const aiScene* scene, const std::string& directory, std::string parentMeshName = "")
{
    for ( unsigned int i = 0; i < node->mNumMeshes; i++ )
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        Core::Mesh newMesh = processMesh(mesh, scene, parentMeshName, directory);
        newMesh._material = m_material ? m_material : m_materialCache->getDefault();
        m_childMeshes.push_back(newMesh);
    }

    for ( unsigned int i = 0; i < node->mNumChildren; i++ )
    {
        processNode(node->mChildren[i], scene, directory, parentMeshName);
    }
}

Reflex::Core::Mesh Reflex::Builders::MeshBuilder::processMesh(std::vector<glm::vec3> pos,
                                                              std::vector<unsigned> _indices, std::string parentMeshName) const
{
    std::vector<Core::Vertex> vertices;
    std::vector<unsigned int> indices;

    for ( unsigned int i = 0; i < pos.size(); i++ )
    {
        Core::Vertex vertex;
        vertex.Position.x = pos[i].x;
        vertex.Position.y = pos[i].y;
        vertex.Position.z = pos[i].z;

        vertices.push_back(vertex);
    }
    indices = _indices;
    std::string name = m_meshName + "_Mesh_" + std::to_string(m_childMeshes.size());
    return Core::Mesh(name, vertices, indices, nullptr);
}

Reflex::Core::Mesh Reflex::Builders::MeshBuilder::processMesh(aiMesh* mesh, const aiScene* scene, 
                                                                std::string parentMeshName, 
                                                                const std::string& directory)
{
    std::vector<Core::Vertex> vertices;
    std::vector<unsigned int> indices;
    std::shared_ptr<Core::Material> _material;

    for ( unsigned int i = 0; i < mesh->mNumVertices; i++ )
    {
        Reflex::Core::Vertex vertex;
        // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's

        vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};


        // normals
        if ( mesh->mNormals != NULL )
        {
            vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
        }

        // texture coordinates
        if ( mesh->mTextureCoords[0] ) // does the mesh contain texture coordinates?
        {
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use Meshs where a vertex can have multiple texture coordinates so we always take the first set (0).
            vertex.TexCoords = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
        }

        if ( mesh->mTangents != NULL )
        {
            vertex.Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
        }

        if ( mesh->mBitangents != NULL )
        {
            vertex.Bitangent = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
        }

        vertices.push_back(vertex);
    }

    for ( unsigned int i = 0; i < mesh->mNumFaces; i++ )
    {
        aiFace face = mesh->mFaces[i];
        for ( unsigned int j = 0; j < face.mNumIndices; j++ )
            indices.push_back(face.mIndices[j]);
    }
    // process materials
    auto material = scene->mMaterials[mesh->mMaterialIndex];

    aiString MaterialName;
    material->Get(AI_MATKEY_NAME, MaterialName);

    if ( !_material )
    {
        std::string matName = MaterialName.C_Str();

        auto mat = std::make_shared<Core::Material>(m_assetFactory->createMaterial(matName,
            TextureBuilder::loadTexturesFromAIMaterial(material, directory), m_material->m_shader));

        if(mat)
        {
            _material = mat;
        }
        else
        {
            //Log::writeError("MeshBuilder - Error creating material {0}", matName);
            _material = m_materialCache->getDefault();
        }

    }

    std::string name = parentMeshName + "_Mesh_";// +std::to_string(m_meshes.size());
    return Core::Mesh(name, vertices, indices, _material);
}


Reflex::Core::Mesh Reflex::Builders::MeshBuilder::build()
{
    Core::Mesh newMesh;
    newMesh._meshName = m_meshName;
    for ( Core::Mesh mesh : m_childMeshes )
        newMesh._childMeshes.insert_or_assign(m_meshName, std::make_shared<Core::Mesh>(mesh));

    return newMesh;
}
