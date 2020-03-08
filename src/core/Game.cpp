/*
 * Game.cpp
 *
 *  Created on: Feb 28, 2020
 *      Author: ycai
 */

#include "Game.h"

Game::Game() : 
  _mShaderManager(),
  _mTextureManager(),
  _mProgramManager(_mShaderManager),
  _mClearColor(0.1f, 0.7f, 0.5f, 1.0f),
  _mCurrentScene(new Scene())
{}

Game::~Game() 
{
  delete  _mCurrentScene;
}

void Game::init() 
{
  // our temporary mesh
  Geometry* triangleMesh = new Geometry();
  triangleMesh->vertices = {
     -0.5f, -0.5f, 0.0f,  // lower left
     0.5f, -0.5f, 0.0f,   // lower right
     0.0f,  0.5f, 0.0f,   // top
     0.0f, -1.f, 0.0f    // bottom
  };
  triangleMesh->texCoords = {
    0, 0,
    1.f, 0,
    0.5f, 1.f,
    0.5f, -.5f
  };
  triangleMesh->indices = {
    0, 1, 2,
    0, 1, 3
  };
  triangleMesh->initArrayObject();

  // our model
  Model* triModel = new Model(triangleMesh);

  // initialize the material
  ShaderInfo vertexShaderInfo(
    "./shaders/VertexShader.glsl",
    GL_VERTEX_SHADER
  );

  ShaderInfo fragmentShaderInfo(
    "./shaders/FragmentShader.glsl",
    GL_FRAGMENT_SHADER
  );

  ShaderProgramInfo programInfo(
    vertexShaderInfo,
    fragmentShaderInfo
  );

  TestMaterial* mat = new TestMaterial(
    _mProgramManager, 
    *_mProgramManager.getOrCreate(programInfo)
  );

  triModel->material = mat;

  // set the textures for the material
  TextureInfo wallTex(
    "assets/wall.jpg",
    true
  );
  mat->diffuseTex = _mTextureManager.getOrCreate(wallTex);

  TextureInfo fujiwaraTex(
    "assets/fujiwara.jpg",
    true
  );
  mat->specularTex = _mTextureManager.getOrCreate(fujiwaraTex);

  _mCurrentScene->addChild(triModel);
  _mCurrentScene->setActiveCamera(new TargetCamera());

  glClearColor(_mClearColor.r, _mClearColor.g, _mClearColor.b, _mClearColor.a);
}

void Game::render() const
{
  glClear(GL_COLOR_BUFFER_BIT);
  if (_mCurrentScene != nullptr)
  {
    _mCurrentScene->draw();
  }
}

void Game::update(float deltaT)
{
  if (_mCurrentScene != nullptr)
  {
    _mCurrentScene->update(deltaT);
  }
}
