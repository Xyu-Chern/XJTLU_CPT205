#define FREEGLUT_STATIC 
#include <GL/freeglut.h>
#include <cmath>
#include <string>
#include <vector>
#include <cstdlib>

enum ArrowState { NONE, LEFT, RIGHT };
ArrowState hoveredArrow = NONE;

struct FireworkStripe {
    float x, y;          
    float speed;         
    float color[3];      
    float angle;         
    float alpha;         
};
std::vector<FireworkStripe> stripes; 
bool leftHovered = false, rightHovered = false; 

int page = 0;             
float M_PI = 3.1415926;

float backgrondColorR = 0.9f;
float backgrondColorG = 0.8f;
float backgrondColorB = 0.6f;

float backgrondCoverColorR = 0.95f; 
float backgrondCoverColorG = 0.95f;
float backgrondCoverColorB = 0.9f;

bool leftArrowClicked = false; 
bool rightArrowClicked = false; 

bool bulbHovered = false;    
bool bulbClicked = false;    

bool buttonHovered = false;

// control banner
float bannerOffsetX = 1.0f;    
float bannerSpeed = 0.001f;    
bool bannerActive = false;     

// helper functions
void setColor(float r, float g, float b) {
    glColor3f(r, g, b);
}

void drawQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);
    glVertex2f(x4, y4);
    glEnd();
}

