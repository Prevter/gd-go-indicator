#pragma once
#include <Geode/modify/PlayLayer.hpp>

class IndicatorNode;

struct GoIndicatorPlayLayer final : geode::Modify<GoIndicatorPlayLayer, PlayLayer> {
    struct Fields {
        std::vector<IndicatorNode*> m_indicators;
    };

    [[nodiscard]] IndicatorNode* addIndicator(IndicatorData const& data, float width) const;
    [[nodiscard]] std::string getLevelId() const;

    void recreateIndicators();
    $override void setupHasCompleted();
};