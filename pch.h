#pragma once

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <regex>
#include <map>
#include <mutex>
#include <optional>
#include <fstream>
#include <optional>

//RapidJSON
#include <rapidjson/error/en.h>
#include <rapidjson/error/error.h>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/rapidjson.h>

//Assimp
#include <assimp/material.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include <glm/gtc/bitfield.hpp>
#include <glm/ext.hpp>
#include <glm/common.hpp>
