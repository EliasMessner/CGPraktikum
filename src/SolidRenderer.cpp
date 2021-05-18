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
        hitRecord.color = Color(0, 0.7, 0.7); // TODO pass color as parameter?
        hitRecord.parameter = -1;
        hitRecord.triangleId = -1;
        hitRecord.sphereId = -1;
        bool hit = mScene->intersect(ray, hitRecord, std::numeric_limits<float>::epsilon());
        mImage->setValue(colNumber, rowNumber, hit ? hitRecord.color : mImage->getBackgroundColor());
    }
}

/**
 *  Aufgabenblatt 4: Hier wird das raytracing implementiert. Siehe Aufgabenstellung!
 */
void SolidRenderer::shade(HitRecord &r) {
}
