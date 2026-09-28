#include <cassert>
#include "Player.h"
#include "Bullet.h"

Player::Player(CMPUT350::Point2D loc)
{
    // TODO: Update code
}

void Player::Initialize(CMPUT350::GameContext* context)
{
}

void Player::Update(CMPUT350::GameContext* context)
{
}

void Player::LateUpdate(CMPUT350::GameContext* context)
{
}

bool Player::HandleKeyEvent(CMPUT350::GameContext* context, char key)
{
    return true;
}

void Player::RenderBackground(CMPUT350::GameContext* context)
{
}

void Player::RenderForeground(CMPUT350::GameContext* context)
{
}

void Player::CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj)
{
    //inspiration for code from: https://stackoverflow.com/a/351865
    Bullet* bullet = dynamic_cast<Bullet*>(obj.get());
    if (bullet != nullptr){
        if (!bullet->IsPlayerBullet()){
            Kill();
        }
    }
}

void Player::Kill()
{
}

bool Player::IsAlive() const
{
    // TODO: Update code
    return true;
}

const CMPUT350::Rect& Player::GetBounds()
{
    // TODO: Update code
    static CMPUT350::Rect sBounds(0, 0, 0, 0);
    return sBounds;
}
