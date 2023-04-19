#include "renderwidget.h"


#include <iostream>
#include <QPainter>
#include <Core/SystemStatus/GameEngine.h>
#include <QElapsedTimer>
#include <Core/ResourceManagement/SceneMgr.h>
#include "qevent.h"
#include <qopenglframebufferobject.h>
#include <qapplication.h>

RenderWidget* RenderWidget::sceneWidget= nullptr;
RenderWidget* RenderWidget::gameWidget = nullptr;
std::string source_path;
static int count = 0;

RenderWidget::RenderWidget(QWidget* parent) : QOpenGLWidget(parent)
{
	setFocusPolicy(Qt::StrongFocus);
	connect(&timer, SIGNAL(timeout()), this, SLOT(on_timeout()));
	frameCount = 0;
	fbo = nullptr;
	fboOverlay = nullptr;
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
	if (auto img = gameobj->getComponent<Image>(); img != nullptr && img->get_enabled())
	{
		renderImage(img, camera);
	}
	if (auto boxCollider = gameobj->getComponent<BoxCollider>();boxCollider != nullptr && boxCollider->get_enabled())
	{
		renderBoxCollider(boxCollider, camera);
	}

	if (auto text = gameobj->getComponent<Text>(); text != nullptr && text->get_enabled())
	{
		renderText(text, camera);
	}

	// renderText();
}

void RenderWidget::renderBoxCollider(BoxCollider* box, Camera* boxColliderCamera)
{
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
		//-----------------position--------------------//
		//告知显卡如何解析缓冲里的属性值
		glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		//开启VAO管理的第一个属性值
		glEnableVertexAttribArray(posLocation);

		box->vao->release();
		box->vbo->release();
		box->vbo->destroy();
		delete box->vbo;
		box->vbo = nullptr;
		/*img->ibo->release();
		img->ibo->destroy();
		img->ibo = nullptr;
		delete img->ibo;*/
	}
	box->vao->bind();
	
	//calculate the MVP matrix
	QMatrix4x4 matrix;
	if (editMode)
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
	boxColliderShaderProgram->setUniformValue("color",box->color.red(), box->color.green(), box->color.blue(),box->color.alpha());

	//set line mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(3.0f);

	//draw
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);
	box->vao->release();
}

void RenderWidget::renderImage(Image* img, Camera* imageCamera)
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
		img->vbo->allocate(img->vertices.data(), static_cast<int>(img->vertices.size() * sizeof(Vertex)));
		img->ibo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
		img->ibo->create();
		img->ibo->bind();
		img->ibo->allocate(img->indices.data(), static_cast<int>(img->indices.size() * sizeof(unsigned int)));
		imageShaderProgram->bind();
		
		GLint posLocation = imageShaderProgram->attributeLocation("aPos");
		GLint textureLocation = imageShaderProgram->attributeLocation("aTexCord");
		
		GLsizei stride = sizeof(Vertex);		
		//-----------------position--------------------//
		//告知显卡如何解析缓冲里的属性值
		glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		//开启VAO管理的第一个属性值
		glEnableVertexAttribArray(posLocation);

		if (textureLocation)
		{
			//------------------Texture-----------------------//		
			glVertexAttribPointer(textureLocation, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));			
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
	QMatrix4x4 matrix;
	if (editMode)
	{
		 matrix = mCamera->CalculateProjectionMulViewMatrix();
	}
	else
	{
		 matrix = SceneMgr::get_instance().get_main_camera()->CalculateProjectionMulViewMatrix();
	}
	matrix = imageCamera->CalculateProjectionMulViewMatrix();
	auto transform = img->gameObject->transform;
	matrix.translate(transform->getWorldPosition().toQVector3D());
	matrix.rotate(transform->getWorldRotation(), QVector3D(0.f, 0.f, 1.f));
	matrix.scale(transform->getWorldScale().toQVector3D(1.0f));

	//transfer the MVP matrix to the shader
	imageShaderProgram->bind();
	imageShaderProgram->setUniformValue("MVPMatrix", matrix);
	imageShaderProgram->setUniformValue("color", img->color.red(), img->color.green(), img->color.blue(), img->color.alpha());

	//bind texture
	if(img->texture!=nullptr)
		img->texture->bind();
	img->ibo->bind();
	//enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//enable depth test
	glDepthFunc(GL_LEQUAL);

	//set texture mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//draw texture
	glDrawElements(GL_TRIANGLES, sizeof(unsigned int)*6, GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	img->ibo->release();
	
	//draw border
	if (img->borderIbo == nullptr)
	{
		img->borderIbo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
		img->borderIbo->create();
		img->borderIbo->bind();
		img->borderIbo->allocate(img->borderIndices.data(), static_cast<int>(img->borderIndices.size() * sizeof(unsigned int)));
	}
	img->borderIbo->bind();
	
	boxColliderShaderProgram->bind();
	boxColliderShaderProgram->setUniformValue("MVPMatrix", matrix);
	boxColliderShaderProgram->setUniformValue("color",1.0f,1.0f,1.0f,1.0f);

	glLineWidth(3.0f);
	glDrawElements(GL_LINES, static_cast<GLsizei>(sizeof(unsigned int) * img->borderIndices.size()), GL_UNSIGNED_INT, 0);
	img->borderIbo->release();

	//release
	img->vao->release();
	if(img->texture!=nullptr)
		img->texture->release();
}

