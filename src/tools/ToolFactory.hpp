#pragma once
#include "Tool.hpp"
#include <memory>
#include <string>

class ToolFactory {
public:
    static std::unique_ptr<Tool> createTool(const std::string& name) {
        return nullptr;
    }
};
