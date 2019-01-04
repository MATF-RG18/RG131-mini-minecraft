#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include <GL/glut.h>
#define TIMER_INTERVAL 10
#define TIMER_ID 0
#define MAX1 50 
#define MAX2 4

//kocke
enum {
	CUBE1_MATERIAL = 1, CUBE2_MATERIAL = 2, CUBE3_MATERIAL = 3,
  CUBE4_MATERIAL = 4, CUBE5_MATERIAL = 5, CUBE6_MATERIAL = 6,
  CUBE7_MATERIAL = 7, CUBE8_MATERIAL = 8, CUBE9_MATERIAL = 9,
  CUBE10_MATERIAL = 10, CUBE11_MATERIAL = 11, CUBE12_MATERIAL = 12,
  CUBE13_MATERIAL = 13, CUBE14_MATERIAL = 14, CUBE15_MATERIAL = 15
} Kocke;

//boje
enum {
  ZUTO, CRVENO, ZELENO, PLAVO, BELO, BRISI
} Boje;


//pravljenje boja

//Zuta
GLfloat zuto_ambient[] = {0.33, 0.22, 0.03, 1.0}, zuto_diffuse[] = {0.78, 0.57, 0.11, 1.0},
zuto_specular[] = {0.99, 0.91, 0.81, 1.0},
zuto_shininess = 27.8;

// Plava
GLfloat plavo_ambient[] = {0.0, 0.0, 1.0}, 
plavo_diffuse[] = {0.0, 0.0, 1.0},
plavo_specular[] = {0.0, 0.0, 1.6},
plavo_shininess = 32.0;

//Bela
GLfloat belo_ambient[] = {1.0, 1.0, 1.0}, 
belo_diffuse[] = {1.0, 1.0, 1.0}, 
belo_specular[] = {1.0, 1.0, 1.6}, 
belo_shininess = 32.0;

//Crvena
GLfloat crveno_ambient[] = {0.0, 0.0, 0.0},
crveno_diffuse[] = {0.5, 0.0, 0.0},
crveno_specular[] = {0.7, 0.6, 0.6},
crveno_shininess = 32.0;

//Zelena
GLfloat zeleno_ambient[] = {0.0215, 0.1745, 0.0215}, zeleno_diffuse[] = {0.07568, 0.61424, 0.07568}, zeleno_specular[] = {0.633, 0.727811, 0.633}, zeleno_shininess = 76.8;

//Crna boja
GLfloat brisi_ambient[] = {0.02, 0.02, 0.02}, 
brisi_diffuse[] = {0.02, 0.01, 0.01}, 
brisi_specular[] = {0.4, 0.4, 0.4}, 
brisi_shininess = .78125;

int shade_model = GL_SMOOTH;

char *cube1_material, *cube2_material, *cube3_material, *cube4_material, *cube5_material, *cube6_material,
*cube7_material, *cube8_material, *cube9_material, *cube10_material, *cube11_material, *cube12_material, *cube13_material,
*cube14_material, *cube15_material;

//f-ja za pravljenje niza kocki, za ispis na ekran
void output(GLfloat x, GLfloat y, char *format,...)
{
  va_list args;
  char buffer[200], *p;

  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);
  glPushMatrix();
  glTranslatef(x, y, 0);
  for (p = buffer; *p; p++)
    glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
  glPopMatrix();
}


//matrica da cita koordinate iz datoteke
float X[MAX1][MAX2];
/* Fleg koji odredjuje polozaj svetla. */
static int headlight = 0;

/* Dimenzije prozora */
static int window_width, window_height;

/* Kumulativana matrica rotacije. */
static float matrix[16];

/* Konstanta pi. */
const static float pi = 3.141592653589793;

/* Uglovi u sfernom koordinatnom sistemu koji
   odredjuju polozaj vidne tacke. */
static float phi, theta;

/* Inkrementi gornjih uglova. */
static float delta_phi, delta_theta;

static int mouse_x, mouse_y;

//brojac za kocke
int a = 0;

/* Deklaracije callback funkcija. */
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void on_timer(int id);
static void on_mouse(int button, int state, int x, int y);
static void on_motion(int x, int y);
char* material_select(int object, int value);
void material(int dlist, GLfloat * ambient, GLfloat * diffuse,
  GLfloat * specular, GLfloat shininess);

