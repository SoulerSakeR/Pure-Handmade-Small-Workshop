#include "renderwidget.h"


#include <iostream>
#include <QPainter>
#include <Core/SystemStatus/GameEngine.h>
#include <QElapsedTimer>
#include <Core/ResourceManagement/SceneMgr.h>
#include "qevent.h"
#include <qopenglframebufferobject.h>
#include <qapplication.h>
#include <Core/ResourceManagement/ResourceMgr.h>
#include "Core/GameLogic/GameLoop.h"

#define glCheckError() glCheckError_(__FILE__, __LINE__)

RenderWidget* RenderWidget::sceneWidget= nullptr;
RenderWidget* RenderWidget::gameWidget = nullptr;
RenderWidget* RenderWidget::currentWidget = nullptr;
QOpenGLContext* RenderWidget::sharedContext = nullptr;

bool RenderWidget::widgetChanged = false;
std::string source_path;
static int count = 0;


GLenum RenderWidget::glCheckError_(const char* file, int line) {
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR) {
		std::string error;
		switch (errorCode) {
		case GL_INVALID_ENUM:
			error = "INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			error = "INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			error = "INVALID_OPERATION";
			break;
			//case GL_STACK_OVERFLOW:
			//  error = "STACK_OVERFLOW";
			//  break;
			//case GL_STACK_UNDERFLOW:
			//  error = "STACK_UNDERFLOOR";
			//  break;
		case GL_OUT_OF_MEMORY:
			error = "OUT_OF_MEMORY";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			error = "INVALID_FRAMEBUFFER_OPERATION";
			break;
		}

		std::cout << error << " | " << file << " (" << line << ") " << std::endl;
	}

	return errorCode;
}




RenderWidget::RenderWidget(QWidget* parent) : QOpenGLWidget(parent)
{
	setFocusPolicy(Qt::StrongFocus);
	connect(&timer, SIGNAL(timeout()), this, SLOT(on_timeout()));
	frameCount = 0;
	fbo = nullptr;
	fboOverlay = nullptr;
	sharedContext = nullptr;
	mCameraObject = new GameObject("Camera");
	mCamera = mCameraObject->addComponent<Camera>();
	mCamera->set_view_width(500);
	setContextMenuPolicy(Qt::NoContextMenu);
}


RenderWidget::~RenderWidget()
{
	if (!isValid()) return;
}

void RenderWidget::drawShape(Shape shape)
{
	m_shape = shape;
	update();
}

void RenderWidget::setWirefame(bool wireframe)
{

	if (wireframe)
		//         画线
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	update();
	doneCurrent();
}


void RenderWidget::renderGameobject(GameObject* gameobj, Camera* camera)
{
	
	if (camera == mCamera) // edit mode, render all components
	{
		bool visBorder = gameobj == getSelectedGameObject();
		if (auto img = gameobj->getComponent<Image>(); img != nullptr && img->get_enabled())
		{
			renderImage(img, camera, visBorder);
		}
		if (auto boxCollider = gameobj->getComponent<BoxCollider>(); boxCollider != nullptr && boxCollider->get_enabled())
		{
			renderBoxCollider(boxCollider, camera, visBorder);
		}

		if (auto text = gameobj->getComponent<Text>(); text != nullptr && text->get_enabled())
		{
			renderText(text, camera, visBorder);
		}

		
		// camera border
		if (auto target = gameobj->getComponent<Camera>(); target != nullptr && target->get_enabled())
		{
			renderCameraBorder(target, camera, visBorder);
		}

		if (auto target = gameobj->getComponent<SpineAnimator>();target != nullptr && target->get_enabled())
		{
			renderSpineAnimator(target, camera, visBorder);
		}
		
	}
	else // game mode, only render image and text
	{
		if (auto img = gameobj->getComponent<Image>(); img != nullptr && img->get_enabled())
		{
			renderImage(img, camera, false);
		}

		if (auto text = gameobj->getComponent<Text>(); text != nullptr && text->get_enabled())
		{
			renderText(text, camera, false);
		}

		if (auto target = gameobj->getComponent<SpineAnimator>();target != nullptr && target->get_enabled())
		{
			renderSpineAnimator(target, camera, false);
		}

	}
	
}

void RenderWidget::renderBoxCollider(BoxCollider* box, Camera* boxColliderCamera, bool visBorder)
{
	if(!visBorder) 
		return;	
	if (box->vao == nullptr)
	{
		box->vao = new QOpenGLVertexArrayObject;
		box->vao->create();
		box->vao->bind();
		box->vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
		box->vbo->create();
		box->vbo->bind();
		box->vbo->allocate(box->vertices.data(), static_cast<int>(box->vertices.size() * sizeof(Vertex)));
		box->borderIbo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
		box->borderIbo->create();
		box->borderIbo->bind();
		box->borderIbo->allocate(box->borderIndices.data(), static_cast<int>(box->borderIndices.size() * sizeof(unsigned int)));

		boxColliderShaderProgram->bind();
		GLint posLocation = boxColliderShaderProgram->attributeLocation("aPos");
		GLuint stride = sizeof(Vertex);				
		glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);		
		glEnableVertexAttribArray(posLocation);

		box->vao->release();
		box->vbo->release();
		box->vbo->destroy();
		delete box->vbo;
		box->vbo = nullptr;
	}
	box->vao->bind();
	
	//calculate the MVP matrix
	QMatrix4x4 matrix;
	if (GameEngine::get_instance().getInEditor() && !isGameWidget)
	{
		matrix = mCamera->CalculateProjectionMulViewMatrix();
	}
	else
	{
		matrix = SceneMgr::get_instance().get_main_camera()->CalculateProjectionMulViewMatrix();
	}

	matrix = boxColliderCamera->CalculateProjectionMulViewMatrix();
	auto transform = box->gameObject->transform;
	matrix.translate(transform->getWorldPosition().toQVector3D());
	matrix.rotate(transform->getWorldRotation(), QVector3D(0.f, 0.f, 1.f));
	matrix.scale(transform->getWorldScale().toQVector3D(1.0f));

	//transfer the MVP matrix to the shader
	boxColliderShaderProgram->bind();
	boxColliderShaderProgram->setUniformValue("MVPMatrix", matrix);
	const auto color = box->get_color();
	boxColliderShaderProgram->setUniformValue("color",color.red(), color.green(), color.blue(),color.alpha());
	boxColliderShaderProgram->setUniformValue("isTexture", false);
	boxColliderShaderProgram->setUniformValue("isLighting", false);
	//set line mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(3.0f);

	//draw
	glDrawElements(GL_LINES, box->borderIndices.size(), GL_UNSIGNED_INT, 0);
	box->vao->release();
}

