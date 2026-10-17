#include "Galaga.h"

void Galaga::SetState(GameState s) {
    if (s == GameState::gameOver) {
        clock.reset();
    }
    if (s == GameState::menu) {
        if (state == GameState::gameOver) {
            // remove score/wave counters
            scoreCtr->Kill();
            waveCtr->Kill();

            // swap pointers out of member vars so they can be deconstructed properly
            std::shared_ptr<UICounter> sTemp = std::move(scoreCtr);
            std::shared_ptr<UICounter> wTemp = std::move(waveCtr);

            scoreCtr = nullptr;
            waveCtr = nullptr;
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
        nextAttacker = 0;
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
    CMPUT350::GameContext* context,
    int enemyNumber)
{
    CMPUT350::Point2D stopPosition;
    std::vector<CMPUT350::Point2D> path;

    float x = 170.f + enemyNumber * 60.f;

    if (wave == 1) {
        float y = 300.f + (3.5f - std::abs(enemyNumber - 3.5f)) * 25.f;
        stopPosition = {x, y};

        path = {{-10.f, -10.f},{450.f, 350.f},{600.f, 250.f},stopPosition};

        return std::make_shared<Enemy1>(path[0], path, stopPosition);
    }
    else if (wave == 2) {
        float offset = enemyNumber - 3.5f;
        stopPosition = {x, 270.f + offset * offset * 7.f};

        path = {{778.f, -10.f},{300.f, 350.f},{100.f, 250.f},stopPosition};

        return std::make_shared<Enemy2>(path[0], path, stopPosition);
    }
    else if (wave == 3) {
        // Zigzag formation, entering from top-center
        float y = (enemyNumber % 2 == 0) ? 280.f : 340.f;
        stopPosition = {x, y};

        path = {{384.f, -10.f},{50.f, 100.f},{700.f, 500.f},stopPosition};

        return std::make_shared<Enemy1>(path[0], path, stopPosition);
    }
    else if (wave == 4) {
        // Diagonal formation, entering from bottom-left
        stopPosition = {x, 250.f + enemyNumber * 20.f};

        path = {{-10.f, 850.f},{100.f, 100.f},{650.f, 50.f},stopPosition};

        return std::make_shared<Enemy2>(path[0], path, stopPosition);
    }
    else if (wave == 5) {
        float row = static_cast<float>(enemyNumber / 4);
        float col = static_cast<float>(enemyNumber % 4);

        stopPosition = {260.f + col * 80.f,270.f + row * 90.f};

        path = {{778.f, 850.f},{750.f, 50.f},{50.f, 100.f},stopPosition};

        return std::make_shared<Enemy3>(path[0], path, stopPosition);
    }

    return nullptr;
}

    

Galaga::Galaga() : state(GameState::menu), wave(0), scoreCtr(nullptr), waveCtr(nullptr), enemies(8) {}

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
}

void Galaga::Update(CMPUT350::GameContext *context) {
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
                enemies[numEnemies] = SpawnEnemy(context, numEnemies);
                context->mEngineView->AddGameObject(enemies[numEnemies]);
                spawnTimer = 0;
                numEnemies++;
            }
            if (numEnemies ==8){
                SetState(GameState::play);
            }
            
            break;
        case GameState::play:{
            bool enemiesDead = true;
            bool inFormation = true;
            for (const auto& enemy: enemies){
                if (enemy&& enemy->IsAlive()){
                    enemiesDead = false;
                }if (enemy && enemy->IsAlive() && !enemy->isInFormation()){
                    inFormation = false;
                }
            }
            if (enemiesDead) {
                if (wave < 5) {
                    SetState(GameState::waveEnter);
                } else {
                    SetState(GameState::menu);
                }
            }else if (inFormation){
                attackTimer++;
                if (attackTimer >= 30){
                    attackTimer = 0;
                
                    for (int i = 0; i<enemies.size(); i++){
                        int attacker = (nextAttacker+i)%enemies.size();
                        if (enemies[attacker] && enemies[attacker]->IsAlive() &&enemies[attacker]->Attack(context, player->GetLocation())){
                            std::cout << "Enemy " << attacker << " attacks!" << std::endl;
                            nextAttacker = (attacker + 1) % enemies.size();

                            break;
                    }
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
        SetState(GameState::gameOver);
    }
}

