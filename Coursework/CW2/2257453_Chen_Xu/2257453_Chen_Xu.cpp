#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include "vector"

using namespace std;

// Global
GLUquadricObj* quadric;
int imagewidth0, imageheight0, pixellength0;
int imagewidth1, imageheight1, pixellength1;
int imagewidth2, imageheight2, pixellength2;
int imagewidth3, imageheight3, pixellength3;
int imagewidth4, imageheight4, pixellength4;
int imagewidth5, imageheight5, pixellength5;
int imagewidth6, imageheight6, pixellength6;
int imagewidth7, imageheight7, pixellength7;
int imagewidth8, imageheight8, pixellength8;
vector<GLubyte*> p; 
GLuint texture[20]; 

#ifndef M_PI
#define M_PI 3.14159265358979323846 
#endif

// people
float personPosX = 0.0f, personPosY = 0.0f, personPosZ = 0.0f;
float personRotation = 0.0f; 
float armSwingAngle = 0.0f; 
float moveSpeed = 1.0f;

// camera
float camOffsetX = 0.0f, camOffsetY = 0.0f, camOffsetZ = 0.0f; 
float camAngleX = 10.0f, camAngleY = -180.0f, camAngleZ = 0.0f;
float camDist = 20.0f;

// bright
float lightAngle = 0.0f; 
GLfloat lightPos[4];  

// mouse
float lastX = 400, lastY = 300; 
bool firstMouse = true;  

// season
float season = 0.0f;

// read bmp
void ReadImage(const char path[256], GLint& imagewidth, GLint& imageheight, GLint& pixellength) {
    GLubyte* pixeldata; 
    FILE* pfile; 

    fopen_s(&pfile, path, "rb");
    if (pfile == 0) exit(0);
    fseek(pfile, 0x0012, SEEK_SET);
    fread(&imagewidth, sizeof(imagewidth), 1, pfile);
    fread(&imageheight, sizeof(imageheight), 1, pfile);

    pixellength = imagewidth * 3; 
    while (pixellength % 4 != 0) pixellength++; 
    pixellength *= imageheight; 

    pixeldata = (GLubyte*)malloc(pixellength);
    if (pixeldata == 0) exit(0); 


    fseek(pfile, 54, SEEK_SET);
    fread(pixeldata, pixellength, 1, pfile);

    p.push_back(pixeldata);
    fclose(pfile); 
}

void myinit(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0); 
    glEnable(GL_DEPTH_TEST); 
    glDepthFunc(GL_LEQUAL); 
    glShadeModel(GL_FLAT); 
    glEnable(GL_TEXTURE_2D);

    ReadImage("textures/spring_grass.bmp", imagewidth0, imageheight0, pixellength0); 
    ReadImage("textures/summer_grass.bmp", imagewidth1, imageheight1, pixellength1); 
    ReadImage("textures/autumn_grass.bmp", imagewidth2, imageheight2, pixellength2); 
    ReadImage("textures/winter_grass.bmp", imagewidth3, imageheight3, pixellength3); 
    ReadImage("textures/body.bmp", imagewidth6, imageheight6, pixellength6); 

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(9, &texture[0]); 

    for (int i = 0; i < 4; i++) {
        glBindTexture(GL_TEXTURE_2D, texture[i]);
        GLint width = (i == 0) ? imagewidth0 : (i == 1) ? imagewidth1 : (i == 2) ? imagewidth2 : imagewidth3;
        GLint height = (i == 0) ? imageheight0 : (i == 1) ? imageheight1 : (i == 2) ? imageheight2 : imageheight3;

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[i]);

  
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    glBindTexture(GL_TEXTURE_2D, texture[4]); 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imagewidth6, imageheight6, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[4]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}


void initQuadric() {
    quadric = gluNewQuadric(); 
    if (quadric == NULL) {
        printf("Failed to create quadric object.\n");
        exit(1);  
    }
    gluQuadricNormals(quadric, GLU_SMOOTH); 
    gluQuadricDrawStyle(quadric, GLU_FILL); 
}

