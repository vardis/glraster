#include "GLTutor.h"
#include "GLUtil.h"
#include "MaterialDB.h"
#include "Mesh.h"
#include "MeshModel.h"
#include "Billboard.h"

#include "RenderPrimitive.h"

#include "Entity.h"
#include "Viewport.h"
#include "RenderTargetTexture.h"

#include "PinholeCamera.h"
#include "OrthographicCamera.h"

#include "Lights.h"
#include "SceneManager.h"
#include "ILTextureManager.h"
#include "RenderablesRasterizer.h"
#include "MeshFactory.h"
#include "TTFont.h"
#include "Text.h"
#include "GLXTimer.h"
#include "ShaderGenerator.h"

#define ILUT_USE_OPENGL
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

GLXTimer timer;

GLenum g_shadeModel = GL_SMOOTH;
GLfloat rotZ = 0.0f;

bool camMoved = false;
GLfloat camX = 0.0f, camY = 0.0f, camZ = -1.0f;
GLfloat focusX = 0.0f, focusY = 0.0f, focusZ = 1.0f;

// In degrees
GLfloat azimuth = 90.0f;
GLfloat elevation = 90.0f;

float yaw, pitch, roll;

GLfloat g_ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat g_diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat g_specularLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat g_posLight[] = { 10.0f, 0.0f, 0.0f, 0.0f };

// Vertex data
GLfloat vertices[] = { 0.0f, 0.2f, 0.0f, -0.2f, -0.2f, 0.0f, 0.2f, -0.2f, 0.0f };

unsigned short indices[] = { 0, 1, 2 };

// Colour data
GLfloat colours[] = { 1.0f, 0.0f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.5f };

long g_lastTime;
MaterialDB g_matDB;
Mesh* bobMesh;
MeshModel* bobModel;
MeshModel* maleModel;
Entity* bobEntity;
Entity* billboardNMapEntity;
Entity* billboardEntity;
Entity* multiMat;
Entity* planeEntity;
Entity* quadStripEnt;
Entity* maleEntity;
Entity* teapotEntity;
Entity* monkeyEntity;
EntityPtr cubeEntity;

RenderTargetTexture* g_planeTextureRT;
PinholeCameraPtr g_camera;
ViewportPtr g_viewport;
SceneManagerPtr scene;
RenderablesRasterizer* g_rasterizer;
ILTextureManager* g_texMgr;

TTFontPtr g_font;
TextPtr g_text;
OrthographicCameraPtr g_cam2d;

LightPtr frontLight, backLight;
LightPtr pointLight;
LightPtr spotLight;

MaterialPtr billboardMat(new Material());

void displayFunc();
void reshapeFunc(int width, int height);
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int buton, int state, int x, int y);
void mouseMotionFunc(int x, int y);
void mousePassiveMotionFunc(int x, int y);
void specialKeyFunc(int key, int x, int y);
void idleFunc(void);
void closeFunc(void);
void createVertexBuffers(void);
void initialize();
void loadModels();
void loadAllTextures();
void rtt(RenderTargetTexture* rt);
void render(PinholeCameraPtr camera, ViewportPtr viewport);

int main(int argc, char** argv) {

	try {
		glutInit(&argc, argv);
		glutInitWindowSize(640, 480);
		glutInitWindowPosition(10, 10);
		glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE | GLUT_ALPHA | GLUT_DEPTH);

		glutInitContextVersion(3, 2);
		glutInitContextProfile( GLUT_COMPATIBILITY_PROFILE);
		glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);

		int mainWinID = glutCreateWindow("mainWin");
		glutSetWindow(mainWinID);

		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (GLEW_OK != err) {
			/* Problem: glewInit failed, something is seriously wrong. */
			std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
			return 1;
		}

		glutSetWindowTitle("OpenGL Tutor ver. 0.0.0.1");

		// set callbacks
		glutDisplayFunc(displayFunc);
		glutReshapeFunc(reshapeFunc);
		glutKeyboardFunc(keyboardFunc);
		glutMotionFunc(mouseMotionFunc);
		glutPassiveMotionFunc(mousePassiveMotionFunc);
		glutSpecialFunc(specialKeyFunc);
		glutIdleFunc(idleFunc);
		glutCloseFunc(closeFunc);

		initialize();
		if (!GLEW_ARB_vertex_buffer_object) {
			std::cerr << "GLEW_ARB_vertex_buffer_object not supported" << std::endl;
			exit(1);
		}

		glutMainLoop();
		glutDestroyWindow(mainWinID);

	} catch (GLException& e) {
		std::cerr << "Caught a GLException: " << e.what() << std::endl;
		std::cerr << "Details: " << e.details() << std::endl;
		std::cerr << "GL error: " << e.glError() << std::endl;
	}
	return 0;
}

