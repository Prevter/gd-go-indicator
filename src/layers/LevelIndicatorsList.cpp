#include "LevelIndicatorsList.hpp"
#include "../GoIndicatorPlayLayer.hpp"
#include "../structs/IndicatorData.hpp"
#include "../nodes/ColorPicker.hpp"
#include "../nodes/FloatSlider.hpp"
#include "../nodes/FontPicker.hpp"

std::string escapeNewlines(std::string const& text) {
    return geode::utils::string::replace(text, "\n", "\\n");
}

std::string unescapeNewlines(std::string const& text) {
    return geode::utils::string::replace(text, "\\n", "\n");
}

bool LevelIndicatorsList::setup(std::string const &id) {
    this->setTitle("Progress Indicators");
    m_title->setPositionX(170.f);

    m_levelId = id;
    m_indicators = IndicatorData::getIndicators(id);
    m_scrollLayer = geode::ScrollLayer::create({0.f, 0.f, 320.f, 215.f});
    m_scrollLayer->setAnchorPoint({ 0, 0.5f });
    m_scrollLayer->setID("scroll-layer"_spr);
    m_scrollLayer->setZOrder(2);
    m_mainLayer->addChildAtPosition(m_scrollLayer, geode::Anchor::BottomLeft, {10.f, 10.f});

    auto scrollBg = cocos2d::extension::CCScale9Sprite::create("square02b_001.png");
    scrollBg->setAnchorPoint({ 0, 0 });
    scrollBg->setColor({ 0, 0, 0 });
    scrollBg->setOpacity(128);
    scrollBg->setContentSize({ 320.f, 215.f });
    scrollBg->setID("content-bg"_spr);
    m_mainLayer->addChildAtPosition(scrollBg, geode::Anchor::BottomLeft, {10.f, 10.f});

    auto togglerMenu = cocos2d::CCMenu::create();
    togglerMenu->setContentSize(m_size);
    togglerMenu->setPosition({0, 0});
    togglerMenu->setAnchorPoint({0, 0});
    togglerMenu->setID("toggler-menu"_spr);
    m_enableToggler = geode::cocos::CCMenuItemExt::createTogglerWithStandardSprites(0.6f, [this](auto) {
        auto enabled = IndicatorData::hasIndicators(m_levelId);
        IndicatorData::setIndicatorsEnabled(m_levelId, !enabled);
    });
    m_enableToggler->toggle(IndicatorData::hasIndicators(m_levelId));
    togglerMenu->addChildAtPosition(m_enableToggler, geode::Anchor::TopRight, {-75.f, -20.f});

    auto enabledLabel = cocos2d::CCLabelBMFont::create("Enabled", "bigFont.fnt");
    enabledLabel->setScale(0.4f);
    togglerMenu->addChildAtPosition(enabledLabel, geode::Anchor::TopRight, {-35.f, -20.f});
    m_mainLayer->addChild(togglerMenu);

    auto sidebarMenu = cocos2d::CCMenu::create();
    sidebarMenu->setContentSize({ 76.f, 260.f });
    sidebarMenu->setPosition({335, -96});
    sidebarMenu->setAnchorPoint({0, 0});
    sidebarMenu->setID("sidebar-menu"_spr);

    auto addIndicatorButton = geode::cocos::CCMenuItemExt::createSpriteExtra(ButtonSprite::create(
        "Add", 60.f, true, "bigFont.fnt", "GJ_button_01.png", 24.f, 0.5
        ), [this](auto) {
        m_indicators.push_back(IndicatorData { 50.0, "Go!", "bigFont.fnt", { 255, 255, 255, 255 }, true });
        this->refreshList();
    });
    sidebarMenu->addChild(addIndicatorButton);

    auto maxWidthLabel = cocos2d::CCLabelBMFont::create("Label Scale", "bigFont.fnt");
    maxWidthLabel->limitLabelWidth(76.f, 0.5f, 0.1f);
    sidebarMenu->addChild(maxWidthLabel);

    auto maxWidthInput = geode::TextInput::create(100.f, "320", "bigFont.fnt");
    maxWidthInput->setFilter("0123456789.");
    maxWidthInput->setScale(0.75f);
    maxWidthInput->setCallback([this](std::string const& text) {
        auto value = geode::utils::numFromString<double>(text);
        if (!value) return;
        double clamped = std::clamp(*value, 10.0, 100.0);
        geode::Mod::get()->setSavedValue(fmt::format("max-width-{}", m_levelId), clamped);
    });
    maxWidthInput->setString(fmt::format("{:.2f}", geode::Mod::get()->getSavedValue<double>(fmt::format("max-width-{}", m_levelId), 20.0)));
    sidebarMenu->addChild(maxWidthInput);

    sidebarMenu->setLayout(
        geode::ColumnLayout::create()
            ->setAxisReverse(true)
            ->setAutoScale(false)
            ->setCrossAxisOverflow(true)
            ->setAutoGrowAxis(this->getContentHeight())
            ->setAxisAlignment(geode::AxisAlignment::End)
            ->setGap(2)
    );
    m_mainLayer->addChild(sidebarMenu);

    this->refreshList();

    return true;
}

