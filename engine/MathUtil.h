#ifndef MATHUTIL_H
#define MATHUTIL_H

#include <cmath>
#include <iostream>

namespace CMPUT350 {

struct Point2D {
    float x, y;
    Point2D(float x = 0, float y = 0) : x(x), y(y) {}
    double Distance(const Point2D &other) const {
        double distance = sqrt(pow(x-other.x, 2)+pow(y-other.y, 2));
        return distance;
    }

    Point2D operator+(const Point2D &other) const {
        // TODO: write this code
        return Point2D(x+other.x, y+other.y);
    }

    Point2D operator+(const float &other) const {
        // TODO: write this code
        return Point2D(x+other, y+other);
    }

    Point2D operator-(const Point2D &other) const {
        // TODO: write this code
        return Point2D(x-other.x, y-other.y);
    }
    Point2D operator-(const float &other) const {
        // TODO: write this code
        return Point2D(x-other, y-other);
    }
    Point2D operator*(const float &scalar) const {
        // TODO: write this code
        return Point2D(x*scalar, y*scalar);
    }
    Point2D &operator+=(const float &scalar) {
        // TODO: write this code
        x += scalar;
        y += scalar;
        return *this;
    }
    Point2D &operator+=(const Point2D &other) {
        // TODO: write this code
        x += other.x;
        y += other.y;
        return *this;
    }
    Point2D &operator-=(const Point2D &other) {
        // TODO: write this code
        x -= other.x;
        y -= other.y;
        return *this;
    }
    bool operator==(const Point2D &other) const {
        // TODO: write this code
        return (x ==other.x && y==other.y);
        
    }
    Point2D &operator*=(const int &scalar) {
        // TODO: write this code
        x *= scalar;
        y *= scalar;
        return *this;
    }
    Point2D &operator/=(const int &scalar) {
        // TODO: write this code
        x /= scalar;
        y /= scalar;
        return *this;
    }
    float operator*(const Point2D &other) const {
        // TODO: write this code
        
        return ((x*other.x)+(y*other.y));
    }
    float Dot(Point2D b) const {
        // TODO: write this code
        return (x*b.x+y*b.y);
    }
    static float Dot(Point2D a, Point2D b) {
        // TODO: write this code
        return (a.x*b.x+a.y*b.y);
    }
    static float Cross(Point2D a, Point2D b) {
        // TODO: write this code
        return a.x*b.y-a.y*b.x;
    }
    void Normalize() {
        // TODO: write this code
        float magnitude = sqrt(pow(x, 2)+pow(y, 2));
        if (magnitude != 0){
            x /= magnitude;
            y /=magnitude;
        }
        
    }
};

static std::ostream &operator<<(std::ostream &os, const Point2D &p) {
    // source: https://stackoverflow.com/a/27770465
    // TODO: write this code
    os << "("<<p.x<<","<<p.y<<")";
    return os;
}

static Point2D operator*(float number, const Point2D &rhs) {
    // TODO: write this code
    return rhs*number;
}

struct Line {
    Point2D p1, p2;

    Line(Point2D p1 = {0, 0}, Point2D p2 = {0, 0}) : p1(p1), p2(p2) {}
    Line(float x1, float y1, float x2, float y2) : p1(x1, y1), p2(x2, y2) {}
    float Length() const {
        // TODO: write this code
        return p1.Distance(p2);
    }
    Point2D ClosestPoint(const Point2D &p) const {
        Point2D ab = p2-p1;
        Point2D av = p-p1;
        float abLength = sqrt(pow(ab.x, 2)+pow(ab.y, 2));
        float ax = av.Dot(ab)/abLength;
        ab.Normalize();
        Point2D x = p1 + (ab*ax);
        return x;
    }

    bool Crosses(Line other, Point2D &crossingPoint) const {
        // TODO: write this code
        // p = p1, r = p2-p1, q = other.p1, s = other.p2-other.p1
        // t = (q - p) x s / (r x s)
        // u = (p − q) × r / (s × r)
        // source: https://stackoverflow.com/a/565282
        Point2D r = p2-p1;
        Point2D s = other.p2-other.p1;
        if (Point2D::Cross(r, s)==0){
            return false;
        }
        float t = Point2D::Cross(other.p1-p1, s)/Point2D::Cross(r, s);
        float u = Point2D::Cross(p1-other.p1, r)/Point2D::Cross(s, r);
        if (t<=1 && t>=0 && u<=1 && u >=0){
            Point2D n = p1+t*r;
            crossingPoint = n;
            return true;
        }
        return false;
    }
};

static std::ostream &operator<<(std::ostream &os, const Line &l) {
    // TODO: write this code
    os <<l.p1<<","<<l.p2;
    return os;
}

struct Circle {
    Point2D center;
    float radius;