void initialize() {

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	g_viewport = ViewportPtr(new Viewport());
	g_viewport->m_width = 640;
	g_viewport->m_height = 480;
	g_viewport->m_clearColor.set(0.0f);

	yaw = pitch = roll = 0.0f;

	g_camera = PinholeCameraPtr(new PinholeCamera());
	g_camera->setPos(Vec3f(0, 0, 5));
//	g_camera->setLook(Vec3f(0, 0, -1));
	g_camera->setNear(0.1f);
	g_camera->setFar(1000.0f);
	g_camera->setFov(60.0f);
	g_camera->setAspectRatio(0.75f);

	g_cam2d = OrthographicCameraPtr(new OrthographicCamera());
	g_cam2d->setPos(Vec3f(0, 0, 0));
	g_cam2d->setRotation(Vec3f(0.0f, 0.0f, 0.0f));
	g_cam2d->setNear(0.1f);
	g_cam2d->setFar(1000.0f);
	g_cam2d->setWidth(640);
	g_cam2d->setHeight(480);

	g_texMgr = new ILTextureManager();
	g_rasterizer = new RenderablesRasterizer(g_texMgr);

	g_font.reset(new TTFont());
	g_font->setFilename("bluebold.ttf");
	g_font->addCodepointRange(CodepointRange(32, 166));
	g_font->setPointSize(7);
	g_font->setHres(300);
	g_font->setVres(200);
	if (!g_font->create()) {
		std::cerr << "Error creating font\n";
	}

	frontLight = LightPtr(new Light());
	frontLight->m_type = Light::LightsTypeDirectional;
	frontLight->m_direction = Vec3f(1.0f, 0.0f, -1.0f);
	frontLight->m_diffuse = Colour::WHITE;
	frontLight->m_specular = Colour::WHITE;

	backLight = LightPtr(new Light());
	backLight->m_type = Light::LightsTypeDirectional;
	backLight->m_direction = Vec3f::Z_Axis;
	backLight->m_diffuse = Colour(1.0f, 0.0f, 0.0f); //Colour::WHITE;
	backLight->m_specular = Colour::WHITE;

	pointLight = LightPtr(new Light());
	pointLight->m_type = Light::LightsTypeLamp;
	pointLight->m_position = Vec3f(0.0f, 0.0f, 1.0f);
	pointLight->m_diffuse = Colour(1.0f, 1.0f, 1.0f);
	pointLight->m_specular = Colour::WHITE;
//	pointLight->m_linearAttenuation = 1.0f;

	spotLight = LightPtr(new Light());
	spotLight->m_type = Light::LightsTypeSpot;
	spotLight->m_position = Vec3f(0.0f, 0.0f, 1.0f);
	spotLight->m_direction = Vec3f(0.0f, 0.0f, -1.0f);
	spotLight->m_diffuse = Colour(1.0f, 1.0f, 1.0f);
	spotLight->m_specular = Colour::WHITE;
	spotLight->m_cosSpotCutOff = 0.9f;
	spotLight->m_cosOuterConeSpotCutOff = 0.8f;
//	spotLight->m_linearAttenuation = 1.0f;

	g_rasterizer->addLight(frontLight);
	//	g_rasterizer->addLight(backLight);
//	g_rasterizer->addLight(pointLight);
//	g_rasterizer->addLight(spotLight);

	loadModels();

	scene = SceneManagerPtr(new SceneManager());
	scene->setTextureManager(g_texMgr);
	scene->initialize();

	//	scene->setActiveSkyMapType(Sky_SphereMap);
	//	scene->setSphereSkyMap("spheremap.png");
	scene->setActiveSkyMapType(Sky_CubeMap);
	scene->setCubeSkyMap("early_morning.jpg");
}

