#include "FontPicker.hpp"

constexpr std::array FontFiles = {
    "bigFont.fnt", "chatFont.fnt", "goldFont.fnt", "gjFont01.fnt",
    "gjFont02.fnt", "gjFont03.fnt", "gjFont04.fnt", "gjFont05.fnt",
    "gjFont06.fnt", "gjFont07.fnt", "gjFont08.fnt", "gjFont09.fnt",
    "gjFont10.fnt", "gjFont11.fnt", "gjFont12.fnt", "gjFont13.fnt",
    "gjFont14.fnt", "gjFont15.fnt", "gjFont16.fnt", "gjFont17.fnt",
    "gjFont18.fnt", "gjFont19.fnt", "gjFont20.fnt", "gjFont21.fnt",
    "gjFont22.fnt", "gjFont23.fnt", "gjFont24.fnt", "gjFont25.fnt",
    "gjFont26.fnt", "gjFont27.fnt", "gjFont28.fnt", "gjFont29.fnt",
    "gjFont30.fnt", "gjFont31.fnt", "gjFont32.fnt", "gjFont33.fnt",
    "gjFont34.fnt", "gjFont35.fnt", "gjFont36.fnt", "gjFont37.fnt",
    "gjFont38.fnt", "gjFont39.fnt", "gjFont40.fnt", "gjFont41.fnt",
    "gjFont42.fnt", "gjFont43.fnt", "gjFont44.fnt", "gjFont45.fnt",
    "gjFont46.fnt", "gjFont47.fnt", "gjFont48.fnt", "gjFont49.fnt",
    "gjFont50.fnt", "gjFont51.fnt", "gjFont52.fnt", "gjFont53.fnt",
    "gjFont54.fnt", "gjFont55.fnt", "gjFont56.fnt", "gjFont57.fnt",
    "gjFont58.fnt", "gjFont59.fnt",
};

constexpr std::array FontNames = {
    "Pusab", "Aller", "Gold Pusab", "FONT01",
    "FONT02", "FONT03", "FONT04", "FONT05",
    "FONT06", "FONT07", "FONT08", "FONT09",
    "FONT10", "FONT11", "Gargle", "Amatic",
    "Cartwheel", "MothproofScript", "LEMON MILK", "LEMON MILK 2",
    "Minecraft", "OptimusPrincepsSemiBold", "Autolova", "Karate",
    "a Annyeong Haseyo", "Ausweis Hollow", "Gypsy Curse", "Magic School Two",
    "Old English Five", "Yeah Papa", "Ninja Naruto", "Metal Lord",
    "Drip Drop", "Electroharmonix", "Aladin", "Creepster",
    "Call Of Ops Duty", "BlocParty", "Astron Boy", "Osaka-Sans Serif",
    "Some Time Later", "Fatboy Slim BLTC BRK", "Moria Citadel", "Rise of Kingdom",
    "FantaisieArtistique", "Edge of the Galaxy", "Wash Your Hand", "Bitwise",
    "Foul Fiend", "Nandaka Western", "Evil Empire", "Comical Cartoon",
    "Carton Six", "aAssassinNinja", "Public Pixel", "New Walt Disney UI",
    "Random 5", "Crafting Lesson", "Game Of Squids", "Monster Game",
    "Lo-Sumires", "Gewtymol",
};

void FontPickerPopup::updateFont() const {
    auto font = FontFiles[m_fontIndex];
    m_fontLabel->setFntFile(font);
    m_fontLabel->setString(lookupFont(font).c_str());
    m_fontLabel->limitLabelWidth(150.f, 1.f, 0.1f);
}

