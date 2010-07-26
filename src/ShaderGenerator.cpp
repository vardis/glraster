/*
 * ShaderGenerator.cpp
 *
 *  Created on: Jan 30, 2010
 *      Author: giorgos
 */

#include "GLTutor.h"
#include "ShaderGenerator.h"

#include <ctemplate/template.h>

const char* const ShaderGenerator::TEX_COORDS = "TEX_COORDS";
const char* const ShaderGenerator::TEX_INDEX = "TEX_INDEX";
const char* const ShaderGenerator::UV_SET_INDEX = "UV_SET_INDEX";
const char* const ShaderGenerator::S_COORD = "s";
const char* const ShaderGenerator::ST_COORDS = "st";
const char* const ShaderGenerator::STP_COORDS = "stp";
const char* const ShaderGenerator::TANGENT_SPACE_NMAP_TPL = "shader_templates/tangent_space_nmap.tpl";

ShaderGenerator::ShaderGenerator() {

}

ShaderGenerator::~ShaderGenerator() {
}

void ShaderGenerator::generateShaders(Material& mat, const RenderState& rs, const VertexFormat& vf) {

	bool hasNormalMap = false;
	bool hasParallaxMap = false;

	ctemplate::TemplateDictionary vertexShaderDict("vertexShader");
	ctemplate::TemplateDictionary fragmentShaderDict("fragmentShader");

	ctemplate::TemplateDictionary* vertexIoDict = vertexShaderDict.AddIncludeDictionary("VERTEX_INPUTS");
	vertexIoDict->SetFilename("shader_templates/ft_vertex_inout.tpl");

	ctemplate::TemplateDictionary* fragmentOutDict = vertexShaderDict.AddIncludeDictionary("FRAGMENT_INPUTS");
	fragmentOutDict->SetFilename("shader_templates/ft_fragment_inout.tpl");
	fragmentOutDict->ShowSection("OUT_DIRECTION");

	ctemplate::TemplateDictionary* uniformsDict = vertexShaderDict.AddIncludeDictionary("UNIFORMS");
	uniformsDict->SetFilename("shader_templates/uniforms.tpl");

	// use lighting when material uses shading and we have normals
	bool useLighting = !mat.m_shadeless && vf.getAttributeBySemantic(Vertex_Normal);
	if (useLighting) {
		uniformsDict->ShowSection("USE_LIGHTING");
	}

	if (vf.getAttributeBySemantic(Vertex_Normal)) {
		vertexShaderDict.ShowSection("HAS_NORMALS");
		vertexShaderDict.ShowSection("NORMALIZE_NORMALS");

		vertexIoDict->ShowSection("HAS_NORMALS");
		vertexIoDict->ShowSection("NORMALIZE_NORMALS");

		fragmentOutDict->ShowSection("HAS_NORMALS");

		uniformsDict->ShowSection("HAS_NORMALS");
	}

	if (vf.getAttributeBySemantic(Vertex_Tangent)) {
		fragmentOutDict->ShowSection("HAS_TANGENTS");
		vertexIoDict->ShowSection("HAS_TANGENTS");
	}

	if (vf.getAttributeBySemantic(Vertex_BiNormal)) {
		fragmentOutDict->ShowSection("HAS_BINORMALS");
		vertexIoDict->ShowSection("HAS_BINORMALS");
	}

	if (vf.getAttributeBySemantic(Vertex_Color)) {
		vertexShaderDict.ShowSection("HAS_COLORS");
		vertexIoDict->ShowSection("HAS_COLORS");
		fragmentOutDict->ShowSection("HAS_COLORS");
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

	ctemplate::TemplateDictionary* parallaxMapDict = 0;

	for (int i = 0; i < MAX_TEXTURES_STACK; i++) {
		TexturePtr tex = mat.m_texStack->textures[i];
		if (tex) {
			// for the fragment shader, here we can fill in the dictionary for the texture application section
			// that takes into account the texture's mapTo, environment color, uvset, etc.
			ctemplate::TemplateDictionary* texUnitDict = fragmentShaderDict.AddIncludeDictionary(
					"SINGLE_TEXTURE_STACK_ENTRY");
			texUnitDict->SetFilename("shader_templates/ft_tex_stack_application.tpl");

			ctemplate::TemplateDictionary* alphaMapDict = 0;
			ctemplate::TemplateDictionary* tangentNormalMapDict = 0;
			ctemplate::TemplateDictionary* offsetMappingDict = 0;

			// depending on the texture's mapTo, we will emit different sections in the template
			ctemplate::TemplateDictionary* texMapToDict = 0;
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
			case TexMapTo_Alpha:
				alphaMapDict = fragmentShaderDict.AddSectionDictionary("ALPHA_MAP_APPLICATION");
				break;
			case TexMapTo_Normal:
				tangentNormalMapDict = fragmentShaderDict.AddIncludeDictionary("TANGENT_SPACE_NORMAL_MAP");
				tangentNormalMapDict->SetFilename(TANGENT_SPACE_NMAP_TPL);
				parallaxMapDict = tangentNormalMapDict;
				hasNormalMap = true;
				break;
			case TexMapTo_Parallax:
				/* IMPORTANT: Parallax maps must come after the normal maps */
				if (parallaxMapDict) {
					offsetMappingDict = parallaxMapDict->AddSectionDictionary("PARALLAX_OFFSET_MAPPING");
				}
				hasParallaxMap = true;
				break;
			default:
				SAFE_THROW(GLException(E_NOTIMPL, "Unimplemented texture output mapping mode"))
			}

			texUnitDict->SetIntValue(TEX_INDEX, activeTextures);
			texUnitDict->SetIntValue(UV_SET_INDEX, mat.m_texStack->texInputs[i].uvSet);

			if (alphaMapDict) {
				alphaMapDict->SetIntValue(TEX_INDEX, activeTextures);
				alphaMapDict->SetIntValue(UV_SET_INDEX, mat.m_texStack->texInputs[i].uvSet);
			}

			if (tangentNormalMapDict) {
				tangentNormalMapDict->SetIntValue(TEX_INDEX, activeTextures);
				tangentNormalMapDict->SetIntValue(UV_SET_INDEX, mat.m_texStack->texInputs[i].uvSet);
			}
			if (offsetMappingDict) {
				offsetMappingDict->SetIntValue(TEX_INDEX, activeTextures);
				offsetMappingDict->SetIntValue(UV_SET_INDEX, mat.m_texStack->texInputs[i].uvSet);
			}

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
			samplerDict->SetIntValue(TEX_INDEX, activeTextures);

			const char* samplerToken;
			const char* coordsToken;
			switch (tex->getTextureTarget()) {
			case GL_TEXTURE_1D:
				samplerToken = "sampler1D";
				coordsToken = S_COORD;
				break;
			case GL_TEXTURE_CUBE_MAP:
				samplerToken = "samplerCube";
				coordsToken = STP_COORDS;
				break;
			case GL_TEXTURE_2D:
			default:
				samplerToken = "sampler2D";
				coordsToken = ST_COORDS;
				break;
			}

			samplerDict->SetValue("SAMPLER_SPEC", samplerToken);
			texUnitDict->SetValue(TEX_COORDS, coordsToken);

			if (alphaMapDict) {
				alphaMapDict->SetValue(TEX_COORDS, coordsToken);
			}
			if (tangentNormalMapDict) {
				tangentNormalMapDict->SetValue(TEX_COORDS, coordsToken);
			}
			if (offsetMappingDict) {
				offsetMappingDict->SetValue(TEX_COORDS, coordsToken);
			}

			// When a special texture coordinate generation system is used, we have to include
			// the TEX_COORDS_GEN_DECL template which contains function declarations for the various
			// tex gen systems. Then for each texture unit that uses custom tex gen, we need to
			// instantiate a SINGLE_TEX_COORDS_GEN section with an appropriately initialized dictionary
			ctemplate::TemplateDictionary* singleTexGen = texGenDict->AddSectionDictionary("SINGLE_TEXCOORDS_ASSIGN");
			singleTexGen->SetIntValue(UV_SET_INDEX, mat.m_texStack->texInputs[i].uvSet);
			singleTexGen->SetValue(TEX_COORDS, coordsToken);

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
					singleTexGen->ShowSection("NORMAL_TEXGEN");
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
				case TexMapInput_Cube:
					singleTexGen->ShowSection("CUBE_TEXGEN");
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

	if (hasNormalMap) {
		vertexShaderDict.ShowSection("NORMAL_MAPPING");
		fragmentOutDict->ShowSection("NORMAL_MAPPING");
		fragmentOutDict->ShowSection("NORMAL_MAPPING");
	}

	// TODO: calculate this per material
	size_t numUvSets = 1;
	if (activeTextures > 0) {
		uniformsDict->SetValueAndShowSection("NUM_TEXTURES", "1", "HAS_TEXTURES");
		vertexIoDict->SetValueAndShowSection("NUM_UV_SETS", "1", "HAS_TEXTURES");
		fragmentOutDict->SetValueAndShowSection("NUM_UV_SETS", "1", "HAS_TEXTURES");
	}

	for (uint uv = 0; uv < numUvSets; uv++) {
		ctemplate::TemplateDictionary* uvsetDict = vertexShaderDict.AddSectionDictionary("SINGLE_UVSET_ASSIGN");
		uvsetDict->SetIntValue(UV_SET_INDEX, uv);
	}

	ctemplate::Template* vertexShaderTpl = ctemplate::Template::GetTemplate("shader_templates/ft_shader.tpl",
			ctemplate::STRIP_BLANK_LINES);
	std::string vertexOutput;
	vertexShaderTpl->Expand(&vertexOutput, &vertexShaderDict);
	std::cout << vertexOutput;
	mat.getGpuProgram()->attachShader(vertexOutput, GL_VERTEX_SHADER);

	//---------------------------[ Fragment Shader ]-------------------------


	// Uniforms are shared between vertex and fragment shaders so we want the same section to appear in both
	// shader outputs. Unfortunately ctemplate doesn't support making copies of non top-level dictionaries
	// so for the fragment shaders, the uniform section is templated by a variable instead of a section.
	std::string fragUniformsStr;
	ctemplate::Template* uniformsTpl = ctemplate::Template::GetTemplate("shader_templates/uniforms.tpl",
			ctemplate::STRIP_BLANK_LINES);
	uniformsTpl->Expand(&fragUniformsStr, uniformsDict);
	fragmentShaderDict.SetValue("UNIFORMS", fragUniformsStr);

	ctemplate::TemplateDictionary* fragmentInDict = fragmentShaderDict.AddIncludeDictionary("FRAGMENT_INPUTS");
	fragmentInDict->SetFilename("shader_templates/ft_fragment_inout.tpl");
	fragmentInDict->ShowSection("IN_DIRECTION");
	if (vf.getAttributeBySemantic(Vertex_Normal)) {
		fragmentInDict->ShowSection("HAS_NORMALS");
	}
	if (vf.getAttributeBySemantic(Vertex_Color)) {
		fragmentInDict->ShowSection("HAS_COLORS");
	}
	if (activeTextures > 0) {
		fragmentInDict->SetValueAndShowSection("NUM_UV_SETS", "1", "HAS_TEXTURES");
	}

	std::string fragIoStr;
	ctemplate::Template* fragIoTpl = ctemplate::Template::GetTemplate("shader_templates/ft_fragment_inout.tpl",
			ctemplate::STRIP_BLANK_LINES);
	fragIoTpl->Expand(&fragIoStr, fragmentOutDict);
	fragmentShaderDict.SetValue("FRAGMENT_INPUTS", fragIoStr);

	if (useLighting) {

		fragmentShaderDict.ShowSection("USE_LIGHTING");

		// for now, enable Blinn-Phong shading mode only
		ctemplate::TemplateDictionary* fragLightingDict = fragmentShaderDict.AddIncludeDictionary("PHONG_LIGHTING");
		fragLightingDict->SetFilename("shader_templates/ft_fs_lighting.tpl");

		if (!hasNormalMap) {
			fragmentShaderDict.ShowSection("GEOMETRIC_NORMAL");
			fragLightingDict->ShowSection("VIEW_SPACE_LIGHTING");
		} else {
			fragLightingDict->ShowSection("NORMAL_MAPPING");
			fragmentInDict->ShowSection("NORMAL_MAPPING");
		}

		ctemplate::TemplateDictionary* lightingDict = fragmentShaderDict.AddIncludeDictionary("LIGHTING");
		lightingDict->SetFilename("shader_templates/ft_lighting.tpl");
		lightingDict->ShowSection("WITH_LIGHT_ATTENUATION");
	}

	ctemplate::Template* fragmentShaderTpl = ctemplate::Template::GetTemplate("shader_templates/ft_fs_shader.tpl",
			ctemplate::STRIP_BLANK_LINES);
	std::string fragOutput;
	fragmentShaderTpl->Expand(&fragOutput, &fragmentShaderDict);
	std::cout << fragOutput;
	mat.getGpuProgram()->attachShader(fragOutput, GL_FRAGMENT_SHADER);
}
