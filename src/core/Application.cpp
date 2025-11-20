#include "Application.hpp"
#include "../tools/ToolFactory.hpp"
#include "../tools/SelectTool.hpp"
#include "Command.hpp"
#include "ColorManager.hpp"
#include "../tools/PencilTool.hpp"
#include "../tools/EraserTool.hpp"
#include "../tools/AirbrushTool.hpp"
#include "../tools/BucketFillTool.hpp"
#include "../tools/GradientTool.hpp"
#include <cstdio>
#include <string>

Application::Application(int width, int height, const std::string& title)
    : window(sf::VideoMode(width, height), title), statusBar(static_cast<float>(width)) {
    window.setFramerateLimit(60);
    currentTool = ToolFactory::createTool("pencil");
    imageManager.setViewportSize({static_cast<float>(width - 220), static_cast<float>(height - 82)});
    setupMenus();
    setupToolPanel();
    setupImageManagerCallbacks();
    setupToolCallbacks();
    newImageDialog.setOnConfirm([this](int width, int height, const std::string& name) {
        std::string finalName = name;
        if (finalName.empty() || finalName == "Untitled") {
            static int untitledCounter = 1;
            finalName = "Untitled-" + std::to_string(untitledCounter++);
        }
        imageManager.createNewImage(width, height, finalName);
    });
    
    currentToolName = "Pencil";
    statusBar.updateToolInfo(currentToolName);
}

void Application::setupMenus() {
    menuBar.addMenu("File");
    
    menuBar.addMenuItem("File", "New Image", [this]() {
        newImage();
    });
    
    menuBar.addMenuItem("File", "Open", [this]() {
        openFile();
    });
    
    menuBar.addMenuItem("File", "Save", [this]() {
        saveFile();
    });
    
    menuBar.addMenuItem("File", "Save As", [this]() {
        saveAsFile();
    });
    
    menuBar.addMenuItem("File", "Close Tab", [this]() {
        imageManager.closeCurrentImage();
    });
    
    menuBar.addMenu("Edit");
    
    menuBar.addMenuItem("Edit", "Undo", [this]() {
        undo();
    });
    
    menuBar.addMenuItem("Edit", "Redo", [this]() {
        redo();
    });
    
    menuBar.addMenuItem("Edit", "Copy", [this]() {
        Image* currentImage = imageManager.getCurrentImage();
        if (currentImage) {
            currentImage->copySelectionToClipboard();
        }
    });
    
    menuBar.addMenuItem("Edit", "Paste", [this]() {
        Image* currentImage = imageManager.getCurrentImage();
        if (currentImage) {
            if (currentImage->pasteClipboardAsFloating()) {
                // Center pasted content near top-left for now
            }
        }
    });
    
    menuBar.addMenu("Select");
    
    menuBar.addMenuItem("Select", "Select All", [this]() {
        selectAll();
    });
    
    menuBar.addMenuItem("Select", "Deselect All", [this]() {
        deselectAll();
    });
    
    menuBar.addMenu("View");
    
    menuBar.addMenuItem("View", "Toggle Grid", [this]() {
        toggleGrid();
    });
    
    menuBar.addMenuItem("View", "Toggle Rulers", [this]() {
        toggleRulers();
    });
    
    menuBar.addMenuItem("View", "Center Image", [this]() {
        centerImage();
    });
    
    menuBar.addMenuItem("View", "Fit Image to View", [this]() {
        fitImageToView();
    });
    
    menuBar.addMenu("Image");
    
    menuBar.addMenuItem("Image", "Flip Horizontal", [this]() {
        flipHorizontal();
    });
    
    menuBar.addMenuItem("Image", "Flip Vertical", [this]() {
        flipVertical();
    });
    
    menuBar.addMenuItem("Image", "Rotate 90", [this]() {
        rotate90();
    });
    
    menuBar.addMenuItem("Image", "Rotate 180", [this]() {
        rotate180();
    });
    
    menuBar.addMenuItem("Image", "Rotate 270", [this]() {
        rotate270();
    });
    
    menuBar.addMenuItem("Image", "Crop to Selection", [this]() {
        cropToSelection();
    });
}

