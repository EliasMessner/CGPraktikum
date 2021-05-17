#include "Scene.hpp"
#include "math.hpp"

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <vector>

Scene::Scene() {}


/**
 * Gibt zurück ob ein gegebener Strahl ein Objekt (Modell oder Kugel) der Szene trifft
 *  (Aufgabenblatt 3)
 */
bool Scene::intersect(const Ray &ray, HitRecord &hitRecord,
                      const float epsilon) {
    for each (auto &sphere in mSpheres) {
        if (sphereIntersect(ray, sphere, hitRecord, epsilon)) {
            return true;
        }
    }
    for each (auto &model in mModels) {
        for each (auto &triangle in model.mTriangles) {
            Triangle tempTriangle = triangle.transform(model.getTransformation());
            if (triangleIntersect(ray, tempTriangle, hitRecord, epsilon)) {
                return true;
            }
        }
    }
    return false;
}

/** Aufgabenblatt 3: Gibt zurück ob ein gegebener Strahl ein Dreieck  eines Modells der Szene trifft
 *  Diese Methode sollte in Scene::intersect für jedes Objektdreieck aufgerufen werden
 *  Aufgabenblatt 4: Diese Methode befüllt den den HitRecord im Fall eines Treffers mit allen für das shading notwendigen informationen
 */
bool Scene::triangleIntersect(const Ray &ray, const Triangle &triangle,
                              HitRecord &hitRecord, const float epsilon) {
    GLVector n = triangle.normal;
    GLPoint p = triangle.vertex[0];
    GLPoint e = ray.origin;
    GLVector v = ray.direction;
    double t = dotProduct((p - e), n) / dotProduct(v, n);
    GLPoint intersecPoint = e + t * v;
    if (triangle.contains(intersecPoint)) {
        hitRecord.intersectionPoint = intersecPoint;
        hitRecord.parameter = getDistance(intersecPoint, ray.origin);
        return true;
    }
    else {
        return false;
    }
}

/** Aufgabenblatt 3: Gibt zurück ob ein gegebener Strahl eine Kugel der Szene trifft
 *  Diese Methode sollte in Scene::intersect für jede Kugel aufgerufen werden
 *  Aufgabenblatt 4: Diese Methode befüllt den den HitRecord im Fall eines Treffers mit allen für das shading notwendigen informationen
*/
bool Scene::sphereIntersect(const Ray &ray, const Sphere &sphere,
                            HitRecord &hitRecord, const float epsilon) {
    double e_1 = ray.origin(0);
    double e_2 = ray.origin(1);
    double e_3 = ray.origin(2);
    double v_1 = ray.direction(0);
    double v_2 = ray.direction(1);
    double v_3 = ray.direction(2);
    double m_1 = sphere.getPosition()(0);
    double m_2 = sphere.getPosition()(1);
    double m_3 = sphere.getPosition()(2);
    double r = sphere.getRadius();
    double radicand = std::pow((2 * e_1 * v_1 + 2 * e_2 * v_2 + 2 * e_3 * v_3 - 2 * m_2 * v_1 - 2 * m_2 * v_2 * -2 * m_3 * v_3), 2) - 4 * (std::pow(v_1, 2) + std::pow(v_2, 2) + std::pow(v_3, 2)) * (-2 * e_1 * m_2 - 2 * e_2 * m_2 - 2 * e_3 * m_3 + std::pow(e_1, 2) + std::pow(e_2, 2) + std::pow(e_3, 2) + 2 * std::pow(m_2, 2) + std::pow(m_3, 2) - std::pow(r, 2));
    if (radicand < 0.0) {
        return false;
    }
    double t;
    if (areSame(radicand, 0.0)) {
        t = (- 2 * e_1 * v_1 - 2 * e_2 * v_2 - 2 * e_3 * v_3 + 2 * m_2 * v_1 + 2 * m_2 * v_2 + 2 * m_3 * v_3) / (2 * (std::pow(v_1, 2) + std::pow(v_2, 2) + std::pow(v_3, 2)));
    }
    else {
        double t_1 = (-sqrt(radicand) - 2 * e_1 * v_1 - 2 * e_2 * v_2 - 2 * e_3 * v_3 + 2 * m_2 * v_1 + 2 * m_2 * v_2 + 2 * m_3 * v_3) / (2 * (std::pow(v_1, 2) + std::pow(v_2, 2) + std::pow(v_3, 2)));
        double t_2 = (sqrt(std::pow((2 * e_1 * v_1 + 2 * e_2 * v_2 + 2 * e_3 * v_3 - 2 * m_2 * v_1 - 2 * m_2 * v_2 * -2 * m_3 * v_3), 2) - 4 * (std::pow(v_1, 2) + std::pow(v_2, 2) + std::pow(v_3, 2)) * (-2 * e_1 * m_2 - 2 * e_2 * m_2 - 2 * e_3 * m_3 + std::pow(e_1, 2) + std::pow(e_2, 2) + std::pow(e_3, 2) + 2 * std::pow(m_2, 2) + std::pow(m_3, 2) - std::pow(r, 2))) - 2 * e_1 * v_1 - 2 * e_2 * v_2 - 2 * e_3 * v_3 + 2 * m_2 * v_1 + 2 * m_2 * v_2 + 2 * m_3 * v_3) / (2 * (std::pow(v_1, 2) + std::pow(v_2, 2) + std::pow(v_3, 2)));
        t = fmin(t_1, t_2);
    }
    GLPoint intersecPoint = ray.origin + t * ray.direction;
    hitRecord.intersectionPoint = intersecPoint;
    hitRecord.parameter = getDistance(intersecPoint, ray.origin);
    return true;
}

