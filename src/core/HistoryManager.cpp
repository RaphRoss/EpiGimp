#include "HistoryManager.hpp"

HistoryManager::HistoryManager() {
}

void HistoryManager::executeCommand(std::unique_ptr<Command> command) {
    if (!command) return;
    
    command->execute();
    undoStack.push_back(std::move(command));
    
    redoStack.clear();
    
    trimHistory();
    notifyHistoryChanged();
}

void HistoryManager::undo() {
    if (undoStack.empty()) return;
    
    auto command = std::move(undoStack.back());
    undoStack.pop_back();
    
    command->undo();
    redoStack.push_back(std::move(command));
    
    notifyHistoryChanged();
}

void HistoryManager::redo() {
    if (redoStack.empty()) return;
    
    auto command = std::move(redoStack.back());
    redoStack.pop_back();
    
    command->execute();
    undoStack.push_back(std::move(command));
    
    notifyHistoryChanged();
}

void HistoryManager::clear() {
    undoStack.clear();
    redoStack.clear();
    notifyHistoryChanged();
}

bool HistoryManager::canUndo() const {
    return !undoStack.empty();
}

bool HistoryManager::canRedo() const {
    return !redoStack.empty();
}

size_t HistoryManager::getUndoCount() const {
    return undoStack.size();
}

size_t HistoryManager::getRedoCount() const {
    return redoStack.size();
}

void HistoryManager::trimHistory() {
    while (undoStack.size() > maxHistorySize) {
        undoStack.erase(undoStack.begin());
    }
}

void HistoryManager::notifyHistoryChanged() {
    if (onHistoryChanged) {
        onHistoryChanged();
    }
}
