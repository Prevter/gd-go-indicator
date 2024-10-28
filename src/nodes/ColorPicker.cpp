#include "ColorPicker.hpp"

bool ColorPicker::init(cocos2d::ccColor4B const &value, std::function<void(cocos2d::ccColor4B const &)> const &callback) {
    m_colorSprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    if (!m_colorSprite) return false;

    m_color = value;
    m_callback = callback;

    m_colorSprite->setColor({ value.r, value.g, value.b });
    m_colorSprite->setScale(0.5f);

    if (!CCMenuItemSpriteExtra::init(m_colorSprite, nullptr, this, menu_selector(ColorPicker::onPickColor)))
        return false;

    return true;
}

void ColorPicker::updateColor(cocos2d::ccColor4B const &color) {
    m_color = color;
    m_callback(color);
}

void ColorPicker::onPickColor(CCObject*) {
    auto picker = geode::ColorPickPopup::create(m_color);
    picker->setDelegate(this);
    picker->setColorTarget(m_colorSprite);
    picker->show();
}

ColorPicker* ColorPicker::create(cocos2d::ccColor4B const &value, std::function<void(cocos2d::ccColor4B const &)> const &callback) {
    auto ret = new ColorPicker;
    if (ret->init(value, callback)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