void drawPerson() {

    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);    
    gluQuadricNormals(quad, GLU_SMOOTH); 


    GLfloat matAmbient[] = { 1.0f, 0.6f, 0.6f, 1.0f }; 
    GLfloat matDiffuse[] = { 0.1f, 0.1f, 0.1f, 1.0f }; 
    GLfloat matSpecular[] = { 0.2f, 0.2f, 0.2f, 1.0f }; 
    GLfloat matShininess[] = { 0.0f }; 


    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);


    glPushMatrix();
    glTranslatef(personPosX, personPosY, personPosZ); 
    glRotatef(personRotation, 0.0f, 1.0f, 0.0f); 

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[4]); 
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f); 


    glBegin(GL_QUADS);

    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.4f, -1.5f, 0.2f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.4f, -1.5f, 0.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.4f, 1.5f, 0.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.4f, 1.5f, 0.2f);


    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.4f, -1.5f, -0.2f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.4f, -1.5f, -0.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.4f, 1.5f, -0.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.4f, 1.5f, -0.2f);


    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.4f, -1.5f, -0.2f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.4f, -1.5f, 0.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.4f, 1.5f, 0.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.4f, 1.5f, -0.2f);


    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.4f, -1.5f, -0.2f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.4f, -1.5f, 0.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.4f, 1.5f, 0.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.4f, 1.5f, -0.2f);

 
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.4f, 1.5f, -0.2f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.4f, 1.5f, 0.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.4f, 1.5f, 0.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.4f, 1.5f, -0.2f);


    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.4f, -1.5f, -0.2f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.4f, -1.5f, 0.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.4f, -1.5f, 0.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.4f, -1.5f, -0.2f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();


    GLfloat armAmbient3[] = { 1.0f, 1.0f, 1.0f,1.0f }; 
    glMaterialfv(GL_FRONT, GL_AMBIENT, armAmbient3);


    glPushMatrix();
    glTranslatef(0.0f, 2.6f, -0.5f); 
    glColor3f(1.0f, 1.0f, 1.0f); 
    gluSphere(quad, 0.2f, 5, 5);
    glPopMatrix();

    GLfloat defaultAmbient3[] = { 1.0f, 0.6f, 0.6f, 1.0f }; 
    glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient3);

    GLfloat armAmbient2[] = { 0.0f, 0.0f, 0.0f,1.0f }; 
    glMaterialfv(GL_FRONT, GL_AMBIENT, armAmbient2);

    glPushMatrix();
    glTranslatef(0.2f, 2.0f, -0.5f); 
    glColor3f(1.0f, 1.0f, 1.0f); 
    gluSphere(quad, 0.15f, 5, 5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.2f, 2.0f,- 0.5f); 
    glColor3f(1.0f, 1.0f, 1.0f);
    gluSphere(quad, 0.15f, 5, 5);
    glPopMatrix();

 
    GLfloat defaultAmbient2[] = { 1.0f, 0.6f, 0.6f, 1.0f }; 
    glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient2);

    GLfloat armAmbient1[] = { 0.7f, 0.5f, 0.3f,1.0f }; 
    glMaterialfv(GL_FRONT, GL_AMBIENT, armAmbient1);

    glPushMatrix();
    glTranslatef(0.5f, 2.0f, 0.0f); 
    glColor3f(1.0f, 1.0f, 1.0f); 
    gluSphere(quad, 0.2f, 5, 5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.5f, 2.0f, 0.0f); 
    glColor3f(1.0f, 1.0f, 1.0f);
    gluSphere(quad, 0.2f, 5, 5);
    glPopMatrix();

    GLfloat defaultAmbient1[] = { 1.0f, 0.6f, 0.6f, 1.0f }; 
    glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient1);

    GLfloat armAmbient[] = { 1.0f, 0.0f, 0.0f, 1.0f }; 
    glMaterialfv(GL_FRONT, GL_AMBIENT, armAmbient);

    glPushMatrix();
    glTranslatef(0.0f, 2.4f, 0.0f); 
    glColor3f(1.0f, 1.0f, 1.0f); 
    gluSphere(quad, 0.6f, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 2.0f, 0.0f); 
    glColor3f(1.0f, 1.0f, 1.0f); 
    gluSphere(quad, 0.5f, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.7f, 1.5f, 0.0f); 
    glColor3f(0.7f, 0.5f, 0.3f); 
    glutSolidSphere(0.2f, 20, 20); 
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.7f, 1.0f, 0.0f); 
    glScalef(0.2f, 1.0f, 0.2f); 
    glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.7f, 0.5f, 0.0f); 
    glColor3f(0.7f, 0.5f, 0.3f);
    glutSolidSphere(0.2f, 20, 20); 
    glPopMatrix();


    glPushMatrix();
    glTranslatef(-0.7f, 0.5f, 0.0f); 
    glRotatef(-armSwingAngle, 1.0f, 0.0f, 0.0f);  

    glTranslatef(0.0f, -0.5f, 0.0f); 
    glScalef(0.2f, 1.0f, 0.2f); 
    glutSolidCube(1.0f); 
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.7f, 1.5f, 0.0f); 
    glColor3f(0.7f, 0.5f, 0.3f); 
    glutSolidSphere(0.2f, 20, 20); 
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.7f, 1.0f, 0.0f); 
    glScalef(0.2f, 1.0f, 0.2f); 
    glutSolidCube(1.0f); 
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.7f, 0.5f, 0.0f); 
    glColor3f(0.7f, 0.5f, 0.3f); 
    glutSolidSphere(0.2f, 20, 20); 
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.7f, 0.5f, 0.0f);
    glRotatef(armSwingAngle, 1.0f, 0.0f, 0.0f); 
    glTranslatef(0.0f, -0.5f, 0.0f); 
    glScalef(0.2f, 1.0f, 0.2f); 
    glutSolidCube(1.0f); 
    glPopMatrix();

    GLfloat defaultAmbient[] = { 1.0f, 0.6f, 0.6f, 1.0f }; 
    glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);

    GLfloat legAmbient[] = { 0.0f, 0.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, legAmbient);

    glPushMatrix();
    glTranslatef(-0.5f, -1.5f, 0.0f);
    glScalef(0.2f, 2.0f, 0.2f); 
    glutSolidCube(1.0f); 
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.5f, -1.5f, 0.0f); 
    glScalef(0.2f, 2.0f, 0.2f);
    glutSolidCube(1.0f); 
    glPopMatrix();


    glMaterialfv(GL_FRONT, GL_AMBIENT, defaultAmbient);

    glPopMatrix(); 

    gluDeleteQuadric(quad); 
}

