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
    bool hit = false;
    int sphereId = 0;
    for each (auto &sphere in mSpheres) {
        if (sphereIntersect(ray, sphere, hitRecord, epsilon)) {
            hit = true;
            hitRecord.sphereId = sphereId;
        }
        sphereId++;
    }
    int modelId = 0;
    for each (auto &model in mModels) {
        int triangleId = 0;
        for each (auto &triangle in model.mTriangles) {
            Triangle tempTriangle = triangle.transform(model.getTransformation());
            if (triangleIntersect(ray, tempTriangle, hitRecord, epsilon)) {
                hit = true;
                hitRecord.modelId = modelId;
                hitRecord.triangleId = triangleId;
            }
            triangleId++;
        }
        modelId++;
    }
    return hit;
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
    if (areSame(dotProduct(v, n), 0.0)) return false; // ray is parallel to triangle plane -> no intersection
    double t = dotProduct((p - e), n) / dotProduct(v, n);
    if (t <= 0) return false; // t negative -> intersecpoint behind camera
    GLPoint intersecPoint = e + t * v;
    if (!triangle.containsBaryzent(intersecPoint)) return false;
    if (t < hitRecord.parameter) {
        hitRecord.intersectionPoint = intersecPoint;
        hitRecord.parameter = t;
        hitRecord.rayDirection = ray.direction;
        hitRecord.normal = triangle.normal;
        return true;
    }
    return false;
}

/** Aufgabenblatt 3: Gibt zurück ob ein gegebener Strahl eine Kugel der Szene trifft
 *  Diese Methode sollte in Scene::intersect für jede Kugel aufgerufen werden
 *  Aufgabenblatt 4: Diese Methode befüllt den den HitRecord im Fall eines Treffers mit allen für das shading notwendigen informationen
*/
bool Scene::sphereIntersect(const Ray &ray, const Sphere &sphere,
                            HitRecord &hitRecord, const float epsilon) {
    GLPoint e = ray.origin;
    GLVector v = ray.direction;
    GLPoint m = sphere.getPosition();
    double r = sphere.getRadius();
    double radicand = dotProduct(v, (e - m)) * dotProduct(v, (e - m)) - (dotProduct((e - m), (e - m)) - r * r);
    if (radicand < 0) return false; // no intersection
    double t1 = -dotProduct(v, (e - m)) + std::sqrt(radicand);
    double t2 = -dotProduct(v, (e - m)) - std::sqrt(radicand);
    double t = minDiscardNegatives(t1, t2);
    if (t < 0) return false; // intersection behind camera
    if (t < hitRecord.parameter) {
        GLPoint intersecPoint = ray.origin + t * ray.direction;
        hitRecord.intersectionPoint = intersecPoint;
        hitRecord.parameter = t;
        hitRecord.rayDirection = ray.direction;
        hitRecord.normal = intersecPoint - m;
        hitRecord.normal.normalize();
        return true;
    }
    return false;
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
