#include <cstdio>
#include <cstdlib>

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
  // Nothing to do as Importer dtor cleans up
}

std::shared_ptr<VertexBuffer>
AiScene::readVertexData (unsigned meshNum) const
{
  // Get the correct mesh object
  const aiMesh* mesh = m_scene->mMeshes[meshNum];
  // Will store vertex data in some collection type
  std::shared_ptr<VertexBuffer> collection (new VertexBuffer);
  // Might want to set vertex size for "collection"

  
  // Go through array of faces
  for (unsigned faceNum = 0; faceNum < mesh->mNumFaces; ++faceNum)
  {
    // Get a face
    const aiFace& face = mesh->mFaces[faceNum];
    // Faces are triangles, so three indices
    const unsigned INDICES_PER_FACE = 3;
    // Go through the 3 indices
    for (unsigned indexNum = 0; indexNum < INDICES_PER_FACE; ++indexNum)
    {
      // Get position at location "indexNum"
      // Indexing is used so data isn't duplicated
      auto vertexNum = face.mIndices[indexNum];
      aiVector3D position = mesh->mVertices[vertexNum];
      // Put the position in collection
      collection->addFloat(position.x);
      collection->addFloat(position.y);
      collection->addFloat(position.z);
      aiVector3D normal = mesh->mNormals[vertexNum];
      // Put the normal in collection
      collection->addFloat(normal.x);
      collection->addFloat(normal.y);
      collection->addFloat(normal.z);
      // Do the same for other attributes if desired
      aiVector3D texture = mesh->mTextureCoords[0][vertexNum];
      collection->addFloat(texture.x);
      collection->addFloat(texture.y);


    }
  }
  return collection;
}

