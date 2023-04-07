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

#include "common.h"

class RenderWidget : public QOpenGLWidget,public QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    enum Shape { None, Rect, Circle, Triangle };
    explicit RenderWidget(QWidget *parent = nullptr);
    ~RenderWidget();

    void drawShape(Shape shape);
    void setWirefame(bool wireframe);

    void cleanup();

    static RenderWidget& getInstance();

    float* getTextureVertices(QVector3D offset, QVector2D size);
    
    

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
    std::unique_ptr<QOpenGLVertexArrayObject> vao;
    std::unique_ptr<QOpenGLShaderProgram> shaderProgram;

    std::unique_ptr<QOpenGLBuffer> vbo;
    std::unique_ptr<QOpenGLBuffer> ibo;

    std::unique_ptr<QOpenGLDebugLogger> logger;

    QTimer timer;
    std::unique_ptr<QOpenGLTexture> textureSmile;
    std::unique_ptr<QOpenGLTexture> textureBoss;
    std::unique_ptr<QOpenGLTexture> textureWall;

    GLint textureWallBinding;

    GLint shaderOffsetBinding;
    GLint shaderSizeBinding;

    void createProgram();
    void createVAO();
    void createVBO();
    void createIBO();
    int renderMain(QString*, QVector3D*, QVector2D*);
    


    void renderTexture(QOpenGLTexture* texture,QVector3D offset,QVector2D size);


    

};

#endif // RENDERWIDGET_H
