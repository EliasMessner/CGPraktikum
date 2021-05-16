#include <math.h>

#include "Model.hpp"

// Konstruktor
Model::Model() {
  /* Aufgabenblatt 2, Aufgabe 3: Setzen Sie die default Werte */
	mRotation = GLVector(0, 0, 0);
	mTranslation = GLVector(0, 0, 0);
	mScale = GLVector(1, 1, 1);
	mMatrix = GLMatrix();
	updateMatrix();
}

// Setter für das Material
void Model::setMaterial(Material material) {
  mMaterial = Material();
  mMaterial.smooth = material.smooth;
  mMaterial.reflection = material.reflection;
  mMaterial.refraction = material.refraction;
  mMaterial.transparency = material.transparency;
  mMaterial.color = Color(material.color.r, material.color.g, material.color.b);
}

/* Aufgabenblatt 2, Aufgabe 3: Implementieren Sie die vier Methoden für die Transformationen hier */
void Model::setRotation(GLVector rotation) {
	mRotation = rotation;
	updateMatrix();
}

static double d2r(double d) {
	static const auto PI = std::acos(-1);
	return (d / 180.0) * PI;
}

void Model::setRotationDeg(GLVector rotation) {
	setRotation(GLVector(d2r(rotation(0)), d2r(rotation(1)), d2r(rotation(2))));
}

void Model::setTranslation(GLVector translation) {
	mTranslation = translation;
	updateMatrix();
}

void Model::setScale(GLVector scale) {
	mScale = scale;
	updateMatrix();
}

void Model::updateMatrix() {
	// x axis rotation
	GLMatrix xRotMatrix = GLMatrix();
	xRotMatrix.setValue(1, 1, cos(mRotation(0)));
	xRotMatrix.setValue(1, 2, -sin(mRotation(0)));
	xRotMatrix.setValue(2, 1, sin(mRotation(0)));
	xRotMatrix.setValue(2, 2, cos(mRotation(0)));

	// y axis rotation
	GLMatrix yRotMatrix = GLMatrix();
	yRotMatrix.setValue(0, 0, cos(mRotation(1)));
	yRotMatrix.setValue(0, 2, -sin(mRotation(1)));
	yRotMatrix.setValue(2, 0, sin(mRotation(1)));
	yRotMatrix.setValue(2, 2, cos(mRotation(1)));

	// z axis rotation
	GLMatrix zRotMatrix = GLMatrix();
	zRotMatrix.setValue(0, 0, cos(mRotation(2)));
	zRotMatrix.setValue(0, 1, -sin(mRotation(2)));
	zRotMatrix.setValue(1, 0, sin(mRotation(2)));
	zRotMatrix.setValue(1, 1, cos(mRotation(2)));
	
	// scale
	GLMatrix scaleMatrix = GLMatrix();
	scaleMatrix.setValue(0, 0, mScale(0));
	scaleMatrix.setValue(1, 1, mScale(1));
	scaleMatrix.setValue(2, 2, mScale(2));

	mMatrix = GLMatrix();
	mMatrix = scaleMatrix * xRotMatrix * yRotMatrix * zRotMatrix * mMatrix;

	// translation
	mMatrix.setValue(0, 3, mTranslation(0));
	mMatrix.setValue(1, 3, mTranslation(1));
	mMatrix.setValue(2, 3, mTranslation(2));

}

GLMatrix Model::getTransformation() const { return mMatrix; }

Material Model::getMaterial() const { return mMaterial; }
