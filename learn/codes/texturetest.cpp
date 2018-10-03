#define WindowWidth   400
#define WindowHeight 400
#define WindowTitle  "OpenGL纹理测试"

#include <gl/glut.h>
#include <stdio.h>
#include <stdlib.h>

/* 函数grab
* 抓取窗口中的像素
* 假设窗口宽度为WindowWidth，高度为WindowHeight
*/
#define BMP_Header_Length 54
void grab(void)
{
     FILE*     pDummyFile;
     FILE*     pWritingFile;
     GLubyte* pPixelData;
     GLubyte   BMP_Header[BMP_Header_Length];
     GLint     i, j;
     GLint     PixelDataLength;

     // 计算像素数据的实际长度
     i = WindowWidth * 3;    // 得到每一行的像素数据长度
    while( i%4 != 0 )       // 补充数据，直到i是的倍数
         ++i;                // 本来还有更快的算法，
                            // 但这里仅追求直观，对速度没有太高要求
     PixelDataLength = i * WindowHeight;

     // 分配内存和打开文件
     pPixelData = (GLubyte*)malloc(PixelDataLength);
    if( pPixelData == 0 )
        exit(0);

     pDummyFile = fopen("dummy.bmp", "rb");
    if( pDummyFile == 0 )
        exit(0);

     pWritingFile = fopen("grab.bmp", "wb");
    if( pWritingFile == 0 )
        exit(0);

     // 读取像素
     glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
     glReadPixels(0, 0, WindowWidth, WindowHeight,
         GL_BGR_EXT, GL_UNSIGNED_BYTE, pPixelData);

     // 把dummy.bmp的文件头复制为新文件的文件头
    fread(BMP_Header, sizeof(BMP_Header), 1, pDummyFile);
    fwrite(BMP_Header, sizeof(BMP_Header), 1, pWritingFile);
    fseek(pWritingFile, 0x0012, SEEK_SET);
     i = WindowWidth;
     j = WindowHeight;
    fwrite(&i, sizeof(i), 1, pWritingFile);
    fwrite(&j, sizeof(j), 1, pWritingFile);

     // 写入像素数据
    fseek(pWritingFile, 0, SEEK_END);
    fwrite(pPixelData, PixelDataLength, 1, pWritingFile);

     // 释放内存和关闭文件
    fclose(pDummyFile);
    fclose(pWritingFile);
    free(pPixelData);
}
/* 函数power_of_two
* 检查一个整数是否为2的整数次方，如果是，返回1，否则返回0
* 实际上只要查看其二进制位中有多少个，如果正好有1个，返回1，否则返回0
* 在“查看其二进制位中有多少个”时使用了一个小技巧
* 使用n &= (n-1)可以使得n中的减少一个（具体原理大家可以自己思考）
*/
int power_of_two(int n)
{
    if( n <= 0 )
        return 0;
    return (n & (n-1)) == 0;
}

