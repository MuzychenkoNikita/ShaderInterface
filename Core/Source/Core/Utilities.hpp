#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

namespace SHAD {
    void addTexture2D(uint32_t& textureID, char const *path) {
        glGenTextures(1, &textureID);
            glBindTexture(GL_TEXTURE_2D, textureID);

            // Set the texture wrapping and filtering options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // Load the texture image
            int width, height, nrChannels;
            stbi_set_flip_vertically_on_load(true); // Flip texture vertically
            unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            else
            {
                std::cout << "Failed to load texture" << std::endl;
            }
            if (!data) {
                std::cerr << "Failed to load texture: " << path
                          << "\nCWD: " << std::filesystem::current_path()
                          << "\nExists? " << std::filesystem::exists(path)
                          << "\nReason: " << stbi_failure_reason() << "\n";
                return;
            }
            stbi_image_free(data); // Free the texture data
    }
}
