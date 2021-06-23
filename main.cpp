#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <tuple>
#include <algorithm>
#include <map>

using namespace std;

#include "Image.hpp"

int height = 1000, width = 1000;
pix White = {255,255,255}, Red = {255,0,0}, Green = {0,255,0}, Black = {0,0,0};

void drawLine(int x1,int y1, int x2, int y2, Image &img, pix &Col, map<int, vector<int> > *mp = nullptr) {

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

vector<pair<double, double>> vertices;
vector<tuple<int, int, int>> trianglesIndices;

void readFile() {

    fstream oin("data.txt", ios::in);

    while (oin) {

        string s;
        getline(oin, s);
        if(s.empty()) continue;

        string id = s.substr(0,2);
        vector<string> dat = split(s, ' ');

        if(id == "v ") {
            double x = stod(dat[1]), y = stod(dat[2]);
            vertices.push_back({x,y});
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

void drawTriangle(vector<int> p1, vector<int> p2, vector<int> p3, Image &img, pix &Col) {
    vector<vector<int>> points = {p1,p2,p3};
    sort(points.begin(), points.end(), [](vector<int> &a, vector<int> &b){
        return a[1] > b[1];
    });
    map<int, vector<int>> mp;
    drawLine(p1[0],p1[1],p2[0],p2[1],img,Col, &mp);
    drawLine(p2[0],p2[1],p3[0],p3[1],img,Col, &mp);
    drawLine(p1[0],p1[1],p3[0],p3[1],img,Col, &mp);

    for(auto p: mp) {
        sort(p.second.begin(), p.second.end());
        int i1=p.second[0], i2 = p.second[(int)p.second.size()-1], j = p.first;
                for(int i=i1;i<=i2;++i) {
            img.SetPixel(i,j,Col);
        }
    }


}

void human(Image &img, pix &Col) {
    for(auto face: trianglesIndices) {
        int i1 = get<0>(face), i2 = get<1>(face), i3 = get<2>(face);
        int x1 = translate(vertices[i1].first, width), y1 = translate(vertices[i1].second, height);
        int x2 = translate(vertices[i2].first, width), y2 = translate(vertices[i2].second, height);
        int x3 = translate(vertices[i3].first, width), y3 = translate(vertices[i3].second, height);

        pix CC = {rand()%256, rand()%256, rand()%256};
        drawTriangle({x1,y1}, {x2,y2}, {x3,y3}, img, CC);
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