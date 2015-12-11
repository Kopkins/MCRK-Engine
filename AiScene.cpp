#include <assimp/postprocess.h>

#include "AiScene.h"

AiScene::AiScene (const std::string& fileName)
{
  m_scene = m_importer.ReadFile (fileName,
		 aiProcess_Triangulate |
		 aiProcess_GenSmoothNormals |
		 aiProcess_GenUVCoords |
		 aiProcess_JoinIdenticalVertices);
  if (!m_scene)
  {
    auto error = m_importer.GetErrorString ();
    fprintf (stderr, "Failed to load model %s. Error: %s\n",
	     fileName.c_str (), error);
    exit (-1);
  }
}

AiScene::~AiScene ()
{

}

void
AiScene::readBufferData (unsigned meshNum, std::shared_ptr<VertexBuffer> vertices,
			 std::shared_ptr<IndexBuffer> indices) const
{
  const aiMesh* mesh = m_scene->mMeshes[meshNum];
  
  for (unsigned faceNum = 0; faceNum < mesh->mNumFaces; ++faceNum)
  {
    const aiFace& face = mesh->mFaces[faceNum];
    for (unsigned indexNum = 0; indexNum < 3; ++indexNum)
    {
      indices->add(face.mIndices[indexNum]);
    }
  }
  for (unsigned vertNum = 0; vertNum < mesh->mNumVertices; ++vertNum)
    {
      aiVector3D position = mesh->mVertices[vertNum];
      vertices->addFloat(position.x);
      vertices->addFloat(position.y);
      vertices->addFloat(position.z);

      aiVector3D normal = mesh->mNormals[vertNum];
      vertices->addFloat(normal.x);
      vertices->addFloat(normal.y);
      vertices->addFloat(normal.z);

      aiVector3D texture = mesh->mTextureCoords[0][vertNum];
      vertices->addFloat(texture.x);
      vertices->addFloat(texture.y);
    }
}

