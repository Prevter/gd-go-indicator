#pragma once

class ColorPicker : public CCMenuItemSpriteExtra, public geode::ColorPickPopupDelegate {
protected:
    bool init(cocos2d::ccColor4B const& value, std::function<void(cocos2d::ccColor4B const&)> const& callback);
    void updateColor(cocos2d::ccColor4B const& color) override;
    void onPickColor(CCObject*);

public:
    static ColorPicker* create(cocos2d::ccColor4B const& value, std::function<void(cocos2d::ccColor4B const&)> const& callback);

protected:
    std::function<void(cocos2d::ccColor4B const&)> m_callback;
    cocos2d::CCSprite* m_colorSprite{};
    cocos2d::ccColor4B m_color{};
};
