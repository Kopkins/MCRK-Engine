#ifndef AISCENE_H
#define AISCENE_H

#include <string>
#include <memory>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include "VertexBuffer.h"
#include "IndexBuffer.h"

// Make sure you add "-lassimp" to your Makefile!


class AiScene
{
public:
  AiScene (const std::string& fileName);

  ~AiScene ();

  void
  readBufferData (unsigned meshNum, std::shared_ptr<VertexBuffer> vertices,
			 std::shared_ptr<IndexBuffer> indices) const;

private:
  // Importer dtor destroys the scene!
  Assimp::Importer m_importer;
  // Maintain this, o/w we have to call GetScene which releases
  //   the Importer's ownership of the Scene and we become
  //   responsible for its destruction.
  const aiScene* m_scene;
};

#endif
