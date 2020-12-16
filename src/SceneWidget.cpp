/**
 * -------------------------------------------------------------------------------
 * This source file is part of CGQtAppBase, one of the examples for
 * Computer Graphics Course of School of Engineering Science,
 * University of Chinese Academy of Sciences (UCAS).
 * Copyright (C) 2020 Xue Jian (xuejian@ucas.ac.cn)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * -------------------------------------------------------------------------------
 */
#include "SceneWidget.h"
#include "AssimpHelper.h"
#include "GLUtils.h"
#include "LogUtils.h"
#include "OpenGLMaterialEntity.h"
#include "OpenGLRenderableEntity.h"

#include <QOpenGLShaderProgram>

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

SceneWidget::SceneWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    mPhongSimpleProgram = nullptr;

    set_float4(mBackgroundColor, 0.0f, 0.0f, 0.0f, 0.0f);
    mSceneCenter = glm::zero<glm::vec3>();
    mSceneBounds[0] = mSceneBounds[1] = mSceneBounds[2] = mSceneBounds[3] = mSceneBounds[4] = mSceneBounds[5] = 0.0f;
    mCameraZoomSpeed = 1.0f;
    mCameraPanSpeed = 1.0f;
    mSceneRadius = 1.0f;
    mAngleFoV = 60.0f;

    mOpenGLInitialized = false;
    mNeedToAlignScene = false;
}

SceneWidget::~SceneWidget()
{
    this->cleanupGL();
    disconnect(this->context(), &QOpenGLContext::aboutToBeDestroyed, this, &SceneWidget::cleanupGL);
}

void SceneWidget::cleanupGL()
{
    this->makeCurrent();
    this->cleanupSceneGL();
    mDefaultMaterial->destroyGL(this->context());
    DELETE_OPENGL_RESOURCE(mPhongSimpleProgram);
    this->doneCurrent();
}

void SceneWidget::initializeGL()
{
    mOpenGLInitialized = false;

    connect(this->context(), &QOpenGLContext::aboutToBeDestroyed, this, &SceneWidget::cleanupGL);
    this->initializeOpenGLFunctions(); // IMPORTANT!

    glClearColor(mBackgroundColor[0], mBackgroundColor[1], mBackgroundColor[2], mBackgroundColor[3]);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mProjectionMatrix = glm::mat4x4(1.0f);
    mCameraMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    mModelMatrix = glm::mat4x4(1.0f);
    mModelViewMatrix = mCameraMatrix * mModelMatrix;
    mLight.setPosition(0.0f, 0.0f, 0.0f);
    mDefaultMaterial = std::make_shared<OpenGLMaterialEntity>();
    mTrackBall.setRadius(0.6f);
    mTrackBall.reset();

    mPhongSimpleProgram = new QOpenGLShaderProgram;
#if defined(USE_COMPATIBILITY_PROFILE) || defined(USE_OPENGLES)
    if (!initialize_shader_program_comp("PhongSimple", mPhongSimpleProgram, "phong_simple_comp.vert", "phong_simple_comp.frag",
                                        [](QOpenGLShaderProgram *program) -> void {
                                            program->bindAttributeLocation("positionIn", VertexAttribute::POSITION);
                                            program->bindAttributeLocation("normalIn", VertexAttribute::NORMAL);
                                        })) {
        return;
    }
#else
    if (!initialize_shader_program("PhongSimple", mPhongSimpleProgram, "phong_simple.vert", "phong_simple.frag")) {
        return;
    }
#endif

    mOpenGLInitialized = true;
    mNeedToAlignScene = true;
}

void SceneWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glGetIntegerv(GL_VIEWPORT, mViewport);

    mNeedToAlignScene = true;
}

void SceneWidget::paintGL()
{
    if (!mOpenGLInitialized) return;
    aiScene const *scene = mSceneImporter.GetScene();
    if (scene == nullptr) return;

    this->alignScene();

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    //glFrontFace(GL_CW);
    glEnable(GL_BLEND);

    mModelViewMatrix = mCameraMatrix * mModelMatrix;
    mLight.getPosition(mLightPos);
    //mLightPos = mCameraMatrix * mLightPos;
    this->drawSceneNode(mModelViewMatrix, scene->mRootNode);
}

void SceneWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        int x = event->x()-mViewport[0];
        int y = mViewport[3]-1-event->y()-mViewport[1];
        mTrackBall.start(2.0*x/mViewport[2]-1.0, 2.0*y/mViewport[3]-1.0);
        this->update();
        event->accept();
    } else if (event->buttons() & Qt::RightButton) {
        event->accept();
    } else {
        event->ignore();
    }

    mLastMousePos = event->pos();
}

void SceneWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        int x = event->x()-mViewport[0];
        int y = mViewport[3]-1-event->y()-mViewport[1];
        mTrackBall.update(2.0*x/mViewport[2]-1.0, 2.0*y/mViewport[3]-1.0);
        glm::mat4x4 rotMat = glm::mat4_cast(mTrackBall.currentQuaternion());
        mModelMatrix = glm::translate(glm::mat4(1.0f), mSceneCenter);
        mModelMatrix = mModelMatrix * rotMat;
        mModelMatrix = glm::translate(mModelMatrix, -mSceneCenter);
        this->update();
        event->accept();
    } else if (event->buttons() & Qt::RightButton) {
        float dx = static_cast<float>(event->pos().x() - mLastMousePos.x()) * mCameraPanSpeed;
        float dy = static_cast<float>(mLastMousePos.y() - event->pos().y()) * mCameraPanSpeed;
        this->cameraPan(dx, dy);
        event->accept();
    } else {
        event->ignore();
    }

    mLastMousePos = event->pos();
}

void SceneWidget::wheelEvent(QWheelEvent *event)
{
    QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta() / 8;

    float dz = 0;
    if (!numPixels.isNull()) {
        dz = numPixels.y() * mCameraZoomSpeed;
    } else if (!numDegrees.isNull()) {
        QPoint numSteps = numDegrees / 15;
        dz = numSteps.y() * mCameraZoomSpeed;
    }
    this->cameraZoom(dz);
    event->accept();
}

void SceneWidget::loadSceneFromFile(const QString &pathName)
{
    if (pathName.isEmpty()) return;
    aiScene const *scene = mSceneImporter.ReadFile(pathName.toLocal8Bit().constData(),
                                                   aiProcess_Triangulate |
                                                   aiProcess_JoinIdenticalVertices |
                                                   aiProcess_GenSmoothNormals);
    if (scene == nullptr) {
        LOG_ERROR_QSTRING(tr("Fail to read scene from file %1!").arg(pathName));
        return;
    }

    this->loadSceneData(scene);
}

void SceneWidget::loadSceneData(aiScene const *scene)
{
    if (scene == nullptr) return;

    this->makeCurrent();

    this->cleanupSceneGL();

    mMaterials.clear();
    mRenderables.clear();

    for (unsigned int i=0; i<scene->mNumMaterials; ++i) {
        aiMaterial *sceneMaterial = scene->mMaterials[i];
        OpenGLMaterialEntityPtr newMaterialEntity = std::make_shared<OpenGLMaterialEntity>();
        if (newMaterialEntity->loadData(this->context(), sceneMaterial)) {
            newMaterialEntity->setName(sceneMaterial->GetName().C_Str());
            mMaterials.emplace_back(newMaterialEntity);
        } else {
            // hold the place to keep consistent with the scene structure of assimp
            // 占位，保持与assimp导入的场景图中的material索引一致
            mMaterials.emplace_back(OpenGLMaterialEntityPtr());
        }
    }

    for (unsigned int i=0; i<scene->mNumMeshes; ++i) {
        aiMesh const *sceneMesh = scene->mMeshes[i];
        OpenGLRenderableEntityPtr newRenderableEntity = std::make_shared<OpenGLRenderableEntity>();
        if (newRenderableEntity->loadData(this->context(), sceneMesh)) {
            newRenderableEntity->setName(sceneMesh->mName.C_Str());
            newRenderableEntity->setMaterial(mMaterials[sceneMesh->mMaterialIndex]);
            mRenderables.emplace_back(newRenderableEntity);
        } else {
            // hold the place to keep consistent with the scene structure of assimp
            // 占位，保持与assimp导入的场景图中的mesh索引一致
            mRenderables.emplace_back(OpenGLRenderableEntityPtr());
        }
    }

    this->doneCurrent();

    for (unsigned int i=0; i<scene->mNumLights; ++i) {
        aiLight const *light = scene->mLights[i];
        if (light->mType!=aiLightSource_POINT && light->mType!=aiLightSource_DIRECTIONAL) continue;
        if (light->mType == aiLightSource_POINT) {
            mLight.setPosition(light->mPosition.x, light->mPosition.y, light->mPosition.z);
        }
        mLight.setAmbient(light->mColorAmbient.r, light->mColorAmbient.g, light->mColorAmbient.b);
        mLight.setDiffuse(light->mColorDiffuse.r, light->mColorDiffuse.g, light->mColorDiffuse.b);
        mLight.setSpecular(light->mColorSpecular.r, light->mColorSpecular.g, light->mColorSpecular.b);
        break;
    }

    this->recalculateBoundsCenter();
    mNeedToAlignScene = true;
    this->update();
}

