#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        // Add the title square
        auto titleSquare = CCSprite::create("titleSquare.png"_spr);
        titleSquare->setAnchorPoint({0.5f, 0.5f});
        titleSquare->setPosition({145.0f, 455.0f});
        titleSquare->setID("sonic-title-square");
        titleSquare->setZOrder(50);
        this->addChild(titleSquare);

        // Add the level name label
        auto levelLabel = CCLabelBMFont::create(level->m_levelName.c_str(), "sonic3Font.fnt"_spr);
        levelLabel->setAnchorPoint({1.0f, 0.5f});
        levelLabel->setPosition({930.0f, 155.0f});
        levelLabel->setAlignment(kCCTextAlignmentRight);
        levelLabel->setID("sonic-level-label");
        levelLabel->setZOrder(50);
        this->addChild(levelLabel);

        // Add the "ZONE" label
        auto zoneLabel = CCLabelBMFont::create("ZONE", "sonic3Font.fnt"_spr);
        zoneLabel->setAnchorPoint({1.0f, 0.5f});
        zoneLabel->setPosition({930.0f, 105.0f});
        zoneLabel->setAlignment(kCCTextAlignmentRight);
        zoneLabel->setID("sonic-zone-text");
        zoneLabel->setZOrder(50);
        this->addChild(zoneLabel);

        return true;
    }

    void startGame(){
    	PlayLayer::startGame();

    	auto square = this->getChildByID("sonic-title-square");
    	auto levelname = this->getChildByID("sonic-level-label");
    	auto zonetxt = this->getChildByID("sonic-zone-text");

    	auto squareMovements = CCSequence::create(
    		CCMoveTo::create(0.25f, {145, 190}),
    		CCDelayTime::create(2.0f),
    		CCMoveTo::create(0.25f, {145, 455}),
    		nullptr
    		);

    	auto levelLabelMovements = CCSequence::create(
    		CCMoveTo::create(0.25f, {530, 155}),
    		CCDelayTime::create(2.0f),
    		CCMoveTo::create(0.25f, {930, 155}),
    		nullptr
    		);

    	auto zoneTextMovements = CCSequence::create(
    		CCDelayTime::create(0.08f),
    		CCMoveTo::create(0.25f, {530, 105}),
    		CCDelayTime::create(2.0f),
    		CCMoveTo::create(0.25f, {930, 105}),
    		nullptr
    		);

    	square->runAction(squareMovements);
    	levelname->runAction(levelLabelMovements);
    	zonetxt->runAction(zoneTextMovements);

    }
};