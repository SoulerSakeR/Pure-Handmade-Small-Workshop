#include "renderwidget.h"
#include <iostream>
#define SOURCE_DIR "E:/GroupProject/PHE(3)/"


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

RenderWidget::RenderWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    timer.start(100);
    //    connect(&timer, SIGNAL(timeout()), this, SLOT(on_timeout()));
    //connect(&timer, SIGNAL(timeout()), this, SLOT(on_timeout()));
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

    if(wireframe)
        //         画线
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    update();
    doneCurrent();
}

void RenderWidget::initializeGL()
{
    initializeOpenGLFunctions();

    logger=std::make_unique<QOpenGLDebugLogger>(this);
    logger->initialize();
    connect(logger.get(),&QOpenGLDebugLogger::messageLogged,this,&RenderWidget::messageLogHandler);
    logger->startLogging();

    glClearColor(0.2,0.2,0.2,1);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glEnable(GL_DEPTH_TEST);

    createProgram();
    createVBO();
    createVAO();
    createIBO();

    std::cout << SOURCE_DIR "resources/awesomeface.png" << std::endl;

    textureSmile = std::make_unique<QOpenGLTexture>(QImage(SOURCE_DIR "resources/awesomeface.png").mirrored(), QOpenGLTexture::GenerateMipMaps);
    textureSmile->create();

    textureWall = std::make_unique<QOpenGLTexture>(QImage(SOURCE_DIR "resources/container.jpg").mirrored(), QOpenGLTexture::GenerateMipMaps);
    textureWall->create();

}

void RenderWidget::resizeGL(int w, int h)
{
    Q_UNUSED(w); Q_UNUSED(h);
    glViewport(0, 0, w, h);
}

void RenderWidget::paintGL()
{
    makeCurrent();
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    shaderProgram->bind();

    renderTexture(textureWall.get(),{},{0.3,0.3});
    renderTexture(textureSmile.get(),{0.2,0.2,0.1},{0.3,0.3});

    //    if(textureSmileBinding>=0)
    //        textureSmile->bind(textureSmileBinding);
    //    glBindVertexArray(VAO);
    //    switch (m_shape){
    //    case Rect:
    //        glActiveTexture(GL_TEXTURE0);
    //        //textureWall->bind(0);
    //        //glActiveTexture(GL_TEXTURE1);
    //        textureSmile->bind(1);
    //        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
    //        break;
    //    default:
    //        break;
    //    }
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, &indices );
}

void RenderWidget::on_timeout()
{
    //if (m_shape = None) return;

    //    makeCurrent();

    //    int timeValue = QDateTime::currentDateTime().time().second();
    //    float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
    //    shaderProgram.setUniformValue("ourColor", 0.0f, greenValue, 0.0f, 1.0f);

    //    doneCurrent();
    //    update();
}
void RenderWidget::cleanup()
{

}

void RenderWidget::createProgram()
{
    bool success;
    shaderProgram=std::make_unique<QOpenGLShaderProgram>();
    shaderProgram->create();
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex,  SOURCE_DIR "shaders/shaders.vert");
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, SOURCE_DIR "shaders/shaders.frag");
    success = shaderProgram->link(); //
    if (!success)
        qDebug() << "ERR:" << shaderProgram->log();

    //        textureSmileBinding=shaderProgram->uniformLocation("textureSmile");
    //        textureSmileBinding=shaderProgram->uniformLocation("textureSmile");

    shaderOffsetBinding=shaderProgram->uniformLocation("offset");
    shaderSizeBinding=shaderProgram->uniformLocation("size");
    //    textureWallBinding=shaderProgram->uniformLocation("textureWall");
    textureWallBinding=0;
}
void RenderWidget::createVAO()
{
    // 找shader的起始位置，并且同时修改下两行的poslocation
    shaderProgram->bind();
    GLint posLocation = shaderProgram->attributeLocation("aPos");
    GLint colorLocation = shaderProgram->attributeLocation("aColor");
    GLint textureLocation = shaderProgram->attributeLocation("aTexCord");

    vao=std::make_unique<QOpenGLVertexArrayObject>();

    vao->create();

    vao->bind();
    vbo->bind();

    auto stride=8*sizeof(float);

    //-----------------position--------------------//
    //告知显卡如何解析缓冲里的属性值
    glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);

    //开启VAO管理的第一个属性值
    glEnableVertexAttribArray(posLocation);

    if(colorLocation>=0)
    {
        //------------------Color-----------------------//
        //告知显卡如何解析缓冲里的属性值
        glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE,stride , (void*)(3 * sizeof(float)));
        //开启VAO管理的第一个属性值
        glEnableVertexAttribArray(colorLocation);
    }
    if(textureLocation)
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
    vbo=std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::VertexBuffer);
    vbo->create();
    vbo->bind();
    vbo->allocate(vertices,sizeof(vertices));
}
void RenderWidget::createIBO()
{
    ibo=std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::IndexBuffer);
    ibo->create();
    ibo->bind();
    ibo->allocate(indices,sizeof(indices));
}
void RenderWidget::messageLogHandler(const QOpenGLDebugMessage &debugMessage)
{
    qDebug()<<debugMessage.message();
}
void RenderWidget::renderTexture(QOpenGLTexture* texture,QVector3D offset,QVector2D size)
{
    shaderProgram->setUniformValue(shaderOffsetBinding,offset);
    shaderProgram->setUniformValue(shaderSizeBinding,size);
    vao->bind();
    ibo->bind();
    if(textureWallBinding>=0)
        texture->bind(textureWallBinding);
    glDrawElements(GL_TRIANGLES,std::size(indices),GL_UNSIGNED_INT,0);
}
