#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include "redblacktree.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class QPainter;

class OpenglWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

protected:
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(GLint w, GLint h) override;

public:
    OpenglWidget(QWidget *parent = nullptr);
    ~OpenglWidget();
void drawNode(RBNode<int, int>* node);
void drawNodeLine(RBNode<int, int>* node);
void drawTextKey(QPainter* painter, RBNode<int, int>* node);

private slots:
     void on_pbtInsert_clicked();
     void on_pushButtonShow_clicked();
     void on_pbtDelete_clicked();
private:
    QOpenGLShaderProgram shaderProgram;
    QOpenGLShaderProgram lineShaderProgram;
    QOpenGLShader* shader_V;
    QOpenGLShader* shader_F;
    QOpenGLShader* lineShader_V;
    QOpenGLShader* lineShader_F;
    RBTree<int, int>* rbtree;
    Ui::Widget *ui;
};
#endif // OPENGLWIDGET_H
