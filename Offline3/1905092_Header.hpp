#include<bits/stdc++.h>
#include "bitmap_image.hpp"
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
using namespace std;
#define red glColor3f(1.0, 0.0, 0.0)
#define green glColor3f(0.0, 1.0, 0.0)
#define blue glColor3f(0.0, 0.0, 1.0)
#define cyan glColor3f(0, 1, 1)
#define magenta glColor3f(1, 0, 1)
#define yellow glColor3f(1, 1, 0)
#define black glColor3f(0,0,0)
#define white glColor3f(1,1,1)
#define PI acos(-1)


// class point;
class Object;
class Light;
class SpotLight;
// class Sphere;
// class Triangle;
// class Floor;
// class Ray;

// extern vector <Light*> lights;
// extern vector <SpotLight*> spotlights;
extern vector <Object*> ObjVec;
extern vector <Light*> lights;
extern vector< SpotLight* > spotlights;
extern int recursionLevel;

double determinantValue(const vector< vector< double > >& v){
    return (v[0][0] * (v[1][1] * v[2][2] - v[1][2] * v[2][1])
            -v[0][1] * (v[1][0] * v[2][2] - v[2][0] * v[1][2])
                +v[0][2] * (v[1][0] * v[2][1] - v[2][0] * v[1][1]));
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
    point operator +(point b)  {return point(x+b.x,y+b.y, z+b.z);}
    point operator -(point b)  {return point(x-b.x,y-b.y, z-b.z);}
	point operator *(double b)  {return point(x*b,y*b, z*b);}
	point operator /(double b)  {return point(x/b,y/b, z/b);}
	double operator *(point b)  {return x*b.x+y*b.y+z*b.z;} // DOT PRODUCT
	point operator ^(point b)  {return point(y*b.z-z*b.y, z*b.x-x*b.z, x*b.y-y*b.x);} // CROSS PRODUCT
    point operator -()  {return point(-x,-y,-z);}

    double length() {return sqrt(x*x+y*y+z*z);}
    friend istream& operator>>(istream &in, point &p)
    {
        in >> p.x >> p.y >> p.z;
        return in;
    }

    friend ostream& operator<<(ostream &output, point &p)
    {
        output << fixed << setprecision(7) << p.x << " " << p.y <<" " << p.z;
        return output;
    }
};

struct Ray{
    point origin , direction;
    Ray(){

    }
    Ray(point _origin , point _direction){
        origin = _origin;
        direction = _direction;
        direction.normalize();
    }

    friend ostream& operator<<(ostream &out, Ray& r)
    {
        out << "Origin : " << r.origin << ", Direction : " << r.direction;
        return out;
    }
};

struct Light{
    point pos,color;

    Light(){

    }

    Light(point _pos , point _color){
        pos = _pos;
        color = _color;
    }

    void draw(){
        glColor3f(color.x, color.y, color.z);
        glPointSize(5);
        glBegin(GL_POINTS);
        glVertex3f(pos.x, pos.y, pos.z);
        glEnd();
    }

    friend istream& operator>>(istream &in, Light &l)
    {
        in >> l.pos.x >> l.pos.y >> l.pos.z >> l.color.x >> l.color.y >> l.color.z;
        return in;
    }    

    friend ostream& operator<<(ostream &out, Light& l)
    {
        out << "Position : " << l.pos << ", Color : " << l.color;
        return out;
    }
};


struct SpotLight{
    point direction , color;
    Light pointLight;
    double cutoffAngle;

    void draw(){
        color = pointLight.color;
        
        glColor3f(color.x,color.y,color.z);
        glPointSize(15);
        glBegin(GL_POINTS);{
            glVertex3f(pointLight.pos.x , pointLight.pos.y , pointLight.pos.z);
        }glEnd();
    }

    friend istream& operator>>(istream &in, SpotLight &l)
    {
        in >> l.pointLight.pos  >> l.pointLight.color.x >> l.pointLight.color.y >> l.pointLight.color.z;
        in >> l.direction;
        in >> l.cutoffAngle;
        return in;
    }

