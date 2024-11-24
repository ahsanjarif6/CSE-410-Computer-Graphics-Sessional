#include <bits/stdc++.h>
using namespace std;


#ifdef __linux__
    #include<GL/glut.h>
#elif _WIN32
    #include<windows.h>
    #include<glut.h>
#endif

#define red glColor3f(1.0, 0.0, 0.0)
#define green glColor3f(0.0, 1.0, 0.0)
#define blue glColor3f(0.0, 0.0, 1.0)
#define cyan glColor3f(0, 1, 1)
#define magenta glColor3f(1, 0, 1)
#define yellow glColor3f(1, 1, 0)
#define PI acos(-1)
int counter = 0 ;
int animate = 0 ;
float centX = 1.0 / 3.0;
float centY = 1.0 / 3.0;
float centZ = 1.0 / 3.0;
float shrinkF = 1.0;
float dist = 0;
float radius = sqrt(1.0/3.0);
vector<float> verticesXpos;
int subdivision = 4;
float cheight , cradius;

std::vector<float> buildUnitPositiveX(int subdivision)
{
    const float DEG2RAD = acos(-1) / 180.0f;

    std::vector<float> vertices;
    float n1[3];        // normal of longitudinal plane rotating along Y-axis
    float n2[3];        // normal of latitudinal plane rotating along Z-axis
    float v[3];         // direction vector intersecting 2 planes, n1 x n2
    float a1;           // longitudinal angle along Y-axis
    float a2;           // latitudinal angle along Z-axis

    // compute the number of vertices per row, 2^n + 1
    int pointsPerRow = (int)pow(2, subdivision) + 1;

    // rotate latitudinal plane from 45 to -45 degrees along Z-axis (top-to-bottom)
    for(unsigned int i = 0; i < pointsPerRow; ++i)
    {
        // normal for latitudinal plane
        // if latitude angle is 0, then normal vector of latitude plane is n2=(0,1,0)
        // therefore, it is rotating (0,1,0) vector by latitude angle a2
        a2 = DEG2RAD * (45.0f - 90.0f * i / (pointsPerRow - 1));
        n2[0] = -sin(a2);
        n2[1] = cos(a2);
        n2[2] = 0;

        // rotate longitudinal plane from -45 to 45 along Y-axis (left-to-right)
        for(unsigned int j = 0; j < pointsPerRow; ++j)
        {
            // normal for longitudinal plane
            // if longitude angle is 0, then normal vector of longitude is n1=(0,0,-1)
            // therefore, it is rotating (0,0,-1) vector by longitude angle a1
            a1 = DEG2RAD * (-45.0f + 90.0f * j / (pointsPerRow - 1));
            n1[0] = -sin(a1);
            n1[1] = 0;
            n1[2] = -cos(a1);

            // find direction vector of intersected line, n1 x n2
            v[0] = n1[1] * n2[2] - n1[2] * n2[1];
            v[1] = n1[2] * n2[0] - n1[0] * n2[2];
            v[2] = n1[0] * n2[1] - n1[1] * n2[0];

            // normalize direction vector
            float scale = 1 / sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
            v[0] *= scale * radius;
            v[1] *= scale * radius;
            v[2] *= scale * radius;

            // add a vertex into array
            vertices.push_back(v[0]);
            vertices.push_back(v[1]);
            vertices.push_back(v[2]);
        }
    }

    return vertices;
}


struct point{
    float x , y , z;
    point(){

    }
    point(float _x , float _y , float _z){
        x = _x;
        y = _y;
        z = _z;
    }
    point cross(point b){
        float ax = x;
        float ay = y;
        float az = z;
        float bx = b.x;
        float by = b.y;
        float bz = b.z;
        return point(ay * bz - az * by , az * bx - ax * bz , ax * by - ay * bx);
    }
    void normalize(){
        float len = sqrt(x * x + y * y + z * z);
        x /= len;
        y /= len;
        z /= len;
    }
};
point current , center , fward , uward , rward;

float distance(){
    return sqrt((center.x - current.x) * (center.x - current.x) + (center.y - current.y) * (center.y - current.y) + (center.z - current.z) * (center.z - current.z));
}

point make_forward(){
    return point(center.x - current.x , center.y - current.y , center.z - current.z);
}

point make_up(){
    return rward.cross(fward);
}

point make_right(){
    return fward.cross(uward);
}

void axes(){
    glBegin(GL_LINES);{
        red;
        glVertex3f(-500 , 0 , 0);
        glVertex3f(500, 0 , 0);

        green;
        glVertex3f(0 , -500 , 0);
        glVertex3f(0, 500 , 0);

        blue;
        glVertex3f(0 , 0 , -500);
        glVertex3f(0, 0 , 500);
    }glEnd();
}

