#include "Enemy.h"
#include "Bullet.h"

Enemy::Enemy(CMPUT350::Point2D loc)
	: pos(loc), bounds(loc, 20.f), alive(true)
{
}

void Enemy::Initialize(CMPUT350::GameContext* context)
{
}

void Enemy::Update(CMPUT350::GameContext* context)
{
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
