#pragma once
#include <Reflex-Core/Model.h>


namespace Reflex {
    namespace Core {
        struct Mesh;
    }
}

namespace Reflex::Cache
{
    struct ResourceCache;
}

namespace Reflex::Builders {
        class ModelBuilder
        {
        public:
            ModelBuilder() = default;
            ModelBuilder& create(int id, const std::string& newModelName);
            ModelBuilder& addMesh(std::vector<std::shared_ptr<Core::Mesh>> meshes);
            //ModelBuilder& loadFromPath(const std::string&);
            Core::Model build();

        private:
            std::string m_modelName;
            std::vector<std::shared_ptr<Core::Mesh>> m_meshes{};
            std::string m_directory;
            std::string m_fullFileName;

            void resetBuilder();

            /*void processNode(aiNode* node, const aiScene* scene);
            Core::Mesh processMesh(std::vector<glm::vec3> pos, std::vector<unsigned> _indices) const;
            Core::Mesh processMesh(aiMesh* mesh, const aiScene* scene);*/

            /*std::shared_ptr<Reflex::Core::Mesh> handleMesh(const rapidjson::GenericMember<rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<>>& mesh);
            void handleTextures(rapidjson::GenericArray<false, rapidjson::GenericValue<rapidjson::UTF8<>>> textureArray);
            std::shared_ptr<Material> handleMaterial(const rapidjson::GenericValue<rapidjson::UTF8<>>::Object& materialObject, std::string materialName);*/
        };
 }

//namespace Reflex {
//    namespace Factory {
//        namespace ModelFactory {
//
//            inline std::shared_ptr<Core::Model> createMesh(const std::string& name, const std::string& path)
//            {
//                Builders::ModelBuilder modelBuilder(Reflex::resourceManager._resourceCache);
//                std::shared_ptr<Core::Model> model = Reflex::resourceManager._resourceCache->m_ModelCache.findResource(name);
//                if ( !model )
//                {
//                    Core::Model newModel = modelBuilder.createMesh(GUID_Allocator::getNewUniqueGUID(), name)
//                                                 .loadFromPath(path)
//                                                 .build();
//
//                    model = Reflex::resourceManager._resourceCache->m_ModelCache.addResource(name, std::make_shared<Core::Model>(newModel));
//                    
//                }
//                return model;
//            }
//
//        }
//    }
//};
