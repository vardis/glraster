/*
 * MeshFactory.cpp
 *
 *  Created on: Nov 27, 2009
 *      Author: giorgos
 */

#include "GLTutor.h"
#include "Quaternion.h"
#include "VertexFormat.h"
#include "MeshFactory.h"

#include <assimp.hpp>      // C++ importer interface
#include <aiScene.h>       // Output data structure
#include <aiPostProcess.h> // Post processing flags
MeshFactory::MeshFactory(/*MaterialDB* matDB,*/ITextureManager* texMgr) :
	/*m_matDB(matDB),*/m_texMgr(texMgr) {

}

MeshFactory::~MeshFactory() {
}

Mesh* MeshFactory::createSphere(float radius, uint numSegments, uint numRings) {

	ulong numVertices = 2 + numSegments * (numRings - 1);
	std::cout << "numVertices " << numVertices << std::endl;

	VertexFormat* vf = VertexFormat::create(VF_V3_N3_T2);
	VertexAttribute* vaVertices = vf->getAttributeBySemantic(Vertex_Pos);
	vaVertices->m_vbo->allocate(numVertices);

	VertexAttribute* vaNormals = vf->getAttributeBySemantic(Vertex_Normal);
	vaNormals->m_vbo->allocate(numVertices);

	VertexAttribute* vaTexCoords = vf->getAttributeBySemantic(Vertex_TexCoord0);
	vaTexCoords->m_vbo->allocate(numVertices);

	float* vertices = reinterpret_cast<float*> (vaVertices->m_vbo->mapData());
	float* normals = reinterpret_cast<float*> (vaNormals->m_vbo->mapData());
	float* uvs = reinterpret_cast<float*> (vaTexCoords->m_vbo->mapData());

	// every vertex in an internal ring is shared by 6 triangles, the outmost rings have vertices
	// that are shared by 5 triangles and the top and bottom vertices are shared by as many triangles
	// as there are segments
	uint32_t numIndices = 6 * numSegments * (numRings - 1);
	std::cout << "numIndices " << numIndices << std::endl;

	IndexArrayPtr ib(new uint32_t[numIndices]);
	uint32_t* indices = ib.get();

	Vec3f tempNormal;
	float dTheta = M_PI * (1.0f / numRings);
	float dPhi = 2.0f * M_PI * (1.0f / numSegments);
	uint32_t currentVertexIdx = 0;

	long idxVe = 0, idxNo = 0, idxUV = 0, idxCnt = 0;

	// start from the bottom vertex and process vertices and triangles of each ring
	for (uint ring = 0; ring <= numRings; ring++) {
		float theta = ring * dTheta;
		for (uint segment = 0; segment < numSegments; segment++) {
			float phi = segment * dPhi;
			float sinTheta = sinf(theta);
			//std::cout << "phi: " << (180.0f / M_PI) * phi << std::endl;
//			std::cout << "theta: " << (180.0f / M_PI) * theta << std::endl;
			float x = radius * cosf(phi) * sinTheta;
			float y = radius * cosf(theta);
			float z = radius * sinf(phi) * sinTheta;
			if (fabs(x) < 1.0e-6)
				x = 0.0f;
			if (fabs(y) < 1.0e-6)
				y = 0.0f;
			if (fabs(z) < 1.0e-6)
				z = 0.0f;
			*vertices++ = x;
			*vertices++ = y;
			*vertices++ = z;
			idxVe += 3;

			tempNormal.set(x, y, z);
			tempNormal.normalize();
			*normals++ = tempNormal.x;
			*normals++ = tempNormal.y;
			*normals++ = tempNormal.z;
			idxNo += 3;

			*uvs++ = float(segment) / numSegments;
			*uvs++ = 1.0f - float(ring) / numRings;
			idxUV += 2;

			if (ring != 0 && ring != numRings) {

				if (ring != (numRings - 1)) {
					// fill in the indices of the triangle consisting of this vertex, the vertex above and to the right
					// of the currentthe vertex immediately above the current
					*indices++ = currentVertexIdx;
					if (segment == numSegments - 1) {
						*indices++ = ring * numSegments + 1;
					} else {
						*indices++ = currentVertexIdx + numSegments + 1;
					}
					*indices++ = currentVertexIdx + numSegments;
					idxCnt += 3;
				}

				// fill in the indices of the triangle consisting of this vertex, the vertex at the right of the current
				// and the vertex above and to the right of the current
				*indices++ = currentVertexIdx;
				if (segment == numSegments - 1) {
					*indices++ = currentVertexIdx - segment;
					*indices++ = ring * numSegments + 1;
				} else {
					*indices++ = currentVertexIdx + 1;
					*indices++ = clamp(currentVertexIdx + numSegments + 1, 0, numVertices - 1);
				}
				idxCnt += 3;
			}

			if (ring == 0) {
				uint rightAbove = currentVertexIdx + 1;
				for (uint i = 0, v = rightAbove; i < numSegments - 1; i++, v++) {
					*indices++ = currentVertexIdx;
					*indices++ = v + 1;
					*indices++ = v;
					idxCnt += 3;
				}
				*indices++ = currentVertexIdx;
				*indices++ = rightAbove;
				*indices++ = rightAbove + numSegments - 1;
				idxCnt += 3;
			}

			++currentVertexIdx;

			// first and last rings have a single vertex
			if (ring == 0 || ring == numRings) {
//				std::cout << "BREAK\n";
				break;
			}
		}
	}

//	std::cout << "indices copied " << idxCnt << std::endl;
//	std::cout << "vertices copied " << idxVe << std::endl;
//	std::cout << "normals copied " << idxNo << std::endl;
//	std::cout << "uvs copied " << idxUV << std::endl;

	vaVertices->m_vbo->unmapData();
	vaNormals->m_vbo->unmapData();
	vaTexCoords->m_vbo->unmapData();

	Mesh* sphere = new Mesh();
	sphere->getBounds().addPoint(radius, radius, radius);
	sphere->getBounds().addPoint(-radius, -radius, -radius);
	sphere->updateVertexData(vf, numVertices);
	sphere->updateIndexData(ib, numIndices);
	return sphere;
}

