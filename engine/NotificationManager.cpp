#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "GameObject.h"
#include "NotificationManager.h"

namespace CMPUT350 {

    void NotificationManager::Register(std::weak_ptr<GameObject> object, const std::string& key){
        mListeners[key].push_back(object);
    }

    void NotificationManager::Unregister(std::weak_ptr<GameObject> object, const std::string& key){
        for (auto i = mListeners[key].begin(); i != mListeners[key].end(); i++){ // source for .begin() and .end():  https://stackoverflow.com/q/50870951
            if (i->lock() == object.lock()){ // source for how to access item from shared pointer: https://stackoverflow.com/a/67977161
                mListeners[key].erase(i);
                break;
            }
        }
    }

    void NotificationManager::Notify(const std::string& message){
        for (auto i= mListeners[message].begin(); i!= mListeners[message].end(); ){
            if (auto i->lock())
        }
    }
    
    };
