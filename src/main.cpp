#include "GLTutor.h"
#include "MaterialDB.h"
#include "Mesh.h"
#include "MeshModel.h"
#include "Billboard.h"
#include "Entity.h"
#include "Viewport.h"
#include "RenderTargetTexture.h"
#include "PinholeCamera.h"
#include "SceneManager.h"
#include "ILTextureManager.h"
#include "RenderablesRasterizer.h"
#include "MeshFactory.h"
#include "GLXTimer.h"

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

GLuint g_texID;

// In degrees
GLfloat azimuth = 90.0f;
GLfloat elevation = 90.0f;

float yaw, pitch, roll;

// Vertex Buffers IDs
GLuint triangleVBO;
GLuint coloursVBO;
GLuint indicesVBO;
GLuint meshVBO;

GLfloat g_ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat g_diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat g_specularLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat g_posLight[] = { 10.0f, 0.0f, 0.0f, 0.0f };

// Vertex data
GLfloat vertices[] = { 0.0f, 0.2f, 0.0f, -0.2f, -0.2f, 0.0f, 0.2f, -0.2f, 0.0f };

unsigned short indices[] = { 0, 1, 2 };

// Colour data
GLfloat colours[] = { 1.0f, 0.0f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.5f };

MaterialDB g_matDB;
Mesh* bobMesh;
MeshModel* bobModel;
Entity* bobEntity;
Entity* billboardEntity;
Entity* multiMat;
Entity* planeEntity;
RenderTargetTexture* g_planeTextureRT;
PinholeCameraPtr g_camera;
ViewportPtr g_viewport;
SceneManager scene;
RenderablesRasterizer* g_rasterizer;
ILTextureManager* g_texMgr;

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
	glutInit(&argc, argv);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(10, 10);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE | GLUT_ALPHA | GLUT_DEPTH);
	int mainWinID = glutCreateWindow("mainWin");

	GLenum err = glewInit();
	if (GLEW_OK != err) {
		/* Problem: glewInit failed, something is seriously wrong. */
		std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
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
	return 0;
}

void initialize() {

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

	glGenTextures(1, &g_texID);

	g_texMgr = new ILTextureManager();
	g_rasterizer = new RenderablesRasterizer(g_texMgr, &g_matDB);
	scene.setTextureManager(g_texMgr);
	scene.setActiveSkyMapType(Sky_SphereMap);
	scene.setSphereSkyMap("spheremap.png");
	//	scene.setActiveSkyMapType(Sky_CubeMap);
	//	scene.setCubeSkyMap("stevecube.jpg");

	loadModels();
	//	loadAllTextures();
	//	createVertexBuffers();

}

