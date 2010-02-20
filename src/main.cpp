#include "GLTutor.h"
#include "MaterialDB.h"
#include "Mesh.h"
#include "MeshModel.h"
#include "Billboard.h"

#include "RenderPrimitive.h"

#include "Entity.h"
#include "Viewport.h"
#include "RenderTargetTexture.h"
#include "PinholeCamera.h"
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
Entity* billboardEntity;
Entity* multiMat;
Entity* planeEntity;
Entity* quadStripEnt;
Entity* maleEntity;

RenderTargetTexture* g_planeTextureRT;
PinholeCameraPtr g_camera;
ViewportPtr g_viewport;
SceneManager scene;
RenderablesRasterizer* g_rasterizer;
ILTextureManager* g_texMgr;

TTFontPtr g_font;
TextPtr g_text;

void displayFunc(void);
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

		glutInitContextVersion (3, 2);
		glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
//		glutInitContextFlags (GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);


		int mainWinID = glutCreateWindow("mainWin");

		GLenum err = glewInit();
		if (GLEW_OK != err) {
			/* Problem: glewInit failed, something is seriously wrong. */
			std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
			return 1;
		}

		glutSetWindow(mainWinID);
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
	g_camera->setPos(Vec3f(0, 0, 0));
	g_camera->setRotation(Vec3f(yaw, pitch, roll));
	g_camera->setNear(0.1f);
	g_camera->setFar(1000.0f);
	g_camera->setFov(60.0f);
	g_camera->setAspectRatio(0.75f);

	g_texMgr = new ILTextureManager();
	g_rasterizer = new RenderablesRasterizer(g_texMgr, &g_matDB);
	scene.setTextureManager(g_texMgr);
	scene.setActiveSkyMapType(Sky_SphereMap);
	scene.setSphereSkyMap("spheremap.png");
	//	scene.setActiveSkyMapType(Sky_CubeMap);
	//	scene.setCubeSkyMap("stevecube.jpg");

	g_font.reset(new TTFont());
	g_font->setFilename("bluebold.ttf");
	g_font->addCodepointRange(CodepointRange(32, 166));
	g_font->setPointSize(3);
	g_font->setHres(800);
	g_font->setVres(600);
	if (!g_font->create()) {
		std::cerr << "Error creating font\n";
	}

	loadModels();
}

