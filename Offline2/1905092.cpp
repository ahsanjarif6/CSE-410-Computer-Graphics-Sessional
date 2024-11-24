#include<bits/stdc++.h>
#include "bitmap_image.hpp"
using namespace std;
#define PI 2.0*acos(0.0)
#define zero 1e-8

struct Point
{
    double x , y , z , n;

    Point(double x , double y , double z , double n){
        this->x = x;
        this->y = y;
        this->z = z;
        this->n = n;
    }

    Point(double x, double y, double z){
        this->x = x;
        this->y = y;
        this->z = z;
        this->n = 0.1;
    }

    Point(){
        this->x = 0;
        this->y = 0;
        this->z = 0;
        this->n = 1.0;
    }

    Point(const Point &p) : x(p.x), y(p.y), z(p.z), n(p.n) {}
};


Point operator+(const Point&a , const Point&b){
    return Point(a.x + b.x , a.y + b.y , a.z + b.z);
}

Point operator-(const Point&a , const Point&b){
    return Point(a.x - b.x , a.y - b.y , a.z - b.z);
}

Point operator*(const Point&a , double c){
    return Point(a.x * c , a.y * c , a.z * c);
}

Point operator/(const Point&a , double c){
    return Point(a.x / c , a.y / c , a.z / c);
}

double operator*(const Point&a , const Point& b){
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}

Point operator^(const Point&a , const Point& b){
    return Point(a.y * b.z - a.z * b.y , a.z * b.x - a.x * b.z , a.x * b.y - a.y * b.x);
}


// Point pointAdd(Point a , Point b){
//     return Point(a.x + b.x , a.y + b.y , a.z + b.z);
// }

// Point pointSub(Point a , Point b){
//     return Point(a.x - b.x , a.y - b.y , a.z - b.z);
// }

// Point pointMul(Point a , double c){
//     return Point(a.x * c , a.y * c , a.z * c);
// }

// Point pointDiv(Point a , double c){
//     return Point(a.x / c , a.y / c , a.z / c);
// }

// double dotProduct(Point a , Point b){
//     return (a.x * b.x + a.y * b.y + a.z * b.z);
// }

// Point crossProduct(Point a , Point b){
//     return Point(a.y * b.z - a.z * b.y , a.z * b.x - a.x * b.z , a.x * b.y - a.y * b.x);
// }

double length(Point p){
    return sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
}

void normalize(Point& p){
    double len = length(p);
    p = p / len;
}

void scaleDown(Point& p){
    p.x /= p.n;
    p.y /= p.n;
    p.z /= p.n;
    p.n = 1.0;

}

static unsigned long long int g_seed = 1;
inline int rand(){
    g_seed = (214013*g_seed+2531011);
    return (g_seed>>16)&0x7FFF;
}


struct Triangle
{
    Point a , b , c;
    tuple< int , int , int > color;
    
    Triangle(Point a, Point b, Point c){
        this->a = a;
        this->b = b;
        this->c = c;
    }

    Triangle(){
        this->a = Point();
        this->b = Point();
        this->c = Point();
    }

    void recolor(){
        color = make_tuple(rand() , rand() , rand());
    }

    void sort(){
        // b > a
        // c > a
        // c > b
        if(b.y < a.y) swap(a,b);
        if(c.y < a.y) swap(a,c);
        if(c.y < b.y) swap(b,c);
    }
};


const int sz = 4;
struct Matrix{
    double mat[sz][sz];

    Matrix(){
        memset(mat , 0 , sizeof mat);
    }

    void identity(){
        memset(mat , 0 , sizeof mat);
        for(int i = 0 ; i < sz ; i ++ ) mat[i][i] = 1;
    }

    void scale(Point p){
        identity();
        mat[0][0] = p.x;
        mat[1][1] = p.y;
        mat[2][2] = p.z;
    }

    void translate(Point p){
        identity();
        mat[0][3] = p.x;
        mat[1][3] = p.y;
        mat[2][3] = p.z;
    }
};

Matrix operator*(const Matrix& a , const Matrix& b){
    Matrix res;
    for(int i = 0 ; i < sz ; i ++ ){
        for(int j = 0 ; j < sz ; j ++ ){
            for(int k = 0 ; k < sz ; k ++ ){
                res.mat[i][j] += a.mat[i][k] * b.mat[k][j];
            }
        }
    }
    return res;
}