std::list<Mesh*> MeshFactory::createFromFile(String filename, bool genNormals, bool genUVs) {
	Assimp::Importer importer;
	std::list<Mesh*> meshes;

	int postProcessOpts = aiProcess_FixInfacingNormals | aiProcess_Triangulate | aiProcess_CalcTangentSpace;
	if (genNormals) {
		postProcessOpts |= aiProcess_GenSmoothNormals;
	}
	if (genUVs) {
		postProcessOpts |= aiProcess_GenUVCoords;
	}
	const aiScene* model = importer.ReadFile(filename, postProcessOpts);
	if (!model) {
		std::cerr << importer.GetErrorString() << std::endl;
	} else {
		std::vector<Material*> materials;
		for (uint32_t m = 0; m < model->mNumMaterials; m++) {
			Material* mat = _readSingleMaterial(model->mMaterials[m]);
			materials.push_back(mat);
			//			m_matDB->addMaterial(mat);
		}
		for (uint32_t i = 0; i < model->mNumMeshes; i++) {
			meshes.push_back(_readSingleMesh(model->mMeshes[i], materials));
		}
	}
	return meshes;
}

Mesh* MeshFactory::createQuad(const Vec3f& center, const Vec3f& facingDir, float xsize, float ysize) {
	/*
	 * V3 (-1, 1)              V2(1, 1)
	 * +-------------------------+
	 * |                         |
	 * |                         |
	 * |                         |
	 * |                         |
	 * |                         |
	 * |                         |
	 * +-------------------------+
	 * V0 (-1, -1)             V1(1, -1)
	 */

	//	uint16_t indices[] = { 0, 1, 2, 0, 2, 3 };

	VertexFormat* vf = new VertexFormat();

	// add vertex positions
	uint32_t offset = 4 * VertexAttribute::getFormatSize(VertexFormat_FLOAT3);

	VertexAttribute* ve = vf->addAttribute(Vertex_Pos, VertexFormat_FLOAT3, 0, 0);
	ve->m_vbo->allocate(4);
	float* fData = reinterpret_cast<float*> (ve->m_vbo->mapData());

	Vec3f lowLeft(-xsize, -ysize, 0.0f);
	Vec3f lowRight(xsize, -ysize, 0.0f);
	Vec3f upLeft(-xsize, ysize, 0.0f);
	Vec3f upRight(xsize, ysize, 0.0f);

	lowLeft += center;
	lowRight += center;
	upLeft += center;
	upRight += center;

	Quaternionf quat = Quaternionf::getRotationArc(Vec3f::Z_Axis, facingDir);
	Matrix4f m = quat.toMatrix();
	lowLeft = m * lowLeft;
	lowRight = m * lowRight;
	upLeft = m * upLeft;
	upRight = m * upRight;

	lowLeft.memCpyTo(fData);
	lowRight.memCpyTo(&fData[3]);
	upRight.memCpyTo(&fData[6]);
	upLeft.memCpyTo(&fData[9]);

	ve->m_vbo->unmapData();

	// add vertex normals
	Vec3f n(facingDir.x, facingDir.y, facingDir.z);
	n.normalize();

	ve = vf->addAttribute(Vertex_Normal, VertexFormat_FLOAT3, offset, 0);
	ve->m_vbo->allocate(4);
	fData = reinterpret_cast<float*> (ve->m_vbo->mapData());
	n.memCpyTo(fData, 4);

	ve->m_vbo->unmapData();
	offset += 4 * VertexAttribute::getFormatSize(VertexFormat_FLOAT3);

	// add vertex colors
	ve = vf->addAttribute(Vertex_Color, VertexFormat_FLOAT4, offset, 0);
	ve->m_vbo->allocate(4);
	fData = reinterpret_cast<float*> (ve->m_vbo->mapData());
	Colour::WHITE.memCpyTo(fData, 4);

	ve->m_vbo->unmapData();
	offset += 4 * VertexAttribute::getFormatSize(VertexFormat_FLOAT4);

	// add texture coordinates
	ve = vf->addAttribute(Vertex_TexCoord0, VertexFormat_FLOAT2, offset, 0);
	ve->m_vbo->allocate(4);
	fData = reinterpret_cast<float*> (ve->m_vbo->mapData());
	// V0
	fData[0] = 0.0f;
	fData[1] = 0.0f;

	// V1
	fData[2] = 1.0f;
	fData[3] = 0.0f;

	// V2
	fData[4] = 1.0f;
	fData[5] = 1.0f;

	// V3
	fData[6] = 0.0f;
	fData[7] = 1.0f;

	ve->m_vbo->unmapData();

	IndexArrayPtr indices(new uint32_t[6]);
	uint32_t* p = indices.get();

	// lower-right triangle
	*p++ = 0;
	*p++ = 1;
	*p++ = 2;

	// upper-left triangle
	*p++ = 0;
	*p++ = 2;
	*p++ = 3;

//	vf->printData();

	Mesh* mesh = new Mesh();
	mesh->updateVertexData(vf, 4);
	mesh->updateIndexData(indices, 6);
//	mesh->getIbo()->printData();
	return mesh;
}

