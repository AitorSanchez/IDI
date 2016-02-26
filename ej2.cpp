#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <algorithm>
#include "model.h"
using namespace std;

int prim = 0, patricio = 1;
int a = 0, func = 0;
int gradosY = 0, gradosX = 0;
int modo = 0;
float trasX = 0.0, trasY = 0.0, trasZ = 0.0;
float scaX = 1.0, scaY = 1.0, scaZ = 1.0;
double Fescala, Fescala2;
double xmaxe, xmine, ymaxe, ymine, zmaxe, zmine;
int GLx = 600, GLy = 600;
int posy = GLy/2;
int posx = GLx/2;
double radio;
float angulo = atan(float(600)/(2*radio));
Model m;
bool cam = false, z = false, pared = true, rota = true, izquierda = false, mueve = false, del = false, luz = true, n = false, gl1 = true;
bool gl0 = false, gl2 = false;
float zoom = 1.0;
double velx = 0, velz = 0, factorvel = 0.2;
int rot = -90;
int pos0 = 0;
GLfloat norm[3];
GLfloat luze[4];
GLfloat v1[4] = {-5.0, 1.5, 5.0, 1.0};
GLfloat v2[4] = {-5.0, 1.5, -5.0, 1.0};
GLfloat v3[4] = {5.0, 1.5, -5.0, 1.0};
GLfloat v4[4] = {5.0, 1.5, 5.0, 1.0};
GLfloat pat[4] = {2.5, 1.5, 2.5, 1.0};


struct centro {
	double x;
	double y;
	double z;
};

centro cen;//centro esfera escena

struct caja {
	double xmax;
	double xmin;
	double ymax;
	double ymin;
	double zmax;
	double zmin;

	centro c_caja;
};

caja c_mod;

void cambia_val(GLfloat a, GLfloat b, GLfloat c, GLfloat d) {
	luze[0] = a;
	luze[1] = b;
	luze[2] = c;
	luze[3] = d;
}

void calcula_caja_modelo() {
  c_mod.xmax = m.vertices()[0];
  c_mod.xmin = m.vertices()[0];
  c_mod.ymax = m.vertices()[1];
  c_mod.ymin = m.vertices()[1];
  c_mod.zmax = m.vertices()[2];
  c_mod.zmin = m.vertices()[2];
  
  for(int i=3; i < m.faces().size(); i +=3) {
    if(c_mod.xmax < m.vertices()[i]) c_mod.xmax = m.vertices()[i];
    if(c_mod.xmin > m.vertices()[i]) c_mod.xmin = m.vertices()[i];
    if(c_mod.ymax < m.vertices()[i+1]) c_mod.ymax = m.vertices()[i+1];
    if(c_mod.ymin > m.vertices()[i+1]) c_mod.ymin = m.vertices()[i+1];
    if(c_mod.zmax < m.vertices()[i+2]) c_mod.zmax = m.vertices()[i+2];
    if(c_mod.zmin > m.vertices()[i+2]) c_mod.zmin = m.vertices()[i+2];
  }

	//calculo centro base
	c_mod.c_caja.x = (c_mod.xmax + c_mod.xmin)/2;
	c_mod.c_caja.y = c_mod.ymin;
	c_mod.c_caja.z = (c_mod.zmax + c_mod.zmin)/2;


  Fescala = max(c_mod.xmax-c_mod.xmin, c_mod.ymax-c_mod.ymin);
  Fescala = max(Fescala, c_mod.zmax-c_mod.zmin);
  Fescala = 1/Fescala;

  Fescala2 = 1/Fescala;
  Fescala2 -= Fescala2/2;
  Fescala2 = 1/Fescala2;

  trasX = -0.75-c_mod.xmin*Fescala;  
  trasY = -0.2-c_mod.ymin*Fescala;
  trasZ = 0.75-c_mod.zmax*Fescala;

 
  
}

