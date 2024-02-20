#include "ShadowMap.h"
#include "../Core/GL.h"
#include "../Core/Scene.h"

void ShadowMap::Init()
{
	if (id == 0)
	{
		glGenFramebuffers(1, &id);
		glGenTextures(1, &depthTexture);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glBindTexture(GL_TEXTURE_2D, depthTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer is not complete.\n";

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 50.0f);
	glm::mat4 lightView = glm::lookAt(-Scene::globalLight.direction * 20.0f, Scene::globalLight.direction, glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = lightProjection * lightView;
}

void ShadowMap::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, GL::GetWindowWidth(), GL::GetWindowHeight());
}

void ShadowMap::Destroy()
{
	glDeleteTextures(1, &depthTexture);
	glDeleteFramebuffers(1, &id);
}

void ShadowMap::Clear()
{
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glClear(GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
