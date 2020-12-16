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
#include "AssimpHelper.h"

aiMesh const * get_first_mesh(aiNode *node, aiScene const *scene) {
    if (node) {
        if (node->mMeshes) {
            return scene->mMeshes[node->mMeshes[0]];
        } else {
            for (unsigned int i=0; i<node->mNumChildren; ++i) {
                aiMesh const *mesh = get_first_mesh(node->mChildren[i], scene);
                if (mesh) return mesh;
            }
        }
    }
    
    return nullptr;
}

aiRetMeshItem get_mesh_by_name(aiNode *node, aiMatrix4x4 parentTrans, aiScene const *scene, aiString const &name) {
    aiRetMeshItem retItem;
    retItem.mesh = nullptr;
    if (node) {
        aiMatrix4x4 curTrans = parentTrans * node->mTransformation;
        for (unsigned int i=0; i<node->mNumMeshes; ++i) {
            aiMesh *curMesh = scene->mMeshes[node->mMeshes[i]];
            if (curMesh && curMesh->mName == name) {
                retItem.mesh = curMesh;
                retItem.transform = curTrans;
                return retItem;
            }
        }
        
        for (unsigned int i=0; i<node->mNumChildren; ++i) {
            retItem = get_mesh_by_name(node->mChildren[i], curTrans, scene, name);
            if (retItem.mesh) return retItem;
        }
    }
    return retItem;    
}

void get_all_meshes(aiNode *node, aiMatrix4x4 parentTrans, aiScene const *scene, aiRetMeshArray &meshes) {
    if (node) {
        aiMatrix4x4 curTrans = parentTrans * node->mTransformation;
        for (unsigned int i=0; i<node->mNumMeshes; ++i) {
            aiRetMeshItem curItem;
            curItem.mesh = scene->mMeshes[node->mMeshes[i]];
            curItem.transform = curTrans;
            meshes.push_back(curItem);
        }
        
        for (unsigned int i=0; i<node->mNumChildren; ++i) {
            get_all_meshes(node->mChildren[i], curTrans, scene, meshes);
        }
    }
}