    friend ostream& operator<<(ostream &out, SpotLight& s)
    {
        out << "PointLight : " << s.pointLight << '\n';
        out << "Color : " << s.pointLight.color << '\n';
        out << "Direction : " << s.direction << '\n';
        out << "Degree : " << s.cutoffAngle << '\n';
        return out;
    }
};

class Object{
public:
    point reference_point;
    double height, width, length;
    double color[3];
    double coefficients[4];
    //
    // ambient,
    // diffuse,
    // specular, reflection coefficients
    int shine; // exponent term of specular component
    Object(){
        color[0]=0,color[1]=1,color[2]=1;
    }
    virtual void draw(){}
    virtual double intersectHelper(Ray& ray,point& color){ return 0.0; }
    virtual Ray getNormal(point p, Ray incidentRay) = 0;
    virtual double intersect(Ray& ray,point& color,int level=1){ 
        double t = intersectHelper(ray,color);
        
        if(t < 0) return -1.0;
        if(level == 0) return t;


        point intersectionPoint = ray.origin + ray.direction * t;
        point colorAtIntersection = color;
        color = color*coefficients[0];
        // cout << color << ' ' << coefficients[0] << '\n';

        // for(int i = 0 ; i < lights.size() ; i ++ ){
        for(Light* l : lights){
            // cout << *l << '\n';
            point pos = l->pos;
            point dir = intersectionPoint - pos;
            dir.normalize();

            Ray lightRay = Ray(pos,dir);
            Ray normal = getNormal(intersectionPoint,lightRay);

            double t2 = (intersectionPoint - pos).length();
            if(t2 < 0) continue;
            bool obscured = false;
            for(Object* obj: ObjVec){
                point dummyColor;
                double t3 = obj->intersectHelper(lightRay,dummyColor);
                if(t3 > 0 and t3 + 1e-1 < t2){
                    obscured = true;
                    break;
                }
            }

            if(!obscured){
                double val = max(0.0 , - normal.direction * lightRay.direction);

                Ray reflection = Ray(intersectionPoint,lightRay.direction - normal.direction * (2.0 * (normal.direction * lightRay.direction)));
                double phong = max(0.0,-reflection.direction*ray.direction);

                color.x += l->color.x * colorAtIntersection.x * (val * coefficients[1] + pow(phong , shine) * coefficients[2]);
                color.y += l->color.y * colorAtIntersection.y * (val * coefficients[1] + pow(phong , shine) * coefficients[2]);
                color.z += l->color.z * colorAtIntersection.z * (val * coefficients[1] + pow(phong , shine) * coefficients[2]);

            }
        }

        for(SpotLight* s: spotlights){
            point pos = s->pointLight.pos;
            point dir = intersectionPoint - pos;
            dir.normalize();

            double dot = dir * s->direction;
            double angle = (180.0 / PI) * acos(dot / (dir.length()*s->direction.length()));

            if(fabs(angle) < s->cutoffAngle){
                Ray lightRay = Ray(pos,dir);
                Ray normal = getNormal(intersectionPoint,lightRay);

                Ray reflection = Ray(intersectionPoint , lightRay.direction - normal.direction * (2.0 * (normal.direction * lightRay.direction)));
                double t2 = (intersectionPoint - pos).length();
                if(t2 < 0) continue;

                bool obscured = false;
                for(Object* obj: ObjVec){
                    point dummyColor;
                    double t3 = obj->intersectHelper(lightRay,dummyColor);
                    if(t3 > 0 and t3 +1e-20 < t2){
                        obscured = true;
                        break;
                    }
                }
                if(!obscured){
                    double val = max(0.0 , -normal.direction * lightRay.direction);
                    double phong = max(0.0 , -reflection.direction * ray.direction);

                    color.x += s->pointLight.color.x * colorAtIntersection.x * (val * coefficients[1] + pow(phong , shine) * coefficients[2]);
                    color.y += s->pointLight.color.y * colorAtIntersection.y * (val * coefficients[1] + pow(phong , shine) * coefficients[2]);
                    color.z += s->pointLight.color.z * colorAtIntersection.z * (val * coefficients[1] + pow(phong , shine) * coefficients[2]);

                }
            }
        }

        if(level < recursionLevel){
            Ray normal = getNormal(intersectionPoint,ray);
            Ray reflection = Ray(intersectionPoint,ray.direction - normal.direction*(2.0*(normal.direction*ray.direction)));
            reflection.origin = reflection.origin + reflection.direction * 1e-5;
            int idx = 0;
            int nearest = -1;
            double t = -1 , tMin = 1e9;
            for(Object* obj: ObjVec){
                point dummyColor;
                t = obj->intersect(reflection,dummyColor,0);
                if((nearest == -1 or t < tMin) and t > 0){
                    tMin = t , nearest = idx;
                }
                idx ++ ;
            }
            if(nearest != -1){
                point dummyColor(0,0,0);
                // dummyColor = color;
                double t = ObjVec[nearest]->intersect(reflection,dummyColor,level+1);
                color.x += dummyColor.x * coefficients[3];
                color.y += dummyColor.y * coefficients[3];
                color.z += dummyColor.z * coefficients[3];
            }
        }
        return t;
    }
    void setColor(double r , double g , double b){
        color[0]=r;
        color[1]=g;
        color[2]=b;
    }
    void setShine(int __shine){
        shine=__shine;
    }
    void setCoEfficients(double a,double b,double c,double d){
        coefficients[0]=a;
        coefficients[1]=b;
        coefficients[2]=c;
        coefficients[3]=d;
    }
};