void RenderWidget::renderImage(Image* img, Camera* imageCamera, bool visBorder)
{
	auto transform = img->gameObject->transform;
	imageShaderProgram->bind();
	//============================================LIGHTING=========================================//
	// 获取场景里的所有光源
	auto& lightSources = SceneMgr::get_instance().lights;

	// 将所有的光源信息存储到 lightSources 中

	// 计算需要传递给着色器的光源数量
	int numLights = lightSources.size();

	// 创建一个固定大小的数组来存储所有的光源
	QVector<LightSourceData> lightData(numLights);

	QVector3D lightsColor[32];
	QVector2D lightsPos[32];
	QVector2D lightsIntensityAndRadius[32];

	int validLight = 0;

	for (int i = 0, j = 0; j < numLights; j++)
	{
		auto worldPos = lightSources[j]->gameObject->transform->getWorldPosition();
		auto color = lightSources[j]->get_light_color();

		auto distance = (worldPos - transform->getWorldPosition()).toQVector2D().length();
		auto radius = lightSources[j]->get_radius();
		

		if ((worldPos - transform->getWorldPosition()).toQVector2D().length() >= lightSources[j]->get_radius())
			continue;

		lightsColor[i].setX(color.red());
		lightsColor[i].setY(color.green());
		lightsColor[i].setZ(color.blue());

		lightsPos[i].setX(worldPos.x);
		lightsPos[i].setY(worldPos.y);

		lightsIntensityAndRadius[i].setX(lightSources[j]->get_intensity());
		lightsIntensityAndRadius[i].setY(lightSources[j]->get_radius());

		i++;
		validLight++;
		

	}

	// 将光源数量和光源数组传递到着色器中
	imageShaderProgram->setUniformValue("numLights", validLight);

	imageShaderProgram->setUniformValueArray("lightsColor", lightsColor, 32);
	imageShaderProgram->setUniformValueArray("lightsPosition", lightsPos, 32);
	imageShaderProgram->setUniformValueArray("lightsIntensityAndRadius", lightsIntensityAndRadius, 32);

	for (int i = 0; i < validLight; i++)
	{
		shadowTest(img, imageCamera, visBorder, lightsPos[i]);
	}
	
	//=====================================================================================//
	
	
	
	if (widgetChanged)
	{
		if (img->vao != nullptr)
		{
			img->vao->release();
			img->vao->destroy();
			delete img->vao;
			img->vao = nullptr;
		}
		if (img->vbo != nullptr)
		{
			img->vbo->release();
			img->vbo->destroy();
			delete img->vbo;
			img->vbo = nullptr;
		}
		if (img->ibo != nullptr)
		{
			img->ibo->release();
			img->ibo->destroy();
			delete img->ibo;
			img->ibo = nullptr;
		}
		if (img->borderIbo != nullptr)
		{
			img->borderIbo->release();
			img->borderIbo->destroy();
			delete img->borderIbo;
			img->borderIbo = nullptr;
		}
	}
	//bind vao if exist
	if (img->vao == nullptr)
	{
		img->vao = new QOpenGLVertexArrayObject;
		img->vao->create();
		img->vao->bind();
		img->vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
		img->vbo->create();
		img->vbo->bind();
		img->vbo->allocate(img->vertices.data(), static_cast<int>(img->vertices.size() * sizeof(Vertex)));
		img->ibo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
		img->ibo->create();
		img->ibo->bind();
		img->ibo->allocate(img->indices.data(), static_cast<int>(img->indices.size() * sizeof(unsigned int)));
		imageShaderProgram->bind();

		GLint posLocation = imageShaderProgram->attributeLocation("aPos");
		GLint textureLocation = imageShaderProgram->attributeLocation("aTexCoord");
		GLsizei stride = sizeof(Vertex);
		glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		glEnableVertexAttribArray(posLocation);

		if (textureLocation)
		{
			glVertexAttribPointer(textureLocation, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(textureLocation);
		}
		img->vao->release();
		img->vbo->release();
		img->vbo->destroy();
		delete img->vbo;
		img->vbo = nullptr;
	}
	img->vao->bind();

	auto view_width = imageCamera->get_view_width();
	imageShaderProgram->bind();

	QMatrix4x4 projMatrix;
	auto resolution = GameEngine::get_instance().get_resolution();
	float ratio = resolution.y / resolution.x;
	projMatrix.ortho(-view_width / 2, view_width / 2, -view_width / 2 * ratio, view_width / 2 * ratio, -10.f, 10.f);
	imageShaderProgram->setUniformValue("projMatrix", projMatrix);

	QMatrix4x4 Matrix = QMatrix4x4();
	auto cameraWorldPos = imageCamera->gameObject->transform->getWorldPosition();
	Matrix.rotate(imageCamera->gameObject->transform->getWorldRotation(), 0.f, 0.f, 1.f);
	QMatrix4x4 viewMatrix = QMatrix4x4();
	viewMatrix.lookAt(cameraWorldPos.toQVector3D(1.f), cameraWorldPos.toQVector3D(-1.f), Matrix* QVector4D(0.f, 1.f, 0.f, 1.f).toVector3D());
	imageShaderProgram->setUniformValue("viewMatrix", viewMatrix);

	// calculate the model matrix
	QMatrix4x4 modelMatrix;
	// matrix = imageCamera->CalculateProjectionMulViewMatrix();
	
	modelMatrix.translate(transform->getWorldPosition().toQVector3D());
	modelMatrix.rotate(transform->getWorldRotation(), QVector3D(0.f, 0.f, 1.f));
	modelMatrix.scale(transform->getWorldScale().toQVector3D(1.0f));
	imageShaderProgram->setUniformValue("modelMatrix", modelMatrix);

	QVector2D SCREEN_SIZE = QVector2D(GameEngine::get_instance().get_resolution().x, GameEngine::get_instance().get_resolution().y);
	imageShaderProgram->setUniformValue("SCREEN_SIZE", SCREEN_SIZE);

	QMatrix4x4 matrix;
	matrix = imageCamera->CalculateProjectionMulViewMatrix();

	//auto transform = img->gameObject->transform;
	matrix.translate(transform->getWorldPosition().toQVector3D());
	matrix.rotate(transform->getWorldRotation(), QVector3D(0.f, 0.f, 1.f));
	matrix.scale(transform->getWorldScale().toQVector3D(1.0f));

	//transfer the MVP matrix to the shader
	imageShaderProgram->setUniformValue("MVPMatrix", matrix);

	const auto& color = img->get_color();
	imageShaderProgram->setUniformValue("color", color.red(), color.green(), color.blue(), color.alpha());

	imageShaderProgram->setUniformValue("isTexture", true);
	imageShaderProgram->setUniformValue("isLighting", true);


	
	//=================================================================================================//

	//bind texture
	/*if(img->texture!=nullptr)
		img->texture->bind();*/
	if (img->isTextureValid())
	{
		img->get_texture()->create();
		bool flag = img->get_texture()->isCreated();
		img->get_texture()->bind();
	}
		
	img->ibo->bind();
	//enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//enable depth test
	glDepthFunc(GL_LEQUAL);

	//set texture mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//draw texture
	glDrawElements(GL_TRIANGLES, img->indices.size(), GL_UNSIGNED_INT, 0);
	
	if (img->isTextureValid())
		img->get_texture()->release();

	if (visBorder == true)
	{
		//draw border
		if (img->borderIbo == nullptr)
		{
			img->borderIbo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
			img->borderIbo->create();
			img->borderIbo->bind();
			img->borderIbo->allocate(img->borderIndices.data(), static_cast<int>(img->borderIndices.size() * sizeof(unsigned int)));
		}
		img->borderIbo->bind();
	
		imageShaderProgram->setUniformValue("isTexture", false);
		imageShaderProgram->setUniformValue("color", 1.0f, 1.0f, 1.0f, 1.0f);
		imageShaderProgram->setUniformValue("isLighting", false);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(3.0f);
		glDrawElements(GL_LINES, static_cast<GLsizei>(img->borderIndices.size()), GL_UNSIGNED_INT, 0);
		img->borderIbo->release();
	}	
	//release
	img->vao->release();
	/*if(img->texture!=nullptr)
		img->texture->release();*/
}

void RenderWidget::renderText(Text* text, Camera* textCamera, bool visBorder)
{

	// 获取纹理
	QString data = QString::fromStdString(text->get_text());	
	data.replace("\\n", "\n");
	// 将data根据\n拆分成若干个字符串
	QStringList list = data.split("\n");
	// 获取最长字符串的长度和字符串的个数
	int maxLen = 0;
	int count = list.count();
	for (int i = 0; i < count; i++)
	{
		if (list[i].length() > maxLen)
		{
			maxLen = list[i].length();
		}
	}
	// 宽度设置为最大字符串长度 * 字符宽度，字符宽度设置为35
	int width = maxLen * 35;
	// 高度设置为字符串个数 * 字符高度，字符高度设置为75
	int height = count * 75;

	text->set_size(Vector2D(width, height));

	// 创建QColor对象color为text.color
	const auto& color32 = text->get_color();
	QColor color(color32.r, color32.g, color32.b, color32.a);
	if (mTexture != nullptr)
	{
		mTexture->release();
		mTexture->destroy();
		delete mTexture;
		mTexture = nullptr;
	}
	mTexture = genTextTexture(width, height, data, 60, color);

	if (widgetChanged)
	{
		if (text->vao != nullptr)
		{
			text->vao->release();
			text->vao->destroy();
			delete text->vao;
			text->vao = nullptr;
		}
		if (text->vbo != nullptr)
		{
			text->vbo->release();
			text->vbo->destroy();
			delete text->vbo;
			text->vbo = nullptr;
		}
		if (text->ibo != nullptr)
		{
			text->ibo->release();
			text->ibo->destroy();
			delete text->ibo;
			text->ibo = nullptr;
		}
		if (text->borderIbo != nullptr)
		{
			text->borderIbo->release();
			text->borderIbo->destroy();
			delete text->borderIbo;
			text->borderIbo = nullptr;
		}
	}

	//bind vao if exist
	if (text->vao == nullptr)
	{
		text->vao = new QOpenGLVertexArrayObject;
		text->vao->create();
		text->vao->bind();
		text->vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
		text->vbo->create();
		text->vbo->bind();
		//text->vbo->allocate(TextVertices, 36 * sizeof(float));
		text->vbo->allocate(text->vertices.data(), static_cast<int>(text->vertices.size() * sizeof(Vertex)));
		
		text->ibo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
		text->ibo->create();
		text->ibo->bind();
		text->ibo->allocate(text->indices.data(), static_cast<int>(text->indices.size() * sizeof(unsigned int)));
		textShaderProgram->bind();

		GLint posLocation = textShaderProgram->attributeLocation("aPos");
		//GLint colorLocation = textShaderProgram->attributeLocation("aColor");
		GLint textureLocation = textShaderProgram->attributeLocation("aTexCord");

		GLsizei stride = sizeof(Vertex);
		glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);		
		glEnableVertexAttribArray(posLocation);		
		if (textureLocation)
		{
			//------------------Texture-----------------------//
			glVertexAttribPointer(textureLocation, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(textureLocation);
		}
		text->vao->release();
		text->vbo->release();
		text->vbo->destroy();
		delete text->vbo;
		text->vbo = nullptr;
	}
	text->vao->bind();

	//calculate the MVP matrix
	QMatrix4x4 matrix;
	if (GameEngine::get_instance().getInEditor() && !isGameWidget)
	{
		matrix = mCamera->CalculateProjectionMulViewMatrix();
	}
	else
	{
		matrix = SceneMgr::get_instance().get_main_camera()->CalculateProjectionMulViewMatrix();
	}

	matrix = textCamera->CalculateProjectionMulViewMatrix();
	auto transform = text->gameObject->transform;
	matrix.translate(transform->getWorldPosition().toQVector3D());
	matrix.rotate(transform->getWorldRotation(), QVector3D(0.f, 0.f, 1.f));
	matrix.scale(transform->getWorldScale().toQVector3D(1.0f));

	//transfer the MVP matrix to the shader
	textShaderProgram->bind();
	textShaderProgram->setUniformValue("MVPMatrix", matrix);

	//bind texture
	mTexture->bind();
	//text->texture = mTexture;	
	text->ibo->bind();
	
	//enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//enable depth test
	glDepthFunc(GL_LEQUAL);

	//set texture mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	
	//draw
	glDrawElements(GL_TRIANGLES, text->indices.size(), GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	text->ibo->release();
	mTexture ->release();
	
	if (textCamera == mCamera && visBorder == true)
	{
		//draw border
		if (text->borderIbo == nullptr)
		{
			text->borderIbo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
			text->borderIbo->create();
			text->borderIbo->bind();
			text->borderIbo->allocate(text->borderIndices.data(), static_cast<int>(text->borderIndices.size() * sizeof(unsigned int)));
		}
		text->borderIbo->bind();

		boxColliderShaderProgram->bind();
		boxColliderShaderProgram->setUniformValue("MVPMatrix", matrix);

		glLineWidth(3.0f);
		glDrawElements(GL_LINES, static_cast<GLsizei>(text->borderIndices.size()), GL_UNSIGNED_INT, 0);
		text->borderIbo->release();

	}	
	//release
	text->vao->release();
}

void RenderWidget::renderSpineAnimator(SpineAnimator* target, Camera* camera, bool visBorder)
{
	target->updateVertices();
	if(target->is_Valid())
		drawMesh(target, camera,visBorder);
}

// render camera border

void RenderWidget::renderCameraBorder(Camera* target, Camera* renderCamera, bool visBorder)
{
	if(!visBorder)
		return;

	float cameraWidth = target->get_view_width();
	Vector2D ratio = GameEngine::get_instance().get_resolution();
	// 换算opengl宽高比
	float aspect = static_cast<float>(ratio.x) / static_cast<float>(ratio.y);
	// 计算camera的高
	float cameraHeight = cameraWidth / aspect;
	target->set_size(Vector2D(cameraWidth, cameraHeight));
	
	if (widgetChanged)
	{
		if (target->vao != nullptr)
		{
			target->vao->release();
			target->vao->destroy();
			delete target->vao;
			target->vao = nullptr;
		}
		if (target->vbo != nullptr)
		{
			target->vbo->release();
			target->vbo->destroy();
			delete target->vbo;
			target->vbo = nullptr;
		}
		if (target->ibo != nullptr)
		{
			target->ibo->release();
			target->ibo->destroy();
			delete target->ibo;
			target->ibo = nullptr;
		}
		if (target->borderIbo != nullptr)
		{
			target->borderIbo->release();
			target->borderIbo->destroy();
			delete target->borderIbo;
			target->borderIbo = nullptr;
		}
	}
	
	if (target->vao == nullptr)
	{
		target->vao = new QOpenGLVertexArrayObject;
		target->vao->create();
		target->vao->bind();
		target->vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
		target->vbo->create();
		target->vbo->bind();
		target->vbo->allocate(target->vertices.data(), static_cast<int>(target->vertices.size() * sizeof(Vertex)));
		target->borderIbo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
		target->borderIbo->create();
		target->borderIbo->bind();
		target->borderIbo->allocate(target->borderIndices.data(), static_cast<int>(target->borderIndices.size() * sizeof(unsigned int)));
		
		cameraBorderShaderProgram->bind();
		GLint posLocation = cameraBorderShaderProgram->attributeLocation("aPos");
		
		GLsizei stride = sizeof(Vertex);				
		glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		glEnableVertexAttribArray(posLocation);

		
		target->vao->release();
		target->vbo->release();
		target->vbo->destroy();
		delete target->vbo;
		target->vbo = nullptr;
		
	}

	auto matrix = renderCamera->CalculateProjectionMulViewMatrix();
	auto transform = target->gameObject->transform;
	matrix.translate(transform->getWorldPosition().toQVector3D());
	matrix.rotate(transform->getWorldRotation(), QVector3D(0.f, 0.f, 1.f));
	matrix.scale(transform->getWorldScale().toQVector3D(1.0f));

	target->vao->bind();
	const auto& color = target->get_color();
	cameraBorderShaderProgram->bind();
	cameraBorderShaderProgram->setUniformValue("MVPMatrix", matrix);
	cameraBorderShaderProgram->setUniformValue("color", color.red(),color.green(), color.blue(), color.alpha());
	cameraBorderShaderProgram->setUniformValue("isTexture", false);
	cameraBorderShaderProgram->setUniformValue("isLighting", false);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(3.0f);
	
	glDrawElements(GL_LINES, static_cast<GLsizei>(target->borderIndices.size()), GL_UNSIGNED_INT, 0);

	target->vao->release();

}

	

void RenderWidget::drawMesh(IRenderable* target, Camera* camera, bool visBorder)
{
	static auto vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);	
	vbo->create();
	vbo->bind();	
	vbo->allocate(target->vertices.data(), static_cast<int>(target->vertices.size() * sizeof(Vertex)));
	vbo->setUsagePattern(QOpenGLBuffer::DynamicDraw);
	static auto ibo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
	ibo->create();
	ibo->bind();
	ibo->allocate(target->indices.data(), static_cast<int>(target->indices.size() * sizeof(unsigned int)));
	ibo->setUsagePattern(QOpenGLBuffer::DynamicDraw);
	imageShaderProgram->bind();
	
	GLint posLocation = imageShaderProgram->attributeLocation("aPos");
	GLint texCoordLocation = imageShaderProgram->attributeLocation("aTexCoord");

	GLsizei stride = sizeof(Vertex);
	if (posLocation != -1)
	{
		glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		glEnableVertexAttribArray(posLocation);
	}
	if (texCoordLocation != -1)
	{
		glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(texCoordLocation);
	}

	// mvp matrix
	if(camera == nullptr)
		camera = mCamera;
	auto matrix = camera->CalculateProjectionMulViewMatrix();
	auto transform = target->gameObject->transform;
	matrix.translate(transform->getWorldPosition().toQVector3D());
	matrix.rotate(transform->getWorldRotation(), QVector3D(0.f, 0.f, 1.f));
	matrix.scale(transform->getWorldScale().toQVector3D(1.0f));
	
	const auto& color = target->get_color();
	imageShaderProgram->setUniformValue("MVPMatrix", matrix);
	imageShaderProgram->setUniformValue("color", color.red(), color.green(), color.blue(), color.alpha());
	imageShaderProgram->setUniformValue("isTexture", true);
	imageShaderProgram->setUniformValue("isLighting", true);

	//texture
	if (target->isTextureValid())
		target->get_texture()->bind();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(target->indices.size()), GL_UNSIGNED_INT, 0);

	
	//release
	vbo->release();
	ibo->release();
	if (target->isTextureValid())
		target->get_texture()->release();

	//draw border
	if (visBorder)
	{
		auto box = dynamic_cast<IBoxResizable*>(target);
		if (box == nullptr)
			return;
		if (box->borderVbo == nullptr)
		{
			box->borderVbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
			box->borderVbo->create();
			box->borderVbo->bind();
			box->borderVbo->allocate(box->borderVertices.data(), static_cast<int>(box->borderVertices.size() * sizeof(Vertex)));
		}
		box->borderVbo->bind();
		if (box->borderIbo == nullptr)
		{
			box->borderIbo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
			box->borderIbo->create();
			box->borderIbo->bind();
			box->borderIbo->allocate(box->borderIndices.data(), static_cast<int>(box->borderIndices.size() * sizeof(unsigned int)));
		}
		box->borderIbo->bind();
		
		GLint posLocation = imageShaderProgram->attributeLocation("aPos");

		GLuint stride = sizeof(Vertex);
		glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		glEnableVertexAttribArray(posLocation);

		imageShaderProgram->setUniformValue("isTexture", false);
		imageShaderProgram->setUniformValue("isLighting", false);
		imageShaderProgram->setUniformValue("color", 1.0f, 1.0f, 1.0f, 1.0f);
		


		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(1.0f);
		glDrawElements(GL_LINES, static_cast<GLsizei>(box->borderIndices.size()), GL_UNSIGNED_INT, 0);
		box->borderVbo->release();
		box->borderIbo->release();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void RenderWidget::renderScene(Camera* camera)
{	
	auto scene = SceneMgr::get_instance().get_current_scene();
	if ( scene == nullptr || scene->getRootGameObjs().size() == 0 || mCamera == nullptr)
		return;
	for (auto& layer : SceneMgr::get_instance().get_render_setting()->render_order_map)
	{
		// TODO: batch rendering
		for (auto& obj : layer.second)
		{
			if (obj->is_active())
				renderGameobject(obj, camera);
		}
	}
}


void RenderWidget::initializeGL()
{

	 // Enable debug context
	initializeOpenGLFunctions();
	// debug info
	
	/*
	logger = std::make_unique<QOpenGLDebugLogger>(this);
	logger->initialize();
	connect(logger.get(), &QOpenGLDebugLogger::messageLogged, this, &RenderWidget::messageLogHandler);
	logger->startLogging();
	*/

	source_path = GameEngine::get_instance().getRootPath();

	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	
	createALLShaderProgram();

	createTextProgram();
	createTextureProgram();
	
	
	textBuffer = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::VertexBuffer);
	timer.setInterval(10);
}

void RenderWidget::resetResolution()
{
	resetFBO();
}

void RenderWidget::resizeGL(int w, int h)
{
	Q_UNUSED(w); Q_UNUSED(h);
	glViewport(0, 0, w, h);	
	resetFBO();
}

void RenderWidget::paintGL()
{
	if (!SceneMgr::get_instance().hasCurrentScene())
		return;
	isRendering = true;
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	frameCount++;
	renderFbo();	
	renderFboOverlay();
	mixTexture();
	glCheckError();
	//update scene render order
	if (auto renderSetting = SceneMgr::get_instance().get_render_setting();renderSetting != nullptr && renderSetting->refresh_later)
	{
		renderSetting->refreshAll();
		renderSetting->refresh_later = false;
	}

	if(widgetChanged)
		widgetChanged = false;
	isRendering = false;
}


void RenderWidget::on_timeout()
{
	update();
}

void RenderWidget::cleanup()
{

}

RenderWidget& RenderWidget::getSceneWidget()
{
	return *sceneWidget;
}

RenderWidget& RenderWidget::getGameWidget()
{
	return *gameWidget;
}

RenderWidget& RenderWidget::getCurrentWidget()
{
	return *currentWidget;
}

void RenderWidget::renderFbo() 
{
	if (fbo == nullptr)
	{
		// 创建一个 FBO，大小为窗口大小乘以设备像素比
		qreal dpr = qApp->primaryScreen()->devicePixelRatio();
		auto res = GameEngine::get_instance().get_resolution();

		//设定分辨率就无需再次缩放
		QSize scaledSize = QSize(res.x, res.y);//*dpr;
		Debug::logInfo() << "FBO size: " << scaledSize.width()<<","<<scaledSize.height()<<"\n";
		fbo = new QOpenGLFramebufferObject(scaledSize);
	}
	fbo->bind();
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, fbo->width(), fbo->height());

	glActiveTexture(GL_TEXTURE0);
	
	if (!isGameWidget && GameEngine::get_instance().getInEditor())
		renderScene(mCamera);
	else
	{
		if (auto camera = SceneMgr::get_instance().get_main_camera();camera!=nullptr && camera->get_enabled() && camera->gameObject->is_active())
			renderScene(camera);
	}

	
	//if (frameCount % 200 == 0)
	//{
	//	QString filePath = "E:/Pictures/imageFbo.png";
	//	// 将渲染结果保存到 QImage 中
	//	QImage image = fbo->toImage();
	//	if (image.save(filePath, "PNG")) {
	//		qDebug() << "Image saved successfully!";
	//	}
	//	else {
	//		qDebug() << "Error saving image!";
	//	}
	//}
	
	

	fbo->release();
}