void SceneWidget::cleanupSceneGL()
{
    for (OpenGLMaterialEntityPtr me : mMaterials) {
        if (me) me->destroyGL(this->context());
    }
    for (OpenGLRenderableEntityPtr re : mRenderables) {
        if (re) re->destroyGL(this->context());
    }
    mSceneCenter = glm::zero<glm::vec3>();
    mSceneBounds[0] = mSceneBounds[1] = mSceneBounds[2] = mSceneBounds[3] = mSceneBounds[4] = mSceneBounds[5] = 0.0f;
}

void SceneWidget::clearSceneData()
{
    for (OpenGLRenderableEntityPtr renderable : mRenderables) {
        if (renderable) renderable->clearData();
    }
    mSceneCenter = glm::zero<glm::vec3>();
    mSceneBounds[0] = mSceneBounds[1] = mSceneBounds[2] = mSceneBounds[3] = mSceneBounds[4] = mSceneBounds[5] = 0.0f;
}

void SceneWidget::alignScene()
{
    if (!mNeedToAlignScene) return;

    float verticalAngle = mAngleFoV;
    float half_h = mSceneRadius;

    float dist = half_h / tan(glm::radians(verticalAngle*0.5f));
    if (dist < half_h+0.2f) dist = half_h+0.2f;
    mCameraPos = mSceneCenter + glm::vec3(0, 0, dist);
    mTargetPos = mSceneCenter;
    mCameraDir = mTargetPos - mCameraPos;
    mCameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    mCameraZoomSpeed = dist * 0.05f;
    mCameraPanSpeed = mSceneRadius * 0.01f;

    mTrackBall.reset();
    mModelMatrix = glm::mat4x4(1.0f);
    mCameraMatrix = glm::lookAt(mCameraPos, mTargetPos, mCameraUp);
    mProjectionMatrix = glm::perspective(glm::radians(verticalAngle),
                                         (float)(mViewport[2])/(float)(mViewport[3]),
                                         (dist - half_h)*0.5f,
                                         (dist + half_h)*2.0f);

    mNeedToAlignScene = false;
}

void SceneWidget::recalculateBoundsCenter()
{
    bool initialized = false;
    for (size_t i=0; i<mRenderables.size(); ++i) {
        if (!mRenderables[i]) continue;
        float const *rb = mRenderables[i]->bounds();
        if (initialized) {
            if (rb[0] < mSceneBounds[0]) mSceneBounds[0] = rb[0];
            if (rb[1] > mSceneBounds[1]) mSceneBounds[1] = rb[1];
            if (rb[2] < mSceneBounds[2]) mSceneBounds[2] = rb[2];
            if (rb[3] > mSceneBounds[3]) mSceneBounds[3] = rb[3];
            if (rb[4] < mSceneBounds[4]) mSceneBounds[4] = rb[4];
            if (rb[5] > mSceneBounds[5]) mSceneBounds[5] = rb[5];
        } else {
            mSceneBounds[0] = rb[0];
            mSceneBounds[1] = rb[1];
            mSceneBounds[2] = rb[2];
            mSceneBounds[3] = rb[3];
            mSceneBounds[4] = rb[4];
            mSceneBounds[5] = rb[5];
            initialized = true;
        }
    }
    float lx = mSceneBounds[1]-mSceneBounds[0];
    float ly = mSceneBounds[3]-mSceneBounds[2];
    float lz = mSceneBounds[5]-mSceneBounds[4];
    mSceneRadius = sqrt(lx*lx+ly*ly+lz*lz)*0.5f;

    mSceneCenter.x = (mSceneBounds[0] + mSceneBounds[1]) * 0.5f;
    mSceneCenter.y = (mSceneBounds[2] + mSceneBounds[3]) * 0.5f;
    mSceneCenter.z = (mSceneBounds[4] + mSceneBounds[5]) * 0.5f;
}

