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
unsigned int VBOBOX, VAOBOX, EBOBOX;

float vertices[] = {
	// positions   // colors           // texture coords
	1.f,  1.f, 0,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
	1.f, -1.f, 0,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
   -1.f, -1.f, 0,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
   -1.f,  1.f, 0,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
};

//counter clockwise

unsigned int indices[] = { // note that we start from 0!
			   0, 3, 2, // first triangle
			   0, 2, 1 // second triangle
};


/*
//counter clockwise
unsigned int indices[] = { // note that we start from 0!
			   0, 1, 2, // first triangle
			   0, 2, 3 // second triangle
};
*/


RenderWidget::RenderWidget(QWidget* parent) : QOpenGLWidget(parent)
{
	setFocusPolicy(Qt::StrongFocus);
	connect(&timer, SIGNAL(timeout()), this, SLOT(on_timeout()));
	frameCount = 0;
	//timer.start(2);
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


float* RenderWidget::getTextureVertices(QVector3D offset, QVector2D size)
{

	// print size.y()
	//std::cout << "size.y() " << 1.0 / 2 * size.y() << std::endl;

	float ltx = offset.x() - 1.0 / 2 * size.x();
	float lty = offset.y() + 1.0 / 2 * size.y();

	float rtx = offset.x() + 1.0 / 2 * size.x();
	float rty = offset.y() + 1.0 / 2 * size.y();

	float rbx = offset.x() + 1.0 / 2 * size.x();
	float rby = offset.y() - 1.0 / 2 * size.y();

	float lbx = offset.x() - 1.0 / 2 * size.x();
	float lby = offset.y() - 1.0 / 2 * size.y();


	//std::cout << "x: " << x << std::endl;

	QVector3D leftTop{ ltx, lty, 0.0f };
	QVector3D rightTop{ rtx, rty, 0.0f };
	QVector3D rightBottom{ rbx, rby, 0.0f };
	QVector3D leftBottom{ lbx, lby, 0.0f };

	/*
	vertices[0] = {
		// positions                           // colors           // texture coords
		rightTop.x(),     rightTop.y(),    0,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		rightBottom.x(),  rightBottom.y(), 0,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
	    leftBottom.x(),   leftBottom.y(),  0,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		leftTop.x(),	  leftTop.y(),     0,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
	};
	*/

	bool old = true;
	if (old)
	{
		vertices[0] = rightTop.x();
		vertices[1] = rightTop.y();

		vertices[8] = rightBottom.x();
		vertices[9] = rightBottom.y();

		vertices[16] = leftBottom.x();
		vertices[17] = leftBottom.y();

		vertices[24] = leftTop.x();
		vertices[25] = leftTop.y();
	}
	else
	{
		// 左上
		vertices[0] = leftTop.x();
		vertices[1] = leftTop.y();
		// 左下
		vertices[8] = leftBottom.x();
		vertices[9] = leftBottom.y();
		// 右上
		vertices[16] = rightTop.x();
		vertices[17] = rightTop.y();
		// 右下
		vertices[24] = rightBottom.x();
		vertices[25] = rightBottom.y();

	}
	
	// print
	/*
	std::cout << "leftTop " << leftTop.x() << " " << leftTop.y() << " " << leftTop.z() << std::endl;
	std::cout << "rightTop " << rightTop.x() << " " << rightTop.y() << " " << rightTop.z() << std::endl;
	std::cout << "rightBottom " << rightBottom.x() << " " << rightBottom.y() << " " << rightBottom.z() << std::endl;
	std::cout << "leftBottom " << leftBottom.x() << " " << leftBottom.y() << " " << leftBottom.z() << std::endl;
	*/

	/*
	// print vertices with standar formal
	std::cout<< "right top:     "<< vertices[0] << " " << vertices[1] << " " << vertices[2] << " " << vertices[3] << " " << vertices[4] << " " << vertices[5] << " " << vertices[6] << " " << vertices[7] << std::endl;
	std::cout << "right bottom: " << vertices[8] << " " << vertices[9] << " " << vertices[10] << " " << vertices[11] << " " << vertices[12] << " " << vertices[13] << " " << vertices[14] << " " << vertices[15] << std::endl;
	std::cout << "left bottom:  " << vertices[16] << " " << vertices[17] << " " << vertices[18] << " " << vertices[19] << " " << vertices[20] << " " << vertices[21] << " " << vertices[22] << " " << vertices[23] << std::endl;
	std::cout << "left top:     " << vertices[24] << " " << vertices[25] << " " << vertices[26] << " " << vertices[27] << " " << vertices[28] << " " << vertices[29] << " " << vertices[30] << " " << vertices[31] << std::endl;
	
	*/


	createVBO();

	return nullptr;
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

	/*logger = std::make_unique<QOpenGLDebugLogger>(this);
	logger->initialize();
	connect(logger.get(), &QOpenGLDebugLogger::messageLogged, this, &RenderWidget::messageLogHandler);
	logger->startLogging();*/

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);


	createProgram();
	//createBoxProgram();
	//createVBO();
	//createVAO();
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

	auto scene = SceneMgr::get_instance().get_current_scene();
	if (auto cam = SceneMgr::get_instance().get_main_camera(); scene == nullptr || scene->getRootGameObjs().size() == 0 || cam == nullptr)
		return;
	
	
	// render
	for (auto gameobj : scene->getRootGameObjs())
	{
		QString* texturePathQ = new QString;
		QVector3D* offset = new QVector3D;
		QVector2D* size = new QVector2D;
		auto img = gameobj->getComponent<Image>();
		if (img == nullptr)
			continue;

		//std::cout << "time 2:" << double(end2 - begin) / CLOCKS_PER_SEC * 1000 << "ms" << std::endl;
		
		getTextureInfo(*img, texturePathQ, offset, size);
		
		
		//getTextureInfoTest(texturePathQ, offset, size);
		getTextureVertices(QVector3D(0.f,0.f,0.f), *size);

		//createVBO();
		createVAO();
		
		//createBoxVAO();

		auto matrix = SceneMgr::get_instance().get_main_camera()->CalculateProjectionMulViewMatrix();
		matrix.translate(gameobj->transform->getWorldPosition().toQVector3D());
		gameobj->transform->set_localRotation(gameobj->transform->get_localRotation() + 5.f);
		//matrix.rotate(gameobj->transform->getWorldRotation(), QVector3D(0.f, 0.f, 1.f));
		matrix.scale(gameobj->transform->getWorldScale().toQVector3D(1.0f));

		//getTextureInfoTest(texturePathQ, offset, size);
		if (textures.find(texturePathQ->toStdString()) != textures.end())
		{
			auto texture = textures[texturePathQ->toStdString()];
			shaderProgram->bind();
			shaderProgram->setUniformValue("rotationMatrix", matrix);

			//std::cout << "time 4:" << double(end4 - begin) / CLOCKS_PER_SEC * 1000 << "ms" << std::endl;
			renderTexture(texture, *offset, *size);
			

			//std::cout << "time 5:" << double(end5 - begin) / CLOCKS_PER_SEC * 1000 << "ms" << std::endl;
			continue;
		}
		auto texture = new QOpenGLTexture(QImage(*texturePathQ).mirrored().convertToFormat(QImage::Format_RGBA8888), QOpenGLTexture::GenerateMipMaps);
		texture->create();
		textures[texturePathQ->toStdString()] = texture;
		
		shaderProgram->bind();
		shaderProgram->setUniformValue("rotationMatrix", matrix);


		renderTexture(texture, *offset, *size);

		
		renderBox();


		// render box
		

		
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

void RenderWidget::createBoxProgram() {
	bool success;
	shaderBoxProgram = std::make_unique<QOpenGLShaderProgram>();
	shaderBoxProgram->create();
	shaderBoxProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, QString::fromStdString(source_path + "\\shaders\\boxShader.vert"));	
	shaderBoxProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, QString::fromStdString(source_path + "\\shaders\\boxShader.frag"));
	success = shaderBoxProgram->link();
	if (!success)
		qDebug() << "ERR:" << shaderProgram->log();
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void RenderWidget::createBoxVAO()
{
	shaderBoxProgram->bind();

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//告知显卡如何解析缓冲里的属性值
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//开启VAO管理的第一个属性值
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

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
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(std::size(indices)), GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(3.0f);
	glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, NULL);

	shaderProgram->release();
	
}

void RenderWidget::renderBox()
{
	//创建VBO和VAO对象，并赋予ID
	glGenVertexArrays(1, &VAOBOX);
	glGenBuffers(1, &VBOBOX);

	//绑定VBO和VAO对象
	glBindVertexArray(VAOBOX);
	glBindBuffer(GL_ARRAY_BUFFER, VBOBOX);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//告知显卡如何解析缓冲里的属性值
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//开启VAO管理的第一个属性值
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	shaderBoxProgram = std::make_unique<QOpenGLShaderProgram>(this);
	shaderBoxProgram->create();
	shaderBoxProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, QString::fromStdString(source_path + "\\shaders\\boxShader.vert"));
	shaderBoxProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, QString::fromStdString(source_path + "\\shaders\\boxShader.frag"));
	shaderBoxProgram->link();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glGenBuffers(1, &EBOBOX);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOBOX);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindVertexArray(0);

	shaderBoxProgram->bind();
	glBindVertexArray(VAOBOX);

	glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, NULL);

}
