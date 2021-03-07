#pragma once
#include <Component/SimpleScene.h>
#include "LabCamera.h"

class Tema2 : public SimpleScene
{
	public:
		Tema2();
		~Tema2();

		void Init() override;

	private:
		void FrameStart() override;
		void DrawPlayer(float deltaTimeSeconds);
		void DrawPlatforms();
		void VerificareCadere();
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix) override;
		void RenderSpecialMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix , const float ceva);
		//void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color) ;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

	protected:
		Tema::Camera *camera;
		glm::mat4 projectionMatrix;
		bool renderCameraTarget;
		float angularStep;
};
