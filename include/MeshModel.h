/*
 * MeshModel.h
 *
 *  Created on: Nov 29, 2009
 *      Author: giorgos
 */

#ifndef MESHMODEL_H_
#define MESHMODEL_H_

#include "Renderable.h"
#include "Mesh.h"

class MeshModel: public Renderable {

protected:
	MeshPtr     m_mesh;
	GLuint      m_vboID;
	GLuint      m_iboID;

public:
	MeshModel(Mesh* mesh);
	virtual ~MeshModel();

	Mesh* getMesh() {
		return m_mesh.get();
	}

	virtual MaterialPtr getMaterial() {
		return m_mesh->getMaterial();
	}

	virtual void renderGeometry();

private:
	void _updateVertexBuffer();
	void _updateIndicesBuffer();
	void _updateBounds();
};

#endif /* MESHMODEL_H_ */