void drawBase(int season) {

    glPushMatrix();

    if (season == 0 || season == 1) {

        GLfloat matAmbient[] = { 0.0f, 0.3f, 0.0f, 1.0f }; 
        GLfloat matDiffuse[] = { 0.0f, 0.6f, 0.0f, 1.0f }; 
        GLfloat matSpecular[] = { 0.1f, 0.1f, 0.1f, 1.0f }; 
        GLfloat matShininess[] = { 10.0f };                

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);

        glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-100.0f, -4.0f, -100.0f);
        glVertex3f(100.0f, -4.0f, -100.0f);
        glVertex3f(100.0f, -4.0f, 100.0f);
        glVertex3f(-100.0f, -4.0f, 100.0f);
        glEnd();
    }
    else {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture[season]); 


        GLfloat matAmbientGrass[] = { 1.0f, 1.0f, 1.0f, 1.0f }; 
        GLfloat matDiffuseGrass[] = { 0.1f, 0.1f, 0.1f, 1.0f }; 
        GLfloat matSpecularGrass[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        GLfloat matShininessGrass[] = { 0.0f }; 

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbientGrass);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuseGrass);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecularGrass);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininessGrass);


        glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        float grassRepeat = 20.0f; 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-100.0f, -4.0f, -100.0f);
        glTexCoord2f(grassRepeat, 0.0f); glVertex3f(100.0f, -4.0f, -100.0f);
        glTexCoord2f(grassRepeat, grassRepeat); glVertex3f(100.0f, -4.0f, 100.0f);
        glTexCoord2f(0.0f, grassRepeat); glVertex3f(-100.0f, -4.0f, 100.0f);
        glEnd();

        glDisable(GL_TEXTURE_2D);
    }

    glPopMatrix();

}

