#include "pch.h"
#include "Includes/ShaderBuilder.h"
#include "Reflex-Logging.h"
#include "Reflex-Core/Texture.h"


inline std::optional<std::string> folderSearchRecursive(const std::string& path, const std::filesystem::path fileLookingFor)
{
    for(const auto& entry : std::filesystem::directory_iterator(path))
    {
        if(entry.is_directory())
        {
            auto t = folderSearchRecursive(entry.path().string(), fileLookingFor);
            if(t.has_value())
            {
                return t;
            }
        }
        auto t = entry.path().filename();
        auto u = fileLookingFor.filename().string();
        if(t == u)
        {
            return entry.path().string();
        }
    }

    return std::nullopt;
}

//used to clear all var from previous builds
Reflex::Builders::ShaderBuilder& Reflex::Builders::ShaderBuilder::reset()
{
    resID = 0;
    shaderFileName.clear();      
    shaderProgramName.clear();
    programID = 0;
    programID = 0;
    _customUniformLocations.clear();
    shadersIDs.clear();
    shaderTypeMask = 0;
    shadersIDs.clear();
    m_hasError = false;

    return *this;
}

Reflex::Builders::ShaderBuilder& Reflex::Builders::ShaderBuilder::createNewShader(int id, const std::string& refName, const std::string& fileName, bool core)
{
    reset();
    resID = id;
    shaderFileName = fileName;      
    shaderProgramName = refName;
    return *this;
}

Reflex::Builders::ShaderBuilder& Reflex::Builders::ShaderBuilder::addShaderType(Reflex::Enums::ShaderType shaderType, const std::string& fileName)
{

    using Reflex::Enums::ShaderType;
    std::string shaderRAWCode;
    std::ifstream shaderFile;
    //fileName = shaderFileName.empty() ? shaderProgramName : shaderFileName;

    if (m_hasError)//bail out early if we have previous issue;
    {
        //Reflex::Log::writeError("-----  Error::Shader: HALTING ADD_SHADER DUE TO EARLIER ISSUE - File: {0}", fileName);
        m_hasError = false;
        return *this;
    }

    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        std::string fileEXT;
        switch ( shaderType )
        {
        case Reflex::Enums::ShaderType::VERTEX:
            shaderTypeMask |= static_cast<int>(Reflex::Enums::ShaderType::VERTEX);
            fileEXT = ".vert";
            break;
        case Reflex::Enums::ShaderType::FRAGMENT:
            shaderTypeMask |= static_cast<int>(Reflex::Enums::ShaderType::FRAGMENT);
            fileEXT = ".frag";
            break;
        case Reflex::Enums::ShaderType::GEOMETRY:
            shaderTypeMask |= static_cast<int>(Reflex::Enums::ShaderType::GEOMETRY);
            fileEXT = ".geo";
            break;
        case Reflex::Enums::ShaderType::COMPUTE:
            shaderTypeMask |= static_cast<int>(Reflex::Enums::ShaderType::COMPUTE);
            fileEXT = ".comp";
            break;
        case Reflex::Enums::ShaderType::TESS_CONTROL:
            shaderTypeMask |= static_cast<int>(Reflex::Enums::ShaderType::TESS_CONTROL);
            fileEXT = ".tessc";
            break;
        case Reflex::Enums::ShaderType::TESS_EVAL:
            shaderTypeMask |= static_cast<int>(Reflex::Enums::ShaderType::TESS_EVAL);
            fileEXT = ".tesse";
            break;
        default:
            break;
        }

        std::filesystem::path filePath( fileName + fileEXT);
        std::filesystem::path fullbackfilePath( "core/" + fileName + fileEXT);

        if (exists(filePath))
            shaderFile.open(filePath);
        else
        {
            std::filesystem::path path = fileName + fileEXT;

            //Log::writeNotice("Searching recursively for {0}", path.filename().string());
            auto fileToOpen =  folderSearchRecursive("Resources/Shaders/", path);

            if(fileToOpen.has_value())
            {
                shaderFile.open(*fileToOpen);
            }
            else
            {
                //Log::writeError("-----  Error::Shader: {0} - ERROR OPENING/READING THE FILE", fileName);
                m_hasError = true;
                return *this;
            }
        }
        if(shaderFile.is_open())
        {
            std::stringstream shaderStream;
            shaderStream << shaderFile.rdbuf();
            shaderRAWCode = shaderStream.str();
        }
        shaderFile.close();
    }
    catch ( std::ifstream::failure e)
    {
        //Log::writeError("-------  Error::Shader - {0} - FILE_NOT_SUCCESFULLY_READ. Error: {1}", fileName, e.what());
        m_hasError = true;
        return *this;
    }

    const char* shaderCode = shaderRAWCode.c_str();
    unsigned int shaderID = glCreateShader(shaderType);
    int success;
    char infoLog[512];

    glShaderSource(shaderID, 1, &shaderCode, NULL);
    glCompileShader(shaderID);
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        //Reflex::Log::writeError("Error - Shader - COMPILATION_FAILED - {0}. Error: {1}", infoLog, shaderType);
        m_hasError = true;
        return *this;
    }

    shadersIDs.push_back(shaderID);
    //Log::writeNotice("Success - {0} Shader compiled - {1}", Enums::Helpers::shaderType_ToString(shaderType), fileName);

    return *this;
}

