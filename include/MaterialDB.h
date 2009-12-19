/*
 * MaterialDB.h
 *
 *  Created on: Nov 29, 2009
 *      Author: giorgos
 */

#ifndef MATERIALDB_H_
#define MATERIALDB_H_

#include "Material.h"

class MaterialDB {
public:
	MaterialDB();
	virtual ~MaterialDB();

	void clearMaterials();
	void addMaterial(Material* mat);

	MaterialPtr getMaterialByName(const String& name);
	MaterialPtr getMaterialById(uint16_t id);

	void removeMaterialByName(const String& name);
	void removeMaterialById(uint16_t id);

	uint32_t getNumMaterials() const {
		return m_materials.size();
	}

private:
	std::vector<MaterialPtr> m_materials;
};

#endif /* MATERIALDB_H_ */
