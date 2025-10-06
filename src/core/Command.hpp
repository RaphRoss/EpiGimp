#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class Image;

class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual std::unique_ptr<Command> clone() const = 0;
};

class DrawCommand : public Command {
public:
    DrawCommand(Image* image, const sf::Vector2f& from, const sf::Vector2f& to, const sf::Color& color);
    
    void execute() override;
    void undo() override;
    std::unique_ptr<Command> clone() const override;

private:
    Image* targetImage;
    sf::Vector2f fromPos;
    sf::Vector2f toPos;
    sf::Color drawColor;
    sf::Image backupRegion;
    sf::Vector2i regionPos;
    sf::Vector2i regionSize;
    
    void saveBackup();
    void restoreBackup();
};

class StrokeCommand : public Command {
public:
    StrokeCommand(Image* image);
    
    void addPoint(const sf::Vector2f& point, const sf::Color& color);
    void execute() override;
    void undo() override;
    std::unique_ptr<Command> clone() const override;

private:
    Image* targetImage;
    std::vector<std::pair<sf::Vector2f, sf::Color>> points;
    sf::Image backupImage;
    bool backupSaved = false;
    
    void saveBackup();
    void restoreBackup();
};
