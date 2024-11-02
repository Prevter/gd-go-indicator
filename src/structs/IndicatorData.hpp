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
    if (auto val = value.try_get(key)) {
        return val.value().get().as<T>();
    }
    return fallback;
}

template<>
struct matjson::Serialize<IndicatorData> {
    static IndicatorData from_json(Value const& value) {
        return IndicatorData {
            .percentage = value["percentage"].as_double(),
            .label = value["label"].as_string(),
            .font = getOr<std::string>(value, "font", "bigFont.fnt"),
            .color = value["color"].as<cocos2d::ccColor4B>(),
            .enabled = value["enabled"].as_bool(),
        };
    }

    static Value to_json(IndicatorData const& value) {
        auto obj = Object();
        obj["percentage"] = value.percentage;
        obj["label"] = value.label;
        obj["color"] = value.color;
        obj["enabled"] = value.enabled;
        obj["font"] = value.font;
        return obj;
    }
};