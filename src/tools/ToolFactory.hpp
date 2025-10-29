#pragma once
#include "Tool.hpp"
#include "PencilTool.hpp"
#include "PanTool.hpp"
#include "SelectTool.hpp"
#include "EyedropperTool.hpp"
#include "EraserTool.hpp"
#include "AirbrushTool.hpp"
#include "BucketFillTool.hpp"
#include "GradientTool.hpp"
#include <memory>
#include <string>

class ToolFactory {
public:
    static std::unique_ptr<Tool> createTool(const std::string& name) {
        if (name == "pencil") return std::make_unique<PencilTool>();
        if (name == "pan") return std::make_unique<PanTool>();
        if (name == "select") return std::make_unique<SelectTool>();
        if (name == "eyedropper") return std::make_unique<EyedropperTool>();
        if (name == "eraser") return std::make_unique<EraserTool>();
        if (name == "airbrush") return std::make_unique<AirbrushTool>();
        if (name == "bucket") return std::make_unique<BucketFillTool>();
        if (name == "gradient") return std::make_unique<GradientTool>();
        return nullptr;
    }
};