void RenderWidget::renderText(Text* text, Camera* textCamera)
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
	// 输出最大长度和字符串的个数
	// qDebug() << "maxLen:" << maxLen << "count:" << count;

	// 宽度设置为最大字符串长度 * 字符宽度，字符宽度设置为35
	int width = maxLen * 35;
	// 高度设置为字符串个数 * 字符高度，字符高度设置为75
	int height = count * 75;

	text->set_size(Vector2D(width, height));

	// 创建QColor对象color为text.color
	QColor color(text->color.r, text->color.g, text->color.b, text->color.a);

	mTexture = genTextTexture(width, height, data, 60, color);


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

		//-----------------position--------------------//
		//告知显卡如何解析缓冲里的属性值
		glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		//开启VAO管理的第一个属性值
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
		/*text->ibo->release();
		text->ibo->destroy();
		text->ibo = nullptr;
		delete text->ibo;*/
	}
	text->vao->bind();

	//calculate the MVP matrix
	QMatrix4x4 matrix;
	if (editMode)
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
	glDrawElements(GL_TRIANGLES, sizeof(unsigned int) * 6, GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	text->ibo->release();

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
	glDrawElements(GL_LINES, static_cast<GLsizei>(sizeof(unsigned int) * text->borderIndices.size()), GL_UNSIGNED_INT, 0);
	text->borderIbo->release();
	
	
	//release
	text->vao->release();
	if(text->texture!=nullptr)
		text->texture->release();

}

void RenderWidget::renderScene(Camera* camera)
{
	auto scene = SceneMgr::get_instance().get_current_scene();
	if ( scene == nullptr || scene->getRootGameObjs().size() == 0 || mCamera == nullptr)
		return;
	for (auto& pair : scene->getAllGameObjsByDepth())
	{
		if (pair.second->isActive)
			renderGameobject(pair.second, camera);
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

	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	createProgram();
	createBoxProgram();	
	createTextProgram();
	createTextureProgram();
	
	textBuffer = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::VertexBuffer);
}