class Sphere :public Object{
public:
    Sphere(){

    }
    Sphere(point center,double radius){
        reference_point = center;
        length = radius;
    }
    void draw(){
        // cout << "draw call hocche sphere e " << '\n';
        glColor3f(color[0],color[1],color[2]);
        glPushMatrix();

        glTranslatef(reference_point.x,reference_point.y,reference_point.z);
        glutSolidSphere(length,40,40);
        glPopMatrix();
    }
    Ray getNormal(point p , Ray incidentRay){
        return Ray(p , p - reference_point);
    }
    double intersectHelper(Ray& rray,point& color){
        // cout << "sphere\n";
        color.x = this->color[0];
        color.y = this->color[1];
        color.z = this->color[2];
        Ray ray;
        ray.origin = rray.origin;
        ray.direction = rray.direction;
        ray.origin = ray.origin - reference_point;
        double a = 1;
        double b = 2.0 * (ray.direction * ray.origin);
        double c = (ray.origin * ray.origin) - length*length;

        double d = b * b - 4.0 * a * c;
        double t;
        // cout << length << '\n';
        // cout << " d = " << d << '\n';
        if(d < 0.0){
            return -1.0;
        }
        double root[2];
        for(int i = 0 ; i < 2 ; i ++ ){
            root[i] = -b;
            if(i % 2) root[i] += sqrt(d);
            else root[i] -= sqrt(d);
            root[i] /= 2.0;
        }
        sort(root , root + 2);
        if(root[0] > 0){
            return root[0];
        }
        if(root[1] > 0){
            return root[1];
        }
        return -1.0;
    }
    friend std::istream& operator>>(std::istream& in, Sphere& s){
        in >> s.reference_point >> s.length; // center and radius
        //color
        for(int i = 0 ; i < 3 ; i ++ ){
            in >> s.color[i];
        }
        //coeff
        for(int i = 0; i < 4; i++){
            in >> s.coefficients[i];
        }
        in >> s.shine;
        return in;
    }
    friend ostream& operator<<(ostream &output, Sphere &s)
    {
        // output << fixed << setprecision(7) << p.x << " " << p.y <<" " << p.z;
        output << fixed << setprecision(7);
        output << s.reference_point << '\n';
        output << s.length << '\n';
        for(int i = 0 ; i < 3 ; i ++ ) output << s.color[i] << ' ' ;
        output << '\n';
        for(int i = 0 ; i < 4 ; i ++ ) output << s.coefficients[i] << ' ' ;
        output << '\n';
        output << s.shine << '\n';
        return output;
    }
};


