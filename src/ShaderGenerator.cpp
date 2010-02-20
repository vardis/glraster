/*
 * ShaderGenerator.cpp
 *
 *  Created on: Jan 30, 2010
 *      Author: giorgos
 */

#include "GLTutor.h"
#include "ShaderGenerator.h"

#include <ctemplate/template.h>

ShaderGenerator::ShaderGenerator() {
	// TODO Auto-generated constructor stub

}

ShaderGenerator::~ShaderGenerator() {
	// TODO Auto-generated destructor stub
}

void ShaderGenerator::generateShaders(const Material& mat, const VertexFormat& vf) {

	ctemplate::TemplateDictionary vertexShaderDict("vertexShader");
	ctemplate::TemplateDictionary fragmentShaderDict("fragmentShader");

	ctemplate::TemplateDictionary* vertexIoDict = vertexShaderDict.AddIncludeDictionary("VERTEX_INPUTS");
	vertexIoDict->SetFilename("shader_templates/ft_vertex_inout.tpl");

	ctemplate::TemplateDictionary* fragmentIoDict = vertexShaderDict.AddIncludeDictionary("FRAGMENT_INPUTS");
	fragmentIoDict->SetFilename("shader_templates/ft_fragment_inout.tpl");

	ctemplate::TemplateDictionary* uniformsDict = vertexShaderDict.AddIncludeDictionary("UNIFORMS");
	uniformsDict->SetFilename("shader_templates/uniforms.tpl");

	if (vf.getAttributeBySemantic(Vertex_Normal)) {
		vertexShaderDict.ShowSection("HAS_NORMALS");
		vertexShaderDict.ShowSection("NORMALIZE_NORMALS");

		vertexIoDict->ShowSection("HAS_NORMALS");
		vertexIoDict->ShowSection("NORMALIZE_NORMALS");

		fragmentIoDict->ShowSection("HAS_NORMALS");

		uniformsDict->ShowSection("HAS_NORMALS");
	}

	if (vf.getAttributeBySemantic(Vertex_Color)) {
		vertexShaderDict.ShowSection("HAS_COLORS");
		vertexIoDict->ShowSection("HAS_COLORS");
		fragmentIoDict->ShowSection("HAS_COLORS");
	}

	// indicates if the tex coords generation functions declarations template has already been
	// included in the vertex shader template
	bool texGenDeclIncluded = false;

	// number of active texture units
	uint activeTextures = 0;

	ctemplate::TemplateDictionary* texGenDict = vertexShaderDict.AddIncludeDictionary("TEX_COORDS_GEN");
	texGenDict->SetFilename("shader_templates/ft_tex_coords_gen.tpl");

	// Loops over all material textures and performs the following:
	//	1. Declares a respective uniform sampler object which will be named as u_sampler#,
	//     where # is the active texture index.
	//  2. Emits code for any texture coordinates generation scheme, if not simple UV.
	for (int i = 0; i < MAX_TEXTURES_STACK; i++) {
		TexturePtr tex = mat.m_texStack->textures[i];
		if (tex) {
			// for the fragment shader, here we can fill in the dictionary for the texture application section
			// that takes into account the texture's mapTo, environment color, uvset, etc.
			ctemplate::TemplateDictionary* texUnitDict = fragmentShaderDict.AddIncludeDictionary(
					"SINGLE_TEXTURE_STACK_ENTRY");
			texUnitDict->SetFilename("shader_templates/ft_tex_stack_application.tpl");

			// depending on the texture's mapTo, we will emit different sections in the template
			ctemplate::TemplateDictionary* texMapToDict;
			switch (mat.m_texStack->texOutputs[i].mapTo) {
			case TexMapTo_Diffuse:
				texMapToDict = texUnitDict->AddSectionDictionary("TEX_MAP_TO_DIFFUSE");
				break;
			case TexMapTo_CSpecular:
				texMapToDict = texUnitDict->AddSectionDictionary("TEX_MAP_TO_SPECULAR");
				break;
			case TexMapTo_Shininess:
				texMapToDict = texUnitDict->AddSectionDictionary("TEX_MAP_TO_SHININESS");
				break;
			default:
				SAFE_THROW(GLException(E_NOTIMPL, "Unimplemented texture output mapping mode"))
			}

			texUnitDict->SetIntValue("TEX_INDEX", activeTextures);
			texUnitDict->SetIntValue("UV_SET_INDEX", mat.m_texStack->texInputs[i].uvSet);

			switch (mat.m_texStack->texOutputs[i].blendOp) {
			case TexBlendOp_Mix:
				texUnitDict->ShowSection("TEX_BLENDOP_BLEND");
				break;
			case TexBlendOp_Add:
				texUnitDict->ShowSection("TEX_BLENDOP_ADD");
				break;
			case TexBlendOp_Multiply:
				texUnitDict->ShowSection("TEX_BLENDOP_MULTIPLY");
				break;
			case TexBlendOp_Decal:
				texUnitDict->ShowSection("TEX_BLENDOP_DECAL");
				break;
			default:
				SAFE_THROW(GLException(E_NOTIMPL, "Unimplemented texture blending mode"))
			}

			// this is the dictionary for a single uniform sampler declaration
			ctemplate::TemplateDictionary* samplerDict = uniformsDict->AddSectionDictionary("SINGLE_SAMPLER_DECL");
			samplerDict->SetIntValue("TEX_INDEX", activeTextures);

			switch (tex->getTextureTarget()) {
			case GL_TEXTURE_1D:
				samplerDict->SetValue("SAMPLER_SPEC", "sampler1D");
				texUnitDict->SetValue("TEX_COORDS", "s");
				break;
			case GL_TEXTURE_2D:
			default:
				samplerDict->SetValue("SAMPLER_SPEC", "sampler2D");
				texUnitDict->SetValue("TEX_COORDS", "st");
				break;
			}

			// When a special texture coordinate generation system is used, we have to include
			// the TEX_COORDS_GEN_DECL template which contains function declarations for the various
			// tex gen systems. Then for each texture unit that uses custom tex gen, we need to
			// instantiate a SINGLE_TEX_COORDS_GEN section with an appropriately initialized dictionary
			ctemplate::TemplateDictionary* singleTexGen = texGenDict->AddSectionDictionary("SINGLE_TEXCOORDS_ASSIGN");
			singleTexGen->SetIntValue("UV_SET_INDEX", mat.m_texStack->texInputs[i].uvSet);

			TexMapInput inputMapping = mat.m_texStack->texInputs[i].mapping;
			if (inputMapping != TexMapInput_UV) {
				if (!texGenDeclIncluded) {
					ctemplate::TemplateDictionary* texGenDecl = vertexShaderDict.AddIncludeDictionary(
							"TEX_COORDS_GEN_DECL");
					texGenDecl->SetFilename("shader_templates/ft_tex_coords_gen_decl.tpl");
					texGenDeclIncluded = true;
				}
				switch (inputMapping) {
				case TexMapInput_Normal:
					singleTexGen->ShowSection("SPHERE_TEXGEN");
					break;
				case TexMapInput_Refl:
					singleTexGen->ShowSection("REFLECTION_TEXGEN");
					break;
				case TexMapInput_Spherical:
					singleTexGen->ShowSection("SPHERE_TEXGEN");
					break;
				case TexMapInput_EyeSpace:
					singleTexGen->ShowSection("EYE_TEXGEN");
					break;
				case TexMapInput_ObjectSpace:
					singleTexGen->ShowSection("OBJECT_TEXGEN");
					break;
				default:
					SAFE_THROW(GLException(E_NOTIMPL, "Unimplemented texture mapping mode"))
				}
			} else {
				singleTexGen->ShowSection("UV_MAPPING");
			}

			++activeTextures;
		}
	}
	uniformsDict->SetIntValue("NUM_TEXTURES", activeTextures);

	// TODO: calculate this per material
	size_t numUvSets = 1;
	if (activeTextures > 0) {
		vertexIoDict->SetValueAndShowSection("NUM_UV_SETS", "1", "HAS_TEXTURES");
		fragmentIoDict->SetValueAndShowSection("NUM_UV_SETS", "1", "HAS_TEXTURES");
	}

	for (uint uv = 0; uv < numUvSets; uv++) {
		ctemplate::TemplateDictionary* uvsetDict = vertexShaderDict.AddSectionDictionary("SINGLE_UVSET_ASSIGN");
		uvsetDict->SetIntValue("UV_SET_INDEX", uv);
	}

	ctemplate::Template* vertexShaderTpl = ctemplate::Template::GetTemplate("shader_templates/ft_shader.tpl",
			ctemplate::STRIP_BLANK_LINES);
	std::string output;
	vertexShaderTpl->Expand(&output, &vertexShaderDict);
	std::cout << output;

	//---------------------------[ Fragment Shader ]-------------------------


	// Uniforms are shared between vertex and fragment shaders so we want the same section to appear in both
	// shader outputs. Unfortunately ctemplate doesn't support making copies of non top-level dictionaries
	// so for the fragment shaders, the uniform section is templated by a variable instead of a section.
	std::string fragUniformsStr;
	ctemplate::Template* uniformsTpl = ctemplate::Template::GetTemplate("shader_templates/uniforms.tpl",
			ctemplate::STRIP_BLANK_LINES);
	uniformsTpl->Expand(&fragUniformsStr, uniformsDict);
	fragmentShaderDict.SetValue("UNIFORMS", fragUniformsStr);

	// the same problem as above holds true for the fragment shader inputs/outputs dictionary, so we
	// again manually expand the template with the dictionary already created for the vertex shader
	std::string fragIoStr;
	ctemplate::Template* fragIoTpl = ctemplate::Template::GetTemplate("shader_templates/ft_fragment_inout.tpl",
			ctemplate::STRIP_BLANK_LINES);
	fragIoTpl->Expand(&fragIoStr, fragmentIoDict);
	fragmentShaderDict.SetValue("FRAGMENT_INPUTS", fragIoStr);

	// for now, enable Blinn-Phong shading mode only
	fragmentShaderDict.ShowSection("PHONG_LIGHTING");
	ctemplate::TemplateDictionary* lightingDict = fragmentShaderDict.AddIncludeDictionary("LIGHTING");
	lightingDict->SetFilename("shader_templates/ft_lighting.tpl");

	ctemplate::Template* fragmentShaderTpl = ctemplate::Template::GetTemplate("shader_templates/ft_fs_shader.tpl",
			ctemplate::STRIP_BLANK_LINES);
	std::string fragOutput;
	fragmentShaderTpl->Expand(&fragOutput, &fragmentShaderDict);
	std::cout << fragOutput;

}
