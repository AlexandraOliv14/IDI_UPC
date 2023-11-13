// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>
#include <QTimer>

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

  //LL2GLWidget::iniCamera();
  distanciaCamara = radiEscena*4;
  iniCamera();

  if(!camaraPrimeraPersona){
      iniCamera();
      viewTransform();
      projectTransform();
    }else{
      FirstPersonCameraTransform(7.5f,angCarVerde);
      LL2GLWidget::viewTransform();
      projectTransform();
    }

  timer = new QTimer(this);  // Inicializa el timer
  connect(timer, SIGNAL(timeout()), this, SLOT(animacion()));  // Conecta la señal timeout() a la ranura animacion()
}

void MyGLWidget::iniCamera()
{
  float dist = radiEscena * 2.0; // La mitad de la diagonal del terreno
  obs = glm::vec3(0, dist, 0.1); // Eleva la cámara en el eje Y
  vrp = glm::vec3(0, 0, 0); // Mira hacia el centro de la escena
  up = glm::vec3(0, 0, -1); // Vector 'up' correcto
  fov = glm::radians(45.0f); // Campo de visión para incluir toda la escena
  znear = 0.1f;
  zfar = 3 * dist;
  viewTransform();
  projectTransform();                  // Lejos del plano de recorte, suficientemente grande para ver toda la escena
  //rotateCamara(anguloX, anguloY);
}
void MyGLWidget::iniCamera2()
{
  // obs = posicionCamP;
  // vrp = vistaCamP;
  // up = up;
  // fov = glm::radians(60.0f);
  // znear =  0.25f;
  // zfar  = 5.f;
  psi = angMoveCarVerde;
  theta =angCarVerde;

  viewTransform();
  projectTransform();
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
  CarTransform(6.f, angCarAzul,angMoveCarAzul, glm::vec3(0,0,1));
  glDrawArrays(GL_TRIANGLES, 0, models[CAR].faces().size()*3);

  glBindVertexArray (VAO_models[CAR]);
  CarTransform(7.5f,angCarVerde, angMoveCarVerde, glm::vec3(0,1,0));
  glDrawArrays(GL_TRIANGLES, 0, models[CAR].faces().size()*3);

  glBindVertexArray (VAO_models[CAR]);
  CarTransform(9.f,angCarRojo, angMoveCarRojo, glm::vec3(1,0,0));
  glDrawArrays(GL_TRIANGLES, 0, models[CAR].faces().size()*3);

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

  //calculaCapsaModel(models[ROAD], escalaModels[ROAD], 10.0f, centreBaseModels[ROAD]);
  float tamanoOriginalCarretera = 2.f/escalaModels[ROAD]/* tamaño original */;
  float escalaDeseada = 10.0f / tamanoOriginalCarretera;

  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, pos); // Posicionamos la carretera
  TG = glm::scale(TG, glm::vec3(escalaDeseada, 1.0f, escalaDeseada)); // Escalamos el segmento de carretera a 10x10 unidades
  TG = glm::rotate(TG, glm::radians(angle), glm::vec3(0, 1, 0)); // Aplicamos la rotación indicada
  TG = glm::translate(TG, -centreBaseModels[ROAD]); // Centramos la carretera en su base

  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
  glm::vec3 color = glm::vec3(1, 1, 1); // Suponiendo que queremos que la carretera sea de color blanco
  glUniform3fv(colorLoc, 1, &color[0]);
}

void MyGLWidget::PipeTransform (){
  LL2GLWidget::PipeTransform ();
  //calculaCapsaModel(models[PIPE], escalaModels[PIPE], 3.f, centreBaseModels[PIPE]);
  float tamanoOriginalCarretera = 1.f/escalaModels[PIPE]/* tamaño original */;
  float escalaDeseada = 3.0f / tamanoOriginalCarretera;

  glm::mat4 TG(1.0f);
  TG = glm::scale(TG, glm::vec3(escalaDeseada));
  TG = glm::translate(TG, glm::vec3(0,0,0));
  TG = glm::translate(TG, -centreBaseModels[PIPE]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
  glm::vec3 color = glm::vec3(1, 1, 1);
  glUniform3fv(colorLoc, 1, &color[0]);
}

void MyGLWidget::CarTransform (float radio, float angleAuto, float angleMove, glm::vec3 color){
  LL2GLWidget::CarTransform (0.0, 0.0);
  float tamanoOriginalCarretera = 2.f/escalaModels[CAR]/* tamaño original */;
  float escalaDeseada = 2.0f / tamanoOriginalCarretera;

  //calculaCapsaModel(models[CAR], escalaModels[CAR], 2.f, centreBaseModels[CAR]);

  glUniform3fv(colorLoc, 1, &color[0]);
  // Calcula la dirección en la que se moverá el auto
  float radianAngle = glm::radians(angleMove);
  glm::vec3 direction = glm::vec3(std::cos(radianAngle), 0, std::sin(radianAngle));

  // Calcula la nueva posición del auto
  glm::vec3 newPosition = centreEscena + direction*radio;

  // Transformaciones
  glm::mat4 TG(1.0f);
  TG = glm::scale(TG, glm::vec3(escalaDeseada,1,escalaDeseada));
  TG = glm::translate(TG, glm::vec3(newPosition));
  TG = glm::rotate(TG, glm::radians(angleAuto), glm::vec3(0,1,0));
  TG = glm::translate(TG, -centreBaseModels[CAR]);

  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);

}


