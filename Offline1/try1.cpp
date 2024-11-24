#include <bits/stdc++.h>
using namespace std;


#ifdef __linux__
    #include<GL/glut.h>
#elif _WIN32
    #include<windows.h>
    #include<glut.h>
#endif

int main(int argc, char** argv){
    printf("Hello World\n");
    glutInit(&argc, argv);

    glutCreateWindow("OpenGL Setup Test");


    glutMainLoop();
    return 0;
}