#pragma once

#include <math.h>
#include <stdio.h>
#include <string>

/*
		VECTOR.H

		scalar_t
		Vector class

		OpenGL Game Programming
		by Kevin Hawkins and Dave Astle

		Some operators of the Vector class based on
		operators of the Vector class by Bas Kuenen.
		Copyright (c) 2000 Bas Kuenen. All Rights Reserved.
		homepage: baskuenen.cfxweb.net

		=============================================================
		Some methods to calculate the distance and distance squared
		have between added. Set method and the toString method added 
		and errors in the dot product methods corrected.
		2011 Peter Lager - University of Bolton
		=============================================================
*/

typedef float scalar_t;

class Vector{
public:
	scalar_t x;
	scalar_t y;
	scalar_t z;	 // x,y,z coordinates

public:
	Vector(scalar_t a = 0, scalar_t b = 0, scalar_t c = 0) : x(a), y(b), z(c) {}

	Vector(const Vector &vec) : x(vec.x), y(vec.y), z(vec.z) {}

		// vector assignment
	const Vector &operator=(const Vector &vec){
		x = vec.x;
		y = vec.y;
		z = vec.z;

		return *this;
	}

	// vecector equality
	const bool operator==(const Vector &vec) const {
		return ((x == vec.x) && (y == vec.y) && (z == vec.z));
	}

	// vector inequality
	const bool operator!=(const Vector &vec) const {
		return !(*this == vec);
	}

	// vector add
	const Vector operator+(const Vector &vec) const {
	return Vector(x + vec.x, y + vec.y, z + vec.z);
	}

	// vector add (opposite of negation)
	const Vector operator+() const {	 
		return Vector(*this);
	}

	// vector increment
	const Vector& operator+=(const Vector& vec){	 
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	}

	// vector subtraction
	const Vector operator-(const Vector& vec) const {	 
		return Vector(x - vec.x, y - vec.y, z - vec.z);
	}
	
	// vector negation
	const Vector operator-() const {	 
		return Vector(-x, -y, -z);
	}

	// vector decrement
	const Vector &operator-=(const Vector& vec) {
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;

		return *this;
	}

	// scalar self-multiply
	const Vector &operator*=(const scalar_t &s) {
		x *= s;
		y *= s;
		z *= s;
		
		return *this;
	}

	// scalar self-divecide
	const Vector &operator/=(const scalar_t &s) {
		const float recip = 1/s; // for speed, one division

		x *= recip;
		y *= recip;
		z *= recip;

		return *this;
	}

	// post multiply by scalar
	const Vector operator*(const scalar_t &s) const {
		return Vector(x*s, y*s, z*s);
	}

	// pre multiply by scalar
	friend inline const Vector operator*(const scalar_t &s, const Vector &vec){
		return vec*s;
	}

/*	friend inline const Vector operator*(const Vector &vec, const scalar_t &s)
	{
	return Vector(vec.x*s, vec.y*s, vec.z*s);
	}

*/	
 
	// divecide by scalar
	const Vector operator/(scalar_t s) const {
		s = 1/s;

		return Vector(s*x, s*y, s*z);
	}

	// cross product
	const Vector CrossProduct(const Vector &vec) const {
	return Vector(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - y*vec.x);
	}

	// cross product
	const Vector operator^(const Vector &vec) const {
		return Vector(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - y*vec.x);
	}

	// dot product
	const scalar_t DotProduct(const Vector &vec) const {
		return x*vec.x + y*vec.y + z*vec.z;
	}

	// dot product
	const scalar_t operator%(const Vector &vec) const {
		return x*vec.x + y*vec.y + z*vec.z;
	}

	// length of vector
	const scalar_t Length() const {
		return (scalar_t)sqrt((double)(x*x + y*y + z*z));
	}

	// distance to vector
	const scalar_t Dist(const Vector &vec) const {
		float dx = x - vec.x;
		float dy = y - vec.y;
		float dz = z - vec.z;
		return (scalar_t)sqrt(dx*dx + dy*dy + dz*dz);
	}

	// distance to vector squared
	const scalar_t Dist2(const Vector &vec) const {
		float dx = x - vec.x;
		float dy = y - vec.y;
		float dz = z - vec.z;
		return (scalar_t)(dx*dx + dy*dy + dz*dz);
	}

	// distance between 2vectors
	static scalar_t Dist(const Vector &vec1, const Vector &vec2) {
		float dx = vec1.x - vec2.x;
		float dy = vec1.y - vec2.y;
		float dz = vec1.z - vec2.z;
		return (scalar_t)sqrt(dx*dx + dy*dy + dz*dz);
	}

	// distance between vectors squared
	static scalar_t Dist2(const Vector &vec1, const Vector &vec2) {
		float dx = vec1.x - vec2.x;
		float dy = vec1.y - vec2.y;
		float dz = vec1.z - vec2.z;
		return (scalar_t)(dx*dx + dy*dy + dz*dz);
	}

	// return the unit vector
	const Vector UnitVector() const {
		return (*this) / Length();
	}

	// normalize this vector
	void Normalize() {
		(*this) /= Length();
	}

	const scalar_t operator!() const {
		return sqrtf(x*x + y*y + z*z);
	}

	// return vector with specified length
	const Vector operator | (const scalar_t length) const {
		return *this * (length / !(*this));
	}

	// set length of vector equal to length
	const Vector& operator |= (const float length) {
		return *this = *this | length;
	}

	void set(scalar_t x, scalar_t y, scalar_t z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	// return angle between two vectors
	const float inline Angle(const Vector& normal) const {
		return acosf(*this % normal);
	}

	// reflect this vector off surface with normal vector
	const Vector inline Reflection(const Vector& normal) const {	 
		const Vector vec(*this | 1);		// normalize this vector
		return (vec - normal * 2.0 * (vec % normal)) * !*this;
	}

	// Convert the vector to text for display
	const std::string toString()
	{
		char text[256];
		sprintf_s(text, "[%.2f, %.2f, %.2f]", x, y, z);
		return std::string(text);
	}

};
