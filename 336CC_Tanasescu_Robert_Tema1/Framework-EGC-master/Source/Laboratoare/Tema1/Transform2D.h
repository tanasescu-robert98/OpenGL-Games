#pragma once

#include <include/glm.h>

namespace Transform2D
{
	// Translate matrix
	inline glm::mat3 Translate(float translateX, float translateY)
	{
		// TODO implement translate matrix
		glm::mat3 translate_matrix(1, 0, 0, 0, 1, 0, translateX, translateY, 1);
		return translate_matrix * glm::mat3(1);
	}

	// Scale matrix
	inline glm::mat3 Scale(float scaleX, float scaleY)
	{
		// TODO implement scale matrix
		glm::mat3 scale_matrix(scaleX, 0, 0, 0, scaleY, 0, 0, 0, 1);
		return scale_matrix * glm::mat3(1);
	}

	// Rotate matrix
	inline glm::mat3 Rotate(float radians)
	{
		// TODO implement rotate matrix
		glm::mat3 rotation_matrix(cos(radians), sin(radians), 0, -sin(radians), cos(radians), 0, 0, 0, 1);
		return rotation_matrix * glm::mat3(1);
	}
}
