#pragma once


#include "Reflex-Core/Mesh.h"

#include <Reflex-Core/Cache.h>

struct aiScene;
struct aiNode;
struct aiMesh;


namespace Reflex {
    class AssetFactory;
    namespace Cache
    {
        struct ResourceCache;
    }

    namespace Core
    {
        struct Material;
    }
}


namespace Reflex::Builders {
        class MeshBuilder
        {
        public:
            MeshBuilder() = default;
            explicit MeshBuilder(std::shared_ptr<AssetFactory> assetFactory) : m_assetFactory(assetFactory){}
            MeshBuilder& createMesh(int id, const std::string&, std::shared_ptr<Core::Material> material, std::string parentMeshName);
            MeshBuilder& loadFromPath(const std::string&);
            Core::Mesh build();

        private:
            void processNode(aiNode* node, const aiScene* scene, const std::string& directory, std::string parentMeshName);
            Core::Mesh processMesh(std::vector<glm::vec3> pos, std::vector<unsigned> _indices, std::string parentMeshName) const;
            Core::Mesh processMesh(aiMesh* mesh, const aiScene* scene, std::string parentMeshName, const std::string& directory);

            std::string m_meshName;
            std::vector<Core::Mesh> m_childMeshes{};
            std::shared_ptr<Core::Material> m_material;

            std::string m_parentMeshName;

            std::shared_ptr<Core::Cache<Core::Material>> m_materialCache{};

            std::shared_ptr<AssetFactory> m_assetFactory;

        };
    }

