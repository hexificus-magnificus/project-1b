#include "Enemy.h"
#include "Bullet.h"
#include "Bezier.h"

Enemy::Enemy(CMPUT350::Point2D loc, const std::vector<CMPUT350::Point2D>& path)
	: pos(loc), bounds(loc.x-20.0f, loc.y -20.f, 40.f, 40.f), alive(true), mShapes(1), mPath(path)
{
	mShapes[0] = CMPUT350::Shape(bounds);
}

void Enemy::Initialize(CMPUT350::GameContext* context)
{
}

void Enemy::Update(CMPUT350::GameContext* context)
{
	if (t<1.0f){
		pos = CMPUT350::Bezier::GetPoint(mPath, t);
		t += 0.01f;
		bounds = CMPUT350::Rect(pos.x-20.0f, pos.y -20.f, 40.f, 40.f);
		mShapes[0] = CMPUT350::Shape(bounds);
	}
}

void Enemy::LateUpdate(CMPUT350::GameContext* context)
{
}

bool Enemy::HandleKeyEvent(CMPUT350::GameContext* context, char key)
{
	return true;
}

void Enemy::RenderBackground(CMPUT350::GameContext* context)
{
}

void Enemy::RenderForeground(CMPUT350::GameContext* context)
{
	if (!alive)
		return;
	context->ScreenContext->DrawRect(bounds, CMPUT350::Colors::red);
}

void Enemy::CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj)
{
	if (std::dynamic_pointer_cast<Bullet>(obj)) {
		// both Enemy and Bullet are already axis-aligned rectangles, so no need to check further
		Kill();
	}
}

void Enemy::Kill()
{
	alive = false;
}

bool Enemy::IsAlive() const
{
    return alive;
}

const CMPUT350::Rect& Enemy::GetBounds()
{
    return bounds;
}

const std::vector<CMPUT350::Shape> &Enemy::GetShapes() {
	return mShapes;
}