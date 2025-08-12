#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace SHAD
{

    class Shader
    {
    public:
        unsigned int ID;
        // constructor generates the shader on the fly
        // ------------------------------------------------------------------------
        Shader(const char* vertexPath, const char* fragmentPath)
        {
            setVertexPath(vertexPath);
            setFragmentPath(fragmentPath);

            // ensure ifstream objects can throw exceptions:
            mVertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            mFragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            try
            {
                // open files
                mVertexShaderFile.open(vertexPath);
                mFragmentShaderFile.open(fragmentPath);
                std::stringstream vShaderStream, fShaderStream;
                // read file's buffer contents into streams
                vShaderStream << mVertexShaderFile.rdbuf();
                fShaderStream << mFragmentShaderFile.rdbuf();
                // close file handlers
                mVertexShaderFile.close();
                mFragmentShaderFile.close();
                // convert stream into string
                mVertexCode = vShaderStream.str();
                mFragmentCode = fShaderStream.str();
            }
            catch (std::ifstream::failure& e)
            {
                std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
            }
            setup();
        }
        // activate the shader
        // ------------------------------------------------------------------------
        void use()
        {
            glUseProgram(ID);
        }
        void setup()
        {
            // delete previous program if any
            if (ID) { glDeleteProgram(ID); ID = 0; }

            const char* vShaderCode = mVertexCode.c_str();
            const char* fShaderCode = mFragmentCode.c_str();
            // 2. compile shaders
            unsigned int vertex, fragment;
            // vertex shader
            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            glCompileShader(vertex);
            checkCompileErrors(vertex, "VERTEX");
            // fragment Shader
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            glCompileShader(fragment);
            checkCompileErrors(fragment, "FRAGMENT");
            // shader Program
            ID = glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment);
            glLinkProgram(ID);
            checkCompileErrors(ID, "PROGRAM");
            // delete the shaders as they're linked into our program now and no longer necessary
            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }
        // utility uniform functions
        // ------------------------------------------------------------------------
        void setBool(const std::string& name, bool value) const
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
        }
        // ------------------------------------------------------------------------
        void setInt(const std::string& name, int value) const
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }
        // ------------------------------------------------------------------------
        void setFloat(const std::string& name, float value) const
        {
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
        }
        // ------------------------------------------------------------------------
        void setVec2(const std::string& name, const glm::vec2& value) const
        {
            glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        }
        // ------------------------------------------------------------------------
        void setFragmentCode(const std::string fragmentCode)
        {
            mFragmentCode = fragmentCode;
        }
        // ------------------------------------------------------------------------
        std::string getFragmentCode() { return mFragmentCode; };
        // ------------------------------------------------------------------------
        void setFragmentPath(const char* fragmentPath)
        {
            mFragmentPath = fragmentPath;
        }
        // ------------------------------------------------------------------------
        const char* getFragmentPath() { return mFragmentPath; }
        // ------------------------------------------------------------------------
        void setVertexPath(const char* vertexPath)
        {
            mVertexPath = vertexPath;
        }
        // ------------------------------------------------------------------------
        const char* getVertexPath() { return mVertexPath; }
        // ------------------------------------------------------------------------
        void saveFragmentFile()
        {
            std::ofstream file(mFragmentPath);
            file << mFragmentCode;
            file.close();
        }
        // ------------------------------------------------------------------------
        void openFragmentFile()
        {
            try
            {
                // open files
                mFragmentShaderFile.open(mFragmentPath);
                std::stringstream vShaderStream, fShaderStream;
                // read file's buffer contents into streams
                fShaderStream << mFragmentShaderFile.rdbuf();
                // close file handlers
                mFragmentShaderFile.close();
                // convert stream into string
                mFragmentCode = fShaderStream.str();
            }
            catch (std::ifstream::failure& e)
            {
                std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
            }
            setup();
        }
        // ------------------------------------------------------------------------
        void newFragmentFile()
        {
            std::ofstream file(mFragmentPath);
            file << "#version 330 core\nout vec4 FragColor;\nin vec2 TexCoord;\nvec2 FragCoord = gl_FragCoord.xy;\n\nuniform float iTime;\nuniform vec2 iResolution;\n\nvoid main()\n{\n    vec2 uv = FragCoord/iResolution.xy;\n    vec3 col = 0.5 + 0.5*cos(iTime+uv.xyx+vec3(0,2,4));\n    FragColor = vec4(col, 1.0);\n}"
;
            file.close();
            openFragmentFile();
        }


    private:

        std::string mVertexCode;
        std::string mFragmentCode;
        std::ifstream mVertexShaderFile;
        std::ifstream mFragmentShaderFile;
        const char* mVertexPath;
        const char* mFragmentPath;

        // utility function for checking shader compilation/linking errors.
        // ------------------------------------------------------------------------
        void checkCompileErrors(unsigned int shader, std::string type)
        {
            int success;
            char infoLog[1024];
            if (type != "PROGRAM")
            {
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                if (!success)
                {
                    glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                    std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                }
            }
            else
            {
                glGetProgramiv(shader, GL_LINK_STATUS, &success);
                if (!success)
                {
                    glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                    std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                }
            }
        }
    };
}