void loadModels() {

	ShaderGenerator sg;
	MeshFactory mf(g_texMgr);

	///////////////////////////////////////////////////////////////////////
	// create multi-material cube
//	std::cout << "creating cube\n";
//	std::list<Mesh*> multiMatMeshes = mf.createFromFile("multitex.obj");
//	multiMat = new Entity();
//	std::list<Mesh*>::iterator it = multiMatMeshes.begin();
//	while (it != multiMatMeshes.end()) {
//		MeshModel* model = new MeshModel(*it);
//		multiMat->addRenderable(model);
//		++it;
//	}
//	Matrix4f& m2 = multiMat->getTransform();
//	m2 = Matrix4f::Translation(2.0f, 5.0f, 5.0f) * m2;
//	std::cout << "done cube\n";
	///////////////////////////////////////////////////////////////////////

//	// create a cube mesh
//	{
//		Mesh* cubeMesh = mf.createCube();
//
//		MaterialPtr cubeMat(new Material("cubeMat"));
//		cubeMat->m_twoSided = true;
//		cubeMat->m_specular.set(0.0f);
//		cubeMat->m_shadeless = true;
//
//		Texture* tex = new Texture();
//		g_texMgr->loadTexture("rockwall.bmp", tex);
//
//		cubeMat->addTexture(TexturePtr(tex), TexMapTo_Diffuse, TexBlendOp_Multiply);
//		cubeMesh->setMaterial(cubeMat);
//
//		cubeEntity = EntityPtr(new Entity());
//		MeshModel* cubeMeshModel = new MeshModel(cubeMesh);
//		cubeMeshModel->getRenderState().setCulling(false);
//		cubeEntity->addRenderable(cubeMeshModel);
//		Matrix4f& cubeXform = cubeEntity->getTransform();
//		cubeXform = Matrix4f::Translation(0.0f, 5.0f, 0.0f) * cubeXform;
//	}

	///////////////////////////////////////////////////////////////////////
	// create Bob mesh
//	std::list<Mesh*> meshes = mf.createFromFile("data/models/Bob.md5mesh");
//	bobEntity = new Entity();
//	it = meshes.begin();
//	while (it != meshes.end()) {
//		MeshModel* bobModel = new MeshModel(*it);
//		bobEntity->addRenderable(bobModel);
//		++it;
//	}
//	Matrix4f& m = bobEntity->getTransform();
//	m = Matrix4f::Scale(0.1f, 0.1f, 0.1f) * m;
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// create male figure mesh
	/*
	 std::list<Mesh*> maleMeshes = mf.createFromFile("muscular_male_basemesh.obj");
	 maleEntity = new Entity();
	 it = maleMeshes.begin();
	 while (it != maleMeshes.end()) {
	 MeshModel* maleModel = new MeshModel(*it);
	 maleModel->getMaterial()->m_texStack->textures[0]->setMinFilter(TexFilter_Bilinear_Mipmap_Bilinear);
	 maleModel->getMaterial()->m_texStack->textures[0]->setAnisotropy(8);
	 maleEntity->addRenderable(maleModel);
	 ++it;
	 }
	 Matrix4f& m1 = maleEntity->getTransform();
	 m1 = Matrix4f::Translation(0.0f, 0.0f, -3.0f) * Matrix4f::Scale(0.4f, 0.4f, 0.4f) * m1;
	 */
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// create teapot mesh
	/*
	 {
	 std::cout << "CREATING TEAPOT->>>>>>>>.\n";
	 Texture* tex = new Texture();
	 tex->m_useMipmaps = false;
	 g_texMgr->loadTexture("rockwall_NH.tga", tex);

	 std::list<Mesh*> teapotMeshes = mf.createFromFile("teapot-tbn.obj", true, true);
	 teapotEntity = new Entity();
	 it = teapotMeshes.begin();
	 while (it != teapotMeshes.end()) {
	 MeshModel* teapotModel = new MeshModel(*it);
	 teapotModel->getRenderState().setCulling(false);

	 MaterialPtr mat = teapotModel->getMaterial();
	 mat->m_texStack->textures[0].reset(tex);
	 mat->m_texStack->texOutputs[0].mapTo = TexMapTo_Normal;

	 teapotEntity->addRenderable(teapotModel);
	 ++it;
	 }

	 Matrix4f& m1 = teapotEntity->getTransform();
	 m1 = Matrix4f::Translation(4.0f, 0.4f, -2.0f) * m1;
	 }
	 */
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// create monkey mesh
	 {
		Texture* tex = new Texture();
		tex->m_useMipmaps = false;
		g_texMgr->loadTexture("uvgrid.png", tex);

		std::list<Mesh*> monkeMeshes = mf.createFromFile("unwrap.ply", true, true);
		monkeyEntity = new Entity();
		std::list<Mesh*>::iterator it = monkeMeshes.begin();
		while (it != monkeMeshes.end()) {
			MeshModel* monkeyModel = new MeshModel(*it);

			MaterialPtr mat = monkeyModel->getMaterial();
			mat->addTexture(TexturePtr(tex), TexMapTo_Diffuse, TexBlendOp_Multiply, TexMapInput_UV);

			monkeyEntity->addRenderable(monkeyModel);
			++it;
		}

		Matrix4f& m1 = monkeyEntity->getTransform();
		m1 = Matrix4f::Translation(4.0f, 0.4f, -2.0f) * m1;
	}

	///////////////////////////////////////////////////////////////////////
	// create billboard

	{
		billboardMat->m_name = "billboardMat";
		billboardMat->m_twoSided = true;
		billboardMat->m_diffuse.set(1.0f);
		billboardMat->m_specular.set(1.0f);
		billboardMat->m_shininess = 20.0f;

		//		billboardMat->m_shadeless = true;
		//	billboardMat->setCustomShaders(true);
		//	billboardMat->setFragmentShader("fs.glsl");
		billboardMat->m_texStack = TextureStackPtr(new TextureStack());

		Texture* tex = new Texture();
		tex->m_useMipmaps = true;
		tex->m_minFilter = TexFilter_Bilinear_Mipmap_Bilinear;
		tex->m_magFilter = TexFilter_Bilinear_Mipmap_Bilinear;
		g_texMgr->loadTexture(/*"fgrass1_v2_256.png" */"rockwall.bmp", tex);
		billboardMat->m_texStack->textures[0].reset(tex);
		billboardMat->m_texStack->texOutputs[0].blendOp = TexBlendOp_Multiply;

		Texture* normalTex = new Texture();
		normalTex->m_useMipmaps = false;
		g_texMgr->loadTexture("rockwall_NormalMap.bmp", normalTex);
		normalTex->configureGLState();
		billboardMat->addTexture(TexturePtr(normalTex), TexMapTo_Normal);
		normalTex->m_minFilter = TexFilter_Bilinear_Mipmap_Bilinear;
		normalTex->m_magFilter = TexFilter_Bilinear;
		normalTex->setWrapping(TexWrapMode_Repeat);

		Texture* parallaxTex = new Texture();
		parallaxTex->m_useMipmaps = true;
		g_texMgr->loadTexture("rockwall_Parallax.bmp", parallaxTex);
		parallaxTex->configureGLState();
		billboardMat->addTexture(TexturePtr(parallaxTex), TexMapTo_Parallax);
		parallaxTex->m_minFilter = TexFilter_Bilinear_Mipmap_Bilinear;
		parallaxTex->m_magFilter = TexFilter_Bilinear;
		parallaxTex->setWrapping(TexWrapMode_Repeat);

		BillboardPtr bb = Billboard::create(Billboard_Spherical, 1.0f, 1.0f);
		bb->setMaterial(billboardMat);
		bb->getRenderState().setCulling(false);

		billboardNMapEntity = new Entity();
		billboardNMapEntity->addRenderable(bb);

		Matrix4f& m1 = billboardNMapEntity->getTransform();
		m1 = Matrix4f::Translation(-4.0f, 0.0f, 0.0f) * m1;
	}

	// create billboard
	{
		MaterialPtr billboardMat(new Material());
		billboardMat->m_name = "billboardMat";
		billboardMat->m_twoSided = true;
		billboardMat->m_diffuse.set(1.0f);
		billboardMat->m_specular.set(0.0f);
		//		billboardMat->m_shadeless = true;
		billboardMat->m_texStack = TextureStackPtr(new TextureStack());

		Texture* tex = new Texture();
		tex->m_useMipmaps = true;
		tex->m_minFilter = TexFilter_Bilinear_Mipmap_Bilinear;
		tex->m_magFilter = TexFilter_Bilinear;
		g_texMgr->loadTexture("rockwall.bmp", tex);
		billboardMat->m_texStack->textures[0].reset(tex);
		billboardMat->m_texStack->texOutputs[0].blendOp = TexBlendOp_Multiply;

		BillboardPtr bb = Billboard::create(Billboard_Spherical, 1.0f, 1.0f);
		bb->setMaterial(billboardMat);
		bb->getRenderState().setCulling(false);

		billboardEntity = new Entity();
		billboardEntity->addRenderable(bb);

		Matrix4f& m1 = billboardEntity->getTransform();
		m1 = Matrix4f::Translation(2.0f, 0.0f, 0.0f) * m1;
	}
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// QuadStrip
	VertexFormat* vf = new VertexFormat();
	vf->addAttribute(Vertex_Pos, VertexFormat_FLOAT3);
	vf->addAttribute(Vertex_Normal, VertexFormat_FLOAT3);
	vf->addAttribute(Vertex_Tangent, VertexFormat_FLOAT3);
	vf->addAttribute(Vertex_BiNormal, VertexFormat_FLOAT3);
	vf->addAttribute(Vertex_TexCoord0, VertexFormat_FLOAT2);

	RenderPrimitive<QuadStripPrimitiveType>* qs = new RenderPrimitive<QuadStripPrimitiveType> ();
	qs->setMaterial(billboardMat);
	qs->specifyVertexFormat(VertexFormatPtr(vf));
	qs->beginGeometry(3);

	float quads_pos[] = { -1.0f, 1.0f, 0.0f,
	                      -1.0f, -1.0f, 0.0f,
	                      0.0f, 1.0f, 0.0f,
	                      0.0f, -1.0f, 0.0f,
	                      1.0f, 1.0f, 0.0f,
	 					  1.0f, -1.0f, 0.0f,
	 					  2.0f, 1.0f, 0.0f,
	 					  2.0f, -1.0f, 0.0f };
	qs->vertexAttribArray(Vertex_Pos, quads_pos);

	float quads_normals[] = { 0.0f, 0.0f, 1.0f };
	for (int v = 0; v < 8; v++)
		qs->vertexAttrib(Vertex_Normal, quads_normals);

	float quads_tangents[] = { 1.0f, 0.0f, 0.0f };
	for (int v = 0; v < 8; v++)
		qs->vertexAttrib(Vertex_Tangent, quads_tangents);

	float quads_binormals[] = { 0.0f, 1.0f, 0.0f };
	for (int v = 0; v < 8; v++)
		qs->vertexAttrib(Vertex_BiNormal, quads_binormals);

	float quads_uvs[] = { 0.0f, 1.0f, 0.0f, 0.0f, 0.33f, 1.0f, 0.33f, 0.0f, 0.66f, 1.0f, 0.66f, 0.0f, 1.0f, 1.0f, 1.0f,
							0.0f };

	qs->vertexAttribArray(Vertex_TexCoord0, quads_uvs);

	if (!qs->endGeometry()) {
		std::cerr << "error while specifying quad strip geometry\n";
		exit(0);
	}
	quadStripEnt = new Entity();
	quadStripEnt->addRenderable(qs);

//	Matrix4f& stripMat = quadStripEnt->getTransform();
//	stripMat = Matrix4f::Translation(4.0f, 0.0f, 0.0f);

	///////////////////////////////////////////////////////////////////////

	g_text.reset(new Text(g_font));
	//		g_text->setPos(100 - g_viewport->m_width/2, 100 - g_viewport->m_height/2);
	Matrix4f& mtxt = g_text->getTransform();
	mtxt = Matrix4f::Translation(10.0f - g_viewport->m_width / 2.0f, 10.0f - g_viewport->m_height / 2.0f, 0.0f) * mtxt;
	g_text->setText("hello");

	///////////////////////////////////////////////////////////////////////
	// create RTT quad
	MaterialPtr quadMat(new Material());
	quadMat->m_name = "quadMat";
	quadMat->m_twoSided = true;
	quadMat->m_diffuse.set(1.0f);
	quadMat->m_texStack = TextureStackPtr(new TextureStack());

	Texture* quadTex = new Texture();
	quadTex->m_width = quadTex->m_sourceWidth = 512;
	quadTex->m_height = quadTex->m_sourceHeight = 512;
	quadTex->m_textureTarget = GL_TEXTURE_2D;
	quadTex->m_useMipmaps = true;
	quadTex->m_minFilter = TexFilter_Bilinear_Mipmap_Bilinear;
	quadTex->m_magFilter = TexFilter_Bilinear;
	quadTex->allocate();
	quadTex->configureGLState();
	quadMat->m_texStack->textures[0].reset(quadTex);

	Mesh* quad = mf.createQuad();
	quad->setMaterial(quadMat);

	planeEntity = new Entity();
	planeEntity->addRenderable(new MeshModel(quad));
	Matrix4f& planeXform = planeEntity->getTransform();
	planeXform = Matrix4f::Translation(-2.0f, 2.0f, 5.0f) * planeXform;
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// render target texture
	g_planeTextureRT = new RenderTargetTexture(TexturePtr(quadTex), true, 0);
	g_planeTextureRT->allocate();
	///////////////////////////////////////////////////////////////////////
}

