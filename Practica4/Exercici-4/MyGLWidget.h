// MyGLWidget.h
#include "LL4GLWidget.h"

class MyGLWidget : public LL4GLWidget {
  Q_OBJECT
  public:
    MyGLWidget(QWidget *parent);
    ~MyGLWidget();
  protected:

    virtual void initializeGL ( );
    virtual void iniEscena ();
    virtual void iniMaterialTerra ();

    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void paintGL ( );

    // virtual void viewTransform ();
    virtual void modelTransformTerra ();
    virtual void modelTransformTrack (float radi, float rotY);
    // virtual void modelTransformFantasma ();
    virtual void modelTransformMineCart ();

    void carregaShaders ();
    void focos(GLuint varColor, GLuint varPos,glm::vec3 pos, glm::vec3 color);
    void focosCarro(GLuint varColor, GLuint varPos,glm::vec3 pos, glm::vec3 color);

    float anguloCar = 0.0;
    glm::vec4 fondo = glm::vec4(0.5, 0.7, 1.0, 1.0);

    //uniform
    GLuint locColor;
    GLuint locPosFoco;
    GLuint locColor1;
    GLuint locPosFoco1;
    GLuint locColor2;
    GLuint locPosFoco2;
  private:
    int printOglError(const char file[], int line, const char func[]);
};