VertexAttributeSemantic MeshFactory::_vertexTexCoordSemanticFromTexCoordIndex(uint8_t texIndex) {
	switch (texIndex) {
	case 0:
		return Vertex_TexCoord0;
	case 1:
		return Vertex_TexCoord1;
	case 2:
		return Vertex_TexCoord2;
	case 3:
		return Vertex_TexCoord3;
	case 4:
		return Vertex_TexCoord4;
	case 5:
		return Vertex_TexCoord5;
	case 6:
		return Vertex_TexCoord6;
	case 7:
		return Vertex_TexCoord7;
	default:
		return Vertex_TexCoord0;
	}
}

void MeshFactory::_copyVertexPosToVertexAttributeArray(aiMesh* mesh, VertexAttribute* ve) {
	float* fData;
	short* sData;
	uint8_t* bData;
	void* data = ve->m_vbo->mapData();
	size_t elementCount = VertexAttribute::getFormatElementCount(ve->m_format);
	for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
		fData = 0;
		sData = 0;
		bData = 0;
		switch (ve->m_format) {
		case VertexFormat_FLOAT1:
		case VertexFormat_FLOAT2:
			fData = reinterpret_cast<float*> (data);
			fData[i * elementCount] = mesh->mVertices[i].x;
			fData[i * elementCount + 1] = mesh->mVertices[i].y;
			break;
		case VertexFormat_FLOAT3:
			fData = reinterpret_cast<float*> (data);
			fData[i * elementCount] = mesh->mVertices[i].x;
			fData[i * elementCount + 1] = mesh->mVertices[i].y;
			fData[i * elementCount + 2] = mesh->mVertices[i].z;
			break;
		case VertexFormat_SHORT2:
			sData = reinterpret_cast<short*> (data);
			sData[i * elementCount] = mesh->mVertices[i].x;
			sData[i * elementCount + 1] = mesh->mVertices[i].y;
			break;
		case VertexFormat_SHORT3:
			sData = reinterpret_cast<short*> (data);
			sData[i * elementCount] = mesh->mVertices[i].x;
			sData[i * elementCount + 1] = mesh->mVertices[i].y;
			sData[i * elementCount + 2] = mesh->mVertices[i].z;
			break;
		default:
			break;
		}
	}
	ve->m_vbo->unmapData();
}