void displayFunc() {

	// Note: necessary as the text's render state set the depth mask to false
	GLUtil::setDefaultGLState();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	yaw = clamp(yaw, -360.0f, 360.0f);
	pitch = clamp(pitch, -90.0f, 90.0f);
	roll = clamp(roll, -360.0f, 360.0f);
	g_camera->updateViewProj();
	g_cam2d->updateViewProj();

	//	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	long elapsed = timer.getMillis();
	g_lastTime += elapsed;

	if (g_lastTime > 1000) {
		std::stringstream ss;
		ss << "Frame time: " << elapsed << " ms";

		g_text->setTransform(Matrix4f::Translation(10.0f - g_viewport->m_width / 2.0f, 10.0f - g_viewport->m_height	/ 2.0f, 0.0f));
		g_text->setText(ss.str());
		g_lastTime -= 1000;
	}

	//	rtt(g_planeTextureRT);
	render(g_camera, g_viewport);

//	g_rasterizer->render(g_text.get(), g_cam2d, 1);

	glutSwapBuffers();
}

void render(PinholeCameraPtr camera, ViewportPtr viewport) {

	glViewport(viewport->m_x, viewport->m_y, viewport->m_width, viewport->m_height);
	glClearColor(viewport->m_clearColor.r, viewport->m_clearColor.g, viewport->m_clearColor.b, viewport->m_clearColor.a);


	//	glPushMatrix();
	//	Vec3f& p = g_camera->getPos();
	//	glTranslatef(p.x, p.y, p.z);
//		scene->renderSky(g_rasterizer);
	//	glPopMatrix();


	// render Bob
	//	for (uint32_t i = 0; i < bobEntity->getNumRenderables(); i++) {
	//		Renderable* r = bobEntity->getRenderable(i);
	//		r->setTransform(bobEntity->getTransform());
	//		g_rasterizer->getRenderLayer(0).addRenderable(r);
	//	}

	// render male figure
	//	for (uint32_t i = 0; i < maleEntity->getNumRenderables(); i++) {
	//		Renderable* r = maleEntity->getRenderable(i);
	//		r->setTransform(maleEntity->getTransform());
	//		g_rasterizer->getRenderLayer(0).addRenderable(r);
	//	}

	// render teapot
	/*
	 for (uint32_t i = 0; i < teapotEntity->getNumRenderables(); i++) {
	 Renderable* r = teapotEntity->getRenderable(i);
	 r->setTransform(teapotEntity->getTransform());
	 g_rasterizer->getRenderLayer(0).addRenderable(r);
	 }*/

	for (uint32_t i = 0; i < monkeyEntity->getNumRenderables(); i++) {
		Renderable* r = monkeyEntity->getRenderable(i);
		r->setTransform(monkeyEntity->getTransform());
		g_rasterizer->getRenderLayer(0).addRenderable(r);
	}

	// render cube
//	Renderable* cube = cubeEntity->getRenderable(0);
//	cube->setTransform(cubeEntity->getTransform());
//	g_rasterizer->getRenderLayer(0).addRenderable(cube);

	// render billboard
//	Renderable* bb = billboardEntity->getRenderable(0);
//	bb->setTransform(billboardEntity->getTransform());
//	g_rasterizer->getRenderLayer(0).addRenderable(bb);

	// render normal mapped billboard
	//		Renderable* bbNMap = billboardNMapEntity->getRenderable(0);
	//		bbNMap->setTransform(billboardNMapEntity->getTransform());
	//		g_rasterizer->getRenderLayer(0).addRenderable(bbNMap);


	// render quad strip
//	Renderable* qs = quadStripEnt->getRenderable(0);
//	qs->setTransform(quadStripEnt->getTransform());
//	g_rasterizer->getRenderLayer(0).addRenderable(qs);

	/*
	 // render multi-material cube
	 for (uint32_t i = 0; i < multiMat->getNumRenderables(); i++) {
	 Renderable* r = multiMat->getRenderable(i);
	 r->setTransform(multiMat->getTransform());
	 g_rasterizer->getRenderLayer(0).addRenderable(r);
	 }

	 // render reflective plane
	 Renderable* pl = planeEntity->getRenderable(0);
	 pl->setTransform(planeEntity->getTransform());
	 g_rasterizer->getRenderLayer(0).addRenderable(pl);
	 */
	g_rasterizer->beginFrame(camera);
	g_rasterizer->endFrame();
}

