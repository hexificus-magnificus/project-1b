#include <cassert>
#include "Player.h"
#include "Bullet.h"
#include "NotificationManager.h"

Player::Player(CMPUT350::Point2D loc) : mMoveLeft(false), mMoveRight(false)
{
    mLocation = loc;
    mBounds.topLeft = mLocation - (mSize/2);
    mBounds.height = mSize;
    mBounds.width = mSize;
    mAlive = true;
    mShapes = std::vector<CMPUT350::Shape>(1);
    mShapes[0] = CMPUT350::Shape(mBounds);
}

void Player::Initialize(CMPUT350::GameContext* context)
{
    context->mNotificationManager->Register(context->CurrObject, "MoveLeft");
    context->mNotificationManager->Register(context->CurrObject, "MoveRight");
    context->mNotificationManager->Register(context->CurrObject, "Shoot");
    context->mNotificationManager->Register(context->CurrObject, "MoveLeftStop");
    context->mNotificationManager->Register(context->CurrObject, "MoveRightStop");
    clock.reset();
}

void Player::Update(CMPUT350::GameContext* context)
{
    mBulletCooldown--;

    float dt = clock.restart().asMicroseconds() / 1000.;
    float speed = 0.3 * dt;
    if (mMoveLeft && !mMoveRight) {
        if (mLocation.x -speed -(mBounds.width/2) >0){
            mLocation.x -= speed;
            mBounds.topLeft.x -= speed;
            mShapes[0] = CMPUT350::Shape(mBounds);
        }
    }
    if (mMoveRight && !mMoveLeft) {
        if (mLocation.x + speed+(mBounds.width/2) <768){
            mLocation.x += speed;
            mBounds.topLeft.x += speed;
            mShapes[0] = CMPUT350::Shape(mBounds);
        }
    }
    if (mShoot) {
        if ((bullet1.expired() || bullet2.expired()) && mBulletCooldown <= 0){
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
        mShoot = false;
    }
}

void Player::LateUpdate(CMPUT350::GameContext* context)
{
}

/**
* @brief Handling notifications being received
*/ 
void Player::ReceiveNotification(const std::string& key){
    int speed = 10;
    if (key == "MoveLeft"){
        mMoveLeft = true;
    }else if (key == "MoveRight"){
        mMoveRight = true;
    } else if (key == "MoveLeftStop") {
        mMoveLeft = false;
    } else if (key == "MoveRightStop") {
        mMoveRight = false;
    }else if (key == "Shoot"){
        mShoot = true;
    }
}
void Player::RenderBackground(CMPUT350::GameContext* context)
{
}

/**
* @brief Draw spaceship
*/ 
void Player::RenderForeground(CMPUT350::GameContext* context)
{
    //context->ScreenContext->DrawRect(mBounds, CMPUT350::Colors::red);
    /*
    CMPUT350::Rect body({-5.0f, 19.0f}, {5.0f, -21.0f});
    CMPUT350::Rect cockpit({-2.0f, 5.0f}, {2.0f, 10.0f});
    CMPUT350::Rect wingL({-5.0f, 0.0f}, {-15.0f, 10.0f});
    CMPUT350::Rect wingtipL({-18.0f, -10.0f}, {-15.0f, 15.0f});
    CMPUT350::Rect wingR({5.0f, 0.0f}, {15.0f, 10.0f});
    CMPUT350::Rect wingtipR({15.0f, -10.0f}, {18.0f, 15.0f});

    context->ScreenContext->DrawRect(body + mLocation, CMPUT350::Colors::cyan);
    context->ScreenContext->DrawRect(cockpit + mLocation, CMPUT350::Colors::white);
    context->ScreenContext->DrawRect(wingL + mLocation, CMPUT350::Colors::white);
    context->ScreenContext->DrawRect(wingtipL + mLocation, CMPUT350::Colors::cyan);
    context->ScreenContext->DrawRect(wingR + mLocation, CMPUT350::Colors::white);
    context->ScreenContext->DrawRect(wingtipR + mLocation, CMPUT350::Colors::cyan);
    */

    CMPUT350::Point2D centerPos(0.f, 0.f);
    //!!!!!!!!!!!!!!!!!!!!! version 2 ship I didn't realize you already did one
    CMPUT350::Rect center = CMPUT350::Rect(-2, -22, 4, 52);
    CMPUT350::Rect centerLower = CMPUT350::Rect(-4, -10, 8, 37);
    CMPUT350::Rect centerLowest = CMPUT350::Rect(-8, -4, 16, 26);
    //wings
    int wingWidth = 4;
    int wingHeight = 14;
    int wingOffsetY = 4;
    int wingCount = 5;
    int wingStartOffsetX = 8;
    int wingSpacing = 4;
    for (int i = 0; i <wingCount;i++){
        CMPUT350::Rect leftWing = CMPUT350::Rect(- wingStartOffsetX - i * wingWidth,wingOffsetY + i * wingSpacing,wingWidth,wingHeight);
        CMPUT350::Rect rightWing = CMPUT350::Rect(wingWidth + i * wingWidth,wingOffsetY + i * wingSpacing,wingWidth,wingHeight);
        context->ScreenContext->DrawRect(leftWing, CMPUT350::Colors::white);
        context->ScreenContext->DrawRect(rightWing, CMPUT350::Colors::white);
    }

    CMPUT350::Rect tip = CMPUT350::Rect(-2, -22, 4, 6);
   
    context->ScreenContext->DrawRect(center, CMPUT350::Colors::white);
    context->ScreenContext->DrawRect(centerLower, CMPUT350::Colors::white);
    context->ScreenContext->DrawRect(centerLowest, CMPUT350::Colors::white);
    context->ScreenContext->DrawRect(tip, CMPUT350::Colors::magenta);
    CMPUT350::Rect wingEndLeft = CMPUT350::Rect(-24, 0, 4, 20);
    CMPUT350::Rect wingEndRight = CMPUT350::Rect(20, 0, 4, 20);
    CMPUT350::Rect wingEndLeftTip = CMPUT350::Rect(-24, 0, 4, 8);
    CMPUT350::Rect wingEndRightTip = CMPUT350::Rect(20, 0, 4, 8);
    CMPUT350::Rect engineLeft = CMPUT350::Rect(-12, 22, 6, 8);
    CMPUT350::Rect engineRight = CMPUT350::Rect(6, 22, 6, 8);
    CMPUT350::Rect centerDesign = CMPUT350::Rect(-1.5, 0, 3, 8);
    CMPUT350::Rect centerDesignLeft = CMPUT350::Rect(-4.5, 3, 3, 8);
    CMPUT350::Rect centerDesignRight = CMPUT350::Rect(1.5, 3, 3, 8);
   
    context->ScreenContext->DrawRect(engineLeft, CMPUT350::Colors::magenta);
    context->ScreenContext->DrawRect(engineRight, CMPUT350::Colors::magenta);
    context->ScreenContext->DrawRect(wingEndLeft, CMPUT350::Colors::white);
    context->ScreenContext->DrawRect(wingEndRight, CMPUT350::Colors::white);
    context->ScreenContext->DrawRect(wingEndLeftTip, CMPUT350::Colors::magenta);
    context->ScreenContext->DrawRect(wingEndRightTip, CMPUT350::Colors::magenta);
    context->ScreenContext->DrawRect(centerDesign, CMPUT350::Colors::magenta);
    context->ScreenContext->DrawRect(centerDesignLeft, CMPUT350::Colors::magenta);
    context->ScreenContext->DrawRect(centerDesignRight, CMPUT350::Colors::magenta);
    
}

/**
* @brief Handles collisions with other collision objects
*/ 
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
    return mAlive;
}

const CMPUT350::Rect& Player::GetBounds()
{
    return mBounds;
}

const std::vector<CMPUT350::Shape> &Player::GetShapes() {
    return mShapes;
}

CMPUT350::Point2D Player::GetLocation() const{
    return mLocation;
};
float Player::GetRotation() const {
    return 0;
};

