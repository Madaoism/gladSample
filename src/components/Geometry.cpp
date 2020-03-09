#include "Geometry.h"

int Geometry::objectCount = 0;

Geometry::Geometry()
  : _mUniqueId(++objectCount),
  _mVerticesVbo(0), 
  _mNormalsVbo(0), 
  _mTexVbo(0), 
  _mIndicesEbo(0),
  _mObjectVao(0),
  vertices(),
  normals(),
  texCoords(),
  indices(),
  renderWireMesh(false)
{

}

Geometry::~Geometry() 
{

}

void Geometry::initArrayObject() 
{
  // assuming all triangles
  unsigned int numVertices = vertices.size() / 3;
  unsigned int numTexCoords = texCoords.size() / 2;
  unsigned int numNormals = normals.size() / 3;
  unsigned int numFaces = indices.size() / 3;

  if (numVertices == 0) {
    Log.print<Severity::warning>("Trying to initialize a VAO with no vertices!");
  }

  if (numTexCoords > 0 && numVertices != numTexCoords) {
    Log.print<Severity::warning>("Trying to initialize tex coords with different size!");
  }

  if (numNormals > 0 && numVertices != numNormals) {
    Log.print<Severity::warning>("trying to initialize normals with different size!");
  }

  if (numFaces == 0) {
    Log.print<Severity::warning>("Trying to initialize a mesh without faces");
  }

  glGenVertexArrays(1, &_mObjectVao);
  glBindVertexArray(_mObjectVao);

  if (numVertices) 
  {
    // generate buffer object
    glGenBuffers(1, &_mVerticesVbo);
    glBindBuffer(GL_ARRAY_BUFFER, _mVerticesVbo);
    glBufferData(
      GL_ARRAY_BUFFER,
      sizeof(float) * vertices.size(),
      static_cast<const void*>(vertices.data()),
      GL_STATIC_DRAW
    );
    // define position vertex attribute
    glVertexAttribPointer(
      Geometry::ATTRIBUTE_POSITION,  // attribute layout
      Geometry::SIZE_POSITION,       // size of attribute
      GL_FLOAT,                        // type of attribute value
      GL_FALSE,                        // normalize data?
      Geometry::SIZE_POSITION * sizeof(float),  // data stride
      (void*)0                         // data offset
    );
    glEnableVertexAttribArray(Geometry::ATTRIBUTE_POSITION);
  }

  if (numNormals) 
  {
    // generate buffer object
    glGenBuffers(1, &_mNormalsVbo);
    glBindBuffer(GL_ARRAY_BUFFER, _mNormalsVbo);
    glBufferData(
      GL_ARRAY_BUFFER,
      sizeof(float) * normals.size(),
      static_cast<const void*>(normals.data()),
      GL_STATIC_DRAW
    );
    // define normal vertex attribute
    glVertexAttribPointer(
      Geometry::ATTRIBUTE_NORMAL,
      Geometry::SIZE_NORMAL,
      GL_FLOAT,
      GL_FALSE,
      Geometry::SIZE_NORMAL * sizeof(float),
      (void*)0
    );
    glEnableVertexAttribArray(Geometry::ATTRIBUTE_NORMAL);
  }

  if (numTexCoords) 
  {
    // generate buffer object
    glGenBuffers(1, &_mTexVbo);
    glBindBuffer(GL_ARRAY_BUFFER, _mTexVbo);
    glBufferData(
      GL_ARRAY_BUFFER,
      sizeof(float) * texCoords.size(),
      static_cast<const void*>(texCoords.data()),
      GL_STATIC_DRAW
    );

    // define texture vertex attribute
    glVertexAttribPointer(
      Geometry::ATTRIBUTE_TEX,
      Geometry::SIZE_TEX,
      GL_FLOAT,
      GL_FALSE,
      Geometry::SIZE_TEX * sizeof(float),
      (void*)0
    );
    glEnableVertexAttribArray(Geometry::ATTRIBUTE_TEX);
  }

  if (numFaces)
  {
    glGenBuffers(1, &_mIndicesEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mIndicesEbo);
    glBufferData(
      GL_ELEMENT_ARRAY_BUFFER,
      sizeof(unsigned int) * indices.size(),
      static_cast<const void*>(indices.data()),
      GL_STATIC_DRAW
    );
  }

  glBindVertexArray(0);
}

void Geometry::render() const
{
  if (!_mObjectVao)
  {
    Log.print<Severity::warning>("Mesh ", _mUniqueId, "'s VAO is not initialized yet!");
    return;
  }

  glBindVertexArray(_mObjectVao);

  if (renderWireMesh) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }
  else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  if (_mIndicesEbo) glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  else if (_mVerticesVbo) glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
  else {
    Log.print<Severity::warning>("Mesh ", _mUniqueId, " does not have vertices!");
    return;
  }

  glBindVertexArray(0);
}