void MeshFactory::_copyVertexNormalsToVertexAttributeArray(aiMesh* mesh, VertexAttribute* ve) {
	float* fData;
	short* sData;
	void* data = ve->m_vbo->mapData();
	size_t elementCount = VertexAttribute::getFormatElementCount(ve->m_format);
	for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
		fData = 0;
		sData = 0;
		switch (ve->m_format) {
		case VertexFormat_FLOAT3:
			fData = reinterpret_cast<float*> (data);
			fData[i * elementCount] = mesh->mNormals[i].x;
			fData[i * elementCount + 1] = mesh->mNormals[i].y;
			fData[i * elementCount + 2] = mesh->mNormals[i].z;
			break;
		case VertexFormat_SHORT3:
			sData = reinterpret_cast<short*> (data);
			sData[i * elementCount] = mesh->mNormals[i].x;
			sData[i * elementCount + 1] = mesh->mNormals[i].y;
			sData[i * elementCount + 2] = mesh->mNormals[i].z;
			break;
		default:
			break;
		}
	}
	ve->m_vbo->unmapData();
}

void MeshFactory::_copyVertexBiNormalsToVertexAttributeArray(aiMesh* mesh, VertexAttribute* ve) {
	float* fData;
	short* sData;
	void* data = ve->m_vbo->mapData();
	size_t elementCount = VertexAttribute::getFormatElementCount(ve->m_format);
	for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
		fData = 0;
		sData = 0;
		switch (ve->m_format) {
		case VertexFormat_FLOAT3:
			fData = reinterpret_cast<float*> (data);
			fData[i * elementCount] = mesh->mBitangents[i].x;
			fData[i * elementCount + 1] = mesh->mBitangents[i].y;
			fData[i * elementCount + 2] = mesh->mBitangents[i].z;
			break;
		case VertexFormat_SHORT3:
			sData = reinterpret_cast<short*> (data);
			sData[i * elementCount] = mesh->mBitangents[i].x;
			sData[i * elementCount + 1] = mesh->mBitangents[i].y;
			sData[i * elementCount + 2] = mesh->mBitangents[i].z;
			break;
		default:
			break;
		}
	}
	ve->m_vbo->unmapData();
}

