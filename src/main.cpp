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

        // 2. Imagen de Pou reducida para no desbordar la pantalla
        auto pouSprite = CCSprite::create("pouimg.png"_spr);
        if (pouSprite) {
            pouSprite->setPosition(winSize / 2);
            pouSprite->setScale(0.4f); // Reducido a 0.4 para ajustar a pantalla
            this->addChild(pouSprite);
        }

        // 3. Audio de Pou
        FMODAudioEngine::sharedEngine()->playEffect("pou.mp3"_spr);

        // 4. Botón de regreso (flecha)
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

        this->setTouchEnabled(true);
        this->setKeypadEnabled(true);

        return true;
    }

    void onClose(CCObject* sender) {
        this->removeFromParentAndCleanup(true);
    }

    void keyBackClicked() override {
        onClose(nullptr);
    }
};

class $modify(MyPouMenu, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        // Botón en el menú principal reducido a icono pequeño
        auto pouSprite = CCSprite::create("pouimg.png"_spr);
        if (pouSprite) {
            pouSprite->setScale(0.12f); // Reducido a 0.12 para el menú
            
            auto btn = CCMenuItemSpriteExtra::create(
                pouSprite,
                this,
                menu_selector(MyPouMenu::onPouButton)
            );

            auto menu = this->getChildByID("bottom-menu");
            if (menu) {
                menu->addChild(btn);
                menu->updateLayout();
            }
        }

        return true;
    }

    void onPouButton(CCObject* sender) {
        auto layer = PouLayer::create();
        this->addChild(layer, 100);
    }
};

