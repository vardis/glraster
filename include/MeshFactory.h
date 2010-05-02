/*
 * MeshFactory.h
 *
 *  Created on: Nov 27, 2009
 *      Author: giorgos
 */

#ifndef MESHFACTORY_H_
#define MESHFACTORY_H_

//#include "MaterialDB.h"
#include "ITextureManager.h"
#include "Mesh.h"

class aiMesh;
class aiMaterial;

class MeshFactory {

private:
//	MaterialDB* m_matDB;
	ITextureManager* m_texMgr;

public:
	MeshFactory(/*MaterialDB* matDB,*/ ITextureManager* texMgr);
	virtual ~MeshFactory();

	/**
	 * Creates a quad centered at the world origin with its normal facing the +Z direction.
	 * It extends from -1.0 to 1.0 in the X and Y directions. Vertex normals and uv coordinates are also generated.
	 * @return the quad mesh
	 */
	Mesh* createQuad(const Vec3f& center = Vec3f::Zero, const Vec3f& facingDir = Vec3f::Z_Axis, float xsize = 2.0f, float ysize = 2.0f);

	/**
	 * Creates a sphere of the given radius and using numSegments subdivisions for the azimuth
	 * and numRings subdivisions for the elevation. The center of the sphere is at (0, 0, 0).
	 *
	 * @param radius the radius of the sphere
	 * @param numSegments number of subdivisions for the azimuth angle
	 * @param numRings number of subdivisions for the elevation angle
	 * @return
	 */
	Mesh* createSphere(float radius = 1.0f, uint numSegments = 32, uint numRings = 16);

	std::list<Mesh*> createFromFile(String filename, bool genNormals = false, bool genUVs = false);

private:
	VertexAttributeSemantic _vertexTexCoordSemanticFromTexCoordIndex(uint8_t texIndex);
	void _copyVertexPosToVertexAttributeArray(aiMesh* mesh, VertexAttribute* ve);
	void _copyVertexNormalsToVertexAttributeArray(aiMesh* mesh, VertexAttribute* ve);
	void _copyVertexTangentsToVertexAttributeArray(aiMesh* mesh, VertexAttribute* ve);
	void _copyVertexUVsToVertexAttributeArray(aiMesh* mesh, uint8_t uvSet, VertexAttribute* ve);
	void _copyVertexColorsToVertexAttributeArray(aiMesh* mesh, VertexAttribute* ve);
	Mesh* _readSingleMesh(aiMesh* importedMesh, std::vector<Material*> materials);
	Material* _readSingleMaterial(aiMaterial* mat);
};

#endif /* MESHFACTORY_H_ */
