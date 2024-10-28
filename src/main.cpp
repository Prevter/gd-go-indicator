#include <cvolton.level-id-api/include/EditorIDs.hpp>
#include <Geode/modify/PauseLayer.hpp>

#include "layers/LevelIndicatorsList.hpp"
#include "nodes/IndicatorNode.hpp"
#include "GoIndicatorPlayLayer.hpp"

IndicatorNode* GoIndicatorPlayLayer::addIndicator(IndicatorData const &data) const {
    auto indicator = IndicatorNode::create(data);
    auto progress = static_cast<float>(m_progressBar->getContentSize().width * data.percentage / 100.0);
    m_progressBar->addChildAtPosition(indicator, cocos2d::Anchor::Left, { progress, 0 });
    return indicator;
}

std::string GoIndicatorPlayLayer::getLevelId() const {
    if (m_level->m_levelType == GJLevelType::Editor) {
        return fmt::format("editor-{}", EditorIDs::getID(m_level));
    }
    return fmt::format("level-{}", m_level->m_levelID.value());
}

void GoIndicatorPlayLayer::recreateIndicators() {
    auto fields = m_fields.self();

    for (auto indicator : fields->m_indicators)
        indicator->removeFromParentAndCleanup(true);
    fields->m_indicators.clear();

    if (!IndicatorData::hasIndicators(getLevelId())) return;

    auto indicators = IndicatorData::getIndicators(getLevelId());
    for (auto const& indicator : indicators) {
        if (!indicator.enabled) continue;
        fields->m_indicators.push_back(this->addIndicator(indicator));
    }
}

void GoIndicatorPlayLayer::setupHasCompleted() {
    PlayLayer::setupHasCompleted();
    this->recreateIndicators();
}

struct GoIndicatorPauseLayer final : geode::Modify<GoIndicatorPauseLayer, PauseLayer> {
    void onAddIndicator(CCObject*) {
        auto pl = static_cast<GoIndicatorPlayLayer*>(PlayLayer::get());
        if (!pl) return;
        LevelIndicatorsList::create(pl->getLevelId())->show();
    }

    void customSetup() override {
        PauseLayer::customSetup();
        auto menu = this->getChildByID("right-button-menu");
        auto btn = CCMenuItemSpriteExtra::create(
            cocos2d::CCSprite::createWithSpriteFrameName("GJ_audioOnBtn_001.png"),
            this, menu_selector(GoIndicatorPauseLayer::onAddIndicator)
        );
        btn->setID("indicators-list"_spr);
        menu->addChild(btn);
        menu->updateLayout();
    }
};