void drawSkybox() {
    float size = 90.0f; 
    glDisable(GL_LIGHTING);  

    glPushMatrix();  
    glLoadIdentity(); 

    glColor3f(0.5f, 0.7f, 1.0f);  


    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f); 
    glVertex3f(-size, size, size);
    glVertex3f(size, size, size);
    glVertex3f(size, -size, size);
    glVertex3f(-size, -size, size);
    glEnd();

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f); 
    glVertex3f(size, size, -size);
    glVertex3f(-size, size, -size);
    glVertex3f(-size, -size, -size);
    glVertex3f(size, -size, -size);
    glEnd();

    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f); 
    glVertex3f(-size, size, -size);
    glVertex3f(-size, size, size);
    glVertex3f(-size, -size, size);
    glVertex3f(-size, -size, -size);
    glEnd();


    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f); 
    glVertex3f(size, size, size);
    glVertex3f(size, size, -size);
    glVertex3f(size, -size, -size);
    glVertex3f(size, -size, size);
    glEnd();


    glEnable(GL_LIGHTING);  

    glPopMatrix();  
}


void drawBranches(float treeX, float treeZ) {
    float branchAngleStep = 30.0f; 
    float branchLength = 4.0f; 
    float branchRadius = 0.1f; 
    float treeHeight = 3.7f; 


    float treeRadius = 0.3f;

    for (int i = 0; i < 12; i++) {

        glPushMatrix();

        float angle = i * branchAngleStep * M_PI / 180.0f;


        float branchX = treeX + cos(angle) * treeRadius;
        float branchZ = treeZ + sin(angle) * treeRadius;


        glTranslatef(branchX, treeHeight, branchZ); 

        glRotatef(i * branchAngleStep, 0.0f, 1.0f, 0.0f); 

        GLfloat matAmbient[] = { 0.55f, 0.27f, 0.07f, 1.0f };  
        GLfloat matDiffuse[] = { 0.55f, 0.27f, 0.07f, 1.0f };
        GLfloat matSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f };  
        GLfloat matShininess[] = { 10.0f }; 

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);

        glRotatef(-45.0f, 1.0f, 0.0f, 0.0f); 


        glutSolidCone(branchRadius * 2.0f, branchLength, 10, 10); 

        glPopMatrix();

        glPushMatrix();

        branchX = treeX + cos(angle) * treeRadius;
        branchZ = treeZ + sin(angle) * treeRadius;

        glTranslatef(branchX, treeHeight + 2.0f, branchZ); 

        glRotatef(i * branchAngleStep, 0.0f, 1.0f, 0.0f);

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);

        glRotatef(-45.0f, 1.0f, 0.0f, 0.0f);

        glutSolidCone(branchRadius * 1.5f, branchLength - 1.0f, 10, 10);

        glPopMatrix();
    }
}