/* 函数load_texture
* 读取一个BMP文件作为纹理
* 如果失败，返回0，如果成功，返回纹理编号
*/
GLuint load_texture(const char* file_name)
{
     GLint width, height, total_bytes;
     GLubyte* pixels = 0;
     GLuint last_texture_ID, texture_ID = 0;

     // 打开文件，如果失败，返回
     FILE* pFile = fopen(file_name, "rb");
    if( pFile == 0 )
        return 0;

     // 读取文件中图象的宽度和高度
    fseek(pFile, 0x0012, SEEK_SET);
    fread(&width, 4, 1, pFile);
    fread(&height, 4, 1, pFile);
    fseek(pFile, BMP_Header_Length, SEEK_SET);

     // 计算每行像素所占字节数，并根据此数据计算总像素字节数
     {
         GLint line_bytes = width * 3;
        while( line_bytes % 4 != 0 )
             ++line_bytes;
         total_bytes = line_bytes * height;
     }

     // 根据总像素字节数分配内存
     pixels = (GLubyte*)malloc(total_bytes);
    if( pixels == 0 )
     {
        fclose(pFile);
        return 0;
     }

     // 读取像素数据
    if( fread(pixels, total_bytes, 1, pFile) <= 0 )
     {
        free(pixels);
        fclose(pFile);
        return 0;
     }

     // 在旧版本的OpenGL中
     // 如果图象的宽度和高度不是的整数次方，则需要进行缩放
     // 这里并没有检查OpenGL版本，出于对版本兼容性的考虑，按旧版本处理
     // 另外，无论是旧版本还是新版本，
     // 当图象的宽度和高度超过当前OpenGL实现所支持的最大值时，也要进行缩放
     {
         GLint max;
         glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
        if( !power_of_two(width)
          || !power_of_two(height)
          || width > max
          || height > max )
         {
            const GLint new_width = 256;
            const GLint new_height = 256; // 规定缩放后新的大小为边长的正方形
             GLint new_line_bytes, new_total_bytes;
             GLubyte* new_pixels = 0;

             // 计算每行需要的字节数和总字节数
             new_line_bytes = new_width * 3;
            while( new_line_bytes % 4 != 0 )
                 ++new_line_bytes;
             new_total_bytes = new_line_bytes * new_height;

             // 分配内存
             new_pixels = (GLubyte*)malloc(new_total_bytes);
            if( new_pixels == 0 )
             {
                free(pixels);
                fclose(pFile);
                return 0;
             }

             // 进行像素缩放
             gluScaleImage(GL_RGB,
                 width, height, GL_UNSIGNED_BYTE, pixels,
                 new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

             // 释放原来的像素数据，把pixels指向新的像素数据，并重新设置width和height
            free(pixels);
             pixels = new_pixels;
             width = new_width;
             height = new_height;
         }
     }

     // 分配一个新的纹理编号
     glGenTextures(1, &texture_ID);
    if( texture_ID == 0 )
     {
        free(pixels);
        fclose(pFile);
        return 0;
     }

     // 绑定新的纹理，载入纹理并设置纹理参数
     // 在绑定前，先获得原来绑定的纹理编号，以便在最后进行恢复
     glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture_ID);
     glBindTexture(GL_TEXTURE_2D, texture_ID);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
     glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
         GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
     glBindTexture(GL_TEXTURE_2D, last_texture_ID);

     // 之前为pixels分配的内存可在使用glTexImage2D以后释放
     // 因为此时像素数据已经被OpenGL另行保存了一份（可能被保存到专门的图形硬件中）
    free(pixels);
    return texture_ID;
}
/* 两个纹理对象的编号
*/
GLuint texGround;
GLuint texWall;

void display(void)
{
     // 清除屏幕
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

     // 设置视角
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     gluPerspective(75, 1, 1, 21);
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
     gluLookAt(1, 5, 5, 0, 0, 0, 0, 0, 1);

     // 使用“地”纹理绘制土地
     glBindTexture(GL_TEXTURE_2D, texGround);
     glBegin(GL_QUADS);
         glTexCoord2f(0.0f, 0.0f); glVertex3f(-8.0f, -8.0f, 0.0f);
         glTexCoord2f(0.0f, 5.0f); glVertex3f(-8.0f, 8.0f, 0.0f);
         glTexCoord2f(5.0f, 5.0f); glVertex3f(8.0f, 8.0f, 0.0f);
         glTexCoord2f(5.0f, 0.0f); glVertex3f(8.0f, -8.0f, 0.0f);
     glEnd();
     // 使用“墙”纹理绘制栅栏
     glBindTexture(GL_TEXTURE_2D, texWall);
     glBegin(GL_QUADS);
         glTexCoord2f(0.0f, 0.0f); glVertex3f(-6.0f, -3.0f, 0.0f);
         glTexCoord2f(0.0f, 1.0f); glVertex3f(-6.0f, -3.0f, 1.5f);
         glTexCoord2f(5.0f, 1.0f); glVertex3f(6.0f, -3.0f, 1.5f);
         glTexCoord2f(5.0f, 0.0f); glVertex3f(6.0f, -3.0f, 0.0f);
     glEnd();

     // 旋转后再绘制一个
     glRotatef(-90, 0, 0, 1);
     glBegin(GL_QUADS);
         glTexCoord2f(0.0f, 0.0f); glVertex3f(-6.0f, -3.0f, 0.0f);
         glTexCoord2f(0.0f, 1.0f); glVertex3f(-6.0f, -3.0f, 1.5f);
         glTexCoord2f(5.0f, 1.0f); glVertex3f(6.0f, -3.0f, 1.5f);
         glTexCoord2f(5.0f, 0.0f); glVertex3f(6.0f, -3.0f, 0.0f);
     glEnd();

     // 交换缓冲区，并保存像素数据到文件
     glutSwapBuffers();
     grab();
}

int main(int argc, char* argv[])
{
     // GLUT初始化
     glutInit(&argc, argv);
     glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
     glutInitWindowPosition(100, 100);
     glutInitWindowSize(WindowWidth, WindowHeight);
     glutCreateWindow(WindowTitle);
     glutDisplayFunc(&display);

     // 在这里做一些初始化
     glEnable(GL_DEPTH_TEST);
     glEnable(GL_TEXTURE_2D);
     texGround = load_texture("ground.bmp");
     texWall = load_texture("wall.bmp");

     // 开始显示
     glutMainLoop();

    return 0;
}