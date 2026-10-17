#include "Enemy.h"
#include "Bullet.h"
#include "Bezier.h"

Enemy::Enemy(CMPUT350::Point2D loc, const std::vector<CMPUT350::Point2D>& path, CMPUT350::Point2D stopPosition)
	: pos(loc), bounds(loc.x-20.0f, loc.y -20.f, 40.f, 40.f), alive(true), mShapes(1), mPath(path), mStopPosition(stopPosition)
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
		CMPUT350::Point2D slope = CMPUT350::Bezier::GetSlope(mPath, t);
		rotation = CMPUT350::Bezier::GetRotation(slope, t);
		t += 0.01f;
	}else{
		rotation = 0.0f;
	}
	bounds = CMPUT350::Rect(pos.x-20.0f, pos.y -20.f, 40.f, 40.f);
	mShapes[0] = CMPUT350::Shape(bounds);
}

void Enemy::LateUpdate(CMPUT350::GameContext* context)
{
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

bool Enemy::isInFormation() const{
	return t>=1.0f;
}

const CMPUT350::Rect& Enemy::GetBounds()
{
    return bounds;
}

const std::vector<CMPUT350::Shape> &Enemy::GetShapes() {
	return mShapes;
}

CMPUT350::Point2D Enemy::GetLocation() const{
	return pos;
};
float Enemy::GetRotation() const{
	return rotation;
};


//Enemy 1
void Enemy1::Initialize(CMPUT350::GameContext* context){
	Enemy::Initialize(context);
}

void Enemy1::LateUpdate(CMPUT350::GameContext* context){
	Enemy::LateUpdate(context);
}

bool Enemy1::Attack(CMPUT350::GameContext* context, const CMPUT350::Point2D &target){
	return false;
}

void Enemy1::RenderForeground(CMPUT350::GameContext *context){
	Enemy::RenderForeground(context);
}

//Enemy 2
void Enemy2::Initialize(CMPUT350::GameContext* context){
	Enemy::Initialize(context);
}

void Enemy2::LateUpdate(CMPUT350::GameContext* context){
	Enemy::LateUpdate(context);
}

bool Enemy2::Attack(CMPUT350::GameContext* context, const CMPUT350::Point2D &target){
	return false;
}

void Enemy2::RenderForeground(CMPUT350::GameContext *context){
	Enemy::RenderForeground(context);
}

//Enemy 3
void Enemy3::Initialize(CMPUT350::GameContext* context){
	Enemy::Initialize(context);
}

void Enemy3::LateUpdate(CMPUT350::GameContext* context){
	Enemy::LateUpdate(context);
}

bool Enemy3::Attack(CMPUT350::GameContext* context, const CMPUT350::Point2D &target){
	return false;
}

void Enemy3::RenderForeground(CMPUT350::GameContext *context){
	Enemy::RenderForeground(context);
}