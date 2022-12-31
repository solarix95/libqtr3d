#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "qtr3dassimploader.h"

//-------------------------------------------------------------------------------------------------
void qtr3dAssimpNode(const aiScene *ascene, aiNode *anode, Qtr3dModel &model, const QMatrix4x4 &parentTransform);
void qtr3dAssimpMesh(const aiScene *ascene, aiMesh *amesh, Qtr3dModel &model);
void qtr3dAssimpNode(const aiScene *ascene, aiNode *anode, Qtr3dModel &model,  Qtr3dModel::Node *parent = nullptr);

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
bool Qtr3dAssimpLoader::loadFile(Qtr3dModel &model, const QString &filename,Options /* opts */)
{
    auto *scene = aiImportFile(filename.toUtf8().constData(),aiProcessPreset_TargetRealtime_Fast); // aiProcessPreset_TargetRealtime_MaxQuality);
    if (!scene || (scene->mNumMeshes <= 0))
        return false;

    if (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
        aiReleaseImport(scene);
        return false;
    }

    if (scene->mFlags & AI_SCENE_FLAGS_VALIDATION_WARNING) {
        aiReleaseImport(scene);
        return false;
    }

    for (unsigned i=0; i<scene->mNumMeshes; i++)
        qtr3dAssimpMesh(scene,scene->mMeshes[i],model);

    qtr3dAssimpNode(scene, scene->mRootNode, model);

    qDebug() << "ASSIMP" << model.meshes().count() << model.nodes().count();
    aiReleaseImport(scene);
    return false;
}

//-------------------------------------------------------------------------------------------------
void qtr3dAssimpMesh(const aiScene *ascene, aiMesh *amesh, Qtr3dModel &model)
{
    Q_ASSERT(amesh);

    auto *mesh       = model.context()->createMesh(false)->startMesh(Qtr3d::Triangle);
    auto *amaterial  = amesh->mMaterialIndex >= 0 ? ascene->mMaterials[amesh->mMaterialIndex ] : nullptr;

    if (amaterial && amesh->mTextureCoords[0]) {
        aiString path;
        if (amaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {

            auto *textureData = ascene->GetEmbeddedTexture(path.data);
            if (textureData) {
                QImage img;
                /*
                      mHeight: If this value is zero, pcData points to an compressed texture in any format (e.g. JPEG).
                      mWidth:  If mHeight is zero the texture is compressed in a format like JPEG. In this case mWidth specifies the size of the memory area pcData is pointing to, in bytes.
                    */
                if (textureData->mHeight <= 0 && textureData->mWidth > 0) {
                    img = QImage::fromData((const uchar *)textureData->pcData, textureData->mWidth);

                    if (!img.isNull()) {
                        mesh->setTexture(img);
                    }
                }
            }
        }
    }

    if (amaterial) {
        aiColor3D color (0.f,0.f,0.f);
        if (amaterial->Get(AI_MATKEY_COLOR_DIFFUSE,color) == AI_SUCCESS) {
            mesh->setDefaultColor(QColor(color.r * 255, color.g * 255, color.b * 255));
        }
    }

    for (unsigned v=0; v<amesh->mNumVertices; v++) {
        if (mesh->hasTexture()) {
            mesh->addVertex(QVector3D(amesh->mVertices[v].x,
                                      amesh->mVertices[v].y,
                                      amesh->mVertices[v].z),
                            QVector3D(amesh->mNormals[v].x,
                                      amesh->mNormals[v].y,
                                      amesh->mNormals[v].z),
                            amesh->mTextureCoords[0][v].x,
                            amesh->mTextureCoords[0][v].y
                    );
        } else {
            mesh->addVertex(QVector3D(amesh->mVertices[v].x,
                                      amesh->mVertices[v].y,
                                      amesh->mVertices[v].z),
                            QVector3D(amesh->mNormals[v].x,
                                      amesh->mNormals[v].y,
                                      amesh->mNormals[v].z)
                            );
        }
    }

    for (unsigned f=0; f<amesh->mNumFaces; f++) {
        if (amesh->mFaces[f].mNumIndices < 3 || amesh->mFaces[f].mNumIndices > 4)
            continue;

        mesh->addIndex(amesh->mFaces[f].mIndices[0]);
        mesh->addIndex(amesh->mFaces[f].mIndices[1]);
        mesh->addIndex(amesh->mFaces[f].mIndices[2]);

        if (amesh->mFaces[f].mNumIndices == 4) {
            mesh->addIndex(amesh->mFaces[f].mIndices[0]);
            mesh->addIndex(amesh->mFaces[f].mIndices[2]);
            mesh->addIndex(amesh->mFaces[f].mIndices[3]);
        }
    }
    mesh->endMesh();
    model.addMesh(mesh);
}

//-------------------------------------------------------------------------------------------------
void qtr3dAssimpNode(const aiScene *ascene, aiNode *anode, Qtr3dModel &model,  Qtr3dModel::Node *parent)
{
    Q_ASSERT(anode);

    aiMatrix4x4 *t = &anode->mTransformation;

    QMatrix4x4 nodeTransform(
                t->a1, t->a2, t->a3, t->a4,
                t->b1, t->b2, t->b3, t->b4,
                t->c1, t->c2, t->c3, t->c4,
                t->d1, t->d2, t->d3, t->d4);

    Qtr3dMeshes meshes;
    const Qtr3dMeshes &modelMeshes = model.meshes();
    for (unsigned i=0; i<anode->mNumMeshes; i++)
        meshes << modelMeshes[anode->mMeshes[i]];

    auto *nextNode = model.createNode(meshes, nodeTransform, parent);

    for (unsigned i=0; i<anode->mNumChildren; i++)
        qtr3dAssimpNode(ascene, anode->mChildren[i], model, nextNode);
}
