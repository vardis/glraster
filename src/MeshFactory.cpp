/*
 * MeshFactory.cpp
 *
 *  Created on: Nov 27, 2009
 *      Author: giorgos
 */

#include "GLTutor.h"
#include "MeshFactory.h"

#include <assimp.hpp>      // C++ importer interface
#include <aiScene.h>       // Output data structure
#include <aiPostProcess.h> // Post processing flags

MeshFactory::MeshFactory(MaterialDB* matDB, ITextureManager* texMgr) :
	m_matDB(matDB), m_texMgr(texMgr) {

}

MeshFactory::~MeshFactory() {
}

void* _allocateArrayForElement(uint32_t count, VertexElementFormat format) {
	switch (format) {
	case VertexFormat_FLOAT1:
	case VertexFormat_FLOAT2:
	case VertexFormat_FLOAT3:
	case VertexFormat_FLOAT4:
		return new float[count * VertexElement::getFormatElementCount(format)];

	case VertexFormat_SHORT1:
	case VertexFormat_SHORT2:
	case VertexFormat_SHORT3:
	case VertexFormat_SHORT4:
		return new short[count * VertexElement::getFormatElementCount(format)];

	case VertexFormat_UBYTE3:
	case VertexFormat_UBYTE4:
		return new uint8_t[count * VertexElement::getFormatElementCount(format)];
	default:
		return 0;
	}
}

VertexElementSemantic MeshFactory::_vertexTexCoordSemanticFromTexCoordIndex(uint8_t texIndex) {
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

void MeshFactory::_copyVertexPosToVertexElementArray(aiMesh* mesh, VertexElement* ve) {
	float* fData;
	short* sData;
	uint8_t* bData;
	size_t elementCount = VertexElement::getFormatElementCount(ve->m_format);
	for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
		fData = 0;
		sData = 0;
		bData = 0;
		switch (ve->m_format) {
		case VertexFormat_FLOAT1:
		case VertexFormat_FLOAT2:
			fData = reinterpret_cast<float*> (ve->m_data);
			fData[i * elementCount] = mesh->mVertices[i].x;
			fData[i * elementCount + 1] = mesh->mVertices[i].y;
			break;
		case VertexFormat_FLOAT3:
			fData = reinterpret_cast<float*> (ve->m_data);
			fData[i * elementCount] = mesh->mVertices[i].x;
			fData[i * elementCount + 1] = mesh->mVertices[i].y;
			fData[i * elementCount + 2] = mesh->mVertices[i].z;
			break;
		case VertexFormat_SHORT2:
			sData = reinterpret_cast<short*> (ve->m_data);
			sData[i * elementCount] = mesh->mVertices[i].x;
			sData[i * elementCount + 1] = mesh->mVertices[i].y;
			break;
		case VertexFormat_SHORT3:
			sData = reinterpret_cast<short*> (ve->m_data);
			sData[i * elementCount] = mesh->mVertices[i].x;
			sData[i * elementCount + 1] = mesh->mVertices[i].y;
			sData[i * elementCount + 2] = mesh->mVertices[i].z;
			break;
		default:
			break;
		}
	}
}

void MeshFactory::_copyVertexNormalsToVertexElementArray(aiMesh* mesh, VertexElement* ve) {
	float* fData;
	short* sData;
	size_t elementCount = VertexElement::getFormatElementCount(ve->m_format);
	for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
		fData = 0;
		sData = 0;
		switch (ve->m_format) {
		case VertexFormat_FLOAT3:
			fData = reinterpret_cast<float*> (ve->m_data);
			fData[i * elementCount] = mesh->mNormals[i].x;
			fData[i * elementCount + 1] = mesh->mNormals[i].y;
			fData[i * elementCount + 2] = mesh->mNormals[i].z;
			break;
		case VertexFormat_SHORT3:
			sData = reinterpret_cast<short*> (ve->m_data);
			sData[i * elementCount] = mesh->mNormals[i].x;
			sData[i * elementCount + 1] = mesh->mNormals[i].y;
			sData[i * elementCount + 2] = mesh->mNormals[i].z;
			break;
		default:
			break;
		}
	}
}

void MeshFactory::_copyVertexTangentsToVertexElementArray(aiMesh* mesh, VertexElement* ve) {
	float* fData;
	short* sData;
	size_t elementCount = VertexElement::getFormatElementCount(ve->m_format);
	for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
		fData = 0;
		sData = 0;
		switch (ve->m_format) {
		case VertexFormat_FLOAT3:
			fData = reinterpret_cast<float*> (ve->m_data);
			fData[i * elementCount] = mesh->mTangents[i].x;
			fData[i * elementCount + 1] = mesh->mTangents[i].y;
			fData[i * elementCount + 2] = mesh->mTangents[i].z;
			break;
		case VertexFormat_SHORT3:
			sData = reinterpret_cast<short*> (ve->m_data);
			sData[i * elementCount] = mesh->mTangents[i].x;
			sData[i * elementCount + 1] = mesh->mTangents[i].y;
			sData[i * elementCount + 2] = mesh->mTangents[i].z;
			break;
		default:
			break;
		}
	}
}