void calcula_caja_escena() {
	xmaxe = 5.0;
	xmine = -5.0;
	ymaxe = 0.8;
	ymine = 0;
	zmaxe = 5.0;
	zmine = -5.0;
	double aux1 = (ymaxe-ymine)*(ymaxe-ymine);
  	double aux2 = (xmaxe-xmine)*(xmaxe-xmine);
  	double aux3 = (zmaxe-zmine)*(zmaxe-zmine);
  	radio = sqrt(aux1 + aux2 + aux3)/2;
	cen.x = (xmaxe+xmine)/2;
	cen.y = (ymaxe+ymine)/2;
	cen.z = (zmaxe+zmine)/2;
	angulo = asin(radio/(2*radio));	
}

void pinta_paredes() {
	glPushMatrix();//pared fondo
		glTranslated(0, 1.5/2, -5);
		glScaled(10, 1.5, 0.2);
		if (not luz)glColor3f(0.8,0.2,0.0);
		else {
			float esp[4] = {0, 0, 0, 0};
			float dif[4] = { 0, 1, 0, 1};
			float amb[4] = { 0, 0.5, 0, 1};
			glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT, amb);
			glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, dif);
			glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, esp);
	   		glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,50);
		}
		norm[0] = 1;
		norm[1] = 0;
		norm[2] = 0;	
		glNormal3fv(norm);
		glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();//pared centro
		glTranslated(1.5, 1.5/2, 2.5);
		glScaled(0.2, 1.5, 4);
		if (not luz)glColor3f(0.8,0.2,0.0);
		else {
			float esp[4] = {0, 0, 0, 1};
			float dif[4] = { 0, 1, 0, 1};
			float amb[4] = { 0, 0.5, 0, 1};
			glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT, amb);
			glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, dif);
			glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, esp);
	   		glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,100);
		}
		norm[0] = 1;
		norm[1] = 0;
		norm[2] = 0;	
		glNormal3fv(norm);
		glutSolidCube(1);
	glPopMatrix();
}
void pinta_cuadrado() {
	glPushMatrix();
	glBegin(GL_QUADS);
		if (not luz)glColor3f(0.6,0.4,0.3);
		else {
			float esp[4] = {1, 1, 1, 1};
			float dif[4] = { 0, 0, 1, 1};
			float amb[4] = { 0, 0, 0.5, 1};
			glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT, amb);
			glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, dif);
			glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, esp);
	   		glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,50);
		}
		norm[0] = 0;
		norm[1] = 1;
		norm[2] = 0;	
		if (not n) {
			glNormal3fv(norm);
			glVertex3f(-5.0, 0, -5.0);
			glVertex3f(5.0, 0, -5);
			glVertex3f(5.0 , 0, 5.0);
			glVertex3f(-5.0 , 0, 5.0);
		}
		else {		
			glNormal3fv(norm);
			glVertex3f(-5.0, 0, -5.0);
			glNormal3fv(norm);
			glVertex3f(5.0, 0, -5);
			glNormal3fv(norm);
			glVertex3f(5.0 , 0, 5.0);
			glNormal3fv(norm);
			glVertex3f(-5.0 , 0, 5.0);
		}
	glEnd();
	glPopMatrix();
}


