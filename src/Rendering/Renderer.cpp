#include "Renderer.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Renderer::Renderer(Shader* shader, Camera* camera, GLuint width, GLuint height)
    : shader(shader), camera(camera), width(width), height(height)
{
    perspective =
        glm::perspective(glm::radians(45.0f), (float) width / (float) height, 0.1f, 100.0f);
}

void Renderer::Render(const std::vector<Mesh>& modelMeshes)
{
    shader->use();
    shader->setMat4("view", camera->GetViewMatrix());
    shader->setMat4("projection", perspective);

    for (const auto& mesh : modelMeshes)
    {
        if (mesh.transparent)
            continue;

        shader->setMat4("model", mesh.transform);
        glBindVertexArray(mesh.vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mesh.texture);
        shader->setInt("texture1", 0);
        glDrawElements(GL_TRIANGLES, mesh.indexCount, mesh.indexType, 0);
    }

    glDepthMask(GL_FALSE);
    for (const auto& mesh : modelMeshes)
    {
        if (!mesh.transparent)
            continue;

        shader->setMat4("model", mesh.transform);
        glBindVertexArray(mesh.vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mesh.texture);
        shader->setInt("texture1", 0);
        glDrawElements(GL_TRIANGLES, mesh.indexCount, mesh.indexType, 0);
    }
    glDepthMask(GL_TRUE);
}

void Renderer::Render(const Mesh& modelMesh)
{
    shader->use();
    shader->setMat4("view", camera->GetViewMatrix());
    shader->setMat4("projection", perspective);

    shader->setMat4("model", modelMesh.transform);
    glBindVertexArray(modelMesh.vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, modelMesh.texture);
    shader->setInt("texture1", 0);
    glDrawElements(GL_TRIANGLES, modelMesh.indexCount, modelMesh.indexType, 0);
}

void Renderer::handleResize(GLuint nwidth, GLuint nheight)
{
    perspective =
        glm::perspective(glm::radians(45.0f), (float) nwidth / (float) nheight, 0.1f, 100.0f);
};
