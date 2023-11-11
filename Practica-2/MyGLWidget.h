#include "LL2GLWidget.h"

#include "model.h"

class MyGLWidget : public LL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : LL2GLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void initializeGL ( );
    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ( );
    // resizeGL - És cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);

    virtual void keyPressEvent (QKeyEvent *event);

    virtual void mousePressEvent (QMouseEvent *event);
    virtual void mouseReleaseEvent (QMouseEvent *event);
    virtual void mouseMoveEvent (QMouseEvent *event);
    virtual void rotateObject (float angleX, float angleY);

    virtual void carregaShaders ();
    virtual void iniCamera();
// viewTransform i projecTransform - Es fan servir per a construir i enviar als shader les matrius de càmera (View i Projection respectivament).
    virtual void viewTransform ();
    virtual void projectTransform ();
  //TRANSFORM MODELOS
  virtual void TerraTransform();
  virtual void PipeTransform();
  virtual void RoadTransform(glm::vec3 pos, float angle);
  virtual void CarTransform(glm::vec3 pos, float angle, float angleMove, glm::vec3 color);

  GLuint colorLoc;
  GLuint viewRotateLoc;

  //INICIAL AUTOS
  glm::vec3 posCarAzul = glm::vec3(6.f, 0.f, 0.f);
  glm::vec3 posCarVerde = glm::vec3(7.5f, 0.f, 0.f);
  glm::vec3 posCarRojo = glm::vec3(9.f, 0.f, 0.f);

  float angCarAzul = 0.f;
  float angCarVerde = 0.f;
  float angCarRojo = 0.f;


  private:
  
    int printOglError(const char file[], int line, const char func[]);
   
};
