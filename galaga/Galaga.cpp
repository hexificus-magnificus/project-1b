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