void loadModels() {

	MeshFactory mf(&g_matDB, g_texMgr);

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

	// create billboard of vegetation
	MaterialPtr billboardMat(new Material());
	billboardMat->m_name = "billboardMat";
	billboardMat->m_twoSided = true;
	billboardMat->m_diffuse.set(1.0f);
//	billboardMat->m_fragmentShader = "fs.glsl";
	billboardMat->m_textures = TextureStackPtr(new TextureStack());

	Texture* tex = new Texture();
	tex->m_useMipmaps = true;
	tex->m_minFilter = TexFilter_Bilinear_Mipmap_Bilinear;
	tex->m_magFilter = TexFilter_Bilinear;
	g_texMgr->loadTexture("fgrass1_v2_256.png", tex);
	billboardMat->m_textures->textures[0].reset(tex);
	billboardMat->m_textures->texOutputs[0].blendOp = TexBlendOp_Multiply;

	Billboard* bb = new Billboard(Billboard_Spherical, 1.0f, 1.0f);
	bb->setMaterial(billboardMat);
	billboardEntity = new Entity();
	billboardEntity->addRenderable(bb);

	// create RTT quad
	MaterialPtr quadMat(new Material());
	quadMat->m_name = "quadMat";
	quadMat->m_twoSided = true;
	quadMat->m_diffuse.set(1.0f);
	quadMat->m_textures = TextureStackPtr(new TextureStack());

	Texture* quadTex = new Texture();
	quadTex->m_width = quadTex->m_sourceWidth = 512;
	quadTex->m_height = quadTex->m_sourceHeight = 512;
	quadTex->m_textureTarget = GL_TEXTURE_2D;
	quadTex->m_useMipmaps = true;
	quadTex->m_minFilter = TexFilter_Bilinear_Mipmap_Bilinear;
	quadTex->m_magFilter = TexFilter_Bilinear;
	quadTex->allocate();
	quadTex->configureGLState();
	quadMat->m_textures->textures[0].reset(quadTex);

	Mesh* quad = mf.createQuad();
	quad->setMaterial(quadMat);

	planeEntity = new Entity();
	planeEntity->addRenderable(new MeshModel(quad));
	Matrix4f& planeXform = planeEntity->getTransform();
	planeXform = Matrix4f::Translation(-2.0f, 2.0f, 5.0f) * planeXform;

	g_planeTextureRT = new RenderTargetTexture(TexturePtr(quadTex), true, 0);
	g_planeTextureRT->allocate();

	/*
	 Assimp::Importer importer;
	 const aiScene
	 * model =
	 importer.ReadFile("data/models/Bob.md5mesh",
	 aiProcess_FixInfacingNormals  | aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals);
	 if (!model) {
	 std::cerr << importer.GetErrorString() << std::endl;
	 }
	 aiMesh* mesh = model->mMeshes[0];

	 if (!mesh->HasFaces()) {
	 std::cerr << "The model has no faces!" << std::endl;
	 }

	 std::cout << "num vertices: " << mesh->mNumVertices << std::endl;

	 VertexDataArrayPtr vertices(new VertexData[mesh->mNumVertices]);
	 VertexData* v = vertices.get();
	 for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
	 v->x = mesh->mVertices[i].x;
	 v->y = mesh->mVertices[i].y;
	 v->z = mesh->mVertices[i].z;
	 std::cout << "pos: " << v->x << ", " << v->y << ", " << v->z
	 << std::endl;
	 if (mesh->HasNormals()) {
	 v->normal[0] = mesh->mNormals[i].x;
	 v->normal[1] = mesh->mNormals[i].y;
	 v->normal[2] = mesh->mNormals[i].z;
	 }
	 if (mesh->HasVertexColors(0)) {
	 v->r = mesh->mColors[0][i].r;
	 v->g = mesh->mColors[0][i].g;
	 v->b = mesh->mColors[0][i].b;
	 v->a = mesh->mColors[0][i].a;
	 } else {
	 v->r = v->g = v->b = v->a = 1.0f;
	 }
	 std::cout << "col: " << v->r << ", " << v->g << ", " << v->b << ", "
	 << v->a << std::endl;
	 if (mesh->HasTextureCoords(0)) {
	 v->uv[0] = mesh->mTextureCoords[0][i].x;
	 v->uv[1] = mesh->mTextureCoords[0][i].y;
	 std::cout << "uv: " << v->uv[0] << ", " << v->uv[1] << std::endl;
	 } else {
	 v->uv[0] = v->uv[1] = 0.0f;
	 }
	 ++v;
	 }

	 uint32_t numIndices = 0;
	 for (uint32_t f = 0; f < mesh->mNumFaces; f++) {
	 if (mesh->mFaces[f].mNumIndices > 3) {
	 std::cerr << "Warning: face  " << f
	 << " has more than 3 vertices and will be skipped"
	 << std::endl;
	 } else {
	 numIndices += mesh->mFaces[f].mNumIndices;
	 }
	 }
	 std::cout << "num indices: " << numIndices << std::endl;

	 IndexArrayPtr indices(new uint32_t[numIndices]);
	 uint32_t* p = indices.get();
	 for (uint32_t f = 0; f < mesh->mNumFaces; f++) {
	 if (mesh->mFaces[f].mNumIndices == 3) {
	 *p++ = mesh->mFaces[f].mIndices[0];
	 *p++ = mesh->mFaces[f].mIndices[1];
	 *p++ = mesh->mFaces[f].mIndices[2];
	 }
	 }

	 bobMesh.updateVertexData(vertices, mesh->mNumVertices);
	 bobMesh.updateIndexData(indices, numIndices);

	 if (model->HasMaterials()) {
	 aiString name;
	 aiColor4D color;
	 float fval;
	 int ival;

	 std::cout << "Model has material\n";
	 aiMaterial* mat = model->mMaterials[mesh->mMaterialIndex];
	 Material& bobMat = *bobMesh.getMaterial();

	 mat->Get(AI_MATKEY_NAME,name) ;
	 bobMat.m_name.assign(&name.data[0], name.length);

	 mat->Get(AI_MATKEY_COLOR_DIFFUSE,color) ;
	 bobMat.m_diffuse.set(color.r, color.g, color.b, color.a);

	 mat->Get(AI_MATKEY_COLOR_SPECULAR,color) ;
	 bobMat.m_specular.set(color.r, color.g, color.b, color.a);

	 mat->Get(AI_MATKEY_COLOR_EMISSIVE,color) ;
	 bobMat.m_emissive.set(color.r, color.g, color.b, color.a);

	 mat->Get(AI_MATKEY_OPACITY,fval) ;
	 bobMat.m_opacity = fval;

	 mat->Get(AI_MATKEY_SHININESS,fval) ;
	 bobMat.m_shininess = fval;

	 mat->Get(AI_MATKEY_TWOSIDED,ival) ;
	 bobMat.m_twoSided = ival;

	 TextureStack* texStack = new TextureStack();
	 bobMat.m_textures = TextureStackPtr(texStack);

	 for (uint8_t t = 0; t < mat->GetTextureCount(aiTextureType_DIFFUSE); t++) {
	 aiString texPath;
	 aiTextureMapping texMapping;
	 unsigned int uvIndex;
	 float blend;
	 aiTextureOp texOp;
	 aiTextureMapMode texWrapMode;

	 if (AI_SUCCESS == mat->GetTexture(aiTextureType_DIFFUSE, t,
	 &texPath, &texMapping, &uvIndex, &blend, &texOp,
	 &texWrapMode)) {
	 std::cout << "loading texture " << &texPath.data[0] << std::endl;
	 Texture* tex = new Texture();
	 tex->filename = &texPath.data[0];
	 tex->minFilter = TexFilter_Linear;
	 tex->magFilter = TexFilter_Linear;
	 tex->useMipmaps = true;
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
	 tex->wrapping = TexWrapMode_Clamp;
	 break;
	 default:
	 tex->wrapping = TexWrapMode_Repeat;
	 }
	 texStack->texInputs[t].uvSet = uvIndex;
	 texStack->textures[t] = TexturePtr(tex);
	 texStack->texOutputs[t].mapTo = TexMapTo_Diffuse;
	 texStack->texOutputs[t].blendFactor = blend;

	 g_texID = tex->texID = g_texMgr->loadTexture(tex->filename, tex);
	 }

	 }
	 }
	 */
}