void setLeafMaterial(int season) {
    GLfloat matLeafAmbient[4];
    GLfloat matLeafDiffuse[4];
    GLfloat matLeafSpecular[] = { 0.2f, 0.2f, 0.2f, 1.0f }; 
    GLfloat matLeafShininess[] = { 10.0f };

    switch (season) {
    case 0: 
        matLeafAmbient[0] = 0.0f;
        matLeafAmbient[1] = 0.8f;
        matLeafAmbient[2] = 0.0f;
        matLeafAmbient[3] = 0.8f;

        matLeafDiffuse[0] = 0.0f;
        matLeafDiffuse[1] = 1.0f;
        matLeafDiffuse[2] = 0.0f;
        matLeafDiffuse[3] = 1.0f;
        break;
    case 1:
        matLeafAmbient[0] = 0.0f;
        matLeafAmbient[1] = 0.5f;
        matLeafAmbient[2] = 0.0f;
        matLeafAmbient[3] = 1.0f;

        matLeafDiffuse[0] = 0.0f;
        matLeafDiffuse[1] = 0.5f;
        matLeafDiffuse[2] = 0.0f;
        matLeafDiffuse[3] = 1.0f;
        break;
    case 2: { 
        if (rand() % 2 == 0) {
            matLeafAmbient[0] = 0.9f;  
            matLeafAmbient[1] = 0.5f;  
            matLeafAmbient[2] = 0.0f;  
            matLeafAmbient[3] = 1.0f;  

            matLeafDiffuse[0] = 1.0f;  
            matLeafDiffuse[1] = 1.0f;  
            matLeafDiffuse[2] = 0.5f;  
            matLeafDiffuse[3] = 1.0f;  
        }
        else {
  
            matLeafAmbient[0] = 1.0f;  
            matLeafAmbient[1] = 0.3f;  
            matLeafAmbient[2] = 0.0f;   
            matLeafAmbient[3] = 1.0f;  

            matLeafDiffuse[0] = 0.8f;   
            matLeafDiffuse[1] = 0.45f; 
            matLeafDiffuse[2] = 0.0f;  
            matLeafDiffuse[3] = 1.0f;   
        }
        break;
    }
    case 3: 
        matLeafAmbient[0] = 1.0f;
        matLeafAmbient[1] = 1.0f;
        matLeafAmbient[2] = 1.0f;
        matLeafAmbient[3] = 1.0f;

        matLeafDiffuse[0] = 1.0f;
        matLeafDiffuse[1] = 1.0f;
        matLeafDiffuse[2] = 1.0f;
        matLeafDiffuse[3] = 1.0f;
        break;
    default: 
        matLeafAmbient[0] = 0.0f;
        matLeafAmbient[1] = 0.8f;
        matLeafAmbient[2] = 0.0f;
        matLeafAmbient[3] = 1.0f;

        matLeafDiffuse[0] = 0.0f;
        matLeafDiffuse[1] = 0.8f;
        matLeafDiffuse[2] = 0.0f;
        matLeafDiffuse[3] = 1.0f;
        break;
    }

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matLeafAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matLeafDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matLeafSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matLeafShininess);
}

void drawTree(float x, float z, int season) {
    srand((unsigned int)(x * 1000 + z + season));

    glPushMatrix();
    glTranslatef(x, -2.0f, z); 
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); 

    GLfloat matAmbient[] = { 0.55f, 0.27f, 0.07f, 1.0f }; 
    GLfloat matDiffuse[] = { 0.55f, 0.27f, 0.07f, 1.0f };  
    GLfloat matSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f }; 
    GLfloat matShininess[] = { 10.0f };  

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);

    gluCylinder(quadric, 0.6f, 0.6f, 10.0f, 20, 20);  
    glPopMatrix();

   
    if (season != 3 || rand()%2 ==0) {  
        glPushMatrix();
        glTranslatef(x, 0.0f, z);  

        setLeafMaterial(season); 
     
        glPushMatrix();
        glTranslatef(0.0f, 5.0f, 0.0f); 
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);  
        glutSolidCone(4.0f, 8.0f, 20, 20);  
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.0f, 9.0f, 0.0f);  
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); 
        glutSolidCone(3.0f, 6.0f, 20, 20); 
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.0f, 13.0f, 0.0f); 
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); 
        glutSolidCone(2.0f, 4.0f, 20, 20);  
        glPopMatrix();

        glPopMatrix();
    }

    drawBranches(x , z);


}

void drawTreesAtCrossroad(float season) {
    float x = 15.0f;
    float z = 15.0f;

    for (float i = 0.0f; i < 6; i++) {
        for (float j = 0.0f; j < 6; j++) {
            drawTree( x + 10.0 * i, z + 10.0 * j, season);
            drawTree(-x - 10.0 * i, z +10.0 * j, season);
            drawTree( x + 10.0 * i, -z - 10.0 * j, season);
            drawTree(-x - 10.0 * i, -z - 10.0 * j, season);
        }
    }
}


struct Particle {
    float x, y, z;           
    float velocityX, velocityY, velocityZ; 
    float size;              
    float r, g, b, a;        
    bool active;             
};

const int MAX_LEAVES = 10000;
const int MAX_SNOWFLAKES = 10000;

Particle leaves[MAX_LEAVES];
Particle snowflakes[MAX_SNOWFLAKES];

