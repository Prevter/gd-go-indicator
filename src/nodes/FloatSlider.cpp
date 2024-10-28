#include "FloatSlider.hpp"

bool FloatSlider::init(double min, double max, double value, std::function<void(double)> const& callback) {
    if (!CCMenu::init()) return false;

    m_min = min;
    m_max = max;
    m_value = value;
    m_callback = callback;

    this->setContentSize({84, 24});

    m_slider = Slider::create(this, menu_selector(FloatSlider::onSliderChanged));
    m_slider->setValue(static_cast<float>(toPercentage(value)));
    m_slider->setContentSize({140, 0});
    m_slider->setScale(0.4);
    m_slider->setPosition({0, 2});
    this->addChild(m_slider);

    m_input = geode::TextInput::create(100.f, "50.00", "bigFont.fnt");
    m_input->setFilter("0123456789.");
    m_input->setCallback([this](std::string const& text) {
        auto value = geode::utils::numFromString<double>(text);
        if (!value) return;
        double clamped = std::clamp(*value, m_min, m_max);
        m_value = clamped;
        m_slider->setValue(static_cast<float>(toPercentage(clamped)));
        m_callback(clamped);
    });
    m_input->setString(fmt::format("{:.2f}", value));
    m_input->setPosition({42.f, 15.5f});
    m_input->setScale(0.5);
    this->addChild(m_input);

    return true;
}

double FloatSlider::toValue(double percentage) const {
    auto range = m_max - m_min;
    return m_min + range * percentage;
}

double FloatSlider::toPercentage(double value) const {
    auto range = m_max - m_min;
    return (value - m_min) / range;
}

void FloatSlider::onSliderChanged(CCObject*) {
    auto value = toValue(m_slider->getValue());
    m_value = value;
    m_input->setString(fmt::format("{:.2f}", value));
    m_callback(value);
}

FloatSlider* FloatSlider::create(double min, double max, double value, std::function<void(double)> const &callback) {
    auto ret = new FloatSlider;
    if (ret->init(min, max, value, callback)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void FloatSlider::setValue(double value) {
    m_slider->setValue(static_cast<float>(toPercentage(value)));
    m_value = value;
    m_input->setString(fmt::format("{:.2f}", value));
    m_callback(value);
}
