#ifndef SHADER_HPP
#define SHADER_HPP
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
    public:
        unsigned int shaderProgram;
        Shader(const char* vertexPath, const char* fragmentPath);
        void use();
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void setVec2(const std::string &name, const glm::vec2 &value) const;
        void setVec4(const std::string &name, const glm::vec4 &value) const;
        void setMat4(const std::string &name, const glm::mat4 &value) const;
        void setMat4Array(const std::string &name, const glm::mat4 &value, int size) const;
};


#endif