void drawEllipse(float centerX, float centerY, float radiusX, float radiusY, float r, float g, float b) {
    setColor(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(centerX, centerY);
    for (float angle = 0; angle <= 2 * M_PI; angle += 0.01f) {
        float x = centerX - radiusX * cos(angle);
        float y = centerY - radiusY * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}
void drawPetal(float centerX, float centerY, float radiusX, float radiusY, float rotationAngle, float r, float g, float b) {
    setColor(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(centerX, centerY);
    for (float angle = 0; angle <= 2 * M_PI; angle += 0.01f) {
        float x = radiusX * cos(angle);
        float y = radiusY * sin(angle);
        float rotatedX = centerX + x * cos(rotationAngle) - y * sin(rotationAngle);
        float rotatedY = centerY + x * sin(rotationAngle) + y * cos(rotationAngle);
        glVertex2f(rotatedX, rotatedY);
    }
    glEnd();
}
void drawLeaf(float centerX, float centerY, float radiusX, float radiusY) {
    for (int i = 0; i < 6; ++i) {
        float angle = i * (M_PI / 3) - M_PI / 3;
        float leafX = centerX + 0.05 * cos(angle);
        float leafY = centerY + 0.05 * sin(angle);
        drawPetal(leafX, leafY, radiusX, radiusY, angle, 0.3f, 0.7f, 0.3f);// make sure green color
    }
}
void drawFlowerPetal(float centerX, float centerY, float radiusX, float radiusY) {
    for (int i = 0; i < 12; i++) {
        float angle = i * (2 * M_PI / 12);
        float petalX = centerX + radiusX * cos(angle);
        float petalY = centerY + radiusY * sin(angle);
        drawPetal(petalX, petalY, 0.05f, 0.01f, angle, 1.0f, 1.0f, 1.0f);// make sure white color
    }
}
void drawText(const std::string& text, float x, float y, void* font = GLUT_BITMAP_HELVETICA_12) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(font, c);
    }
}
void drawHalfEllipse(float centerX, float centerY, float radiusX, float radiusY) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(centerX, centerY);
    for (float angle = 0; angle <= 3.1415926f; angle += 0.01f) { // only 0 to pi
        float x = centerX + radiusX * cos(angle);
        float y = centerY - radiusY * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

void drawCakeLayer(float centerX, float centerY, float width, float height, float borderHeight) {
    float segmentWidth = width / 6.0f;
    setColor(0.99f, 0.75f, 0.2f);
    drawQuad(centerX - width / 2, centerY - height / 2,
        centerX - segmentWidth / 2, centerY - height / 2,
        centerX - segmentWidth / 2, centerY + height / 2,
        centerX - width / 2, centerY + height / 2);
    setColor(0.99f, 0.8f, 0.3f);
    drawQuad(centerX - segmentWidth / 2, centerY - height / 2,
        centerX + segmentWidth / 2, centerY - height / 2,
        centerX + segmentWidth / 2, centerY + height / 2,
        centerX - segmentWidth / 2, centerY + height / 2);
    setColor(0.99f, 0.85f, 0.4f);
    drawQuad(centerX + segmentWidth / 2, centerY - height / 2,
        centerX + width / 2, centerY - height / 2,
        centerX + width / 2, centerY + height / 2,
        centerX + segmentWidth / 2, centerY + height / 2);
    setColor(1.0f, 1.0f, 1.0f);
    for (float x = centerX - width / 3.0f; x <= centerX + width / 3.0f; x += segmentWidth) {
        drawHalfEllipse(x, centerY + height / 2, segmentWidth / 2, borderHeight / 2);
    }
}

void drawCandleOnCake(float centerX, float centerY) {

    setColor(0.8f, 0.5f, 0.3f);
    drawQuad(centerX - 0.005f, centerY - 0.1f, centerX + 0.005f, centerY - 0.1f,
        centerX + 0.005f, centerY + 0.05f, centerX - 0.005f, centerY + 0.05f);
}

void drawScaleTriangle(float x1, float x2, float x3) {
    glBegin(GL_TRIANGLES);
    glVertex2f(x1, 0.0f);
    glVertex2f(0.0f, x2);
    glVertex2f(x3, 0.0f);
    glEnd();
}
// draw case 0 objects
void drawCrease() {
    setColor(0.8f, 0.8f, 0.8f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(0.0f, -0.5f);
    glVertex2f(0.0f, 0.5f);
    glEnd();
}

void drawBackground(bool isLeft, bool isCover) {
    if (isCover) {
        setColor(backgrondCoverColorR, backgrondCoverColorG, backgrondCoverColorB);
    }
    else {
        setColor(backgrondColorR, backgrondColorG, backgrondColorB);
    }
    float offset = isCover ? 0.025f : 0.0f;
    float startX = isLeft ? -0.6f + offset : 0.0f;
    float endX = isLeft ? 0.0f : 0.6f - offset;
    drawQuad(startX, -0.5f + offset, endX, -0.5f + offset, endX, 0.5f - offset, startX, 0.5f - offset);
}

void drawFontCover() {
    drawEllipse(0.2875f, 0.14f, 0.03f, 0.09f, 0.8f, 0.0f, 0.0f); // red fire
    drawEllipse(0.2875f, 0.086f, 0.017f, 0.046f, 0.9f, 0.9f, 0.2f); // yellow fire
    setColor(0.8f, 0.5f, 0.3f);
    drawQuad(0.26f, -0.2f, 0.315f, -0.2f, 0.315f, 0.04f, 0.26f, 0.04f);
    drawLeaf(0.39f, -0.2f, 0.045f, 0.0145f);
    drawLeaf(0.185f, -0.2f, 0.045f, 0.0145f);
    drawFlowerPetal(0.2875f, -0.2f, 0.0275f, 0.0275f);
    drawEllipse(0.2875f, -0.2f, 0.0275f, 0.0275f, 1.0f, 0.75f, 0.4f);

}
void drawTitle() {
    setColor(1.f, 1.f, 1.f);
    drawText("HAPPY BIRTHDAY", 0.104f, 0.27f, GLUT_BITMAP_HELVETICA_18);
}

void drawGuide() {
    setColor(0.0f, 0.0f, 0.0f);
    drawText("User Guide:", -0.9f, 0.34f, GLUT_BITMAP_HELVETICA_18);
    drawText("Press Esc to exit", -0.9f, 0.25f, GLUT_BITMAP_HELVETICA_18);
    drawText("Click the left and right arrows to flip pages", -0.9f, 0.15f, GLUT_BITMAP_HELVETICA_18);
    drawText("Click the fireworks tube to release stripes", -0.9f, 0.05f, GLUT_BITMAP_HELVETICA_18);
    drawText("Click the bulb to turn off the light", -0.9f, -0.05f, GLUT_BITMAP_HELVETICA_18);
}

void drawArrows(bool isLeft) {
    glPushMatrix();
    if (isLeft) {
        if (hoveredArrow == LEFT) {
            glTranslatef(-0.875f, -0.8f, 0.0f);
            glScalef(1.2f, 1.2f, 1.0f);
            glTranslatef(0.875f, 0.8f, 0.0f);
            setColor(1.0f, 0.0f, 0.0f);
        }
        else {
            setColor(0.2f, 0.2f, 0.2f);
        }
        glBegin(GL_TRIANGLES);
        glVertex2f(-0.9f, -0.8f);
        glVertex2f(-0.85f, -0.75f);
        glVertex2f(-0.85f, -0.85f);
        glEnd();
    }
    else {
        if (hoveredArrow == RIGHT) {
            glTranslatef(0.875f, -0.8f, 0.0f);
            glScalef(1.2f, 1.2f, 1.0f);
            glTranslatef(-0.875f, 0.8f, 0.0f);
            setColor(1.0f, 0.0f, 0.0f);
        }
        else {
            setColor(0.2f, 0.2f, 0.2f);
        }
        glBegin(GL_TRIANGLES);
        glVertex2f(0.9f, -0.8f);
        glVertex2f(0.85f, -0.75f);
        glVertex2f(0.85f, -0.85f);
        glEnd();
    }
    glPopMatrix();
}

// draw case 1 objects
void drawCake() {
    drawCakeLayer(0.0f, 0.16f, 0.3f, 0.16f, 0.04f);
    drawCakeLayer(0.0f, -0.01f, 0.4f, 0.18f, 0.06f);
    drawCakeLayer(0.0f, -0.2f, 0.5f, 0.2f, 0.06f);
    drawCandleOnCake(0.0f, 0.34f);
    drawCandleOnCake(-0.02f, 0.34f);
    drawCandleOnCake(0.02f, 0.34f);
}

void drawFireworksTube(float centerX, float centerY, float baseWidth, float height, float angle, bool hovered) {
    glPushMatrix();

    glTranslatef(centerX, centerY, 0.0f);  
    glRotatef(angle, 0.0f, 0.0f, 1.0f);   
    float scale = hovered ? 1.2f : 1.0f; 

    setColor(1.0f, 0.8f, 0.5f); 
    drawScaleTriangle(-baseWidth / 2 * scale, height * scale, baseWidth / 2 * scale);
    setColor(1.0f, 0.7f, 0.4f);
    drawScaleTriangle(-baseWidth / 2 * 2 / 3 * scale, height * scale, baseWidth / 2 * 2 / 3 * scale);
    setColor(1.0f, 0.6f, 0.3f);
    drawScaleTriangle(-baseWidth / 2 / 3 * scale, height * scale, baseWidth / 2 / 3 * scale);

    glPopMatrix();
}

void drawStripe(FireworkStripe& stripe) {
    glColor4f(stripe.color[0], stripe.color[1], stripe.color[2], stripe.alpha); 
    glBegin(GL_LINES);
    glVertex2f(stripe.x, stripe.y);
    glVertex2f(stripe.x + 0.2f * cos(stripe.angle), stripe.y + 0.1f);
    glEnd();
}

void drawBulb(bool enlarge) {
    float scale = enlarge ? 1.2f : 1.0f;
    glPushMatrix();
    glTranslatef(0.8f, 0.7f, 0.0f);
    glScalef(scale, scale, 1.0f);

    setColor(0.8f, 0.8f, 0.8f); // grey
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.08f, 0.0f);
    glVertex2f(0.0f, 0.3f);
    glVertex2f(0.08f, 0.0f);
    glEnd();

    setColor(0.95f, 0.95f, 0.3f); // yellow circle
    glBegin(GL_POLYGON);
    for (float angle = 0; angle <= 2 * 3.14159f; angle += 0.1f) {
        float x = 0.1f * cos(angle);
        float y = 0.1f * sin(angle);
        glVertex2f(x, y + 0.0f);
    }
    glEnd();
    glPopMatrix();
}

void drawButton(const std::string& text, float x, float y, float width, float height) {
    glPushMatrix();
    if (buttonHovered) {
        glTranslatef(x, y, 0.0f);
        glScalef(1.2f, 1.2f, 1.0f); // up to 1.2 scale
        glTranslatef(-x, -y, 0.0f);
    }

    if (buttonHovered) {
        setColor(0.5f, 0.5f, 0.5f); 
    }
    else if (bulbClicked) {
        setColor(0.3f, 0.3f, 0.3f); 
    }
    else {
        setColor(0.7f, 0.7f, 0.7f); 
    }
    drawQuad(x - width / 2, y - height / 2, x + width / 2, y - height / 2, x + width / 2, y + height / 2, x - width / 2, y + height / 2);

    // change string color
    if (buttonHovered) {
        setColor(1.0f, 1.0f, 1.0f); // white
    }
    else {
        setColor(0.0f, 0.0f, 0.0f); // black
    }
    glRasterPos2f(x - width / 4, y - height / 8);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
    glPopMatrix();
}

void drawScrollingBanner() {
    std::string bannerText = "Happy birthday to you! ";
    std::string fullBanner = bannerText + bannerText + bannerText + bannerText; // 4 TIMES AS THE BIRTHDAY SONG
    setColor(1.0f, 1.0f, 1.0f); 
    glRasterPos2f(bannerOffsetX, 0.8f); // set location
    for (char c : fullBanner) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}
// render
void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);

    if (!bulbClicked) {
        switch (page) {
        case 0:
            drawBackground(false, false);
            drawFontCover();
            drawArrows(false);
            drawTitle();
            drawGuide();
            break;
        case 1:
            drawBackground(true, false);
            drawBackground(false, false);
            drawBackground(true, true);
            drawBackground(false, true);
            drawCrease();
            drawArrows(true);

            drawCake();
            drawFireworksTube(-0.4f, -0.2f, 0.12f, 0.2f, -220.0f, leftHovered);
            drawFireworksTube(0.4f, -0.2f, 0.12f, 0.2f, 220.0f, rightHovered);
            for (auto& stripe : stripes) {
                drawStripe(stripe);
            }

            drawBulb(bulbHovered);
            break;
        }
    }
    else {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        drawEllipse(0.0f, 0.14f, 0.03f, 0.09f, 0.8f, 0.0f, 0.0f); 
        drawEllipse(0.0f, 0.086f, 0.017f, 0.046f, 0.9f, 0.9f, 0.2f); 

        drawButton("Blow out the candle", 0.0f, -0.8f, 0.8f, 0.2f);
        drawScrollingBanner();
    }

    glPopMatrix();
    glutSwapBuffers();
}

