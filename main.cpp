#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <tuple>
#include <algorithm>
#include <map>

using namespace std;

#include "Image.hpp"
#include "Vector.hpp"

template<class T>
double abs(Vec3<T> &V);

int height = 1000, width = 1000;
pix White = {255,255,255}, Red = {255,0,0}, Green = {0,255,0}, Black = {0,0,0};

void drawLine(Vec2i P1, Vec2i P2, Image &img, pix &Col, map<int, vector<int> > *mp = nullptr) {

    int x1 = P1.x, x2 = P2.x;
    int y1 = P1.y, y2 = P2.y;
    
    // If both the points coincide then handle the case seperately.
    if(x1 == x2 && y1 == y2) {
        img.SetPixel(x1,x2,Col);
        if(mp != nullptr) {
            (*mp)[y1].push_back(x1);
        }
        return;
    }

    bool steep = 0;
    if(abs(y2-y1) > abs(x2-x1)) {
        swap(x1,y1), swap(x2,y2);
        steep = 1;
    }

    // Go from left to right
    if(x1 > x2) {
        swap(x1, x2), swap(y1,y2);
    }
    
    int A = (y2-y1), B = (x2-x1), C = x2*y1 - x1*y2;
    
    for(int X = x1; X <= x2; ++X) {
        int D = C + X*A;
        int Y1 = ceil((double)D/B), Y2 = D/B;

        int d1 = abs(B*Y1 - D), d2 = abs(B*Y2 - D);
        int Y = (d1<d2?Y1:Y2);

        int rX = X, rY = Y;
        
        if(steep)
            swap(rX,rY);
        
        img.SetPixel(rX,rY,Col);
        if(mp != nullptr)
            (*mp)[rY].push_back(rX);
    }

}

vector<string> split(string s, char ch) {
    string tmp = "";
    vector<string> ans;
    for(auto i: s) {
        if(i == ch  && tmp.size()) {
            ans.push_back(tmp);
            tmp.clear();
        }
        else if(i != ch) {
            tmp.push_back(i);
        }
    }
    if(tmp.size()) ans.push_back(tmp);
    return ans;
}

vector<Vec3f> vertices;
vector<tuple<int, int, int>> trianglesIndices;

void readFile() {

    fstream oin("data.obj", ios::in);

    while (oin) {

        string s;
        getline(oin, s);
        if(s.empty()) continue;

        string id = s.substr(0,2);
        vector<string> dat = split(s, ' ');

        if(id == "v ") {
            double x = stod(dat[1]), y = stod(dat[2]), z = stod(dat[3]);
            vertices.push_back({x,y,z});
        }
        else if(id == "f ") {
            int a = stoi(split(dat[1],'/')[0]), b = stoi(split(dat[2],'/')[0]), c = stoi(split(dat[3], '/')[0]);
            trianglesIndices.push_back(make_tuple(a-1,b-1,c-1));
        }
    }
    oin.close();
}

int translate(double x, int len) {
    double i = x * (len/2);
    int ans = i + (len/2);
    return ans;
}

void drawTriangle(Vec2i p1, Vec2i p2, Vec2i p3, Image &img, pix &Col) {
    map<int, vector<int>> mp;
    drawLine(p1,p2,img,Col, &mp);
    drawLine(p2,p3,img,Col, &mp);
    drawLine(p1,p3,img,Col, &mp);

    for(auto p: mp) {
        sort(p.second.begin(), p.second.end());
        int i1=p.second[0], i2 = p.second[(int)p.second.size()-1], j = p.first;
        for(int i=i1;i<=i2;++i) {
            img.SetPixel(i,j,Col);
        }
    }
}

Vec3f light_dir = {0,0,-1};

void human(Image &img, pix &Col) {
    for(auto face: trianglesIndices) {
        int i1 = get<0>(face), i2 = get<1>(face), i3 = get<2>(face);
        int x1 = translate(vertices[i1].x, width), y1 = translate(vertices[i1].y, height);
        int x2 = translate(vertices[i2].x, width), y2 = translate(vertices[i2].y, height);
        int x3 = translate(vertices[i3].x, width), y3 = translate(vertices[i3].y, height);

        Vec3f Normal = (vertices[i3] - vertices[i1])^(vertices[i2] - vertices[i1]);
        double intensity = light_dir*(Normal/abs(Normal));

        // pix CC = {rand()%256, rand()%256, rand()%256};
        if(intensity > 0) {
        pix CC = {White.r*intensity, White.g*intensity, White.b*intensity};
        drawTriangle({x1,y1}, {x2,y2}, {x3,y3}, img, CC);
        }
        // drawLine(x1,y1,x2,y2, img, Black);
        // drawLine(x2,y2,x3,y3, img, Black);
        // drawLine(x1,y1,x3,y3, img, Black);
    }
}

int main() {
    
    Image img("img.bmp", height, width);
    pix Col = {84,253,227};

    readFile();
    human(img, Col);

    img.Generate();

    return 0;
}