void rtt(RenderTargetTexture* rt) {

	Viewport rttViewport;
	rttViewport.m_width = rt->m_texture->m_width;
	rttViewport.m_height = rt->m_texture->m_height;
	rttViewport.m_clearColor.set(0.0f);
	glViewport(0, 0, rttViewport.m_width, rttViewport.m_height);
	glClearColor(rttViewport.m_clearColor.r, rttViewport.m_clearColor.g, rttViewport.m_clearColor.b,
			rttViewport.m_clearColor.a);

	PinholeCameraPtr rttCamera(new PinholeCamera());
	rttCamera->setPos(Vec3f(0.0f, 0.0f, 20.0f)); //View(planeEntity->getTransform());
	rttCamera->setFov(90.0f);
	rttCamera->updateView();
	rttCamera->updateProj();

	g_planeTextureRT->bind();

	// render Bob
	for (uint32_t i = 0; i < bobEntity->getNumRenderables(); i++) {
		Renderable* r = bobEntity->getRenderable(i);
		r->setTransform(bobEntity->getTransform());
		g_rasterizer->getRenderLayer(0).addRenderable(r);
	}

	// render billboard
	Renderable* bb = billboardEntity->getRenderable(0);
	bb->setTransform(billboardEntity->getTransform());
	g_rasterizer->getRenderLayer(0).addRenderable(bb);

	// render multi-material cube
	for (uint32_t i = 0; i < multiMat->getNumRenderables(); i++) {
		Renderable* r = multiMat->getRenderable(i);
		r->setTransform(multiMat->getTransform());
		g_rasterizer->getRenderLayer(0).addRenderable(r);
	}

	g_rasterizer->beginFrame(rttCamera);
	g_rasterizer->endFrame();

	g_planeTextureRT->unbind();
}