void initLeaves() {
    for (int i = 0; i < MAX_LEAVES; ++i) {
        leaves[i].active = true;
        leaves[i].x = ((float)rand() / RAND_MAX) * 100.0f - 50.0f;   
        leaves[i].y = ((float)rand() / RAND_MAX) * 50.0f + 10.0f;    
        leaves[i].z = ((float)rand() / RAND_MAX) * 100.0f - 50.0f;   
        leaves[i].velocityX = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;   
        leaves[i].velocityY = -((float)rand() / RAND_MAX) * 0.3f - 0.1f; 
        leaves[i].velocityZ = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;  
        leaves[i].size = 0.3f + ((float)rand() / RAND_MAX) * 0.4f;   
        leaves[i].r = 1.0f;
        leaves[i].g = 0.6f + ((float)rand() / RAND_MAX) * 0.4f;   
        leaves[i].b = 0.0f;
        leaves[i].a = 1.0f;
    }
}

void initSnowflakes() {
    for (int i = 0; i < MAX_SNOWFLAKES; ++i) {
        snowflakes[i].active = true;
        snowflakes[i].x = ((float)rand() / RAND_MAX) * 100.0f - 50.0f; 
        snowflakes[i].y = ((float)rand() / RAND_MAX) * 50.0f + 10.0f;  
        snowflakes[i].z = ((float)rand() / RAND_MAX) * 100.0f - 50.0f; 
        snowflakes[i].velocityX = ((float)rand() / RAND_MAX - 0.5f) * 1.5f;  
        snowflakes[i].velocityY = -((float)rand() / RAND_MAX) * 0.1f - 0.05f; 
        snowflakes[i].velocityZ = ((float)rand() / RAND_MAX - 0.5f) * 1.5f; 
        snowflakes[i].size = 0.3f + ((float)rand() / RAND_MAX) * 0.4f;  
        snowflakes[i].r = 1.0f;
        snowflakes[i].g = 1.0f;
        snowflakes[i].b = 1.0f;
        snowflakes[i].a = 0.8f;
    }
}

void updateParticles() {

    for (int i = 0; i < MAX_LEAVES; ++i) {
        if (leaves[i].active) {
            leaves[i].velocityX += ((float)rand() / RAND_MAX - 0.5f) * 0.1f; 
            leaves[i].velocityZ += ((float)rand() / RAND_MAX - 0.5f) * 0.1f;
            leaves[i].velocityY += ((float)rand() / RAND_MAX - 0.5f) * 0.05f; 


            leaves[i].x += leaves[i].velocityX;
            leaves[i].y += leaves[i].velocityY;
            leaves[i].z += leaves[i].velocityZ;

            if (leaves[i].y < -4.0f) {
                leaves[i].x = ((float)rand() / RAND_MAX) * 100.0f - 50.0f;
                leaves[i].y = ((float)rand() / RAND_MAX) * 50.0f + 10.0f;
                leaves[i].z = ((float)rand() / RAND_MAX) * 100.0f - 50.0f;
                leaves[i].velocityX = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;
                leaves[i].velocityY = -((float)rand() / RAND_MAX) * 0.3f - 0.1f;
                leaves[i].velocityZ = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;
            }
        }
    }


    for (int i = 0; i < MAX_SNOWFLAKES; ++i) {
        if (snowflakes[i].active) {

            snowflakes[i].velocityX += ((float)rand() / RAND_MAX - 0.5f) * 0.05f; 
            snowflakes[i].velocityZ += ((float)rand() / RAND_MAX - 0.5f) * 0.05f; 
            snowflakes[i].velocityY += ((float)rand() / RAND_MAX - 0.5f) * 0.02f; 

            snowflakes[i].x += snowflakes[i].velocityX;
            snowflakes[i].y += snowflakes[i].velocityY;
            snowflakes[i].z += snowflakes[i].velocityZ;

            if (snowflakes[i].y < -4.0f) {
                snowflakes[i].x = ((float)rand() / RAND_MAX) * 100.0f - 50.0f;
                snowflakes[i].y = ((float)rand() / RAND_MAX) * 50.0f + 10.0f;
                snowflakes[i].z = ((float)rand() / RAND_MAX) * 100.0f - 50.0f;
                snowflakes[i].velocityX = ((float)rand() / RAND_MAX - 0.5f) * 1.5f;
                snowflakes[i].velocityY = -((float)rand() / RAND_MAX) * 0.1f - 0.05f;
                snowflakes[i].velocityZ = ((float)rand() / RAND_MAX - 0.5f) * 1.5f;
            }
        }
    }
}

