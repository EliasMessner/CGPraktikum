
#include "WireframeRenderer.hpp"
#include <stack>
#include <array>

/**
** Zeichnet alle Dreiecke der Scene als Wireframe-Rendering unter Verwendung des
* Bresenham-Algorithmus
** Precondition: Sowohl mImage als auch mScene müssen gesetzt sein.
** (Aufgabenblatt 2 - Aufgabe 1)
**/
void WireframeRenderer::renderScene(Color color) {
    for each (auto model in mScene->getModels()) {
        for each (auto triangle in model.mTriangles) {
            for (int i = 0; i < 3; i++) {
                drawBresenhamLine(
                    model.getTransformation() * triangle.vertex[i],
                    model.getTransformation() * triangle.vertex[(i + 1) % 3],
                    color);
            }
        }
    }
}

/**
** Zeichnet unter Verwendung des Bresenham Algorithmus eine Linie zwischen p1
* und p2 (nutzt x & y Komponente - z Komponente wird ignoriert)
** Precondition: Das mImage muss gesetzt sein.
** (Aufgabenblatt 1 - Aufgabe 2)
**/
void WireframeRenderer::drawBresenhamLine(GLPoint p1, GLPoint p2, Color color) {
    int dx, dy;
    dx = p2(0) - p1(0);
    dy = p2(1) - p1(1);
    if (dx < 0) {
        if (dy < 0) {
            if (std::abs(dx) < std::abs(dy)) {
                //oct = 6
                this->drawBresenhamLine6(p1, p2, color);
            }
            else {
                //oct = 5
                this->drawBresenhamLine5(p1, p2, color);
            }
        }
        else {
            if (std::abs(dx) < std::abs(dy)) {
                //oct = 3
                this->drawBresenhamLine3(p1, p2, color);
            }
            else {
                //oct = 4
                this->drawBresenhamLine4(p1, p2, color);
            }
        }
    }
    else {
        if (dy < 0) {
            if (std::abs(dx) < std::abs(dy)) {
                //oct = 7
                this->drawBresenhamLine7(p1, p2, color);
            }
            else {
                //oct = 8
                this->drawBresenhamLine8(p1, p2, color);
            }
        }
        else {
            if (std::abs(dx) < std::abs(dy)) {
                //oct = 2
                this->drawBresenhamLine2(p1, p2, color);
            }
            else {
                //oct = 1
                this->drawBresenhamLine1(p1, p2, color);
            }
        }
    }
}

void WireframeRenderer::drawBresenhamLine1(GLPoint p1, GLPoint p2, Color color) {
    int dx = p2(0) - p1(0);
    int dy = p2(1) - p1(1);
    int x = p1(0);
    int y = p1(1);
    int e = 2 * dy - dx;
    for (int i = 1; i <= dx; i++) {
        mImage->setValue(x, y, color);
        if (e >= 0) {
            ++y;
            e -= 2 * dx;
        }
        ++x;
        e += 2 * dy;
    }
    mImage->setValue(x, y, color);
}

void WireframeRenderer::drawBresenhamLine2(GLPoint p1, GLPoint p2, Color color) {
    int dx = p2(0) - p1(0);
    int dy = p2(1) - p1(1);
    int x = p1(0);
    int y = p1(1);
    int e = 2 * dx - dy;
    for (int i = 1; i <= dy; i++) {
        mImage->setValue(x, y, color);
        if (e >= 0) {
            ++x;
            e -= 2 * dy;
        }
        ++y;
        e += 2 * dx;
    }
    mImage->setValue(x, y, color);
}

void WireframeRenderer::drawBresenhamLine3(GLPoint p1, GLPoint p2, Color color) {
    drawBresenhamLine7(p2, p1, color);
}

void WireframeRenderer::drawBresenhamLine4(GLPoint p1, GLPoint p2, Color color) {
    drawBresenhamLine8(p2, p1, color);
}

void WireframeRenderer::drawBresenhamLine5(GLPoint p1, GLPoint p2, Color color) {
    drawBresenhamLine1(p2, p1, color);
}

void WireframeRenderer::drawBresenhamLine6(GLPoint p1, GLPoint p2, Color color) {
    drawBresenhamLine2(p2, p1, color);
}

void WireframeRenderer::drawBresenhamLine7(GLPoint p1, GLPoint p2, Color color) {
    int dx = p2(0) - p1(0);
    int dy = p1(1) - p2(1);
    int x = p1(0);
    int y = p1(1);
    int e = 2 * dx - dy;
    for (int i = 1; i <= dy; i++) {
        mImage->setValue(x, y, color);
        if (e >= 0) {
            ++x;
            e -= 2 * dy;
        }
        --y;
        e += 2 * dx;
    }
    mImage->setValue(x, y, color);
}

void WireframeRenderer::drawBresenhamLine8(GLPoint p1, GLPoint p2, Color color) {
    int dx = p2(0) - p1(0);
    int dy = p1(1) - p2(1);
    int x = p1(0);
    int y = p1(1);
    int e = 2 * dy - dx;
    for (int i = 1; i <= dx; i++) {
        mImage->setValue(x, y, color);
        if (e >= 0) {
            --y;
            e -= 2 * dx;
        }
        ++x;
        e += 2 * dy;
    }
    mImage->setValue(x, y, color);
}

/**
** Füllt einen vorgegebenen Bereich (abgegrenzt durch Randfarbe/borderColor) mit einer vorgegebenen Farbe (fillColor).
** Precondition: Das mImage muss gesetzt sein.
** (Aufgabenblatt 1 - Aufgabe 3) 
**/
void WireframeRenderer::seedFillArea(GLPoint seed, Color borderColor, Color fillColor) {
    std::stack<GLPoint> pointStack;
    pointStack.push(seed);
    while (!pointStack.empty()) {
        GLPoint current = pointStack.top();
        int x = current(0);
        int y = current(1);
        pointStack.pop();
        mImage->setValue(x, y, fillColor);
        int sides[4][2] = { 0, 1, 0, -1, 1, 0, -1, 0 };
        for (auto side : sides) {
            int _x = x + side[0];
            int _y = y + side[1];
            GLPoint neighborPoint = GLPoint(_x, _y, 0);
            Color neighborColor = mImage->getValues()[_y * mImage->getWidth() + _x];
            if (!Color::isEqual(neighborColor, borderColor)
                    && !Color::isEqual(neighborColor, fillColor)
                    && mImage->contains(neighborPoint)) {
                pointStack.push(neighborPoint);
            }
        }
    }
}



