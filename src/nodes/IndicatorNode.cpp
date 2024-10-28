#include "IndicatorNode.hpp"

IndicatorNode* IndicatorNode::create(IndicatorData const &data) {
    auto ret = new IndicatorNode;
    if (ret->init(data)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool IndicatorNode::init(IndicatorData const &data) {
    if (!CCNode::init())
        return false;

    m_indicator = cocos2d::CCSprite::create("indicator.png"_spr);
    m_indicator->setColor({ data.color.r, data.color.g, data.color.b });
    this->addChild(m_indicator, 5);

    m_label = cocos2d::CCLabelBMFont::create(data.label.c_str(), "bigFont.fnt");
    m_label->setAlignment(cocos2d::kCCTextAlignmentCenter);
    m_label->setColor({ data.color.r, data.color.g, data.color.b });
    m_label->setOpacity(data.color.a);
    m_label->setAnchorPoint({ 0.5f, 1 });
    m_label->setPosition({ 0, -m_indicator->getScaledContentHeight() * 0.5f });
    m_label->limitLabelWidth(20.0f, 0.5f, 0.1f);
    this->addChild(m_label);

    return true;
}
