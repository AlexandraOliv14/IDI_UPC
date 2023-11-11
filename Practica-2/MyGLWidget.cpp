#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__, __FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

int MyGLWidget::printOglError(const char file[], int line, const char func[])
{
  GLenum glErr;
  int retCode = 0;

  glErr = glGetError();
  const char *error = 0;
  switch (glErr)
  {
  case 0x0500:
    error = "GL_INVALID_ENUM";
    break;
  case 0x501:
    error = "GL_INVALID_VALUE";
    break;
  case 0x502:
    error = "GL_INVALID_OPERATION";
    break;
  case 0x503:
    error = "GL_STACK_OVERFLOW";
    break;
  case 0x504:
    error = "GL_STACK_UNDERFLOW";
    break;
  case 0x505:
    error = "GL_OUT_OF_MEMORY";
    break;
  default:
    error = "unknown error!";
  }
  if (glErr != GL_NO_ERROR)
  {
    printf("glError in file %s @ line %d: %s function: %s\n",
           file, line, error, func);
    retCode = 1;
  }
  return retCode;
}

MyGLWidget::~MyGLWidget()
{
}

void MyGLWidget::initializeGL()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();
  glEnable(GL_DEPTH_TEST);

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  LL2GLWidget::creaBuffersModels();
  LL2GLWidget::creaBuffersTerra();
  LL2GLWidget::iniEscena();
  iniCamera();
}

void MyGLWidget::iniCamera()
{
  LL2GLWidget::iniCamera();
  obs = glm::vec3(0, 10, 20);
  fov=45.0f;
  viewTransform();
  rotateObject(0.0f, 0.0f);
}

void MyGLWidget::paintGL()
{
  // descomentar per canviar paràmetres
  // glViewport (0, 0, ample, alt);

  // Esborrem el frame-buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Road
  glBindVertexArray(VAO_models[ROAD]);
  RoadTransform(glm::vec3(-5, 0.01, 5), 180.0f);
  glDrawArrays(GL_TRIANGLES, 0, models[ROAD].faces().size() * 3);

  glBindVertexArray(VAO_models[ROAD]);
  RoadTransform(glm::vec3(5, 0.01, 5), 270.0f);
  glDrawArrays(GL_TRIANGLES, 0, models[ROAD].faces().size() * 3);

  glBindVertexArray(VAO_models[ROAD]);
  RoadTransform(glm::vec3(5, 0.01, -5), 0.0f);
  glDrawArrays(GL_TRIANGLES, 0, models[ROAD].faces().size() * 3);

  glBindVertexArray(VAO_models[ROAD]);
  RoadTransform(glm::vec3(-5, 0.01, -5), 90.0f);
  glDrawArrays(GL_TRIANGLES, 0, models[ROAD].faces().size() * 3);
  
  // Pipe
  glBindVertexArray (VAO_models[PIPE]);
  PipeTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);
  

  // Terra
  glBindVertexArray(VAO_Terra);
  TerraTransform();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  // Car
  glBindVertexArray (VAO_models[CAR]);
  CarTransform(6.f,0.0f, angCarAzul, glm::vec3(0,0,1));
  glDrawArrays(GL_TRIANGLES, 0, models[CAR].faces().size()*3);

  // glBindVertexArray (VAO_models[CAR]);
  // CarTransform(7.5f,0.0f, glm::vec3(0,1,0));
  // glDrawArrays(GL_TRIANGLES, 0, models[CAR].faces().size()*3);

  // glBindVertexArray (VAO_models[CAR]);
  // CarTransform(9.f,0.0f, glm::vec3(1,0,0));
  // glDrawArrays(GL_TRIANGLES, 0, models[CAR].faces().size()*3);

  glBindVertexArray(0);
}

void MyGLWidget::resizeGL(int w, int h)
{
  // Aquest codi és necessari únicament per a MACs amb pantalla retina.
  #ifdef __APPLE__
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    ample = vp[2];
    alt = vp[3];
  #else
    ample = w;
    alt = h;
  #endif
    ra = float(ample) / float(alt);
    factorAngleY = M_PI / ample;
    factorAngleX = M_PI / alt;
    projectTransform();
}