void RenderWidget::renderFboOverlay()
{
	// render overlay
	if (fboOverlay == nullptr)
	{
		// 创建一个 FBO，大小为窗口大小乘以设备像素比
		qreal dpr = qApp->primaryScreen()->devicePixelRatio();

		//设定分辨率就无需再次缩放
		auto res = GameEngine::get_instance().get_resolution();
		QSize scaledSize = QSize(res.x, res.y);//*dpr;
		fboOverlay = new QOpenGLFramebufferObject(scaledSize);
	}
	fboOverlay->bind();
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, fboOverlay->width(), fboOverlay->height());

	auto& cameras = SceneMgr::get_instance().cameras;

	if (isGameWidget)
	{
		for (auto camera : cameras)
		{
			if (camera->get_enabled() && camera->gameObject->is_active() && camera->is_overlay())
			{
				renderScene(camera);
			}

		}
	}

	

	fboOverlay->release();

	/*
	if (frameCount % 200 == 0)
	{
		QString filePath = "E:/GroupProject/github/Pure-Handmade-Small-Workshop/output/imageFboOverlay.png";
		// 将渲染结果保存到 QImage 中
		QImage image = fbo->toImage();
		if (image.save(filePath, "PNG")) {
			qDebug() << "Image saved successfully!";
		}
		else {
			qDebug() << "Error saving image!";
		}
	}
	*/

}