void pinta_model(Model m) {
	glPushMatrix();
	glTranslated(velx, 0, velz);
	glRotated(rot, 0, 1, 0);
	glScaled(Fescala, Fescala, Fescala);
	glTranslated(-c_mod.c_caja.x, -c_mod.c_caja.y, -c_mod.c_caja.z);
	glBegin(GL_TRIANGLES); 
	int anterior = -1;
	for (int i=0; i < m.faces().size(); ++i) { 
		const Face &f = m.faces()[i];
		if (luz and anterior != f.mat) {
			anterior = f.mat;
			glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,Materials[f.mat].ambient);
			glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,Materials[f.mat].diffuse);
			glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,Materials[f.mat].specular);
	   		glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,Materials[f.mat].shininess);
		}
	    else if (not luz)glColor4fv(Materials[f.mat].diffuse);
		if(n) {
			glNormal3dv(&m.normals()[f.n[0]]);
			glVertex3dv(&m.vertices()[f.v[0]]);
			glNormal3dv(&m.normals()[f.n[1]]);
			glVertex3dv(&m.vertices()[f.v[1]]);
			glNormal3dv(&m.normals()[f.n[2]]);
			glVertex3dv(&m.vertices()[f.v[2]]);
      	}
      	else {
			glNormal3dv(f.normalC);
			glVertex3dv(&m.vertices()[f.v[0]]);
			glVertex3dv(&m.vertices()[f.v[1]]);
			glVertex3dv(&m.vertices()[f.v[2]]);
		} 
	}
	glEnd();
	glPopMatrix();
}

void pinta_patricio2() {
	glPushMatrix();
	glTranslated(2.5, 0, 2.5);
	glScaled(Fescala2, Fescala2, Fescala2);
	glTranslated(-c_mod.c_caja.x, -c_mod.c_caja.y, -c_mod.c_caja.z);
	glBegin(GL_TRIANGLES);
	int anterior = -1;
	for (int i=0; i < m.faces().size(); ++i) { 
		const Face &f = m.faces()[i];
		if (luz and anterior != f.mat) {
			anterior = f.mat;
			glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,Materials[f.mat].ambient);
			glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,Materials[f.mat].diffuse);
			glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,Materials[f.mat].specular);
	   		glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,Materials[f.mat].shininess);
		}
	    else if (not luz) glColor4fv(Materials[f.mat].diffuse);
		
		if (n) {
			glNormal3dv(&m.normals()[f.n[0]]);
			glVertex3dv(&m.vertices()[f.v[0]]);
			glNormal3dv(&m.normals()[f.n[1]]);
			glVertex3dv(&m.vertices()[f.v[1]]);
			glNormal3dv(&m.normals()[f.n[2]]);
			glVertex3dv(&m.vertices()[f.v[2]]);
      	}
      	else {
			glNormal3dv(f.normalC);
			glVertex3dv(&m.vertices()[f.v[0]]);
			glVertex3dv(&m.vertices()[f.v[1]]);
			glVertex3dv(&m.vertices()[f.v[2]]);
		} 
	} 
	glEnd();
	glPopMatrix();
}

void pinta_ninot() {
		norm[0] = 1;
		norm[1] = 0;
		norm[2] = 0;
		float esp[4] = {1, 1, 1, 1};
		float dif[4] = { 1, 1, 1, 1};
		float amb[4] = { 0.7, 0.7, 0.7, 1};
	//cuerpo muñeco
	glPushMatrix();
	if (not luz)glColor3f(0.5,0.5,0.5);
	else {
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT, amb);
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, dif);
		glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, esp);
		glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,50);
	}
	glutSolidSphere(0.4,20,20);
	glPopMatrix();
	//cabeza muñeco
	glPushMatrix();
	glTranslated(0.0, 0.6, 0.0);
	if (not luz)glColor3f(0.5,0.5,0.5);
	else {
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT, amb);
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, dif);
		glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, esp);
		glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,50);
	}
	glutSolidSphere(0.2,20,20);
	glPopMatrix();
	//nariz muñeco
	glPushMatrix();
	glTranslated(0.10, 0.6, 0.0);
	glRotated(90.0,0.0,1.0,0.0);
	if (not luz)glColor3f(0.5,0.5,0.0);
	else {
		float dif[4] = { 1, 1, 0, 1};
		float amb[4] = { 0.8, 0.1, 0.0, 1};
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT, amb);
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, dif);
		glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, esp);
		glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,50);
	}
	norm[0] = 0;
	norm[1] = 0;
	norm[2] = 1;
	glutSolidCone(0.1, 0.2, 10, 10);
	glPopMatrix(); 
}

