#include <assimp/postprocess.h>

#include "AiScene.h"
#include "VertexBuffer.h"

AiScene::AiScene (const std::string& fileName)
{
  m_scene = m_importer.ReadFile (fileName,
		 aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_GenUVCoords);
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

std::shared_ptr<VertexBuffer>
AiScene::readVertexData (unsigned meshNum) const
{
  const aiMesh* mesh = m_scene->mMeshes[meshNum];
  std::shared_ptr<VertexBuffer> collection (new VertexBuffer);
  
  for (unsigned faceNum = 0; faceNum < mesh->mNumFaces; ++faceNum)
  {
    const aiFace& face = mesh->mFaces[faceNum];
    const unsigned INDICES_PER_FACE = 3;
    for (unsigned indexNum = 0; indexNum < INDICES_PER_FACE; ++indexNum)
    {
      auto vertexNum = face.mIndices[indexNum];

      aiVector3D position = mesh->mVertices[vertexNum];
      collection->addFloat(position.x);
      collection->addFloat(position.y);
      collection->addFloat(position.z);

      aiVector3D normal = mesh->mNormals[vertexNum];
      collection->addFloat(normal.x);
      collection->addFloat(normal.y);
      collection->addFloat(normal.z);

      aiVector3D texture = mesh->mTextureCoords[0][vertexNum];
      collection->addFloat(texture.x);
      collection->addFloat(texture.y);


    }
  }
  return collection;
}