void RenderWidget::mixTexture()
{
	float vertices[] = 
	{
		// positions      // texture coords
		1.f,  1.f, 0,     1.0f, 1.0f,   // top right
		1.f, -1.f, 0,     1.0f, 0.0f,   // bottom right
	   -1.f, -1.f, 0,     0.0f, 0.0f,   // bottom left
	   -1.f,  1.f, 0,     0.0f, 1.0f    // top left
	};

	unsigned int indices[] = 
	{ 
		0, 3, 2, // first triangle
		0, 2, 1 // second triangle
	};
	if (!vaoTexture)
	{
		vaoTexture = std::make_unique<QOpenGLVertexArrayObject>();
		vaoTexture->create();
		vaoTexture->bind();

		vboTexture = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::VertexBuffer);
		vboTexture->create();
		vboTexture->bind();
		vboTexture->allocate(vertices, sizeof(vertices));

		iboTexture = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::IndexBuffer);
		iboTexture->create();
		iboTexture->bind();
		iboTexture->allocate(indices, sizeof(indices));

		textureShaderProgram->bind();
		GLint posLocation = textureShaderProgram->attributeLocation("aPos");
		GLint textureLocation = textureShaderProgram->attributeLocation("aTexCoord");

		auto stride = 5 * sizeof(float);
		glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		glEnableVertexAttribArray(posLocation);

		if (textureLocation)
		{
			//------------------Texture-----------------------//		
			glVertexAttribPointer(textureLocation, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(textureLocation);
		}
		vaoTexture->release();
	}
	vaoTexture->bind();

	textureShaderProgram->bind();
	textureShaderProgram->setUniformValue("texture1", 1);
	textureShaderProgram->setUniformValue("texture2", 2);

	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, fboOverlay->texture());
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, fbo->texture());

	//enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//enable depth test
	glDepthFunc(GL_LEQUAL);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int width = this->width();
	int height = this->height();
	qreal dpr = qApp->primaryScreen()->devicePixelRatio();
	width = width * dpr;
	height = height * dpr;
	auto res = GameEngine::get_instance().get_resolution();
	float ratio = res.x / res.y;
	if (width > height * ratio)
	{
		auto newWidth = height * ratio;
		glViewport((width - newWidth) / 2, 0, newWidth, height);
	}
	else
	{
		auto newHeight = width / ratio;
		glViewport(0, (height - newHeight) / 2, width, newHeight);
	}

	//draw
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	vaoTexture->release();
	return;
}

