#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QDateTime>
#include <QOpenGLTexture>

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
    void on_timeout();
private:
    Shape m_shape;
    QOpenGLShaderProgram shaderProgram;
    QTimer timer;
    QOpenGLTexture* texture0;
};

#endif // RENDERWIDGET_H