cocos2d::CCMenu* LevelIndicatorsList::createIndicatorCell(IndicatorData& data, int index) {
    auto cell = cocos2d::CCMenu::create();
    cell->setContentSize({320.f, 40.f});
    cell->setAnchorPoint({0, 0});
    cell->setID(fmt::format("indicator-cell-{}", index));

    // add background
    auto bg = cocos2d::extension::CCScale9Sprite::create("GJ_square02.png");
    bg->setContentSize({310.f, 40.f});
    bg->setAnchorPoint({1, 1});
    bg->setPosition({-5.f, 0.f});
    bg->setID("background"_spr);
    cell->addChild(bg);

    // delete button
    auto deleteButton = geode::cocos::CCMenuItemExt::createSpriteExtraWithFrameName("GJ_deleteIcon_001.png", 0.6f, [this, index](auto) {
        m_indicators.erase(m_indicators.begin() + index);
        this->refreshList();
    });
    deleteButton->setID("delete-button"_spr);
    cell->addChildAtPosition(deleteButton, geode::Anchor::Left, {20.f, 0});

    // percentage slider
    auto percentageSlider = FloatSlider::create(0.0, 100.0, data.percentage, [this, index](auto value) {
        m_indicators[index].percentage = value;
    });
    percentageSlider->setID("percentage-slider"_spr);
    cell->addChildAtPosition(percentageSlider, geodr::Anchor::BottomLeft, {35.f, 10.f});

    // toggle button
    auto toggleButton = geode::cocos::CCMenuItemExt::createTogglerWithStandardSprites(0.5f, [this, index](auto) {
        m_indicators[index].enabled = !m_indicators[index].enabled;
    });
    toggleButton->toggle(data.enabled);
    toggleButton->setID("toggle-button"_spr);
    cell->addChildAtPosition(toggleButton, geode::Anchor::BottomLeft, {135.f, 20.f});

    // label input
    auto labelInput = geode::TextInput::create(190.f, "Go!", "chatFont.fnt");
    labelInput->setFilter("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()-_=+[]{};:'\",.<>/?\\|`~ ");
    labelInput->setCallback([this, index](std::string const& text) {
        m_indicators[index].label = unescapeNewlines(text);
    });
    labelInput->setString(escapeNewlines(data.label));
    labelInput->setScale(0.6f);
    labelInput->setID("label-input"_spr);
    cell->addChildAtPosition(labelInput, geode::Anchor::BottomLeft, {205.f, 20.f});

    // font picker
    FontPicker* fontPickerBtn = FontPicker::create(m_indicators[index].font, [this, index](auto font) {
        m_indicators[index].font = font;
    });
    fontPickerBtn->setID("font-picker-button"_spr);
    cell->addChildAtPosition(fontPickerBtn, geode::Anchor::BottomLeft, {275.f, 20.f});

    // color picker
    auto colorPickerBtn = ColorPicker::create(data.color, [this, index](auto color) {
        m_indicators[index].color = color;
    });
    colorPickerBtn->setID("color-picker-button"_spr);
    cell->addChildAtPosition(colorPickerBtn, geode::Anchor::BottomLeft, {295.f, 20.f});

    return cell;
}

void LevelIndicatorsList::refreshList() {
    auto layer = m_scrollLayer->m_contentLayer;
    layer->removeAllChildrenWithCleanup(true);

    for (int i = 0; i < m_indicators.size(); i++) {
        auto cell = createIndicatorCell(m_indicators[i], i);
        layer->addChild(cell);
    }

    auto height = std::max(42.f * static_cast<float>(m_indicators.size()), 215.f);
    layer->setLayout(
        geode::ColumnLayout::create()
            ->setAxisReverse(true)
            ->setAutoGrowAxis(height)
            ->setAxisAlignment(geode::AxisAlignment::End)
            ->setGap(2)
    );

    layer->setContentSize({ 320.f, height });
    m_scrollLayer->scrollToTop();
}

LevelIndicatorsList * LevelIndicatorsList::create(std::string const &id) {
    auto ret = new LevelIndicatorsList;
    if (ret->initAnchored(420.f, 260.f, id)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void LevelIndicatorsList::onExit() {
    Popup::onExit();
    IndicatorData::saveIndicators(m_levelId, m_indicators);
    if (auto pl = static_cast<GoIndicatorPlayLayer*>(PlayLayer::get()))
        pl->recreateIndicators();
}