void RenderWidget::createALLShaderProgram()
{
	// image
	bool success;
	imageShaderProgram = std::make_unique<QOpenGLShaderProgram>();
	imageShaderProgram->create();
	imageShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, QString::fromStdString(source_path + "\\shaders\\commonShaders.vert"));
	imageShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, QString::fromStdString(source_path + "\\shaders\\commonShaders.frag"));
	success = imageShaderProgram->link();
	if (!success)
		qDebug() << "ERR:" << imageShaderProgram->log();
	
	
	shaderOffsetBinding = imageShaderProgram->uniformLocation("offset");
	shaderSizeBinding = imageShaderProgram->uniformLocation("size");
	imageTextureBinding = 0;
	


	// box collider
	bool successBox;
	boxColliderShaderProgram = std::make_unique<QOpenGLShaderProgram>();
	boxColliderShaderProgram->create();
	boxColliderShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, QString::fromStdString(source_path + "\\shaders\\commonShaders.vert"));
	boxColliderShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, QString::fromStdString(source_path + "\\shaders\\commonShaders.frag"));
	successBox = boxColliderShaderProgram->link();
	if (!successBox)
		qDebug() << "ERR:" << boxColliderShaderProgram->log();
	

	// camera border
	bool successCamera;
	cameraBorderShaderProgram = std::make_unique<QOpenGLShaderProgram>();
	cameraBorderShaderProgram->create();
	cameraBorderShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, QString::fromStdString(source_path + "\\shaders\\commonShaders.vert"));
	cameraBorderShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, QString::fromStdString(source_path + "\\shaders\\commonShaders.frag"));
	successCamera = cameraBorderShaderProgram->link();
	if (!successCamera)
		qDebug() << "ERR:" << cameraBorderShaderProgram->log();
	
}



void RenderWidget::createTextProgram() 
{
	bool success;
	textShaderProgram = std::make_unique<QOpenGLShaderProgram>();
	textShaderProgram->create();
	textShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, QString::fromStdString(source_path + "\\shaders\\textShaders.vert"));
	textShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, QString::fromStdString(source_path + "\\shaders\\textShaders.frag"));
	success = textShaderProgram->link(); //
	if (!success)
		qDebug() << "ERR:" << textShaderProgram->log();
	shaderOffsetBinding = textShaderProgram->uniformLocation("offset");
	shaderSizeBinding = textShaderProgram->uniformLocation("size");
}

void RenderWidget::createTextureProgram()
{
	bool success;
	textureShaderProgram = std::make_unique<QOpenGLShaderProgram>();
	textureShaderProgram->create();
	textureShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, QString::fromStdString(source_path + "\\shaders\\mixTexttureShader.vert"));
	textureShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, QString::fromStdString(source_path + "\\shaders\\mixTexttureShader.frag"));
	success = textureShaderProgram->link();
	if (!success)
		qDebug() << "ERR:" << textureShaderProgram->log();

}


void RenderWidget::messageLogHandler(const QOpenGLDebugMessage& debugMessage)
{
	qDebug() << debugMessage.message();
}


void RenderWidget::on_widgetChanged(int index)
{
	widgetChanged = true;
	if (index == 0)
	{
		currentWidget = sceneWidget;
		Debug::logInfo() << "widget changed, current widget is scene\n";
	}		
	else
	{
		currentWidget = gameWidget;
			Debug::logInfo() << "widget changed, current widget is game\n";
	}	
}

// text render 
QOpenGLTexture *RenderWidget::genTextTexture(int width, int height, const QString& text, int textPixelSize, const QColor& textColor)
{
	// 创建一个 2D 纹理对象
	QOpenGLTexture* texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
	// 创建一张空的 QImage，格式为 ARGB32_Premultiplied
	QImage img(width, height, QImage::Format_ARGB32_Premultiplied);
	// 将图片填充为完全透明的黑色
	img.fill(QColor(0, 0, 0, 0));
	// 创建一个 QPainter，用于在 QImage 上绘制文字
	QPainter painter;
	// 开始在 QImage 上绘制
	painter.begin(&img);
	// 设置字体大小
	QFont font;
	font.setPixelSize(textPixelSize);
	// 设置 QPainter 使用的字体
	painter.setFont(font);
	// 创建一个画笔，用于绘制文字颜色
	QPen pen;
	pen.setColor(textColor);
	painter.setPen(pen);
	// 设置文字的对齐方式为左上对齐
	QTextOption option(Qt::AlignLeft | Qt::AlignTop);
	// 设置文字的换行模式为不换行
	option.setWrapMode(QTextOption::NoWrap);
	// 设置绘制文字的矩形区域
	QRectF rect(0, 0, width, height);
	// 在 QImage 上绘制文字
	painter.drawText(rect, text, option);
	// 停止在 QImage 上绘制
	painter.end();
	// 将 QImage 的数据绑定到纹理上
	texture->setData(img);
	// 设置纹理的放大和缩小过滤器为线性过滤
	texture->setMinificationFilter(QOpenGLTexture::Linear);
	texture->setMagnificationFilter(QOpenGLTexture::Linear);
	// 设置纹理的重复模式为 Repeat
	texture->setWrapMode(QOpenGLTexture::Repeat);
	// 返回生成的纹理对象指针
	return texture;
}

