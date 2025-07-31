#include "Mesh.hpp"

// void Model::addMesh(const Mesh &mesh) { meshes.push_back(mesh); }
//
// void Model::render(Shader *shader) {
//   for (const auto &mesh : meshes) {
//     shader->setMat4("model", mesh.transform);
//     glBindVertexArray(mesh.vao);
//     glActiveTexture(GL_TEXTURE0);
//     glBindTexture(GL_TEXTURE_2D, mesh.texture);
//     shader->setInt("texture1", 0);
//     glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_SHORT, 0);
//   }
// }