void loadAllTextures() {
	for (uint32_t i = 0; i < g_matDB.getNumMaterials(); i++) {
		MaterialPtr mat = g_matDB.getMaterialById(i);
		if (mat && mat->m_textures) {
			for (uint8_t t = 0; t < MAX_TEXTURES_STACK; t++) {
				if (mat->m_textures->textures[t]) {
					TexturePtr tex = mat->m_textures->textures[t];
					g_texMgr->loadTexture(tex->m_filename, tex.get());
				}
			}
		}
	}
}

void drawTeapots() {

	glDisable(GL_TEXTURE_2D);
	glUseProgram(0);

	glPushMatrix();
	glTranslatef(0.0, 1.0f, 16.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glutSolidTeapot(.4f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 1.0f, -16.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glutSolidTeapot(.4f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(16.0, 1.0f, 0.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glPolygonOffset(1.f, 1.f);
	glutSolidTeapot(.4f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutWireTeapot(.4f);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-16.0, 1.0f, 0.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glPolygonOffset(1.f, 1.f);
	glutSolidTeapot(.4f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutWireTeapot(.4f);
	glPopMatrix();
}

void displayFunc(void) {

	glDisable(GL_CULL_FACE);

	yaw = clamp(yaw, -360.0f, 360.0f);
	pitch = clamp(pitch, -90.0f, 90.0f);
	roll = clamp(roll, -360.0f, 360.0f);
	g_camera->updateViewProj();

	//		glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, g_ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, g_diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, g_specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, g_posLight);

	glShadeModel(g_shadeModel);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	rtt(g_planeTextureRT);
	render(g_camera, g_viewport);

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

	drawTeapots();

	// render Bob
//	for (uint32_t i = 0; i < bobEntity->getNumRenderables(); i++) {
//		Renderable* r = bobEntity->getRenderable(i);
//		r->setTransform(bobEntity->getTransform());
//		g_rasterizer->getRenderLayer(0).addRenderable(r);
//	}

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

	// render reflective plane
	Renderable* pl = planeEntity->getRenderable(0);
	pl->setTransform(planeEntity->getTransform());
	g_rasterizer->getRenderLayer(0).addRenderable(pl);

	g_rasterizer->beginFrame(camera);
	g_rasterizer->endFrame();

	uint32_t elapsed = timer.getMillis();
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

	drawTeapots();

	// render Bob
	for (uint32_t i = 0; i < bobEntity->getNumRenderables(); i++) {
		Renderable* r = bobEntity->getRenderable(i);
		r->setTransform(bobEntity->getTransform());
		g_rasterizer->getRenderLayer(0).addRenderable(r);
	}

	// render billboard
	g_rasterizer->getRenderLayer(0).addRenderable(billboardEntity->getRenderable(0));

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
	if (glIsBuffer(triangleVBO))
		glDeleteBuffers(1, &triangleVBO);

	glDeleteTextures(1, &g_texID);

	scene.dispose();
}