void MeshFactory::_copyVertexTangentsToVertexAttributeArray(aiMesh* mesh, VertexAttribute* ve) {
	float* fData;
	short* sData;
	void* data = ve->m_vbo->mapData();
	size_t elementCount = VertexAttribute::getFormatElementCount(ve->m_format);
	for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
		fData = 0;
		sData = 0;
		switch (ve->m_format) {
		case VertexFormat_FLOAT3:
			fData = reinterpret_cast<float*> (data);
			fData[i * elementCount] = mesh->mTangents[i].x;
			fData[i * elementCount + 1] = mesh->mTangents[i].y;
			fData[i * elementCount + 2] = mesh->mTangents[i].z;
			break;
		case VertexFormat_SHORT3:
			sData = reinterpret_cast<short*> (data);
			sData[i * elementCount] = mesh->mTangents[i].x;
			sData[i * elementCount + 1] = mesh->mTangents[i].y;
			sData[i * elementCount + 2] = mesh->mTangents[i].z;
			break;
		default:
			break;
		}
	}
	ve->m_vbo->unmapData();
}

void MeshFactory::_copyVertexUVsToVertexAttributeArray(aiMesh* mesh, uint8_t uvSet, VertexAttribute* ve) {
	float* fData;
	short* sData;
	void* data = ve->m_vbo->mapData();
	size_t elementCount = VertexAttribute::getFormatElementCount(ve->m_format);
	for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
		fData = 0;
		sData = 0;
		switch (ve->m_format) {
		case VertexFormat_FLOAT1:
			fData = reinterpret_cast<float*> (data);
			fData[i * elementCount] = mesh->mTextureCoords[uvSet][i].x;
			break;
		case VertexFormat_FLOAT2:
			fData = reinterpret_cast<float*> (data);
			fData[i * elementCount] = mesh->mTextureCoords[uvSet][i].x;
			fData[i * elementCount + 1] = mesh->mTextureCoords[uvSet][i].y;
			break;
		case VertexFormat_FLOAT3:
			fData = reinterpret_cast<float*> (data);
			fData[i * elementCount] = mesh->mTextureCoords[uvSet][i].x;
			fData[i * elementCount + 1] = mesh->mTextureCoords[uvSet][i].y;
			fData[i * elementCount + 2] = mesh->mTextureCoords[uvSet][i].z;
			break;
		case VertexFormat_SHORT1:
			sData = reinterpret_cast<short*> (data);
			sData[i * elementCount] = mesh->mTextureCoords[uvSet][i].x;
			break;
		case VertexFormat_SHORT2:
			sData = reinterpret_cast<short*> (data);
			sData[i * elementCount] = mesh->mTextureCoords[uvSet][i].x;
			sData[i * elementCount + 1] = mesh->mTextureCoords[uvSet][i].y;
			break;
		case VertexFormat_SHORT3:
			sData = reinterpret_cast<short*> (data);
			sData[i * elementCount] = mesh->mTextureCoords[uvSet][i].x;
			sData[i * elementCount + 1] = mesh->mTextureCoords[uvSet][i].y;
			sData[i * elementCount + 2] = mesh->mTextureCoords[uvSet][i].z;
			break;
		default:
			break;
		}
	}
	ve->m_vbo->unmapData();
}

