#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H

#include "../Game.h"
#include "./SpriteComponent.h"
#include "./TransformComponent.h"

class InputComponent : public Component {
    private:
        std::string m_upKey;
        std::string m_downKey;
        std::string m_rightKey;
        std::string m_leftKey;
        std::string m_shootKey;

        TransformComponent* m_transform;
        SpriteComponent* m_sprite;
    public:
        InputComponent() {}

        InputComponent(std::string up, std::string right, std::string down, std::string left, std::string shoot) {
            m_upKey = getSDLKeyString(up);
            m_rightKey = getSDLKeyString(right);
            m_downKey = getSDLKeyString(down);
            m_leftKey = getSDLKeyString(left);
            m_shootKey = getSDLKeyString(shoot);            
        }

        std::string getSDLKeyString(std::string key) {
            if (key.compare("up") == 0) return "1073741906";
            if (key.compare("down") == 0) return "1073741905";
            if (key.compare("left") == 0) return "1073741904";
            if (key.compare("right") == 0) return "1073741903";
            if (key.compare("space") == 0) return "32";

            // If letter keys, return ASCII value
            return std::to_string(static_cast<int>(key[0]));
        }

        void initialize() override {
            m_transform = m_owner->getComponent<TransformComponent>();
            m_sprite = m_owner->getComponent<SpriteComponent>();
        }

        void update(float deltaTime) override {
            std::string keyCode = std::to_string(Game::event.key.keysym.sym);

            if (Game::event.type == SDL_KEYDOWN) {
                if(keyCode.compare(m_upKey) == 0) {
                    m_transform->setVelocity({0, -64});
                    m_sprite->play("UpAnimation");
                }
                if(keyCode.compare(m_rightKey) == 0) {
                    m_transform->setVelocity({64, 0});
                    m_sprite->play("RightAnimation");
                }
                if(keyCode.compare(m_downKey) == 0) {
                    m_transform->setVelocity({0, 64});
                    m_sprite->play("DownAnimation");
                }
                if(keyCode.compare(m_leftKey) == 0) {
                    m_transform->setVelocity({-64, 0});
                    m_sprite->play("LeftAnimation");
                }
                if(keyCode.compare(m_shootKey) == 0) {
                    // TODO
                }
            }

            if (Game::event.type == SDL_KEYUP) {
                if(keyCode.compare(m_upKey) == 0) {
                    m_transform->setVelocity({m_transform->getVelocity().x, 0});
                }
                if(keyCode.compare(m_rightKey) == 0) {
                    m_transform->setVelocity({0, m_transform->getVelocity().y});
                }
                if(keyCode.compare(m_downKey) == 0) {
                    m_transform->setVelocity({m_transform->getVelocity().x, 0});
                }
                if(keyCode.compare(m_leftKey) == 0) {
                    m_transform->setVelocity({0, m_transform->getVelocity().y});
                }
                if(keyCode.compare(m_shootKey) == 0) {
                    // TODO
                }
            }
        }

        void render() override {

        }



};

#endif;