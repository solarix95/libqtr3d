#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "qtr3dassimploader.h"

//-------------------------------------------------------------------------------------------------
void qtr3dAssimpNode(const aiScene *scene, aiNode *node, Qtr3dModel &model, const QMatrix4x4 &parentTransform);

//-------------------------------------------------------------------------------------------------
Qtr3dAssimpLoader::Qtr3dAssimpLoader()
{
}

//-------------------------------------------------------------------------------------------------
Qtr3dAssimpLoader::~Qtr3dAssimpLoader() = default;

//-------------------------------------------------------------------------------------------------
bool Qtr3dAssimpLoader::loadModel(Qtr3dModel &model, const QString &filename, Options opts)
{
    Qtr3dAssimpLoader loader;
    return loader.loadModel(model,filename, opts);
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dAssimpLoader::supportsFile(const QString &filename)
{
    int extPos = filename.lastIndexOf(".");
    if (extPos < 0)
        return false;
    QString ext = filename.toLower().mid(extPos);
    return aiIsExtensionSupported(ext.toUtf8().constData()) == AI_TRUE;
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dAssimpLoader::loadFile(Qtr3dModel &model, const QString &filename,Options opts)
{
    auto *scene = aiImportFile(filename.toUtf8().constData(),aiProcessPreset_TargetRealtime_MaxQuality);
    if (!scene)
        return false;

    if (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
        aiReleaseImport(scene);
        return false;
    }

    if (scene->mFlags & AI_SCENE_FLAGS_VALIDATION_WARNING) {
        aiReleaseImport(scene);
        return false;
    }

    QMatrix4x4 rootTransform(1,0,0,0,
                             0,1,0,0,
                             0,0,1,0,
                             0,0,0,1);

    qtr3dAssimpNode(scene, scene->mRootNode, model, rootTransform);
    aiReleaseImport(scene);
    return false;
}

//-------------------------------------------------------------------------------------------------
void qtr3dAssimpNode(const aiScene *scene, aiNode *node, Qtr3dModel &model, const QMatrix4x4 &parentTransform)
{
    Q_ASSERT(node);

    aiMatrix4x4 *t = &node->mTransformation;

    QMatrix4x4 nodeTransform(
                t->a1, t->a2, t->a3, t->a4,
                t->b1, t->b2, t->b3, t->b4,
                t->c1, t->c2, t->c3, t->c4,
                t->d1, t->d2, t->d3, t->d4);

    nodeTransform = parentTransform * nodeTransform;

    for (int m=0; m<node->mNumMeshes; m++) {
        auto *mesh = model.context()->createMesh(false)->startMesh(Qtr3d::Triangle);
        auto *aiMesh = scene->mMeshes[node->mMeshes[m]];
        if (aiMesh->mMaterialIndex >= 0) {
            auto *aiMaterial = scene->mMaterials[aiMesh->mMaterialIndex];

            aiColor3D color (0.f,0.f,0.f);
            aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE,color);
            qDebug() << color.r <<  color.g <<  color.b;
        }
        for (int v=0; v<aiMesh->mNumVertices; v++) {
            mesh->addVertex(QVector3D(aiMesh->mVertices[v].x,
                            aiMesh->mVertices[v].y,
                            aiMesh->mVertices[v].z),
                            QVector3D(aiMesh->mNormals[v].x,
                                      aiMesh->mNormals[v].y,
                                      aiMesh->mNormals[v].z)
                            );
        }

        for (int f=0; f<aiMesh->mNumFaces; f++) {
            if (aiMesh->mFaces[f].mNumIndices < 3 || aiMesh->mFaces[f].mNumIndices > 4)
                continue;

            mesh->addIndex(aiMesh->mFaces[f].mIndices[0]);
            mesh->addIndex(aiMesh->mFaces[f].mIndices[1]);
            mesh->addIndex(aiMesh->mFaces[f].mIndices[2]);

            if (aiMesh->mFaces[f].mNumIndices == 4) {
                mesh->addIndex(aiMesh->mFaces[f].mIndices[0]);
                mesh->addIndex(aiMesh->mFaces[f].mIndices[2]);
                mesh->addIndex(aiMesh->mFaces[f].mIndices[3]);
            }
        }
        mesh->endMesh();
        model.addMesh(mesh);
        model.createNode(mesh, nodeTransform);
    }

    for (int i=0; i<node->mNumChildren; i++)
        qtr3dAssimpNode(scene, node->mChildren[i], model, nodeTransform);
}
