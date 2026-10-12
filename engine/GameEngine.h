
#ifndef GAMEENGINE_H
#define GAMEENGINE_H

namespace CMPUT350 {
class GameEngine;
}

#include <vector>
#include <memory> // for dynamic_pointer_cast

#include "GameContext.h"
#include "EngineView.h"
#include "GameObject.h"
#include "CollisionObject.h"
#include "GraphicsObject.h"
#include "MathUtil.h"
#include <SFML/Graphics.hpp>

namespace CMPUT350 {

class DrawContext;

class GameEngine : public EngineView {
public:
    GameEngine(unsigned int width, unsigned int height, const std::string& name);
    ~GameEngine();

    GameEngine(const GameEngine&) = delete;             // Prevent copy-construction
    GameEngine(GameEngine&&) = delete;                  // Prevent move-construction
    GameEngine& operator=(const GameEngine&) = delete;  // Prevent assignment
    GameEngine& operator=(GameEngine&&) = delete;       // Prevent move-assignment

    void AddGameObject(std::shared_ptr<GameObject> gameObject) override;
    virtual void InstallKeyDownNotification(int, const std::string &notification);
	virtual void InstallKeyUpNotification(int, const std::string &notification);
	virtual const_iterator cbegin() const;
	virtual const_iterator cend() const;
	virtual const_iterator begin() const;
	virtual const_iterator end() const;

    void Run();

private:
    void RemoveDead();
    bool ProcessEvents(GameContext *context);
    std::shared_ptr<GraphicsObject> IsGraphicsObject(std::shared_ptr<GameObject> obj);

    std::shared_ptr<sf::RenderWindow> mWindow;
    std::shared_ptr<sf::Font> mFont;
    std::vector<std::shared_ptr<GameObject>> gameObjects;
    std::vector<std::shared_ptr<GameObject>> newObjects;
    GameContext *ctx;
};

}  // namespace CMPUT350

#endif  // GAMEENGINE_H
