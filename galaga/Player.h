#ifndef PLAYER_H
#define PLAYER_H

#include "CollisionObject.h"
#include "Bullet.h"

class Player : public CMPUT350::CollisionObject
{
private:
    CMPUT350::Point2D mLocation;
    CMPUT350::Rect mBounds;
    std::vector<CMPUT350::Shape> mShapes;
    int mSize = 40;
    int mBulletCooldown = 0;
    bool mAlive;
    bool mMoveLeft;
    bool mMoveRight;
    bool mShoot;

    std::weak_ptr<Bullet> bullet1;
    std::weak_ptr<Bullet> bullet2;
    
    sf::Clock clock;
    
public:
    Player(CMPUT350::Point2D loc);

    // GameObject Functions
    void Initialize(CMPUT350::GameContext* context) override;
    void Update(CMPUT350::GameContext* context) override;
    void LateUpdate(CMPUT350::GameContext* context) override;
    bool IsAlive() const override;
    void Kill() override;
    void ReceiveNotification(const std::string& key) override;

    // Graphics Object Functions
    void RenderBackground(CMPUT350::GameContext* context) override;
    void RenderForeground(CMPUT350::GameContext* context) override;


    // Collision Object Functions
    void CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj) override;
    const CMPUT350::Rect& GetBounds() override;
    const std::vector<CMPUT350::Shape> &GetShapes() override;
    CMPUT350::Point2D GetLocation() const override;
    float GetRotation() const override;

};

#endif
