#pragma once

struct IndicatorData {
    double percentage;
    std::string label;
    std::string font;
    cocos2d::ccColor4B color;
    bool enabled = true;

    static std::vector<IndicatorData> getIndicators(std::string_view key) {
        return geode::Mod::get()->getSavedValue<std::vector<IndicatorData>>(
            fmt::format("indicators-{}", key), {}
        );
    }

    static void saveIndicators(std::string_view key, std::vector<IndicatorData> const& indicators) {
        geode::Mod::get()->setSavedValue(
            fmt::format("indicators-{}", key), indicators
        );
    }

    static bool hasIndicators(std::string_view key) {
        return geode::Mod::get()->getSavedValue<bool>(fmt::format("enabled-{}", key), true);
    }

    static void setIndicatorsEnabled(std::string_view key, bool enabled) {
        geode::Mod::get()->setSavedValue(fmt::format("enabled-{}", key), enabled);
    }
};

template <class T>
T getOr(matjson::Value const& value, std::string_view key, T fallback) {
    if (auto val = value.get(key)) {
        auto v = val.unwrap().as<T>();
        return v.unwrapOr(fallback);
    }
    return fallback;
}

template<>
struct matjson::Serialize<IndicatorData> {
    static geode::Result<IndicatorData> fromJson(Value const& value) {
        return geode::Ok(IndicatorData {
            .percentage = getOr<double>(value, "percentage", 50.0),
            .label = getOr<std::string>(value, "label", "Go!"),
            .font = getOr<std::string>(value, "font", "bigFont.fnt"),
            .color = getOr<cocos2d::ccColor4B>(value, "color", { 255, 255, 255, 255 }),
            .enabled = getOr<bool>(value, "enabled", true),
        });
    }

    static Value toJson(IndicatorData const& value) {
        auto obj = Value();
        obj["percentage"] = value.percentage;
        obj["label"] = value.label;
        obj["color"] = value.color;
        obj["enabled"] = value.enabled;
        obj["font"] = value.font;
        return obj;
    }
};