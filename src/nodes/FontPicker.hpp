#pragma once

class FontPickerPopup : public geode::Popup<std::string const&, std::function<void(std::string const&)> const&> {
protected:
    bool setup(std::string const& font, std::function<void(std::string const&)> const& callback) override;
    void nextFont(CCObject*);
    void prevFont(CCObject*);
    void updateFont() const;

public:
    static FontPickerPopup* create(std::string const& font, std::function<void(std::string const&)> const& callback);
    static size_t getFontIndex(std::string const& fontFile);
    static std::string lookupFont(std::string const& fontFile);

private:
    size_t m_fontIndex{};
    cocos2d::CCLabelBMFont* m_fontLabel{};
    CCMenuItemSpriteExtra* m_nextFontButton{};
    CCMenuItemSpriteExtra* m_prevFontButton{};
};

class FontPicker : public CCMenuItemSpriteExtra {
protected:
    bool init(std::string const& font, std::function<void(std::string const&)> const& callback);
    void onPickFont(CCObject*);

public:
    static FontPicker* create(std::string const& font, std::function<void(std::string const&)> const& callback);
    void setFont(std::string const& font);

protected:
    cocos2d::CCLabelBMFont* m_fontLabel{};
    std::string m_font{};
    std::function<void(std::string const&)> m_callback{};
};