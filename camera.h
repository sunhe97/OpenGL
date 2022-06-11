#pragma once
#ifndef __CAMERA_H__
#define __CAMERA_H__
#include <glad/glad.h>//must in the first
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
enum Camera_Movement{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
class Camera {
public:
	glm::vec3 Position;
	glm::vec3 Up;
	glm::vec3 Front;
	glm::vec3 WorldUp;
	glm::vec3 Right;

	float Pitch;
	float Yaw;

	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	Camera(glm::vec3 positon = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float pitch=PITCH, float yaw=YAW) :Front(glm::vec3(0.0f,0.0f,-1.0f)),
	WorldUp(up),Position(positon),Pitch(pitch),Yaw(yaw),MovementSpeed(SPEED),MouseSensitivity(SENSITIVITY),Zoom(ZOOM){
		UpdateCameraVector();
	}

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw=YAW, float pitch=PITCH) :Front(glm::vec3(0.0f, 0.0f, -1.0f)),
		WorldUp(glm::vec3(upX,upY,upZ)), Position(glm::vec3(posX, posY, posZ)), Pitch(pitch), Yaw(yaw), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
		UpdateCameraVector();
	}

	glm::mat4 CalculateOwnLootAtMatrix(glm::vec3 position,glm::vec3 target,glm::vec3 up) {
		glm::vec3 zaxis = glm::normalize(position - target);
		glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize(up), zaxis));
		glm::vec3 yaxis = glm::cross(zaxis, xaxis);

		glm::mat4 a = glm::mat4(1.0f);
		glm::mat4 b = glm::mat4(1.0f);
		a[0][0] = xaxis.x;
		a[1][0] = xaxis.y;
		a[2][0] = xaxis.z;
		a[0][1] = yaxis.x;
		a[1][1] = yaxis.y;
		a[2][1] = yaxis.z;
		a[0][2] = zaxis.x;
		a[1][2] = zaxis.y;
		a[2][2] = zaxis.z;
		b[3][0] = -position.x;
		b[3][1] = -position.y;
		b[3][2] = -position.z;
		return a * b;
	}

	glm::mat4 GetViewMatrix() {
		return glm::lookAt(Position, Position + Front, Up);
		//return CalculateOwnLootAtMatrix(Position, Position + Front, Up);
	}
	void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
		float velocity = deltaTime * SPEED;
		if (direction==FORWARD) {
			Position += Front * velocity;
		}
		if (direction == BACKWARD) {
			Position -= Front * velocity;
		}
		if (direction == RIGHT) {
			Position += glm::cross(Front,Up) * velocity;
		}
		if (direction == LEFT) {
			Position -= glm::cross(Front, Up) * velocity;
		}
		//Position.y = 0;
	}
	void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true) {
		xOffset *= SENSITIVITY;
		yOffset *= SENSITIVITY;

		Yaw += xOffset;
		Pitch += yOffset;

		if (constrainPitch) {
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch<-89.0f)
				Pitch = -89.0f;
		}
		UpdateCameraVector();
	}
private:
	void UpdateCameraVector() {
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));

	}
};
#endif // !__CAMERA_H__
