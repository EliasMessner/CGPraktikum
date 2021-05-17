#pragma once

#include "Color.hpp"
#include "math.hpp"

struct Ray {
 public:
  Ray() {}

 public:
  // initialized by the main loop or any iteration
  GLPoint origin;
  GLVector direction;

  void print() const {
    std::cout << "Ray origin: " << origin << " direction: " << direction
              << std::endl;
  }
};

struct HitRecord {
 public:
  // initialized by intersection function
  Color color;       // accumulated color
  double parameter;  // distance from eye to intersection point

  GLPoint intersectionPoint;  // point where ray intersects with the object
  GLVector rayDirection;      // ray direction relevant for shading
  GLVector normal;  // normal at the intersection point relevant for shading

  int recursions;  // number of recursions already processed

  int triangleId;  // triangle index of the object hit by ray (default should be
                   // -1)
  int modelId;   // model index of the object hit by ray (default should be -1)
  int sphereId;  // sphere index of the object hit by ray (default should be -1)

  void print() const {
    std::cout << "Intersection at Parameter: " << parameter << " with triangle "
              << triangleId << " at point " << intersectionPoint(0) << ", "
              << intersectionPoint(1) << ", " << intersectionPoint(2)
              << " with normal " << normal << std::endl;
  }
};

struct Triangle {
  std::array<GLPoint, 3> vertex;
  GLVector normal;
  
  bool contains(GLPoint p) const {
      Triangle t1 = Triangle(*this);
      Triangle t2 = Triangle(*this);
      Triangle t3 = Triangle(*this);
      t1.vertex[0] = p;
      t2.vertex[1] = p;
      t3.vertex[2] = p;
      if (t1.area() + t2.area() + t3.area() <= this->area() + std::numeric_limits<float>::epsilon()) {
          return true;
      }
      else {
          return false;
      }
  }

  double area() const {
      GLVector a = vertex[1] - vertex[0];
      GLVector b = vertex[2] - vertex[0];
      return crossProduct(a, b).getLength() / 2;
  }

  Triangle transform(GLMatrix transformation) const {
      Triangle tri;
      tri.vertex = std::array<GLPoint, 3> {
          transformation * vertex[0],
          transformation * vertex[1],
          transformation * vertex[2]};
      GLVector v = tri.vertex[1] - tri.vertex[0];
      GLVector w = tri.vertex[2] - tri.vertex[0];
      GLVector normal = crossProduct(v, w);
      normal.normalize();
      tri.normal = normal;
      return tri;
  }
};