void Application::setupToolPanel() {
    toolPanel.addTool("select", "Select", [this]() {
        currentTool = ToolFactory::createTool("select");
        currentToolName = "Select";
        statusBar.updateToolInfo(currentToolName);
        setupToolCallbacks();
    });
    
    toolPanel.addTool("pencil", "Pencil", [this]() {
        currentTool = ToolFactory::createTool("pencil");
        currentToolName = "Pencil";
        statusBar.updateToolInfo(currentToolName);
        setupToolCallbacks();
        if (auto t = dynamic_cast<PencilTool*>(currentTool.get())) {
            toolOptions.configure("Pencil", "Size", 1, 500, 5, [t](float v){ t->setBrushSize(v); },
                                               "Opacity", 0, 1, 1, [t](float v){ t->setBrushOpacity(v); },
                                               "Hardness", 0, 1, 1, [t](float v){ t->setBrushHardness(v); });
        }
    });
    
    toolPanel.addTool("pan", "Pan", [this]() {
        currentTool = ToolFactory::createTool("pan");
        currentToolName = "Pan";
        statusBar.updateToolInfo(currentToolName);
        setupToolCallbacks();
    });
    
    toolPanel.addTool("eyedropper", "Eyedropper", [this]() {
        currentTool = ToolFactory::createTool("eyedropper");
        currentToolName = "Eyedropper";
        statusBar.updateToolInfo(currentToolName);
        setupToolCallbacks();
    });

    toolPanel.addTool("eraser", "Eraser", [this]() {
        currentTool = ToolFactory::createTool("eraser");
        currentToolName = "Eraser";
        statusBar.updateToolInfo(currentToolName);
        setupToolCallbacks();
        if (auto t = dynamic_cast<EraserTool*>(currentTool.get())) {
            toolOptions.configure("Eraser", "Size", 1, 500, 20, [t](float v){ t->setSize(v); },
                                               "Opacity", 0, 1, 1, [t](float v){ t->setOpacity(v); },
                                               "Hardness", 0, 1, 1, [t](float v){ t->setHardness(v); });
        }
    });

    toolPanel.addTool("airbrush", "Airbrush", [this]() {
        currentTool = ToolFactory::createTool("airbrush");
        currentToolName = "Airbrush";
        statusBar.updateToolInfo(currentToolName);
        setupToolCallbacks();
        if (auto t = dynamic_cast<AirbrushTool*>(currentTool.get())) {
            toolOptions.configure("Airbrush", "Size", 1, 500, 30, [t](float v){ t->setSize(v); },
                                                  "Opacity", 0, 1, 0.2f, [t](float v){ t->setOpacity(v); },
                                                  "Flow", 0, 1, 0.5f, [t](float v){ t->setFlow(v); });
        }
    });

    toolPanel.addTool("bucket", "Bucket Fill", [this]() {
        currentTool = ToolFactory::createTool("bucket");
        currentToolName = "Bucket";
        statusBar.updateToolInfo(currentToolName);
        setupToolCallbacks();
        if (auto t = dynamic_cast<BucketFillTool*>(currentTool.get())) {
            toolOptions.configure("Bucket Fill", "Tolerance", 0, 1020, 0, [t](float v){ t->setTolerance(v); },
                                                     "-", 0, 1, 0, nullptr,
                                                     "-", 0, 1, 0, nullptr);
        }
    });

    toolPanel.addTool("gradient", "Gradient", [this]() {
        currentTool = ToolFactory::createTool("gradient");
        currentToolName = "Gradient";
        statusBar.updateToolInfo(currentToolName);
        setupToolCallbacks();
        if (auto t = dynamic_cast<GradientTool*>(currentTool.get())) {
            toolOptions.configure("Gradient", "Opacity", 0, 1, 1, [t](float v){ t->setOpacity(v); },
                                                  "Type(0=L,1=R)", 0, 1, 0, [t](float v){ t->setType(v<0.5f? GradientTool::Linear:GradientTool::Radial); },
                                                  "Mode(0=FB,1=FT)", 0, 1, 0, [t](float v){ t->setMode(v<0.5f? GradientTool::FGtoBG:GradientTool::FGtoTransparent); });
        }
    });
    
    toolPanel.setSelectedTool("pencil");
    statusBar.updateToolInfo("Select");
    colorPanel.setPosition({10,610});
    toolOptions.setPosition({10,690});
    colorPanel.setOnOpenPicker([this](bool fg){ colorPicker.show(fg); });
}

void Application::setupToolCallbacks() {
    if (auto selectTool = dynamic_cast<SelectTool*>(currentTool.get())) {
        selectTool->setOnSelectionChanged([this](const Selection& selection) {
            Image* currentImage = imageManager.getCurrentImage();
            if (currentImage) {
                currentImage->setSelection(selection);
            }
        });
    }
}