void mov_camara() {
	if (func == 0) {
		glLoadIdentity();
		glTranslated(0, 0, (-2)*radio);
		glRotated(gradosX,1.0,0.0,0.0);
		glRotated(gradosY,0.0,1.0,0.0);
		glTranslated(-cen.x, -cen.y, -cen.z);
	}
	else if (func == 2) {//primera persona
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(2*angulo*(float(180)/(M_PI)), 1, 0.1, 3*radio);
		glMatrixMode(GL_MODELVIEW);
		double vrpx = velx + sin((double)(rot*M_PI)/double(180));
		double vrpz = velz + cos((double)(rot*M_PI)/double(180));
		glLoadIdentity();
		gluLookAt(velx, 1, velz, vrpx, 1, vrpz, 0, 1, 0);
	}
	if (pos0 == 0) glLightfv(GL_LIGHT0, GL_POSITION, v1);
	else if (pos0 == 1) glLightfv(GL_LIGHT0, GL_POSITION, v2);
	else if (pos0 == 2) glLightfv(GL_LIGHT0, GL_POSITION, v3);
	else if (pos0 == 3) glLightfv(GL_LIGHT0, GL_POSITION, v4);
	else if (pos0 == 4) glLightfv(GL_LIGHT0, GL_POSITION, pat);

	if (gl2) {
		cambia_val(float(velx), 1.5, float(velz), 1.0);
		glLightfv(GL_LIGHT2, GL_POSITION, luze);
	}
}

void refresh (void) { 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mov_camara();
	if (pared)pinta_paredes();
	pinta_cuadrado();
	glPushMatrix();
		glTranslated(2.5, 0.4, -2.5);
		pinta_ninot(); 
	glPopMatrix();

	glPushMatrix();
		glTranslated(-2.5, 0.4, 2.5);
		pinta_ninot(); 
	glPopMatrix();

	glPushMatrix();
		glTranslated(-2.5, 0.4, -2.5);
		pinta_ninot(); 
	glPopMatrix();
	pinta_patricio2();
	pinta_model(m);
	glutSwapBuffers();
  
}