void renderParticles(int season) {
    if (season == 2) {  
        glDisable(GL_LIGHTING);        
        glEnable(GL_BLEND);           
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

        glPointSize(4.0f);             
        glBegin(GL_POINTS);
        for (int i = 0; i < MAX_LEAVES; ++i) {
            if (leaves[i].active) {
                glColor4f(leaves[i].r, leaves[i].g, leaves[i].b, leaves[i].a);
                glVertex3f(leaves[i].x, leaves[i].y, leaves[i].z);
            }
        }
        glEnd();

        glDisable(GL_BLEND);          
        glEnable(GL_LIGHTING);          
    }
    else if (season == 3) {  
        glDisable(GL_LIGHTING);        
        glEnable(GL_BLEND);           
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

        glPointSize(2.0f);              
        glBegin(GL_POINTS);
        for (int i = 0; i < MAX_SNOWFLAKES; ++i) {
            if (snowflakes[i].active) {
                glColor4f(snowflakes[i].r, snowflakes[i].g, snowflakes[i].b, snowflakes[i].a);
                glVertex3f(snowflakes[i].x, snowflakes[i].y, snowflakes[i].z);
            }
        }
        glEnd();

        glDisable(GL_BLEND);            
        glEnable(GL_LIGHTING);         
    }
}


void drawLightSources() {

    glDisable(GL_LIGHT0);

    glPushMatrix();
    glColor3f(1.0f, 1.0f, 0.0f); 
    glTranslatef(lightPos[0], lightPos[1], lightPos[2]); 
    glutSolidSphere(0.5f, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);  
    GLfloat moonPos[4];
    moonPos[0] = -lightPos[0];  
    moonPos[1] = -lightPos[1];
    moonPos[2] = lightPos[2];
    glTranslatef(moonPos[0], moonPos[1], moonPos[2]);  
    glutSolidSphere(0.5f, 20, 20);  
    glPopMatrix();

    glEnable(GL_LIGHT0);

}

void updateLightPosition() {
    lightPos[0] = 100.0f * cos(- lightAngle);  
    lightPos[1] = 100.0f * sin(- lightAngle);  
    lightPos[2] = 5.0f;                     
    lightPos[3] = 1.0f;                    

    glEnable(GL_LIGHTING);  
    glEnable(GL_LIGHT0);   

    GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };  
    GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };  
    GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };  

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);  

    if (lightPos[1] > -4.0f) {
        glEnable(GL_LIGHT0);
    }
    else {
        glDisable(GL_LIGHT0);
    }
}

void setCameraView() {
    glLoadIdentity();  

   
    float camPosX = sin(camAngleY * M_PI / 180.0f) * cos(camAngleX * M_PI / 180.0f) * camDist + camOffsetX;
    float camPosY = sin(camAngleX * M_PI / 180.0f) * camDist + camOffsetY;
    float camPosZ = -cos(camAngleY * M_PI / 180.0f) * cos(camAngleX * M_PI / 180.0f) * camDist + camOffsetZ;

    gluLookAt(camPosX, camPosY, camPosZ,    
        personPosX, personPosY, personPosZ,    
        0.0f, 1.0f, 0.0f);   
    }


void renderScene(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    setCameraView(); 
    updateLightPosition();

    drawSkybox();

    drawBase(season);
    drawPerson();
    drawTreesAtCrossroad(season);

    updateParticles();
    renderParticles(season);

    drawLightSources();

    glutSwapBuffers(); 
}

void setupPerspective() {
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
    gluPerspective(45.0f, 1.0f, 1.0f, 100.0f); 
    glMatrixMode(GL_MODELVIEW); 
}

