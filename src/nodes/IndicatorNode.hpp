#pragma once
#include "../structs/IndicatorData.hpp"

class IndicatorNode : public cocos2d::CCNode {
protected:
    cocos2d::CCSprite* m_indicator{};
    cocos2d::CCLabelBMFont* m_label{};

public:
    static IndicatorNode* create(IndicatorData const& data, float width);

protected:
    bool init(IndicatorData const& data, float width);
};