    Circle(Point2D c = {0, 0}, float r = 0) : center(c), radius(r) {}

    Circle(float x, float y, float r) : center(x, y), radius(r) {}
};

struct Rect {
    Point2D topLeft;
    float width, height;

    Rect(float left, float top, float width, float height)
        : topLeft(Point2D(left, top)), width(width), height(height) {} 
        //!!!!!!!!!!!!!! I changed Point2D from Point2D(top, left) to Point2D(left, top)
        // want to check it again later to make sure it's correct

    Rect(Point2D tl = {0, 0}, int w = 0, int h = 0) : topLeft(tl), width(w), height(h) {}

    // Creates bounding box around p1 and p2 with positive width/height
    Rect(Point2D p1, Point2D p2)
        : topLeft(std::min(p1.x, p2.x), std::min(p1.y, p2.y)),
          width(fabs(p1.x - p2.x)),
          height(fabs(p1.y - p2.y)) {}

    Rect(Point2D center, float radius)
        : topLeft(center.x - radius, center.y - radius), width(2 * radius), height(2 * radius) {}

    Rect &operator|=(const Rect &other) {
        Point2D newTopLeft = Point2D(std::min(topLeft.x, other.topLeft.x), std::min(topLeft.y, other.topLeft.y));
        float rightEdge = std::max(topLeft.x+width, other.topLeft.x+other.width);
        float bottomEdge = std::max(topLeft.y+height, other.topLeft.y+other.height);
        topLeft = newTopLeft;
        width = rightEdge-topLeft.x;
        height = bottomEdge-topLeft.y;
    
        return *this;
    }
    Rect &operator|=(const Point2D &other) {
        // TODO: write this code
        float rightEdge = std::max(topLeft.x+width, other.x);
        float bottomEdge = std::max(topLeft.y+height, other.y);
        topLeft.x = std::min(topLeft.x, other.x);
        topLeft.y = std::min(topLeft.y, other.y);
        width = rightEdge-topLeft.x;
        height = bottomEdge-topLeft.y;
        return *this;
    }
    Rect &operator|=(const Line &other) {
        *this |= other.p1;
        *this |= other.p2;
        return *this;
    }
    Rect &operator&=(const Rect &other) {
        // source: https://www.geeksforgeeks.org/dsa/intersecting-rectangle-when-bottom-left-and-top-right-corners-of-two-rectangles-are-given/
        // !!!!!!!! what should this return if they don't overlap?
        Point2D bottomRight = Point2D(topLeft.x+width, topLeft.y+height);
        Point2D bottomRightOther = Point2D(other.topLeft.x+other.width, other.topLeft.y+other.height);
        Point2D newBottomRight = Point2D(std::min(bottomRight.x, bottomRightOther.x), std::min(bottomRight.y, bottomRightOther.y));
        Point2D newTopLeft = Point2D(std::max(topLeft.x, other.topLeft.x), std::max(topLeft.y, other.topLeft.y));
        topLeft = newTopLeft;
        width = newBottomRight.x - newTopLeft.x;
        height = newBottomRight.y - newTopLeft.y;
        return *this;
    }
    Rect &operator+=(const Point2D &other) {
        topLeft.x += other.x;
        topLeft.y += other.y;
        return *this;
    }
    Rect operator+(const Point2D &other) const {
        return Rect(topLeft.x+other.x, topLeft.y+other.y, width, height);
    }
    void Inset(int inset) {
        // TODO: write this code
        topLeft.x += inset;
        topLeft.y += inset;
        width -= 2*inset;
        height -= 2*inset;
    }
    bool IsInside(const Point2D &p) const {
        if (p.x >= topLeft.x && p.x <= topLeft.x+width && p.y >= topLeft.y && p.y<= topLeft.y+height){
            return true;
        }
        return false;
    }
};

static std::ostream &operator<<(std::ostream &os, const Rect &l) {
    os <<l.topLeft<<", "<<l.width<<", "<<l.height;
    return os;
}

}  // namespace CMPUT350

#endif  // MATHUTIL_H
