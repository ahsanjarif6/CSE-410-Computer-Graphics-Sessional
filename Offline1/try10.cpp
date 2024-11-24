#include <bits/stdc++.h>
using namespace std;


#ifdef __linux__
    #include<GL/glut.h>
#elif _WIN32
    #include<windows.h>
    #include<glut.h>
#endif


int counter = 0 ;
int animate = 0 ;


void axes(){
    glBegin(GL_LINES);{
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(0 , 0 , 0);
        glVertex3f(500, 0 , 0);

        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(0 , 0 , 0);
        glVertex3f(0, 500 , 0);

        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(0 , 0 , 0);
        glVertex3f(0, 0 , 500);
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
float angle = 0;

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0,0.0,-8.0);
    glRotatef(angle,1,1,1);
    axes();
    glBegin(GL_QUADS);


    //front
    glColor3f(1.0,0.0,0.0);
    glVertex3f(-1.0,1.0,1.0);
    glVertex3f(-1.0,-1.0,1.0);
    glVertex3f(1.0,-1.0,1.0);
    glVertex3f(1.0,1.0,1.0);
    //back
    glColor3f(0.0,1.0,0.0);
    glVertex3f(1.0,1.0,-1.0);
    glVertex3f(1.0,-1.0,-1.0);
    glVertex3f(-1.0,-1.0,-1.0);
    glVertex3f(-1.0,1.0,-1.0);
    //right
    glColor3f(0.0,0.0,1.0);
    glVertex3f(1.0,1.0,1.0);
    glVertex3f(1.0,-1.0,1.0);
    glVertex3f(1.0,-1.0,-1.0);
    glVertex3f(1.0,1.0,-1.0);
    //left
    glColor3f(1.0,1.0,0.0);
    glVertex3f(-1.0,1.0,-1.0);
    glVertex3f(-1.0,-1.0,-1.0);
    glVertex3f(-1.0,-1.0,1.0);
    glVertex3f(-1.0,1.0,1.0);
    //top
    glColor3f(0.0,1.0,1.0);
    glVertex3f(-1.0,1.0,-1.0);
    glVertex3f(-1.0,1.0,1.0);
    glVertex3f(1.0,1.0,1.0);
    glVertex3f(1.0,1.0,-1.0);
    //bottom
    glColor3f(1.0,0.0,1.0);
    glVertex3f(-1.0,-1.0,-1.0);
    glVertex3f(-1.0,-1.0,1.0);
    glVertex3f(1.0,-1.0,1.0);
    glVertex3f(1.0,-1.0,-1.0);

    glEnd();
    glutSwapBuffers();
    //glFlush();
}

void initialize(){
    // animate = 1 ;
    // printf("We are in initialize function\n");
    // printf("Animate : %d\n", animate);
    glClearColor(0.0, 0.0, 0.0, 1.0); // Set background color to black and opaque
    glEnable(GL_DEPTH_TEST);
}

void reshape(int w , int h){
    glViewport(0,0,(GLsizei)w , (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,1,2.0,50.0);
    glMatrixMode(GL_MODELVIEW);
}

// void idle(){
//     printf("Currently we don't have anything to do!!!.\n");

//     glutPostRedisplay();
// }

void Timer(int value){

    // if(animate){
    //     printf("Inside if(Animate)");
    //     printf("Counter: %d\n", counter);

    //     counter++;
    // }
    
    glutPostRedisplay();
    glutTimerFunc(1000/60, Timer, 0);
    angle += 0.8;
    if(angle > 360.0){
        angle -= 360.0;
    }
}


// void keyboard_Handler(unsigned char key, int x, int y){
//     switch(key){
//         case 'V':
//             printf("animate: %d\n", animate);
//             printf("V Pressed\n");
            
//             animate = 1 - animate ;
//             printf("animate: %d\n", animate);
//             break;
//         case 'v':
//             printf("v Pressed\n");
//             animate = 1-animate ;
//             break;
//         default:
//             printf("Unknown Key Pressed\n");
//             break;
//     }
// }

int main(int argc, char** argv){
    printf("Hello World\n");
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(600, 600);


    glutCreateWindow("OpenGL Setup Test");
    initialize();

    glutDisplayFunc(display);
    glutTimerFunc(0, Timer, 0);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}