FontPickerPopup* FontPickerPopup::create(std::string const& font, std::function<void(std::string const&)> const& callback) {
    auto ret = new FontPickerPopup;
    if (ret->initAnchored(300.f, 200.f, font, callback)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

size_t FontPickerPopup::getFontIndex(std::string const &fontFile) {
    auto index = std::ranges::find(FontFiles, fontFile) - FontFiles.begin();
    if (index == FontFiles.size()) {
        return 0;
    }
    return index;
}

std::string FontPickerPopup::lookupFont(std::string const &fontFile) {
    auto index = std::ranges::find(FontFiles, fontFile) - FontFiles.begin();
    if (index == FontFiles.size()) {
        return FontNames[0];
    }
    return FontNames[index];
}

bool FontPickerPopup::setup(std::string const &font, std::function<void(std::string const &)> const &callback) {
    this->setTitle("Select a font");

    m_fontIndex = getFontIndex(font);

    m_fontLabel = cocos2d::CCLabelBMFont::create(" ", "bigFont.fnt");
    m_fontLabel->setAnchorPoint({ 0.5f, 0.5f });
    m_fontLabel->setID("font-label"_spr);
    m_mainLayer->addChildAtPosition(m_fontLabel, cocos2d::Anchor::Center);
    updateFont();

    auto sprNext = cocos2d::CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    sprNext->setFlipX(true);
    sprNext->setScale(0.6f);
    m_nextFontButton = geode::cocos::CCMenuItemExt::createSpriteExtra(
        sprNext, [this](auto s) { nextFont(s); }
    );
    m_nextFontButton->setID("next-font-button"_spr);
    m_buttonMenu->addChildAtPosition(m_nextFontButton, cocos2d::Anchor::Right, { -30.f, 0 });


    m_prevFontButton = geode::cocos::CCMenuItemExt::createSpriteExtraWithFrameName(
        "GJ_arrow_02_001.png", 0.6f, [this](auto s) { prevFont(s); }
    );
    m_prevFontButton->setID("prev-font-button"_spr);
    m_buttonMenu->addChildAtPosition(m_prevFontButton, cocos2d::Anchor::Left, { 30.f, 0 });

    auto okButton = geode::cocos::CCMenuItemExt::createSpriteExtra(
        ButtonSprite::create(
            "OK", 30.f, true, "bigFont.fnt", "GJ_button_01.png", 24.f, 0.5
        ), [this, callback](auto c) {
            callback(FontFiles[m_fontIndex]);
            this->onClose(c);
        }
    );
    okButton->setID("ok-button"_spr);
    m_buttonMenu->addChildAtPosition(okButton, cocos2d::Anchor::Bottom, { 0, 20 });

    return true;
}

void FontPickerPopup::nextFont(CCObject*) {
    m_fontIndex = (m_fontIndex + 1) % FontFiles.size();
    updateFont();
}

void FontPickerPopup::prevFont(CCObject*) {
    m_fontIndex = (m_fontIndex + FontFiles.size() - 1) % FontFiles.size();
    updateFont();
}

bool FontPicker::init(std::string const &font, std::function<void(std::string const &)> const &callback) {
    auto spr = ButtonSprite::create("A", 10.f, true, font.c_str(), "GJ_button_05.png", 24.f, 0.4);
    if (!spr) return false;

    spr->setScale(0.75f);
    m_fontLabel = spr->getChildByType<cocos2d::CCLabelBMFont>(0);
    m_fontLabel->limitLabelWidth(10.f, 2.f, 0.1f);

    m_font = font;
    m_callback = callback;

    if (!CCMenuItemSpriteExtra::init(spr, nullptr, this, menu_selector(FontPicker::onPickFont)))
        return false;

    return true;
}

void FontPicker::onPickFont(CCObject*) {
    FontPickerPopup::create(m_font, [this](std::string const& newFont) {
        m_callback(newFont);
        this->setFont(newFont);
    })->show();
}

FontPicker* FontPicker::create(std::string const& font, std::function<void(std::string const&)> const& callback) {
    auto ret = new FontPicker;
    if (ret->init(font, callback)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void FontPicker::setFont(std::string const &font) {
    geode::log::debug("FontPicker::setFont: {}", font);
    m_font = font;
    if (!m_fontLabel) {
        geode::log::error("FontPicker::setFont: m_fontLabel is null");
        return;
    }
    m_fontLabel->setFntFile(font.c_str());
    m_fontLabel->limitLabelWidth(10.f, 2.f, 0.1f);
}
