#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <fmod.hpp>

using namespace geode::prelude;

class PouLayer : public CCLayer {
public:
    static PouLayer* create() {
        auto ret = new PouLayer();
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool init() override {
        if (!CCLayer::init()) return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        // 1. Fondo blanco
        auto whiteBG = CCLayerColor::create(ccc4(255, 255, 255, 255));
        this->addChild(whiteBG);

        // 2. Imagen de Pou en el centro (usando pouimg.png)
        // Nota: Geode maneja automáticamente la extensión, "pouimg.png"_spr es correcto.
        auto pouSprite = CCSprite::create("pouimg.png"_spr);
        if (pouSprite) {
            pouSprite->setPosition(winSize / 2);
            // Puedes ajustar la escala si es muy grande o chica
            pouSprite->setScale(1.0f); 
            this->addChild(pouSprite);
        } else {
            FLAlertLayer::create("Error", "No se pudo cargar pouimg.png", "OK")->show();
        }

        // 3. Reproducir la música de Pou (usando pou.mp3)
        // Usamos playBackgroundMusic para música, o playEffect para sonidos cortos.
        // FMODAudioEngine::sharedEngine()->playBackgroundMusic("pou.mp3"_spr, true); // true para loop
        
        // Para efectos (sonidos cortos):
        FMODAudioEngine::sharedEngine()->playEffect("pou.mp3"_spr);

        // 4. Botón de salir (flecha estándar de GD)
        auto backBtnSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        auto backButton = CCMenuItemSpriteExtra::create(
            backBtnSprite,
            this,
            menu_selector(PouLayer::onClose)
        );

        auto menu = CCMenu::create();
        menu->addChild(backButton);
        menu->setPosition({25.0f, winSize.height - 25.0f});
        this->addChild(menu);

        // Habilitar controles
        this->setTouchEnabled(true);
        this->setKeypadEnabled(true);

        return true;
    }

    void onClose(CCObject* sender) {
        // Detener la música si se está reproduciendo como background
        // FMODAudioEngine::sharedEngine()->stopBackgroundMusic();
        
        this->removeFromParentAndCleanup(true);
    }

    void keyBackClicked() override {
        onClose(nullptr);
    }
};

class $modify(MyPouMenu, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        // Usamos pouimg.png también para el icono del botón, pero más chico
        auto pouSprite = CCSprite::create("pouimg.png"_spr);
        if (pouSprite) {
            pouSprite->setScale(0.3f); // Escala para que quepa en el menú
            
            auto btn = CCMenuItemSpriteExtra::create(
                pouSprite,
                this,
                menu_selector(MyPouMenu::onPouButton)
            );

            // Intentamos agregarlo al menú inferior
            auto menu = this->getChildByID("bottom-menu");
            if (menu) {
                menu->addChild(btn);
                menu->updateLayout();
            } else {
                // Si no encontramos el menú inferior, lo ponemos en una posición genérica
                auto winSize = CCDirector::sharedDirector()->getWinSize();
                auto genericMenu = CCMenu::create();
                genericMenu->addChild(btn);
                genericMenu->setPosition({winSize.width / 2, 50.0f});
                this->addChild(genericMenu);
            }
        }

        return true;
    }

    void onPouButton(CCObject* sender) {
        auto layer = PouLayer::create();
        // ZOrder alto para que la pantalla blanca tape todo
        this->addChild(layer, 100);
    }
};

