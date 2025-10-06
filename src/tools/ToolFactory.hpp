#pragma once
#include "Tool.hpp"
#include "PencilTool.hpp"
#include "PanTool.hpp"
#include "SelectTool.hpp"
#include <memory>
#include <string>

class ToolFactory {
public:
    static std::unique_ptr<Tool> createTool(const std::string& name) {
        if (name == "pencil") return std::make_unique<PencilTool>();
        if (name == "pan") return std::make_unique<PanTool>();
        if (name == "select") return std::make_unique<SelectTool>();
        return nullptr;
    }
};
