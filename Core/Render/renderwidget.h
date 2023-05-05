#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <algorithm>
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
#include <Core/UI/HierarchyWidget.h>
#include <Core/UI/GameWindow.h>
#include "Core/Core/SpineAnimator.h"
#include "Core/Render/LightSource.h"
#include "Core/Render/PointLight.h"


class GameLoop;


class RenderWidget : public QOpenGLWidget,public QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
    friend class MainWindow;
    friend class GameWindow;
public:
    enum Shape { None, Rect, Circle, Triangle };
    explicit RenderWidget(QWidget *parent = nullptr);
    ~RenderWidget();

    void drawShape(Shape shape);
    void setWirefame(bool wireframe);
    int frameCount;
    void cleanup();

    static RenderWidget& getSceneWidget(); 
    static RenderWidget& getGameWidget();
    static RenderWidget& getCurrentWidget();

    HierarchyWidget* hierarchyWidget;

    void drawMesh(IRenderable* target, Camera* camera,bool visBorder);
  
    void renderScene(Camera* camera);
    void renderGameobject(GameObject* gameobj, Camera* camera);
    void renderBoxCollider(BoxCollider* boxCollider, Camera* boxColliderCamera, bool visBorder);
    void renderImage(Image* image, Camera* imageCamera, bool visBorder);
    void renderText(Text* text, Camera* textCamera, bool visBorder);
    void renderSpineAnimator(SpineAnimator* spineAnimator, Camera* camera, bool visBorder);
    void renderCameraBorder(Camera* target, Camera* camera, bool visBorder);
    void renderFbo();
    void renderFboOverlay();
    void mixTexture();
    
    
    void mouseMoveEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;

    void setFullScreen(bool fullScreen);

    std::vector<GameObject*> hitRay(Vector2D screenPos);
    void lookAt(GameObject* target);
    void resetResolution();
    void resetFBO();

    void setLight(LightSource* light, GameObject* target);
    PointLight addPointLight();

    GameObject* getSelectedGameObject();

    float moveSpeed;
    int edgeNumber = 0; // 1: left, 2: top, 2: right, 3: bottom
    int connerNumber = 0; // 1: left-top, 2: right-top, 3: right-bottom, 4: left-bottom
    QPoint lastPos; // mouse location
    QPoint doubleClickPos; // mouse location
    QPoint lastMovePos; // object move last location

    static QOpenGLContext* sharedContext;
    bool editMode = true;
    bool isGameWidget = false;
    bool moveObjectMode = false;
    bool moveCameraMode = false;
    bool sizeChangeMode = false;
    bool fixedRatioMode = false;
    bool isFullScreen = false;
    bool isRendering = false;
    static bool widgetChanged;
    static RenderWidget* currentWidget;
    
    QRect m_normalGeometry; // record normal geometry

    GameLoop* gameLoop;

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

signals:
    void resizeGameWindow(bool isFullScreen);

public slots:

    void on_timeout();
    void messageLogHandler(const QOpenGLDebugMessage &debugMessage);
    static void on_widgetChanged(int index);

private:

    static RenderWidget* sceneWidget;
    static RenderWidget* gameWidget;

    

    std::unordered_map<std::string, QOpenGLTexture*>textures;
    
    Shape m_shape;
  
    std::unique_ptr<QOpenGLShaderProgram> imageShaderProgram;   
    std::unique_ptr<QOpenGLShaderProgram> boxColliderShaderProgram;
    std::unique_ptr<QOpenGLShaderProgram> textShaderProgram;
    std::unique_ptr<QOpenGLShaderProgram> textureShaderProgram;
    std::unique_ptr<QOpenGLShaderProgram> cameraBorderShaderProgram;

    std::unique_ptr<QOpenGLVertexArrayObject> vao;
    std::unique_ptr<QOpenGLVertexArrayObject> vaoBox;
    std::unique_ptr<QOpenGLVertexArrayObject> vaoText;
    std::unique_ptr<QOpenGLVertexArrayObject> vaoTexture;
    std::unique_ptr<QOpenGLVertexArrayObject> vaoCameraBorder;

    std::unique_ptr<QOpenGLBuffer> vbo;
    std::unique_ptr<QOpenGLBuffer> vboBox;
    std::unique_ptr<QOpenGLBuffer> vboText;
    std::unique_ptr<QOpenGLBuffer> vboTexture;
    std::unique_ptr<QOpenGLBuffer> vboCameraBorder;

    std::unique_ptr<QOpenGLBuffer> ibo;
    std::unique_ptr<QOpenGLBuffer> iboBox;
    std::unique_ptr<QOpenGLBuffer> iboText;
    std::unique_ptr<QOpenGLBuffer> iboTexture;
    std::unique_ptr<QOpenGLBuffer> iboCameraBorder;

    std::unique_ptr<QOpenGLDebugLogger> logger;

    QTimer timer;
    GLint imageTextureBinding;
    GLint shaderOffsetBinding;
    GLint shaderSizeBinding;

    
    // Text
    void createTextProgram();
    // Texture
    void createTextureProgram();

    void createALLShaderProgram(); // image, boxcollider,cameraBorder
    
    QOpenGLTexture* genTextTexture(int width, int height, const QString& text, int textPixelSize, const QColor& textColor);
    
    std::unique_ptr<QOpenGLBuffer> textBuffer;
    std::unique_ptr<QOpenGLTexture> *textTexture;
    
    QOpenGLTexture* mTexture = nullptr;

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

    
    GLenum glCheckError_(const char* file, int line);
};

#endif // RENDERWIDGET_H
