#pragma once

struct IndicatorData;

class LevelIndicatorsList : public geode::Popup<std::string const&> {
protected:
    bool setup(std::string const& id) override;
    cocos2d::CCMenu* createIndicatorCell(IndicatorData& data, int index);
    void refreshList();

public:
    static LevelIndicatorsList* create(std::string const& id);
    void onExit() override;

protected:
    std::vector<IndicatorData> m_indicators;
    std::string m_levelId;
    geode::ScrollLayer* m_scrollLayer{};
    cocos2d::CCMenu* m_indicatorsMenu{};
    CCMenuItemSpriteExtra* m_addIndicatorButton{};
    CCMenuItemToggler* m_enableToggler{};
};
