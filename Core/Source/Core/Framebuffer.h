#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>


namespace SHAD 
{
	class Framebuffer
	{
	public:
		Framebuffer(uint32_t width, uint32_t height)
			: mFBO(0)
			, mTextureID(0)
			, mWidth(width)
			, mHeight(height)
			, mColor(1, 1, 1, 1)
		{
			// Create Frame Buffer
			glGenFramebuffers(1, &mFBO);
			glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
			// Create Color Texture
			glGenTextures(1, &mTextureID);
			glBindTexture(GL_TEXTURE_2D, mTextureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTextureID, 0);
			// Check FBO for completness
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "Frame Buffer Is Not compiled";

		}
		~Framebuffer()
		{
			glDeleteFramebuffers(1, &mFBO);
			mFBO = 0;
			mTextureID;
		}

		inline uint32_t GetFBO() const { return mFBO; }
		inline uint32_t GetTextureID() const { return mTextureID; }
		inline void GetSize(uint32_t& width, uint32_t& height) const { width = mWidth; height = mHeight; }
		inline void GetClearColor(glm::vec4& color) const { color = mColor; }
		inline void SetClearColor(glm::vec4& color) { mColor = color; }

		void BindBuffer() 
		{
			glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
			glClear(GL_COLOR_BUFFER_BIT);
		}
		void UnBindBuffer()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClear(GL_COLOR_BUFFER_BIT);
		}

	private:
		uint32_t mFBO;
		uint32_t mTextureID;
		uint32_t mWidth, mHeight;
		glm::vec4 mColor;
	};
}