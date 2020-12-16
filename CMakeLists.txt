# -------------------------------------------------------------------------------
# This source file is part of CGQtAppBase, one of the examples for
# Computer Graphics Course of School of Engineering Science,
# University of Chinese Academy of Sciences (UCAS).
# Copyright (C) 2020 Xue Jian (xuejian@ucas.ac.cn)
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
# -------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.5)

project(CGQtAppBase VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS ui)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets OpenGL REQUIRED)
find_package(assimp REQUIRED)
find_package(glm REQUIRED)

find_package(OpenGL REQUIRED)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(
  ${PROJECT_SOURCE_DIR}/AppInfo.h.in
  ${PROJECT_BINARY_DIR}/AppInfo.h
)

set(CGQTAPP_INCLUDE_FILES
  include/GLInc.h
  include/GLUtils.h
  include/LogUtils.h
  include/Light.h
  include/TrackBall.h
  include/SharedPointerTypes.h
  include/OpenGLMaterialEntity.h
  include/OpenGLRenderableEntity.h
  include/AssimpHelper.h
  include/Logger.h
  include/SceneWidget.h
  include/MainWindow.h
)

set(CGQTAPP_SOURCE_FILES
  src/GLUtils.cpp
  src/TrackBall.cpp
  src/Light.cpp
  src/OpenGLMaterialEntity.cpp
  src/OpenGLRenderableEntity.cpp
  src/AssimpHelper.cpp
  src/Logger.cpp
  src/SceneWidget.cpp
  src/MainWindow.cpp
  src/main.cpp  
)

set(CGQTAPP_UI_FILES
  ui/MainWindow.ui
)

set(CGQTAPP_QRC_FILES
  res/CGQtAppBase.qrc
)

set(TARGET_NAME CGQtApp)
add_executable(${TARGET_NAME}
  ${CGQTAPP_INCLUDE_FILES}
  ${CGQTAPP_SOURCE_FILES}
  ${CGQTAPP_UI_FILES}
  ${CGQTAPP_QRC_FILES}
)

target_include_directories(${TARGET_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR})
target_link_libraries(${TARGET_NAME} PRIVATE Qt5::Widgets Qt5::OpenGL assimp::assimp glm::glm ${OPENGL_LIBRARIES})