void reshape(int a, int h) {
	GLx = a;
	GLy = h;
	glViewport(0,0,a,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float ra = float(a)/float(h);
	if (cam == false) {
		if (ra < 1) gluPerspective((zoom+2*atan(tan(angulo)/ra)*(float(180)/(M_PI))), ra, radio, 3*radio);
		else gluPerspective(zoom+2*angulo*(float(180)/(M_PI)), ra, radio, 3*radio);
	}
	else {
		if (ra > 1) glOrtho(-radio*ra, radio*ra, -radio-zoom/10, radio+zoom/10, -radio*2, radio*3);
		else if (ra < 1) glOrtho(-radio-zoom/10, radio+zoom/10, -radio/ra, radio/ra, -radio*2, radio*3);
		else glOrtho(-radio-zoom/10, radio+zoom/10, -radio-zoom/10, radio+zoom/10, -radio*2, radio*3);
	}
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
	refresh();//necesario para pintar inicialmente en mi portatil
}

void mouse(int bot, int accio, int x, int y) {
	if (bot == GLUT_RIGHT_BUTTON && accio == GLUT_DOWN) z = true;
	else z = false;
	posy = y;
	posx = x;	
}

void motion(int x, int y) {
	if (y > posy) {
		if (func == 0) gradosY += 5;
		else if (func == 1) trasZ += 0.02;
		if (z and zoom < 111 and func ==9) {
			if (cam)zoom += 2;
			else zoom += 5;
		}
		posy = y;
		if (posy >= GLy - 10) {
			posy = GLy/2;
			if (func == 0) gradosY = 0;
			else if (func == 1) trasZ = -ymine-((ymaxe-ymine)/2);
		}
	}
	else if (y < posy) {
		if (func == 0) gradosY -= 5;
		else if (func == 1) trasZ -= 0.02;
		if (z and func ==9) {
			if (cam == false and zoom > -40) zoom -= 5;
			else if (cam and zoom > -8) zoom -= 2;
		}
		posy = y;
		if (posy <= 10) {
			posy = GLy/2;
			if (func == 0) gradosY = 0;
			else if (func == 1) trasZ = -ymine-((ymaxe-ymine)/2);
		}
	}
	if (x > posx) {
		if (func == 0) gradosX += 5;
		else if (func == 1) trasX += 0.02;
		posx = x;
		if (posx >= GLx - 10) {
			posx = GLx/2;
			if (func == 0) gradosX = 0;
			else if (func == 1) -xmine-((xmaxe-xmine)/2);  
		}
	}
	else if (x < posx) {
		if (func == 0) gradosX -= 5;
		else if (func == 1) trasX -= 0.02;
		posx = x;
		if (posx <= 10) {
			posx = GLx/2;
			if (func == 0) gradosX = 0;
			else if (func == 1) -xmine-((xmaxe-xmine)/2);  
		}
	}
	if (z) reshape(GLx, GLy);
	else glutPostRedisplay();
}

void keyboard(unsigned char tecla, int x, int y) {

	if (tecla == 'c') {
		if (func == 2) func = 0;
		else func = 2;
		mov_camara();
	}

	else if (tecla == 'g') {
		if (func != 0) func = 0;
		else func = 9;
	}
	
	else if (tecla == 'h') {
		cout << "------------------HELP:---------------------" << endl;
		cout << "*/Pulsar a para rotar el muñeco a la izquierda" << endl;
		cout << "*/Pulsar c para pasar a primera persona" << endl;
		cout << "*/Pulsar d para rotar el muñeco a la derecha" << endl;
		cout << "*/Pulsar ESC para salir del programa" << endl;
		cout << "*/Pulsar g para cambiar de modo inspeccion a modo zoom" << endl;
		cout << "*/Pulsar i para activar/desactivar la iluminacion" << endl;
		cout << "*/Pulsar n para alternar normal por vertice/cara" << endl;
		cout << "*/Pulsar m para cambiar la posicion de la luz de escena" << endl;
		cout << "*/Pulsar p para alternar entre camara perspectiva y axonometrica" << endl;
		cout << "*/Pulsar r para reiniciar la posicion de la camara y las condiciones de iluminacion" << endl;
		cout << "*/Pulsar v para hacer visibles/invisibles las paredes" << endl;
		cout << "*/Pulsar s para desplazar el muñeco hacia detras" << endl;
		cout << "*/Pulsar w para desplazar el muñeco hacia delante" << endl;
		cout << "*/Pulsar z para aumentar velocidad de movimiento" << endl;
		cout << "*/Pulsar x para disminuir velocidad de movimiento" << endl;
		cout << "*/Pulsar 0 para activar/desactivar la luz de escena" << endl;
		cout << "*/Pulsar 1 para activar/desactivar la luz de camara" << endl;
		cout << "*/Pulsar 2 para activar/desactivar la luz que se mueve con Patricio" << endl;
		cout << "--------------------------------------------" << endl;
	}

	else if (tecla == 'i') {
		if (luz) glDisable(GL_LIGHTING);
		else glEnable(GL_LIGHTING);
		luz = not(luz);
	}

	else if (tecla == 'm') {
		pos0++;
		if (pos0 >= 5) pos0 = 0;
	}

	else if (tecla == 'n') n = not n;

	else if (tecla == 'p') {
		if (cam) {
			cam = false;
			cout << "camara perspectiva activada" << endl;
			reshape(GLx, GLy);
		}
		else {
			cam = true;
			cout << "camara axonometrica activada" << endl;
			reshape(GLx, GLy);
		}
	}

	else if (tecla == 'r') {
		glLoadIdentity();//camara
		glTranslated(0, 0, (-2)*radio);
		glRotated(-gradosX,1.0,0.0,0.0);
		glRotated(-gradosY,0.0,1.0,0.0);
		glTranslated(-cen.x, -cen.y, -cen.z);
		gradosY = 0;
		gradosX = 0;
		pos0 = 0;//iluminacion
		gl0 = false;
		gl1 = true;
		gl2 = false;
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT2);
		glEnable(GL_LIGHT1);
	}

	else if (tecla == 's') {
		velx -= sin((double)(rot*M_PI)/double(180))*factorvel;
		velz -= cos((double)(rot*M_PI)/double(180))*factorvel;
	}
	
	else if (tecla == 'v') {
		if (pared) {
			cout << "<<paredes modo visible>>" << endl;
			pared = false;
		}
		else {
			cout << "<<paredes modo  no visible>>" << endl;
			pared = true;
		}
		reshape(GLx, GLy);
	}

	else if (tecla == 'w') {
		velx += sin((double)(rot*M_PI)/double(180))*factorvel;
		velz += cos((double)(rot*M_PI)/double(180))*factorvel;
	}
	
	else if (tecla == 'a') rot += 10;
	else if (tecla == 'd') rot -= 10;
	else if (tecla == 'z') factorvel += 0.2;
	else if (tecla == 'x') factorvel -= 0.2;

	else if (tecla == '1') {
		if (gl1) glDisable(GL_LIGHT1);
		else glEnable(GL_LIGHT1);
		gl1 = not gl1;
	}

	else if (tecla == '0') {
		if (gl0) glDisable(GL_LIGHT0);
		else glEnable(GL_LIGHT0);
		gl0 = not gl0;
	}

	else if (tecla == '2') {
		if (gl2) glDisable(GL_LIGHT2);
		else glEnable(GL_LIGHT2);
		gl2 = not gl2;
	}
	
	else if (tecla == 27) exit(0);

	glutPostRedisplay();
}

