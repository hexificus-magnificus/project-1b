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
                mListeners[key].erase(i); // source for erase: https://www.geeksforgeeks.org/cpp/vector-erase-in-cpp-stl/
                break;
            }
        }
    }

    void NotificationManager::Notify(const std::string& message){
        Clean();
        for (auto i= mListeners[message].begin(); i!= mListeners[message].end(); ){
            std::shared_ptr<GameObject> objectToNotify = i->lock(); 
            if (objectToNotify){
                objectToNotify->ReceiveNotification(message);
                i++;
            }else{
                i = mListeners[message].erase(i);
            }
        }
    }

    void NotificationManager::Clean() {
        for (auto i = mListeners.begin(); i != mListeners.end(); i++) {
            auto objs = i->second;
            std::vector<std::weak_ptr<GameObject>> updated;
            for (auto obj : objs) {
                if (!obj.expired()) {
                    updated.push_back(obj);
                }
            }
            std::swap(mListeners[i->first], updated);
        }
    }
    
    };