void Application::setupImageManagerCallbacks() {
    imageManager.setOnSaveRequest([this](size_t imageIndex) {
        Image* image = imageManager.getImage(imageIndex);
        if (image) {
            if (image->getFilePath().empty()) {
                std::string filePath = saveFileDialog();
                if (!filePath.empty()) {
                    image->saveToFile(filePath);
                    imageManager.notifyImageModified(imageIndex);
                    imageManager.forceCloseImage(imageIndex);
                }
            } else {
                image->saveToFile(image->getFilePath());
                imageManager.notifyImageModified(imageIndex);
                imageManager.forceCloseImage(imageIndex);
            }
        }
    });
    
    imageManager.setOnSaveAsRequest([this](size_t imageIndex) {
        Image* image = imageManager.getImage(imageIndex);
        if (image) {
            std::string filePath = saveFileDialog();
            if (!filePath.empty()) {
                image->saveToFile(filePath);
                imageManager.notifyImageModified(imageIndex);
            }
        }
    });
}

void Application::handleImageInput(const sf::Event& event) {
    if (!currentTool) return;
    Image* currentImage = imageManager.getCurrentImage();
    if (!currentImage) return;
    sf::Vector2i pixelPos;
    sf::Vector2f pos;
    if (event.type == sf::Event::MouseButtonPressed) {
        pixelPos = {event.mouseButton.x, event.mouseButton.y};
        pos = window.mapPixelToCoords(pixelPos);
        if (imageManager.isPositionInCurrentImage(pos)) {
            currentTool->onMousePressed(pos, currentImage, event.mouseButton.button);
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased) {
        pixelPos = {event.mouseButton.x, event.mouseButton.y};
        pos = window.mapPixelToCoords(pixelPos);
        currentTool->onMouseReleased(pos, currentImage, event.mouseButton.button);
    }
    else if (event.type == sf::Event::MouseMoved) {
        pixelPos = {event.mouseMove.x, event.mouseMove.y};
        pos = window.mapPixelToCoords(pixelPos);
        currentTool->onMouseMoved(pos, currentImage);
    }
}

void Application::handleZoom(const sf::Event& event) {
    if (event.type == sf::Event::MouseWheelScrolled) {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f pos = window.mapPixelToCoords(pixelPos);
        if (imageManager.isPositionInCurrentImage(pos)) {
            if (event.mouseWheelScroll.delta > 0) {
                imageManager.zoomInAt(pos);
            } else {
                imageManager.zoomOutAt(pos);
            }
            
            Image* currentImage = imageManager.getCurrentImage();
            if (currentImage) {
                statusBar.updateZoomLevel(currentImage->getZoom());
            }
        }
    }
}

void Application::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        imageManager.handleSaveDialogEvent(event, window);
        
        newImageDialog.handleEvent(event, window);
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.control && event.key.code == sf::Keyboard::S) {
                saveFile();
            } else if (event.key.control && event.key.code == sf::Keyboard::N) {
                newImage();
            } else if (event.key.control && event.key.code == sf::Keyboard::O) {
                openFile();
            } else if (event.key.control && event.key.code == sf::Keyboard::Z) {
                if (imageManager.getCurrentImage()) {
                    imageManager.getCurrentImage()->getHistoryManager().undo();
                }
            } else if (event.key.control && event.key.code == sf::Keyboard::Y) {
                if (imageManager.getCurrentImage()) {
                    imageManager.getCurrentImage()->getHistoryManager().redo();
                }
            } else if (event.key.code == sf::Keyboard::Add || event.key.code == sf::Keyboard::Equal) {
                imageManager.zoomIn();
            } else if (event.key.code == sf::Keyboard::Subtract || event.key.code == sf::Keyboard::Hyphen) {
                imageManager.zoomOut();
            } else if (event.key.code == sf::Keyboard::Num0) {
                imageManager.resetZoom();
            } else if (event.key.control && event.key.code == sf::Keyboard::Tab) {
                imageManager.nextImage();
            } else if (event.key.code == sf::Keyboard::G && event.key.control) {
                toggleGrid();
            } else if (event.key.code == sf::Keyboard::R && event.key.control) {
                toggleRulers();
            } else if (event.key.code == sf::Keyboard::Space) {
                currentTool = ToolFactory::createTool("pan");
            } else if (event.key.code == sf::Keyboard::H) {
                centerImage();
            } else if (event.key.code == sf::Keyboard::F) {
                fitImageToView();
            } else if (event.key.control && event.key.code == sf::Keyboard::W) {
                imageManager.closeCurrentImage();
            } else if (event.key.control && event.key.code == sf::Keyboard::Z) {
                undo();
            } else if (event.key.control && event.key.code == sf::Keyboard::Y) {
                redo();
            } else if (event.key.control && event.key.code == sf::Keyboard::A) {
                selectAll();
            } else if (event.key.control && event.key.shift && event.key.code == sf::Keyboard::A) {
                deselectAll();
            } else if (event.key.control && event.key.code == sf::Keyboard::C) {
                Image* currentImage = imageManager.getCurrentImage();
                if (currentImage) currentImage->copySelectionToClipboard();
            } else if (event.key.control && event.key.code == sf::Keyboard::V) {
                Image* currentImage = imageManager.getCurrentImage();
                if (currentImage) currentImage->pasteClipboardAsFloating();
            } else if (event.key.code == sf::Keyboard::X) {
                ColorManager::instance().swapColors();
            } else if (event.key.code == sf::Keyboard::D) {
                ColorManager::instance().reset();
            } else if (event.key.code == sf::Keyboard::C) {
                colorPicker.show(true);
            } else if (event.key.code == sf::Keyboard::B) {
                if (auto t = dynamic_cast<EraserTool*>(currentTool.get())) {
                    static bool toTransparent = false;
                    toTransparent = !toTransparent;
                    t->setMode(toTransparent ? EraserTool::ToTransparent : EraserTool::ToBackground);
                }
            }
        }
        
        if (event.type == sf::Event::KeyReleased) {
            if (event.key.code == sf::Keyboard::Space) {
                currentTool = ToolFactory::createTool("select");
            }
        }

        sf::Vector2i pixelPos;
        sf::Vector2f pos;

        if (event.type == sf::Event::MouseButtonPressed) {
            pixelPos = {event.mouseButton.x, event.mouseButton.y};
            pos = window.mapPixelToCoords(pixelPos);
            
            if (!newImageDialog.isVisible() && !colorPicker.isVisible()) {
                bool menuHandled = menuBar.handleClick(pos);
                
                bool tabHandled = false;
                if (!menuHandled && pos.y >= 30 && pos.y <= 62) {
                    tabHandled = imageManager.handleTabClick(pos);
                }
                
                if (!menuHandled && !tabHandled && pos.y > 62) {
                    toolPanel.handleClick(pos);
                    colorPanel.handleClick(pos, event.mouseButton.button);
                }
            }
        }
        
        if (event.type == sf::Event::MouseMoved) {
            pixelPos = {event.mouseMove.x, event.mouseMove.y};
            pos = window.mapPixelToCoords(pixelPos);
            
            if (!newImageDialog.isVisible() && !colorPicker.isVisible()) {
                menuBar.handleMouseMove(pos);
                imageManager.handleTabMouseMove(pos);
                updateStatusBar(pos);
            }
        }
        
        if (!newImageDialog.isVisible() && !colorPicker.isVisible()) {
            handleImageInput(event);
            handleZoom(event);
        }

        if (colorPicker.isVisible()) {
            colorPicker.handleEvent(event, window);
        } else {
            toolOptions.handleEvent(event);
        }
    }
}