class Triangle :public Object{
public:
    point a,b,c;
    Triangle(){

    }
    Triangle(point _a,point _b,point _c){
        a=_a;
        b=_b;
        c=_c;
    }
    void draw(){
        // cout << "traingle draw hocche" << '\n';
        // cout << "point a = " << a << '\n';
        // cout << "point b = " << b << '\n';
        // cout << "point c = " << c << '\n';
        
        glColor3f(color[0],color[1],color[2]);
        // glColor3f(246/255.0,99/255.0,143/255.0);
        glBegin(GL_TRIANGLES);{
            glVertex3f(a.x,a.y,a.z);
            glVertex3f(b.x,b.y,b.z);
            glVertex3f(c.x,c.y,c.z);
        }
        glEnd();
    }
    double intersectHelper(Ray& ray,point& color){
        color.x = this->color[0];
        color.y = this->color[1];
        color.z = this->color[2];
        // cout << "triangle\n";
        // cout << " point  a= " << this->a << '\n';
        // cout << " point  b= " << this->b << '\n';
        // cout << " point  c= " << this->c << '\n';
        vector< vector< double > > betaMat(3,vector< double > (3,0.0));
        vector< vector< double > > gammaMat(3,vector< double > (3,0.0));
        vector< vector< double > > tMat(3,vector< double > (3,0.0));
        vector< vector< double > > AMat(3,vector< double > (3,0.0));
        {
            betaMat[0][0] = a.x - ray.origin.x;
            betaMat[1][0] = a.y - ray.origin.y;
            betaMat[2][0] = a.z - ray.origin.z;
        }
        {
            betaMat[0][1] = a.x - c.x;
            betaMat[1][1] = a.y - c.y;
            betaMat[2][1] = a.z - c.z;
        }
        {
            betaMat[0][2] = ray.direction.x;
            betaMat[1][2] = ray.direction.y;
            betaMat[2][2] = ray.direction.z;
        }
        {
            gammaMat[0][0] = a.x - b.x;
            gammaMat[1][0] = a.y - b.y;
            gammaMat[2][0] = a.z - b.z;
        }
        {
            gammaMat[0][1] = a.x - ray.origin.x;
            gammaMat[1][1] = a.y - ray.origin.y;
            gammaMat[2][1] = a.z - ray.origin.z;
        }
        {
            gammaMat[0][2] = ray.direction.x;
            gammaMat[1][2] = ray.direction.y;
            gammaMat[2][2] = ray.direction.z;
        }
        {
            tMat[0][0] = a.x - b.x;
            tMat[1][0] = a.y - b.y;
            tMat[2][0] = a.z - b.z;
        }
        {
            tMat[0][1] = a.x - c.x;
            tMat[1][1] = a.y - c.y;
            tMat[2][1] = a.z - c.z;
        }
        {
            tMat[0][2] = a.x - ray.origin.x;
            tMat[1][2] = a.y - ray.origin.y;
            tMat[2][2] = a.z - ray.origin.z;
        }
        {
            AMat[0][0] = a.x - b.x;
            AMat[1][0] = a.y - b.y;
            AMat[2][0] = a.z - b.z;
        }
        {
            AMat[0][1] = a.x - c.x;
            AMat[1][1] = a.y - c.y;
            AMat[2][1] = a.z - c.z;
        }
        {
            AMat[0][2] = ray.direction.x;
            AMat[1][2] = ray.direction.y;
            AMat[2][2] = ray.direction.z;
        }
        double A = determinantValue(AMat);
        // if(A < 1e-6) return -1.0;
        double beta = determinantValue(betaMat) / A;
        double gamma = determinantValue(gammaMat) / A;
        double t = determinantValue(tMat) / A;
        
        // cout << "dekih t = " << t << '\n';
        if(beta > 0 and gamma > 0 and beta + gamma < 1 and t > 0) return t;
        return -1.0;
    }
    Ray getNormal(point p , Ray incidentRay){
        point normal = (b-a)^(c-a);
        normal.normalize();

        return (incidentRay.direction * normal < 0)? Ray(p,-normal):Ray(p,normal);
    }
    friend istream& operator>>(istream &in, Triangle &t)
    {
        in >> t.a >> t.b >> t.c; // 3 vertices
        //color
        for(int i = 0 ; i < 3 ; i ++ ){
            in >> t.color[i];
        }
        //coeff
        for(int i = 0; i < 4; i++){
            in >> t.coefficients[i];
        }
        in >> t.shine;
        return in;
    }
    friend ostream& operator<<(ostream &output, Triangle &s)
    {
        // output << fixed << setprecision(7) << p.x << " " << p.y <<" " << p.z;
        output << fixed << setprecision(7);
        output << s.a << '\n';
        output << s.b << '\n';
        output << s.c << '\n';
        for(int i = 0 ; i < 3 ; i ++ ) output << s.color[i] << ' ' ;
        output << '\n';
        for(int i = 0 ; i < 4 ; i ++ ) output << s.coefficients[i] << ' ' ;
        output << '\n';
        output << s.shine << '\n';
        return output;
    }
};


