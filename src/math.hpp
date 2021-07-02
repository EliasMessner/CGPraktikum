#pragma once

#include <array>
#include <vector>
#include <limits>
#include <memory>

#include "GLMatrix.hpp"
#include "GLPoint.hpp"
#include "GLVector.hpp"

inline GLVector operator*(const GLVector &lhs, double scale) {
  return GLVector(lhs(0) * scale, lhs(1) * scale, lhs(2) * scale);
}

inline GLVector operator*(double scale, const GLVector &rhs) {
  return GLVector(rhs(0) * scale, rhs(1) * scale, rhs(2) * scale);
}

inline GLPoint operator*(const GLPoint &lhs, double scale) {
  return GLPoint(lhs(0) * scale, lhs(1) * scale, lhs(2) * scale);
}

inline GLPoint operator*(double scale, const GLPoint &rhs) {
  return GLPoint(rhs(0) * scale, rhs(1) * scale, rhs(2) * scale);
}

inline GLPoint operator+(const GLPoint &p1, const GLVector &p2) {
  return GLPoint(p1(0) + p2(0), p1(1) + p2(1), p1(2) + p2(2));
}

inline GLPoint operator+(const GLPoint &p1, const GLPoint &p2) {
  return GLPoint(p1(0) + p2(0), p1(1) + p2(1), p1(2) + p2(2));
}

inline GLVector operator+(const GLVector &p1, const GLVector &p2) {
  return GLVector(p1(0) + p2(0), p1(1) + p2(1), p1(2) + p2(2));
}

inline GLVector operator-(const GLPoint &p1, const GLPoint &p2) {
  return GLVector(p1(0) - p2(0), p1(1) - p2(1), p1(2) - p2(2));
}

inline GLVector crossProduct(const GLVector &lhs, const GLVector &rhs) {
  return GLVector(lhs(1) * rhs(2) - lhs(2) * rhs(1),
                  lhs(2) * rhs(0) - lhs(0) * rhs(2),
                  lhs(0) * rhs(1) - lhs(1) * rhs(0));
}

inline double dotProduct(const GLVector &lhs, const GLVector &rhs) {
  return lhs(0) * rhs(0) + lhs(1) * rhs(1) + lhs(2) * rhs(2);
}

inline int sgn(int x) { return (x > 0) ? 1 : (x < 0) ? -1 : 0; }

inline bool areSame(double a, double b) {
    return (std::fabs(a - b) < std::numeric_limits<double>::epsilon());
}

/** Aufgabenblatt 2, Aufgabe 2 **/
inline GLVector operator*(const GLMatrix& lhs, const GLVector& rhs) {
    return GLVector(lhs(0, 0) * rhs(0) + lhs(0, 1) * rhs(1) + lhs(0, 2) * rhs(2),
                   lhs(1, 0) * rhs(0) + lhs(1, 1) * rhs(1) + lhs(1, 2) * rhs(2),
                   lhs(2, 0) * rhs(0) + lhs(2, 0) * rhs(1) + lhs(2, 2) * rhs(2));
}

inline GLPoint operator*(const GLMatrix& lhs, const GLPoint& rhs) {
    return GLPoint(lhs(0, 0) * rhs(0) + lhs(0, 1) * rhs(1) + lhs(0, 2) * rhs(2) + lhs(0, 3),
                   lhs(1, 0) * rhs(0) + lhs(1, 1) * rhs(1) + lhs(1, 2) * rhs(2) + lhs(1, 3),
                   lhs(2, 0) * rhs(0) + lhs(2, 0) * rhs(1) + lhs(2, 2) * rhs(2) + lhs(2, 3));
}

inline GLMatrix operator*(const GLMatrix& lhs, const GLMatrix& rhs) {
    auto result = GLMatrix();
    for (int i = 0; i < 4; i++) {
        result.setColumn(i,
            GLVector(
                lhs(0, 0) * rhs(0, i) + lhs(0, 1) * rhs(1, i) + lhs(0, 2) * rhs(2, i),
                lhs(1, 0) * rhs(0, i) + lhs(1, 1) * rhs(1, i) + lhs(1, 2) * rhs(2, i),
                lhs(2, 0) * rhs(0, i) + lhs(2, 1) * rhs(1, i) + lhs(2, 2) * rhs(2, i)));
    }
    return result;
}

inline bool areSame(const float a, const float b) {
    return (std::fabs(a - b) < std::numeric_limits<float>::epsilon());
}

inline double getDistance(const GLPoint &lhs, const GLPoint &rhs) {
    double result = sqrt(
        pow(lhs(0) - rhs(0), 2) +
        pow(lhs(1) - rhs(1), 2) +
        pow(lhs(2) - rhs(2), 2)
    );
    return result;
}

/*
* returns the minimum of two values, if both are greater than 0.
* If one is greater 0 and the other one is not, returns the value greater 0.
* If both are negative or 0, returns -1.
*/
inline double minDiscardNegatives(const double d1, const double d2) {
    if (d1 <= 0 && d2 <= 0) return -1;
    else if (d1 > 0 && d2 > 0) return fmin(d1, d2);
    else if (d1 > 0) return d1;
    else return d2;
}