// logic
void updateBanner(int value) {
    if (bannerActive) {
        bannerOffsetX -= bannerSpeed;
        if (bannerOffsetX < -4.0f) {
            bannerOffsetX = 1.0f;
        }
        glutPostRedisplay();
        glutTimerFunc(16, updateBanner, 0); // 60 Hz
    }
}

void createStripes(float startX, float startY, bool isLeft) {
    stripes.clear();
    int numStripes = rand() % 10 + 5; // generate 5-15 strips
    for (int i = 0; i < numStripes; ++i) {
        FireworkStripe stripe;
        stripe.x = startX + static_cast<float>(rand() % 20 - 10) / 100.0f; 
        stripe.y = startY;
        stripe.speed = static_cast<float>(rand() % 3 + 1) / 40000.0f; //speed
        stripe.angle = (isLeft ? static_cast<float>(rand() % 30) - 60.0f : -static_cast<float>(rand() % 30)) * M_PI / 180.0f + 90.0f; // the right and left tube output angle
        stripe.alpha = 1.0f; //transparency
        stripe.color[0] = static_cast<float>(rand() % 100) / 100.0f;
        stripe.color[1] = static_cast<float>(rand() % 100) / 100.0f;
        stripe.color[2] = static_cast<float>(rand() % 100) / 100.0f;
        stripes.push_back(stripe);
    }
}

