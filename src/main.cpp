#include <assimp/postprocess.h>
#include <assimp/scene.h>
// #include <unistd.h>

#include <algorithm>
#include <assimp/Importer.hpp>
#include <cmath>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "Scene.hpp"
#include "SolidRenderer.hpp"
#include "WireframeRenderer.hpp"
#include "math.hpp"

int main(int argc, char **argv) {
    // Dimensionen des Ergebnisbildes im Konstruktor setzen
    std::shared_ptr<Image> img = std::make_shared<Image>(401, 301);

    // Verwendete Modelle festlegen
    std::vector<std::string> path_vector;
    path_vector.push_back(std::string("../data/bunny/bunny_scaled.ply"));
    path_vector.push_back(std::string("../data/basicObjects/cube_scaled.ply"));
    // Erzeuge die Szene mit dem default Konstruktor und lade die Modelle
    auto scene = std::make_shared<Scene>();
    scene->load(path_vector);
    
    // bunny
    auto &bunny = scene->getModels()[0];
    // cube
    auto &cube = scene->getModels()[1];

    Color yellow = Color(1.0, 1.0, 0);

    int blatt = 3;
    int aufgabe = 3;

    /* Aufgabenblatt 1: Instanziieren Sie einen WireframeRenderer */
    std::shared_ptr<WireframeRenderer> wireframeRenderer = std::make_shared<WireframeRenderer>(scene, img);

    switch (blatt) {
    case 1:
        switch (aufgabe) {
        case 2:
            /* Aufgabenblatt 1, Aufgabe 2: Testen Sie Ihre drawBresenhamLine-Methode hier */
            wireframeRenderer->drawBresenhamLine(GLPoint(200, 150, 0), GLPoint(300, 170, 0), yellow); //1
            wireframeRenderer->drawBresenhamLine(GLPoint(200, 150, 0), GLPoint(220, 250, 0), yellow); //2
            wireframeRenderer->drawBresenhamLine(GLPoint(200, 150, 0), GLPoint(180, 250, 0), yellow); //3
            wireframeRenderer->drawBresenhamLine(GLPoint(200, 150, 0), GLPoint(100, 170, 0), yellow); //4
            wireframeRenderer->drawBresenhamLine(GLPoint(200, 150, 0), GLPoint(100, 130, 0), yellow); //5
            wireframeRenderer->drawBresenhamLine(GLPoint(200, 150, 0), GLPoint(180, 50, 0), yellow); //6
            wireframeRenderer->drawBresenhamLine(GLPoint(200, 150, 0), GLPoint(220, 50, 0), yellow); //7
            wireframeRenderer->drawBresenhamLine(GLPoint(200, 150, 0), GLPoint(300, 130, 0), yellow); //8
            break;
        case 3:
            /* Aufgabenblatt 1, Aufgabe 3: Testen Sie Ihre seedFillArea-Methode hier */
            // triangle outside
            wireframeRenderer->drawBresenhamLine(GLPoint(30, 70, 0), GLPoint(370, 100, 0), yellow);
            wireframeRenderer->drawBresenhamLine(GLPoint(370, 100, 0), GLPoint(180, 270, 0), yellow);
            wireframeRenderer->drawBresenhamLine(GLPoint(180, 270, 0), GLPoint(30, 70, 0), yellow);
            wireframeRenderer->drawBresenhamLine(GLPoint(140, 110, 0), GLPoint(160, 110, 0), yellow);
            // rectangle inside
            wireframeRenderer->drawBresenhamLine(GLPoint(160, 110, 0), GLPoint(160, 130, 0), yellow);
            wireframeRenderer->drawBresenhamLine(GLPoint(160, 130, 0), GLPoint(140, 130, 0), yellow);
            wireframeRenderer->drawBresenhamLine(GLPoint(140, 130, 0), GLPoint(140, 110, 0), yellow);
            GLPoint seed = GLPoint(250, 170, 0);
            wireframeRenderer->seedFillArea(seed, yellow, Color(0, .7, 0));
            break;
        }
        break;
    case 2:
        /* Aufgabenblatt 2, Aufgabe 3: Setzen Sie die Transformationen der Modelle */
        bunny.setTranslation(GLVector(250, 100, 0));
        bunny.setScale(GLVector(0.8, 0.8, 0.8));
        bunny.setRotationDeg(GLVector(90, 90, 0));

        cube.setTranslation(GLVector(100, 100, 0));
        cube.setScale(GLVector(0.7, 0.7, 0.7));
        cube.setRotationDeg(GLVector(20, 42, 0));

        /* Aufgabenblatt 2, Aufgabe 1: Rufen Sie Ihre renderScene-Methode hier auf */
        wireframeRenderer->renderScene(yellow);
        break;
    case 3:
    case 4:
        /* Setup der Camera - Erst ab Aufgabenblatt 3 relevant. */
        // Diese Einstellungen beziehen sich auf den world space
        // Beachten Sie, dass Sie in diesem Praktikum keine explizite Umwandlung in
        // den ViewSpace benötigen, da die Strahen für Raycasting und Raytracing im
        // World space definiert sind. Modelle müssen also lediglich in den World
        // space überführt werden
  
        /* Aufgabenblatt 3:  kommentieren Sie die Zeilen wieder ein, die eine Kamera erzeugen und zur Scene hinzufügen*/
  
        auto cam = std::make_shared<Camera>();
        GLPoint eye = GLPoint(0.0, 0.0, 300.0);
        cam->setEyePoint(eye);
        cam->setUp(GLVector(0.0, 1.0, 0.0));
        GLVector viewDirection = GLVector(0.0, 0, -1.0);
        viewDirection.normalize();
        cam->setViewDirection(viewDirection);
        cam->setSize(img->getWidth(), img->getHeight());
        scene->setCamera(cam);

        /* Aufgabenblatt 3: Erzeugen Sie mindestens eine Kugel und fügen Sie diese zur Szene hinzu*/
        scene->addSphere(Sphere(GLPoint(-150, 0, 0), 50));
        scene->addSphere(Sphere(GLPoint(150, 0, -30), 50));
    
        if (aufgabe == 4) {
            /* Aufgabenblatt 4: Setzen Sie materialeigenschaften für die Kugelen und die Modelle. Die Materialeigenschaften für eine Darstellung entsprechend der Beispiellösung ist in der Aufgabenstellung gegeben. */
        }

        /* Aufgabenblatt 3: (Wenn nötig) Transformationen der Modelle im World space, sodass sie von der Kamera gesehen werden könnnen. Die nötigen Transformationen für eine Darstellung entsprechend der Beispiellösung ist in der Aufgabenstellung gegeben. */
        bunny.setTranslation(GLVector(0, 30, -30));
        bunny.setRotationDeg(GLVector(0, 0, 0));
        bunny.setScale(GLVector(1, 1, 1));

        cube.setTranslation(GLVector(0, -100, 0));
        cube.setRotationDeg(GLVector(50, 170, 0));
        cube.setScale(GLVector(1, 1, 1));

        if (aufgabe == 4) {
            /* Stelle materialeigenschaften zur verfügung (Relevant für Aufgabenblatt 4)*/

            /* Aufgabenblatt 4  Fügen Sie ein Licht zur Szene hinzu */

        }
        /* Aufgabenblatt 3: erzeugen Sie einen SolidRenderer (vorzugsweise mir einem shared_ptr) und rufen sie die Funktion renderRaycast auf */
        auto solidRenderer = std::make_shared<SolidRenderer>(scene, img, cam);
        solidRenderer->renderRaycast();
        break;
    }

  
    // Schreiben des Bildes in Datei
    if (argc > 1) {
    img->writeAsPPM(argv[1]);
    std::cout << "Bild mit Dimensionen " << img->getWidth() << "x"
                << img->getHeight() << " in Datei " << argv[1] << " geschrieben."
                << std::endl;
    } else {
    std::cerr
        << "Fehler: Kein Dateiname angegeben. Es wurde kein Output generiert."
        << std::endl;
    }

    return 0;
}
