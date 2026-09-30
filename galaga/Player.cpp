#include <cassert>
#include "Player.h"
#include "Bullet.h"

Player::Player(CMPUT350::Point2D loc)
{
    // TODO: Update code
    mLocation = loc;
    mBounds.topLeft = mLocation - (mSize/2);
    mBounds.height = mSize;
    mBounds.width = mSize;
    mAlive = true;

    mBulletCooldown = 0;

}

void Player::Initialize(CMPUT350::GameContext* context)
{
}

void Player::Update(CMPUT350::GameContext* context)
{
    mBulletCooldown--;
}

void Player::LateUpdate(CMPUT350::GameContext* context)
{
}

bool Player::HandleKeyEvent(CMPUT350::GameContext* context, char key)
{
    int speed = 10;
    if (key=='a'){
        mLocation.x -= speed;
        mBounds.topLeft.x -= speed;
    }
    if (key=='d'){
        mLocation.x += speed;
        mBounds.topLeft.x += speed;
    }
    if (key == ' '){
        if (bullet1.expired() || bullet2.expired()){
            CMPUT350::Point2D direction = CMPUT350::Point2D(0, -10);
            std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>(mLocation, direction, true);
            context->mEngineView->AddGameObject(bullet);
            if (bullet1.expired()){
                bullet1 = bullet;
            }else{
                bullet2 = bullet;
            }
            mBulletCooldown = 5;
        }
        
        
    }

    return true;
}

void Player::RenderBackground(CMPUT350::GameContext* context)
{
}

void Player::RenderForeground(CMPUT350::GameContext* context)
{
    //context->ScreenContext->DrawRect(mBounds, CMPUT350::Colors::red);
    CMPUT350::Rect body({-5, 19}, {5, -21});
    CMPUT350::Rect cockpit({-2, 5}, {2, 10});
    CMPUT350::Rect wingL({-5, 0}, {-15, 10});
    CMPUT350::Rect wingtipL({-18, -10}, {-15, 15});
    CMPUT350::Rect wingR({5, 0}, {15, 10});
    CMPUT350::Rect wingtipR({15, -10}, {18, 15});

    context->ScreenContext->DrawRect(body + mLocation, CMPUT350::Colors::cyan);
    context->ScreenContext->DrawRect(cockpit + mLocation, CMPUT350::Colors::white);
    context->ScreenContext->DrawRect(wingL + mLocation, CMPUT350::Colors::white);
    context->ScreenContext->DrawRect(wingtipL + mLocation, CMPUT350::Colors::cyan);
    context->ScreenContext->DrawRect(wingR + mLocation, CMPUT350::Colors::white);
    context->ScreenContext->DrawRect(wingtipR + mLocation, CMPUT350::Colors::cyan);
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
    mAlive = false;
}

bool Player::IsAlive() const
{
    // TODO: Update code
    return mAlive;
}

const CMPUT350::Rect& Player::GetBounds()
{
    // TODO: Update code
    return mBounds;
}
