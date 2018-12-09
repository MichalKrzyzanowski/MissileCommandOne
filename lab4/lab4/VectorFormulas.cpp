// Author: Michal K.

#include "VectorFormulas.h"
#include "Game.h"

// get length of vector using sqrt of the sum of the squares
float vectorLength(sf::Vector2f t_vector)
{
	float sumOfSquares = (t_vector.x * t_vector.x) + (t_vector.y * t_vector.y);
	const float length = sqrt(sumOfSquares); // square root of sum

	return length;
}

// get length squared of a vector 
float vectorLengthSquared(sf::Vector2f t_vector)
{
	float lenghtSquared = (t_vector.x * t_vector.x) + (t_vector.y * t_vector.y);

	return lenghtSquared;
}

// get area of parallelogram using vectors
float vectorCrossProduct(sf::Vector2f t_vectorA, sf::Vector2f t_VectorB)
{
	float crossProduct = (t_vectorA.x * t_VectorB.y) - (t_vectorA.y * t_VectorB.x);

	return crossProduct;
}

// get scalar value of vector A and B
float vectorDotProduct(sf::Vector2f t_vectorA, sf::Vector2f t_VectorB)
{
	float DotProduct = (t_vectorA.x * t_VectorB.x) + (t_vectorA.y * t_VectorB.y);

	return DotProduct;
}

// get angle in between two vectors
float vectorAngleBetween(sf::Vector2f t_vectorA, sf::Vector2f t_VectorB)
{
	float scalar = vectorDotProduct(t_vectorA, t_VectorB); // scalar value via dot product

	float lenghtA = vectorLength(t_vectorA); // magnitude of vector A
	float lenghtB = vectorLength(t_VectorB); // magnitude of vector B

	// angle in between workings
	float AngleBetween = (scalar / (lenghtA * lenghtB));
	AngleBetween = acos(AngleBetween); // cos inverse
	AngleBetween = AngleBetween * (180.0f / PI); // radians to degrees

	return AngleBetween;
}

// get a value of rotation, can rotate a vector by this value 
sf::Vector2f vectorRotateBy(sf::Vector2f t_vector, float t_angleRadians)
{
	sf::Vector2f vectorRotation{ 0.0f, 0.0f }; // amount of rotation done

	vectorRotation.x = (cos(t_angleRadians) * t_vector.x) + (-sin(t_angleRadians) * t_vector.y); // x coordinate
	vectorRotation.y = (sin(t_angleRadians) * t_vector.x) + (cos(t_angleRadians) * t_vector.y); // y coordinate

	return vectorRotation;
}

// get a second vector parallel to original vector
sf::Vector2f vectorProjection(sf::Vector2f t_vector, sf::Vector2f t_onto)
{
	float scalar = vectorDotProduct(t_vector, t_onto); // scalar value via dot product
	float lenghtB = vectorLength(t_onto); // magnitude of B
	float magnitudeSquared = vectorLengthSquared(t_onto); // magnitude squared of B 

	// projection formula calculations
	sf::Vector2f projection = scalar * t_onto;
	projection = t_onto / magnitudeSquared;

	return projection;
}

// get a second vector that is perpendicular to original vector
sf::Vector2f vectorRejection(sf::Vector2f t_vector, sf::Vector2f t_onto)
{
	sf::Vector2f projection = vectorProjection(t_vector, t_onto); // projection of a vector
	sf::Vector2f rejection = t_vector - t_onto;

	return rejection;
}

// gets a scalar value based on the projection formula
float vectorScalarProjection(sf::Vector2f t_vector, sf::Vector2f t_onto)
{
	float scalar = vectorDotProduct(t_vector, t_onto); // scalar value via dot product
	float lenghtB = vectorLength(t_onto); // magnitude of B

	float scalarProjection = scalar / lenghtB;

	return scalarProjection;
}

// gets a vector with a magnitude of 1
sf::Vector2f vectorUnitVector(sf::Vector2f t_vector)
{
	sf::Vector2f unitVector{ 0.0f, 0.0f };
	float lengthA = vectorLength(t_vector); // magnitude of A

	if (lengthA != 0.0f)
	{
		unitVector = t_vector / lengthA;
	}

	return unitVector;
}
