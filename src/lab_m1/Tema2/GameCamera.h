#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"


namespace implemented
{
	class GameCamera
	{
	public:
		GameCamera()
		{
			position = glm::vec3(0, 2, 5);
			forward = glm::vec3(0, 0, -1);
			up = glm::vec3(0, 1, 0);
			right = glm::vec3(1, 0, 0);
			distanceToTarget = 2;
		}

		GameCamera(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
		{
			Set(position, center, up);
		}

		~GameCamera()
		{ }

		// Update camera
		void Set(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
		{
			this->position = position;
			forward = glm::normalize(center - position);
			right = glm::cross(forward, up);
			this->up = glm::cross(right, forward);
		}

		void MoveForward(float distance)
		{
			// Translates the camera using the `dir` vector computed from
			// `forward`. Movement will always keep the camera at the same
			// height. For example, if you rotate your head up/down, and then
			// walk forward, then you will still keep the same relative
			// distance (height) to the ground
			glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
			position += dir * distance;
		}

		void TranslateForward(float distance)
		{
			// Translate the camera using the `forward` vector
			position += forward * distance;
		}

		void TranslateUpward(float distance)
		{
			// Translate the camera using the `up` vector
			position += up * distance;
		}

		void TranslateRight(float distance)
		{
			// Translate the camera using the `right` vector
			glm::vec3 dir = glm::normalize(glm::vec3(right.x, 0, right.z));
			position += dir * distance;
		}

		void RotateFirstPerson_OX(float angle)
		{
			// Compute the new `forward` and `up` vectors
			forward = glm::rotate(glm::mat4(1), angle, right) * glm::vec4(forward, 0);
			forward = glm::normalize(forward);
			up = glm::cross(right, forward);
		}

		void RotateFirstPerson_OY(float angle)
		{
			// Compute the new `forward`, `up` and `right` vectors
			forward = glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 0);
			forward = glm::normalize(forward);

			right = glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 0);
			right = glm::normalize(right);

			up = glm::cross(right, forward);
		}


		void RotateThirdPerson_OX(float angle)
		{
			// Rotate the camera in third-person mode around the OX axis
			TranslateForward(distanceToTarget);
			RotateFirstPerson_OX(angle);
			TranslateForward(-distanceToTarget);
		}

		void RotateThirdPerson_OY(float angle)
		{
			// Rotate the camera in third-person mode around the OY axis.
			TranslateForward(distanceToTarget);
			RotateFirstPerson_OY(angle);
			TranslateForward(-distanceToTarget);
		}

		glm::mat4 GetViewMatrix()
		{
			// Returns the view matrix
			return glm::lookAt(position, position + forward, up);
		}

		glm::vec3 GetTargetPosition()
		{
			return position + forward * distanceToTarget;
		}

	public:
		float distanceToTarget;
		glm::vec3 position;
		glm::vec3 forward;
		glm::vec3 right;
		glm::vec3 up;
	};
}   // namespace implemented
