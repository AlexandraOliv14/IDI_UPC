#version 330 core

in vec3  fmatamb;
in vec3  fmatdiff;
in vec3  fmatspec;
in float fmatshin;
in vec3  fvertex;
in vec3  fnormal;

const vec3 llumAmbient = vec3(0.1, 0.1, 0.1);

out vec4 FragColor;

uniform vec3 colFocus;
uniform vec3 posFocus;

uniform vec3 colFocus1;
uniform vec3 posFocus1;

uniform vec3 colFocus2;
uniform vec3 posFocus2;


vec3 Ambient() {
  return llumAmbient*fmatamb;
}

vec3 Difus (vec3 NormSCO, vec3 L, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme de Lambert!
  // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
  vec3 colRes = vec3(0);
  if (dot (L, NormSCO) > 0)
    colRes = colFocus * fmatdiff * dot (L, NormSCO);
  return (colRes);
}

vec3 Especular (vec3 NormSCO, vec3 L, vec3 vertSCO, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme especular!
  // Assumim que els vectors estan normalitzats
  vec3 colRes = vec3 (0);
  // Si la llum ve de darrera o el material és mate no fem res
  if ((dot(NormSCO,L) < 0) || (fmatshin == 0))
    return colRes;  // no hi ha component especular

  vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
  vec3 V = normalize(-vertSCO); // perquè la càmera està a (0,0,0) en SCO

  if (dot(R, V) < 0)
    return colRes;  // no hi ha component especular

  float shine = pow(max(0.0, dot(R, V)), fmatshin);
  return (colRes + fmatspec * colFocus * shine);
}

void main()
{
    vec3 normalizeSCO = normalize(fnormal);

    vec3 luzDirecta = normalize( posFocus - fvertex);
    vec3 luzDifuza = Difus (normalizeSCO, luzDirecta, colFocus);
    vec3 lusEsp = Especular (normalizeSCO, luzDirecta, fvertex, colFocus);

    vec3 luzDirecta1 = normalize( posFocus1 - fvertex);
    vec3 luzDifuza1 = Difus (normalizeSCO, luzDirecta1, colFocus1);
    vec3 lusEsp1 = Especular (normalizeSCO, luzDirecta1, fvertex, colFocus1);

    vec3 luzDirecta2 = normalize( posFocus2 - fvertex);
    vec3 luzDifuza2 = Difus (normalizeSCO, luzDirecta2, colFocus2);
    vec3 lusEsp2 = Especular (normalizeSCO, luzDirecta2, fvertex, colFocus2);


    vec3 color = Ambient() + luzDifuza + lusEsp+ luzDifuza1 + lusEsp1+ luzDifuza2 + lusEsp2;

    FragColor = vec4(color, 1);
}
