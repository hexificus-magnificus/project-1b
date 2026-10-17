#ifndef ENEMY_H
#define ENEMY_H

#include "CollisionObject.h"
#include "GameContext.h"
#include "MathUtil.h"

class Enemy : public CMPUT350::CollisionObject
{
public:
    Enemy(CMPUT350::Point2D loc, const std::vector<CMPUT350::Point2D>& path, CMPUT350::Point2D stopPosition);

    // GameObject Functions
    void Initialize(CMPUT350::GameContext* context) override;
    void Update(CMPUT350::GameContext* context) override;
    void LateUpdate(CMPUT350::GameContext* context) override;
    bool IsAlive() const override;
    void Kill() override;
    virtual bool Attack(CMPUT350::GameContext *context, const CMPUT350::Point2D &target) = 0;
    virtual CMPUT350::Point2D GetLocation() const override;
	virtual float GetRotation() const override;
	
	

    // Graphics Object Functions
    void RenderBackground(CMPUT350::GameContext* context) override;
    void RenderForeground(CMPUT350::GameContext* context) override;


    // Collision Object Functions
    void CollisionEnter(const std::shared_ptr<CMPUT350::CollisionObject>& obj) override;
    const CMPUT350::Rect& GetBounds() override;
    const std::vector<CMPUT350::Shape> &GetShapes() override;
	bool isInFormation() const;
	std::vector<CMPUT350::Point2D> mPath;
	CMPUT350::Point2D pos;
	float t = 0.0f;
	CMPUT350::Point2D mStopPosition;
	int mLives;
	CMPUT350::RGBColor darkColor = CMPUT350::Colors::red;
	CMPUT350::RGBColor mediumColor =CMPUT350::Colors::red;
	CMPUT350::RGBColor lightColor = CMPUT350::Colors::red;
	CMPUT350::RGBColor color = CMPUT350::Colors::red;
	void Shoot(CMPUT350::GameContext* context);
	int mPoints = 0;
	int addScore = 0;



private:
	CMPUT350::Rect bounds;
    std::vector<CMPUT350::Shape> mShapes;
	bool alive;
    CMPUT350::Point2D mStartPos;
    float rotation = 0.0f;
	sf::Clock clock;
};

class Enemy1 : public Enemy {
public:
	Enemy1(CMPUT350::Point2D loc, const std::vector<CMPUT350::Point2D> &path, CMPUT350::Point2D stopPosition)
	:Enemy(loc, path, stopPosition){
		lightColor = CMPUT350::Colors::red;
		color = lightColor;
		mPoints = 50;
	}
	void Initialize(CMPUT350::GameContext *context) override;
	void LateUpdate(CMPUT350::GameContext *context) override;
	bool Attack(CMPUT350::GameContext *context, const CMPUT350::Point2D &target) override;
	void RenderForeground(CMPUT350::GameContext *context) override;
	

};

class Enemy2 : public Enemy {
public:
	Enemy2(CMPUT350::Point2D loc, const std::vector<CMPUT350::Point2D> &path, CMPUT350::Point2D stopPosition)
	:Enemy(loc, path, stopPosition){
		mediumColor = CMPUT350::Colors::mediumGreen;
		lightColor = CMPUT350::Colors::lightGreen;
		color = mediumColor;
		mPoints = 100;
	}
	void Initialize(CMPUT350::GameContext *context) override;
	void LateUpdate(CMPUT350::GameContext *context) override;
	bool Attack(CMPUT350::GameContext *context, const CMPUT350::Point2D &target) override;
	void RenderForeground(CMPUT350::GameContext *context) override;
	
};

class Enemy3 : public Enemy {
public:
	Enemy3(CMPUT350::Point2D loc, const std::vector<CMPUT350::Point2D> &path, CMPUT350::Point2D stopPosition)
	:Enemy(loc, path, stopPosition){
		darkColor = CMPUT350::Colors::darkBlue;
		mediumColor = CMPUT350::Colors::mediumBlue;
		lightColor = CMPUT350::Colors::lightBlue;
		mPoints = 200;
	}
	void Initialize(CMPUT350::GameContext *context) override;
	void LateUpdate(CMPUT350::GameContext *context) override;
	bool Attack(CMPUT350::GameContext *context, const CMPUT350::Point2D &target) override;
	void RenderForeground(CMPUT350::GameContext *context) override;
	
};

#endif

