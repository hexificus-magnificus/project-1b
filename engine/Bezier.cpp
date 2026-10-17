#include "Bezier.h"
 

namespace CMPUT350 {

Bezier::Bezier(const std::vector<Point2D> &pts)
    : points(pts)
{}

int binom(int n, int k) {
	if (k == 0)
		return 1;
	int sol = n-k+1;
	for (int i = 1; i < k; ++i) {
		sol = sol * (n - k + i + 1) / (i + 1);
	}
	return sol;
}

float bernstein(int i, int n, float t) {
	return binom(n, i) * std::pow(t, i) * std::pow(1-t, n-i);
}

Point2D Bezier::GetPoint(float t) const {
    return GetPoint(points, t);
}

Point2D Bezier::GetSlope(float t) const {
    return GetSlope(points, t);
}

Point2D Bezier::GetPoint(const std::vector<Point2D> &pts, float t) {
    int segcount = (pts.size()-1) / 3;
    assert(t < segcount);
	int seg = std::floor(t);
	int start = seg*3;
	float x, y;
	x = y = 0;
	for (int i = 0; i < 4; i++) {
		float c = bernstein(i, 3, t);
		x += c * pts[start+i].x;
		y += c * pts[start+i].y;
	}
	return {x, y};
}

Point2D Bezier::GetSlope(const std::vector<Point2D> &pts, float t) {
    int segcount = (pts.size()-1) / 3;
    assert(t < segcount);
	int seg = std::floor(t);
	int start = seg*3;
	float x = 0;
	float y = 0;
	for (int i = 0; i < 3; i++) {
		Point2D d = pts[start+i+1] - pts[start+i];
		float c = bernstein(i, 2, t);
		x += c*d.x;
		y += c*d.y;
	}
	x *= 3;
	y *= 3;
	return Point2D{x, y};
}

float Bezier::GetRotation(Point2D slope, float t){
	float radians = std::atan2(slope.y, slope.x);
	return radians;
}
}

/*
Point2D getPoint(const std::vector<sf::Vector2f>& pts, float t) {
	int segcount = (pts.size()-1) / 3;
	int seg = 0;
	if (t < 1) {
		seg = (int)(t*segcount);
		t = t*segcount - seg;
	} else {
		seg = segcount-1;
	}
	int start = seg*3;
	float x, y;
	x = y = 0;
	for (int i = 0; i < 4; i++) {
		float c = bernstein(i, 3, t);
		x += c * pts[start+i].x;
		y += c * pts[start+i].y;
	}
	return {x, y};
}

// TODO: (Part 2) Define a function that returns the curve's slope at t in [0, 1].
Point2D getSlope(const std::vector<sf::Vector2f>& pts, float t) {
	int segcount = (pts.size()-1) / 3;
	int seg = 0;
	if (t < 1) {
		seg = (int)(t*segcount);
		t = t*segcount - seg;
	} else {
		seg = segcount-1;
	}
	int start = seg*3;
	float x = 0;
	float y = 0;
	for (int i = 0; i < 3; i++) {
		sf::Vector2f d = pts[start+i+1] - pts[start+i];
		float c = bernstein(i, 2, t);
		x += c*d.x;
		y += c*d.y;
	}
	x *= 3;
	y *= 3;
	return Point2D{x, y};
}
	*/