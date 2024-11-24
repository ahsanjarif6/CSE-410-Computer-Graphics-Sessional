#include <bits/stdc++.h>
using namespace std;


#ifdef __linux__
    #include<GL/glut.h>
#elif _WIN32
    #include<windows.h>
    #include<glut.h>
#endif


int counter = 0 ;


void display(){
    printf("We are in display function ; ");
    printf("Counter: %d\n", ++counter);

    glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer (background)



    glBegin(GL_LINES);{
        glColor3f(1.0, 0.0, 0.5);
        glVertex2f(-1, 0);
        // glVertex2f(1, 0);

        glVertex2f(0, 1);
        glVertex2f(0, -1);
        glVertex2f(1, 0);
    }glEnd();







    glFlush(); // Render now
}

void initialize(){
    glClearColor(0.0, 0.0, 0.0, 1.0); // Set background color to black and opaque
}

int main(int argc, char** argv){
    printf("Hello World\n");
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(600, 480);


    glutCreateWindow("OpenGL Setup Test");
    initialize();

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}