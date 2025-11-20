#pragma once
#include <SFML/Graphics.hpp>
#include "ImageManager.hpp"
#include "Grid.hpp"
#include "Rulers.hpp"
#include "../tools/Tool.hpp"
#include <memory>
#include "../ui/MenuBar.hpp"
#include "../ui/ToolPanel.hpp"
#include "../ui/ColorPanel.hpp"
#include "../ui/ColorPickerDialog.hpp"
#include "../ui/ToolOptionsPanel.hpp"
#include "../ui/NewImageDialog.hpp"
#include "../ui/StatusBar.hpp"
#include "../ui/LayerPanel.hpp"
#include <iostream>

class Application {
public:
    Application(int width, int height, const std::string& title);
    void run();

private:
    void processEvents();
    void render();
    void setupMenus();
    void setupToolPanel();
    void setupImageManagerCallbacks();
    void setupToolCallbacks();
    void handleImageInput(const sf::Event& event);
    void handleZoom(const sf::Event& event);
    void updateStatusBar(const sf::Vector2f& mousePos);
    void toggleGrid();
    void toggleRulers();
    void centerImage();
    void fitImageToView();
    void undo();
    void redo();
    void selectAll();
    void deselectAll();
    void saveFile();
    void saveAsFile();
    void openFile();
    void newImage();
    void flipHorizontal();
    void flipVertical();
    void rotate90();
    void rotate180();
    void rotate270();
    void cropToSelection();
    std::string openFileDialog();
    std::string saveFileDialog();

private:
    sf::RenderWindow window;
    ImageManager imageManager;
    Grid grid;
    Rulers rulers;
    std::unique_ptr<Tool> currentTool;
    MenuBar menuBar;
    ToolPanel toolPanel;
    ColorPanel colorPanel;
    ColorPickerDialog colorPicker;
    ToolOptionsPanel toolOptions;
    NewImageDialog newImageDialog;
    StatusBar statusBar;
    LayerPanel layerPanel;
    std::string currentToolName = "pencil";
};