void reshapeFunc(int width, int height) {
	float aspectRatio;
	height = (height == 0) ? 1 : height;
	g_viewport->m_width = width;
	g_viewport->m_height = height;

	g_cam2d->setWidth(width);
	g_cam2d->setHeight(height);

	aspectRatio = (float) height / (float) width;
	g_camera->setAspectRatio(aspectRatio);
}

void keyboardFunc(unsigned char key, int x, int y) {
	if (key == 's') {
		g_shadeModel = GL_SMOOTH;
	} else if (key == 'f') {
		g_shadeModel = GL_FLAT;
	} else if (key == 'a') {
		camMoved = true;
		g_camera->left(.5f);
	} else if (key == 'd') {
		camMoved = true;
		g_camera->right(.5f);
	} else if (key == 'r') {
		camMoved = true;
		g_camera->setPos(5.0f, 2.0f, 10.0f);
		g_camera->setLook(Vec3f::Zero);
	} else if (key == 'o') {
		spotLight->m_cosSpotCutOff -= 0.1f;
		spotLight->m_cosOuterConeSpotCutOff -= 0.1f;
		std::cout << "spotligt cut off angle: " << spotLight->m_cosSpotCutOff << "\n";
	} else if (key == 'p') {
		spotLight->m_cosSpotCutOff += 0.1f;
		spotLight->m_cosOuterConeSpotCutOff += 0.1f;
		std::cout << "spotligt cut off angle: " << spotLight->m_cosSpotCutOff << "\n";
	} else if (key == ',') {
		billboardMat->m_parallaxScale -= 0.01f;
		std::cout << "parallax scale: " << billboardMat->m_parallaxScale << "\n";
	} else if (key == '.') {
		billboardMat->m_parallaxScale += 0.01f;
		std::cout << "parallax scale: " << billboardMat->m_parallaxScale << "\n";
	} else if (key == '<') {
		billboardMat->m_parallaxBias -= 0.01f;
		std::cout << "parallax bias: " << billboardMat->m_parallaxBias << "\n";
	} else if (key == '>') {
		billboardMat->m_parallaxBias += 0.01f;
		std::cout << "parallax bias: " << billboardMat->m_parallaxBias << "\n";
	}
}

