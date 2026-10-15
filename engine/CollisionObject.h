#ifndef COLLISION_OBJECT_H
#define COLLISION_OBJECT_H

#include <memory>
#include <vector>

#include "GraphicsObject.h"
#include "MathUtil.h"

namespace CMPUT350 {

enum class ShapeType {
    kLine, kCircle, kRect
};
union ShapeUnion {
    ShapeUnion() {}
    ShapeUnion(Line l) : line(l) {}
    ShapeUnion(Circle c) : circle(c) {}
    ShapeUnion(Rect r) : rect(r) {}
    Line line;
    Circle circle;
    Rect rect; // Axis-aligned rectangle
};
struct Shape {
    Shape() {}
    Shape(const Line &l) : shape(l), t(ShapeType::kLine) {}
    Shape(const Circle &c) : shape(c), t(ShapeType::kCircle) {}
    Shape(const Rect &r) : shape(r), t(ShapeType::kRect) {}
    ShapeUnion shape;
    ShapeType t;

    static bool Collides(Rect r0, const Rect& r1, Point2D& collisionPoint) {
        r0 &= r1;
        if (r0.width > 0 && r0.height > 0) {
            collisionPoint = r0.center();
            return true;
        }
        return false;
    }

    static bool Collides(const Circle& c0, const Circle& c1, Point2D& collisionPoint) {
        if (c0.center.Distance(c1.center) < c0.radius + c1.radius) {
            collisionPoint = c0.center + (c1.center - c0.center)/2.;
            return true;
        }
        return false;
    }

    static bool Collides(const Line& l0, const Line& l1, Point2D& collisionPoint) {
        return l0.Crosses(l1, collisionPoint);
    }

    static bool Collides(const Rect& r, const Line& l, Point2D& collisionPoint) {
        Line sides[4];
        r.Sides(sides);

        for (int i = 0; i < 4; i++) {
            if (l.Crosses(sides[i], collisionPoint)) {
                return true;
            }
        }
        return false;
    }

    static bool Collides(const Line& l, const Circle& c, Point2D& collisionPoint) {
        Point2D p = l.ClosestPoint(c.center);
        float d = p.Distance(c.center);
        if (d < c.radius) {
            collisionPoint = p;
            return true;
        }
        return false;
    }

    static bool Collides(const Rect &r, const Circle &c, Point2D& collisionPoint) {
        Line sides[4];
        r.Sides(sides);

        for (int i = 0; i < 4; i++) {
            if (Collides(sides[i], c, collisionPoint)) {
                return true;
            }
        }
        return false;
    }

    static bool Collides(const Line& l, const Rect& r, Point2D& collisionPoint) { return Collides(r, l, collisionPoint); }
    static bool Collides(const Circle& c, const Line& l, Point2D& collisionPoint) { return Collides(l, c, collisionPoint); }
    static bool Collides(const Circle& c, const Rect& r, Point2D& collisionPoint) { return Collides(r, c, collisionPoint); }

    /**
     * is there a better way to do this? i don't think templates
     * will work since types need to be known at compile time,
     * nested ternaries don't work, i don't know what other
     * options are
     */
    static bool Collides(const Shape& a, const Shape& b, Point2D& collisionPoint) {
        switch (a.t) {
            case ShapeType::kCircle:
                switch (b.t) {
                    case ShapeType::kCircle:
                        return Collides(a.shape.circle, b.shape.circle, collisionPoint);
                    case ShapeType::kLine:
                        return Collides(a.shape.circle, b.shape.line, collisionPoint);
                    case ShapeType::kRect:
                        return Collides(a.shape.circle, b.shape.rect, collisionPoint);
                }
                break;
            case ShapeType::kLine:
                switch (b.t) {
                    case ShapeType::kCircle:
                        return Collides(a.shape.line, b.shape.circle, collisionPoint);
                    case ShapeType::kLine:
                        return Collides(a.shape.line, b.shape.line, collisionPoint);
                    case ShapeType::kRect:
                        return Collides(a.shape.line, b.shape.rect, collisionPoint);
                }
                break;
            case ShapeType::kRect:
                switch (b.t) {
                    case ShapeType::kCircle:
                        return Collides(a.shape.rect, b.shape.circle, collisionPoint);
                    case ShapeType::kLine:
                        return Collides(a.shape.rect, b.shape.line, collisionPoint);
                    case ShapeType::kRect:
                        return Collides(a.shape.rect, b.shape.rect, collisionPoint);
                }
                break;
        }
        return false;
    }
};

class CollisionObject : public GraphicsObject {
public:
    virtual void CollisionEnter(const std::shared_ptr<CollisionObject> &obj) = 0;
    virtual void CollisionEnter(const std::shared_ptr<CollisionObject> &obj, const Point2D &collisionPoint) {
        CollisionEnter(obj);
    }
    virtual const Rect &GetBounds() = 0;
    virtual const std::vector<Shape> &GetShapes() = 0;
};

}  // namespace CMPUT350

#endif