Point operator*(const Matrix& a , const Point& p){
    vector< double > res(sz , 0.0);
    vector< double > p_mat(sz);
    p_mat[0] = p.x;
    p_mat[1] = p.y;
    p_mat[2] = p.z;
    p_mat[3] = p.n;

    for(int i = 0 ; i < sz ; i ++ ){
        for(int j = 0 ; j < sz ; j ++ ){
            res[i] += a.mat[i][j] * p_mat[j];
        }
    }

    Point ans(res[0] , res[1] , res[2] , res[3]);
    scaleDown(ans);
    return ans;
}

Triangle operator*(const Matrix& a , const Triangle& t){
    Triangle res;
    res.a = a * t.a;
    res.b = a * t.b;
    res.c = a * t.c;
    return res;
}

Point R(Point x , Point a , double theta){
    Point res;
    normalize(a);
    res = x * cos(theta) + a * ((a * x) * (1 - cos(theta))) + (a ^ x) * sin(theta);
    return res;
}

void rotate(Matrix& m , Point a , double angle){
    normalize(a);
    m.identity();
    Point i(1.0,0.0,0.0),j(0.0,1.0,0.0),k(0.0,0.0,1.0);
    Point c1 = R(i,a,angle);
    Point c2 = R(j,a,angle);
    Point c3 = R(k,a,angle);
    m.mat[0][0] = c1.x;     m.mat[0][1] = c2.x;     m.mat[0][2] = c3.x;
    m.mat[1][0] = c1.y;     m.mat[1][1] = c2.y;     m.mat[1][2] = c3.y;
    m.mat[2][0] = c1.z;     m.mat[2][1] = c2.z;     m.mat[2][2] = c3.z;
}


