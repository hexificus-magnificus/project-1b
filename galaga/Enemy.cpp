#include "Enemy.h"
#include "Bullet.h"
#include "Bezier.h"
#include "Player.h"

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
	float dt = clock.restart().asMicroseconds() / 1000000.;
	if (t<1.0f){
		pos = CMPUT350::Bezier::GetPoint(mPath, t);
		CMPUT350::Point2D slope = CMPUT350::Bezier::GetSlope(mPath, t);
		rotation = CMPUT350::Bezier::GetRotation(slope, t);
		t = std::min(1.0f, t + (1.f/3.f) * dt);
	}else{
		rotation *= 0.9f;
        if (std::abs(rotation) < 0.01f) {
            rotation = 0.0f;
        }
	}
	bounds = CMPUT350::Rect(pos.x - 20.f, pos.y - 20.f, 40.f, 40.f);
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
	context->ScreenContext->DrawRect({{-20, -20}, {20, 20}}, color);
}

void Enemy::CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj)
{	
	auto bullet = std::dynamic_pointer_cast<Bullet>(obj); 
	auto player = std::dynamic_pointer_cast<Player>(obj);

	if (bullet&& bullet->IsPlayerBullet()) {
		// both Enemy and Bullet are already axis-aligned rectangles, so no need to check further
		mLives--;
		if (mLives ==1){
			color = lightColor;
		}else if (mLives == 2){
			color = mediumColor;
		}else if (mLives == 3){
			color = darkColor;
		}
		else if (mLives <=0){
			Kill();	
		}
		
	}
	else if (player){
		Kill();
	}
}

void Enemy::Shoot(CMPUT350::GameContext* context){
	CMPUT350::Point2D heading(0.0f, 6.0f);
	std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>(pos, heading, false);
	context->mEngineView->AddGameObject(bullet);
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
	mLives = 1;
	
}

void Enemy1::LateUpdate(CMPUT350::GameContext* context){
	Enemy::LateUpdate(context);
}

bool Enemy1::Attack(CMPUT350::GameContext* context, const CMPUT350::Point2D &target){
	if (!isInFormation()){
		return false;
	}

	mPath = {
        pos,
		{pos.x + 500.f, pos.y + 1800.f},
		{pos.x - 500.f, pos.y + 1800.f},
		mStopPosition
    };
	Shoot(context);
	t = 0.0f;
	return true;
}

void Enemy1::RenderForeground(CMPUT350::GameContext *context){
	Enemy::RenderForeground(context);
}

//Enemy 2
void Enemy2::Initialize(CMPUT350::GameContext* context){
	Enemy::Initialize(context);
	mLives = 2;
}

void Enemy2::LateUpdate(CMPUT350::GameContext* context){
	Enemy::LateUpdate(context);
}

bool Enemy2::Attack(CMPUT350::GameContext* context, const CMPUT350::Point2D &target){
	if (!isInFormation()){
		return false;
	}

	mPath = {
        pos,
		{pos.x - 700.f, pos.y + 1100.f},
		{pos.x + 700.f, pos.y + 1100.f},
		mStopPosition
	};
	t = 0.0f;
	return true;
}

void Enemy2::RenderForeground(CMPUT350::GameContext *context){
	Enemy::RenderForeground(context);
}



//Enemy 3
void Enemy3::Initialize(CMPUT350::GameContext* context){
	Enemy::Initialize(context);
	mLives = 3;
}

void Enemy3::LateUpdate(CMPUT350::GameContext* context){
	Enemy::LateUpdate(context);
}

bool Enemy3::Attack(CMPUT350::GameContext* context, const CMPUT350::Point2D &target){
	if (!isInFormation()){
		return false;
	}

	mPath = {
        pos,
		{pos.x + 100.f, pos.y + 1400.f},
		{target.x - 200.f, target.y + 200.f},
		mStopPosition
    };
	t = 0.0f;
	return true;
}

void Enemy3::RenderForeground(CMPUT350::GameContext *context){
	Enemy::RenderForeground(context);
}