#pragma once

#include "Image.hpp"
#include "Model.hpp"
#include "Scene.hpp"

class WireframeRenderer {
 public:
    // construct WireframeRenderer. Pointers mImage, mScene are are constructed by
    // passing the pointers image, scene. (pointers image and scene are ivalidated
    // )
    WireframeRenderer(std::shared_ptr<Scene> scene, std::shared_ptr<Image> image)
        : mImage(image), mScene(scene) {}

    void renderScene(Color color);

    void drawBresenhamLine(GLPoint p1, GLPoint p2, Color color);
    void seedFillArea(GLPoint seed, Color borderColor, Color fillColor);

    std::shared_ptr<Image> mImage;
    std::shared_ptr<Scene> mScene;
private:
    void drawBresenhamLine1(GLPoint p1, GLPoint p2, Color color);
    void drawBresenhamLine2(GLPoint p1, GLPoint p2, Color color);
    void drawBresenhamLine3(GLPoint p1, GLPoint p2, Color color);
    void drawBresenhamLine4(GLPoint p1, GLPoint p2, Color color);
    void drawBresenhamLine5(GLPoint p1, GLPoint p2, Color color);
    void drawBresenhamLine6(GLPoint p1, GLPoint p2, Color color);
    void drawBresenhamLine7(GLPoint p1, GLPoint p2, Color color);
    void drawBresenhamLine8(GLPoint p1, GLPoint p2, Color color);
};