void SceneWidget::drawRenderableEntity(glm::mat4x4 const &modelMat, OpenGLRenderableEntityPtr renderableEntity)
{
    if (!renderableEntity) return;
    OpenGLMaterialEntityPtr material = renderableEntity->material();
    if (!material) material = mDefaultMaterial;

    QOpenGLShaderProgram *glslProgram = mPhongSimpleProgram;

    glslProgram->bind();
    glm::mat3x3 normMat = glm::transpose(glm::inverse(glm::mat3x3(modelMat)));
    glUniformMatrix4fv(glslProgram->uniformLocation("projectionMatrix"), 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));
    glUniformMatrix4fv(glslProgram->uniformLocation("modelViewMatrix"), 1, GL_FALSE, glm::value_ptr(modelMat));
    glUniformMatrix3fv(glslProgram->uniformLocation("normalMatrix"), 1, GL_FALSE, glm::value_ptr(normMat));
    glUniform4fv(glslProgram->uniformLocation("lightPosition"), 1, glm::value_ptr(mLightPos));
    glUniform4fv(glslProgram->uniformLocation("lightAmbient"), 1, mLight.ambient());
    glUniform4fv(glslProgram->uniformLocation("lightDiffuse"), 1, mLight.diffuse());
    glUniform4fv(glslProgram->uniformLocation("lightSpecular"), 1, mLight.specular());
    glUniform4fv(glslProgram->uniformLocation("materialAmbient"), 1, material->ambient());
    glUniform4fv(glslProgram->uniformLocation("materialDiffuse"), 1, material->diffuse());
    glUniform4fv(glslProgram->uniformLocation("materialEmission"), 1, material->emission());
    glUniform4fv(glslProgram->uniformLocation("materialSpecular"), 1, material->specular());
    glUniform1f(glslProgram->uniformLocation("materialShininess"), material->shininess());
    renderableEntity->drawSurface(this->context());
    glslProgram->release();
}

void SceneWidget::drawSceneNode(glm::mat4x4 const &parentModelMat, const aiNode *node)
{
    if (node == nullptr) return;
    glm::mat4x4 nodeModelMat = get_glm_mat4x4(node->mTransformation);
    glm::mat4x4 modelMat = parentModelMat * nodeModelMat;
    for (unsigned int i=0; i<node->mNumMeshes; ++i) {
        unsigned int curMeshIdx = node->mMeshes[i];
        assert(curMeshIdx < mRenderables.size());
        OpenGLRenderableEntityPtr renderableEntity = mRenderables[node->mMeshes[i]];
        this->drawRenderableEntity(modelMat, renderableEntity);
    }

    for (unsigned int i=0; i<node->mNumChildren; ++i) {
        this->drawSceneNode(modelMat, node->mChildren[i]);
    }
}

void SceneWidget::cameraZoom(float dz)
{
    mCameraPos += glm::normalize(mCameraDir) * dz;
    float dist = glm::distance(mCameraPos, mTargetPos);
    float zNear = dist > mSceneRadius ? (dist-mSceneRadius) * 0.5f : (mSceneRadius-dist) * 0.1f;
    float zFar = (dist + mSceneRadius)*2.0f;
    mCameraMatrix = glm::lookAt(mCameraPos, mTargetPos, mCameraUp);
    mProjectionMatrix = glm::perspective(glm::radians(mAngleFoV),
                                         (float)(mViewport[2])/(float)(mViewport[3]),
                                         zNear,
                                         zFar);
    this->update();
}

void SceneWidget::cameraPan(float dx, float dy)
{
    glm::vec3 axisX(1.0f, 0.0f, 0.0f);
    glm::vec3 axisY(0.0f, 1.0f, 0.0f);
    glm::vec3 panDir = axisX*dx + axisY*dy;
    mCameraPos += panDir;
    mTargetPos += panDir;
    mCameraMatrix = glm::lookAt(mCameraPos, mTargetPos, mCameraUp);
    this->update();
}
