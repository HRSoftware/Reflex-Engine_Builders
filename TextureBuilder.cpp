#include "pch.h"

#include "Includes/TextureBuilder.h"
#include "glad/glad.h"
//std

#include <Reflex-Core/Shader.h>
#include "Reflex-Logging.h"
#include "Reflex-Core/Texture.h"
#include "Reflex-Core/Texture2D.h"
#include "Reflex-Core/TextureCubeMap.h"

#include "stb/stb.h"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stb/stb_image.h"





std::shared_ptr<Reflex::Core::Texture2D> Reflex::Builders::TextureBuilder::createTexture2D(const std::string& name, const std::string& path,
                                                                                           aiTextureType type = aiTextureType_UNKNOWN)
{
    //globalAssetFactory.createTexture(name, path, )
    return Reflex::Builders::TextureBuilder::createTexture2D(name, path, Reflex::Enums::Helpers::aiTextureType_ToTextureType(type));
}

std::shared_ptr<Reflex::Core::Texture2D> Reflex::Builders::TextureBuilder::createTexture2D(const std::string& name, const std::string& path,
                                                                                           Reflex::Enums::TextureType type = Reflex::Enums::TextureType::MAP_UNKNOWN)
{
    std::shared_ptr <Reflex::Core::Texture2D > newTexture;
    glm::vec2 textureDimensions;
    newTexture->setName(name);
    newTexture->setPath(path);
    newTexture->setTextureID(
        Reflex::Builders::TextureBuilder::createTextureIDFromFile(path.c_str(), "", true, textureDimensions));
    newTexture->setDimensions(textureDimensions);
    newTexture->setTextureType(type);
    return newTexture;
}

std::unordered_map<Reflex::Enums::TextureType, std::shared_ptr<Reflex::Core::Texture>> Reflex::Builders::TextureBuilder::loadTexturesFromAIMaterial(
    aiMaterial* _material, const std::string& directory)
{
    using Reflex::Enums::ShaderLocationIndex;
    std::filesystem::path filepath = directory;
    std::string path = filepath.parent_path().string();
    std::string file = filepath.filename().string();

    aiTextureType _type = aiTextureType_UNKNOWN;
    Reflex::Enums::TextureType _locationIndex = Reflex::Enums::TextureType::MAP_UNKNOWN;
    std::vector<std::string> types = {
        "texture_diffuse",
        "texture_normal",
        "texture_specular",
        "texture_height"
    };

    std::unordered_map<Enums::TextureType, std::shared_ptr<Core::Texture>> _textures;

    for ( std::string _typeString : types )
    {
        //for each texture in the m_shader map it to the relevant aiTexture and textureType(ShaderLocationIndex) 
        if ( _typeString == "texture_diffuse" )
        {
            _type = aiTextureType_DIFFUSE;
            _locationIndex = Enums::TextureType::MAP_DIFFUSE;
        }

        if ( _typeString == "texture_normal" )
        {
            _type = aiTextureType_NORMALS;
            _locationIndex = Enums::TextureType::MAP_NORMAL;
        }

        if ( _typeString == "texture_specular" )
        {
            _type = aiTextureType_SPECULAR;
            _locationIndex = Enums::TextureType::MAP_SPECULAR;
        }

        if ( _typeString == "texture_height" )
        {
            _type = aiTextureType_HEIGHT;
            _locationIndex = Enums::TextureType::MAP_HEIGHT;
        }

        for ( unsigned int i = 0; i < _material->GetTextureCount(_type); i++ )
        {
            aiString str;

            _material->GetTexture(_type, i, &str);

            path = directory + "/" + str.C_Str();
            path = std::regex_replace(path, std::regex("//"), "/");
            auto type = Enums::Helpers::aiTextureType_ToTextureType(_type);

            _textures.insert_or_assign(_locationIndex,
                                       createTexture2D(str.C_Str(), path,
                                                                     type));

        }
    }
    return _textures;
}

int Reflex::Builders::TextureBuilder::createTextureIDFromFile(const char* path, const std::string& directory,
    bool gamma, glm::vec2&)
{
    std::filesystem::path filePath = path;

    std::string fullbackFilePath = std::string(
        filePath.parent_path().string() + "/textures/" + filePath.filename().string());


    std::string fullFilePath = directory == "" ? filePath.string() : directory + "/" + filePath.string();
    fullFilePath = std::regex_replace(fullFilePath, std::regex("//"), "/");
    fullbackFilePath = std::regex_replace(fullbackFilePath, std::regex("//"), "/");

    if ( !std::filesystem::exists(fullFilePath) )
    {
        fullFilePath = fullbackFilePath;
        if ( !std::filesystem::exists(fullFilePath) )
        {
            //Reflex::Log::writeWarning("Textures not found for {0} at location {1}", filePath.filename().string(),
             //   filePath.parent_path().string());
            return -1;
        }

        //Reflex::Log::writeDebug("texture file found!");
    }

    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(fullFilePath.c_str(), &width, &height, &nrComponents, 0);
    if ( data )
    {
        GLenum format = GL_RGBA;
        switch ( nrComponents )
        {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        default:
            break;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);
        //glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data); Old way
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        //Reflex::Log::writeError("Textures failed to load at path: {0}", filePath.string());
    }

    stbi_image_free(data);

    //dimensions = glm::vec2(height, width);
    return textureID;
}

std::shared_ptr<Reflex::Core::Texture> Reflex::Builders::TextureBuilder::loadSingleTextureFromMaterial(aiString str, aiTextureType type,
                                                                                                         const std::string& directory)
{
    std::string path = directory + "/" + str.C_Str();
    return Reflex::Builders::TextureBuilder::createTexture2D(str.C_Str(), std::regex_replace(path, std::regex("//"), "/"),
        type);
}

Reflex::Core::TextureCubeMap Reflex::Builders::TextureBuilder::createCubemap(
    const std::string& cubemapName, const std::map<Reflex::Enums::CubeFace, std::string>& mapFaceToPath)
{
    GLuint textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for ( auto face : mapFaceToPath )
    {
        unsigned char* data = stbi_load(face.second.c_str(), &width, &height, &nrChannels, 0);
        if ( data )
        {
            GLenum format = 0;
            if ( nrChannels == 1 )
                format = GL_RED;
            else if ( nrChannels == 3 )
                format = GL_RGB;
            else if ( nrChannels == 4 )
                format = GL_RGBA;


            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + (int)face.first, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE,
                data);

            //Reflex::Log::writeNotice("Cubemap texture: {0} loaded", face.second);
            stbi_image_free(data);
        }
        else
        {
           // Reflex::Log::writeWarning("Skybox texture failed to load at path: {0}", face.second);
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    Core::TextureCubeMap _newTexture;
    _newTexture.setName(cubemapName);
    _newTexture.setFaceTextureMap(mapFaceToPath);
    _newTexture.setTextureID(textureID);
    _newTexture.setDimensions({width, height});
    _newTexture.setTextureType(Enums::TextureType::MAP_CUBEMAP_COLLECTION);
    return _newTexture;
}
