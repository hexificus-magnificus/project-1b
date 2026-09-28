#include "Bullet.h"
#include "Player.h"

Bullet::Bullet(CMPUT350::Point2D location, CMPUT350::Point2D heading, bool player)
{
    mLocation = location;
    mPreviousLocation = location;
    mHeading = heading;
    mAlive = true;
    mPlayer = player;

    mBounds = CMPUT350::Rect(mLocation, mLocation);
    mBounds.Inset(-2);

}

bool Bullet::IsPlayerBullet()
{
    return mPlayer;
}

void Bullet::Initialize(CMPUT350::GameContext* context)
{  
}

void Bullet::Update(CMPUT350::GameContext* context)
{
    
    mPreviousLocation = mLocation;
    mLocation += mHeading; 
    mBounds.topLeft = mPreviousLocation;
    mBounds.width = 0;
    mBounds.height = 0;
    mBounds |= mLocation;
    mBounds.Inset(-2);

    if (mLocation.y <0){
        Kill();
    }

}

void Bullet::LateUpdate(CMPUT350::GameContext* context)
{
}

bool Bullet::HandleKeyEvent(CMPUT350::GameContext* context, char key)
{
    return false;
}

void Bullet::RenderBackground(CMPUT350::GameContext* context)
{
}

void Bullet::RenderForeground(CMPUT350::GameContext* context)
{
    context->ScreenContext->FrameRect(mBounds,1,CMPUT350::Colors::yellow);
}

void Bullet::CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj)
{
    //inspiration for code from: https://stackoverflow.com/a/351865

    Player* player = dynamic_cast<Player*>(obj.get());
    if (player != nullptr && IsPlayerBullet()){
        return;
    }
    Kill();
}

void Bullet::Kill()
{
    mAlive = false;
}

bool Bullet::IsAlive() const
{
    // TODO: Update code
    return mAlive;
}

const CMPUT350::Rect& Bullet::GetBounds()
{
    // TODO: Update code
    // static CMPUT350::Rect sBounds(0, 0, 0, 0);
    // return sBounds;
    return mBounds;
}
