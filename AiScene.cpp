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
  std::vector<float> vertices;
  std::vector<int> indices;
  int lastIndex = 0;
  
  for (unsigned faceNum = 0; faceNum < mesh->mNumFaces; ++faceNum)
  {
    const aiFace& face = mesh->mFaces[faceNum];
    const unsigned INDICES_PER_FACE = 3;
    for (unsigned indexNum = 0; indexNum < INDICES_PER_FACE; ++indexNum)
    {
      auto vertexNum = face.mIndices[indexNum];

      aiVector3D position = mesh->mVertices[vertexNum];
      aiVector3D normal = mesh->mNormals[vertexNum];
      aiVector3D texture = mesh->mTextureCoords[0][vertexNum];
      float vertex [] = {
	position.x,
	position.y,
	position.z,
	normal.x,
	normal.y,
	normal.z,
	texture.x,
	texture.y};

      int index = -1;
      for (size_t i = 0; i < vertices.size(); ++i)
	{
	  if (vertex[0] == vertices[i])
	    {
	      for (int j = 1; j < 7; ++j)
		{
		  if (vertex[j] != vertices[i + j])
		    break;
		  index = i / 7;
		}
	    }
	  if (index != -1)
	    break;
	}
      if (index == -1) {
	for (auto f : vertex) {vertices.push_back(f);}
	indices.push_back(lastIndex);
	++lastIndex;
      }
      else {
	  indices.push_back(index);
      }

    }
  }
  return collection;
}