void mouseFunc(int buton, int state, int x, int y) {
}

void mouseMotionFunc(int x, int y) {
}

void mousePassiveMotionFunc(int x, int y) {
}

std::ostream& operator<<(std::ostream& os, Matrix4f mat) {
	os << "\n[" << mat.m[0][0] << " , " << mat.m[0][1] << " , " << mat.m[0][2] << " , " << mat.m[0][3] << "]\n";
	os << "[" << mat.m[1][0] << " , " << mat.m[1][1] << " , " << mat.m[1][2] << " , " << mat.m[1][3] << "]\n";
	os << "[" << mat.m[2][0] << " , " << mat.m[2][1] << " , " << mat.m[2][2] << " , " << mat.m[2][3] << "]\n";
	os << "[" << mat.m[3][0] << " , " << mat.m[3][1] << " , " << mat.m[3][2] << " , " << mat.m[3][3] << "]\n";
	return os;
}

void specialKeyFunc(int key, int x, int y) {
	camMoved = true;
	if (key == GLUT_KEY_LEFT) {
		g_camera->turnLeft(2.5f);
	} else if (key == GLUT_KEY_RIGHT) {
		g_camera->turnRight(2.5f);
	} else if (key == GLUT_KEY_PAGE_UP) {
		g_camera->lookUp(2.5f);
	} else if (key == GLUT_KEY_PAGE_DOWN) {
		g_camera->lookDown(2.5f);
	} else if (key == GLUT_KEY_UP) {
		g_camera->forward(0.2f);
	} else if (key == GLUT_KEY_DOWN) {
		g_camera->backward(0.2f);
	} else if (key == GLUT_KEY_HOME) {
		g_camera->up(.4f);
	} else if (key == GLUT_KEY_END) {
		g_camera->down(.4f);
	} else if (key == GLUT_KEY_F1) {
		std::cout << g_camera->getView();
	} else {
		camMoved = false;
	}
}

void idleFunc(void) {
	//    rotZ += 0.05f;
	if (camMoved) {
		std::cout << "cam pos (x, y, z): " << g_camera->getPos().x << ", " << g_camera->getPos().y << ", "
				<< g_camera->getPos().z << std::endl;
		std::cout << "cam orientation (yaw, pitch, roll): " << g_camera->getRotation().y << ", "
				<< g_camera->getRotation().x << ", " << g_camera->getRotation().z << std::endl;
		std::cout << "cam look-at (x, y, z): " << g_camera->getLook().x << ", " << g_camera->getLook().y << ", "
				<< g_camera->getLook().z << std::endl;
		std::cout << "cam up (x, y, z): " << g_camera->getUp().x << ", " << g_camera->getUp().y << ", "
				<< g_camera->getUp().z << std::endl;
		std::cout << "cam right (x, y, z): " << g_camera->getRight().x << ", " << g_camera->getRight().y << ", "
				<< g_camera->getRight().z << std::endl;
		camMoved = false;
	}
	glutPostRedisplay();
}

void closeFunc() {

}

