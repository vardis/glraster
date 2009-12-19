/*
 * MaterialDB.cpp
 *
 *  Created on: Nov 29, 2009
 *      Author: giorgos
 */

#include "GLTutor.h"
#include "MaterialDB.h"

MaterialDB::MaterialDB() :
	m_materials() {

}

MaterialDB::~MaterialDB() {
}

void MaterialDB::clearMaterials() {
	m_materials.clear();
}

void MaterialDB::addMaterial(Material* mat) {
	mat->m_id = m_materials.size() + 1;	// +1 to ensure it is > 0
	m_materials.push_back(MaterialPtr(mat));
}

struct MaterialNamePredicate {
	MaterialNamePredicate(const String& name) :
		m_name(name) {
	}
	bool operator()(const MaterialPtr& mat) {
		return mat->m_name == m_name;
	}
	String m_name;
};

struct MaterialIdPredicate {
	MaterialIdPredicate(uint16_t id) :
		m_id(id) {
	}
	bool operator()(const MaterialPtr& mat) {
		return mat->m_id == m_id;
	}
	uint16_t m_id;
};

MaterialPtr MaterialDB::getMaterialByName(const String& name) {
	std::vector<MaterialPtr>::iterator it = std::find_if(m_materials.begin(), m_materials.end(), MaterialNamePredicate(name));
	return (it != m_materials.end()) ? *it : MaterialPtr();
}

MaterialPtr MaterialDB::getMaterialById(uint16_t id) {
	std::vector<MaterialPtr>::iterator it = std::find_if(m_materials.begin(), m_materials.end(), MaterialIdPredicate(id));
	return (it != m_materials.end()) ? *it : MaterialPtr();
}

void MaterialDB::removeMaterialByName(const String& name) {
	std::vector<MaterialPtr>::iterator it = std::find_if(m_materials.begin(), m_materials.end(), MaterialNamePredicate(name));
	if (it != m_materials.end()) {
		m_materials.erase(it);
	}
}

void MaterialDB::removeMaterialById(uint16_t id) {
	std::vector<MaterialPtr>::iterator it = std::find_if(m_materials.begin(), m_materials.end(), MaterialIdPredicate(id));
	if (it != m_materials.end()) {
		m_materials.erase(it);
	}
}