void MeshFactory::_copyVertexUVsToVertexElementArray(aiMesh* mesh, uint8_t uvSet, VertexElement* ve) {
	float* fData;
	short* sData;
	size_t elementCount = VertexElement::getFormatElementCount(ve->m_format);
	for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
		fData = 0;
		sData = 0;
		switch (ve->m_format) {
		case VertexFormat_FLOAT1:
			fData = reinterpret_cast<float*> (ve->m_data);
			fData[i * elementCount] = mesh->mTextureCoords[uvSet][i].x;
			break;
		case VertexFormat_FLOAT2:
			fData = reinterpret_cast<float*> (ve->m_data);
			fData[i * elementCount] = mesh->mTextureCoords[uvSet][i].x;
			fData[i * elementCount + 1] = mesh->mTextureCoords[uvSet][i].y;
			std::cout << "u,v = " << fData[i * elementCount] << ", " << fData[i * elementCount + 1] << std::endl;
			break;
		case VertexFormat_FLOAT3:
			fData = reinterpret_cast<float*> (ve->m_data);
			fData[i * elementCount] = mesh->mTextureCoords[uvSet][i].x;
			fData[i * elementCount + 1] = mesh->mTextureCoords[uvSet][i].y;
			fData[i * elementCount + 2] = mesh->mTextureCoords[uvSet][i].z;
			break;
		case VertexFormat_SHORT1:
			sData = reinterpret_cast<short*> (ve->m_data);
			sData[i * elementCount] = mesh->mTextureCoords[uvSet][i].x;
			break;
		case VertexFormat_SHORT2:
			sData = reinterpret_cast<short*> (ve->m_data);
			sData[i * elementCount] = mesh->mTextureCoords[uvSet][i].x;
			sData[i * elementCount + 1] = mesh->mTextureCoords[uvSet][i].y;
			break;
		case VertexFormat_SHORT3:
			sData = reinterpret_cast<short*> (ve->m_data);
			sData[i * elementCount] = mesh->mTextureCoords[uvSet][i].x;
			sData[i * elementCount + 1] = mesh->mTextureCoords[uvSet][i].y;
			sData[i * elementCount + 2] = mesh->mTextureCoords[uvSet][i].z;
			break;
		default:
			break;
		}
	}
}

void MeshFactory::_copyVertexColorsToVertexElementArray(aiMesh* mesh, VertexElement* ve) {
	float* fData;
	short* sData;
	uint8_t* bData;
	size_t elementCount = VertexElement::getFormatElementCount(ve->m_format);
	for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
		fData = 0;
		sData = 0;
		bData = 0;
		switch (ve->m_format) {
		case VertexFormat_FLOAT3:
			fData = reinterpret_cast<float*> (ve->m_data);
			fData[i * elementCount] = mesh->mColors[0][i].r;
			fData[i * elementCount + 1] = mesh->mColors[0][i].g;
			fData[i * elementCount + 2] = mesh->mColors[0][i].b;
			break;
		case VertexFormat_FLOAT4:
			fData = reinterpret_cast<float*> (ve->m_data);
			fData[i * elementCount] = mesh->mColors[0][i].r;
			fData[i * elementCount + 1] = mesh->mColors[0][i].g;
			fData[i * elementCount + 2] = mesh->mColors[0][i].b;
			fData[i * elementCount + 3] = mesh->mColors[0][i].a;
			break;
		case VertexFormat_SHORT3:
			sData = reinterpret_cast<short*> (ve->m_data);
			sData[i * elementCount] = mesh->mColors[0][i].r;
			sData[i * elementCount + 1] = mesh->mColors[0][i].g;
			sData[i * elementCount + 2] = mesh->mColors[0][i].b;
			break;
		case VertexFormat_SHORT4:
			sData = reinterpret_cast<short*> (ve->m_data);
			sData[i * elementCount] = mesh->mColors[0][i].r;
			sData[i * elementCount + 1] = mesh->mColors[0][i].g;
			sData[i * elementCount + 2] = mesh->mColors[0][i].b;
			sData[i * elementCount + 3] = mesh->mColors[0][i].a;
			break;
		case VertexFormat_UBYTE3:
			bData = reinterpret_cast<uint8_t*> (ve->m_data);
			bData[i * elementCount] = mesh->mColors[0][i].r;
			bData[i * elementCount + 1] = mesh->mColors[0][i].g;
			bData[i * elementCount + 2] = mesh->mColors[0][i].b;
			break;
		case VertexFormat_UBYTE4:
			bData = reinterpret_cast<uint8_t*> (ve->m_data);
			bData[i * elementCount] = mesh->mColors[0][i].r;
			bData[i * elementCount + 1] = mesh->mColors[0][i].g;
			bData[i * elementCount + 2] = mesh->mColors[0][i].b;
			bData[i * elementCount + 3] = mesh->mColors[0][i].a;
			break;
		default:
			break;
		}
	}
}

