#include "openglwidget.h"
#include "ui_widget.h"
#include <QPainter>
#include "texture.h"

static GLuint VAO, VBO, lineVAO, lineVBO, texture;
const float WINDOW_WIDTH=1280.0, WINDOW_HEIGHT=720.0;
extern const float nodeLength=25.0;
static GLfloat vertices[] = {
    // Pos      // Tex
    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,

    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f
};

static GLfloat lineVertices[] = {
    // Pos
    1.0f, 1.0f,
    1.0f, 1.0f
};
void OpenglWidget::initializeGL()
{
    this->initializeOpenGLFunctions();
    shader_V=new QOpenGLShader(QOpenGLShader::Vertex);
    shader_F=new QOpenGLShader(QOpenGLShader::Fragment);

    lineShader_V=new QOpenGLShader(QOpenGLShader::Vertex);
    lineShader_F=new QOpenGLShader(QOpenGLShader::Fragment);

    bool success=shader_F->compileSourceFile("recShader_F.txt");
    if(!success){
        qDebug()<<"Fragment Shader compileSourceFile failed:"<<shader_F->log();
    }

    success=shader_V->compileSourceFile("recShader_V.txt");
        if(!success){
            qDebug()<<"Vertex Shader compileSourceFile failed:"<<shader_V->log();
        }
    shaderProgram.addShader(shader_V);
    shaderProgram.addShader(shader_F);
    success=shaderProgram.link();
    if(!success){
        qDebug()<<"ShaderPrograme link failed :"<<shaderProgram.log();
    }
//////////////////////////////////////////////////////
    success=lineShader_F->compileSourceFile("LineShader_F.txt");
    if(!success){
        qDebug()<<"Line Fragment Shader compileSourceFile failed:"<<lineShader_F->log();
    }

    success=lineShader_V->compileSourceFile("LineShader_V.txt");
        if(!success){
            qDebug()<<"Line Vertex Shader compileSourceFile failed:"<<lineShader_V->log();
        }
    lineShaderProgram.addShader(lineShader_V);
    lineShaderProgram.addShader(lineShader_F);
    success=lineShaderProgram.link();
    if(!success){
        qDebug()<<"ShaderPrograme link failed :"<<lineShaderProgram.log();
    }

    //VAO VBO EBO   
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4 , GL_FLOAT, GL_FALSE, 4 * sizeof (GL_FLOAT), (void*)0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &lineVAO);
    glGenBuffers(1, &lineVBO);
    glBindVertexArray(lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof (lineVertices), lineVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2 , GL_FLOAT, GL_FALSE, 2 * sizeof (GL_FLOAT), (void*)0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    //Texture
    TextureCreator textureCreator;
    textureCreator.create2DTexture("Red_Circle.png", texture);
    shaderProgram.bind();
    shaderProgram.setUniformValue("image", 0);

    //Translation
    QMatrix4x4 projection;
    projection.ortho(0.0, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0, -1.0, 1.0);
    shaderProgram.bind();
    shaderProgram.setUniformValue("projection", projection);
    lineShaderProgram.bind();
    lineShaderProgram.setUniformValue("projection", projection);

    //opengl config
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenglWidget::paintGL()
{

    QPainter painter(this);
    painter.beginNativePainting();
    //opengl config
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    drawNodeLine(rbtree->root);
    drawNode(rbtree->root);
    painter.endNativePainting();

    QFont font = painter.font();
    font.setPixelSize(12);
    painter.setFont(font);
    QPen pen = painter.pen();
    pen.setColor(QColor(255.0, 255.0, 255.0));
    painter.setPen(pen);
    drawTextKey(&painter, rbtree->root);
}

void OpenglWidget::resizeGL(GLint w, GLint h)
{
    glViewport(0,0,w,h);
}

OpenglWidget::OpenglWidget(QWidget *parent)
    : QOpenGLWidget(parent),ui(new Ui::Widget),rbtree(new RBTree<int, int>())
{
    ui->setupUi(this);
    this->resize(WINDOW_WIDTH,WINDOW_HEIGHT);
}

OpenglWidget::~OpenglWidget()
{
    delete  shader_F;
    delete  shader_V;
    delete  lineShader_V;
    delete  lineShader_F;
    delete  ui;
    delete  rbtree;
}

void OpenglWidget::drawNode(RBNode<int, int> *node)  //递归显示节点
{
    if(node==NULL)
        return;
    shaderProgram.bind();
    glBindVertexArray(VAO);
    QMatrix4x4 model;
    model.translate(QVector3D(node->posXTree,node->posYTree,0.0));
    model.scale(QVector3D(node->size,1.0));
    shaderProgram.setUniformValue("model", model);
    QVector3D color;
    if(node->color==RED){
        color=QVector3D(1.0, 1.0, 1.0);
    }else{
        color=QVector3D(0.1, 0.1, 0.1);
    }
    shaderProgram.setUniformValue("Color", color);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLES,0,6);
    glBindVertexArray(0);
    shaderProgram.release();
    drawNode(node->leftChild);
    drawNode(node->rightChild);
}

