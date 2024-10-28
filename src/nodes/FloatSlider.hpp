#pragma once

class FloatSlider : public cocos2d::CCMenu {
protected:
    bool init(double min, double max, double value, std::function<void(double)> const& callback);
    double toValue(double percentage) const;
    double toPercentage(double value) const;
    void onSliderChanged(CCObject*);

public:
    static FloatSlider* create(double min, double max, double value, std::function<void(double)> const& callback);
    double getValue() const { return m_value; }
    void setValue(double value);

protected:
    Slider* m_slider{};
    geode::TextInput* m_input{};
    double m_min{};
    double m_max{};
    double m_step{};
    double m_value{};
    std::function<void(double)> m_callback;
};