Mesh* MeshFactory::_readSingleMesh(aiMesh* importedMesh, std::vector<Material*> materials) {

	if (!importedMesh->HasFaces()) {
		std::cerr << "The model has no faces!" << std::endl;
		return 0;
	}
	std::cout << "num vertices: " << importedMesh->mNumVertices << std::endl;

	// Vertex_Pos vertex element, mandatory
	uint32_t offset = 0;
	VertexElementFormat format = VertexFormat_FLOAT3;
	VertexFormat* vf = new VertexFormat();
	size_t formatSize = VertexElement::getFormatSize(format);

	void* posData = _allocateArrayForElement(importedMesh->mNumVertices, format);
	VertexElement* ve = vf->addElement(Vertex_Pos, format, 0, posData);
	offset += importedMesh->mNumVertices * formatSize;
	_copyVertexPosToVertexElementArray(importedMesh, ve);
	ve->m_vbo->setData(ve->m_data, importedMesh->mNumVertices);

	//TODO: ve->data can now be deallocated

	// Vertex_Normal vertex element, optional
	if (importedMesh->HasNormals()) {
		format = VertexFormat_FLOAT3;
		void* normalArray = _allocateArrayForElement(importedMesh->mNumVertices, format);
		ve = vf->addElement(Vertex_Normal, format, offset, normalArray);
		_copyVertexNormalsToVertexElementArray(importedMesh, ve);
		offset += importedMesh->mNumVertices * VertexElement::getFormatSize(format);
		ve->m_vbo->setData(ve->m_data, importedMesh->mNumVertices);
	}

	if (importedMesh->HasVertexColors(0)) {
		format = VertexFormat_FLOAT4;
		void* colorArray = _allocateArrayForElement(importedMesh->mNumVertices, format);
		ve = vf->addElement(Vertex_Color, VertexFormat_FLOAT4, offset, colorArray);
		_copyVertexColorsToVertexElementArray(importedMesh, ve);
		offset += importedMesh->mNumVertices * VertexElement::getFormatSize(format);
		ve->m_vbo->setData(ve->m_data, importedMesh->mNumVertices);
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
			void* uvArray = _allocateArrayForElement(importedMesh->mNumVertices, format);
			ve = vf->addElement(_vertexTexCoordSemanticFromTexCoordIndex(t), format, offset, uvArray);
			_copyVertexUVsToVertexElementArray(importedMesh, t, ve);
			offset += importedMesh->mNumVertices * VertexElement::getFormatSize(format);
			ve->m_vbo->setData(ve->m_data, importedMesh->mNumVertices);
		}
	}

	if (importedMesh->HasTangentsAndBitangents()) {
		format = VertexFormat_FLOAT3;
		void* tangetsArray = _allocateArrayForElement(importedMesh->mNumVertices, format);
		ve = vf->addElement(Vertex_Tangent, format, offset, tangetsArray);
		_copyVertexTangentsToVertexElementArray(importedMesh, ve);
		offset += importedMesh->mNumVertices * VertexElement::getFormatSize(format);
		ve->m_vbo->setData(ve->m_data, importedMesh->mNumVertices);

		//		void* bitangetsArray = _allocateArrayForElement(mesh->mNumVertices, VertexFormat_FLOAT3);
		//		vf->addElement(Vertex_Tangent, VertexFormat_FLOAT3, offset, bitangetsArray);
		//		_copyVertexBiNormalssToVertexElementArray(mesh, ve);
		//		offset += mesh->mNumVertices * VertexElement::getFormatSize(VertexFormat_FLOAT3);
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
			uvIndices[uv] = mesh->getMaterial()->m_textures->texInputs[uv].uvSet;
		}

		for (uint32_t input = 0; input < MAX_TEXTURES_STACK; input++) {
			for (uint32_t uv = 0; uv < AI_MAX_NUMBER_OF_TEXTURECOORDS; uv++) {
				if (mesh->getMaterial()->m_textures->texInputs[input].uvSet == uvIndices[uv]) {
//					std::cout << "adjusting input  " << input << " to uv set " << uv << std::endl;
					mesh->getMaterial()->m_textures->texInputs[input].uvSet = uv;
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
	meshModelMat->m_textures = TextureStackPtr(texStack);

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

std::list<Mesh*> MeshFactory::createFromFile(String filename) {
	Assimp::Importer importer;
	std::list<Mesh*> meshes;
	const aiScene* model = importer.ReadFile(filename, aiProcess_FixInfacingNormals | aiProcess_Triangulate
			| aiProcess_CalcTangentSpace);
	if (!model) {
		std::cerr << importer.GetErrorString() << std::endl;
	} else {
		std::vector<Material*> materials;
		for (uint32_t m = 0; m < model->mNumMaterials; m++) {
			Material* mat = _readSingleMaterial(model->mMaterials[m]);
			materials.push_back(mat);
			m_matDB->addMaterial(mat);
		}
		for (uint32_t i = 0; i < model->mNumMeshes; i++) {
			meshes.push_back(_readSingleMesh(model->mMeshes[i], materials));
		}
	}
	return meshes;
}

Mesh* MeshFactory::createQuad() {
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
	uint32_t offset = 4 * VertexElement::getFormatSize(VertexFormat_FLOAT3);

	void* posData = _allocateArrayForElement(4, VertexFormat_FLOAT3);
	VertexElement* ve = vf->addElement(Vertex_Pos, VertexFormat_FLOAT3, 0, posData);
	float* fData = reinterpret_cast<float*> (ve->m_data);
	// V0
	fData[0] = -1.0f;
	fData[1] = -1.0f;
	fData[2] = 0.0f;

	// V1
	fData[3] = 1.0f;
	fData[4] = -1.0f;
	fData[5] = 0.0f;

	// V2
	fData[6] = 1.0f;
	fData[7] = 1.0f;
	fData[8] = 0.0f;

	// V3
	fData[9] = -1.0f;
	fData[10] = 1.0f;
	fData[11] = 0.0f;
	ve->m_vbo->setData(ve->m_data, 4);

	// add vertex normals
	void* normalArray = _allocateArrayForElement(4, VertexFormat_FLOAT3);
	ve = vf->addElement(Vertex_Normal, VertexFormat_FLOAT3, offset, normalArray);
	fData = reinterpret_cast<float*> (ve->m_data);
	// V0
	fData[0] = 0.0f;
	fData[1] = 0.0f;
	fData[2] = 1.0f;

	// V1
	fData[3] = 0.0f;
	fData[4] = 0.0f;
	fData[5] = 1.0f;

	// V2
	fData[6] = 0.0f;
	fData[7] = 0.0f;
	fData[8] = 1.0f;

	// V3
	fData[9] = 0.0f;
	fData[10] = 0.0f;
	fData[11] = 1.0f;

	ve->m_vbo->setData(ve->m_data, 4);
	offset += 4 * VertexElement::getFormatSize(VertexFormat_FLOAT3);

	// add vertex colors
	void* colorArray = _allocateArrayForElement(4, VertexFormat_FLOAT4);
	ve = vf->addElement(Vertex_Color, VertexFormat_FLOAT4, offset, colorArray);
	fData = reinterpret_cast<float*> (ve->m_data);

	// V0
	fData[0] = 1.0f;
	fData[1] = 1.0f;
	fData[2] = 1.0f;
	fData[3] = 1.0f;

	// V1
	fData[4] = 1.0f;
	fData[5] = 1.0f;
	fData[6] = 1.0f;
	fData[7] = 1.0f;

	// V2
	fData[8] = 1.0f;
	fData[9] = 1.0f;
	fData[10] = 1.0f;
	fData[11] = 1.0f;

	// V3
	fData[12] = 1.0f;
	fData[13] = 1.0f;
	fData[14] = 1.0f;
	fData[15] = 1.0f;

	ve->m_vbo->setData(ve->m_data, 4);
	offset += 4 * VertexElement::getFormatSize(VertexFormat_FLOAT4);

	// add texture coordinates
	void* uvArray = _allocateArrayForElement(4, VertexFormat_FLOAT2);
	ve = vf->addElement(Vertex_TexCoord0, VertexFormat_FLOAT2, offset, uvArray);
	fData = reinterpret_cast<float*> (ve->m_data);
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

	ve->m_vbo->setData(ve->m_data, 4);

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

	Mesh* mesh = new Mesh();
	mesh->updateVertexData(vf, 4);
	mesh->updateIndexData(indices, 6);
	return mesh;
}