void updateStripes() {
    for (auto it = stripes.begin(); it != stripes.end();) {
        it->x += it->speed * cos(it->angle); 
        it->y += it->speed * sin(it->angle);
        it->alpha -= 0.0001f; // - transparency speed
        if (it->alpha <= 0.0f) {
            it = stripes.erase(it); // delete if transparency < H
        }
        else {
            ++it;
        }
    }
    glutPostRedisplay();
}

void mouseMove(int x, int y) { // check loction 
    int win_width = glutGet(GLUT_WINDOW_WIDTH);
    int win_height = glutGet(GLUT_WINDOW_HEIGHT);
    float normalizedX = (float)(x - win_width / 2) / (win_width / 2);
    float normalizedY = (float)(win_height / 2 - y) / (win_height / 2);

    // check left/ right arrow
    leftHovered = (normalizedX >= -0.46f && normalizedX <= -0.34f && normalizedY >= -0.3f && normalizedY <= -0.1f);
    rightHovered = (normalizedX >= 0.34f && normalizedX <= 0.46f && normalizedY >= -0.3f && normalizedY <= -0.1f);
    if (normalizedX >= -0.92f && normalizedX <= -0.83f && normalizedY >= -0.87f && normalizedY <= -0.73f) {
        hoveredArrow = LEFT;
    }
    else if (normalizedX >= 0.83f && normalizedX <= 0.92f && normalizedY >= -0.87f && normalizedY <= -0.73f) {
        hoveredArrow = RIGHT;
    }
    else {
        hoveredArrow = NONE;
    }

    // check bulb
    float bulbCenterX = 0.8f;
    float bulbCenterY = 0.7f;
    float bulbRadius = 0.15f;
    float distance = sqrt(pow(normalizedX - bulbCenterX, 2) + pow(normalizedY - bulbCenterY, 2));
    bulbHovered = (distance <= bulbRadius);

    // check button
    float buttonX = 0.0f;
    float buttonY = -0.8f;
    float buttonWidth = 0.8f;
    float buttonHeight = 0.2f;
    if (normalizedX >= (buttonX - buttonWidth / 2) && normalizedX <= (buttonX + buttonWidth / 2) &&
        normalizedY >= (buttonY - buttonHeight / 2) && normalizedY <= (buttonY + buttonHeight / 2)) {
        buttonHovered = true;
    }
    else {
        buttonHovered = false;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {// check click
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {
        int win_width = glutGet(GLUT_WINDOW_WIDTH);
        int win_height = glutGet(GLUT_WINDOW_HEIGHT);
        float normalizedX = static_cast<float>(x - win_width / 2) / (win_width / 2);
        float normalizedY = static_cast<float>(win_height / 2 - y) / (win_height / 2);

        // if click arrow -->update page
        if (normalizedX >= -0.92f && normalizedX <= -0.83f && normalizedY >= -0.87f && normalizedY <= -0.73f) {
            page = 0; 
        }
        else if (normalizedX >= 0.83f && normalizedX <= 0.92f && normalizedY >= -0.87f && normalizedY <= -0.73f) {
            page = 1; 
        }
        // if click fireworktube ---> create strips
        if (leftHovered) {
            createStripes(-0.4f, -0.1f, true); 
        }
        if (rightHovered) {
            createStripes(0.4f, -0.1f, false); 
        }
        // if click bulb --> change black
        float bulbCenterX = 0.8f;
        float bulbCenterY = 0.7f;
        float bulbRadius = 0.15f;
        float distance = sqrt(pow(normalizedX - bulbCenterX, 2) + pow(normalizedY - bulbCenterY, 2));
        if (distance <= bulbRadius) {
            bulbClicked = true; 
            bannerActive = true;
            bannerOffsetX = 1.0f; 
            glutTimerFunc(0, updateBanner, 0); 
        }
        // if click button ---> recover page 1
        if (bulbClicked) {
            float buttonX = 0.0f;
            float buttonY = -0.8f;
            float buttonWidth = 0.8f;
            float buttonHeight = 0.2f;
            if (normalizedX >= (buttonX - buttonWidth / 2) && normalizedX <= (buttonX + buttonWidth / 2) &&
                normalizedY >= (buttonY - buttonHeight / 2) && normalizedY <= (buttonY + buttonHeight / 2)) {
                bulbClicked = false;   
                glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
                page = 1;              
                bannerActive = false;  
                bannerOffsetX = 1.0f;  
                stripes.clear();       
            }
        }
        glutPostRedisplay(); 
    }
}

// keyboard for ESC
void handleKeypress(unsigned char key, int x, int y) {
    if (key == 27) { // ASCII Code for ESC
        exit(0);
    }
}

void initWindow() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // white bakground
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0); // set o-xy
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 500);
    glutCreateWindow("Birthday Card");

    initWindow();
    glutDisplayFunc(renderScene);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(mouseMove);
    glutIdleFunc(updateStripes);
    glutKeyboardFunc(handleKeypress); 

    glutMainLoop();
    return 0;
}




