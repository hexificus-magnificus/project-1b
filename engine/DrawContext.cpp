#include "DrawContext.h"

namespace CMPUT350 {

DrawContext::DrawContext(std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<sf::Font> font)
    : mWindow(window), mFont(font) {}

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

void DrawContext::DrawText(const std::string &text, int pixelSize, Point2D p, RGBColor c) {
	sf::Text t(*mFont, text, pixelSize);
	sf::Color color(c.r, c.g, c.b);
	t.setFillColor(color);
	t.setPosition(sf::Vector2f(p.x, p.y));
	mWindow->draw(t);
}

void DrawContext::DrawCircle(Point2D p, float radius, RGBColor c) {
	sf::CircleShape circle(radius);
	circle.setFillColor(sf::Color(c.r, c.g, c.b));
	circle.setPosition(sf::Vector2f(p.x, p.y));
	mWindow->draw(circle);
}

void DrawContext::DrawRect(Rect r, RGBColor c) {
	sf::RectangleShape rect(sf::Vector2f(r.width, r.height));
	rect.setFillColor(sf::Color(c.r, c.g, c.b));
	rect.setPosition({r.topLeft.x, r.topLeft.y});
	mWindow->draw(rect);
}

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
	float angle = std::acos(from.Dot(to) / (from.Distance({0, 0}) * to.Distance({0, 0})));
	float dist = to.Distance(from);
	sf::Vector2f size(dist, width);
	sf::RectangleShape line(size);
	line.setOrigin({0, width/2});
	line.setPosition({from.x, from.y});
	line.setRotation(sf::radians(angle));
	line.setFillColor(sf::Color(c.r, c.g, c.b));
	mWindow->draw(line);
}

int DrawContext::GetWindowWidth() { return mWindow->getSize().x; }

int DrawContext::GetWindowHeight() { return mWindow->getSize().y; }

}  // namespace CMPUT350
