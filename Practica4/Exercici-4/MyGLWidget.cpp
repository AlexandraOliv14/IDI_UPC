// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

MyGLWidget::MyGLWidget(QWidget *parent=0) : LL4GLWidget(parent) {

}

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
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

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  

  //glClearColor( 1.0, 1.0, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glClearColor(fondo.r, fondo.g, fondo.b, fondo.a); // defineix color de fons (d'esborrat)

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  
  carregaShaders();
  
  creaBuffersTrack();
  creaBuffersFantasma();
  creaBuffersMineCart();
  creaBuffersTerra();

  iniEscena();
  iniCamera();

}
void MyGLWidget::carregaShaders(){
  LL4GLWidget::carregaShaders();

  locColor = glGetUniformLocation (program->programId(), "colFocus");
  locPosFoco = glGetUniformLocation (program->programId(), "posFocus");
  locColor1 = glGetUniformLocation (program->programId(), "colFocus1");
  locPosFoco1 = glGetUniformLocation (program->programId(), "posFocus1");
  locColor2 = glGetUniformLocation (program->programId(), "colFocus2");
  locPosFoco2 = glGetUniformLocation (program->programId(), "posFocus2");

}

void MyGLWidget::iniMaterialTerra ()
{
  // Donem valors al material del terra
  amb = glm::vec3(0.1,0.0,0.0);
  diff = glm::vec3(0.4,0.0,0.0);
  spec = glm::vec3(0.8,0.8,0.8);
  shin = 100;
}

void MyGLWidget::iniEscena ()
{
  centreEsc = glm::vec3(0,10,0);
  radiEsc = 21;  
}

void MyGLWidget::paintGL () 
{
  // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
  // useu els paràmetres que considereu (els que hi ha són els de per defecte)
  // glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer i el depth-buffer
  glClearColor(fondo.r, fondo.g, fondo.b, fondo.a);  
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // TERRA
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLES, 0, 6);
  
  // TRACK
  for(int i=0; i<=360; i=i+4){
    glBindVertexArray (VAO_Track);
    modelTransformTrack (10,i);
    glDrawArrays(GL_TRIANGLES, 0, track.faces().size()*3);
  }
  
  // FANTASMA
  // glBindVertexArray (VAO_Fantasma);
  // modelTransformFantasma ();
  // glDrawArrays(GL_TRIANGLES, 0, fantasma.faces().size()*3);

  // MINE CART
  glBindVertexArray (VAO_MineCart);
  modelTransformMineCart ();
  glDrawArrays(GL_TRIANGLES, 0, mineCart.faces().size()*3);
  
  glm::vec3 foco = glm::vec3(0, 0, -2*radiEsc);
  glm::vec3 colorF = glm::vec3(0.8,0.8,0.8);
  focos(locColor, locPosFoco,foco,colorF);

  glm::vec3 colorFocoCarro = glm::vec3(0.6,0.6,0.0);
  glm::vec3 focoCarro1 = glm::vec3(0.5,0.5,-2.0);
  glm::vec3 focoCarro2 = glm::vec3(-0.5,0.5,-2.0);

  focosCarro(locColor1, locPosFoco1,focoCarro1,colorFocoCarro);
  focosCarro(locColor2, locPosFoco2,focoCarro2,colorFocoCarro);
  
  glBindVertexArray(0);
}

void MyGLWidget::focos(GLuint varColor, GLuint varPos,glm::vec3 pos, glm::vec3 color){

  glUniform3fv(varColor, 1, &color[0]);
  glUniform3fv(varPos, 1, &pos[0]);
}

void MyGLWidget::focosCarro(GLuint varColor, GLuint varPos,glm::vec3 pos, glm::vec3 color){

  glm::vec3 direccion = glm::vec3(std::sin(glm::radians(anguloCar)), 0, std::cos(glm::radians(anguloCar)))*10.f;
  glm::vec3 direccionNueva = direccion + pos;
  
  glUniform3fv(varColor, 1, &color[0]);
  glUniform3fv(varPos, 1, &direccionNueva[0]);
}

void MyGLWidget::modelTransformTerra ()
{
  glm::mat4 TG = glm::mat4(1.f);  // Matriu de transformació
  TG = glm::translate(TG, glm::vec3(0,0,0));
  TG = glm::scale(TG, glm::vec3(3, 3, 3));
  TG = glm::translate(TG, -glm::vec3(5,0,5));

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformTrack (float radi, float rotY)
{
  glm::mat4 TG;
  
  glm::vec3 direccion = glm::vec3(0,0,0) + glm::vec3(std::sin(glm::radians(rotY)), 0, std::cos(glm::radians(rotY)))*radi;

  TG = glm::translate(glm::mat4(1.f), glm::vec3(direccion));
  TG = glm::scale(TG, glm::vec3(escalaTrack, escalaTrack, escalaTrack));
  TG = glm::rotate(TG, float(glm::radians(rotY)), glm::vec3(0,1,0));
  TG = glm::translate(TG, -centreBaseTrack);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformMineCart ()
{

  glm::vec3 direccion = glm::vec3(0,0,0) + glm::vec3(std::sin(glm::radians(anguloCar)), 0, std::cos(glm::radians(anguloCar)))*10.f;

  glm::mat4 TG;
  TG = glm::translate(glm::mat4(1.f), direccion);
  TG = glm::rotate(TG, float(glm::radians(90.0 + anguloCar)), glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(escalaMineCart, escalaMineCart, escalaMineCart));
  TG = glm::translate(TG, -centreBaseMineCart);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / ample;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_A: {
      // ... 5grados
      anguloCar = anguloCar +5.f;
    break;
	}
  case Qt::Key_L: {

    if(fondo == glm::vec4(0.5, 0.7, 1.0, 1.0)){
      fondo = glm::vec4(0.0, 0.0, 0.0, 1.0);
    }else{
      fondo = glm::vec4(0.5, 0.7, 1.0, 1.0);
    }

    break;
	}
  case Qt::Key_R: {
      // ...
    break;
	}
  case Qt::Key_S: {
      // ...
    break;
	}	
  default: LL4GLWidget::keyPressEvent(event); break;
  }
  update();
}
