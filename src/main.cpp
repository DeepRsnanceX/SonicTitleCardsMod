#include <Geode/modify/PlayLayer.hpp>
#include <algorithm>
#include <cctype>

using namespace geode::prelude;

auto chosenGame = Mod::get()->getSettingValue<std::string>("game-card");
auto spriteQuality = Mod::get()->getSettingValue<std::string>("quality-type");
auto stayTime = Mod::get()->getSettingValue<double>("stay-time");

$on_mod(Loaded){
    listenForSettingChanges("game-card", [](std::string value) {
        chosenGame = value;
    });
    listenForSettingChanges("quality-type", [](std::string value) {
        spriteQuality = value;
    });
    listenForSettingChanges("stay-time", [](double value) {
        stayTime = value;
    });
}

class $modify(PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        // Add the title square
        auto squareToUse = fmt::format("{}_titleSquare_{}.png"_spr, chosenGame, spriteQuality);
        auto titleSquare = CCSprite::create(squareToUse.c_str());
        titleSquare->setID("sonic-title-square");
        titleSquare->setZOrder(50);
        titleSquare->setPosition({5000.0f, 5000.0f});
        this->addChild(titleSquare);
        if (chosenGame == "sonic2") {
            auto bottomSquareToUse = fmt::format("sonic2_titleSquareBottom_{}.png"_spr, spriteQuality);
            auto bottomSquare = CCSprite::create(bottomSquareToUse.c_str());
            bottomSquare->setID("sonic-title-square-bottom");
            bottomSquare->setZOrder(50);
            bottomSquare->setPosition({1300.0f, 20.0f});
            bottomSquare->setAnchorPoint({1.0f, 0.5f});
            this->addChild(bottomSquare);
        }

        auto fontToUse = fmt::format("{}Font_{}.fnt"_spr, chosenGame, spriteQuality);
        std::string levelNameResult = level->m_levelName;
        std::string zoneText = "";

        // Convert level name to uppercase (if sonic 1/2)
        if (chosenGame == "sonic1" || chosenGame == "sonic2") {
            std::transform(levelNameResult.begin(), levelNameResult.end(), levelNameResult.begin(), ::toupper);
            zoneText = "ZONE";
        }
        
        // convert level name to lowercase (if sonic 3)
        if (chosenGame == "sonic3") {
            if (spriteQuality == "og") {
                std::transform(levelNameResult.begin(), levelNameResult.end(), levelNameResult.begin(), ::toupper);
                zoneText = "ZONE";
            } else if (spriteQuality == "hq"){
                std::transform(levelNameResult.begin(), levelNameResult.end(), levelNameResult.begin(), ::tolower);
                zoneText = "zone";
            }
        }

        // Add the level name label
        auto levelLabel = CCLabelBMFont::create(levelNameResult.c_str(), fontToUse.c_str());
        levelLabel->setID("sonic-level-label");
        levelLabel->setZOrder(51);
        levelLabel->setPosition({5000.0f, 5000.0f});
        this->addChild(levelLabel);

        // Add the "ZONE" label
        auto zoneLabel = CCLabelBMFont::create(zoneText.c_str(), fontToUse.c_str());
        zoneLabel->setID("sonic-zone-text");
        zoneLabel->setZOrder(51);
        zoneLabel->setPosition({5000.0f, 5000.0f});
        this->addChild(zoneLabel);

        if (chosenGame == "sonic1") {
            // title square
            titleSquare->setAnchorPoint({0.5f, 0.5f});
            titleSquare->setPosition({650.0f, 170.0f});

            // level name label
            levelLabel->setAnchorPoint({1.0f, 0.5f});
            levelLabel->setAlignment(kCCTextAlignmentRight);
            levelLabel->setScale(0.75f);

            // zone label
            zoneLabel->setAnchorPoint({1.0f, 0.5f});
            zoneLabel->setAlignment(kCCTextAlignmentRight);
            zoneLabel->setScale(0.75f);
            if (spriteQuality == "hq") {
                levelLabel->setPosition({-200.0f, 188.0f});
                zoneLabel->setPosition({-180.0f, 153.0f});
            } else if (spriteQuality == "og") {
                levelLabel->setPosition({-200.0f, 180.0f});
                zoneLabel->setPosition({-180.0f, 145.0f});
            }
        }

        if (chosenGame == "sonic2") {
            // title square
            titleSquare->setPosition({-300.0f, 158.0f});

            // level name label
            levelLabel->setAnchorPoint({1.0f, 0.5f});
            levelLabel->setPosition({1200.0f, 235.0f});
            levelLabel->setAlignment(kCCTextAlignmentRight);

            // zone label
            zoneLabel->setAnchorPoint({1.0f, 0.5f});
            zoneLabel->setPosition({-300.0f, 190.0f});
            zoneLabel->setAlignment(kCCTextAlignmentRight);

            if (spriteQuality == "og") {
                levelLabel->setScale(1.65f);
                zoneLabel->setScale(1.65f);
            }
        }

        if (chosenGame == "sonic3") {
            // title square
            titleSquare->setAnchorPoint({0.5f, 0.5f});
            titleSquare->setPosition({145.0f, 455.0f});

            // level name label
            levelLabel->setAnchorPoint({1.0f, 0.5f});
            levelLabel->setPosition({1080.0f, 155.0f});
            levelLabel->setAlignment(kCCTextAlignmentRight);

            // zone label
            zoneLabel->setAnchorPoint({1.0f, 0.5f});
            zoneLabel->setPosition({1080.0f, 105.0f});
            zoneLabel->setAlignment(kCCTextAlignmentRight);
        }

        return true;
    }

    void startGame(){
    	PlayLayer::startGame();

        // Get the elements so i can animate them
    	auto square = this->getChildByID("sonic-title-square");
    	auto levelname = this->getChildByID("sonic-level-label");
    	auto zonetxt = this->getChildByID("sonic-zone-text");

        // Create the movement sequences for
        // Sonic 1
        auto sonic1_squareMovements = CCSequence::create(
            CCMoveTo::create(0.25f, {405.0f, 170.0f}),
            CCDelayTime::create(stayTime),
            CCMoveTo::create(0.25f, {650.0f, 170.0f}),
            nullptr
        );
        auto sonic1_levelLabelMovementsHQ = CCSequence::create(
            CCMoveTo::create(0.25f, {435.0f, 188.0f}),
            CCDelayTime::create(stayTime),
            CCMoveTo::create(0.25f, {-200.0f, 188.0f}),
            nullptr
        );
        auto sonic1_zoneTextMovementsHQ = CCSequence::create(
            CCMoveTo::create(0.25f, {405.0f, 153.0f}),
            CCDelayTime::create(stayTime),
            CCMoveTo::create(0.25f, {-180.0f, 153.0f}),
            nullptr
        );
        auto sonic1_levelLabelMovementsOG = CCSequence::create(
            CCMoveTo::create(0.25f, {435.0f, 180.0f}),
            CCDelayTime::create(stayTime),
            CCMoveTo::create(0.25f, {-200.0f, 180.0f}),
            nullptr
        );
        auto sonic1_zoneTextMovementsOG = CCSequence::create(
            CCMoveTo::create(0.25f, {405.0f, 145.0f}),
            CCDelayTime::create(stayTime),
            CCMoveTo::create(0.25f, {-180.0f, 145.0f}),
            nullptr
        );
        // Sonic 2: TODO
        float sonic2Delay = stayTime + 0.65f;
        auto sonic2_bottomSquareMovements = CCSequence::create(
            CCMoveTo::create(0.25f, {570.0f, 20.0f}),
            CCDelayTime::create(stayTime),
            CCMoveTo::create(0.25f, {1300.0f, 20.0f}),
            nullptr
        );
        auto sonic2_mainSquareMovements = CCSequence::create(
            CCMoveTo::create(0.28f, {40.0f, 158.0f}),
            CCDelayTime::create(stayTime),
            CCMoveTo::create(0.25f, {-300.0f, 158.0f}),
            nullptr
        );
        auto sonic2_levelLabelMovements = CCSequence::create(
            CCDelayTime::create(0.05f),
            CCMoveTo::create(0.25f, {545.0f, 235.0f}),
            CCDelayTime::create(sonic2Delay),
            CCMoveTo::create(0.25f, {1200.0f, 235.0f}),
            nullptr
        );
        auto sonic2_zoneTextMovements = CCSequence::create(
            CCDelayTime::create(0.05f),
            CCMoveTo::create(0.25f, {515.0f, 190.0f}),
            CCDelayTime::create(sonic2Delay),
            CCMoveTo::create(0.25f, {-300.0f, 190.0f}),
            nullptr
        );

        // Sonic 3
        auto sonic3_squareMovements = CCSequence::create(
            CCMoveTo::create(0.25f, {145.0f, 190.0f}),
            CCDelayTime::create(stayTime),
            CCMoveTo::create(0.25f, {145.0f, 455.0f}),
            nullptr
        );
        auto sonic3_levelLabelMovements = CCSequence::create(
            CCMoveTo::create(0.25f, {530.0f, 155.0f}),
            CCDelayTime::create(stayTime),
            CCMoveTo::create(0.25f, {1080.0f, 155.0f}),
            nullptr
        );
        auto sonic3_zoneTextMovements = CCSequence::create(
            CCMoveTo::create(0.25f, {530.0f, 105.0f}),
            CCDelayTime::create(stayTime),
            CCMoveTo::create(0.25f, {1080.0f, 105.0f}),
            nullptr
        );

        // Run actions here
        if (chosenGame == "sonic1") {
            square->runAction(sonic1_squareMovements);
            if (spriteQuality == "hq") {
                levelname->runAction(sonic1_levelLabelMovementsHQ);
                zonetxt->runAction(sonic1_zoneTextMovementsHQ);
            } else if (spriteQuality == "og") {
                levelname->runAction(sonic1_levelLabelMovementsOG);
                zonetxt->runAction(sonic1_zoneTextMovementsOG);
            }
        } else if (chosenGame == "sonic2") {
            auto bottomSquare = this->getChildByID("sonic-title-square-bottom");

            square->runAction(sonic2_mainSquareMovements);
            bottomSquare->runAction(sonic2_bottomSquareMovements);
            levelname->runAction(sonic2_levelLabelMovements);
            zonetxt->runAction(sonic2_zoneTextMovements);
        } else if (chosenGame == "sonic3") {
            square->runAction(sonic3_squareMovements);
            levelname->runAction(sonic3_levelLabelMovements);
            zonetxt->runAction(sonic3_zoneTextMovements);
        }

    }
};