int main(){
    ifstream fin("scene.txt");
    ofstream fout("stage1.txt");

    Point eye , look , up;
    fin >> eye.x >> eye.y >> eye.z;
    fin >> look.x >> look.y >> look.z;
    fin >>  up.x >> up.y >> up.z;

    double fovY , aspectRatio , near , far;
    fin >> fovY >> aspectRatio >> near >> far;

    string s;
    stack< Matrix > st;
    Matrix I;
    I.identity();
    st.push(I);
    fout << fixed << setprecision(7);
    while(true){
        fin >> s;
        if(s == "triangle"){
            Triangle t;
            fin >> t.a.x >> t.a.y >> t.a.z;
            fin >> t.b.x >> t.b.y >> t.b.z;
            fin >> t.c.x >> t.c.y >> t.c.z;
            //fin >> t.a >> t.b >> t.c;
            t = st.top() * t;
            fout << t.a.x << ' ' << t.a.y << ' ' << t.a.z << '\n';
            fout << t.b.x << ' ' << t.b.y << ' ' << t.b.z << '\n';
            fout << t.c.x << ' ' << t.c.y << ' ' << t.c.z << '\n';
            fout << '\n';
        }
        else if(s == "translate"){
            Point p;
            fin >> p.x >> p.y >> p.z;
            Matrix m;
            m.translate(p);
            st.top() = st.top() * m;
        }
        else if(s == "scale"){
            Point p;
            fin >> p.x >> p.y >> p.z;
            Matrix m;
            m.scale(p);
            st.top() = st.top() * m;
        }
        else if(s == "rotate"){
            Point p;
            double angle;
            fin >> angle >> p.x >> p.y >> p.z;
            angle = angle * (PI / 180.0);
            Matrix m;
            rotate(m , p , angle);
            st.top() = st.top() * m;
        }
        else if(s == "push"){
            st.push(st.top());
        }
        else if(s == "pop"){
            st.pop();
        }
        else if(s == "end"){
            break;
        }
    }
    fin.close();
    fout.close();

    fin.open("stage1.txt");
    fout.open("stage2.txt");

    Point l , r , u;
    l = look - eye;
    normalize(l);
    r = l ^ up;
    normalize(r);
    u = r ^ l;
    normalize(u);
    Matrix T;
    T.identity();
    T.mat[0][3] = -eye.x;
    T.mat[1][3] = -eye.y;
    T.mat[2][3] = -eye.z;
    Matrix R;
    R.identity();
    R.mat[0][0] = r.x;      R.mat[0][1] = r.y;      R.mat[0][2] = r.z;
    R.mat[1][0] = u.x;      R.mat[1][1] = u.y;      R.mat[1][2] = u.z;
    R.mat[2][0] = -l.x;     R.mat[2][1] = -l.y;     R.mat[2][2] = -l.z;

    Matrix V = R * T;
    Triangle t;
    while(fin>>t.a.x>>t.a.y>>t.a.z>>t.b.x>>t.b.y>>t.b.z>>t.c.x>>t.c.y>>t.c.z){
        // cout << t.a.x << ' ' << t.a.y << ' ' << t.a.z << '\n';
        // cout << t.b.x << ' ' << t.b.y << ' ' << t.b.z << '\n';
        // cout << t.c.x << ' ' << t.c.y << ' ' << t.c.z << '\n';
        // cout << '\n';
        t = V * t;
        // for(int i = 0 ; i < 4 ; i ++ ){
        //     for(int j = 0 ; j < 4 ; j ++ ){
        //         cout << V.mat[i][j] << ' ' ;
        //     }
        //     cout << endl;
        // }
        fout << t.a.x << ' ' << t.a.y << ' ' << t.a.z << '\n';
        fout << t.b.x << ' ' << t.b.y << ' ' << t.b.z << '\n';
        fout << t.c.x << ' ' << t.c.y << ' ' << t.c.z << '\n';
        fout << '\n';
    }

    fin.close();
    fout.close();

    fin.open("stage2.txt");
    fout.open("stage3.txt");

    double fovX = fovY * aspectRatio;
    double tt = near * tan((fovY * PI) / 360.0);
    double rr = near * tan((fovX * PI) / 360.0);

    Matrix P;
    P.mat[0][0] = near / rr;
    P.mat[1][1] = near / tt;
    P.mat[2][2] = -(far + near) / (far - near);
    P.mat[2][3] = -(2.0 * far * near) / (far - near);
    P.mat[3][2] = -1.0;

    while(fin>>t.a.x>>t.a.y>>t.a.z>>t.b.x>>t.b.y>>t.b.z>>t.c.x>>t.c.y>>t.c.z){
        t = P * t;
        fout << t.a.x << ' ' << t.a.y << ' ' << t.a.z << '\n';
        fout << t.b.x << ' ' << t.b.y << ' ' << t.b.z << '\n';
        fout << t.c.x << ' ' << t.c.y << ' ' << t.c.z << '\n';
        fout << '\n';
    }
    fin.close();
    fout.close();

    fin.open("stage3.txt");
    fout.open("z_buffer.txt");
    ifstream fin2("config.txt");
    

    int screen_width , screen_height;
    fin2 >> screen_width >> screen_height;
    double box_bottom = -1 , box_top = 1;
    double box_left = -1 , box_right = 1;
    double z_min = -1 , z_max = 1;
    double dx = (box_right - box_left) / (double)screen_width;
    double dy = (box_top - box_bottom) / (double)screen_height;
    double top_y = box_top - dy / 2.0;
    double left_x = box_left + dx / 2.0;

    vector< vector< double > > z_buffer(screen_height , vector< double > (screen_width , z_max));
    bitmap_image img(screen_width , screen_height);
    img.set_all_channels(0 , 0 , 0);
    
    while(fin>>t.a.x>>t.a.y>>t.a.z>>t.b.x>>t.b.y>>t.b.z>>t.c.x>>t.c.y>>t.c.z){

        double min_y = max(t.a.y , box_bottom);
        double max_y = min(t.b.y , box_top);
        
        t.sort();
        t.recolor();

        for(double y = min_y ; y <= max_y ; y += dy){
            double min_x = 0 ,  max_x = -1 , min_z = 0 , max_z = -1;

            if(fabs(t.b.y - t.a.y) > zero and fabs(t.a.y - t.c.y) > zero){
                // min_x = t.a.x + ((t.b.x - t.a.x) * (y - t.a.y)) / (t.b.y - t.a.y);
                // max_x = t.a.x + ((t.c.x - t.a.x) * (y - t.a.y)) / (t.c.y - t.a.y);

                // min_z = t.a.z + ((t.b.z - t.a.z) * (y - t.a.y)) / (t.b.y - t.a.y);
                // max_z = t.a.z + ((t.c.z - t.a.z) * (y - t.a.y)) / (t.c.y - t.a.y);

                min_x = (t.b.x - t.a.x) * (y - t.a.y);
                min_x /= (t.b.y - t.a.y);
                min_x += t.a.x;

                max_x = (t.c.x - t.a.x) * (y - t.a.y);
                max_x /= (t.c.y - t.a.y);
                max_x += t.a.x;

                min_z = (t.b.z - t.a.z) * (y - t.a.y);
                min_z /= (t.b.y - t.a.y);
                min_z += t.a.z;

                max_z = (t.c.z - t.a.z) * (y - t.a.y);
                max_z /= (t.c.y - t.a.y);
                max_z += t.a.z;

                if(min_x > max_x){
                    swap(min_x , max_x);
                    swap(min_z , max_z);
                }
            }

            min_x = max(min_x, box_left);
            max_x = min(max_x, box_right);

            for(double x = min_x ; x <= max_x ; x += dx){
                if(fabs(max_x - min_x) > zero){
                    int i = (top_y - y) / dy;
                    int j = (x - left_x) / dx;

                    //double z = min_z + ((max_z - min_z) * (x - min_x)) / (max_x - min_x);
                    double z = (max_z - min_z) * (x - min_x);
                    z /= (max_x - min_x);
                    z += min_z;
                    if(z < z_buffer[i][j] and z > z_min){
                        z_buffer[i][j] = z;
                        img.set_pixel(j , i , get<0>(t.color) , get<1>(t.color) , get<2>(t.color));
                    }
                }
            }

        }

        min_y = max(t.b.y , box_bottom);
        max_y = min(t.c.y , box_top);

        for(double y = min_y ; y <= max_y ; y += dy){
            double min_x = 0 ,  max_x = -1 , min_z = 0 , max_z = -1;

            if(fabs(t.b.y - t.c.y) > zero and fabs(t.a.y - t.c.y) > zero){
                // min_x = t.c.x + ((t.b.x - t.c.x) * (y - t.c.y)) / (t.b.y - t.c.y);
                // max_x = t.a.x + ((t.c.x - t.a.x) * (y - t.a.y)) / (t.c.y - t.a.y);

                // min_z = t.c.z + ((t.b.z - t.c.z) * (y - t.c.y)) / (t.b.y - t.c.y);
                // max_z = t.a.z + ((t.c.z - t.a.z) * (y - t.a.y)) / (t.c.y - t.a.y);

                min_x = (t.b.x - t.c.x) * (y - t.c.y);
                min_x /= (t.b.y - t.c.y);
                min_x += t.c.x;

                max_x = (t.c.x - t.a.x) * (y - t.a.y);
                max_x /= (t.c.y - t.a.y);
                max_x += t.a.x;

                min_z = (t.b.z - t.c.z) * (y - t.c.y);
                min_z /= (t.b.y - t.c.y);
                min_z += t.c.z;

                max_z = (t.c.z - t.a.z) * (y - t.a.y);
                max_z /= (t.c.y - t.a.y);
                max_z += t.a.z;

                if(min_x > max_x){
                    swap(min_x , max_x);
                    swap(min_z , max_z);
                }
            }

            min_x = max(min_x, box_left);
            max_x = min(max_x, box_right);

            for(double x = min_x ; x <= max_x ; x += dx){
                if(fabs(max_x - min_x) > zero){
                    int i = (top_y - y) / dy;
                    int j = (x - left_x) / dx;
                    //cout << dx << ' ' << dy << i << ' ' << j << endl;
                    //double z = min_z + ((max_z - min_z) * (x - min_x)) / (max_x - min_x);
                    double z = (max_z - min_z) * (x - min_x);
                    z /= (max_x - min_x);
                    z += min_z;
                    if(z < z_buffer[i][j] and z > z_min){
                        z_buffer[i][j] = z;
                        img.set_pixel(j , i , get<0>(t.color) , get<1>(t.color) , get<2>(t.color));
                    }
                }
            }
        }
    }

    fout << setprecision(7) << fixed;
    img.save_image("out.bmp");
    for(auto u: z_buffer){
        for(auto v : u){
            if(v < z_max){
                fout << v << '\t';
            }
        }
        fout << '\n';
    }


    fin.close();
    fin2.close();
    fout.close();
    
    img.clear();
    z_buffer.clear();
    z_buffer.shrink_to_fit();
    
    return 0;

}
// for(int i=0; i<screen_height; i++){
//     for(int j=0; j<screen_width; j++){
//         if (z_buffer[i][j] < z_max) {
//             fout << z_buffer[i][j] << "\t";
//         }
//     }
//     fout << '\n';
// }