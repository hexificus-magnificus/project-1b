#include "Galaga.h"

void Galaga::SetState(GameState s) {
    if (s == GameState::gameOver) {
        clock.restart();
    }
    if (s == GameState::menu) {
        if (state == GameState::gameOver||state == GameState::play) {
            // remove score/wave counters
            scoreCtr->Kill();
            waveCtr->Kill();

            // swap pointers out of member vars so they can be deconstructed properly
            std::shared_ptr<UICounter> sTemp = std::move(scoreCtr);
            std::shared_ptr<UICounter> wTemp = std::move(waveCtr);
            sTemp->Kill();
            wTemp->Kill();

            scoreCtr = nullptr;
            waveCtr = nullptr;
            
            //debugged with ChatGPT
            for (auto& enemy : enemies) {
                if (enemy) {
                    enemy->Kill();
                }
            }

            enemies.clear();
        }
        wave = 0;
    }
    if (s == GameState::waveEnter) {
        std::cout << "waveEnter\n";
        ++wave;
        ++*waveCtr;
        numEnemies = 0;
        spawnTimer = 0;
        attackTimer = 0;
        waveTimer = 0;
        player->canEnter = false;
        std::cout << "done waveEnter";
    }
    if (s == GameState::play) {
        player->canEnter = true;
    }
    if (s == GameState::startup) {
    }
    state = s;
}


std::shared_ptr<Enemy> Galaga::SpawnEnemy(
    //debugged and formatted with ChatGPT
    CMPUT350::GameContext* context,
        int enemyNumber)
    {
        if (wave < 1 || wave > 5 || enemyNumber < 0 || enemyNumber >= 8) {
            return nullptr;
        }

        CMPUT350::Point2D stopPosition;
        std::vector<CMPUT350::Point2D> path;

        // Separate each wave vertically
        float x = 170.f + enemyNumber * 60.f;
        float y = 120.f + (wave - 1) * 140.f;

        stopPosition = {x, y};

        if (wave == 1) {
            path = {
                {40.f + enemyNumber * 10.f, 40.f},
                {180.f, 280.f},
                {550.f, 230.f},
                stopPosition
            };

            return std::make_shared<Enemy1>(
                path[0], path, stopPosition
            );
        }
        else if (wave == 2) {
            path = {
                {728.f - enemyNumber * 10.f, 40.f},
                {580.f, 320.f},
                {180.f, 220.f},
                stopPosition
            };

            return std::make_shared<Enemy2>(
                path[0], path, stopPosition
            );
        }
        else if (wave == 3) {
            path = {
                {340.f + enemyNumber * 12.f, 40.f},
                {140.f, 160.f},
                {620.f, 480.f},
                stopPosition
            };

            return std::make_shared<Enemy1>(
                path[0], path, stopPosition
            );
        }
        else if (wave == 4) {
            path = {
                {40.f + enemyNumber * 8.f, 720.f},
                {180.f, 260.f},
                {570.f, 150.f},
                stopPosition
            };

            return std::make_shared<Enemy2>(
                path[0], path, stopPosition
            );
        }
        else if (wave == 5) {
            path = {
                {728.f - enemyNumber * 8.f, 720.f},
                {600.f, 250.f},
                {160.f, 150.f},
                stopPosition
            };

            return std::make_shared<Enemy3>(
                path[0], path, stopPosition
            );
        }

        return nullptr;
    }

Galaga::Galaga() : state(GameState::menu), wave(0), scoreCtr(nullptr), waveCtr(nullptr), enemies() {}

void Galaga::Initialize(CMPUT350::GameContext *context) {
    clock.reset();
    SetState(GameState::menu);
    context->mEngineView->InstallKeyDownNotification(static_cast<int>(sf::Keyboard::Key::A), "MoveLeft");
    context->mEngineView->InstallKeyDownNotification(static_cast<int>(sf::Keyboard::Key::D), "MoveRight");
    context->mEngineView->InstallKeyDownNotification(static_cast<int>(sf::Keyboard::Key::Space), "Shoot");
    context->mEngineView->InstallKeyDownNotification(static_cast<int>(sf::Keyboard::Key::Num5), "InsertCoins");
    context->mEngineView->InstallKeyDownNotification(static_cast<int>(sf::Keyboard::Key::Num1), "StartGame");
    context->mEngineView->InstallKeyUpNotification(static_cast<int>(sf::Keyboard::Key::A), "MoveLeftStop");
    context->mEngineView->InstallKeyUpNotification(static_cast<int>(sf::Keyboard::Key::D), "MoveRightStop");
    context->mNotificationManager->Register(context->CurrObject, "InsertCoins");
    context->mNotificationManager->Register(context->CurrObject, "StartGame");
    context->mNotificationManager->Register(context->CurrObject, "PlayerDead");
    context->mNotificationManager->Register(context->CurrObject, "Score50");
    context->mNotificationManager->Register(context->CurrObject, "Score100");
    context->mNotificationManager->Register(context->CurrObject, "Score200");
}

