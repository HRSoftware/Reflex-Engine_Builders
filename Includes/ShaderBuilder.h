#pragma once
#include <string>
#include <map>
#include <optional>
#include <vector>
#include "Reflex-Core/Shader.h"


namespace Reflex::Builders
    {
        class ShaderBuilder
        {
        public:
        explicit ShaderBuilder(){}
            Reflex::Builders::ShaderBuilder& addShaderTypeByMask(int shaderBitMask, const std::string& pathToFile);

            Reflex::Builders::ShaderBuilder& reset();

            ShaderBuilder& createNewShader(int id, const std::string& refName, const std::string& fileName = "",
                bool core = false);
            ShaderBuilder& addShaderType(Enums::ShaderType shaderType, const std::string& fileName = "");
            std::optional<Reflex::Core::Shader> build();
            std::map<Reflex::Enums::ShaderLocationIndex, int> findUniformLocations() const;

        private:
            std::string shaderProgramName;
            std::string shaderFileName;
            int shaderTypeMask{};
            std::vector<int> shadersIDs;

            std::map<Reflex::Enums::ShaderLocationIndex, int> _customUniformLocations;
            int programID = -1;
            int resID = -1;

            bool m_hasError = false;
        };

        class ShaderFactory
        {
        public:
            ShaderFactory(){}

            std::optional<Reflex::Core::Shader> createBasicShader(int id, const std::string& refName,
                const std::string& filename = "");

        private:
             ShaderBuilder builder;
        };
    }