class Floor:public Object{
public:
    int tiles;
    Floor(){

    }

    Floor(int floorWidth , int tileWidth){
        this->coefficients[0] = 0.4;
        length = tileWidth;
        reference_point = point(-floorWidth/2.0,-floorWidth/2.0,0);
        tiles = floorWidth / tileWidth;
    }

    void checkerBoard(){
        for(int i = 0 ; i < tiles ; i ++ ){
            for(int j = 0 ; j < tiles ; j ++ ){
                int k = (i+j)&1;
                if(k) black;
                else white;
                glBegin(GL_QUADS);{
                    glVertex3f(reference_point.x + i * length, reference_point.y + j * length, 0);
					glVertex3f(reference_point.x + (i + 1) * length, reference_point.y + j * length, 0);
					glVertex3f(reference_point.x + (i + 1) * length, reference_point.y + (j + 1) * length, 0);
					glVertex3f(reference_point.x + i * length, reference_point.y + (j + 1) * length, 0);
                }glEnd();
            }
        }
    }

    void draw(){
        checkerBoard();   
    }

    void colorAt(point p,point& color){
        int i = (p.x - reference_point.x) / length;
        int j = (p.y - reference_point.y) / length;

        if(i < 0 or i >= tiles or j < 0 or j >= tiles){
            color = point(0,0,0);
            return;
        }
        int k = (i + j)%2;
        if(k) color = point(0,0,0);
        else color = point(1,1,1);
    }

    Ray getNormal(point p , Ray incidentRay){
        return (incidentRay.direction.z > 0)? Ray(p,point(0,0,1)):Ray(p,point(0,0,-1));
    }
    double intersectHelper(Ray& ray,point& color){
        color.x = this->color[0];
        color.y = this->color[1];
        color.z = this->color[2];
        // cout << "floor\n";
        point normal = point(0,0,1);
        double nRd = normal * ray.direction;

        // if(nRd < 1e-6) return -1.0;

        double nRo = normal * ray.origin;
        double t = -nRo / nRd;
        point p = ray.origin + ray.direction * t;
        colorAt(p,color);
        if(p.x <= reference_point.x or p.x >= -reference_point.x or p.y <= reference_point.y or p.y >= -reference_point.y) return -1.0;
        return t;
    }
};

