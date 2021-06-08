#pragma once
#include "math.hpp"

/**
 ** Klasse für Farbdefinitionen
 ** Der Wertebereich je Farbkanal liegt zwischen 0.0 und 1.0
 **/
class Color {
public:
	Color();

	Color(float r, float g, float b);

	Color &operator*=(double i);

	Color &operator/=(double i);

	Color &operator+=(const Color &rhs);

	friend bool operator==(const Color& lhs, const Color& rhs) {
		bool result = (areSame(lhs.r, rhs.r) && areSame(lhs.g, rhs.g) && areSame(lhs.b, rhs.b));
		return result;
	}

	friend bool operator!=(const Color& lhs, const Color& rhs) {
		bool result = !(lhs == rhs);
		return result;
	}

	float r, g, b;

};
