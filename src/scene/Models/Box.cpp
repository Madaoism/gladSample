#include "Box.h"
#include <sstream>

Box::Box(PrimitiveManager& primitiveManager, float w, float h, float d, bool invertNormal)
  : Model(nullptr)
{
  Primitive* boxPrimitive = nullptr;
  std::stringstream idStream;
  idStream << "createBox_" << w << "_" << h << "_" << d << "_" << invertNormal;
  std::string id = idStream.str();

  if ( !(boxPrimitive = primitiveManager.find(id)) )
  {
    PrimitiveData boxMesh;

    float halfW = w / 2.f;
    float halfH = h / 2.f;
    float halfD = d / 2.f;

    glm::vec3 p1(-halfW, -halfH, -halfD);
    glm::vec3 p2(halfW, -halfH, -halfD);
    glm::vec3 p3(halfW, -halfH, halfD);
    glm::vec3 p4(-halfW, -halfH, halfD);

    glm::vec3 p5(-halfW, halfH, -halfD);
    glm::vec3 p6(halfW, halfH, -halfD);
    glm::vec3 p7(halfW, halfH, halfD);
    glm::vec3 p8(-halfW, halfH, halfD);

    boxMesh.vertices = {
      // bottom - 1 2 3 4
      p1.x, p1.y, p1.z,
      p2.x, p2.y, p2.z,
      p3.x, p3.y, p3.z,
      p4.x, p4.y, p4.z,

      // top - 5 6 7 8
      p5.x, p5.y, p5.z,
      p6.x, p6.y, p6.z,
      p7.x, p7.y, p7.z,
      p8.x, p8.y, p8.z,

      // front
      p8.x, p8.y, p8.z,
      p4.x, p4.y, p4.z,
      p3.x, p3.y, p3.z,
      p7.x, p7.y, p7.z,

      // back
      p5.x, p5.y, p5.z,
      p6.x, p6.y, p6.z,
      p2.x, p2.y, p2.z,
      p1.x, p1.y, p1.z,

      // right
      p6.x, p6.y, p6.z,
      p7.x, p7.y, p7.z,
      p3.x, p3.y, p3.z,
      p2.x, p2.y, p2.z,

      // left
      p8.x, p8.y, p8.z,
      p5.x, p5.y, p5.z,
      p1.x, p1.y, p1.z,
      p4.x, p4.y, p4.z
    };

    boxMesh.normals = {
      // bottom
      0, -1, 0,
      0, -1, 0,
      0, -1, 0,
      0, -1, 0,

      // top
      0, 1, 0,
      0, 1, 0,
      0, 1, 0,
      0, 1, 0,

      // front
      0, 0, 1,
      0, 0, 1,
      0, 0, 1,
      0, 0, 1,

      // back
      0, 0, -1,
      0, 0, -1,
      0, 0, -1,
      0, 0, -1,

      // right
      1, 0, 0,
      1, 0, 0,
      1, 0, 0,
      1, 0, 0,

      // left
      -1, 0, 0,
      -1, 0, 0,
      -1, 0, 0,
      -1, 0, 0
    };

    if (invertNormal)
    {
      for (int i = 0; i < boxMesh.normals.size(); i++)
      {
        boxMesh.normals[i] = -boxMesh.normals[i];
      }
    }

    boxMesh.texCoords = {
      // bottom - 1 2 3 4
      0, 0,
      1.f, 0,
      1.f, 1.f,
      0, 1.f,

      // top - 5 6 7 8
      0, 1.f,
      1.f, 1.f,
      1.f, 0,
      0, 0,

      // front - 8 4 3 7
      0, 1.f,
      0, 0,
      1.f, 0,
      1.f, 1.f,

      // back - 5 6 2 1
      1.f, 1.f,
      0, 1.f,
      0, 0,
      1.f, 0,

      // right - 6 7 3 2
      1.f, 1.f,
      0, 1.f,
      0, 0,
      1.f, 0,

      // left - 8 5 1 4
      1.f, 1.f,
      0, 1.f,
      0, 0,
      1.f, 0
    };

    boxMesh.indices = {
      // bottom
      0, 1, 3,
      2, 3, 1,

      // top
      4, 7, 5,
      6, 5, 7,

      // front - 8 4 3 7
      8, 9, 10,
      10, 11, 8,

      // back - 5 6 2 1
      12, 13, 14,
      14, 15, 12,

      // right - 6 7 3 2
      16, 17, 18,
      18, 19, 16,

      // left - 8 5 1 4
      20, 21, 22,
      22, 23, 20
    };

    boxPrimitive = primitiveManager.insert(id, boxMesh);
  }

  _mPrimitive = boxPrimitive;
}

Box::~Box() 
{

}