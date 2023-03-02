#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <QtOpenGL/qopenglfunctions_3_3_core.h>
#include <QtOpenGLWidgets/qopenglwidget.h>

#include <QtOpenGL/qopenglshaderprogram.h>

class RenderWidget : public QOpenGLWidget, QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    enum Shape { None, Rect, Circle, Triangle };
    explicit RenderWidget(QWidget *parent = nullptr);
    ~RenderWidget();

    void drawShape(Shape shape);
    void setWirefame(bool wireframe);
protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
signals:

public slots:
private:
    Shape m_shape;
    QOpenGLShaderProgram shaderProgram;
};

#endif // RENDERWIDGET_H