//#define FREEGLUT_STATIC
//#include <GL/freeglut.h>
//#include <stdlib.h>
//#include <stdio.h>
//
//// 定义Checkerboard纹理的宽度和高度
//#define checkImageWidth 64
//#define checkImageHeight 64
//
//// 用于存储Checkerboard纹理的数组（64x64，每个像素有3个通道：RGB）
//GLubyte checkImage[checkImageWidth][checkImageHeight][3];
//
//// 生成Checkerboard图案纹理的函数
//void makeCheckImage(void) {
//    int i, j, c;
//    for (i = 0; i < checkImageWidth; i++) {
//        for (j = 0; j < checkImageHeight; j++) {
//            // 使用位运算生成黑白方格（按位与`&`和异或`^`操作）
//            c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0)) * 255;
//            // 设置像素的RGB值，生成黑白格子
//            checkImage[i][j][0] = (GLubyte)c; // 红色通道
//            checkImage[i][j][1] = (GLubyte)c; // 绿色通道
//            checkImage[i][j][2] = (GLubyte)c; // 蓝色通道
//        }
//    }
//}
//
//// OpenGL初始化函数，配置纹理和绘图环境
//void myinit(void) {
//    // 设置清除颜色（背景颜色为蓝色）
//    glClearColor(0.0, 0.0, 1.0, 0.0);
//
//    // 启用深度测试，用于处理3D场景的遮挡关系
//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LEQUAL); // 深度测试函数：允许小于等于当前深度的像素通过测试
//
//    // 生成Checkerboard纹理
//    makeCheckImage();
//
//    // 设置像素存储方式（对齐方式为1字节）
//    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//
//    // 指定2D纹理的图像数据
//    glTexImage2D(GL_TEXTURE_2D, 0, 3, checkImageWidth, checkImageHeight, 0,
//        GL_RGB, GL_UNSIGNED_BYTE, &checkImage[0][0][0]);
//
//    // 设置纹理的参数
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); // 水平方向的纹理坐标超出范围时的处理
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); // 垂直方向的纹理坐标超出范围时的处理
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // 放大纹理时使用最邻近点
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // 缩小纹理时使用最邻近点
//
//    // 设置纹理环境模式为GL_DECAL（覆盖模式）
//    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
//
//    // 启用纹理映射
//    glEnable(GL_TEXTURE_2D);
//
//    // 设置平面着色模型
//    glShadeModel(GL_FLAT);
//}
//
//// 显示函数，负责绘制场景
//void display(void) {
//    // 清空颜色缓冲区和深度缓冲区
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    // 开始绘制第一个矩形（正面朝向视角）
//    glBegin(GL_QUADS);
//    glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, 0.0); // 左下角
//    glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 1.0, 0.0); // 左上角
//    glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 1.0, 0.0); // 右上角
//    glTexCoord2f(1.0, 0.0); glVertex3f(0.0, -1.0, 0.0); // 右下角
//
//    // 开始绘制第二个矩形（向后倾斜45度）
//    glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, 0.0); // 左下角
//    glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, 0.0); // 左上角
//    glTexCoord2f(1.0, 1.0); glVertex3f(2.41421, 1.0, -1.41421); // 右上角（倾斜点）
//    glTexCoord2f(1.0, 0.0); glVertex3f(2.41421, -1.0, -1.41421); // 右下角（倾斜点）
//    glEnd();
//
//    // 强制执行OpenGL命令
//    glFlush();
//}
//
//// 窗口大小调整时的回调函数
//void myReshape(GLsizei w, GLsizei h) {
//    glViewport(0, 0, w, h); // 设置视口大小为整个窗口
//
//    glMatrixMode(GL_PROJECTION); // 设置当前矩阵为投影矩阵
//    glLoadIdentity(); // 重置投影矩阵
//    gluPerspective(60.0, 1.0 * (GLfloat)w / (GLfloat)h, 1.0, 30.0); // 设置透视投影矩阵
//
//    glMatrixMode(GL_MODELVIEW); // 切换到模型视图矩阵
//    glLoadIdentity(); // 重置模型视图矩阵
//    glTranslatef(0.0, 0.0, -3.6); // 沿Z轴向后移动视角
//}
//
//// 主函数，程序入口
//void main(int argc, char** argv) {
//    glutInit(&argc, argv); // 初始化GLUT库
//    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH); // 设置显示模式（单缓冲、RGBA、深度缓冲）
//    glutInitWindowPosition(0, 0); // 设置窗口位置
//    glutInitWindowSize(500, 500); // 设置窗口大小
//    if (!glutCreateWindow("Texture Mapping")) // 创建窗口并命名为"Texture Mapping"
//        exit(0);
//
//    myinit(); // 调用初始化函数
//    glutReshapeFunc(myReshape); // 注册窗口大小调整回调函数
//    glutDisplayFunc(display); // 注册显示回调函数
//    glutMainLoop(); // 进入GLUT事件处理循环
//}