void triangle(){
    glBegin(GL_TRIANGLES);{
        glVertex3f((1.0 - centX) * shrinkF + centX, (0.0 - centY) * shrinkF + centY, (0.0 - centZ) * shrinkF + centZ);
        glVertex3f((0.0 - centX) * shrinkF + centX, (1.0 - centY) * shrinkF + centY, (0.0 - centZ) * shrinkF + centZ);
        glVertex3f((0.0 - centX) * shrinkF + centX, (0.0 - centY) * shrinkF + centY, (1.0 - centZ) * shrinkF + centZ);
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

void halfOctahedron(){
    cyan;
    triangle();
    glRotatef(90,0,1,0);
    magenta;
    triangle();
    glRotatef(90,0,1,0);
    cyan;
    triangle();
    glRotatef(90,0,1,0);
    magenta;
    triangle();
}

void octahedron(){
    halfOctahedron();
    glPushMatrix();
    glRotatef(180,1,0,0);
    glRotatef(90,0,1,0);
    halfOctahedron();
    glPopMatrix();
}

void oneSixthSphere(){
    int pointsPerRow = (1 << subdivision) + 1;
    vector< point > v;
    for(int i = 0 ; i < verticesXpos.size() ; i += 3){
        float x = verticesXpos[i];
        float y = verticesXpos[i + 1];
        float z = verticesXpos[i + 2];
        v.push_back(point(x , y , z));
    }
    glBegin(GL_QUADS);
    for(int i = 0 ; i < (int)v.size() - pointsPerRow - 1 ; i ++ ){
        point a = v[i];
        point b = v[i + 1];
        point c = v[i + pointsPerRow];
        point d = v[i + pointsPerRow + 1];
        glVertex3f(a.x , a.y , a.z);
        glVertex3f(b.x , b.y , b.z);
        glVertex3f(d.x , d.y , d.z);
        glVertex3f(c.x , c.y , c.z);
    }
    glEnd();
}

void sphere(){
    green;
    glPushMatrix();
    glTranslatef(shrinkF , 0.0 , 0.0);
    glScalef(1.0 - shrinkF , 1.0 - shrinkF , 1.0 - shrinkF);
    oneSixthSphere();
    glPopMatrix();


    red;
    glPushMatrix();
    glRotatef(90,0,1,0);
    glTranslatef(shrinkF , 0.0 , 0.0);
    glScalef(1 - shrinkF , 1 - shrinkF , 1 - shrinkF);
    oneSixthSphere();
    glPopMatrix();

    green;
    glPushMatrix();
    glRotatef(180,0,1,0);
    glTranslatef(shrinkF , 0.0 , 0.0);
    glScalef(1 - shrinkF , 1 - shrinkF , 1 - shrinkF);
    oneSixthSphere();
    glPopMatrix();

    red;
    glPushMatrix();
    glRotatef(270,0,1,0);
    glTranslatef(shrinkF , 0.0 , 0.0);
    glScalef(1 - shrinkF , 1 - shrinkF , 1 - shrinkF);
    oneSixthSphere();
    glPopMatrix();

    blue;
    glPushMatrix();
    glRotatef(90,0,0,1);
    glTranslatef(shrinkF , 0.0 , 0.0);
    glScalef(1 - shrinkF , 1 - shrinkF , 1 - shrinkF);
    oneSixthSphere();
    glPopMatrix();

    blue;
    glPushMatrix();
    glRotatef(-90,0,0,1);
    glTranslatef(shrinkF , 0.0 , 0.0);
    glScalef(1 - shrinkF , 1 - shrinkF , 1 - shrinkF);
    oneSixthSphere();
    glPopMatrix();
}

void curvedSurface(){
    float dT = 5.0;
    yellow;
    glBegin(GL_QUADS);
    for(float angle = -35.2 ; angle < 35.2 ; angle += dT){
        float a = (angle * PI) / 180.0;
        float b = ((angle + dT) * PI) / 180.0;
        glVertex3f(cradius * cos(a) , -cheight / 2 , cradius * sin(a));
        glVertex3f(cradius * cos(b) , -cheight / 2 , cradius * sin(b));
        glVertex3f(cradius * cos(b) , cheight / 2 , cradius * sin(b));
        glVertex3f(cradius * cos(a) , cheight / 2 , cradius * sin(a));
    }
    glEnd();
}

void oneCylinder(){
    cradius = radius * (1 - shrinkF);
    cheight = sqrt(2) * shrinkF;
    glPushMatrix();
    glRotated(45, 0, 0, 1);
    glTranslated(shrinkF / sqrt(2) , 0, 0);
    curvedSurface();
    glPopMatrix();
    

    glPushMatrix();
    glScaled(1,-1,1);
    glRotated(45, 0, 0, 1);
    glTranslated(shrinkF / sqrt(2), 0, 0);
    curvedSurface();
    glPopMatrix();
}

void updownCylinder(){
    glPushMatrix();
    oneCylinder();
    glPopMatrix();

    glPushMatrix();
    glRotatef(90,0,1,0);
    oneCylinder();
    glPopMatrix();

    glPushMatrix();
    glRotatef(180,0,1,0);
    oneCylinder();
    glPopMatrix();

    glPushMatrix();
    glRotatef(270,0,1,0);
    oneCylinder();
    glPopMatrix();
}

void sideCylinder(){
    glPushMatrix();
    glRotatef(90,1,0,0);
    oneCylinder();
    glPopMatrix();

    glPushMatrix();
    glRotatef(180,0,1,0);
    glRotatef(90,1,0,0);
    oneCylinder();
    glPopMatrix();
}

void cylinder(){
    updownCylinder();
    sideCylinder();
}


void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    // glTranslatef(0.0,0.0,-8.0);
    // glRotatef(270,0,1,1);
    gluLookAt(current.x,current.y,current.z,center.x,center.y,center.z,uward.x,uward.y,uward.z);
    axes();
    sphere();
    cylinder();
    octahedron();
    glBegin(GL_QUADS);


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
    current.x = current.z = 5;
    current.y = 5;
    center.x = center.y = center.z = 0;
    fward = make_forward();
    uward = point(0,1,0);
    rward = make_right();
    uward = make_up();
    fward.normalize();
    uward.normalize();
    rward.normalize();
    dist = distance();
    radius = sqrt(centX * centX + centY * centY + centZ * centZ);
    verticesXpos = buildUnitPositiveX(4);
}

void reshape(int w , int h){
    glViewport(0,0,(GLsizei)w , (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,1,2.0,1000.0);
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
    // angle += 0.8;
    // if(angle > 360.0){
    //     angle -= 360.0;
    // }
}

void special_func(int key , int x , int y){
    float rate = 0.8;
    switch (key)
    {
    case GLUT_KEY_UP:
        cout << "up key pressed" << endl;
        current.x += fward.x * rate;
        current.y += fward.y * rate;
        current.z += fward.z * rate;
        center.x += fward.x * rate;
        center.y += fward.y * rate;
        center.z += fward.z * rate;
        break;
    case GLUT_KEY_DOWN:
        cout << "down key pressed" << endl;
        current.x -= fward.x * rate;
        current.y -= fward.y * rate;
        current.z -= fward.z * rate;
        center.x -= fward.x * rate;
        center.y -= fward.y * rate;
        center.z -= fward.z * rate;
        break;
    case GLUT_KEY_PAGE_UP:
        cout << "pgup key pressed" << endl;
        current.x += uward.x * rate;
        current.y += uward.y * rate;
        current.z += uward.z * rate;
        center.x += uward.x * rate;
        center.y += uward.y * rate;
        center.z += uward.z * rate;
        break;
    case GLUT_KEY_PAGE_DOWN:
        cout << "pgdn key pressed" << endl;
        current.x -= uward.x * rate;
        current.y -= uward.y * rate;
        current.z -= uward.z * rate;
        center.x -= uward.x * rate;
        center.y -= uward.y * rate;
        center.z -= uward.z * rate;
        break;
    case GLUT_KEY_RIGHT:
        cout << "right key pressed" << endl;
        current.x += rward.x * rate;
        current.y += rward.y * rate;
        current.z += rward.z * rate;
        center.x += rward.x * rate;
        center.y += rward.y * rate;
        center.z += rward.z * rate;
        break;
    case GLUT_KEY_LEFT:
        cout << "left key pressed" << endl;
        current.x -= rward.x * rate;
        current.y -= rward.y * rate;
        current.z -= rward.z * rate;
        center.x -= rward.x * rate;
        center.y -= rward.y * rate;
        center.z -= rward.z * rate;
        break;
    default:
        break;
    }
}

void keyboard_Handler(unsigned char key, int x, int y){
    float rate = 0.02;
    switch(key){
        case '1':
        {   
            cout << "camera left rotate" << endl;
            point nrward(0,0,0);
            nrward.x = rward.x * cos(rate) + fward.x * sin(rate);
            nrward.y = rward.y * cos(rate) + fward.y * sin(rate);
            nrward.z = rward.z * cos(rate) + fward.z * sin(rate);
            point nfward = uward.cross(nrward);
            rward = nrward;
            fward = nfward;
            rward.normalize();
            fward.normalize();
            center.x = current.x + fward.x * dist;
            center.y = current.y + fward.y * dist;
            center.z = current.z + fward.z * dist;
            // cout << center.x << ' ' << center.y << ' ' << center.z << endl;
            // cout << uward.x << ' ' << uward.y << ' ' << uward.z << endl;
            break;
        }
        case '2':
        {
            cout << "camera right rotate" << endl;
            point nrward(0,0,0);
            nrward.x = rward.x * cos(-rate) + fward.x * sin(-rate);
            nrward.y = rward.y * cos(-rate) + fward.y * sin(-rate);
            nrward.z = rward.z * cos(-rate) + fward.z * sin(-rate);
            point nfward = uward.cross(nrward);
            rward = nrward;
            fward = nfward;
            rward.normalize();
            fward.normalize();
            center.x = current.x + fward.x * dist;
            center.y = current.y + fward.y * dist;
            center.z = current.z + fward.z * dist;
            break;
        }
        case '3':
        {
            cout << "camera up" << endl;
            point nfward(0,0,0);
            nfward.x = fward.x * cos(rate) + uward.x * sin(rate);
            nfward.y = fward.y * cos(rate) + uward.y * sin(rate);
            nfward.z = fward.z * cos(rate) + uward.z * sin(rate);
            point nuward = rward.cross(nfward);
            fward = nfward;
            uward = nuward;
            fward.normalize();
            uward.normalize();
            center.x = current.x + fward.x * dist;
            center.y = current.y + fward.y * dist;
            center.z = current.z + fward.z * dist;
            break;
        }
        case '4':
        {
            cout << "camera down" << endl;
            point nfward(0,0,0);
            nfward.x = fward.x * cos(-rate) + uward.x * sin(-rate);
            nfward.y = fward.y * cos(-rate) + uward.y * sin(-rate);
            nfward.z = fward.z * cos(-rate) + uward.z * sin(-rate);
            point nuward = rward.cross(nfward);
            fward = nfward;
            uward = nuward;
            fward.normalize();
            uward.normalize();
            center.x = current.x + fward.x * dist;
            center.y = current.y + fward.y * dist;
            center.z = current.z + fward.z * dist;
            break;
        }
        case '6':
        {
            cout << "camera tilt clock" << endl;
            point nrward(0,0,0);
            nrward.x = rward.x * cos(rate) + uward.x * sin(rate);
            nrward.y = rward.y * cos(rate) + uward.y * sin(rate);
            nrward.z = rward.z * cos(rate) + uward.z * sin(rate);
            point nuward = nrward.cross(fward);
            rward = nrward;
            uward = nuward;
            rward.normalize();
            uward.normalize();
            center.x = current.x + fward.x * dist;
            center.y = current.y + fward.y * dist;
            center.z = current.z + fward.z * dist;
            // cout << center.x << ' ' << center.y << ' ' << center.z << endl;
            // cout << uward.x << ' ' << uward.y << ' ' << uward.z << endl;
            break;
        }
        case '5':
        {
            cout << "camera tilt anti-clock" << endl;
            point nrward(0,0,0);
            nrward.x = rward.x * cos(-rate) + uward.x * sin(-rate);
            nrward.y = rward.y * cos(-rate) + uward.y * sin(-rate);
            nrward.z = rward.z * cos(-rate) + uward.z * sin(-rate);
            point nuward = nrward.cross(fward);
            rward = nrward;
            uward = nuward;
            rward.normalize();
            uward.normalize();
            center.x = current.x + fward.x * dist;
            center.y = current.y + fward.y * dist;
            center.z = current.z + fward.z * dist;
            // cout << center.x << ' ' << center.y << ' ' << center.z << endl;
            // cout << uward.x << ' ' << uward.y << ' ' << uward.z << endl;
            break;
        }
        case ',':
        {
            shrinkF -= rate;
            shrinkF = max(shrinkF , (float)0.0);
            break;
        }
        case '.':
        {
            shrinkF += rate;
            shrinkF = min(shrinkF , (float)1.0);
        }
        default:
            printf("Unknown Key Pressed\n");
            break;
    }
}


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
    glutKeyboardFunc(keyboard_Handler);
    glutSpecialFunc(special_func);
    glutMainLoop();
    return 0;
}