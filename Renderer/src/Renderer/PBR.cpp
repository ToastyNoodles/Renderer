#include "PBR.h"
#include <stb_image.h>

void PBR::Load(const char* filepath)
{
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrComponents;
    float* data = stbi_loadf(filepath, &width, &height, &nrComponents, 0);
    if (data)
    {
        glGenTextures(1, &environmentTexture);
        glBindTexture(GL_TEXTURE_2D, environmentTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        std::cout << "Loaded HDR texture " << filepath << "\n";
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load HDR texture " << filepath << "\n";
    }
}

void PBR::CreateEnvironmentCubemap(Shader shader)
{
    glGenFramebuffers(1, &captureFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

    glGenRenderbuffers(1, &captureRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, cubemapSize, cubemapSize);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    glGenTextures(1, &environmentCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, environmentCubemap);
    for (uint32_t i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, cubemapSize, cubemapSize, 0, GL_RGB, GL_FLOAT, nullptr);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    shader.Bind();
    shader.SetMat4("projection", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, environmentTexture);

    glViewport(0, 0, cubemapSize, cubemapSize);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (uint32_t i = 0; i < 6; ++i)
    {
        shader.SetMat4("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, environmentCubemap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Draw();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PBR::CreateIrradianceMap(Shader shader)
{
    glGenFramebuffers(1, &captureFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

    glGenRenderbuffers(1, &captureRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, irradianceSize, irradianceSize);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    glGenTextures(1, &irradianceMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    for (uint32_t i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, irradianceSize, irradianceSize, 0, GL_RGB, GL_FLOAT, nullptr);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    shader.Bind();
    shader.SetMat4("projection", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, environmentTexture);

    glViewport(0, 0, irradianceSize, irradianceSize);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (uint32_t i = 0; i < 6; ++i)
    {
        shader.SetMat4("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Draw();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PBR::CreatePrefilterMap(Shader shader)
{

}

void PBR::CreateBrdfLUTTexture(Shader shader)
{

}