/**
** Liest die Modelle (deren Dateinamen in pFiles stehen) ein und speichert sie
*in mModels
**/
void Scene::load(const std::vector<std::string> &pFiles) {
  std::cout << "Laden der PLY Dateien:" << std::endl;
  // Für alle Objekte (Objekt meint hier das selbe wie Model)
  for (int obj_nr = 0; obj_nr < pFiles.size(); obj_nr++) {
    std::cout << "\tModel-Index: " << obj_nr << std::endl;
    // Assimp übernimmt das Einlesen der ply-Dateien
    Assimp::Importer importer;
    const aiScene *assimpScene = importer.ReadFile(
        pFiles[obj_nr], aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                            aiProcess_JoinIdenticalVertices |
                            aiProcess_SortByPType);
    assert(assimpScene);
    auto meshes = assimpScene->mMeshes;
    // Neues Model erstellen
    Model model = Model();

    // Für alle Meshes des Models
    for (int i = 0; i < assimpScene->mNumMeshes; i++) {
      std::cout << "\t\tMesh-Index: " << i << " (" << meshes[i]->mNumFaces
                << " Faces)" << std::endl;
      auto faces = meshes[i]->mFaces;
      auto vertices = meshes[i]->mVertices;

      // Für alle Faces einzelner Meshes
      for (int j = 0; j < meshes[i]->mNumFaces; j++) {
        // Dreieck konstruieren und nötige Werte berechnen
        Triangle tri;
        assert(faces[j].mNumIndices == 3);
        tri.vertex[0] = GLPoint(vertices[faces[j].mIndices[0]].x,
                                vertices[faces[j].mIndices[0]].y,
                                vertices[faces[j].mIndices[0]].z);
        tri.vertex[1] = GLPoint(vertices[faces[j].mIndices[1]].x,
                                vertices[faces[j].mIndices[1]].y,
                                vertices[faces[j].mIndices[1]].z);
        tri.vertex[2] = GLPoint(vertices[faces[j].mIndices[2]].x,
                                vertices[faces[j].mIndices[2]].y,
                                vertices[faces[j].mIndices[2]].z);
        GLVector normal = crossProduct(tri.vertex[1] - tri.vertex[0],
                                       tri.vertex[2] - tri.vertex[0]);
        normal.normalize();
        tri.normal = normal;
        // Jedes Dreieck zum Vector der Dreiecke des aktuellen Models hinzufügen
        model.mTriangles.push_back(tri);
      }
    }
    // Immer das gleiche Material für das Model setzen
    Material material;
    material.color = Color(0.00, 1.00, 0.00);
    model.setMaterial(material);
    // Jedes Model zum Vector der Models der Scene hinzufügen
    mModels.push_back(model);
  }

  std::cout << "Laden der PLY Dateien abgeschlossen." << std::endl;
}

void Scene::setCamera(std::shared_ptr<Camera> cam) { mCamera = cam; }

std::shared_ptr<Camera> Scene::getCamera() const { return mCamera; }

GLPoint Scene::getViewPoint() const {
  if (mCamera)
    return mCamera->getEyePoint();
  else {
    std::cerr << "No Camera set to get view point from." << std::endl;
    return GLPoint(0, 0, 0);
  }
}



void Scene::addPointLight(GLPoint pointLight) {
  mPointLights.push_back(pointLight);
}

void Scene::addModel(Model model) { mModels.push_back(model); }

void Scene::addSphere(Sphere sphere) { mSpheres.push_back(sphere); }

std::vector<Model> &Scene::getModels() { return mModels; }

std::vector<Sphere> &Scene::getSpheres() { return mSpheres; }

std::vector<GLPoint> &Scene::getPointLights() { return mPointLights; }
