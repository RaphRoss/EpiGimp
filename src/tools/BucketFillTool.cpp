#include "BucketFillTool.hpp"
#include "../core/ColorManager.hpp"
#include "../core/Command.hpp"
#include <queue>
#include <cmath>

static inline int rd(int a, int b) { return std::abs(a - b); }
static inline int colorDist(const sf::Color& a, const sf::Color& b) {
    return rd(a.r,b.r) + rd(a.g,b.g) + rd(a.b,b.b) + rd(a.a,b.a);
}

void BucketFillTool::onMousePressed(const sf::Vector2f& pos, Image* image, sf::Mouse::Button button) {
    (void)button;
    if (!image) return;
    sf::Vector2f ip = image->worldToImage(pos);
    if (ip.x < 0 || ip.y < 0) return;
    sf::Image before = image->getImageData();
    unsigned int w = before.getSize().x, h = before.getSize().y;
    unsigned int sx = static_cast<unsigned int>(ip.x);
    unsigned int sy = static_cast<unsigned int>(ip.y);
    if (sx >= w || sy >= h) return;
    sf::Color target = before.getPixel(sx, sy);
    sf::Color fill = ColorManager::instance().getForeground();
    if (colorDist(target, fill) == 0) return;
    sf::Image after = before;
    std::vector<char> visited(w*h, 0);
    std::queue<sf::Vector2u> q;
    q.push({sx,sy}); visited[sy*w+sx]=1;
    const Selection& sel = image->getSelection();
    while (!q.empty()) {
        auto p = q.front(); q.pop();
        sf::Vector2f pf(static_cast<float>(p.x), static_cast<float>(p.y));
        if (!sel.isEmpty() && !sel.contains(pf)) continue;
        sf::Color cur = after.getPixel(p.x, p.y);
        if (colorDist(cur, target) <= static_cast<int>(tolerance)) after.setPixel(p.x, p.y, fill);
        else continue;
        if (p.x>0) { unsigned int nx=p.x-1, ny=p.y; if (!visited[ny*w+nx]) { visited[ny*w+nx]=1; q.push({nx,ny}); } }
        if (p.x+1<w) { unsigned int nx=p.x+1, ny=p.y; if (!visited[ny*w+nx]) { visited[ny*w+nx]=1; q.push({nx,ny}); } }
        if (p.y>0) { unsigned int nx=p.x, ny=p.y-1; if (!visited[ny*w+nx]) { visited[ny*w+nx]=1; q.push({nx,ny}); } }
        if (p.y+1<h) { unsigned int nx=p.x, ny=p.y+1; if (!visited[ny*w+nx]) { visited[ny*w+nx]=1; q.push({nx,ny}); } }
    }
    auto cmd = std::make_unique<ApplyImageCommand>(image, before, after);
    image->getHistoryManager().executeCommand(std::move(cmd));
}