void Application::render() {
    window.clear(sf::Color(45, 45, 48));
    
    Image* currentImage = imageManager.getCurrentImage();
    if (currentImage) {
        sf::Vector2f imagePos = currentImage->getViewPosition();
        sf::Vector2f imageSize = currentImage->getDisplaySize();
        float zoom = currentImage->getZoom();
        sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(mousePixelPos);
        
        if (rulers.getVisible()) {
            rulers.draw(window, imagePos, imageSize, zoom, mousePos);
        }
        
        imageManager.draw(window);
        
        if (grid.getVisible()) {
            grid.draw(window, imagePos, imageSize, zoom);
        }
        
        imageManager.notifyImageModified(imageManager.getCurrentImageIndex());
    } else {
        imageManager.draw(window);
    }
    
    toolPanel.draw(window);
    colorPanel.draw(window);
    toolOptions.draw(window);
    
    imageManager.drawTabs(window);
    
    statusBar.draw(window);
    colorPicker.draw(window);
    newImageDialog.draw(window);
    
    menuBar.draw(window);
    
    imageManager.drawSaveDialog(window);
    
    window.display();
}

void Application::run() {
    while (window.isOpen()) {
        processEvents();
        render();
    }
}

void Application::saveFile() {
    Image* currentImage = imageManager.getCurrentImage();
    if (!currentImage) return;
    
    if (currentImage->getFilePath().empty()) {
        std::string filePath = saveFileDialog();
        if (!filePath.empty()) {
            currentImage->saveToFile(filePath);
            imageManager.notifyImageModified(imageManager.getCurrentImageIndex());
        }
    } else {
        currentImage->saveToFile(currentImage->getFilePath());
        imageManager.notifyImageModified(imageManager.getCurrentImageIndex());
    }
}

