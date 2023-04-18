#include "renderwidget.h"


#include <iostream>
#include <QPainter>
#include <Core/SystemStatus/GameEngine.h>
#include "Core/Core/Camera.h"
#include <QElapsedTimer>
#include <Core/ResourceManagement/SceneMgr.h>
#include "qevent.h"
#include <qopenglframebufferobject.h>

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
/*
static const char* vertexShaderSource =
"attribute highp vec4 aPos;\n"
"attribute highp vec2 aTexCord;\n"
"varying highp vec2 v_texcoord;\n"
"uniform highp mat4 matrix;\n"
"uniform highp mat4 projMatrix;\n"
"void main() {\n"
"   v_texcoord = aTexCord;\n"
"   gl_Position = aPos;\n"
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


*/
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
	// positions   // colors                // texture coords
	1.f,  1.f, 0,  1.0f, 0.0f, 0.0f, 1.f,   1.0f, 1.0f,   // top right
	1.f, -1.f, 0,  0.0f, 1.0f, 0.0f, 1.f,   1.0f, 0.0f,   // bottom right
   -1.f, -1.f, 0,  0.0f, 0.0f, 1.0f, 1.f,   0.0f, 0.0f,   // bottom left
   -1.f,  1.f, 0,  1.0f, 1.0f, 0.0f, 1.f,   0.0f, 1.0f    // top left
};


float TextVertices[] = {
	// positions   // colors                // texture coords
	400.f,  400.f, 0,  1.0f, 0.0f, 0.0f, 1.f,   1.0f, 1.0f,   // top right
	400.f,  300.f, 0,  0.0f, 1.0f, 0.0f, 1.f,   1.0f, 0.0f,   // bottom right
    300.f,  300.f, 0,  0.0f, 0.0f, 1.0f, 1.f,   0.0f, 0.0f,   // bottom left
    300.f,  400.f, 0,  1.0f, 1.0f, 0.0f, 1.f,   0.0f, 1.0f    // top left
};

//counter clockwise


unsigned int indices[] = { // note that we start from 0!
			   0, 3, 2, // first triangle
			   0, 2, 1 // second triangle
};

// 定义需要绘制的边
GLuint indicesBOX[] = { 0, 1, 1, 2, 2, 3, 3, 0 };

GLuint indicesText[] = { 0, 1, 1, 2, 2, 3, 3, 0 };



RenderWidget::RenderWidget(QWidget* parent) : QOpenGLWidget(parent)
{
	setFocusPolicy(Qt::StrongFocus);
	connect(&timer, SIGNAL(timeout()), this, SLOT(on_timeout()));
	frameCount = 0;
	instance = this;
	fbo = nullptr;
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

	if (auto text = gameobj->getComponent<Text>(); text != nullptr && text->get_enabled())
	{
		renderText(text);
	}

	// renderText();
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
		box->vbo->allocate(box->vertices.data(), box->vertices.size() * sizeof(Vertex));
		box->borderIbo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
		box->borderIbo->create();
		box->borderIbo->bind();
		box->borderIbo->allocate(box->borderIndices.data(), box->borderIndices.size() * sizeof(unsigned int));

		boxColliderShaderProgram->bind();

		GLint posLocation = boxColliderShaderProgram->attributeLocation("aPos");
		GLint colorLocation = boxColliderShaderProgram->attributeLocation("aColor");

		auto stride = sizeof(Vertex);
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
		img->vbo->allocate(img->vertices.data(), img->vertices.size() * sizeof(Vertex));
		img->ibo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
		img->ibo->create();
		img->ibo->bind();
		img->ibo->allocate(img->indices.data(), img->indices.size() * sizeof(unsigned int));
		imageShaderProgram->bind();
		
		GLint posLocation = imageShaderProgram->attributeLocation("aPos");
		GLint colorLocation = imageShaderProgram->attributeLocation("aColor");
		GLint textureLocation = imageShaderProgram->attributeLocation("aTexCord");
		
		auto stride = sizeof(Vertex);		
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
		img->borderIbo->allocate(img->borderIndices.data(), img->borderIndices.size() * sizeof(unsigned int));
	}
	img->borderIbo->bind();
	
	boxColliderShaderProgram->bind();
	boxColliderShaderProgram->setUniformValue("MVPMatrix", matrix);

	glLineWidth(3.0f);
	glDrawElements(GL_LINES, sizeof(unsigned int) * img->borderIndices.size(), GL_UNSIGNED_INT, 0);
	img->borderIbo->release();

	//release
	img->vao->release();
	if(img->texture!=nullptr)
		img->texture->release();
}