void RenderWidget::resizeGL(int w, int h)
{
	Q_UNUSED(w); Q_UNUSED(h);
	glViewport(0, 0, w, h);
	
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

void RenderWidget::paintGL()
{
	frameCount++;
	makeCurrent();
	renderFbo();
	renderFboOverlay();
	mixTexture();
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
	return *sceneWidget;
}

void RenderWidget::renderFbo() 
{
	if (fbo == nullptr)
	{
		// 创建一个 FBO，大小为窗口大小乘以设备像素比
		qreal dpr = qApp->primaryScreen()->devicePixelRatio();
		QSize scaledSize = size() * dpr;
		fbo = new QOpenGLFramebufferObject(scaledSize);
	}
	fbo->bind();
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	if (editMode)
		renderScene(mCamera);
	else
		renderScene(SceneMgr::get_instance().get_main_camera());

	/*
	if (frameCount % 200 == 0)
	{
		// 将渲染结果保存到 QImage 中
		QImage image = fbo->toImage();
		QString filePath = "E:/GroupProject/github/Pure-Handmade-Small-Workshop/output/image.png";
		if (image.save(filePath, "PNG")) {
			qDebug() << "Image saved successfully!";
		}
		else {
			qDebug() << "Error saving image!";
		}
	}
	*/

	fbo->release();
}

void RenderWidget::renderFboOverlay()
{
	// render overlay
	if (fboOverlay == nullptr)
	{
		// 创建一个 FBO，大小为窗口大小乘以设备像素比
		qreal dpr = qApp->primaryScreen()->devicePixelRatio();
		QSize scaledSize = size() * dpr;
		fboOverlay = new QOpenGLFramebufferObject(scaledSize);
	}
	fboOverlay->bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderScene(SceneMgr::get_instance().get_main_camera()); // 用场景相机
	fboOverlay->release();
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
	//-----------------position--------------------//
	//告知显卡如何解析缓冲里的属性值
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	//开启VAO管理的第一个属性值
	glEnableVertexAttribArray(posLocation);

	if (textureLocation)
	{
		//------------------Texture-----------------------//		
		glVertexAttribPointer(textureLocation, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(textureLocation);
	}

	textureShaderProgram->setUniformValue("texture1", 1);
	textureShaderProgram->setUniformValue("texture2", 2);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, fbo->texture());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, fboOverlay->texture());

	//enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//enable depth test
	glDepthFunc(GL_LEQUAL);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//draw
	glDrawElements(GL_TRIANGLES, sizeof(unsigned int) * 6, GL_UNSIGNED_INT, 0);

	vaoTexture->release();
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

void RenderWidget::createProgram()
{
	bool success;
	imageShaderProgram = std::make_unique<QOpenGLShaderProgram>();
	imageShaderProgram->create();
	imageShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, QString::fromStdString(source_path + "\\shaders\\shaders.vert"));
	imageShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, QString::fromStdString(source_path + "\\shaders\\shaders.frag"));
	success = imageShaderProgram->link(); 
	if (!success)
		qDebug() << "ERR:" << imageShaderProgram->log();
	shaderOffsetBinding = imageShaderProgram->uniformLocation("offset");
	shaderSizeBinding = imageShaderProgram->uniformLocation("size");
	imageTextureBinding = 0;
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

void RenderWidget::messageLogHandler(const QOpenGLDebugMessage& debugMessage)
{
	qDebug() << debugMessage.message();
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
	Vector2D Pos = Vector2D(event->localPos().x(),size().height() - event->localPos().y());
	//Debug::log("mouse position: " + pos.tostring());
	//if(SceneMgr::get_instance().get_main_camera()!=nullptr)
		//Debug::log("camera position: "+SceneMgr::get_instance().get_main_camera()->screenToWorld(pos).tostring());

	if (event->buttons() & Qt::LeftButton) {

	}
	else if (event->buttons() & Qt::RightButton)
	{
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
	

	if (event->button() == Qt::RightButton)
	{
		lastPos = event->pos();
		
	}
}

void RenderWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::RightButton)
	{
		event->ignore();
	}
}



void RenderWidget::keyPressEvent(QKeyEvent* event)
{
	switch (event->key()) {
    
    case Qt::Key_W: mCameraObject->transform->translate(Vector2D(0.f,1.f));break;
    case Qt::Key_S: mCameraObject->transform->translate(Vector2D(0.f,-1.f)); break;
    case Qt::Key_D: mCameraObject->transform->translate(Vector2D(1.f, 0.f)); break;
    case Qt::Key_A: mCameraObject->transform->translate(Vector2D(-1.f, 0.f)); break;

    default:
        break;
    }
}

void RenderWidget::wheelEvent(QWheelEvent* event)
{
	if (event->angleDelta().y() > 0)
		mCamera->set_view_width(mCamera->get_view_width() * 0.9f);
	else
		mCamera->set_view_width(mCamera->get_view_width() * 1.1f);
}

void RenderWidget::contextMenuEvent(QContextMenuEvent* event)
{
	event->accept();
}