void MeshFactory::_copyVertexColorsToVertexAttributeArray(aiMesh* mesh, VertexAttribute* ve) {
	float* fData;
	short* sData;
	uint8_t* bData;
	void* data = ve->m_vbo->mapData();
	size_t elementCount = VertexAttribute::getFormatElementCount(ve->m_format);
	for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
		fData = 0;
		sData = 0;
		bData = 0;
		switch (ve->m_format) {
		case VertexFormat_FLOAT3:
			fData = reinterpret_cast<float*> (data);
			fData[i * elementCount] = mesh->mColors[0][i].r;
			fData[i * elementCount + 1] = mesh->mColors[0][i].g;
			fData[i * elementCount + 2] = mesh->mColors[0][i].b;
			break;
		case VertexFormat_FLOAT4:
			fData = reinterpret_cast<float*> (data);
			fData[i * elementCount] = mesh->mColors[0][i].r;
			fData[i * elementCount + 1] = mesh->mColors[0][i].g;
			fData[i * elementCount + 2] = mesh->mColors[0][i].b;
			fData[i * elementCount + 3] = mesh->mColors[0][i].a;
			break;
		case VertexFormat_SHORT3:
			sData = reinterpret_cast<short*> (data);
			sData[i * elementCount] = mesh->mColors[0][i].r;
			sData[i * elementCount + 1] = mesh->mColors[0][i].g;
			sData[i * elementCount + 2] = mesh->mColors[0][i].b;
			break;
		case VertexFormat_SHORT4:
			sData = reinterpret_cast<short*> (data);
			sData[i * elementCount] = mesh->mColors[0][i].r;
			sData[i * elementCount + 1] = mesh->mColors[0][i].g;
			sData[i * elementCount + 2] = mesh->mColors[0][i].b;
			sData[i * elementCount + 3] = mesh->mColors[0][i].a;
			break;
		case VertexFormat_UBYTE3:
			bData = reinterpret_cast<uint8_t*> (data);
			bData[i * elementCount] = mesh->mColors[0][i].r;
			bData[i * elementCount + 1] = mesh->mColors[0][i].g;
			bData[i * elementCount + 2] = mesh->mColors[0][i].b;
			break;
		case VertexFormat_UBYTE4:
			bData = reinterpret_cast<uint8_t*> (data);
			bData[i * elementCount] = mesh->mColors[0][i].r;
			bData[i * elementCount + 1] = mesh->mColors[0][i].g;
			bData[i * elementCount + 2] = mesh->mColors[0][i].b;
			bData[i * elementCount + 3] = mesh->mColors[0][i].a;
			break;
		default:
			break;
		}
	}
	ve->m_vbo->unmapData();
}