//f-je kocki
void cube1_select(int value);
void cube2_select(int value);
void cube3_select(int value);
void cube4_select(int value);
void cube5_select(int value);
void cube6_select(int value);
void cube7_select(int value);
void cube8_select(int value);
void cube9_select(int value);
void cube10_select(int value);
void cube11_select(int value);
void cube12_select(int value);
void cube13_select(int value);
void cube14_select(int value);
void cube15_select(int value);

void main_menu_select(int value);

//animacije
float animation_parameter;
int animation_ongoing;
   
//f-ja greske
void greska(){
	fprintf(stderr, "-1\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	//parametri
    animation_ongoing = 0;
	animation_parameter = -1.5;
    
	//ucitavanje koordinata iz datoteke
	FILE* f = fopen("1.txt", "r");
	if(f == NULL)
		greska();
		
	int i, j, koordinata = 3;
	int broj_kockica;
	fscanf(f, "%d",&broj_kockica);
	for(i = 0; i < broj_kockica; i++)
		for(j = 0; j < koordinata; j++)
			fscanf(f, "%f",&X[i][j]);


    /* Ambijentalna boja svetla. */
    GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1 };

    /* Difuzna boja svetla. */
    GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1 };

    /* Spekularna boja svetla. */
    GLfloat light_specular[] = { 0.9, 0.9, 0.9, 1 };

    /* Koeficijenti ambijentalne refleksije materijala. */
    GLfloat ambient_coeffs[] = { 0.3, 0.7, 0.3, 1 };

    /* Koeficijenti difuzne refleksije materijala. */
    GLfloat diffuse_coeffs[] = { 0.2, 1, 0.2, 1 };

    /* Koeficijenti spekularne refleksije materijala. */
    GLfloat specular_coeffs[] = { 1, 1, 1, 1 };

    /* Koeficijent glatkosti materijala. */
    GLfloat shininess = 30;

    /* Inicijalizuje se GLUT. */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    /* Kreira se prozor. */
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    /* Registruju se callback funkcije. */
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);
	glutMouseFunc(on_mouse);
    glutMotionFunc(on_motion);

	#define MATERIAL_MENU_ENTRIES() \
    glutAddMenuEntry("Zuto", ZUTO); \
    glutAddMenuEntry("Crveno", CRVENO); \
    glutAddMenuEntry("Plavo", PLAVO); \
	glutAddMenuEntry("Belo", BELO); \
    glutAddMenuEntry("Zeleno", ZELENO); \
    glutAddMenuEntry("Brisi", BRISI);

	int cube_1;
	cube_1 = glutCreateMenu(cube1_select);
	MATERIAL_MENU_ENTRIES();
	int cube_2;
	cube_2 = glutCreateMenu(cube2_select);
	MATERIAL_MENU_ENTRIES();
	int cube_3;
	cube_3 = glutCreateMenu(cube3_select);
	MATERIAL_MENU_ENTRIES();
    int cube_4;
    cube_4 = glutCreateMenu(cube4_select);
    MATERIAL_MENU_ENTRIES();
    int cube_5;
    cube_5 = glutCreateMenu(cube5_select);
    MATERIAL_MENU_ENTRIES();
    int cube_6;
    cube_6 = glutCreateMenu(cube6_select);
    MATERIAL_MENU_ENTRIES();
    int cube_7;
    cube_7 = glutCreateMenu(cube7_select);
    MATERIAL_MENU_ENTRIES();
    int cube_8;
    cube_8 = glutCreateMenu(cube8_select);
    MATERIAL_MENU_ENTRIES();
    int cube_9;
    cube_9 = glutCreateMenu(cube9_select);
    MATERIAL_MENU_ENTRIES();
    int cube_10;
    cube_10 = glutCreateMenu(cube10_select);
    MATERIAL_MENU_ENTRIES();
    int cube_11;
    cube_11 = glutCreateMenu(cube11_select);
    MATERIAL_MENU_ENTRIES();
    int cube_12;
    cube_12 = glutCreateMenu(cube12_select);
    MATERIAL_MENU_ENTRIES();
    int cube_13;
    cube_13 = glutCreateMenu(cube13_select);
    MATERIAL_MENU_ENTRIES();
	int cube_14;
    cube_14 = glutCreateMenu(cube14_select);
    MATERIAL_MENU_ENTRIES();
    int cube_15;
    cube_15 = glutCreateMenu(cube15_select);
    MATERIAL_MENU_ENTRIES();


	//Ovo je padajuci meni
    glutCreateMenu(main_menu_select);
    glutAddMenuEntry("Smooth shading", GL_SMOOTH);
    glutAddMenuEntry("Flat shading", GL_FLAT);
    glutAddSubMenu("Cube1", cube_1);
    glutAddSubMenu("Cube2", cube_2);
    glutAddSubMenu("Cube3", cube_3);
    glutAddSubMenu("Cube4", cube_4);
    glutAddSubMenu("Cube5", cube_5);
    glutAddSubMenu("Cube6", cube_6);
    glutAddSubMenu("Cube7", cube_7);
    glutAddSubMenu("Cube8", cube_8);
    glutAddSubMenu("Cube9", cube_9);
    glutAddSubMenu("Cube10", cube_10);
    glutAddSubMenu("Cube11", cube_11);
    glutAddSubMenu("Cube12", cube_12);
    glutAddSubMenu("Cube13", cube_13);
  	glutAddSubMenu("Cube14", cube_14);
  	glutAddSubMenu("Cube15", cube_15);
  	glutAddMenuEntry("Quit", 666);
  	glutAttachMenu(GLUT_RIGHT_BUTTON);
	
	//inicijalizacija kocki, u startu sam stavio da su plave, bele, zelene...
	cube1_select(ZELENO);
  	cube2_select(PLAVO);
  	cube3_select(PLAVO);
	cube4_select(PLAVO);
  	cube5_select(PLAVO);
  	cube6_select(PLAVO);
  	cube7_select(PLAVO);
  	cube8_select(BELO);
  	cube9_select(BELO);
  	cube10_select(BELO);
  	cube11_select(BELO);
  	cube12_select(BELO);
  	cube13_select(BELO);
  	cube14_select(ZELENO);
  	cube15_select(BELO);


    /* Incijalizuju se globalne promjenljive. */
    phi = 0;
    theta = pi / 4;
    delta_phi = delta_theta = pi / 90;

	mouse_x = 0;
	mouse_y = 0;   

    /* Obavlja se OpenGL inicijalizacija. */
    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);

    /* Postavljaju se svojstva prvog svetla */
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    /* Postavljaju se svojstva materijala */
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	
    


	glEnable(GL_NORMALIZE);
	
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);

	//zatvaranje datoteke
	fclose(f);
    /* Program ulazi u glavnu petlju. */
    glutMainLoop();

    return 0;
}

