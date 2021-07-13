#ifndef VEC3_H
#define VEC3_H

#include <iostream>

template<class T> class Vec3 {
public:
    T x,y,z;

    Vec3& operator+=(Vec3<T> &B) {x += B.x, y += B.y, z += B.z;return *this;}
    Vec3& operator-=(Vec3<T> &B) {x -= B.x, y -= B.y, z -= B.z;return *this;}
    Vec3& operator*=(T k) {x *= k, y *= k, z *= k;return *this;}
    Vec3& operator/=(T k) {x /= k, y /= k, z /= k;return *this;}

    friend Vec3<T> operator+(Vec3<T> &A, Vec3<T> &B) {Vec3<T> C = {A.x+B.x, A.y+B.y, A.z+B.z};return C;}
    friend Vec3<T> operator-(Vec3<T> &A, Vec3<T> &B) {Vec3<T> C = {A.x-B.x, A.y-B.y, A.z-B.z};return C;}
    friend Vec3 operator*(Vec3<T> &A, T k) {Vec3<T> C = {k*A.x, k*A.y, k*A.z};return C;}
    friend Vec3 operator/(Vec3 A, T k) {Vec3<T> C = {A.x/k, A.y/k, A.z/k};return C;}
    friend T operator*(Vec3<T> A, Vec3<T> B) {T C = B.x*A.x + B.y*A.y + B.z*A.z;return C;}
    friend Vec3 operator^(Vec3 A, Vec3 B) {Vec3<T> C = {A.y*B.z - A.z*B.y ,  A.z*B.x-A.x*B.z   , A.x*B.y-A.y*B.x};return C;}
    friend std::ostream& operator<<(ostream &oss, Vec3<T> &P) {oss << "[" << P.x << " " << P.y << " " << P.z << "]\n";return oss;}
};

template<class T> class Vec2 {
public:
    T x,y;

    Vec2& operator+=(Vec2 &B);
    Vec2& operator-=(Vec2 &B);
    Vec2& operator*=(T k);
    Vec2& operator/=(T k);

    template<class U>
    friend Vec2 operator+(Vec2 &A, Vec2 &B);
    template<class U>
    friend Vec2 operator-(Vec2 &A, Vec2 &B);
    template<class U>
    friend Vec2 operator*(Vec2 &A, T k);
    template<class U>
    friend std::ostream& operator<< (std::ostream &oss, Vec2 &P);
};

typedef Vec3<double> Vec3f;
typedef Vec3<int>   Vec3i;
typedef Vec2<double> Vec2f;
typedef Vec2<int>   Vec2i;


template<class T>
double abs(Vec3<T> &V) {
    return sqrt(V.x*V.x + V.y*V.y + V.z*V.z);
}
template<class T>
T Dot(Vec3<T> A, Vec3<T> B) {
    return A.x*B.x+A.y*B.y+A.z*B.z;
}


template<class T>
double abs(Vec2<T> &V) {
    return sqrt(V.x*V.x + V.y*V.y);
}
template<class T>
T Dot(Vec2<T> A, Vec2<T> B) {
    return A.x*B.x+A.y*B.y;
}


template<class T> 
Vec2<T>& Vec2<T>::operator+=(Vec2<T> &B) {
    x += B.x, y += B.y;
    return *this;
}

template<class T>
Vec2<T>& Vec2<T>::operator*=(T k) {
    x *= k, y *= k;
    return *this;
}

template<class T>
Vec2<T>& Vec2<T>::operator/=(T k) {
    x /= k, y /= k;
    return *this;
}
template<class T>
Vec2<T>& Vec2<T>::operator-=(Vec2<T> &B) {
    x -= B.x, y -= B.y;
    return *this;
}
template<class T>
Vec2<T> operator+(Vec2<T> &A, Vec2<T> &B) {
    Vec2<T> C = {A.x+B.x, A.y+B.y};
    return C;
}
template<class T>
Vec2<T> operator-(Vec2<T> &A, Vec2<T> &B) {
    Vec2<T> C = {A.x-B.x, A.y-B.y};
    return C;
}
template<class T>
Vec2<T> operator*(Vec2<T> &A, T k) {
    Vec2<T> C = {k*A.x, k*A.y};
    return C;
}
template<class T>
ostream& operator<<(ostream &oss, Vec2<T> &P) {
    oss << "[" << P.x << " " << P.y << "]\n";
    return oss; 
}

#endif