Mesh* MeshFactory::_readSingleMesh(aiMesh* importedMesh, std::vector<Material*> materials) {

	if (!importedMesh->HasFaces()) {
		std::cerr << "The model has no faces!" << std::endl;
		return 0;
	}
	std::cout << "num vertices: " << importedMesh->mNumVertices << std::endl;

	// Vertex_Pos vertex element, mandatory
	uint32_t offset = 0;
	VertexAttributeFormat format = VertexFormat_FLOAT3;
	VertexFormat* vf = new VertexFormat();
	size_t formatSize = VertexAttribute::getFormatSize(format);

	//	void* posData = _allocateArrayForElement(importedMesh->mNumVertices, format);
	VertexAttribute* ve = vf->addAttribute(Vertex_Pos, format, 0, 0);
	ve->m_vbo->allocate(importedMesh->mNumVertices);
	offset += importedMesh->mNumVertices * formatSize;
	_copyVertexPosToVertexAttributeArray(importedMesh, ve);

	//TODO: ve->data can now be deallocated

	// Vertex_Normal vertex element, optional
	if (importedMesh->HasNormals()) {
		format = VertexFormat_FLOAT3;
		ve = vf->addAttribute(Vertex_Normal, format, offset, 0);
		//		ve->_allocateArrayForElement(importedMesh->mNumVertices);
		ve->m_vbo->allocate(importedMesh->mNumVertices);
		_copyVertexNormalsToVertexAttributeArray(importedMesh, ve);
		offset += importedMesh->mNumVertices * VertexAttribute::getFormatSize(format);
	}

	if (importedMesh->HasVertexColors(0)) {
		format = VertexFormat_FLOAT4;
		ve = vf->addAttribute(Vertex_Color, VertexFormat_FLOAT4, offset, 0);
		//		ve->_allocateArrayForElement(importedMesh->mNumVertices);
		ve->m_vbo->allocate(importedMesh->mNumVertices);
		_copyVertexColorsToVertexAttributeArray(importedMesh, ve);
		offset += importedMesh->mNumVertices * VertexAttribute::getFormatSize(format);
	}

	for (uint8_t t = 0; t < AI_MAX_NUMBER_OF_TEXTURECOORDS; t++) {
		if (importedMesh->HasTextureCoords(t)) {
			switch (importedMesh->mNumUVComponents[t]) {
			case 1:
				format = VertexFormat_FLOAT1;
				break;
			case 2:
				format = VertexFormat_FLOAT2;
				break;
			case 3:
				format = VertexFormat_FLOAT3;
				break;
			default:
				//TODO: error handling
				break;
			}
			ve = vf->addAttribute(_vertexTexCoordSemanticFromTexCoordIndex(t), format, offset, 0);
			//			ve->_allocateArrayForElement(importedMesh->mNumVertices);
			ve->m_vbo->allocate(importedMesh->mNumVertices);
			_copyVertexUVsToVertexAttributeArray(importedMesh, t, ve);
			offset += importedMesh->mNumVertices * VertexAttribute::getFormatSize(format);
		}
	}

	if (importedMesh->HasTangentsAndBitangents()) {
		std::cout << "Adding tangents and binormals\n";
		format = VertexFormat_FLOAT3;
		ve = vf->addAttribute(Vertex_Tangent, format, offset, 0);
		ve->m_vbo->allocate(importedMesh->mNumVertices);
		_copyVertexTangentsToVertexAttributeArray(importedMesh, ve);
		offset += importedMesh->mNumVertices * VertexAttribute::getFormatSize(format);

		ve = vf->addAttribute(Vertex_BiNormal, format, offset, 0);
		ve->m_vbo->allocate(importedMesh->mNumVertices);
		_copyVertexBiNormalsToVertexAttributeArray(importedMesh, ve);
		offset += importedMesh->mNumVertices * VertexAttribute::getFormatSize(format);
	} else {
		std::cout << "No mesh info for tangents and binormals\n";
	}

	uint32_t numIndices = 0;
	for (uint32_t f = 0; f < importedMesh->mNumFaces; f++) {
		if (importedMesh->mFaces[f].mNumIndices > 3) {
			std::cerr << "Warning: face  " << f << " has more than 3 vertices and will be skipped" << std::endl;
		} else {
			numIndices += importedMesh->mFaces[f].mNumIndices;
		}
	}
	std::cout << "num indices: " << numIndices << std::endl;

	IndexArrayPtr indices(new uint32_t[numIndices]);
	uint32_t* p = indices.get();
	for (uint32_t f = 0; f < importedMesh->mNumFaces; f++) {
		if (importedMesh->mFaces[f].mNumIndices == 3) {
			*p++ = importedMesh->mFaces[f].mIndices[0];
			*p++ = importedMesh->mFaces[f].mIndices[1];
			*p++ = importedMesh->mFaces[f].mIndices[2];
		}
	}
	Mesh* mesh = new Mesh();
	mesh->updateVertexData(vf, importedMesh->mNumVertices);
	mesh->updateIndexData(indices, numIndices);

	if (importedMesh->mMaterialIndex >= 0 && importedMesh->mMaterialIndex < materials.size()) {
		Material* m = materials[importedMesh->mMaterialIndex];
		std::cout << "setting material " << m->m_name << std::endl;
		mesh->setMaterial(MaterialPtr(materials[importedMesh->mMaterialIndex]));
	}

	//TODO: Stupid assimp assigns illogical uvIndex values (e.g. 32 instead of 0, for the 1st uvIndex)
	if (mesh->getMaterial()) {
		uint32_t uvIndices[AI_MAX_NUMBER_OF_TEXTURECOORDS];
		for (uint32_t uv = 0; uv < AI_MAX_NUMBER_OF_TEXTURECOORDS; uv++) {
			uvIndices[uv] = 0;
		}

		for (uint32_t uv = 0; uv < std::min(MAX_TEXTURES_STACK, AI_MAX_NUMBER_OF_TEXTURECOORDS); uv++) {
			uvIndices[uv] = mesh->getMaterial()->m_texStack->texInputs[uv].uvSet;
		}

		for (uint32_t input = 0; input < MAX_TEXTURES_STACK; input++) {
			for (uint32_t uv = 0; uv < AI_MAX_NUMBER_OF_TEXTURECOORDS; uv++) {
				if (mesh->getMaterial()->m_texStack->texInputs[input].uvSet == uvIndices[uv]) {
					//					std::cout << "adjusting input  " << input << " to uv set " << uv << std::endl;
					mesh->getMaterial()->m_texStack->texInputs[input].uvSet = uv;
					break;
				}
			}
		}
	}

	return mesh;
}

