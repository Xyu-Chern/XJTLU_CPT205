//#include <GL/glut.h>
//#include <cmath>
//
//// 角度、位置、方向控制
//float cameraAngle = 0.0f;  // 相机旋转角度
//float playerAngle = 0.0f;  // 玩家旋转角度
//float playerPos[3] = { 0.0f, 0.0f, 0.0f };  // 玩家位置
//float speed = 0.1f;  // 玩家移动速度
//int lastMouseX = 0;  // 上次鼠标的X坐标
//
//// 用于捕捉鼠标移动的函数
//void mouseMotion(int x, int y) {
//    int deltaX = x - lastMouseX;
//    lastMouseX = x;
//
//    // 当按住W键时，调整人物旋转
//    if (isWPressed) {
//        playerAngle += deltaX * 0.1f;  // 根据鼠标横向移动改变角度
//    }
//}
//
//// 处理键盘输入的函数
//void keyboard(unsigned char key, int x, int y) {
//    switch (key) {
//    case 'w':
//        isWPressed = true;
//        break;
//    case 's':
//        isSPressed = true;
//        break;
//    case 'a':
//        isAPressed = true;
//        break;
//    case 'd':
//        isDPressed = true;
//        break;
//    }
//}
//
//void keyboardUp(unsigned char key, int x, int y) {
//    switch (key) {
//    case 'w':
//        isWPressed = false;
//        break;
//    case 's':
//        isSPressed = false;
//        break;
//    case 'a':
//        isAPressed = false;
//        break;
//    case 'd':
//        isDPressed = false;
//        break;
//    }
//}
//
//// 更新人物和相机的位置
//void update() {
//    if (isWPressed) {
//        playerPos[0] += cos(playerAngle) * speed;  // 前进
//        playerPos[2] += sin(playerAngle) * speed;
//    }
//    if (isSPressed) {
//        playerPos[0] -= cos(playerAngle) * speed;  // 后退
//        playerPos[2] -= sin(playerAngle) * speed;
//    }
//    if (isAPressed) {
//        playerPos[0] -= cos(playerAngle + M_PI / 2) * speed;  // 左移
//        playerPos[2] -= sin(playerAngle + M_PI / 2) * speed;
//    }
//    if (isDPressed) {
//        playerPos[0] += cos(playerAngle + M_PI / 2) * speed;  // 右移
//        playerPos[2] += sin(playerAngle + M_PI / 2) * speed;
//    }
//
//    // 更新相机位置，使其跟随人物
//    cameraAngle = playerAngle;  // 保持相机与人物相同的旋转角度
//    // 相机位置（假设相机在人物后面一定的距离）
//    cameraPos[0] = playerPos[0] - cos(cameraAngle) * 5.0f;
//    cameraPos[2] = playerPos[2] - sin(cameraAngle) * 5.0f;
//}
//
//// 渲染函数
//void render() {
//    // 更新人物和相机位置
//    update();
//
//    // 设定相机位置
//    glLoadIdentity();
//    gluLookAt(cameraPos[0], 3.0f, cameraPos[2], playerPos[0], 1.0f, playerPos[2], 0.0f, 1.0f, 0.0f);
//
//    // 绘制人物（此处省略人物绘制代码）
//    drawPlayer();
//}
//
//// 主函数
//int main(int argc, char** argv) {
//    glutInit(&argc, argv);
//    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
//    glutCreateWindow("Player and Camera Control");
//
//    // 设置鼠标和键盘回调
//    glutMotionFunc(mouseMotion);
//    glutKeyboardFunc(keyboard);
//    glutKeyboardUpFunc(keyboardUp);
//    glutDisplayFunc(render);
//    glutIdleFunc(glutPostRedisplay);
//
//    // 初始化OpenGL设置
//    glEnable(GL_DEPTH_TEST);
//
//    // 进入主循环
//    glutMainLoop();
//
//    return 0;
//}
