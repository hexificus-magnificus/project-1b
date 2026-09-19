#include "GameEngine.h"

/// @brief
namespace CMPUT350 {
#include "FontData.h"

GameEngine::GameEngine(unsigned int width, unsigned int height, const std::string& name)
	: mFont(std::make_shared<sf::Font>()),
	  mWindow(std::make_shared<sf::RenderWindow>(sf::VideoMode({width, height}), name))
{
    //	need to:
    //		load font
    //		create window
    //		initialize GameContext
    if (!mFont->openFromMemory(&_font, _font_len)) {
    	fprintf(stderr, "WARNING: Font did not load.\n");
    }

    ctx->mEngineView = this;
    ctx->ScreenContext = new DrawContext(mWindow, mFont);

    mWindow->setFramerateLimit(30);
}

GameEngine::~GameEngine() {
    // Cleanup resources
    // mWindow->close();
    mWindow->close();
    delete ctx->ScreenContext;
    // do shared_ptrs get destroyed with the object?
    // i'm worried they won't decrement the ctrl block here
    //delete mFont;
    //delete mWindow;
}

void GameEngine::AddGameObject(std::shared_ptr<GameObject> gameObject) {
	newObjects.push_back(gameObject);
}

/**
 * @method Run
 * @arguments None
 * @description Gives control to the game engine. Will not return until the game window is closed or
 * all objects have been destroyed.
 */
void GameEngine::Run() {
    while (true)  // window is open
    {
        // 0. Remove any objects that are now dead
	RemoveDead();

        // 1. Activate and initialize any objects added during the last frame
	int newCount = newObjects.size();
	for (int i = 0; i < newCount; i++) {
		gameObjects.push_back(newObjects.back());
		newObjects.pop_back();
	}

        // 2. Process events
	ProcessEvents(ctx);

        // 3. Update game objects
	for (int i = 0; i < gameObjects.size(); i++) {
		gameObjects.at(i)->Update(ctx);
	}

        // 4. Process collision events

        // 5. Late updates
	for (int i = 0; i < gameObjects.size(); i++) {
		gameObjects.at(i)->LateUpdate(ctx);
	}

        // Clear window
	mWindow->clear(sf::Color::Black);

        // 6. Render background
	for (int i = 0; i < gameObjects.size(); i++) {
		// if the object is a GraphicsObject, call draw() on it 
	}

        // 7. Render foreground

        // Actually render to window
    }
}

// Sample code for processing events

bool GameEngine::ProcessEvents(GameContext *context)
{
	while (const std::optional event = mWindow->pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
		}
		else if (event->is<sf::Event::Resized>())
		{
		}
		else if (const auto* keyPressed = event->getIf<sf::Event::TextEntered>())
		{
			// use keyPressed->unicode to get character
		}
	}
	return true;
}

void GameEngine::RemoveDead() {
    std::vector<std::shared_ptr<GameObject>> newLive;
    for (int i = 0; i < gameObjects.size(); i++) {
    	if (gameObjects.at(i)->IsAlive()) {
		newLive.push_back(gameObjects.at(i));
	}
    }
    gameObjects = std::move(newLive);
}

}  // namespace CMPUT350
