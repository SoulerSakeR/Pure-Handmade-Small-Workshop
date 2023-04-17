#include "renderwidget.h"


#include <iostream>
#include <QPainter>
#include <Core/SystemStatus/GameEngine.h>
#include "Core/Core/Camera.h"
#include <QElapsedTimer>
#include <Core/ResourceManagement/SceneMgr.h>
#include "qevent.h"

RenderWidget* RenderWidget::instance = nullptr;
std::string source_path;
static int count = 0;


/*
float vertices[] = {
	// positions   // colors           // texture coords
	1.f,  1.f, 0,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   1.f,  1.f, 0, // top right
	1.f, -1.f, 0,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   1.f, -1.f, 0, // bottom right
   -1.f, -1.f, 0,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  -1.f, -1.f, 0, // bottom left
   -1.f,  1.f, 0,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f   -1.f,  1.f, 0, // top left
};
*/

// text render 
static const char* vertexShaderSource =
"attribute highp vec4 posAttr;\n"
"attribute highp vec2 texcoord;\n"
"varying highp vec2 v_texcoord;\n"
"uniform highp mat4 matrix;\n"
"uniform highp mat4 projMatrix;\n"
"void main() {\n"
"   v_texcoord = texcoord;\n"
"   gl_Position = posAttr;\n"
"}\n";

static const char* fragmentShaderSource =
"varying highp vec2 v_texcoord;\n"
"uniform sampler2D texture;\n"
"void main() {\n"
"   highp vec4 texColor = texture2D(texture, v_texcoord);\n"
"   highp float alpha = texColor.a;\n"
"   if(alpha < 0.2)\n"
"       discard;\n"
"   highp vec3 color = vec3(1.0,1.0,1.0);\n"
"   gl_FragColor = texColor * vec4(color, 1.0);\n"
"}\n";



float verticesText[] =
{    
	 // positions          // texture coords
	 1.f,  1.f, 0,   1.0f, 1.0f,	// top right
	 1.f, -1.f, 0,   1.0f, 0.0f,	// bottom right
	-1.f, -1.f, 0,   0.0f, 0.0f, 	// bottom left
	-1.f,  1.f, 0,   0.0f, 1.0f	// top left
};


float verticesBox[] =
{    // positions        // colors
	 200.f,  200.f, 0.0f,  1.0f, 0.0f, 0.0f,	// top right
	 200.f, -200.f, 0.0f,  0.0f, 1.0f, 0.0f,	// bottom right
	-200.f, -200.f, 0.0f,  0.0f, 0.0f, 1.0f, 	// bottom left
	-200.f,  200.f, 0.0f,  0.5f, 0.5f, 0.5f,	// top left
};




float vertices[] = {
	// positions   // colors           // texture coords
	1.f,  1.f, 0,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
	1.f, -1.f, 0,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
   -1.f, -1.f, 0,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
   -1.f,  1.f, 0,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
};

//counter clockwise


unsigned int indices[] = { // note that we start from 0!
			   0, 3, 2, // first triangle
			   0, 2, 1 // second triangle
};

// 定义需要绘制的边
GLuint indicesBOX[] = { 0, 1, 1, 2, 2, 3, 3, 0 };





