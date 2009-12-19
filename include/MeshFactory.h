/*
 * MeshFactory.h
 *
 *  Created on: Nov 27, 2009
 *      Author: giorgos
 */

#ifndef MESHFACTORY_H_
#define MESHFACTORY_H_

#include "MaterialDB.h"
#include "ITextureManager.h"
#include "Mesh.h"

class aiMesh;
class aiMaterial;

class MeshFactory {

private:
	MaterialDB* m_matDB;
	ITextureManager* m_texMgr;

public:
	MeshFactory(MaterialDB* matDB, ITextureManager* texMgr);
	virtual ~MeshFactory();

	/**
	 * Creates a quad centered at the world origin with its normal facing the +Z direction.
	 * It extends from -1.0 to 1.0 in the X and Y directions. Vertex normals and uv coordinates are also generated.
	 * @return the quad mesh
	 */
	Mesh* createQuad();
	std::list<Mesh*> createFromFile(String filename);

private:
	VertexElementSemantic _vertexTexCoordSemanticFromTexCoordIndex(uint8_t texIndex);
	void _copyVertexPosToVertexElementArray(aiMesh* mesh, VertexElement* ve);
	void _copyVertexNormalsToVertexElementArray(aiMesh* mesh, VertexElement* ve);
	void _copyVertexTangentsToVertexElementArray(aiMesh* mesh, VertexElement* ve);
	void _copyVertexUVsToVertexElementArray(aiMesh* mesh, uint8_t uvSet, VertexElement* ve);
	void _copyVertexColorsToVertexElementArray(aiMesh* mesh, VertexElement* ve);
	Mesh* _readSingleMesh(aiMesh* importedMesh, std::vector<Material*> materials);
	Material* _readSingleMaterial(aiMaterial* mat);
};

#endif /* MESHFACTORY_H_ */
