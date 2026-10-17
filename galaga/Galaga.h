#ifndef GALAGA_H
#define GALAGA_H

#include "GameObject.h"
#include "GameContext.h"
#include "Player.h"
#include "UICounter.h"
#include "Enemy.h"


enum class GameState {
    menu, startup, waveEnter, play, gameOver
};

class Galaga : public CMPUT350::GameObject {
    GameState state;
    int wave;
    std::vector<std::shared_ptr<Enemy>> enemies;
    std::shared_ptr<UICounter> scoreCtr;
    std::shared_ptr<UICounter> waveCtr;
    sf::Clock clock;
    int coins = 0;
    bool showCoinMessage = false;
    int numEnemies = 0;
    int spawnTimer =0;

    void SetState(GameState s);
    std::shared_ptr<Enemy> SpawnEnemy(CMPUT350::GameContext *context, int enemyNumber);

public:
    Galaga();

    void Initialize(CMPUT350::GameContext *context) override;
    void Update(CMPUT350::GameContext *context) override;
    void RenderUI(CMPUT350::GameContext *context) override;
    void ReceiveNotification(const std::string& key) override;
};

#endif