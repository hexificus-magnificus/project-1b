#include "DrawContext.h"

namespace CMPUT350 {

DrawContext::DrawContext(std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<sf::Font> font)
    : mWindow(window), mFont(font), viewAngleMat(0) {}

/**
 * Prints the given string at p,
 * using sf::Shape::getLocalBounds
 * to center it.
 * Units in pixels
 */
void DrawContext::DrawCenteredText(const std::string &text, int pixelSize, Point2D p, RGBColor c) {
	sf::Text t(*mFont, text, pixelSize);
	sf::Color color(c.r, c.g, c.b);
	t.setFillColor(color);
	sf::FloatRect box = t.getLocalBounds();
	float width = box.size.x;
	float height = box.size.y;
	t.setOrigin(sf::Vector2f(width/2, height/2));
	t.setPosition(sf::Vector2f(p.x, p.y));
	mWindow->draw(t);
}

/**
 * Prints the given text with its
 * top-left corner at p
 * Units in pixels
 */
void DrawContext::DrawText(const std::string &text, int pixelSize, Point2D p, RGBColor c) {
	sf::Text t(*mFont, text, pixelSize);
	sf::Color color(c.r, c.g, c.b);
	t.setFillColor(color);
	t.setPosition(sf::Vector2f(p.x, p.y));
	mWindow->draw(t);
}

/**
 * Draws a circle centered at p
 * with radius r.
 * Units in pixels
 */
void DrawContext::DrawCircle(Point2D p, float radius, RGBColor c) {
	sf::CircleShape circle(radius);
	circle.setFillColor(sf::Color(c.r, c.g, c.b));
	circle.setPosition(sf::Vector2f(p.x-radius, p.y-radius));
	mWindow->draw(circle);
}

/**
 * Draws a CMPUT350::Rect onto the screen.
 * Does not support rotation.
 * Units in pixels
 */
void DrawContext::DrawRect(Rect r, RGBColor c) {
	sf::RectangleShape rect(sf::Vector2f(r.width, r.height));
	rect.setFillColor(sf::Color(c.r, c.g, c.b));
	rect.setPosition({r.topLeft.x, r.topLeft.y});
	mWindow->draw(rect);
}

/**
 * Same as DrawRect, but adds a border
 * of *width* pixels
 */
void DrawContext::FrameRect(Rect r, float width, RGBColor c) {
	sf::RectangleShape rect(sf::Vector2f(r.width, r.height));
	rect.setFillColor(sf::Color(c.r, c.g, c.b));
	rect.setPosition({r.topLeft.x, r.topLeft.y});
	rect.setOutlineThickness(width);
	mWindow->draw(rect);
}

/**
 * @brief Draws a line between two points with a specified width and color.
 *
 * @param from The starting point of the line (Point2D).
 * @param to The ending point of the line (Point2D).
 * @param width The width of the line in pixels.
 * @param c The color of the line, specified as an RGBColor object.
 *
 * This function calculates the distance and angle between the two points
 * and uses a polygone shape to represent the line. The line is drawn
 * relative to the world offset and rendered onto the associated window.
 */
void DrawContext::DrawLine(Point2D from, Point2D to, float width, RGBColor c) {
	Point2D btwn = to - from;
	float m = btwn.y/btwn.x;
	float perpm = -1/m;
	Point2D perp(1, perpm);
	perp.Normalize();
	perp *= width;
	// winding is not set cw/ccw, but will be constant within each shape
	Point2D c0 = from + perp;
	Point2D c1 = from - perp;
	Point2D c2 = to - perp;
	Point2D c3 = to + perp;
	sf::ConvexShape l;
	l.setPointCount(4);
	l.setPoint(0, {c0.x, c0.y});
	l.setPoint(1, {c1.x, c1.y});
	l.setPoint(2, {c2.x, c2.y});
	l.setPoint(3, {c3.x, c3.y});
	l.setFillColor(sf::Color(c.r, c.g, c.b));
	mWindow->draw(l);
}

int DrawContext::GetWindowWidth() { return mWindow->getSize().x; }

int DrawContext::GetWindowHeight() { return mWindow->getSize().y; }

void DrawContext::SetContextOffset(Point2D p) {
	viewPos = p;
}

// units of radians
void DrawContext::SetContextRotation(float rotation) {
	// normal rotation matrix for 2d point
	viewAngleMat[0] = viewAngleMat[3] = std::cos(rotation);
	viewAngleMat[2] = -1 * (viewAngleMat[1] = std::sin(rotation));
}

Point2D mmul(Point2D p, float m[4]) {
	return {p.x*m[0] + p.y*m[2], p.x*m[1] + p.y*m[3]};
}

Point2D DrawContext::Transform(Point2D p) {
	// returns (p - viewPos)M
	p -= viewPos;
	return mmul(p, viewAngleMat);
}

Point2D DrawContext::ReverseTransform(Point2D p) {
	// takes (p - viewPos)M and returns p
	// via (p - viewPos)M * (M^-1) + viewPos
	float inv[4]{viewAngleMat[0], -viewAngleMat[1], -viewAngleMat[2], viewAngleMat[3]};
	return mmul(p, inv) + viewPos;
}

Rect DrawContext::Transform(Rect r) {
	// cos(viewAngle) = viewAngleMat[0]
	// sin(viewAngle) = viewAngleMat[1]
	float newH = r.height * viewAngleMat[0] + r.width * viewAngleMat[1];
	float newW = r.height * viewAngleMat[1] + r.width * viewAngleMat[0];
	return {r.topLeft, newW, newH};
}

}  // namespace CMPUT350