void RenderWidget::mouseMoveEvent(QMouseEvent* event)
{
	
	if (!SceneMgr::get_instance().hasCurrentScene() || isGameWidget || !GameEngine::get_instance().getInEditor())
		return;
	Vector2D screenPos = Vector2D(event->localPos().x(),size().height() - event->localPos().y());
	//Debug::log("mouse position: " + pos.tostring());
	//if(SceneMgr::get_instance().get_main_camera()!=nullptr)
		//Debug::log("camera position: "+SceneMgr::get_instance().get_main_camera()->screenToWorld(pos).tostring());
	
	auto selectedGameObject = getSelectedGameObject();
	QMatrix4x4 matrix;
	
	if (sizeChangeMode == false && selectedGameObject != nullptr && selectedGameObject->getComponent<IBoxResizable>() != nullptr)
	{
		matrix = mCamera->CalculateProjectionMulViewMatrix();
		auto transform = selectedGameObject->transform;
		matrix.translate(transform->getWorldPosition().toQVector3D());
		matrix.rotate(transform->getWorldRotation(), QVector3D(0.f, 0.f, 1.f));
		matrix.scale(transform->getWorldScale().toQVector3D(1.0f));

		auto resolution = GameEngine::get_instance().get_resolution();

		Vector2D center = Vector2D(0.f, 0.f);

		auto box = selectedGameObject->getComponent<IBoxResizable>();

		
		auto leftX = box->borderVertices[2].position[0];
		auto rightX = box->borderVertices[0].position[0];

		auto topY = box->borderVertices[0].position[1];
		auto bottomY = box->borderVertices[2].position[1];

		leftX = (matrix * QVector4D(leftX, 0.f, 0.f, 1.f)).toVector2DAffine().x();
		rightX = (matrix * QVector4D(rightX, 0.f, 0.f, 1.f)).toVector2DAffine().x();
		topY = (matrix * QVector4D(0.f, topY, 0.f, 1.f)).toVector2DAffine().y();
		bottomY = (matrix * QVector4D(0.f, bottomY, 0.f, 1.f)).toVector2DAffine().y();
		
		// 根据分辨率换算
		leftX = leftX * resolution.x / 2.f + resolution.x / 2.f;
		rightX = rightX * resolution.x / 2.f + resolution.x / 2.f;
		topY = topY * resolution.y / 2.f + resolution.y / 2.f;
		bottomY = bottomY * resolution.y / 2.f + resolution.y / 2.f;
	
		int borderSize = 5; // 边框大小

		// 检查鼠标是否在渲染框的边框上
		if (screenPos.x <= leftX + borderSize && screenPos.x >= leftX - borderSize)
		{
			if (screenPos.y <= topY + borderSize && screenPos.y >= topY - borderSize)// 左上角
			{
				setCursor(Qt::SizeFDiagCursor);
				connerNumber = 1;
				edgeNumber = 0;
				return;
			}
			else if (screenPos.y <= bottomY + borderSize && screenPos.y >= bottomY - borderSize) // 左下角
			{
				setCursor(Qt::SizeBDiagCursor);
				connerNumber = 4;
				edgeNumber = 0;
				return;
			}
			else // 左边
			{
				if (screenPos.y <= topY && screenPos.y >= bottomY)
				{
					setCursor(Qt::SizeHorCursor);
					edgeNumber = 1;
					connerNumber = 0;
					return;
				}	
			}
		}
		else if (screenPos.x <= rightX + borderSize && screenPos.x >= rightX - borderSize) // 右边
		{
			if (screenPos.y <= topY + borderSize && screenPos.y >= topY - borderSize) // 右上角
			{
				setCursor(Qt::SizeBDiagCursor);
				connerNumber = 2;
				
				edgeNumber = 0;
				return;
			}
			else if (screenPos.y <= bottomY + borderSize && screenPos.y >= bottomY - borderSize) // 右下角
			{
				setCursor(Qt::SizeFDiagCursor);
				connerNumber = 3;
				
				edgeNumber = 0;
				return;
			}
			else // 右边
			{
				if (screenPos.y <= topY && screenPos.y >= bottomY)
				{
					setCursor(Qt::SizeHorCursor);
					edgeNumber = 3;
					//Debug::logInfo("right edge");
					connerNumber = 0;
					return;
				}				
			}
		}
		else if (screenPos.y <= topY + borderSize && screenPos.y >= topY - borderSize && screenPos.x <= rightX && screenPos.x >= leftX) // 上边
		{
			setCursor(Qt::SizeVerCursor);
			edgeNumber = 2;
			connerNumber = 0;
			return;
		}
		else if (screenPos.y <= bottomY + borderSize && screenPos.y >= bottomY - borderSize && screenPos.x <= rightX && screenPos.x >= leftX) // 下边
		{
			setCursor(Qt::SizeVerCursor);
			edgeNumber = 4;
			connerNumber = 0;
			return;
		}
		else // 不在边框上
		{
			connerNumber = 0;
			edgeNumber = 0;
			// Debug::logInfo("no \n");
			setCursor(Qt::ArrowCursor);
		}

	}

	



	if (event->buttons() & Qt::LeftButton) 
	{
		if (moveObjectMode)	
		{
			QPoint diff = event->pos() - lastMovePos;
			float ratio = mCamera->get_view_width() / size().width();
			Vector2D diff2 = Vector2D(diff.x(), -diff.y()) * ratio;



			selectedGameObject->transform->translate(diff2);
			lastMovePos = event->pos();
			update();
		}

		else if (sizeChangeMode)
		{
			auto box = selectedGameObject->getComponent<IBoxResizable>();
			auto oldSize = box->get_size();
			QPoint diff = event->pos() - lastMovePos;
			float ratio = mCamera->get_view_width() / size().width();
			Vector2D diff2 = Vector2D(diff.x(), -diff.y()) * ratio;

			auto diffX = diff2.x * 2.0;
			auto diffY = diff2.y * 2.0;
			float ratio2 = box->get_size().x / box->get_size().y;
			auto diffFX = 0;
			auto diffFY = 0;

			if (fixedRatioMode == true)
			{
				if (diffX > diffY)
				{
					diffFX = diffX;
					diffFY = diffX / ratio2;
				}
				else
				{
					diffFY = diffY;
					diffFX = diffY * ratio2;
				}

				if (connerNumber == 1) // 左上角
				{
					box->set_size(Vector2D(oldSize.x + diffFX, oldSize.y + diffFY));
				}

				else if (connerNumber == 2) // 右上角
				{
					box->set_size(Vector2D(oldSize.x + diffFX, oldSize.y + diffFY));
				}
				else if (connerNumber == 4) // 左下角
				{
					box->set_size(Vector2D(oldSize.x - diffFX, oldSize.y - diffFY));
				}
				else if (connerNumber == 3) // 右下角
				{
					box->set_size(Vector2D(oldSize.x + diffFX, oldSize.y - diffFY));
				}
				else if (edgeNumber == 1) // 左边
				{
					box->set_size(Vector2D(oldSize.x - diffFX, oldSize.y));
				}
				else if (edgeNumber == 3) // 右边
				{
					box->set_size(Vector2D(oldSize.x + diffFX, oldSize.y));
				}
				else if (edgeNumber == 2) // 上边
				{
					box->set_size(Vector2D(oldSize.x, oldSize.y + diffFY));
				}
				else if (edgeNumber == 4) // 下边
				{
					box->set_size(Vector2D(oldSize.x, oldSize.y - diffFY));
				}
			}
			
			else
			{
				if (connerNumber == 1) //左上角
				{
					box->set_size(Vector2D(oldSize.x - diffX, oldSize.y + diffY));
				}
				else if (connerNumber == 2) // 右上角
				{
					box->set_size(Vector2D(oldSize.x + diffX, oldSize.y + diffY));
				}
				else if (connerNumber == 4) // 左下角
				{
					box->set_size(Vector2D(oldSize.x - diffX, oldSize.y - diffY));
				}
				else if (connerNumber == 3) // 右下角
				{
					box->set_size(Vector2D(oldSize.x + diffX, oldSize.y - diffY));
				}
				else if (edgeNumber == 1) // 左边
				{
					box->set_size(Vector2D(oldSize.x - diffX, oldSize.y));
				}
				else if (edgeNumber == 3) // 右边
				{
					box->set_size(Vector2D(oldSize.x + diffX, oldSize.y));
				}
				else if (edgeNumber == 2) // 上边
				{
					box->set_size(Vector2D(oldSize.x, oldSize.y + diffY));
				}
				else if (edgeNumber == 4) // 下边
				{
					box->set_size(Vector2D(oldSize.x, oldSize.y - diffY));
				}
			}
			
			lastMovePos = event->pos();
			update();
		}

	}
	else if (event->buttons() & Qt::RightButton)
	{
		if (!moveCameraMode)
			return;

		QPoint diff = event->pos() - lastPos;

		float ratio = mCamera->get_view_width() / size().width();
		
		Vector2D diff2 = Vector2D(-diff.x(), diff.y()) * ratio;

		mCameraObject->transform->translate(diff2);
		lastPos = event->pos();
		update();
	}

}