RenderWidget::RenderWidget(QWidget* parent) : QOpenGLWidget(parent)
{
	setFocusPolicy(Qt::StrongFocus);
	connect(&timer, SIGNAL(timeout()), this, SLOT(on_timeout()));
	frameCount = 0;
	instance = this;
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


void RenderWidget::renderGameobject(GameObject* gameobj)
{
	if (auto img = gameobj->getComponent<Image>(); img != nullptr && img->get_enabled())
	{
		renderImage(img);
	}
	if (auto boxCollider = gameobj->getComponent<BoxCollider>();boxCollider != nullptr && boxCollider->get_enabled())
	{
		renderBoxCollider(boxCollider);
	}
}

void RenderWidget::renderBoxCollider(BoxCollider* box)
{
	if (box->vao == nullptr)
	{
		box->vao = new QOpenGLVertexArrayObject;
		box->vao->create();
		box->vao->bind();
		box->vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
		box->vbo->create();
		box->vbo->bind();
		box->vbo->allocate(box->vertices, 12*sizeof(float));
		box->ibo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
		box->ibo->create();
		box->ibo->bind();
		box->ibo->allocate(box->indices, 8*sizeof(unsigned int));

		boxColliderShaderProgram->bind();
		GLint posLocation = boxColliderShaderProgram->attributeLocation("aPos");
		GLint colorLocation = boxColliderShaderProgram->attributeLocation("aColor");
		auto stride = 3 * sizeof(float);
		//告知显卡如何解析缓冲里的属性值
		glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		//开启VAO管理的第一个属性值
		glEnableVertexAttribArray(posLocation);
		box->vao->release();
		box->vbo->release();
		box->vbo->destroy();
		delete box->vbo;
		box->vbo = nullptr;
		box->ibo->release();
		box->ibo->destroy();
		box->ibo = nullptr;
		delete box->ibo;
	}
	box->vao->bind();
	

	//calculate the MVP matrix
	auto matrix = SceneMgr::get_instance().get_main_camera()->CalculateProjectionMulViewMatrix();
	auto transform = box->gameObject->transform;
	matrix.translate(transform->getWorldPosition().toQVector3D());
	matrix.rotate(transform->getWorldRotation(), QVector3D(0.f, 0.f, 1.f));
	matrix.scale(transform->getWorldScale().toQVector3D(1.0f));

	//transfer the MVP matrix to the shader
	boxColliderShaderProgram->bind();
	boxColliderShaderProgram->setUniformValue("MVPMatrix", matrix);
	boxColliderShaderProgram->setUniformValue("color",box->color.r,box->color.g,box->color.b);

	//set line mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(3.0f);

	//draw
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);
	box->vao->release();
}