void timerFunc(int value) {
    if (lightAngle <= 1.0f * M_PI) {
        lightAngle += 2.0f * M_PI / (2.0f * 60.0f); 
    }
    else {
        lightAngle += 2.0f * M_PI / (10.0f * 60.0f);
    }
    if (lightAngle > 2.0f * M_PI) {
        lightAngle -= 2.0f * M_PI;
        if (season <= 2) {
            season = season + 1.0f;
        }
        else {
            season = 0.0f; 
        }

    }

    glutPostRedisplay(); 
    glutTimerFunc(1000 / 60, timerFunc, 0); 
}

void handleKeypress(unsigned char key, int x, int y) {
    switch (key) {
    case 'w':
        personPosZ -= cos(personRotation * M_PI / 180.0f) * moveSpeed;  
        camOffsetZ -= cos(personRotation * M_PI / 180.0f) * moveSpeed;
        personPosX -= sin(personRotation * M_PI / 180.0f) * moveSpeed;
        camOffsetX -= sin(personRotation * M_PI / 180.0f) * moveSpeed;
        armSwingAngle = sin(personPosZ * 2.0f) * 20.0f;  
        break;
    case 'a':
        personPosZ += sin(personRotation * M_PI / 180.0f) * moveSpeed;
        camOffsetZ += sin(personRotation * M_PI / 180.0f) * moveSpeed;
        personPosX -= cos(personRotation * M_PI / 180.0f) * moveSpeed;
        camOffsetX -= cos(personRotation * M_PI / 180.0f) * moveSpeed;

        break;
    case 's':

        personPosZ += cos(personRotation * M_PI / 180.0f) * moveSpeed;  
        camOffsetZ += cos(personRotation * M_PI / 180.0f) * moveSpeed;
        personPosX += sin(personRotation * M_PI / 180.0f) * moveSpeed;
        camOffsetX += sin(personRotation * M_PI / 180.0f) * moveSpeed;
        armSwingAngle = sin(personPosZ * 2.0f) * 20.0f;  
        break;
    case 'd':

        personPosZ -= sin(personRotation * M_PI / 180.0f) * moveSpeed;
        camOffsetZ -= sin(personRotation * M_PI / 180.0f) * moveSpeed;
        personPosX += cos(personRotation * M_PI / 180.0f) * moveSpeed;  
        camOffsetX += cos(personRotation * M_PI / 180.0f) * moveSpeed;

        break;

    case 'q':
        personRotation += 10.0f;  
        camAngleY = - personRotation - 180.0f;
        break;
    case 'e':
        personRotation -= 10.0f;
        camAngleY = -personRotation - 180.0f;
        break;
    case 'f':
        moveSpeed += 0.5f;
        break;
    case 'r':
        moveSpeed -=0.5f;
        break;
    case 'y':
        camOffsetY += 0.5f;
        break;
    case 'h':
        camOffsetY -= 0.5f;
        break;
    case 't':
        camOffsetX += 0.5f;
        break;
    case 'u':
        camOffsetX -= 0.5f;
        break;
    case 'j':
        camOffsetZ += 0.5f;
        break;
    case 'g':
        camOffsetZ -= 0.5f;
        break;
    case 'n':
        camDist += 1.0f;
        break;
    case 'm':
        camDist -= 1.0f;
        break;
    case 'b':
        if (lightAngle <= 1.0f * M_PI) {
            lightAngle += 2.0f * M_PI / (0.1f * 60.0f); 
        }
        else {
            lightAngle += 2.0f * M_PI / (0.5f * 60.0f);
        }
        break;
    case 27:  
        exit(0);  
        break;
    default:
        break;
    }
    glutPostRedisplay();  
}

void mouseMove(int x, int y) {
    if (firstMouse) {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }
    float xOffset = x - lastX;
    float yOffset = y - lastY;  
    lastX = x;
    lastY = y;

    camAngleX -= 0.008f * yOffset ;
    camAngleY += 1.0f * xOffset;

    glutPostRedisplay();  
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 750);
    glutCreateWindow("3D Model");

    myinit();  

    initLeaves();
    initSnowflakes();

    initQuadric();


    setupPerspective();  

    
    glutMotionFunc(mouseMove); 
    glutPassiveMotionFunc(mouseMove);   

    glutKeyboardFunc(handleKeypress);  

    glutDisplayFunc(renderScene);  

    glutTimerFunc(0, timerFunc, 0); 

    glutMainLoop();
    return 0;
}
