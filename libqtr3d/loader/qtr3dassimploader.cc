#include <QFile>
#include <QFileInfo>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "libqtr3d/qtr3dmodelanimation.h"
#include "qtr3dassimploader.h"

//-------------------------------------------------------------------------------------------------
void        qtr3dAssimpNode(const aiScene *ascene, aiNode *anode, Qtr3dModel &model, const QMatrix4x4 &parentTransform);
void        qtr3dAssimpMesh(const aiScene *ascene, aiMesh *amesh, Qtr3dModel &model);
void        qtr3dAssimpNode(const aiScene *ascene, aiNode *anode, Qtr3dModel &model,  Qtr3dModel::Node *parent = nullptr);
void        qtr3dAssimpAnimation(const aiScene *ascene, aiAnimation *aanim, Qtr3dModel &model);
QString     qtr3dString(const aiString &astring);
QMatrix4x4  qtr3dMatrix(const aiMatrix4x4 &amatrix);
QVector3D   qtr3dVector3D(const aiVector3D &avector);
QQuaternion qtr3dQuaternion(const aiQuaternion &aquaternion);

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
#if load_from_memory
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly))
        return false;
    QByteArray fileBuffer = f.readAll();
    if (fileBuffer.isEmpty())
        return false;

    QFileInfo fi(filename);
    auto *scene = aiImportFileFromMemory(fileBuffer.constData(),fileBuffer.length(),aiProcessPreset_TargetRealtime_MaxQuality,fi.completeSuffix().toUtf8().constData());
#else
    auto *scene = aiImportFile(filename.toUtf8().constData(),aiProcessPreset_TargetRealtime_Fast ); // aiProcessPreset_TargetRealtime_Fast); // aiProcessPreset_TargetRealtime_MaxQuality);
#endif
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

    for (unsigned i=0; i<scene->mNumAnimations; i++)
        qtr3dAssimpAnimation(scene,scene->mAnimations[i],model);

    qDebug() << "ASSIMP" << model.meshes().count() << model.nodes().mNodes.count();
    aiReleaseImport(scene);
    return model.meshes().count() > 0;
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
            if (amesh->mNormals)
                mesh->addVertex(QVector3D(amesh->mVertices[v].x,
                                          amesh->mVertices[v].y,
                                          amesh->mVertices[v].z),
                                QVector3D(amesh->mNormals[v].x,
                                          amesh->mNormals[v].y,
                                          amesh->mNormals[v].z)
                                );
            else
                mesh->addVertex(QVector3D(amesh->mVertices[v].x,
                                          amesh->mVertices[v].y,
                                          amesh->mVertices[v].z));
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

    for (unsigned b=0; b<amesh->mNumBones; b++) {
        auto       *abone = amesh->mBones[b];
        Qtr3dMesh::Bone bone;
        bone.name   = QString::fromUtf8(abone->mName.C_Str());
        bone.offset = qtr3dMatrix(abone->mOffsetMatrix);
        for (unsigned bw=0; bw<abone->mNumWeights; bw++) {
            bone.weights << Qtr3dMesh::BoneWeight(abone->mWeights[bw].mVertexId, abone->mWeights[bw].mWeight);
        }
        mesh->addBone(bone);
    }

    mesh->endMesh();
    model.addMesh(mesh);
}

//-------------------------------------------------------------------------------------------------
void qtr3dAssimpNode(const aiScene *ascene, aiNode *anode, Qtr3dModel &model,  Qtr3dModel::Node *parent)
{
    Q_ASSERT(anode);

    QMatrix4x4 nodeTransform = qtr3dMatrix(anode->mTransformation);

    Qtr3dMeshes meshes;
    const Qtr3dMeshes &modelMeshes = model.meshes();
    for (unsigned i=0; i<anode->mNumMeshes; i++)
        meshes << modelMeshes[anode->mMeshes[i]];

    auto *nextNode = model.createNode(meshes, nodeTransform, qtr3dString(anode->mName), parent);

    for (unsigned i=0; i<anode->mNumChildren; i++)
        qtr3dAssimpNode(ascene, anode->mChildren[i], model, nextNode);
}

//-------------------------------------------------------------------------------------------------
void qtr3dAssimpAnimation(const aiScene *ascene, aiAnimation *aanim, Qtr3dModel &model)
{
    Q_ASSERT(ascene);
    Q_ASSERT(aanim);
    if (aanim->mNumChannels <= 0) {
        qDebug() << "qtr3dAssimpAnimation: got invalid aiAnimation";
        return;
    }

    Qtr3dModelAnimation *anim = new Qtr3dModelAnimation(qtr3dString(aanim->mName),aanim->mDuration,aanim->mTicksPerSecond);

    for (unsigned ci=0; ci<aanim->mNumChannels; ci++) {
        auto *achannel = aanim->mChannels[ci];
        Qtr3dModelAnimation::Channel ch;
        ch.nodeName = qtr3dString(achannel->mNodeName);

        for (unsigned k=0; k<achannel->mNumPositionKeys; k++)
            ch.mPositionKeys << Qtr3dModelAnimation::PositionKey(achannel->mPositionKeys[k].mTime,qtr3dVector3D(achannel->mPositionKeys[k].mValue));
        for (unsigned k=0; k<achannel->mNumScalingKeys; k++)
            ch.mScaleKeys << Qtr3dModelAnimation::ScaleKey(achannel->mScalingKeys[k].mTime,qtr3dVector3D(achannel->mScalingKeys[k].mValue));
        for (unsigned k=0; k<achannel->mNumRotationKeys; k++)
            ch.mRotationKeys << Qtr3dModelAnimation::RotationKey(achannel->mRotationKeys[k].mTime,qtr3dQuaternion(achannel->mRotationKeys[k].mValue));

        anim->addChannel(ch);
    }

    model.addAnimation(anim);
}

//-------------------------------------------------------------------------------------------------
QString qtr3dString(const aiString &astring) {
    return QString::fromUtf8(astring.C_Str());
}

//-------------------------------------------------------------------------------------------------
QMatrix4x4 qtr3dMatrix(const aiMatrix4x4 &amatrix)
{
    return  QMatrix4x4(
                amatrix.a1, amatrix.a2, amatrix.a3, amatrix.a4,
                amatrix.b1, amatrix.b2, amatrix.b3, amatrix.b4,
                amatrix.c1, amatrix.c2, amatrix.c3, amatrix.c4,
                amatrix.d1, amatrix.d2, amatrix.d3, amatrix.d4);
}

//-------------------------------------------------------------------------------------------------
QVector3D  qtr3dVector3D(const aiVector3D &avector)
{
    return QVector3D(avector.x, avector.y,avector.z);
}

//-------------------------------------------------------------------------------------------------
QQuaternion qtr3dQuaternion(const aiQuaternion &aquaternion)
{
    return QQuaternion(aquaternion.w, aquaternion.x, aquaternion.y, aquaternion.z);
}