//timer
static void on_timer(int id)
{
    if (TIMER_ID != id)
        return;

    animation_parameter += 0.01;

    glutPostRedisplay();

    if (animation_ongoing) {
        glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
    }
}

//za f-ju on_timer_cube_move
float x = -0.03;
float y = -0.02;
float z = -0.01;

static void on_timer_cube_move(int id)
{
	if(TIMER_ID != id)
		return;

	X[a][0] += x;
	if(X[a][0] >= 2.5 || X[a][0] <= -2.5) {
		x *= -1;
	}	
	X[a][1] += y;
	if(X[a][1] >= 2.5 || X[a][1] <= -2.5) {
		y *= -1;
	}
	X[a][2] += z;
	if(X[a][2] >= 2.5 || X[a][2] <= -2.5) {
		z *= -1;
	}


	glutPostRedisplay();

    if (animation_ongoing) {
        glutTimerFunc(TIMER_INTERVAL, on_timer_cube_move, TIMER_ID);
    }
}

//tastatura
static void on_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        /* Zavrsava se program. */
        exit(0);
        break;
	
    case 'h':
    case 'H':
        /* Invertuje se fleg koji odredjuje pozicioniranje svetla. */
        headlight = !headlight;
        glutPostRedisplay();
        break;

    case 'p':
        /* Dekrementira se ugao phi i ponovo iscrtava scena. */
        phi -= delta_phi;
        if (phi > 2 * pi) {
            phi -= 2 * pi;
        } else if (phi < 0) {
            phi += 2 * pi;
        }
        glutPostRedisplay();
        break;

    case 'P':
        /* Inkrementira se ugao phi i ponovo iscrtava scena. */
        phi += delta_phi;
        if (phi > 2 * pi) {
            phi -= 2 * pi;
        } else if (phi < 0) {
            phi += 2 * pi;
        }
        glutPostRedisplay();
        break;
	
    case 'r':
    case 'R':
        /* Resetuju se uglovi phi i theta na pocetne vrednosti. */
		animation_ongoing = 0;
		animation_parameter = -1;
        phi = theta = 0;
        glutPostRedisplay();
        break;

    case 't':
        /*
         * Dekrementira se ugao theta i ponovo iscrtava scena. Ovaj
         * ugao se odrzava u intervalu [-89,89] stepeni.
         */
		
        theta -= delta_theta;
        if (theta < -(pi / 2 - pi / 180)) {
            theta = -(pi / 2 - pi / 180);
        }
        glutPostRedisplay();
        break;

    case 'T':
        /*
         * Inkrementira se ugao theta i ponovo iscrtava scena. Ovaj
         * ugao se odrzava u intervalu [-89,89] stepeni.
         */
        theta += delta_theta;
        if (theta > pi / 2 - pi / 180) {
            theta = pi / 2 - pi / 180;
        }
        glutPostRedisplay();
        break;
    
	case 'g':
    case 'G':
        /* Pokrecemo animaciju */
        if (!animation_ongoing) {
            animation_ongoing = 1;
            glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
        }
        break;

	//zaustavljamo animaciju
    case 's':
    case 'S':
        animation_ongoing = 0;
        break;

	//brojac za biranje kocki:
	case 'b':
		if(a == 13){
			a = 0;
		}
		else
			a++;
		break;
	case 'B':
		if(a == 0){
			a = 13;
		}
		else
			a--;
		break;
	// sledecih 6 dugmadi menja poziciju kocku, u zavisnosti koje dugme kliknemo, levo, desno, gore, dole, nazad i napred

	//levo
	case '1':		
        X[a][0] -= 0.2;
        glutPostRedisplay();
        break;
	//desno
	case '2':
        X[a][0] += 0.2;
        glutPostRedisplay();
        break;
	//dole
	case '3':		
        X[a][1] -= 0.2;
        glutPostRedisplay();
        break;
	//gore
	case '4':
        X[a][1] += 0.2;
        glutPostRedisplay();
        break;
	//napred
	case '5':		
        X[a][2] -= 0.2;
        glutPostRedisplay();
        break;
	//nazad
	case '6':
        X[a][2] += 0.2;
        glutPostRedisplay();
        break;

	//za animaciju kocke u prostoru
	case '7':
		if (!animation_ongoing) {
            animation_ongoing = 1;
            glutTimerFunc(TIMER_INTERVAL, on_timer_cube_move, TIMER_ID);
        }
		break;
	//zaustavljanje animacije
	case '8':
		animation_ongoing = 0;
		break;

	}
}

