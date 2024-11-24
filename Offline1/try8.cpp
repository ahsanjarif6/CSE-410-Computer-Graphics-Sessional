#include <bits/stdc++.h>
using namespace std;


#ifdef __linux__
    #include<GL/glut.h>
#elif _WIN32
    #include<windows.h>
    #include<glut.h>
#endif


int counter = 0 ;

void axes(){
    glBegin(GL_LINES);{
        glColor3f(1.0, 0.0, 0.5);
        glVertex2f(-1, 0);
        glVertex2f(1, 0);

        glVertex2f(0, 1);
        glVertex2f(0, -1);
    }glEnd();
}


void drawSquare(double a){
    glBegin(GL_QUADS);{ // Give Points in Anti-Clockwise Direction
        //glColor3f(1.0, 0.3, 0.0);
        glVertex2f(-a/2, -a/2);
        glVertex2f(a/2, -a/2);
        glVertex2f(a/2, a/2);
        glVertex2f(-a/2, a/2);
    }glEnd();

}

void display(){
    printf("We are in display function ; ");
    printf("Counter: %d\n", ++counter);
    glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer (background)

    axes(); // Draw the axes

    drawSquare(0.3);

    // glTranslatef(0.5,0.5,0);
    
    glPushMatrix();


    /*****  ORDER IS IMPORTANT HERE *****/ 
    /*** First rotate then translate ****/
    /************************************/
    
    // glTranslatef(0.5,0.5,0);
    glRotatef(counter % 360,0,0,1); // Rotate about z-axis(0,0,1) by 45 degrees
    glTranslatef(0.5,0.5,0);

    glColor3f(0,0.3,0.5);
    drawSquare(0.3);

    glPopMatrix();
     





    glFlush(); // Render now
}

void initialize(){
    glClearColor(0.0, 0.0, 0.0, 1.0); // Set background color to black and opaque
}


// void idle(){
//     printf("Currently we don't have anything to do!!!.\n");

//     glutPostRedisplay();
// }

void Timer(int value){
    glutPostRedisplay();
    glutTimerFunc(100, Timer, 0);
}

int main(int argc, char** argv){
    printf("Hello World\n");
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(600, 480);


    glutCreateWindow("OpenGL Setup Test");
    initialize();

    glutDisplayFunc(display);

    // glutIdleFunc(idle);
    Timer(0);
    glutMainLoop();
    return 0;
}