#pragma once
#include "Tool.hpp"
#include "PencilTool.hpp"
#include <memory>
#include <string>
#include "../core/Canvas.hpp"

class ToolFactory {
public:
    static std::unique_ptr<Tool> createTool(const std::string& name, Canvas& canvas) {
        if (name == "pencil") return std::make_unique<PencilTool>(canvas);
        return nullptr;
    }
};