void OpenglWidget::drawNodeLine(RBNode<int, int> *node) //递归显示节点与节点的连线
{
    if(node==NULL)
        return;
    if(node->leftChild!=NULL){
        lineVertices[0]=node->posXTree+nodeLength*0.5;
        lineVertices[1]=node->posYTree+nodeLength*0.5;
        lineVertices[2]=node->leftChild->posXTree+nodeLength*0.5;
        lineVertices[3]=node->leftChild->posYTree+nodeLength*0.5;
        glGenVertexArrays(1, &lineVAO);
        glGenBuffers(1, &lineVBO);
        glBindVertexArray(lineVAO);
        glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof (lineVertices), lineVertices, GL_STATIC_DRAW);         glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2 , GL_FLOAT, GL_FALSE, 2 * sizeof (GL_FLOAT), (void*)0);
        lineShaderProgram.bind();
        glDrawArrays(GL_LINES,0,2);
        glBindVertexArray(0);
        lineShaderProgram.release();
        glDeleteBuffers(1, &lineVBO);
        glDeleteVertexArrays(1, &lineVAO);

    }
    if(node->rightChild!=NULL){
        lineVertices[0]=node->posXTree+nodeLength*0.5;
        lineVertices[1]=node->posYTree+nodeLength*0.5;
        lineVertices[2]=node->rightChild->posXTree+nodeLength*0.5;
        lineVertices[3]=node->rightChild->posYTree+nodeLength*0.5;
        glGenVertexArrays(1, &lineVAO);
        glGenBuffers(1, &lineVBO);
        glBindVertexArray(lineVAO);
        glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof (lineVertices), lineVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2 , GL_FLOAT, GL_FALSE, 2 * sizeof (GL_FLOAT), (void*)0);
        lineShaderProgram.bind();
        glDrawArrays(GL_LINES,0,2);
        glBindVertexArray(0);
        lineShaderProgram.release();
        glDeleteBuffers(1, &lineVBO);
        glDeleteVertexArrays(1, &lineVAO);
    }
    drawNodeLine(node->leftChild);
    drawNodeLine(node->rightChild);
}

void OpenglWidget::drawTextKey(QPainter *painter, RBNode<int, int> *node)  //递归显示节点值
{
    if(node==NULL)
        return;
    QRect rectangle = QRect(node->posXTree+5.0, node->posYTree+4.0, 20, 15);
    QRect boundingRect;
    painter->drawText(rectangle, 0, QString::number(node->key), &boundingRect);
    drawTextKey(painter, node->leftChild);
    drawTextKey(painter, node->rightChild);
}


//按钮响应函数
void OpenglWidget::on_pbtInsert_clicked()
{
    rbtree->insertNode(RBNode<int, int>(ui->spinBoxKeyInst->value(), ui->spinBoxValueInst->value()));
    rbtree->updateTreePos(0,0);
    update();
}

void OpenglWidget::on_pushButtonShow_clicked()
{
    update();
}

void OpenglWidget::on_pbtDelete_clicked()
{
    rbtree->deleteNode(ui->spinBox_->value());
    rbtree->updateTreePos(0,0);
    update();
}