void loadModels() {

	MeshFactory mf(&g_matDB, g_texMgr);

	///////////////////////////////////////////////////////////////////////
	// create multi-material cube
	std::cout << "creating cube\n";
	std::list<Mesh*> multiMatMeshes = mf.createFromFile("multitex.obj");
	multiMat = new Entity();
	std::list<Mesh*>::iterator it = multiMatMeshes.begin();
	while (it != multiMatMeshes.end()) {
		MeshModel* model = new MeshModel(*it);
		multiMat->addRenderable(model);
		++it;
	}
	Matrix4f& m2 = multiMat->getTransform();
	m2 = Matrix4f::Translation(2.0f, 5.0f, 5.0f) * m2;
	std::cout << "done cube\n";
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// create Bob mesh
	std::list<Mesh*> meshes = mf.createFromFile("data/models/Bob.md5mesh");
	bobEntity = new Entity();
	it = meshes.begin();
	while (it != meshes.end()) {
		MeshModel* bobModel = new MeshModel(*it);
		bobEntity->addRenderable(bobModel);
		++it;
	}
	Matrix4f& m = bobEntity->getTransform();
	m = Matrix4f::Scale(0.1f, 0.1f, 0.1f) * m;
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// create male figure mesh
	ShaderGenerator sg;
	std::list<Mesh*> maleMeshes = mf.createFromFile("muscular_male_basemesh.obj");
	maleEntity = new Entity();
	it = maleMeshes.begin();
	while (it != maleMeshes.end()) {
		MeshModel* maleModel = new MeshModel(*it);
		maleModel->getMaterial()->m_texStack->textures[0]->setMinFilter(TexFilter_Bilinear_Mipmap_Bilinear);
		maleModel->getMaterial()->m_texStack->textures[0]->setAnisotropy(8);
		maleModel->getMaterial()->m_texStack->texOutputs[0].mapTo = TexMapTo_Shininess;
		maleEntity->addRenderable(maleModel);

		maleModel->getMaterial()->m_texStack->texInputs[0].mapping = TexMapInput_ObjectSpace;
		sg.generateShaders(*maleModel->getMaterial(), *maleModel->getMesh()->getVertexFormat());

		++it;
	}
	Matrix4f& m1 = maleEntity->getTransform();
	m1 = Matrix4f::Scale(0.4f, 0.4f, 0.4f) * m1;

	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// create billboard of vegetation
	MaterialPtr billboardMat(new Material());
	billboardMat->m_name = "billboardMat";
	billboardMat->m_twoSided = true;
	billboardMat->m_diffuse.set(1.0f);
	//	billboardMat->m_fragmentShader = "fs.glsl";
	billboardMat->m_texStack = TextureStackPtr(new TextureStack());

	Texture* tex = new Texture();
	tex->m_useMipmaps = true;
	tex->m_minFilter = TexFilter_Bilinear_Mipmap_Bilinear;
	tex->m_magFilter = TexFilter_Bilinear;
	g_texMgr->loadTexture(/*"fgrass1_v2_256.png"*/"waves2.dds", tex);

	billboardMat->m_texStack->textures[0].reset(tex);
	billboardMat->m_texStack->texOutputs[0].blendOp = TexBlendOp_Multiply;

	Billboard* bb = new Billboard(Billboard_Spherical, 1.0f, 1.0f);
	bb->setMaterial(billboardMat);
	billboardEntity = new Entity();
	billboardEntity->addRenderable(bb);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// QuadStrip
	VertexFormat vf;
	vf.addAttribute(Vertex_Pos, VertexFormat_FLOAT3);
	vf.addAttribute(Vertex_Normal, VertexFormat_FLOAT3);
	vf.addAttribute(Vertex_TexCoord0, VertexFormat_FLOAT2);

	RenderPrimitive<QuadStripPrimitiveType>* qs = new RenderPrimitive<QuadStripPrimitiveType> ();
	qs->setMaterial(billboardMat);
	qs->specifyVertexFormat(VertexFormatPtr(VertexFormat::create(VF_V3_N3_T2)));
	qs->beginGeometry(3);

	float quads_pos[] = { -1.0f, 1.0f, 0.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
							1.0f, -1.0f, 0.0f, 2.0f, 1.0f, 0.0f, 2.0f, -1.0f, 0.0f };
	qs->vertexAttribArray(Vertex_Pos, quads_pos);

	float quads_normals[] = { 0.0f, 0.0f, 1.0f };
	for (int v = 0; v < 8; v++)
		qs->vertexAttrib(Vertex_Normal, quads_normals);

	float quads_uvs[] = { 0.0f, 1.0f, 0.0f, 0.0f, 0.33f, 1.0f, 0.33f, 0.0f, 0.66f, 1.0f, 0.66f, 0.0f, 1.0f, 1.0f, 1.0f,
							0.0f };

	qs->vertexAttribArray(Vertex_TexCoord0, quads_uvs);

	if (!qs->endGeometry()) {
		std::cerr << "error while specifying quad strip geometry\n";
		exit(0);
	}
	quadStripEnt = new Entity();
	quadStripEnt->addRenderable(qs);

	///////////////////////////////////////////////////////////////////////

	g_text.reset(new Text(g_font));
	g_text->setPos(10, 10);
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


void displayFunc(void) {

	glDisable(GL_CULL_FACE);

	yaw = clamp(yaw, -360.0f, 360.0f);
	pitch = clamp(pitch, -90.0f, 90.0f);
	roll = clamp(roll, -360.0f, 360.0f);
	g_camera->updateViewProj();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	rtt(g_planeTextureRT);
	render(g_camera, g_viewport);

	long elapsed = timer.getMillis();
	g_lastTime += elapsed;

	if (g_lastTime > 1000) {
		std::stringstream ss;
		ss << "Frame time: " << elapsed << " ms";
		g_text->setText(ss.str());
		g_lastTime -= 1000;
	}

	g_rasterizer->setRender2D(800, 600);
	g_rasterizer->render(g_text.get(), g_camera);

	glFlush();
	glutSwapBuffers();
}

void render(PinholeCameraPtr camera, ViewportPtr viewport) {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultTransposeMatrixf(&camera->getProj().m[0][0]);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultTransposeMatrixf(&camera->getView().m[0][0]);

	glViewport(viewport->m_x, viewport->m_y, viewport->m_width, viewport->m_height);
	glClearColor(viewport->m_clearColor.r, viewport->m_clearColor.g, viewport->m_clearColor.b, viewport->m_clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	Vec3f& p = g_camera->getPos();
	glTranslatef(p.x, p.y, p.z);
	scene.renderSky();
	glPopMatrix();


	// render Bob
	//	for (uint32_t i = 0; i < bobEntity->getNumRenderables(); i++) {
	//		Renderable* r = bobEntity->getRenderable(i);
	//		r->setTransform(bobEntity->getTransform());
	//		g_rasterizer->getRenderLayer(0).addRenderable(r);
	//	}

	// render male figure
	for (uint32_t i = 0; i < maleEntity->getNumRenderables(); i++) {
		Renderable* r = maleEntity->getRenderable(i);
		r->setTransform(maleEntity->getTransform());
		g_rasterizer->getRenderLayer(0).addRenderable(r);
	}

	// render billboard
	Renderable* bb = billboardEntity->getRenderable(0);
	bb->setTransform(billboardEntity->getTransform());
	g_rasterizer->getRenderLayer(0).addRenderable(bb);

	// render quad strip
	//		Renderable* qs = quadStripEnt->getRenderable(0);
	//		qs->setTransform(quadStripEnt->getTransform());
	//		g_rasterizer->getRenderLayer(0).addRenderable(qs);

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

	glMatrixMode(GL_PROJECTION);
	//	glLoadIdentity();
	//	glMultTransposeMatrixf(&rttCamera->getProj().m[0][0]);
	glLoadTransposeMatrixf(&rttCamera->getProj().m[0][0]);

	glMatrixMode(GL_MODELVIEW);
	glLoadTransposeMatrixf(&rttCamera->getView().m[0][0]);
	//	glMultTransposeMatrixf(&rttCamera->getView().m[0][0]);

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
	glViewport(0, 0, width, height);
	float aspectRatio;
	height = (height == 0) ? 1 : height;
	g_viewport->m_width = width;
	g_viewport->m_height = height;
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
	}
}

void mouseFunc(int buton, int state, int x, int y) {
}

void mouseMotionFunc(int x, int y) {
}

void mousePassiveMotionFunc(int x, int y) {
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
		std::cout << "cam right (x, y, z): " << g_camera->getRight().x << ", " << g_camera->getRight().y << ", "
				<< g_camera->getRight().z << std::endl;
		camMoved = false;
	}
	glutPostRedisplay();
}

void closeFunc() {
	scene.dispose();
}