Reflex::Builders::ShaderBuilder& Reflex::Builders::ShaderBuilder::addShaderTypeByMask(int shaderBitMask, const std::string& pathToFile)
{

    if ( (shaderBitMask & static_cast<int>(Reflex::Enums::ShaderType::VERTEX)) == static_cast<int>(Enums::ShaderType::VERTEX) )
        addShaderType(Reflex::Enums::ShaderType::VERTEX, pathToFile);
    if ( (shaderBitMask & static_cast<int>(Enums::ShaderType::FRAGMENT)) == static_cast<int>(Enums::ShaderType::FRAGMENT) )
        addShaderType(Reflex::Enums::ShaderType::FRAGMENT, pathToFile);
    if ( (shaderBitMask & static_cast<int>(Enums::ShaderType::COMPUTE)) == static_cast<int>(Enums::ShaderType::COMPUTE) )
        addShaderType(Reflex::Enums::ShaderType::COMPUTE, pathToFile);
    if ( (shaderBitMask & static_cast<int>(Enums::ShaderType::GEOMETRY)) == static_cast<int>(Enums::ShaderType::GEOMETRY) )
        addShaderType(Reflex::Enums::ShaderType::GEOMETRY, pathToFile);
    if ( (shaderBitMask & static_cast<int>(Enums::ShaderType::TESS_CONTROL)) == static_cast<int>(Enums::ShaderType::TESS_CONTROL) )
        addShaderType(Reflex::Enums::ShaderType::TESS_CONTROL, pathToFile);
    if ( (shaderBitMask & static_cast<int>(Enums::ShaderType::TESS_EVAL)) == static_cast<int>(Enums::ShaderType::TESS_EVAL) )
        addShaderType(Reflex::Enums::ShaderType::TESS_EVAL, pathToFile);

    return *this;
}


std::optional<Reflex::Core::Shader> Reflex::Builders::ShaderBuilder::build()
{
    int success;
    char infoLog[512];

    if(m_hasError)
    {
        return std::nullopt;
    }

    programID = glCreateProgram();
    for ( auto shaderID : shadersIDs )
    {
        glAttachShader(programID, shaderID);
    }
    glLinkProgram(programID);

    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programID, 512, NULL, infoLog);
        //Reflex::Log::writeError("Error::Shader::Program::LINKING_FAILED - {0}. Error: {1}", shaderProgramName, infoLog);
        m_hasError = true;
        return std::nullopt;
    }

    _customUniformLocations = ShaderBuilder::findUniformLocations();
    for ( auto shaderID : shadersIDs )
    {
        glDeleteShader(shaderID);
    }

    Reflex::Core::Shader newShader;
    newShader.programID = programID;
    newShader._shaderName = shaderProgramName;
    newShader.setShaderTypeMask(shaderTypeMask);
    newShader._uniforms = _customUniformLocations;
    return std::optional<Reflex::Core::Shader>(newShader);
}

std::map<Reflex::Enums::ShaderLocationIndex, int> Reflex::Builders::ShaderBuilder::findUniformLocations() const
{
    std::map<Reflex::Enums::ShaderLocationIndex, int> uniforms;
    GLint numUniforms = 0;
#ifdef __APPLE__
    // For OpenGL 4.1, use glGetActiveUniform
    GLint maxLen;
    GLchar *name;

    glGetProgramiv(handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLen);
    glGetProgramiv(handle, GL_ACTIVE_UNIFORMS, &numUniforms);

    name = new GLchar[maxLen];
    for (GLuint i = 0; i < numUniforms; ++i) {
        GLint size;
        GLenum type;
        GLsizei written;
        glGetActiveUniform(handle, i, maxLen, &written, &size, &type, name);
        GLint location = glGetUniformLocation(handle, name);
        uniformLocations[name] = glGetUniformLocation(handle, name);


    }
    delete[] name;
#else
    // For OpenGL 4.3 and above, use glGetProgramResource
    glGetProgramInterfaceiv( programID, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);

    GLenum properties[] = {GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX};

    for( GLint i = 0; i < numUniforms; ++i ) {
        GLint results[4];

        //get the resource
        glGetProgramResourceiv(programID, GL_UNIFORM, i, 4, properties, 4, nullptr, results);

        if( results[3] != -1 ) continue;  // Skip uniforms in blocks
        const GLint nameBufSize = results[0] + 1;
        char* name = new char[nameBufSize];

        //get the resource's name
        glGetProgramResourceName(programID, GL_UNIFORM, i, nameBufSize, nullptr, name);

        auto shaderLocationIndexName = Reflex::Enums::Helpers::string_toShaderLocationIndexEnum(name);
        uniforms[shaderLocationIndexName] = results[2];
        delete [] name;
    }
#endif

    return uniforms;
}

std::optional<Reflex::Core::Shader> Reflex::Builders::ShaderFactory::createBasicShader(int id, const std::string& refName,
    const std::string& filename)
{
    return builder.reset()
        .createNewShader(id, refName, filename)
        .addShaderType(Reflex::Enums::ShaderType::VERTEX, filename)
        .addShaderType(Reflex::Enums::ShaderType::FRAGMENT, filename)
        .build();
}