void initGL(int a, const char *b[]) {
	glutInit(&a, (char **) b);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(400, 100);
	glutInitWindowSize(GLx,GLy);
	glutCreateWindow("IDI: Practiques OpenGL");
	glEnable(GL_DEPTH_TEST);
	glutDisplayFunc (refresh);
	glutReshapeFunc(reshape);
	glClearColor(0.0,0.0,0.0,0);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	m.load("Patricio.obj");
	calcula_caja_escena();
	calcula_caja_modelo();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(2*angulo*(180/(M_PI)), 1, radio, 3*radio);
	glMatrixMode(GL_MODELVIEW);
	mov_camara();
	glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	//glEnable(GL_LIGHT2);
	cambia_val(0.0, 0.0, 0.0, 1.0);//GL_LIGHT1
	glLightfv(GL_LIGHT1, GL_AMBIENT, luze);
	cambia_val(1.0, 1.0, 1.0, 1.0);
	glLightfv(GL_LIGHT1, GL_SPECULAR, luze);
	cambia_val(0.6, 0.6, 0.6, 1.0);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, luze);
	cambia_val(0.0, 0.0, 0.0, 1.0);//GL_LIGHT0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luze);
	cambia_val(1.0, 1.0, 1.0, 1.0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, luze);
	cambia_val(0.6, 0.7, 0.0, 1.0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luze);
	cambia_val(0.0, 0.0, 0.0, 1.0);//GL_LIGHT2
	glLightfv(GL_LIGHT2, GL_AMBIENT, luze);
	cambia_val(1.0, 1.0, 1.0, 1.0);
	glLightfv(GL_LIGHT2, GL_SPECULAR, luze);
	cambia_val(0.6, 0.6, 0.6, 1.0);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, luze);
	glEnable(GL_NORMALIZE);
	
}

int main(int argc, const char *argv[]) 
{ 
  initGL(argc, argv);
  glutMainLoop();
  return 0;
} 