void RenderWidget::renderImage(Image* img)
{
	//bind vao if exist
	if (img->vao == nullptr)
	{
		img->vao = new QOpenGLVertexArrayObject;
		img->vao->create();
		img->vao->bind();
		img->vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
		img->vbo->create();
		img->vbo->bind();
		img->vbo->allocate(img->vertices, 36 * sizeof(float));
		img->ibo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
		img->ibo->create();
		img->ibo->bind();
		img->ibo->allocate(img->indices, 6 * sizeof(unsigned int));
		imageShaderProgram->bind();
		
		GLint posLocation = imageShaderProgram->attributeLocation("aPos");
		GLint colorLocation = imageShaderProgram->attributeLocation("aColor");
		GLint textureLocation = imageShaderProgram->attributeLocation("aTexCord");
		
		auto stride = 9 * sizeof(float);
		
		//-----------------position--------------------//
		//告知显卡如何解析缓冲里的属性值
		glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		//开启VAO管理的第一个属性值
		glEnableVertexAttribArray(posLocation);

		if (colorLocation >= 0)
		{
			//------------------Color-----------------------//
			
			glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
			
			glEnableVertexAttribArray(colorLocation);
		}
		if (textureLocation)
		{
			//------------------Texture-----------------------//
			
			glVertexAttribPointer(textureLocation, 2, GL_FLOAT, GL_FALSE, stride, (void*)(7 * sizeof(float)));
			
			glEnableVertexAttribArray(textureLocation);
		}
		img->vao->release();
		img->vbo->release();
		img->vbo->destroy();
		delete img->vbo;
		img->vbo = nullptr;
		/*img->ibo->release();
		img->ibo->destroy();
		img->ibo = nullptr;
		delete img->ibo;*/
	}
	img->vao->bind();

	//calculate the MVP matrix
	auto matrix = SceneMgr::get_instance().get_main_camera()->CalculateProjectionMulViewMatrix();
	auto transform = img->gameObject->transform;
	matrix.translate(transform->getWorldPosition().toQVector3D());
	matrix.rotate(transform->getWorldRotation(), QVector3D(0.f, 0.f, 1.f));
	matrix.scale(transform->getWorldScale().toQVector3D(1.0f));

	//transfer the MVP matrix to the shader
	imageShaderProgram->bind();
	imageShaderProgram->setUniformValue("MVPMatrix", matrix);

	
	/*QString date = QDateTime::currentDateTime().toString("现在时间是：yyyy-MM-dd hh:mm:ss.zzz");
	mTexture = genTextTexture(512, 512, date, 60, Qt::red);
	mTexture->bind();*/
	

	//bind texture
	img->texture->bind();
	img->ibo->bind();
	//enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//enable depth test
	glDepthFunc(GL_LEQUAL);

	//set texture mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//draw
	glDrawElements(GL_TRIANGLES, sizeof(unsigned int)*6, GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	img->ibo->release();
	
	auto indices = new unsigned int[8] {
		0, 1, 1, 2, 2, 3, 3, 0
	};
	img->borderIbo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
	img->borderIbo->create();
	img->borderIbo->bind();
	img->borderIbo->allocate(indices, 8 * sizeof(unsigned int));
	boxColliderShaderProgram->bind();
	boxColliderShaderProgram->setUniformValue("MVPMatrix", matrix);
	glLineWidth(3.0f);
	glDrawElements(GL_LINES, sizeof(unsigned int)*8, GL_UNSIGNED_INT, 0);

	

	img->borderIbo->release();
	delete[] indices;
	//release
	img->vao->release();
	img->texture->release();
}

void RenderWidget::renderScene()
{
	auto scene = SceneMgr::get_instance().get_current_scene();
	if (auto cam = SceneMgr::get_instance().get_main_camera(); scene == nullptr || scene->getRootGameObjs().size() == 0 || cam == nullptr)
		return;
	for (auto& pair : scene->getAllGameObjsByDepth())
	{
		if (pair.second->isActive)
			renderGameobject(pair.second);
	}
}


void RenderWidget::initializeGL()
{
	initializeOpenGLFunctions();

	// debug info
	/*
	logger = std::make_unique<QOpenGLDebugLogger>(this);
	logger->initialize();
	connect(logger.get(), &QOpenGLDebugLogger::messageLogged, this, &RenderWidget::messageLogHandler);
	logger->startLogging();
	*/


	
	source_path = GameEngine::get_instance().getRootPath();

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);


	createProgram();
	createBoxProgram();
	
	createTextProgram();
	textBuffer = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::VertexBuffer);
	

	createIBO();

}

void RenderWidget::resizeGL(int w, int h)
{
	Q_UNUSED(w); Q_UNUSED(h);
	glViewport(0, 0, w, h);
}

void RenderWidget::paintGL()
{
	frameCount++;
	makeCurrent();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	renderScene();
	return;
}


void RenderWidget::on_timeout()
{
	update();
}

void RenderWidget::cleanup()
{

}

RenderWidget& RenderWidget::getInstance()
{
	// TODO: 在此处插入 return 语句
	return *instance;
}

void RenderWidget::createProgram()
{
	bool success;
	imageShaderProgram = std::make_unique<QOpenGLShaderProgram>();
	imageShaderProgram->create();
	imageShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, QString::fromStdString(source_path + "\\shaders\\shaders.vert"));
	imageShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, QString::fromStdString(source_path + "\\shaders\\shaders.frag"));
	success = imageShaderProgram->link(); //
	if (!success)
		qDebug() << "ERR:" << imageShaderProgram->log();
	shaderOffsetBinding = imageShaderProgram->uniformLocation("offset");
	shaderSizeBinding = imageShaderProgram->uniformLocation("size");
	textureWallBinding = 0;
}


