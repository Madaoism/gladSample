#include "Model.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

Model::Model(const Geometry* mesh)
  : GameObject(), _mMesh(mesh), material(nullptr)
{}

Model::~Model()
{}

void Model::update(float deltaT)
{
  GameObject::update(deltaT);
}

void Model::draw(const glm::mat4& PV, const glm::mat4& M) const
{
  if (_mMesh == nullptr) return;

  if (material != nullptr) {
    material->use();
  }

  glm::mat4 model = M * _mModelMatrix;
  glm::mat4 PVM = PV * model;
  glm::mat3 modelMat3(model);
  glm::mat3 normal = glm::inverseTranspose(modelMat3);

  material->setModelMatrix(model);
  material->setNormalMatrix(normal);
  material->setProjViewModelMatrix(PVM);

  _mMesh->render();

  GameObject::draw(PV, M);
}