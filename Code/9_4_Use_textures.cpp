//
// lab 9.4


//#define FREEGLUT_STATIC
//#include <GL/freeglut.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include "vector"
//
//using namespace std;
//
//// 全局变量定义
//GLint imagewidth0, imagewidth1; // 两个图像的宽度
//GLint imageheight0, imageheight1; // 两个图像的高度
//GLint pixellength0, pixellength1; // 两个图像的像素数据长度
//vector<GLubyte*> p; // 存储像素数据的指针数组，用于加载多个纹理
//GLuint texture[2]; // 用于存储两个纹理的ID
//GLfloat angle = 0; // 球体旋转角度
//
//// 读取BMP图像数据函数
//void ReadImage(const char path[256], GLint& imagewidth, GLint& imageheight, GLint& pixellength) {
//    GLubyte* pixeldata; // 指向图像像素数据的指针
//    FILE* pfile; // 文件指针
//
//    // 打开BMP文件
//    fopen_s(&pfile, path, "rb");
//    if (pfile == 0) exit(0); // 如果文件打开失败，退出程序
//
//    // 读取BMP文件的宽度和高度
//    fseek(pfile, 0x0012, SEEK_SET);
//    fread(&imagewidth, sizeof(imagewidth), 1, pfile);
//    fread(&imageheight, sizeof(imageheight), 1, pfile);
//
//    // 计算像素数据的长度（考虑4字节对齐）
//    pixellength = imagewidth * 3; // 每个像素3字节（RGB）
//    while (pixellength % 4 != 0) pixellength++; // 补齐4字节对齐
//    pixellength *= imageheight; // 乘以高度得到总大小
//
//    // 为像素数据分配内存
//    pixeldata = (GLubyte*)malloc(pixellength);
//    if (pixeldata == 0) exit(0); // 如果内存分配失败，退出程序
//
//    // 读取像素数据（跳过BMP文件头）
//    fseek(pfile, 54, SEEK_SET);
//    fread(pixeldata, pixellength, 1, pfile);
//
//    // 将像素数据添加到向量中
//    p.push_back(pixeldata);
//    fclose(pfile); // 关闭文件
//}

// 初始化OpenGL函数
//void myinit(void) {
//    glClearColor(0.0, 0.0, 0.0, 0.0); // 设置清除颜色（黑色）
//    glEnable(GL_DEPTH_TEST); // 启用深度测试
//    glDepthFunc(GL_LEQUAL); // 设置深度测试函数
//    glShadeModel(GL_FLAT); // 使用平面着色
//    glEnable(GL_TEXTURE_2D); // 启用2D纹理
//
//    // 加载两个BMP图片
//    ReadImage("textures/img1.bmp", imagewidth0, imageheight0, pixellength0);
//    ReadImage("textures/img1.bmp", imagewidth1, imageheight1, pixellength1);
//
//    // 设置像素存储模式（字节对齐方式）
//    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//
//    // 生成纹理对象
//    glGenTextures(2, &texture[0]);
//
//    // 设置第一个纹理
//    glBindTexture(GL_TEXTURE_2D, texture[0]);
//    glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth0, imageheight0, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[0]);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//
//    // 设置第二个纹理
//    glBindTexture(GL_TEXTURE_2D, texture[1]);
//    glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth1, imageheight1, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[1]);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//
//    // 设置纹理环境模式
//    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
//}
//
//// 显示函数
//void display(void) {
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除颜色和深度缓冲
//    glLoadIdentity(); // 重置模型视图矩阵
//    glTranslatef(0.0, 0.0, -4.0); // 将场景沿Z轴移动
//
//    // 绘制第一个纹理的矩形
//    glBindTexture(GL_TEXTURE_2D, texture[0]);
//    glBegin(GL_QUADS);
//    glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, 0.0);
//    glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 1.0, 0.0);
//    glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 1.0, 0.0);
//    glTexCoord2f(1.0, 0.0); glVertex3f(0.0, -1.0, 0.0);
//    glEnd();
//
//    // 绘制第二个纹理的旋转球体
//    glTranslatef(1.0, 0.0, 0.0); // 移动到新位置
//    glRotatef(angle, 0.0, 1.0, 0.0); // 绕Y轴旋转
//    glBindTexture(GL_TEXTURE_2D, texture[1]);
//    GLUquadric* quadricObj = gluNewQuadric(); // 创建球体对象
//    gluQuadricTexture(quadricObj, GL_TRUE); // 启用纹理映射
//    gluSphere(quadricObj, 0.8, 80, 80); // 绘制球体
//    gluDeleteQuadric(quadricObj); // 删除球体对象，释放内存
//
//    glFlush(); // 执行绘制
//}

// 键盘输入处理函数
//void keyboard_input(unsigned char key, int x, int y) {
//    if (key == 'r' || key == 'R') { // 按下‘r’键时，增加旋转角度
//        if (angle < 360) {
//            angle = angle + 30;
//        }
//        else {
//            angle = 0; // 重置角度
//        }
//    }
//    glutPostRedisplay(); // 触发重新绘制
//}
// 窗口大小改变时的回调函数
//void myReshape(GLsizei w, GLsizei h) {
//    glViewport(0, 0, w, h); // 设置视口大小
//    glMatrixMode(GL_PROJECTION); // 切换到投影矩阵
//    glLoadIdentity(); // 重置投影矩阵
//    gluPerspective(60.0, 1.0 * (GLfloat)w / (GLfloat)h, 0.0, 300.0); // 设置透视投影
//    glMatrixMode(GL_MODELVIEW); // 切换回模型视图矩阵
//    glLoadIdentity(); // 重置模型视图矩阵
//}

// 主函数
//int main(int argc, char** argv) {
//    glutInit(&argc, argv); // 初始化GLUT
//    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH); // 设置显示模式
//    glutInitWindowPosition(0, 0); // 设置窗口位置
//    glutInitWindowSize(500, 500); // 设置窗口大小
//    if (!glutCreateWindow("Texture Mapping")) // 创建窗口
//        exit(0);
//
//    myinit(); // 初始化OpenGL
//    glutKeyboardFunc(keyboard_input); // 注册键盘输入回调函数
//    glutReshapeFunc(myReshape); // 注册窗口大小改变回调函数
//    glutDisplayFunc(display); // 注册显示回调函数
//    glutMainLoop(); // 进入GLUT事件处理循环
//}
