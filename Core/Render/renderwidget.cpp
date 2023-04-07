#include "renderwidget.h"


#include <iostream>
#include <Core/SystemStatus/GameEngine.h>
#include "Core/Core/Image.h"
#include "Core/Core/Camera.h"
#include <QElapsedTimer>
#include <Core/ResourceManagement/SceneMgr.h>
//#define SOURCE_DIR "E:/GroupProject/PHE(3)/"

RenderWidget* RenderWidget::instance = nullptr;
std::string source_path;
static int count = 0;



float vertices[] = {
	// positions  // colors           // texture coords
	1.f ,  1.f,  0,1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
	1.f , -1.f,  0,0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
	-1.f , -1.f, 0, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
	-1.f ,  1.f, 0, 1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
};

//counter clockwise
unsigned int indices[] = { // note that we start from 0!
			   0, 3, 2, // first triangle
			   0, 2, 1 // second triangle
};


RenderWidget::RenderWidget(QWidget* parent) : QOpenGLWidget(parent)
{
	setFocusPolicy(Qt::StrongFocus);
	connect(&timer, SIGNAL(timeout()), this, SLOT(on_timeout()));

	timer.start(2);

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



QString getTexturePath() // get from others
{
	// TODO: get texture path from config file

	std::string texturePath = source_path + "\\resources\\boss_hornet.png";

	QString texturePathQ = QString::fromStdString(texturePath);

	return texturePathQ;
}

QVector3D getOffset() // get from others
{
	// TODO: get offset from config file
	QVector3D offset = QVector3D(0.0f, 0.0f, 0.0f);
	return offset;
}

QVector2D getSize() // get from others
{
	// TODO: get size from config file
	QVector2D size = QVector2D(0.4f, 0.6f);
	return size;
}


void getTextureInfo(Image& imgComponent, QString* texturePathQ, QVector3D* offset, QVector2D* size)
{

	std::string texturePath = source_path + imgComponent.imgPath;

	*texturePathQ = QString(QString::fromStdString(texturePath));

	auto position = imgComponent.gameObject->transform->getWorldPosition();
	*offset = QVector3D(position.x, position.y, 0.0f);
	auto imgSize = imgComponent.size;
	*size = QVector2D(imgSize.x, imgSize.y);

}

void getTextureInfoTest(QString* texturePathQ, QVector3D* offset, QVector2D* size)
{

	std::string texturePath = source_path + "\\resources\\boss_hornet_fixed.png";

	*texturePathQ = QString(QString::fromStdString(texturePath));

	*offset = QVector3D(0.0f, 0.0f, 0.0f);

	*size = QVector2D(0.4f, 0.6f);

}


int RenderWidget::renderMain(QString* texturePathQ, QVector3D* offset, QVector2D* size)
{
	std::unique_ptr<QOpenGLTexture> textureSample = std::make_unique<QOpenGLTexture>(QImage(*texturePathQ).mirrored().convertToFormat(QImage::Format_RGBA8888), QOpenGLTexture::GenerateMipMaps);

	textureSample->create();
	textureSample->setFormat(QOpenGLTexture::RGBAFormat);
	textureSample->allocateStorage();


	shaderProgram->bind();

	renderTexture(textureSample.get(), *offset, *size);

	return 1;
}




void RenderWidget::initializeGL()
{
	initializeOpenGLFunctions();



	// source_path = SOURCE_DIR;
	source_path = GameEngine::get_instance().getRootPath();

	logger = std::make_unique<QOpenGLDebugLogger>(this);
	logger->initialize();
	connect(logger.get(), &QOpenGLDebugLogger::messageLogged, this, &RenderWidget::messageLogHandler);
	logger->startLogging();

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);


	createProgram();
	createVBO();
	createVAO();
	createIBO();


}

void RenderWidget::resizeGL(int w, int h)
{
	Q_UNUSED(w); Q_UNUSED(h);
	glViewport(0, 0, w, h);
}

