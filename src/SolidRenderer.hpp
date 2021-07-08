#pragma once

#include "Camera.hpp"
#include "Image.hpp"
#include "Scene.hpp"

class SolidRenderer {
 public:
  SolidRenderer(std::shared_ptr<Scene> scene, std::shared_ptr<Image> image,
                std::shared_ptr<Camera> camera)
      : mImage(image), mScene(scene), mCamera(camera) {}

  void renderRaycast();

  void computeImageRow(size_t rowNumber);

  void shade(HitRecord &r);

  Material getMaterialFromHitRecord(HitRecord& r);

  void computeIllumination(double k_ambient, double k_diffus, double k_specular, int n, HitRecord& r);

  void computeShadow(HitRecord& r);

  void computeReflection(HitRecord& r, Material material);

  std::shared_ptr<Image> mImage;
  std::shared_ptr<Scene> mScene;
  std::shared_ptr<Camera> mCamera;
};
