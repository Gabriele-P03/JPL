#include "SceneManager.hpp"

jpl::_graphics::_engine::SceneManager* jpl::_graphics::_engine::SceneManager::INSTANCE = nullptr;

jpl::_graphics::_engine::SceneManager::SceneManager(){
    jpl::_logger::info("Initializing new SceneManager");
    this->disable();
}

jpl::_graphics::_engine::Scene* const jpl::_graphics::_engine::SceneManager::getSceneByName(const std::string &name) const{
    typename std::unordered_map<std::string, jpl::_graphics::_engine::Scene*>::const_iterator it = this->scenes.find(name);
    if(it != this->scenes.end()){
        return const_cast<jpl::_graphics::_engine::Scene* const>(it->second);
    }
    return nullptr;
}

void jpl::_graphics::_engine::SceneManager::addNewScene(const std::string &newSceneName, jpl::_graphics::_engine::Scene* newScene){
    if(newScene == nullptr){
        throw jpl::_exception::IllegalArgumentException("Cannot insert a nullptr as new scene");
    }
    if(this->getSceneByName(newSceneName) != nullptr){
        throw jpl::_exception::RuntimeException("Scene " + newSceneName + " is already present");
    }
    this->scenes.insert({newSceneName, newScene});
    jpl::_logger::info("New Scene added: " + newSceneName);
}

void jpl::_graphics::_engine::SceneManager::removeSceneByName(const std::string &name){
    if(this->getSceneByName(name) != nullptr){
        throw jpl::_exception::RuntimeException("Scene " + name + " is already present");
    }
    this->scenes.erase(this->scenes.find(name));
    jpl::_logger::info("Scene " + name + " removed");
}

void jpl::_graphics::_engine::SceneManager::setNewCurrentSceneByName(const std::string& name){
    jpl::_graphics::_engine::Scene* scene = this->getSceneByName(name);
    if(scene == nullptr){
        throw jpl::_exception::RuntimeException("Scene " + name + " does not exists");
    }
    this->currentScene = scene;
    this->currentSceneName = name;
    jpl::_logger::info("Scene " + name + " set as current");
}

std::string jpl::_graphics::_engine::SceneManager::getCurrentSceneName() const noexcept{
    return this->currentSceneName;
}
jpl::_graphics::_engine::Scene* const jpl::_graphics::_engine::SceneManager::getCurrentScene() const noexcept{
    return const_cast<jpl::_graphics::_engine::Scene* const>(this->currentScene);
}


void jpl::_graphics::_engine::SceneManager::disable() noexcept{
    this->currentScene = nullptr;
    this->currentSceneName = "";
}

void jpl::_graphics::_engine::SceneManager::render(){
    this->currentScene->render();
}


void jpl::_graphics::_engine::SceneManager::initializeSceneManager(){
    jpl::_graphics::_engine::SceneManager::INSTANCE = new jpl::_graphics::_engine::SceneManager();
}

jpl::_graphics::_engine::SceneManager::~SceneManager(){
    this->scenes.clear();
}