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
#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include "glm/mat4x4.hpp"
#include "assimp/Importer.hpp"

#include "SharedPointerTypes.h"
#include "Light.h"
#include "TrackBall.h"

class QOpenGLShaderProgram;

struct aiNode;

class SceneWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit SceneWidget(QWidget *parent = nullptr);
    ~SceneWidget();

    void loadSceneFromFile(QString const &pathName);

    double animationFrameRate() const { return mAnimationFrameRate; }
    void setAnimationFrameRate(double frameRate) { mAnimationFrameRate = frameRate; }

    bool animationStarted() const;

public slots:
    void startAnimation();
    void stopAnimation();

signals:

protected slots:
    void cleanupGL();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;    
    virtual void timerEvent(QTimerEvent *event) override;

protected:
    void loadSceneData(aiScene const *scene, QString const &sourceFilePath);
    void cleanupSceneGL();
    void clearSceneData();
    void alignScene();
    void recalculateBoundsCenter();
    void drawRenderableEntity(glm::mat4x4 const &modelMat, OpenGLRenderableEntityPtr renderableEntity);
    void drawSceneNode(glm::mat4x4 const &parentModelMat, aiNode const *node);

    void cameraZoom(float dz);
    void cameraPan(float dx, float dy);

    void renderText(int x, int y, const QString &str, const QFont &font = QFont("Helvetica", 10), QColor const &color = QColorConstants::White);

protected:
    Assimp::Importer mSceneImporter;
    Light mLight;
    TrackBall mTrackBall;
    OpenGLMaterialEntityPtr mDefaultMaterial;
    OpenGLMaterialEntityArray mMaterials;
    OpenGLRenderableEntityArray mRenderables;

    QOpenGLShaderProgram *mPhongSimpleProgram;
    QOpenGLShaderProgram *mPhongTextureProgram;
    GLint mViewport[4];
    GLfloat mBackgroundColor[4];
    glm::mat4x4 mCameraMatrix;
    glm::mat4x4 mProjectionMatrix;
    glm::mat4x4 mModelMatrix;
    glm::mat4x4 mModelViewMatrix;
    glm::vec4 mLightPos;
    glm::vec3 mTargetPos;
    glm::vec3 mCameraPos;
    glm::vec3 mCameraDir;
    glm::vec3 mCameraUp;
    glm::vec3 mSceneCenter;
    float mSceneBounds[6];
    float mSceneRadius;
    float mCameraZoomSpeed;
    float mCameraPanSpeed;
    float mAngleFoV;

    QPoint mLastMousePos;
    double mAnimationFrameRate;
    unsigned int mAnimationFrameNum;
    int mAnimationTimerId;

    QElapsedTimer mFPSTimer;
    double mCurFPS;

    bool mOpenGLInitialized;
    bool mNeedToAlignScene;
};

#endif // SCENEWIDGET_H