void Application::saveAsFile() {
    Image* currentImage = imageManager.getCurrentImage();
    if (!currentImage) return;
    
    std::string filePath = saveFileDialog();
    if (!filePath.empty()) {
        currentImage->saveToFile(filePath);
        imageManager.notifyImageModified(imageManager.getCurrentImageIndex());
    }
}

void Application::openFile() {
    std::string filePath = openFileDialog();
    if (!filePath.empty()) {
        imageManager.openImage(filePath);
    }
}

void Application::newImage() {
    newImageDialog.show();
}

std::string Application::openFileDialog() {
    std::string command = "zenity --file-selection --file-filter='PNG files | *.png' --title='Open Image' 2>/dev/null";
    
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cout << "Enter file path to open: ";
        std::string path;
        std::getline(std::cin, path);
        return path;
    }
    
    char buffer[1024];
    std::string result = "";
    if (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result = buffer;
        if (!result.empty() && result.back() == '\n') {
            result.pop_back();
        }
    }
    pclose(pipe);
    
    if (result.empty()) {
        std::cout << "Enter file path to open: ";
        std::getline(std::cin, result);
    }
    
    return result;
}

std::string Application::saveFileDialog() {
    std::string command = "zenity --file-selection --save --file-filter='PNG files | *.png' --title='Save Image' --filename='untitled.png' 2>/dev/null";
    
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cout << "Enter file path to save: ";
        std::string path;
        std::getline(std::cin, path);
        if (path.find(".png") == std::string::npos) {
            path += ".png";
        }
        return path;
    }
    
    char buffer[1024];
    std::string result = "";
    if (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result = buffer;
        if (!result.empty() && result.back() == '\n') {
            result.pop_back();
        }
        
        if (!result.empty() && result.find(".png") == std::string::npos) {
            result += ".png";
        }
    }
    pclose(pipe);
    
    if (result.empty()) {
        std::cout << "Enter file path to save: ";
        std::getline(std::cin, result);
        if (result.find(".png") == std::string::npos) {
            result += ".png";
        }
    }
    
    return result;
}

void Application::updateStatusBar(const sf::Vector2f& mousePos) {
    Image* currentImage = imageManager.getCurrentImage();
    if (currentImage) {
        if (imageManager.isPositionInCurrentImage(mousePos)) {
            sf::Vector2f imagePos = currentImage->worldToImage(mousePos);
            statusBar.updateMousePosition(imagePos, currentImage->getZoom());
        }
        statusBar.updateZoomLevel(currentImage->getZoom());
        
        sf::Vector2i size = currentImage->getOriginalSize();
        statusBar.updateImageInfo(currentImage->getName(), size.x, size.y);
    }
}

void Application::toggleGrid() {
    grid.setVisible(!grid.getVisible());
}

void Application::toggleRulers() {
    rulers.setVisible(!rulers.getVisible());
}

void Application::centerImage() {
    imageManager.centerCurrentImage();
}

void Application::fitImageToView() {
    imageManager.fitCurrentImageToView();
}

void Application::undo() {
    Image* currentImage = imageManager.getCurrentImage();
    if (currentImage) {
        currentImage->getHistoryManager().undo();
    }
}

void Application::redo() {
    Image* currentImage = imageManager.getCurrentImage();
    if (currentImage) {
        currentImage->getHistoryManager().redo();
    }
}

void Application::selectAll() {
    Image* currentImage = imageManager.getCurrentImage();
    if (currentImage) {
        sf::Vector2i size = currentImage->getOriginalSize();
        Selection fullSelection;
        fullSelection.setRectangle(sf::FloatRect(0, 0, static_cast<float>(size.x), static_cast<float>(size.y)));
        currentImage->setSelection(fullSelection);
    }
}

void Application::deselectAll() {
    Image* currentImage = imageManager.getCurrentImage();
    if (currentImage) {
        currentImage->clearSelection();
    }
}