static void on_mouse(int button, int state, int x, int y)
{
    /* Pamti se pozicija pokazivaca misa. */
    mouse_x = x;
    mouse_y = y;
}

static void on_motion(int x, int y)
{
    /* Promene pozicije pokazivaca misa. */
    int delta_x, delta_y;

    /* Izracunavaju se promene pozicije pokazivaca misa. */
    delta_x = x - mouse_x;
    delta_y = y - mouse_y;

    /* Pamti se nova pozicija pokazivaca misa. */
    mouse_x = x;
    mouse_y = y;

    /* Izracunava se nova matrica rotacije. */
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glLoadIdentity();
        glRotatef(180 * (float) delta_x / window_width, 0, 1, 0);
        glRotatef(180 * (float) delta_y / window_height, 1, 0, 0);
        glMultMatrixf(matrix);

        glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
    glPopMatrix();

    /* Forsira se ponovno iscrtavanje prozora. */
    glutPostRedisplay();
}

static void on_reshape(int width, int height)
{
	window_width = width;
    window_height = height;


    /* Podesava se viewport. */
    glViewport(0, 0, width, height);

    /* Podesava se projekcija. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float) width / height, 1, 15);
}

//pravljenje boje
void material(int dlist, GLfloat * ambient, GLfloat * diffuse,
  GLfloat * specular, GLfloat shininess)
{
  	glNewList(dlist, GL_COMPILE);
  	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
  	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
  	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
  	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
  	glEndList();
}

//vrste boja
char* material_select(int object, int value)
{
  glutPostRedisplay();
  switch (value) {
  case ZUTO:
    material(object, zuto_ambient,
      zuto_diffuse, zuto_specular, zuto_shininess);
    return "zuto";
  case CRVENO:
    material(object, crveno_ambient, crveno_diffuse,
      crveno_specular, crveno_shininess);
    return "crveno";
  case PLAVO:
    material(object, plavo_ambient,
      plavo_diffuse, plavo_specular, plavo_shininess);
    return "plavo";
  case ZELENO:
    material(object, zeleno_ambient, zeleno_diffuse,
      zeleno_specular, zeleno_shininess);
    return "zeleno";
  case BRISI:
    material(object, brisi_ambient, brisi_diffuse,
      brisi_specular, brisi_shininess);
    return "brisi";
  case BELO:
    material(object, belo_ambient, belo_diffuse,
      belo_specular, belo_shininess);
    return "belo";	
  }
  return NULL;
}

//f-je za kocke
void cube1_select(int value)
{
  cube1_material = material_select(CUBE1_MATERIAL, value);
}

void cube2_select(int value)
{
  cube2_material = material_select(CUBE2_MATERIAL, value);
}

void cube3_select(int value)
{
  cube3_material = material_select(CUBE3_MATERIAL, value);
}

void cube4_select(int value)
{
  cube4_material = material_select(CUBE4_MATERIAL, value);
}

void cube5_select(int value)
{
  cube5_material = material_select(CUBE5_MATERIAL, value);
}

void cube6_select(int value)
{
  cube6_material = material_select(CUBE6_MATERIAL, value);
}

void cube7_select(int value)
{
  cube7_material = material_select(CUBE7_MATERIAL, value);
}

void cube8_select(int value)
{
  cube8_material = material_select(CUBE8_MATERIAL, value);
}

void cube9_select(int value)
{
  cube9_material = material_select(CUBE9_MATERIAL, value);
}

void cube10_select(int value)
{
  cube10_material = material_select(CUBE10_MATERIAL, value);
}

void cube11_select(int value)
{
  cube11_material = material_select(CUBE11_MATERIAL, value);
}

void cube12_select(int value)
{
  cube12_material = material_select(CUBE12_MATERIAL, value);
}

void cube13_select(int value)
{
  cube13_material = material_select(CUBE13_MATERIAL, value);
}

void cube14_select(int value)
{
  cube14_material = material_select(CUBE14_MATERIAL, value);
}

void cube15_select(int value)
{
  cube15_material = material_select(CUBE15_MATERIAL, value);
}

//meni
void main_menu_select(int value)
{
  if (value == 666)
    exit(0);
  glShadeModel(shade_model = value);
  glutPostRedisplay();
}


static void on_display(void)
{
    /* Pozicija svetla (u pitanju je tackasto svetlo). */
    GLfloat light_position[] = { 0, 0, 0, 1 };

    /* Brise se prethodni sadrzaj prozora. */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Inicijalizuje se matrica transformacije. */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	  gluLookAt(0, 0, 3, 0, 0, 0, 0, 1, 0);

    /*
     * Ako se svetlo postavlja zajedno sa vidnom tackom, pozicija
     * svetla se zadaje na ovom mjestu.
     */
    if (headlight) {
        light_position[0] = 0;
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    }

    /* Postavljaju se vidni parametri. */
    gluLookAt(5 * cos(theta) * cos(phi), 5 * cos(theta) * sin(phi),
              5 * sin(theta), 0, 0, 0, 0, 0, 1);

    /*
     * Ako se svetlo ne postavlja zajedno sa vidnom tackom, pozicija
     * svetla se zadaje na ovom mestu.
     */
    if (!headlight) {
        light_position[0] = 5;
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    }

	/* Primenjuje se matrica rotacije. */
    glMultMatrixf(matrix);


glPushMatrix();
    /* Kreiraju se objekti. */
    glRotatef(90, 0, 0, 1);
    glRotatef(90, 1, 0, 0);
    
	glPushMatrix();
		glTranslatef(X[0][0], X[0][1], X[0][2]);
		glCallList(CUBE1_MATERIAL);
    	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
    	glScalef(1, 1, 1);
    	glTranslatef(X[1][0], X[1][1], X[1][2]);
		glCallList(CUBE2_MATERIAL);
    	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
		glScalef(1, 1, 1);
		glTranslatef(X[2][0], X[2][1], X[2][2]);
		glCallList(CUBE3_MATERIAL);
		glutSolidCube(1);
	glPopMatrix();

	
	glPushMatrix();
		glScalef(1, 1, 1);
		glTranslatef(X[3][0], X[3][1], X[3][2]);
		glCallList(CUBE4_MATERIAL);
		glutSolidCube(1);
	glPopMatrix();		

	glPushMatrix();
		glScalef(1, 1, 1);
		glTranslatef(X[4][0], X[4][1], X[4][2]);
    glCallList(CUBE5_MATERIAL);
		glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
		glScalef(1, 1, 1);
		glTranslatef(X[5][0], X[5][1], X[5][2]);
    glCallList(CUBE6_MATERIAL);
		glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
		glScalef(1, 1, 1);
		glTranslatef(X[6][0], X[6][1], X[6][2]);
    glCallList(CUBE7_MATERIAL);
		glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
		glScalef(1, 1, 1);
		glTranslatef(X[7][0], X[7][1], X[7][2]);
    glCallList(CUBE8_MATERIAL);
		glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
		glScalef(1, 1, 1);
		glTranslatef(X[8][0], X[8][1], X[8][2]);
		glCallList(CUBE9_MATERIAL);
		glutSolidCube(1);
	glPopMatrix(); 

	glPushMatrix();
		glScalef(1, 1, 1);
		glTranslatef(X[9][0], X[9][1], X[9][2]);
		glCallList(CUBE10_MATERIAL);
		glutSolidCube(1);
	glPopMatrix(); 


	glPushMatrix();
		glScalef(1, 1, 1);
		glTranslatef(X[10][0], X[10][1], X[10][2]);
    glCallList(CUBE11_MATERIAL);
		glutSolidCube(1);
	glPopMatrix();

  glPushMatrix();
    glScalef(1, 1, 1);
    glTranslatef(X[11][0], X[11][1], X[11][2]);
    glCallList(CUBE12_MATERIAL);
    glutSolidCube(1);
  glPopMatrix();  

  glPushMatrix();
    glScalef(1, 1, 1);
    glTranslatef(X[12][0], X[12][1], X[12][2]);
    glCallList(CUBE13_MATERIAL);
    glutSolidCube(1);
  glPopMatrix(); 

  glPushMatrix();
    glScalef(1, 1, 1);
    glTranslatef(X[13][0], X[13][1], X[13][2]);
    glCallList(CUBE14_MATERIAL);
    glutSolidCube(1);
  glPopMatrix(); 

    //kocke koje se same rotitaju, u pocetku se ne vide, zbog klipinga
	 glCallList(CUBE15_MATERIAL);
	glTranslatef(X[14][0], X[14][1], X[14][2]);
    double clip_plane[] = {1, 0, 0, animation_parameter};
	

    glClipPlane(GL_CLIP_PLANE0, clip_plane);
    glEnable(GL_CLIP_PLANE0);

    glRotatef(animation_parameter * 60, 1, 0, 0);
    glutSolidCube(1);

    glDisable(GL_CLIP_PLANE0);

	 glCallList(CUBE15_MATERIAL);
	glTranslatef(X[15][0], X[15][1], X[15][2]);
    

    glClipPlane(GL_CLIP_PLANE0, clip_plane);
    glEnable(GL_CLIP_PLANE0);

    glRotatef(animation_parameter , 1, 0, 0);
    glutSolidCube(1);

    glDisable(GL_CLIP_PLANE0);
 
glPopMatrix();


  //Ispis teksta po ekranu
  glPushAttrib(GL_ENABLE_BIT);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0, 3000, 0, 3000);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  output(80, 2800, "Cube1: %s", cube1_material);
  output(80, 2650, "Cube2: %s", cube2_material);
  output(80, 2500, "Cube3: %s", cube3_material);
  output(80, 2350, "Cube4: %s", cube4_material);
  output(80, 2200, "Cube5: %s", cube5_material);
  output(1000, 2800, "Cube6: %s", cube6_material);
  output(1000, 2650, "Cube7: %s", cube7_material);
  output(1000, 2500, "Cube8: %s", cube8_material);
  output(1000, 2350, "Cube9: %s", cube9_material);
  output(1000, 2200, "Cube10: %s", cube10_material);
  output(1920, 2800, "Cube11: %s", cube11_material);
  output(1920, 2650, "Cube12: %s", cube12_material);
  output(1920, 2500, "Cube13: %s", cube13_material);
  output(1920, 2350, "Cube14: %s", cube14_material);
  output(1920, 2200, "Cube15: %s", cube15_material);
  output(850, 100, "Shade model: %s",
    shade_model == GL_SMOOTH ? "smooth" : "flat");
  output(850, 300, "Odabrana je kocka: %d", a+1);  

  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();

	

    /* Nova slika se salje na ekran. */
    glutSwapBuffers();
}