void RenderWidget::createBoxProgram() {
	bool success;
	boxColliderShaderProgram = std::make_unique<QOpenGLShaderProgram>();
	boxColliderShaderProgram->create();
	boxColliderShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, QString::fromStdString(source_path + "\\shaders\\boxShader.vert"));	
	boxColliderShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, QString::fromStdString(source_path + "\\shaders\\boxShader.frag"));
	success = boxColliderShaderProgram->link();
	if (!success)
		qDebug() << "ERR:" << boxColliderShaderProgram->log();
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void RenderWidget::createTextProgram() 
{
	bool success;
	shaderTextProgram = std::make_unique<QOpenGLShaderProgram>();
	shaderTextProgram->create();
	shaderTextProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
	shaderTextProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
	success = shaderTextProgram->link();
	if (!success)
		qDebug() << "ERR:" << shaderTextProgram->log();



	
	mPosAttr = shaderTextProgram->attributeLocation("posAttr");
	mTexAttr = shaderTextProgram->attributeLocation("texcoord");
	mMatrixLoc = shaderTextProgram->uniformLocation("matrix");
	mProjLoc = shaderTextProgram->uniformLocation("projMatrix");

}

void RenderWidget::createVAO()
{
	// 找shader的起始位置，并且同时修改下两行的poslocation
	imageShaderProgram->bind();
	GLint posLocation = imageShaderProgram->attributeLocation("aPos");
	GLint colorLocation = imageShaderProgram->attributeLocation("aColor");
	GLint textureLocation = imageShaderProgram->attributeLocation("aTexCord");

	vao = std::make_unique<QOpenGLVertexArrayObject>();

	vao->create();

	vao->bind();
	vbo->bind();
	ibo->bind();
	auto stride = 8 * sizeof(float);

	//-----------------position--------------------//
	
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);

	
	glEnableVertexAttribArray(posLocation);

	if (colorLocation >= 0)
	{
		//------------------Color-----------------------//
		
		glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
		
		glEnableVertexAttribArray(colorLocation);
	}
	if (textureLocation)
	{
		//------------------Texture-----------------------//
		
		glVertexAttribPointer(textureLocation, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
		
		glEnableVertexAttribArray(textureLocation);

	}	
	vbo->release();
	glBindVertexArray(0);
}

void RenderWidget::createBoxVAO() 
{

	boxColliderShaderProgram->bind();
	
	GLint posLocation = boxColliderShaderProgram->attributeLocation("aPos");
	GLint colorLocation = boxColliderShaderProgram->attributeLocation("aColor");

	vaoBox = std::make_unique<QOpenGLVertexArrayObject>();

	vaoBox->create();

	vaoBox->bind();
	//vboBox->bind();
	createBoxVBO();
	createBoxEBO();

	//告知显卡如何解析缓冲里的属性值
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	//开启VAO管理的第一个属性值
	glEnableVertexAttribArray(0);

	//告知显卡如何解析缓冲里的属性值
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//开启VAO管理的第二个属性值
	glEnableVertexAttribArray(1);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	vboBox->release();
	glBindVertexArray(0);
}

void RenderWidget::createVBO()
{
	vbo = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::VertexBuffer);
	vbo->create();
	vbo->bind();
	vbo->allocate(vertices, sizeof(vertices));
}

void RenderWidget::createBoxVBO()
{
	
	vboBox = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::VertexBuffer);
	vboBox->create();
	vboBox->bind();
	vboBox->allocate(verticesBox, sizeof(verticesBox));
}

void RenderWidget::createIBO()
{
	ibo = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::IndexBuffer);
	ibo->create();
	ibo->bind();
	ibo->allocate(indices, sizeof(indices));
}

void RenderWidget::createBoxEBO()
{
	EBOBOX = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::IndexBuffer);
	EBOBOX->create();
	EBOBOX->bind();
	EBOBOX->allocate(indicesBOX, sizeof(indicesBOX));
}