void Application::flipHorizontal() {
    Image* currentImage = imageManager.getCurrentImage();
    if (currentImage) {
        if (!currentImage->getSelection().isEmpty()) {
            sf::FloatRect b = currentImage->getSelection().getBounds();
            sf::IntRect r(static_cast<int>(b.left), static_cast<int>(b.top), static_cast<int>(b.width), static_cast<int>(b.height));
            auto command = std::make_unique<FlipSelectionCommand>(currentImage, FlipCommand::HORIZONTAL, r);
            currentImage->getHistoryManager().executeCommand(std::move(command));
        } else {
            auto command = std::make_unique<FlipCommand>(currentImage, FlipCommand::HORIZONTAL);
            currentImage->getHistoryManager().executeCommand(std::move(command));
        }
    }
}

void Application::flipVertical() {
    Image* currentImage = imageManager.getCurrentImage();
    if (currentImage) {
        if (!currentImage->getSelection().isEmpty()) {
            sf::FloatRect b = currentImage->getSelection().getBounds();
            sf::IntRect r(static_cast<int>(b.left), static_cast<int>(b.top), static_cast<int>(b.width), static_cast<int>(b.height));
            auto command = std::make_unique<FlipSelectionCommand>(currentImage, FlipCommand::VERTICAL, r);
            currentImage->getHistoryManager().executeCommand(std::move(command));
        } else {
            auto command = std::make_unique<FlipCommand>(currentImage, FlipCommand::VERTICAL);
            currentImage->getHistoryManager().executeCommand(std::move(command));
        }
    }
}

void Application::rotate90() {
    Image* currentImage = imageManager.getCurrentImage();
    if (currentImage) {
        if (!currentImage->getSelection().isEmpty()) {
            sf::FloatRect b = currentImage->getSelection().getBounds();
            sf::IntRect r(static_cast<int>(b.left), static_cast<int>(b.top), static_cast<int>(b.width), static_cast<int>(b.height));
            auto command = std::make_unique<RotateSelectionCommand>(currentImage, RotateCommand::ROTATE_90, r);
            currentImage->getHistoryManager().executeCommand(std::move(command));
        } else {
            auto command = std::make_unique<RotateCommand>(currentImage, RotateCommand::ROTATE_90);
            currentImage->getHistoryManager().executeCommand(std::move(command));
            imageManager.fitCurrentImageToView();
        }
    }
}

void Application::rotate180() {
    Image* currentImage = imageManager.getCurrentImage();
    if (currentImage) {
        if (!currentImage->getSelection().isEmpty()) {
            sf::FloatRect b = currentImage->getSelection().getBounds();
            sf::IntRect r(static_cast<int>(b.left), static_cast<int>(b.top), static_cast<int>(b.width), static_cast<int>(b.height));
            auto command = std::make_unique<RotateSelectionCommand>(currentImage, RotateCommand::ROTATE_180, r);
            currentImage->getHistoryManager().executeCommand(std::move(command));
        } else {
            auto command = std::make_unique<RotateCommand>(currentImage, RotateCommand::ROTATE_180);
            currentImage->getHistoryManager().executeCommand(std::move(command));
            imageManager.centerCurrentImage();
        }
    }
}

void Application::rotate270() {
    Image* currentImage = imageManager.getCurrentImage();
    if (currentImage) {
        if (!currentImage->getSelection().isEmpty()) {
            sf::FloatRect b = currentImage->getSelection().getBounds();
            sf::IntRect r(static_cast<int>(b.left), static_cast<int>(b.top), static_cast<int>(b.width), static_cast<int>(b.height));
            auto command = std::make_unique<RotateSelectionCommand>(currentImage, RotateCommand::ROTATE_270, r);
            currentImage->getHistoryManager().executeCommand(std::move(command));
        } else {
            auto command = std::make_unique<RotateCommand>(currentImage, RotateCommand::ROTATE_270);
            currentImage->getHistoryManager().executeCommand(std::move(command));
            imageManager.fitCurrentImageToView();
        }
    }
}

void Application::cropToSelection() {
    Image* currentImage = imageManager.getCurrentImage();
    if (currentImage && !currentImage->getSelection().isEmpty()) {
        sf::FloatRect selectionBounds = currentImage->getSelection().getBounds();
        sf::IntRect cropRect(
            static_cast<int>(selectionBounds.left),
            static_cast<int>(selectionBounds.top),
            static_cast<int>(selectionBounds.width),
            static_cast<int>(selectionBounds.height)
        );
        auto command = std::make_unique<CropCommand>(currentImage, cropRect);
        currentImage->getHistoryManager().executeCommand(std::move(command));
        currentImage->clearSelection();
        imageManager.centerCurrentImage();
    }
}