void MyGLWidget::viewTransform()
{
   // Este valor depende del tamaño de tu escena distanciaCamara
  glm::vec3 VRP(0.0f, 0.0f, 0.0f); // El centro de la escena

  // Inicializamos la matriz de vista (VM) con la identidad
  glm::mat4 VM = glm::mat4(1.0f);

  // Aplicamos las transformaciones en el orden correcto
  VM = glm::translate(VM, glm::vec3(0.0f, 0.0f, -distanciaCamara)); // Nos alejamos del centro
  VM = glm::rotate(VM, -psi, glm::vec3(0, 1, 0)); // Rotación horizontal
  VM = glm::rotate(VM, theta, glm::vec3(1, 0, 0)); // Rotación vertical
  VM = glm::rotate(VM, -psi, glm::vec3(0, 1, 0)); // Podría ser otra rotación si es necesario
  VM = glm::translate(VM, -VRP); // Centramos la cámara en el VRP

  // Enviamos la matriz al shader
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &VM[0][0]);
}

void MyGLWidget::FirstPersonCameraTransform(float radio, float angleAuto) {
  // Este valor depende del tamaño de tu escena distanciaCamara
  glm::mat4 VM = glm::lookAt(posicionCamP, vistaCamP, glm::vec3(0, 1, 0));
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &VM[0][0]);

  fov= glm::radians(60.0f);
}

void MyGLWidget::projectTransform()
{
  glm::mat4 Proj(1.0f);
  Proj = glm::perspective (fov, ra, znear, zfar);
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::keyPressEvent(QKeyEvent *event)
{
  makeCurrent();
  switch (event->key())
  {
  case Qt::Key_Up:
  {
    movimiento();
    
    if(camaraPrimeraPersona){
      // obs = posicionCamP;
      // vrp = vistaCamP;
      // up = up;
      FirstPersonCameraTransform(7.5f,angCarVerde);
      //viewTransform();
      projectTransform();
    }
    break;
  }
  case Qt::Key_C:
  {
    camaraPrimeraPersona= !camaraPrimeraPersona;
    if(!camaraPrimeraPersona){
      psi = 0.f;
      theta = glm::radians(45.0f);
      distanciaCamara = radiEscena*4;
      iniCamera();
      viewTransform();
    }else{
      psi = angMoveCarVerde - 90.f;
      theta =glm::radians(60.0f);;
      distanciaCamara= 7.5f;
      FirstPersonCameraTransform(7.5f,angCarVerde);
      }
    projectTransform();
    break;
  }
  case Qt::Key_R:
  {
    psi = 0.f;
    theta = glm::radians(45.0f);
    distanciaCamara = radiEscena*4;
    angMoveCarRojo = 0.f;
    angCarRojo = 0.f;
    
    angMoveCarVerde = 0.f;
    angCarVerde = 0.f;

    angMoveCarAzul = 0.f;
    angCarAzul = 0.f;

    viewTransform();
    projectTransform();

    break;
  }
  case Qt::Key_T:
  {
    if (animacionCurso) {
        timer->stop();
    } else {
        timer->start(16);
    }
    animacionCurso = !animacionCurso;
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
  if(DoingInteractive==ROTATE && !camaraPrimeraPersona){

    psi -= (e->x() - xClick) * factorAngleY;
    theta -= (e->y() - yClick) * factorAngleX;

    // Llamamos a viewTransform para actualizar la matriz de vista
    viewTransform();
  }
  // Actualizamos la última posición conocida del ratón
  xClick = e->x();
  yClick = e->y();
  update ();
}

void MyGLWidget::movimiento(){
  angMoveCarRojo -= 2.0f;
  angCarRojo += 2.0f;
  
  angMoveCarVerde -= 3.0f;
  angCarVerde += 3.0f;

  angMoveCarAzul-= 4.0f;
  angCarAzul += 4.0f;

  float radianAngle = glm::radians(angMoveCarVerde);
  glm::vec3 direction = glm::vec3(std::cos(radianAngle), centreBaseModels[CAR].y + 0.5 , std::sin(radianAngle));

  // Calcula la nueva posición del auto
  posicionCamP = centreEscena + direction*7.5f;
  
  float radianAngle2 = glm::radians(angMoveCarVerde-3.0f);
  glm::vec3 direction2 = glm::vec3(std::cos(radianAngle2), 0.5, std::sin(radianAngle2));

  // Calcula hacia donde mira el auto auto
  vistaCamP = centreEscena + direction2*7.5f;
}

void MyGLWidget::animacion(){
  makeCurrent();
  movimiento();
  update();
}