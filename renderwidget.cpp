#include "renderwidget.h"



unsigned int VBO, VAO, EBO;

/*
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n" "void main()\n"
"{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
" FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n" "}\n\0";
*/

/*
float vertices[] = {
    // positions          // colors           // texture coords
     0.5f ,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
     0.5f , -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
    -0.5f , -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
    -0.5f ,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};
*/


float vertices[] = {
    // positions          // colors           // texture coords
     0.5f * 9.0 / 16.0,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
     0.5f * 9.0 / 16.0, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
    -0.5f * 9.0 / 16.0, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
    -0.5f * 9.0 / 16.0,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};


/*
float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};
*/


unsigned int indices[] = { // note that we start from 0!
0, 1, 3, // first triangle
1, 2, 3 // second triangle
};


RenderWidget::RenderWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    timer.start(100);
    connect(&timer, SIGNAL(timeout()), this, SLOT(on_timeout()));
    //connect(&timer, SIGNAL(timeout()), this, SLOT(on_timeout()));
}

RenderWidget::~RenderWidget()
{
    if (!isValid()) return;
    makeCurrent();
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &EBO);
    doneCurrent();
}

void RenderWidget::drawShape(Shape shape)
{
    m_shape = shape;
    update();
}

void RenderWidget::setWirefame(bool wireframe)
{
    makeCurrent();
    
    if(wireframe)
        // ����
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    update();
    doneCurrent();
}

void RenderWidget::initializeGL()
{
    initializeOpenGLFunctions();


   
    bool success;
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "./shaders/shaders.vert");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "./shaders/shaders.frag");

   
    

    success = shaderProgram.link();
    if (!success)
        qDebug() << "ERR:" << shaderProgram.log();

    



    //����VBO��VAO���󣬲�����ID
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //��VBO��VAO����
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //Ϊ��ǰ�󶨵�target�Ļ��������󴴽�һ���µ����ݴ洢��
    //���data����NULL����ʹ�����Դ�ָ������ݳ�ʼ�����ݴ洢
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // ��shader����ʼλ�ã�����ͬʱ�޸������е�poslocation
    shaderProgram.bind();
    GLint posLocation = shaderProgram.attributeLocation("aPos");
    GLint colorLocation = shaderProgram.attributeLocation("aColor");
    GLint textureLocation = shaderProgram.attributeLocation("aTexture");



    //-----------------position--------------------//
    //��֪�Կ���ν��������������ֵ
    glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    //����VAO�����ĵ�һ������ֵ
    glEnableVertexAttribArray(posLocation);

    //------------------Color-----------------------//
    //��֪�Կ���ν��������������ֵ
    glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    //����VAO�����ĵ�һ������ֵ
    glEnableVertexAttribArray(colorLocation);

    //------------------Texture-----------------------//
    //��֪�Կ���ν��������������ֵ
    glVertexAttribPointer(textureLocation, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    //����VAO�����ĵ�һ������ֵ
    glEnableVertexAttribArray(textureLocation);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    

    // EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    
    
    

    // ƫ��
    shaderProgram.bind();
    shaderProgram.setUniformValue("xOffset", 0.4f);


    texture0 = new QOpenGLTexture(QImage("./imageTest/boss_hornet.png").mirrored());


    // ���
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void RenderWidget::resizeGL(int w, int h)
{
    Q_UNUSED(w); Q_UNUSED(h);
    //glViewport(0, 0, w, h);
}

void RenderWidget::paintGL()
{
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

   // ͸��
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    shaderProgram.bind();
    glBindVertexArray(VAO);
    switch (m_shape) {
        case Rect:
            texture0->bind(0);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
            break;
        default:
            break;
        }
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, &indices );
}

void RenderWidget::on_timeout()
{
    //if (m_shape = None) return;

    makeCurrent();

    int timeValue = QDateTime::currentDateTime().time().second();
    float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
    shaderProgram.setUniformValue("ourColor", 0.0f, greenValue, 0.0f, 1.0f);

    doneCurrent();
    update();
}
