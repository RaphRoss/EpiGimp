#pragma once
#include "Command.hpp"
#include <vector>
#include <memory>
#include <functional>

class HistoryManager {
public:
    HistoryManager();
    
    void executeCommand(std::unique_ptr<Command> command);
    void undo();
    void redo();
    void clear();
    
    bool canUndo() const;
    bool canRedo() const;
    
    size_t getUndoCount() const;
    size_t getRedoCount() const;
    
    void setMaxHistorySize(size_t maxSize) { maxHistorySize = maxSize; }
    
    void setOnHistoryChanged(std::function<void()> callback) { onHistoryChanged = callback; }

private:
    std::vector<std::unique_ptr<Command>> undoStack;
    std::vector<std::unique_ptr<Command>> redoStack;
    size_t maxHistorySize = 50;
    
    std::function<void()> onHistoryChanged;
    
    void trimHistory();
    void notifyHistoryChanged();
};
