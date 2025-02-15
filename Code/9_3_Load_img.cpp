//lab 9.3

//#define FREEGLUT_STATIC
//#include <GL/freeglut.h>
//#include <stdlib.h>
//#include <stdio.h>
//
//// 全局变量，用于存储图像的信息
//static GLint imagewidth;       // 图像的宽度
//static GLint imageheight;      // 图像的高度
//static GLint pixellength;      // 图像像素数据的总长度（包括对齐填充）
//static GLubyte* pixeldata;     // 指向图像像素数据的指针

// 初始化函数，负责读取图像文件并加载图像数据
//void myinit(void) {
//    // 打开图像文件
//    FILE* pfile = NULL; // 定义文件指针
//    fopen_s(&pfile, "textures/img2.bmp", "rb"); // 以二进制模式打开文件（路径：textures/img2.bmp）
//    if (pfile == 0) exit(0); // 如果文件无法打开，程序退出
//
//    // 获取图像的宽度和高度
//    fseek(pfile, 0x0012, SEEK_SET); // 跳过18个字节到达宽度信息的位置
//    fread(&imagewidth, sizeof(imagewidth), 1, pfile); // 读取图像宽度
//    fread(&imageheight, sizeof(imageheight), 1, pfile); // 读取图像高度
//
//    // 计算像素数据的长度（每像素3字节：BGR格式）
//    pixellength = imagewidth * 3; // 宽度乘以3，计算每行像素的总字节数
//    while (pixellength % 4 != 0)  // 如果总字节数不是4的倍数，补齐对齐字节
//        pixellength++;
//    pixellength *= imageheight; // 再乘以高度，计算总像素数据的大小
//
//    // 为像素数据分配内存
//    pixeldata = (GLubyte*)malloc(pixellength); // 分配内存
//    if (pixeldata == 0) exit(0); // 如果内存分配失败，程序退出
//
//    // 读取像素数据
//    fseek(pfile, 54, SEEK_SET); // 跳过BMP文件头（54字节）
//    fread(pixeldata, pixellength, 1, pfile); // 读取像素数据到pixeldata中
//
//    // 关闭文件
//    fclose(pfile);
//}

// 显示函数，用于绘制图像
//void display(void) {
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除颜色和深度缓冲
//    // 将像素数据绘制到帧缓冲区
//    glDrawPixels(imagewidth, imageheight, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixeldata);
//    glFlush(); // 强制执行所有的OpenGL命令
//}
//
//// 窗口大小改变时的回调函数
//void myReshape(GLsizei w, GLsizei h) {
//    glViewport(0, 0, w, h); // 设置视口大小为窗口大小
//    glMatrixMode(GL_PROJECTION); // 设置当前矩阵为投影矩阵
//    glLoadIdentity(); // 重置投影矩阵
//    // 设置透视投影，视角为60度，宽高比根据窗口计算，近裁剪面为1，远裁剪面为30000
//    gluPerspective(60.0, 1.0 * (GLfloat)w / (GLfloat)h, 1.0, 30000.0);
//    glMatrixMode(GL_MODELVIEW); // 切换回模型视图矩阵
//    glLoadIdentity(); // 重置模型视图矩阵
//}
//
//// 主函数，程序入口
//void main(int argc, char** argv) {
//    glutInit(&argc, argv); // 初始化GLUT库
//    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH); // 设置显示模式：单缓冲、RGBA颜色模式、深度缓冲
//    myinit(); // 调用初始化函数，读取图像文件
//    glutInitWindowSize(imagewidth, imageheight); // 设置窗口大小为图像大小
//    glutCreateWindow("Loading Image by Pixel"); // 创建一个窗口，标题为"Loading Image by Pixel"
//    glutReshapeFunc(myReshape); // 注册窗口大小改变的回调函数
//    glutDisplayFunc(display); // 注册显示的回调函数
//    glutMainLoop(); // 进入GLUT事件处理循环
//}
