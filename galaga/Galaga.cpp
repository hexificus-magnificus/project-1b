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
        ++wave;
    }
    state = s;
}

std::shared_ptr<Enemy> Galaga::SpawnEnemy(CMPUT350::GameContext *context) {
    

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
    
}

void Galaga::Update(CMPUT350::GameContext *context) {
    switch (state) {
        case GameState::menu:
            break;
        case GameState::startup:
            scoreCtr = std::make_shared<UICounter>("Score: {}", 0, CMPUT350::Point2D(10, 10));
            waveCtr = std::make_shared<UICounter>("Wave: {}", 1, CMPUT350::Point2D(10, 870));
            context->mEngineView->AddGameObject(std::make_shared<Player>(CMPUT350::Point2D(768 / 2, 900)));
            context->mEngineView->AddGameObject(scoreCtr);
            context->mEngineView->AddGameObject(waveCtr);
            SetState(GameState::waveEnter);
            break;
        case GameState::waveEnter:
            for (int i = 0; i < 8; i++) {
                enemies[i] = SpawnEnemy(context);
                context->mEngineView->AddGameObject(enemies[i]);
            }
            SetState(GameState::play);
            break;
        case GameState::play:
            if (enemies.size() == 0) {
                if (wave < 5) {
                    SetState(GameState::waveEnter);
                } else {
                    SetState(GameState::menu);
                }
            }
            break;
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
}

