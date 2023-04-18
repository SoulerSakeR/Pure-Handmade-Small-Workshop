#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QDateTime>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLDebugLogger>
#include <Core/Core/BoxCollider.h>
#include <Core/Core/Image.h>
#include <Core/Core/Text.h>
#include <QOpenGLFramebufferObject>



class RenderWidget : public QOpenGLWidget,public QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    enum Shape { None, Rect, Circle, Triangle };
    explicit RenderWidget(QWidget *parent = nullptr);
    ~RenderWidget();

    void drawShape(Shape shape);
    void setWirefame(bool wireframe);
    int frameCount;
    void cleanup();

    static RenderWidget& getInstance();

    
  
    void renderScene();
    void renderGameobject(GameObject* gameobj);
    void renderBoxCollider(BoxCollider* boxCollider);
    void renderImage(Image* image);
    void renderText(Text* text);
    void mouseMoveEvent(QMouseEvent* event) override;

    

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

signals:

public slots:

    void on_timeout();
    void messageLogHandler(const QOpenGLDebugMessage &debugMessage);

private:

    static RenderWidget* instance;


    std::unordered_map<std::string, QOpenGLTexture*>textures;
    
    Shape m_shape;
    



   
    std::unique_ptr<QOpenGLShaderProgram> imageShaderProgram;   
    std::unique_ptr<QOpenGLShaderProgram> boxColliderShaderProgram;
    std::unique_ptr<QOpenGLShaderProgram> textShaderProgram;

    std::unique_ptr<QOpenGLVertexArrayObject> vao;
    std::unique_ptr<QOpenGLVertexArrayObject> vaoBox;
    std::unique_ptr<QOpenGLVertexArrayObject> vaoText;

    std::unique_ptr<QOpenGLBuffer> vbo;
    std::unique_ptr<QOpenGLBuffer> vboBox;
    std::unique_ptr<QOpenGLBuffer> vboText;

    std::unique_ptr<QOpenGLBuffer> ibo;
    std::unique_ptr<QOpenGLBuffer> iboBox;
    std::unique_ptr<QOpenGLBuffer> iboText;

    std::unique_ptr<QOpenGLDebugLogger> logger;

    QTimer timer;
    std::unique_ptr<QOpenGLTexture> textureSmile;
    std::unique_ptr<QOpenGLTexture> textureBoss;
    std::unique_ptr<QOpenGLTexture> textureWall;

    GLint textureWallBinding;

    GLint shaderOffsetBinding;
    GLint shaderSizeBinding;

    // Texture
    void createProgram();
    void createVAO();
    void createVBO();
    void createIBO();

    // BoxCollider
    void createBoxProgram();
    void createBoxVAO();   
    void createBoxVBO();
    void createBoxIBO();
    

   
   



    void renderTexture(QOpenGLTexture* texture,QVector3D offset,QVector2D size);

    void renderBox();
    
    // void getTextureInfo(Image& imgComponent, QString* texturePathQ, QVector3D* offset, QVector2D* size);

    // text
    void createTextProgram();
    

    QOpenGLTexture* genTextTexture(int width, int height, const QString& text, int textPixelSize, const QColor& textColor);
    
    std::unique_ptr<QOpenGLBuffer> textBuffer;
    std::unique_ptr<QOpenGLTexture> *textTexture;
    
    QOpenGLTexture* mTexture;

    QMatrix4x4 mProjMatrix;
    GLuint mPosAttr;
    GLuint mTexAttr;
    GLuint mMatrixLoc;
    GLuint mProjLoc;
    GLuint mVertexCount;

    void createTextVAO();
    void createTextVBO();
    void createTextIBO();


    QOpenGLFramebufferObject* fbo;


};

#endif // RENDERWIDGET_H