void RenderWidget::renderText(Text* text)
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
		text->vbo->allocate(text->vertices.data(), text->vertices.size() * sizeof(Vertex));
		
		text->ibo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
		text->ibo->create();
		text->ibo->bind();
		text->ibo->allocate(text->indices.data(), text->indices.size() * sizeof(unsigned int));
		textShaderProgram->bind();

		GLint posLocation = textShaderProgram->attributeLocation("aPos");
		GLint colorLocation = textShaderProgram->attributeLocation("aColor");
		GLint textureLocation = textShaderProgram->attributeLocation("aTexCord");

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
	auto matrix = SceneMgr::get_instance().get_main_camera()->CalculateProjectionMulViewMatrix();
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
		text->borderIbo->allocate(text->borderIndices.data(), text->borderIndices.size() * sizeof(unsigned int));
	}
	text->borderIbo->bind();

	boxColliderShaderProgram->bind();
	boxColliderShaderProgram->setUniformValue("MVPMatrix", matrix);

	glLineWidth(3.0f);
	glDrawElements(GL_LINES, sizeof(unsigned int) * text->borderIndices.size(), GL_UNSIGNED_INT, 0);
	text->borderIbo->release();
	
	
	//release
	text->vao->release();
	//text->texture->release();

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
	
	if (fbo != nullptr)
	{
		fbo->release();
		delete fbo;
		fbo = nullptr;
	}
	
}

void RenderWidget::paintGL()
{
	frameCount++;
	makeCurrent();
	if (fbo == nullptr)
	{
		fbo = new QOpenGLFramebufferObject(size());

	}
	
	fbo->bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderScene();

	if (frameCount%200 == 0)
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
	
	fbo->release();
	QOpenGLFramebufferObject::blitFramebuffer(nullptr, fbo, GL_COLOR_BUFFER_BIT, GL_LINEAR);
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
	/*
	bool success;
	textShaderProgram = std::make_unique<QOpenGLShaderProgram>();
	textShaderProgram->create();
	textShaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
	textShaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
	success = textShaderProgram->link();
	if (!success)
		qDebug() << "ERR:" << textShaderProgram->log();
	mPosAttr = textShaderProgram->attributeLocation("posAttr");
	mTexAttr = textShaderProgram->attributeLocation("texcoord");
	mMatrixLoc = textShaderProgram->uniformLocation("matrix");
	mProjLoc = textShaderProgram->uniformLocation("projMatrix");
	*/

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
	textureWallBinding = 0;
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
	createBoxIBO();

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

void RenderWidget::createTextVAO()
{
	textShaderProgram->bind();
	GLint posLocation = textShaderProgram->attributeLocation("posAttr");
	GLint texLocation = textShaderProgram->attributeLocation("texcoord");
	vaoText = std::make_unique<QOpenGLVertexArrayObject>();
	vaoText->create();
	vaoText->bind();
	vboText->bind();
	iboText->bind();
	//告知显卡如何解析缓冲里的属性值
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//开启VAO管理的第一个属性值
	glEnableVertexAttribArray(0);
	//告知显卡如何解析缓冲里的属性值
	glVertexAttribPointer(texLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//开启VAO管理的第二个属性值
	glEnableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	vboText->release();
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

void RenderWidget::createTextVBO()
{
	vboText = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::VertexBuffer);
	vboText->create();
	vboText->bind();
	vboText->allocate(verticesText, sizeof(verticesText));
}

void RenderWidget::createIBO()
{
	ibo = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::IndexBuffer);
	ibo->create();
	ibo->bind();
	ibo->allocate(indices, sizeof(indices));
}

void RenderWidget::createBoxIBO()
{
	iboBox = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::IndexBuffer);
	iboBox->create();
	iboBox->bind();
	iboBox->allocate(indicesBOX, sizeof(indicesBOX));
}

void RenderWidget::createTextIBO()
{
	iboText = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::IndexBuffer);
	iboText->create();
	iboText->bind();
	iboText->allocate(indicesText, sizeof(indicesText));
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

	
	createBoxIBO(); 

	// 绘制矩形边框
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, nullptr);
	
	boxColliderShaderProgram->release();
}


//
void RenderWidget::renderBox()
{
	vaoBox->bind();
	
	//ibo->release(); // 释放画图像的ibo
	//createBoxEBO(); // 重新绑定画盒子的ibo
	//iboBox->bind();
	

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(3.0f);

	// 绘制矩形边框
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, nullptr);
	
	boxColliderShaderProgram->release();
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

	// 设置文字的对齐方式为左上对齐，并启用自动换行
	QTextOption option(Qt::AlignLeft | Qt::AlignTop);
	
	//option.setWrapMode(QTextOption::WordWrap);
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
	Vector2D pos = Vector2D(event->localPos().x(),size().height() - event->localPos().y());
	Debug::log("mouse position: " + pos.tostring());
	if(SceneMgr::get_instance().get_main_camera()!=nullptr)
		Debug::log("camera position: "+SceneMgr::get_instance().get_main_camera()->screenToWorld(pos).tostring());
}






