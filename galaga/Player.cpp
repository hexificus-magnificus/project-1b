#include <cassert>
#include "Player.h"
#include "Bullet.h"
#include "NotificationManager.h"

Player::Player(CMPUT350::Point2D loc)
    : mMoveLeft(false), mMoveRight(false), mShoot(false), mLives(3), mActive(true), canEnter(true), needDie(false)
{
    mLocation = mSpawn = loc;
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
    if (!mActive) {
        if (canEnter) {
            mLocation = mSpawn;
            mActive = true;
        } else {
            return;
        }
    }
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
    if (needDie && mAlive) {
        context->mNotificationManager->Notify("PlayerDead");
        Kill();
    }
}

/**
* @brief Handling notifications being received
*/ 
void Player::ReceiveNotification(const std::string& key){
    if (!mActive)
        return;
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

void Player::DrawShip(CMPUT350::DrawContext *ctx, CMPUT350::Point2D p) {
    CMPUT350::Rect center = CMPUT350::Rect(p.x-2, p.y-22, 4, 52);
    CMPUT350::Rect centerLower = CMPUT350::Rect(p.x-4, p.y-10, 8, 37);
    CMPUT350::Rect centerLowest = CMPUT350::Rect(p.x-8, p.y-4, 16, 26);
    //wings
    int wingWidth = 4;
    int wingHeight = 14;
    int wingOffsetY = 4;
    int wingCount = 5;
    int wingStartOffsetX = 8;
    int wingSpacing = 4;
    for (int i = 0; i <wingCount;i++){
        CMPUT350::Rect leftWing = CMPUT350::Rect(p.x-wingStartOffsetX - i * wingWidth, p.y+wingOffsetY + i * wingSpacing,wingWidth,wingHeight);
        CMPUT350::Rect rightWing = CMPUT350::Rect(p.x+wingWidth + i * wingWidth, p.y+wingOffsetY + i * wingSpacing,wingWidth,wingHeight);
        ctx->DrawRect(leftWing, CMPUT350::Colors::white);
        ctx->DrawRect(rightWing, CMPUT350::Colors::white);
    }

    CMPUT350::Rect tip = CMPUT350::Rect(p.x-2, p.y-22, 4, 6);
   
    ctx->DrawRect(center, CMPUT350::Colors::white);
    ctx->DrawRect(centerLower, CMPUT350::Colors::white);
    ctx->DrawRect(centerLowest, CMPUT350::Colors::white);
    ctx->DrawRect(tip, CMPUT350::Colors::magenta);
    CMPUT350::Rect wingEndLeft = CMPUT350::Rect(p.x-24, p.y, 4, 20);
    CMPUT350::Rect wingEndRight = CMPUT350::Rect(p.x+20, p.y, 4, 20);
    CMPUT350::Rect wingEndLeftTip = CMPUT350::Rect(p.x-24, p.y, 4, 8);
    CMPUT350::Rect wingEndRightTip = CMPUT350::Rect(p.x+20, p.y, 4, 8);
    CMPUT350::Rect engineLeft = CMPUT350::Rect(p.x-12, p.y+22, 6, 8);
    CMPUT350::Rect engineRight = CMPUT350::Rect(p.x+6, p.y+22, 6, 8);
    CMPUT350::Rect centerDesign = CMPUT350::Rect(p.x-1.5, p.y, 3, 8);
    CMPUT350::Rect centerDesignLeft = CMPUT350::Rect(p.x-4.5, p.y+3, 3, 8);
    CMPUT350::Rect centerDesignRight = CMPUT350::Rect(p.x+1.5, p.y+3, 3, 8);
   
    ctx->DrawRect(engineLeft, CMPUT350::Colors::magenta);
    ctx->DrawRect(engineRight, CMPUT350::Colors::magenta);
    ctx->DrawRect(wingEndLeft, CMPUT350::Colors::white);
    ctx->DrawRect(wingEndRight, CMPUT350::Colors::white);
    ctx->DrawRect(wingEndLeftTip, CMPUT350::Colors::magenta);
    ctx->DrawRect(wingEndRightTip, CMPUT350::Colors::magenta);
    ctx->DrawRect(centerDesign, CMPUT350::Colors::magenta);
    ctx->DrawRect(centerDesignLeft, CMPUT350::Colors::magenta);
    ctx->DrawRect(centerDesignRight, CMPUT350::Colors::magenta);
}

/**
* @brief Draw spaceship
*/ 
void Player::RenderForeground(CMPUT350::GameContext* context)
{
    if (!mActive)
        return;
    DrawShip(context->ScreenContext, {0, 0});
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
            mActive = false;
            if (--mLives < 0) {
                needDie = true;
            }
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

void Player::RenderUI(CMPUT350::GameContext *context) {
    for (int i = 0; i < mLives; i++) {
        float x = 730 - i*(mSize+20);
        float y = 5 + mSize/2;
        DrawShip(context->GUIContext, {x, y});
    }
}