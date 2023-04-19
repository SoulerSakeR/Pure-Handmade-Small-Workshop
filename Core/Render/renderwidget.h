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
#include <Core/Core/Camera.h>



class RenderWidget : public QOpenGLWidget,public QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
    friend class RenderWindow;
public:
    enum Shape { None, Rect, Circle, Triangle };
    explicit RenderWidget(QWidget *parent = nullptr);
    ~RenderWidget();

    void drawShape(Shape shape);
    void setWirefame(bool wireframe);
    int frameCount;
    void cleanup();

    static RenderWidget& getInstance(); 
  
    void renderScene(Camera* camera);
    void renderGameobject(GameObject* gameobj, Camera* camera);
    void renderBoxCollider(BoxCollider* boxCollider, Camera* boxColliderCamera);
    void renderImage(Image* image, Camera* imageCamera);
    void renderText(Text* text, Camera* textCamera);
    
    void mouseMoveEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

    float moveSpeed;
    QPoint lastPos; //  Û±ÍŒª÷√

    bool editMode = true;

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

signals:

public slots:

    void on_timeout();
    void messageLogHandler(const QOpenGLDebugMessage &debugMessage);

private:

    static RenderWidget* sceneWidget;
    static RenderWidget* gameWidget;

    std::unordered_map<std::string, QOpenGLTexture*>textures;
    
    Shape m_shape;
  
    std::unique_ptr<QOpenGLShaderProgram> imageShaderProgram;   
    std::unique_ptr<QOpenGLShaderProgram> boxColliderShaderProgram;
    std::unique_ptr<QOpenGLShaderProgram> textShaderProgram;
    std::unique_ptr<QOpenGLShaderProgram> textureShaderProgram;

    std::unique_ptr<QOpenGLVertexArrayObject> vao;
    std::unique_ptr<QOpenGLVertexArrayObject> vaoBox;
    std::unique_ptr<QOpenGLVertexArrayObject> vaoText;
    std::unique_ptr<QOpenGLVertexArrayObject> vaoTexture;

    std::unique_ptr<QOpenGLBuffer> vbo;
    std::unique_ptr<QOpenGLBuffer> vboBox;
    std::unique_ptr<QOpenGLBuffer> vboText;
    std::unique_ptr<QOpenGLBuffer> vboTexture;

    std::unique_ptr<QOpenGLBuffer> ibo;
    std::unique_ptr<QOpenGLBuffer> iboBox;
    std::unique_ptr<QOpenGLBuffer> iboText;
    std::unique_ptr<QOpenGLBuffer> iboTexture;

    std::unique_ptr<QOpenGLDebugLogger> logger;

    QTimer timer;
    GLint imageTextureBinding;
    GLint shaderOffsetBinding;
    GLint shaderSizeBinding;

    // Texture
    void createProgram();    
    // BoxCollider
    void createBoxProgram();
    // Text
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

    
    QOpenGLFramebufferObject* fbo;
    QOpenGLFramebufferObject* fboOverlay;

    GameObject* mCameraObject;
    Camera*  mCamera;

};

#endif // RENDERWIDGET_H