void MyGLWidget::carregaShaders()
{
  LL2GLWidget::carregaShaders();
  colorLoc  = glGetUniformLocation (program->programId(), "newColor");
  viewRotateLoc  = glGetUniformLocation (program->programId(), "viewRotate");

  
}

void MyGLWidget::TerraTransform()
{
  glm::mat4 TG(1.0f);
  TG = glm::scale(TG, glm::vec3(2.5f, 1, 2.5f));
  TG = glm::translate(TG, centreEscena);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
  glm::vec3 color = glm::vec3(1, 1, 1);
  glUniform3fv(colorLoc, 1, &color[0]);
}

void MyGLWidget::RoadTransform(glm::vec3 pos, float angle)
{
  LL2GLWidget::RoadTransform(pos, angle);
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, pos);
  TG = glm::scale(TG, glm::vec3(11.11, 1,11.11));
  TG = glm::rotate(TG, glm::radians(angle), glm::vec3(0,1,0));
  TG = glm::translate(TG, -centreBaseModels[ROAD]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
  glm::vec3 color = glm::vec3(1, 1, 1);
  glUniform3fv(colorLoc, 1, &color[0]);
}

void MyGLWidget::PipeTransform (){
  LL2GLWidget::PipeTransform ();
  glm::mat4 TG(1.0f);
  TG = glm::scale(TG, glm::vec3(escalaModels[PIPE]*3, escalaModels[PIPE]*3, escalaModels[PIPE]*3));
  TG = glm::translate(TG, glm::vec3(0,0,0));
  TG = glm::translate(TG, -centreBaseModels[PIPE]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
  glm::vec3 color = glm::vec3(1, 1, 1);
  glUniform3fv(colorLoc, 1, &color[0]);
}

void MyGLWidget::CarTransform (float radio, float angleAuto, float angleMove, glm::vec3 color){
  LL2GLWidget::CarTransform (0.0, 0.0);

  glUniform3fv(colorLoc, 1, &color[0]);
  // Calcula la dirección en la que se moverá el auto
  float radianAngle = glm::radians(angleMove);
  glm::vec3 direction = glm::vec3(std::cos(radianAngle), 0, std::sin(radianAngle));

  // Calcula la nueva posición del auto
  glm::vec3 newPosition = centreEscena + direction*radio;

  // Llama a CarTransform para mover el auto a la nueva posición
  //CarTransform(newPosition.length(), angle, color);
  glm::mat4 TG(1.0f);
  TG = glm::rotate(TG, glm::radians(angleAuto), glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(newPosition));
  TG = glm::translate(TG, -centreBaseModels[CAR]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);

}


void MyGLWidget::viewTransform()
{
  LL2GLWidget::viewTransform();
}

void MyGLWidget::projectTransform()
{
  LL2GLWidget::projectTransform();
}

void MyGLWidget::keyPressEvent(QKeyEvent *event)
{
  makeCurrent();
  switch (event->key())
  {
  case Qt::Key_Up:
  {
    angCarAzul -= 1.0f;
    angCarVerde -= 2.0f;
    angCarRojo -= 3.0f;
    break;
  }
  case Qt::Key_C:
  {
    break;
  }
  case Qt::Key_R:
  {
    break;
  }
  case Qt::Key_T:
  {
    break;
  }
  default:
    event->ignore();
    break;
  }
  update();
}

void MyGLWidget::mousePressEvent(QMouseEvent *event)
{
  LL2GLWidget::mousePressEvent(event);
}
void MyGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
  LL2GLWidget::mouseReleaseEvent(event);
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  if(DoingInteractive==ROTATE){
    float angleY = (e->x() - xClick) * factorAngleY;
    float angleX = (e->y() - yClick) * factorAngleX;

    // Aplica la rotación a tu objeto
    rotateObject(angleX, angleY);
  }


  update ();
}

void MyGLWidget::rotateObject(float angleX, float angleY)
{
 
  glm::mat4 View(1.0f);

// Crear matrices de rotación para los ejes X e Y
  glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), angleX, glm::vec3(1.0, 0.0, 0.0));
  glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), angleY, glm::vec3(0.0, 1.0, 0.0));

  // mandar las rotaciones a el uniform
  View = rotationX * rotationY;
  glUniformMatrix4fv(viewRotateLoc, 1, GL_FALSE, &View[0][0]);

}