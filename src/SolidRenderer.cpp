#include "SolidRenderer.hpp"

//#include <tbb/tbb.h>  // Include, nur wenn TBB genutzt werden soll

#define EPSILON \
  (1e-12)  // Epsilon um ungenauigkeiten und Rundungsfehler zu kompensieren

/**
 ** Erstellt mittels Raycast das Rendering der mScene in das mImage
 ** Precondition: Sowohl mImage, mScene, mCamera müssen gesetzt sein.
 **/
void SolidRenderer::renderRaycast() {
  // This function is part of the base

  std::cout << "Rendern mittels Raycast gestartet." << std::endl;
  // Berechnung der einzelnen Rows in eigener Methode um die
  // Parallelisierbarkeit zu verbessern

  // Ohne parallelisierung:
    
  //for(size_t i = 0; i < mImage->getHeight(); ++i ){
  //        computeImageRow( i );
  // }

  //  Parallelisierung mit OpenMP:
    
  #pragma omp parallel for
      for(long long i = 0; i < (long long) mImage->getHeight(); ++i )
      {
          computeImageRow( i );
      }

  // Parallelisierung mit TBB:
    
  //tbb::parallel_for(size_t(0), mImage->getHeight(),
  //                /* lambda funktion*/ [&](size_t i) { computeImageRow(i); });
}

/**
 * Aufgabenblatt 3: Hier wird das Raycasting implementiert. Siehe Aufgabenstellung!
 * Precondition: Sowohl mImage, mScene und mCamera  müssen gesetzt sein.
 */
void SolidRenderer::computeImageRow(size_t rowNumber) {
    for (size_t colNumber = 0; colNumber < mImage->getWidth(); ++colNumber) {
        auto ray = mCamera->getRay(colNumber, rowNumber);
        auto hitRecord = HitRecord();
        hitRecord.setDefaultValues();
        bool hit = mScene->intersect(ray, hitRecord, std::numeric_limits<float>::epsilon());
        if (hit) {
            hitRecord.recursions = 0;
            shade(hitRecord);
            mImage->setValue(colNumber, rowNumber, hitRecord.color);
        }
        else {
            mImage->setValue(colNumber, rowNumber, mImage->getBackgroundColor());
        }
    }
}

/**
 *  Aufgabenblatt 4: Hier wird das raytracing implementiert. Siehe Aufgabenstellung!
 */
void SolidRenderer::shade(HitRecord &r) {
    Material material = getMaterialFromHitRecord(r);
    r.color = material.color;
    double k_ambient = 0.4;
    double k_diffus = 0.4;
    double k_specular = 0.2;
    int n = 10;
    computeIllumination(k_ambient, k_diffus, k_specular, n, r);
    computeShadow(r);
    computeReflection(r, material);
}

Material SolidRenderer::getMaterialFromHitRecord(HitRecord& r) {
    if (r.modelId != -1) {
        return mScene->getModels()[r.modelId].getMaterial();
    }
    if (r.sphereId != -1) {
        return mScene->getSpheres()[r.sphereId].getMaterial();
    }
}

void SolidRenderer::computeIllumination(double k_ambient, double k_diffus, double k_specular, int n, HitRecord& r) {
    GLVector L = mScene->getPointLights()[0] - r.intersectionPoint;
    L.normalize();
    GLVector N = r.normal;
    GLVector V = mScene->getCamera()->getEyePoint() - r.intersectionPoint;
    V.normalize();
    GLVector R = 2 * dotProduct(N, L) * N - L;
    R.normalize();
    double I = k_ambient + k_diffus * dotProduct(L, N) + k_specular * std::pow(dotProduct(R, V), n);
    r.color *= I;
}

void SolidRenderer::computeShadow(HitRecord& r) {
    auto shadowRay = Ray();
    shadowRay.direction = mScene->getPointLights()[0] - r.intersectionPoint;
    shadowRay.direction.normalize();
    shadowRay.origin = r.intersectionPoint + shadowRay.direction * std::numeric_limits<float>::epsilon();
    auto shadowHitRecord = HitRecord();
    shadowHitRecord.setDefaultValues();
    if (mScene->intersect(shadowRay, shadowHitRecord, std::numeric_limits<float>::epsilon())
        && shadowHitRecord.parameter < getDistance(r.intersectionPoint, mScene->getPointLights()[0])
        && shadowHitRecord.parameter > 0) {
        r.color *= 0.4;
    }
}

void SolidRenderer::computeReflection(HitRecord& r, Material material) {
    if (material.reflection > 0) {
        auto reflectionRay = Ray();
        GLVector N = r.normal;
        GLVector V = mScene->getCamera()->getEyePoint() - r.intersectionPoint;
        V.normalize();
        reflectionRay.direction = 2 * dotProduct(N, V) * N - V;
        reflectionRay.direction.normalize();
        reflectionRay.origin = r.intersectionPoint + reflectionRay.direction * std::numeric_limits<float>::epsilon();
        if (material.reflection == 1.0) {
            r.color = Color(0, 0, 0);
        }
        r.setDefaultValues();
        if (mScene->intersect(reflectionRay, r, std::numeric_limits<float>::epsilon())) {
            r.recursions++;
            if (r.recursions > 5) return;
            shade(r);
        }
    }
}