void RenderWidget::mousePressEvent(QMouseEvent* event)
{
	if (!SceneMgr::get_instance().hasCurrentScene() || isGameWidget || !GameEngine::get_instance().getInEditor())
		return;

	if (event->button() == Qt::RightButton)
	{
		if (!SceneMgr::get_instance().hasCurrentScene())
			return;
		lastPos = event->pos();
		moveCameraMode = true;
	}

	if (event->button() == Qt::LeftButton)
	{
		if(!SceneMgr::get_instance().hasCurrentScene())
			return;
		
		lastMovePos = event->pos();

		Vector2D pos = Vector2D(event->localPos().x(), size().height() - event->localPos().y());
		auto selectedGameObjects = hitRay(pos);

		auto it = std::find(selectedGameObjects.begin(), selectedGameObjects.end(), getSelectedGameObject());
		
		if (it != selectedGameObjects.end())
		{
			selectedGameObjects.erase(it);
			moveObjectMode = true;
		}

		// size change
		if (edgeNumber != 0 || connerNumber != 0)
		{
			sizeChangeMode = true;
			moveObjectMode = false;
		}
		else
		{
			sizeChangeMode = false;
		}
			

	}
}

void RenderWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (!SceneMgr::get_instance().hasCurrentScene() || isGameWidget || !GameEngine::get_instance().getInEditor())
		return;

	if (event->button() == Qt::LeftButton)
	{
		moveObjectMode = false;
		sizeChangeMode = false;
		setCursor(Qt::ArrowCursor);
	}
	
	if (event->button() == Qt::RightButton)
	{
		moveCameraMode = false;
	}

}

void RenderWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
	if (!SceneMgr::get_instance().hasCurrentScene()||isGameWidget||!GameEngine::get_instance().getInEditor())
		return;

	if (event->button() == Qt::LeftButton)
	{
		Vector2D pos = Vector2D(event->localPos().x(), size().height() - event->localPos().y());
		
		/*
		Debug::log("\n""mouse position:" + pos.tostring());
		if (SceneMgr::get_instance().get_main_camera() != nullptr)
			Debug::log("\n""camera position:" + mCamera->screenToWorld(pos).tostring());
		*/	

		auto selectedGameObjects = hitRay(pos);

		if (selectedGameObjects.size() == 0)
		{
			hierarchyWidget->setCurrentItem(nullptr);
			return;
		}
		GameObject* gameObject = nullptr;
		int layer = 0;
		int index = 0;
		for (auto obj : selectedGameObjects)
		{
			if (auto renderer = obj->getComponent<Renderer>();obj->getComponent<Camera>() == nullptr &&  renderer!=nullptr)
			{
				if (auto order = renderer->get_render_layer();order > layer)
				{
					layer = order;
					index = renderer->get_render_order();
					gameObject = obj;
					
				}
				else if (renderer->get_render_layer() == layer)
				{
					if (auto order = renderer->get_render_order();order > index)
					{
						gameObject = obj;
						index = order;
					}
				}
			}
		}
		if (gameObject != nullptr)
		{
			hierarchyWidget->setCurrentItem(hierarchyWidget->gameobj_item_map[gameObject]);

		}
			
		else
			hierarchyWidget->setCurrentItem(nullptr);
	}
}




void RenderWidget::keyPressEvent(QKeyEvent* event)
{
	if (!GameEngine::get_instance().getInEditor())
	{
		if (event->key() == Qt::Key_Return && event->modifiers() == Qt::AltModifier) {

			if (!isFullScreen)
			{
				emit resizeGameWindow(isFullScreen);
				isFullScreen = true;
			}
			else
			{
				emit resizeGameWindow(isFullScreen);
				isFullScreen = false;
			}
		}
	}

	if (!SceneMgr::get_instance().hasCurrentScene() || isGameWidget || !GameEngine::get_instance().getInEditor())
		return;
	switch (event->key()) {
       
	case Qt::Key_Shift: fixedRatioMode = true; break;

    default:
        break;
    }
}

void RenderWidget::keyReleaseEvent(QKeyEvent* event)
{
	if (!SceneMgr::get_instance().hasCurrentScene() || isGameWidget || !GameEngine::get_instance().getInEditor())
		return;
	switch (event->key()) {
	case Qt::Key_Shift: fixedRatioMode = false; break;
	default:
		break;
	}
}

void RenderWidget::wheelEvent(QWheelEvent* event)
{
	if (!SceneMgr::get_instance().hasCurrentScene() || isGameWidget || !GameEngine::get_instance().getInEditor())
		return;

	if (event->angleDelta().y() > 0)
		mCamera->set_view_width(mCamera->get_view_width() * 0.9f);
	else
		mCamera->set_view_width(mCamera->get_view_width() * 1.1f);
}

void RenderWidget::contextMenuEvent(QContextMenuEvent* event)
{
	event->accept();
}

std::vector<GameObject*> RenderWidget::hitRay(Vector2D screenPos)
{
	std::vector<GameObject*> selectedGameObjects;

	auto cameraPos = mCamera->screenToWorld(screenPos);

	for (auto& gameObject : SceneMgr::get_instance().get_current_scene()->getAllGameObjs())
	{
		QMatrix4x4 matrix;
		auto transform = gameObject.second->transform;
		matrix.translate(transform->getWorldPosition().toQVector3D());
		matrix.rotate(transform->getWorldRotation(), QVector3D(0.f, 0.f, 1.f));
		matrix.scale(transform->getWorldScale().toQVector3D(1.0f));

		Vector2D center = Vector2D(0.f, 0.f);
		if (auto box = gameObject.second->getComponent<IBoxResizable>(); box != nullptr)
		{
			auto leftX = box->borderVertices[2].position[0];
			auto rightX = box->borderVertices[0].position[0];

			auto topY = box->borderVertices[0].position[1];
			auto bottomY = box->borderVertices[2].position[1];

			leftX = (matrix * QVector4D(leftX, 0.f, 0.f, 1.f)).toVector2DAffine().x();
			rightX = (matrix * QVector4D(rightX, 0.f, 0.f, 1.f)).toVector2DAffine().x();
			topY = (matrix * QVector4D(0.f, topY, 0.f, 1.f)).toVector2DAffine().y();
			bottomY = (matrix * QVector4D(0.f, bottomY, 0.f, 1.f)).toVector2DAffine().y();

			if (cameraPos.x >= leftX && cameraPos.x <= rightX && cameraPos.y >= bottomY && cameraPos.y <= topY)
			{
				selectedGameObjects.push_back(gameObject.second);
			}
		}
	}
	return selectedGameObjects;

}

void RenderWidget::lookAt(GameObject* target)
{
	if(mCamera !=nullptr)
		mCamera->lookAt(target->transform->getWorldPosition());
}

void RenderWidget::resetFBO()
{
	if (fbo != nullptr)
	{
		fbo->release();
		delete fbo;
		fbo = nullptr;
	}

	if (fboOverlay != nullptr)
	{
		fboOverlay->release();
		delete fboOverlay;
		fboOverlay = nullptr;
	}
}


GameObject* RenderWidget::getSelectedGameObject()
{
	return hierarchyWidget->selectedGameObject;	
}

void RenderWidget::setFullScreen(bool fullScreen)
{
	if (fullScreen) {
		// 记录窗口化时的位置和大小
		m_normalGeometry = geometry();
		// 进入全屏模式
		setWindowState(windowState() | Qt::WindowFullScreen);
	}
	else {
		// 退出全屏模式
		setWindowState(windowState() & ~Qt::WindowFullScreen);
		// 恢复窗口化时的位置和大小
		setGeometry(m_normalGeometry);
	}
}