Material* MeshFactory::_readSingleMaterial(aiMaterial* mat) {

	aiString name;
	aiColor4D color;
	float fval;
	int ival;

	Material* meshModelMat = new Material();

	mat->Get(AI_MATKEY_NAME,name) ;
	meshModelMat->m_name.assign(&name.data[0], name.length);

	std::cout << "loading material " << &name.data[0] << std::endl;

	mat->Get(AI_MATKEY_COLOR_DIFFUSE,color) ;
	meshModelMat->m_diffuse.set(color.r, color.g, color.b, color.a);

	mat->Get(AI_MATKEY_COLOR_SPECULAR,color) ;
	meshModelMat->m_specular.set(color.r, color.g, color.b, color.a);

	mat->Get(AI_MATKEY_COLOR_EMISSIVE,color) ;
	meshModelMat->m_emissive.set(color.r, color.g, color.b, color.a);

	mat->Get(AI_MATKEY_OPACITY,fval) ;
	meshModelMat->m_opacity = fval;

	mat->Get(AI_MATKEY_SHININESS,fval) ;
	meshModelMat->m_shininess = fval;

	mat->Get(AI_MATKEY_TWOSIDED,ival) ;
	meshModelMat->m_twoSided = ival;

	TextureStack* texStack = new TextureStack();
	meshModelMat->m_texStack = TextureStackPtr(texStack);

	for (uint8_t t = 0; t < mat->GetTextureCount(aiTextureType_DIFFUSE); t++) {
		aiString texPath;
		aiTextureMapping texMapping;
		unsigned int uvIndex;
		float blend;
		aiTextureOp texOp;
		aiTextureMapMode texWrapMode;

		if (AI_SUCCESS == mat->GetTexture(aiTextureType_DIFFUSE, t, &texPath, &texMapping, &uvIndex, &blend, &texOp,
				&texWrapMode)) {
			std::cout << "loading texture " << &texPath.data[0] << std::endl;
			Texture* tex = new Texture();
			tex->m_filename = &texPath.data[0];
			tex->m_minFilter = TexFilter_Linear;
			tex->m_magFilter = TexFilter_Linear;
			tex->m_useMipmaps = true;

			switch (texMapping) {
			case aiTextureMapping_BOX:
				texStack->texInputs[0].mapping = TexMapInput_Cube;
				break;
			case aiTextureMapping_CYLINDER:
				texStack->texInputs[0].mapping = TexMapInput_Cylindrical;
				break;
			case aiTextureMapping_PLANE:
				texStack->texInputs[0].mapping = TexMapInput_Flat;
				break;
			case aiTextureMapping_SPHERE:
				texStack->texInputs[0].mapping = TexMapInput_Spherical;
				break;
			case aiTextureMapping_UV:
			default:
				texStack->texInputs[0].mapping = TexMapInput_UV;
			}

			switch (texOp) {
			case aiTextureOp_Add:
				texStack->texOutputs[0].blendOp = TexBlendOp_Add;
				break;
			case aiTextureOp_Subtract:
				texStack->texOutputs[0].blendOp = TexBlendOp_Sub;
				break;
			case aiTextureOp_Divide:
				texStack->texOutputs[0].blendOp = TexBlendOp_Div;
				break;
			case aiTextureOp_Multiply:
			default:
				texStack->texOutputs[0].blendOp = TexBlendOp_Multiply;
			}

			switch (texWrapMode) {
			case aiTextureMapMode_Clamp:
				tex->m_wrapping = TexWrapMode_Clamp;
				break;
			default:
				tex->m_wrapping = TexWrapMode_Repeat;
			}

			m_texMgr->loadTexture(tex->m_filename, tex);

			texStack->texInputs[t].uvSet = uvIndex;
			texStack->textures[t] = TexturePtr(tex);
			texStack->texOutputs[t].mapTo = TexMapTo_Diffuse;
			texStack->texOutputs[t].blendFactor = blend;
		}
	}
	return meshModelMat;
}