void RenderWidget::paintGL()
{
	makeCurrent();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*
		//textureBoss = std::make_unique<QOpenGLTexture>(QImage(QString::fromStdString(source_path + "\\resources\\boss_hornet.png")).mirrored().convertToFormat(QImage::Format_RGBA8888), QOpenGLTexture::GenerateMipMaps);

		//textureSmile = std::make_unique<QOpenGLTexture>(QImage(QString::fromStdString(source_path + "\\resources\\awesomeface.png")).mirrored().convertToFormat(QImage::Format_RGBA8888), QOpenGLTexture::GenerateMipMaps);

		// textureSmile = std::make_unique<QOpenGLTexture>(QImage(QString::fromStdString(source_path + "\\resources\\awesomeface.png")).mirrored(), QOpenGLTexture::GenerateMipMaps);
		// textureSmile->create();
		// textureBoss->create();
		// textureWall = std::make_unique<QOpenGLTexture>(QImage(QString::fromStdString(source_path + "\\resources\\boss_hornet.png")).mirrored(), QOpenGLTexture::GenerateMipMaps);
		// textureSmile->setFormat(QOpenGLTexture::RGBAFormat);
		// textureBoss->setFormat(QOpenGLTexture::RGBAFormat);
	 */


	 //render scene 
	 /*
	 auto scene = GameEngine::getInstance().getCurrentScene();
	 if(scene!=nullptr)
	 for (auto rootObj : scene->getRootGameObjs())
	 {
		 //if(rootobj.needRender())
			 //renderMain(texturePathQ, offset, size);
		 QString* texturePathQ = new QString;
		 QVector3D* offset = new QVector3D;
		 QVector2D* size = new QVector2D;
		 getTextureInfo(*(rootObj->getComponent<Image>()), texturePathQ, offset, size);
		 renderMain(texturePathQ, offset, size);
		 //while()
	 }
	 */

	unsigned int time = QTime::currentTime().msec();

	std::cout << "time 1:" << time << std::endl;

	//matrix.rotate(time, 0.0f, 0.0f, 1.0f);

	//float rotation_speed = 1.0f; // 每帧旋转的角度
	//float angle = (time % 360) * rotation_speed;
	//matrix.rotate(m_angle, 0.0f, 0.0f, 1.0f);

	auto scene = SceneMgr::get_instance().get_current_scene();
	if (auto cam = SceneMgr::get_instance().get_main_camera(); scene == nullptr || scene->getRootGameObjs().size() == 0 || cam == nullptr)
		return;
	for (auto gameobj : scene->getRootGameObjs())
	{
		QString* texturePathQ = new QString;
		QVector3D* offset = new QVector3D;
		QVector2D* size = new QVector2D;
		auto img = gameobj->getComponent<Image>();
		if (img == nullptr)
			continue;

		getTextureInfo(*img, texturePathQ, offset, size);
		auto matrix = SceneMgr::get_instance().get_main_camera()->CalculateProjectionMulViewMatrix();
		matrix.translate(gameobj->transform->getWorldPosition().toQVector3D());
		gameobj->transform->localRotation += 5.f;
		matrix.rotate(gameobj->transform->getWorldRotation(), QVector3D(0.f, 0.f, 1.f));
		matrix.scale(gameobj->transform->getWorldScale().toQVector3D(1.0f));

		//getTextureInfoTest(texturePathQ, offset, size);
		if (textures.find(texturePathQ->toStdString()) != textures.end())
		{
			auto texture = textures[texturePathQ->toStdString()];
			shaderProgram->bind();
			shaderProgram->setUniformValue("rotationMatrix", matrix);

			renderTexture(texture, *offset, *size);

			continue;
		}
		auto texture = new QOpenGLTexture(QImage(*texturePathQ).mirrored().convertToFormat(QImage::Format_RGBA8888), QOpenGLTexture::GenerateMipMaps);
		//std::unique_ptr<QOpenGLTexture> textureSample = std::make_unique<QOpenGLTexture>(QImage(*texturePathQ).mirrored().convertToFormat(QImage::Format_RGBA8888), QOpenGLTexture::GenerateMipMaps);
		texture->create();
		textures[texturePathQ->toStdString()] = texture;
		//textureSample->setFormat(QOpenGLTexture::RGBAFormat);
		//textureSample->setFormat(QOpenGLTexture::RGBA8_UNorm);
		//textureSample->allocateStorage();
		shaderProgram->bind();
		shaderProgram->setUniformValue("rotationMatrix", matrix);

		renderTexture(texture, *offset, *size);
	}
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
	shaderProgram = std::make_unique<QOpenGLShaderProgram>();
	shaderProgram->create();
	shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, QString::fromStdString(source_path + "\\shaders\\shaders.vert"));
	shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, QString::fromStdString(source_path + "\\shaders\\shaders.frag"));
	success = shaderProgram->link(); //
	if (!success)
		qDebug() << "ERR:" << shaderProgram->log();
	//        textureSmileBinding=shaderProgram->uniformLocation("textureSmile");
	//        textureSmileBinding=shaderProgram->uniformLocation("textureSmile");
	shaderOffsetBinding = shaderProgram->uniformLocation("offset");
	shaderSizeBinding = shaderProgram->uniformLocation("size");
	//    textureWallBinding=shaderProgram->uniformLocation("textureWall");
	textureWallBinding = 0;
}
void RenderWidget::createVAO()
{
	// 找shader的起始位置，并且同时修改下两行的poslocation
	shaderProgram->bind();
	GLint posLocation = shaderProgram->attributeLocation("aPos");
	GLint colorLocation = shaderProgram->attributeLocation("aColor");
	GLint textureLocation = shaderProgram->attributeLocation("aTexCord");

	vao = std::make_unique<QOpenGLVertexArrayObject>();

	vao->create();

	vao->bind();
	vbo->bind();

	auto stride = 8 * sizeof(float);

	//-----------------position--------------------//
	//告知显卡如何解析缓冲里的属性值
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);

	//开启VAO管理的第一个属性值
	glEnableVertexAttribArray(posLocation);

	if (colorLocation >= 0)
	{
		//------------------Color-----------------------//
		//告知显卡如何解析缓冲里的属性值
		glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
		//开启VAO管理的第一个属性值
		glEnableVertexAttribArray(colorLocation);
	}
	if (textureLocation)
	{
		//------------------Texture-----------------------//
		//    告知显卡如何解析缓冲里的属性值
		glVertexAttribPointer(textureLocation, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
		//    开启VAO管理的第一个属性值
		glEnableVertexAttribArray(textureLocation);
	}
	vbo->release();

	glBindVertexArray(0);
}
void RenderWidget::createVBO()
{
	vbo = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::VertexBuffer);
	vbo->create();
	vbo->bind();
	vbo->allocate(vertices, sizeof(vertices));
}
void RenderWidget::createIBO()
{
	ibo = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::IndexBuffer);
	ibo->create();
	ibo->bind();
	ibo->allocate(indices, sizeof(indices));
}
void RenderWidget::messageLogHandler(const QOpenGLDebugMessage& debugMessage)
{
	qDebug() << debugMessage.message();
}
void RenderWidget::renderTexture(QOpenGLTexture* texture, QVector3D offset, QVector2D size)
{

	offset.setZ(0.1f); // 统一深度 不要被挡住了 保证正确混合

	shaderProgram->setUniformValue(shaderOffsetBinding, offset);
	shaderProgram->setUniformValue(shaderSizeBinding, size);
	vao->bind();
	ibo->bind();





	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);

	if (textureWallBinding >= 0)
		texture->bind(textureWallBinding);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(std::size(indices)), GL_UNSIGNED_INT, 0);
}