void RenderWidget::shadowTest(IBoxResizable* target, Camera* camera, bool visBorder, QVector2D lightPos)
{
	
	QVector2D targetPos; // 目标位置
	QVector2D targetLeftBottom; // 目标左下角
	QVector2D targetRightBottom; // 目标右下角
	

	// mvp matrix
	if (camera == nullptr)
		camera = mCamera;
	auto matrix = camera->CalculateProjectionMulViewMatrix();
	auto transform = target->gameObject->transform;
	matrix.translate(transform->getWorldPosition().toQVector3D());
	matrix.rotate(transform->getWorldRotation(), QVector3D(0.f, 0.f, 1.f));
	matrix.scale(transform->getWorldScale().toQVector3D(1.0f));

	targetPos = target->gameObject->transform->getWorldPosition().toQVector2D();
	
	// 物体高度
	auto height = target->borderVertices[0].position[1] - target->borderVertices[1].position[1];

	targetLeftBottom = QVector2D(target->vertices[2].position[0], target->vertices[2].position[1]);
	targetRightBottom = QVector2D(target->vertices[1].position[0], target->vertices[1].position[1]);

	QVector2D lightDir = lightPos - targetPos; // 光源方向

	QMatrix4x4 newMatrix;
	newMatrix.translate(transform->getWorldPosition().toQVector3D());
	newMatrix.rotate(transform->getWorldRotation(), QVector3D(0.f, 0.f, 1.f));
	newMatrix.scale(transform->getWorldScale().toQVector3D(1.0f));

	targetLeftBottom = (newMatrix * QVector4D(targetLeftBottom, 0.f, 1.f)).toVector2DAffine();
	targetRightBottom = (newMatrix * QVector4D(targetRightBottom, 0.f, 1.f)).toVector2DAffine();

	// 判断光在上边还是下边
	bool isUp = lightDir.y() > 0;
	// 判断光在左边还是右边
	bool isLeft = lightDir.x() < 0;

	// 计算光源到目标的距离
	float distance = lightDir.length();
	// 计算光源到目标的角度
	float angle = acosf(lightDir.x() / distance);

	

	// 通过光源到目标的距离来计算阴影的高度
	float shadowHeight = distance * tanf(angle);

	
	if (qAbs(shadowHeight) > 2 * height)
	{
		if(shadowHeight < 0)
		{ 
			shadowHeight = -2 * height;
			distance = shadowHeight / tanf(angle);
		}
		else
		{
			shadowHeight = 2 * height;
			distance = shadowHeight / tanf(angle);
		}
		distance = qAbs(distance);
	}

	
	
	QVector2D shadowLeftTop;
	QVector2D shadowRightTop;
	
	// 计算阴影的顶点
	if (isLeft)
	{
		shadowLeftTop = targetLeftBottom + QVector2D(distance, isUp ? shadowHeight : -shadowHeight);
		shadowRightTop = targetRightBottom + QVector2D(distance, isUp ? shadowHeight : -shadowHeight);
	}
	else
	{
		shadowLeftTop = targetLeftBottom + QVector2D(-distance, isUp ? -shadowHeight : shadowHeight);
		shadowRightTop = targetRightBottom + QVector2D(-distance, isUp ? -shadowHeight : shadowHeight);
	}

	

	QVector2D shadowLeftBottom = targetLeftBottom;
	QVector2D shadowRightBottom = targetRightBottom;

	
	std::vector<Vertex> vertices = {
		Vertex{{shadowRightTop.x(), shadowRightTop.y(), 0.0f},{1.0f, 1.0f}},		// top right
		Vertex{{shadowRightBottom.x(), shadowRightBottom.y(), 0.0f},{1.0f, 0.0f}},	// bottom right
		Vertex{{shadowLeftBottom.x(), shadowLeftBottom.y(), 0.0f},{0.0f, 0.0f}},	// bottom left
		Vertex{{shadowLeftTop.x(), shadowLeftTop.y(), 0.0f},{0.0f,1.0f}}			// top left
	};

	std::vector<unsigned int> indices = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};


	static auto vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	vbo->create();
	vbo->bind();
	vbo->allocate(vertices.data(), static_cast<int>(vertices.size() * sizeof(Vertex)));
	vbo->setUsagePattern(QOpenGLBuffer::DynamicDraw);
	static auto ibo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
	ibo->create();
	ibo->bind();
	ibo->allocate(indices.data(), static_cast<int>(indices.size() * sizeof(unsigned int)));
	ibo->setUsagePattern(QOpenGLBuffer::DynamicDraw);
	imageShaderProgram->bind();

	

	GLint posLocation = imageShaderProgram->attributeLocation("aPos");
	GLint texCoordLocation = imageShaderProgram->attributeLocation("aTexCoord");

	GLsizei stride = sizeof(Vertex);
	if (posLocation != -1)
	{
		glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		glEnableVertexAttribArray(posLocation);
	}
	if (texCoordLocation != -1)
	{
		glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(texCoordLocation);
	}

	
	auto view_width = camera->get_view_width();
	//imageShaderProgram->bind();

	imageShaderProgram->setUniformValue("isWorldPos", true);
	imageShaderProgram->setUniformValue("isShadow", true); 
	imageShaderProgram->setUniformValue("isLighting", true);

	QMatrix4x4 projMatrix;
	auto resolution = GameEngine::get_instance().get_resolution();
	float ratio = resolution.y / resolution.x;
	projMatrix.ortho(-view_width / 2, view_width / 2, -view_width / 2 * ratio, view_width / 2 * ratio, -10.f, 10.f);
	imageShaderProgram->setUniformValue("projMatrix", projMatrix);

	QMatrix4x4 Matrix = QMatrix4x4();
	auto cameraWorldPos = camera->gameObject->transform->getWorldPosition();
	Matrix.rotate(camera->gameObject->transform->getWorldRotation(), 0.f, 0.f, 1.f);
	QMatrix4x4 viewMatrix = QMatrix4x4();
	viewMatrix.lookAt(cameraWorldPos.toQVector3D(1.f), cameraWorldPos.toQVector3D(-1.f), Matrix * QVector4D(0.f, 1.f, 0.f, 1.f).toVector3D());
	imageShaderProgram->setUniformValue("viewMatrix", viewMatrix);




	const auto& color = target->get_color();
	imageShaderProgram->setUniformValue("MVPMatrix", matrix);
	imageShaderProgram->setUniformValue("color", color.red(), color.green(), color.blue(), color.alpha());
	imageShaderProgram->setUniformValue("isTexture", true);
	imageShaderProgram->setUniformValue("isLighting", true);

	

	//texture
	if (target->isTextureValid())
		target->get_texture()->bind();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(target->indices.size()), GL_UNSIGNED_INT, 0);


	
	//draw border
	if (visBorder)
	{
		std::vector<unsigned int> borderindices = {
		0, 1, 1, 2, 2, 3, 3, 0
		};


		static auto borderibo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
		borderibo->create();
		borderibo->bind();
		borderibo->allocate(borderindices.data(), static_cast<int>(borderindices.size() * sizeof(unsigned int)));
		borderibo->setUsagePattern(QOpenGLBuffer::DynamicDraw);



		GLint posLocation = imageShaderProgram->attributeLocation("aPos");

		GLuint stride = sizeof(Vertex);
		glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		glEnableVertexAttribArray(posLocation);

		imageShaderProgram->setUniformValue("isTexture", false);
		imageShaderProgram->setUniformValue("isLighting", false);
		imageShaderProgram->setUniformValue("color", 1.0f, 1.0f, 1.0f, 1.0f);



		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(1.0f);
		glDrawElements(GL_LINES, static_cast<GLsizei>(borderindices.size()), GL_UNSIGNED_INT, 0);
		borderibo->release();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	}

	vbo->release();
	
	imageShaderProgram->setUniformValue("isWorldPos", false);
	imageShaderProgram->setUniformValue("isShadow", false);
	imageShaderProgram->setUniformValue("isLighting", false);
}








