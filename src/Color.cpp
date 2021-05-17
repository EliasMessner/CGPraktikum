

#include "Color.hpp"
#include <limits>
#include <cmath>

Color::Color() {}

Color::Color(float r, float g, float b) : r(r), g(g), b(b) {}

Color &Color::operator*=(double i) {
  r *= i;
  g *= i;
  b *= i;
  return *this;
}

Color &Color::operator/=(double i) {
  double frac = 1.0 / i;
  r *= frac;
  g *= frac;
  b *= frac;
  return *this;
}

Color &Color::operator+=(const Color &rhs) {
  r += rhs.r;
  g += rhs.g;
  b += rhs.b;

  return *this;
}

bool areSame(double a, double b) {
	return (std::fabs(a - b) < std::numeric_limits<double>::epsilon());
}

bool &Color::operator==(const Color& rhs) {
	bool result = (areSame(r, rhs.r) && areSame(g, rhs.g) && areSame(b, rhs.b));
	return result;
}

bool &Color::operator!=(const Color& rhs) {
	bool result = !(*this == rhs);
	return result;
}