void Galaga::Update(CMPUT350::GameContext *context) {
    waveTimer++;
    switch (state) {
        case GameState::menu:
            break;
        case GameState::startup:
            scoreCtr = std::make_shared<UICounter>("Score: %d", 0, CMPUT350::Point2D(10, 10));
            waveCtr = std::make_shared<UICounter>("Wave: %d", 0, CMPUT350::Point2D(10, 900));
            player = std::make_shared<Player>(CMPUT350::Point2D(768 / 2, 900));
            context->mEngineView->AddGameObject(player);
            context->mEngineView->AddGameObject(scoreCtr);
            context->mEngineView->AddGameObject(waveCtr);
            SetState(GameState::waveEnter);
            break;
        case GameState::waveEnter:
            spawnTimer++;
            if (spawnTimer >= 20 && numEnemies<8){
                std::cout << "spawning enemy\n";
                //ChatGPT help with the next three lines
                auto enemy = SpawnEnemy(context, numEnemies);
                enemies.push_back(enemy);
                context->mEngineView->AddGameObject(enemy);
                spawnTimer = 0;
                numEnemies++;
            }
            if (numEnemies ==8){
                SetState(GameState::play);
            }
            
            break;
        case GameState::play: {
            //debugged with ChatGPT
            bool enemiesDead = true;

            for (const auto& enemy : enemies) {
                if (enemy && enemy->IsAlive()) {
                    enemiesDead = false;
                    break;
                }
            }

            // Start the next wave after 360 frames,
            if (wave < 5 && waveTimer >= 360) {
                SetState(GameState::waveEnter);
                break;
            }

            // After wave 5, finish only when all enemies are dead.
            if (wave == 5 && enemiesDead) {
                SetState(GameState::menu);
                break;
            }

            // Keep attacking with enemies from ALL waves.
            attackTimer++;

            if (attackTimer >= 30) {
                attackTimer = 0;

                for (int i = 0; i < enemies.size(); i++) {
                    int attacker = (nextAttacker + i) % enemies.size();

                    if (enemies[attacker] &&
                        enemies[attacker]->IsAlive() &&
                        enemies[attacker]->isInFormation() &&
                        enemies[attacker]->Attack(
                            context, player->GetLocation())) {

                        std::cout << "Enemy " << attacker
                                << " attacks!" << std::endl;

                        nextAttacker = (attacker + 1) % enemies.size();
                        break;
                    }
                }
            }

            break;
        }
            
        case GameState::gameOver:
            if (clock.getElapsedTime() > sf::seconds(5)) {
                SetState(GameState::menu);
            }
            break;
    }
}
void Galaga::RenderUI(CMPUT350::GameContext *context){
    if(state == GameState::menu){
        context->GUIContext->DrawCenteredText("GALAGA",  48,CMPUT350::Point2D(768/2, 400), CMPUT350::Colors::white);
        context->GUIContext->DrawCenteredText("Press 5 to insert coins", 24,CMPUT350::Point2D(768/2, 500),  CMPUT350::Colors::white);
        context->GUIContext->DrawCenteredText("Press 1 to start game", 24,CMPUT350::Point2D(768/2, 550),  CMPUT350::Colors::white);
        context->GUIContext->DrawCenteredText("Coins: " + std::to_string(coins), 24,CMPUT350::Point2D(768/2, 650),  CMPUT350::Colors::white);
        if (showCoinMessage){
            context->GUIContext->DrawCenteredText("Not enough coins!", 24,CMPUT350::Point2D(768/2, 600),  CMPUT350::Colors::white);
        }
    } else if (state == GameState::gameOver) {
        context->GUIContext->DrawCenteredText("Game Over",  48,CMPUT350::Point2D(768/2, 400), CMPUT350::Colors::white);
    }
}

void Galaga::ReceiveNotification(const std::string& key){
    if (state == GameState::menu){
        if (key =="InsertCoins"){
            showCoinMessage = false;
            coins++;
        }
        else if (key == "StartGame"){
            if (coins > 0){
                coins--;
                SetState(GameState::startup);
                showCoinMessage = false;
            }else{
                showCoinMessage = true;
            }
        }
    }
    if (key == "PlayerDead") {
        std::cout << "player died\n";
        SetState(GameState::gameOver);
    }else if(key=="Score50"||key=="Score100"||key=="Score200"){
        int points = std::stoi(key.substr(5)); //ChatGPT for std::stoi(message.substr(5))
        for (int i = 0; i < points; i++) {
            ++(*scoreCtr);
        }
    }
}