void RenderWidget::messageLogHandler(const QOpenGLDebugMessage& debugMessage)
{
	qDebug() << debugMessage.message();
}
void RenderWidget::renderTexture(QOpenGLTexture* texture, QVector3D offset, QVector2D size)
{

	offset.setZ(0.1f); // 统一深度 不要被挡住了 保证正确混合

	imageShaderProgram->setUniformValue(shaderOffsetBinding, offset);
	imageShaderProgram->setUniformValue(shaderSizeBinding, size);
	vao->bind();
	//ibo->bind();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);

	if (textureWallBinding >= 0)
		texture->bind(textureWallBinding);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(std::size(indices)), GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	vao->release();
	
	
	// 绘制图像尺寸框
	glDisable(GL_BLEND);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(3.0f);

	
	createBoxEBO(); 

	// 绘制矩形边框
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, nullptr);
	
	boxColliderShaderProgram->release();

	
	//====================== text render test ===========================//
	/*
	// 
	textBuffer->create();
	textBuffer->bind();
	textBuffer->allocate(verticesText, sizeof(verticesText) * 6);
	textBuffer->release();
	mVertexCount = 6;

	QString date = QDateTime::currentDateTime().toString("现在时间是：yyyy-MM-dd hh:mm:ss.zzz");
	//QOpenGLTexture* textTexture = genTextTexture(512, 512, date, 32, QColor(255, 255, 255, 255));
	//*textTexture = genTextTexture(512, 512, date, 60, Qt::red);
	
	mTexture = genTextTexture(512, 512, date, 60, Qt::red);

	shaderTextProgram->bind();
	mTexture->bind();
	textBuffer->bind();
	
	
	glVertexAttribPointer(mPosAttr, 3, GL_FLOAT, GL_FALSE, sizeof(verticesText), 0);
	glVertexAttribPointer(mTexAttr, 2, GL_FLOAT, GL_FALSE, sizeof(verticesText), reinterpret_cast<void*>(sizeof(QVector3D)));
	glEnableVertexAttribArray(mPosAttr);
	glEnableVertexAttribArray(mTexAttr);

	glDrawArrays(GL_TRIANGLES, 0, mVertexCount);

	glDisableVertexAttribArray(mTexAttr);
	glDisableVertexAttribArray(mPosAttr);

	textBuffer->release();
	mTexture->release();
	shaderTextProgram->release();
	*/

	//===============================================================//
}


//
void RenderWidget::renderBox()
{
	vaoBox->bind();
	
	//ibo->release(); // 释放画图像的ibo
	//createBoxEBO(); // 重新绑定画盒子的ibo
	//EBOBOX->bind();
	

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(3.0f);

	// 绘制矩形边框
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, nullptr);
	
	boxColliderShaderProgram->release();
}


// text render 
QOpenGLTexture *RenderWidget::genTextTexture(int width, int height, const QString& text, int textPixelSize, const QColor& textColor)
{
	QOpenGLTexture* texture = new QOpenGLTexture(QOpenGLTexture::Target2D);

	QImage img(width, height, QImage::Format_ARGB32_Premultiplied);
	img.fill(QColor(0, 0, 0, 0));

	QPainter painter;

	QFont font;
	painter.begin(&img);
	font.setPixelSize(textPixelSize);
	painter.setFont(font);
	QPen pen;
	pen.setColor(textColor);
	painter.setPen(pen);
	QTextOption option(Qt::AlignLeft | Qt::AlignTop);
	option.setWrapMode(QTextOption::WordWrap);
	QRectF rect(0, 0, width, height);
	painter.drawText(rect, text, option);
	painter.end();
	texture->setData(img);
	//    texture->setMipLevelRange(0, mipLevelMax);    //off mipmap
	texture->setMinificationFilter(QOpenGLTexture::Linear);
	texture->setMagnificationFilter(QOpenGLTexture::Linear);
	texture->setWrapMode(QOpenGLTexture::Repeat);

	return texture;
}

void RenderWidget::mouseMoveEvent(QMouseEvent* event)
{
	Vector2D pos = Vector2D(event->localPos().x(),size().height() - event->localPos().y());
	Debug::log("mouse position: " + pos.tostring());
	if(SceneMgr::get_instance().get_main_camera()!=nullptr)
		Debug::log("camera position: "+SceneMgr::get_instance().get_main_camera()->screenToWorld(pos).tostring());
}






