#include "renderwidget.h"


#include <iostream>
#include <Core/SystemStatus/GameEngine.h>
#include "Core/Core/Image.h"
#include "Core/Core/Camera.h"
#include <QElapsedTimer>
#include <Core/ResourceManagement/SceneMgr.h>

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

	std::string texturePath = source_path + imgComponent.get_imgPath();

	*texturePathQ = QString(QString::fromStdString(texturePath));

	auto position = imgComponent.gameObject->transform->getWorldPosition();
	*offset = QVector3D(position.x, position.y, 0.0f);
	auto imgSize = imgComponent.get_size();
	*size = QVector2D(imgSize.x, imgSize.y);

}


// 根据图片的分辨率和尺寸，设定四个顶点，并创建VBO （后期需要把这四个顶点换成碰撞盒的四个顶点）
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


	QVector3D leftTop{ ltx, lty, 0.0f };
	QVector3D rightTop{ rtx, rty, 0.0f };
	QVector3D rightBottom{ rbx, rby, 0.0f };
	QVector3D leftBottom{ lbx, lby, 0.0f };
	
	vertices[0] = rightTop.x();
	vertices[1] = rightTop.y();

	vertices[8] = rightBottom.x();
	vertices[9] = rightBottom.y();

	vertices[16] = leftBottom.x();
	vertices[17] = leftBottom.y();

	vertices[24] = leftTop.x();
	vertices[25] = leftTop.y();
	
	
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


	// source_path = SOURCE_DIR;
	source_path = GameEngine::get_instance().getRootPath();

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);


	createProgram();
	createBoxProgram();
	
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
	for (auto& pair : scene->getAllGameObjsByDepth())
	{
		auto gameobj = pair.second;
		QString* texturePathQ = new QString;
		QVector3D* offset = new QVector3D;
		QVector2D* size = new QVector2D;
		auto img = gameobj->getComponent<Image>();
		if (img == nullptr||!img->get_enabled())
			continue;

		//std::cout << "time 2:" << double(end2 - begin) / CLOCKS_PER_SEC * 1000 << "ms" << std::endl;
		
		getTextureInfo(*img, texturePathQ, offset, size);
		
		
		//getTextureInfoTest(texturePathQ, offset, size);
		getTextureVertices(QVector3D(0.f,0.f,0.f), *size); // 根据图片的分辨率和尺寸，设定四个顶点，并创建VBO

		
		createVAO();
		

		
		

		auto matrix = SceneMgr::get_instance().get_main_camera()->CalculateProjectionMulViewMatrix();
		matrix.translate(gameobj->transform->getWorldPosition().toQVector3D());
		gameobj->transform->set_localRotation(gameobj->transform->get_localRotation() + 5.f);
		matrix.rotate(gameobj->transform->getWorldRotation(), QVector3D(0.f, 0.f, 1.f));
		matrix.scale(gameobj->transform->getWorldScale().toQVector3D(1.0f));

		//getTextureInfoTest(texturePathQ, offset, size);
		if (textures.find(texturePathQ->toStdString()) != textures.end())
		{
			auto texture = textures[texturePathQ->toStdString()];
			shaderProgram->bind();
			shaderProgram->setUniformValue("rotationMatrix", matrix);
			renderTexture(texture, *offset, *size);
			
			shaderBoxProgram->bind();
			shaderBoxProgram->setUniformValue("rotationMatrix", matrix);
			createBoxVBO();
			createBoxVAO();
			renderBox();
			shaderBoxProgram->release();
			
			continue;
		}
		auto texture = new QOpenGLTexture(QImage(*texturePathQ).mirrored().convertToFormat(QImage::Format_RGBA8888), QOpenGLTexture::GenerateMipMaps);
		texture->create();
		textures[texturePathQ->toStdString()] = texture;
		
		shaderProgram->bind();
		shaderProgram->setUniformValue("rotationMatrix", matrix);

		// 渲染图像和碰撞盒
		renderTexture(texture, *offset, *size);
		shaderBoxProgram->bind();
		shaderBoxProgram->setUniformValue("rotationMatrix", matrix);
		createBoxVBO();
		createBoxVAO();
		renderBox();

		shaderBoxProgram->release();
		
		
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
	shaderOffsetBinding = shaderProgram->uniformLocation("offset");
	shaderSizeBinding = shaderProgram->uniformLocation("size");
	textureWallBinding = 0;
}

// 暂时没用
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

void RenderWidget::createBoxVAO() 
{

	shaderBoxProgram->bind();
	
	GLint posLocation = shaderBoxProgram->attributeLocation("aPos");
	GLint colorLocation = shaderBoxProgram->attributeLocation("aColor");

	vaoBox = std::make_unique<QOpenGLVertexArrayObject>();

	vaoBox->create();

	vaoBox->bind();
	vboBox->bind();

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

	
	// renderBox();
	
	
	// 绘制碰撞盒
	glDisable(GL_BLEND);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(3.0f);

	ibo->release(); // 释放画图像的ibo
	createBoxEBO(); // 重新绑定画盒子的ibo

	// 绘制矩形边框
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, nullptr);
	
		
}


//
void RenderWidget::renderBox()
{
	vaoBox->bind();
	
	ibo->release(); // 释放画图像的ibo
	createBoxEBO(); // 重新绑定画盒子的ibo
	

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(3.0f);

	// 绘制矩形边框
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, nullptr);
	glLineWidth(3.0f);

}



