#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>

class Tema1 : public SimpleScene
{
	public:
		Tema1();
		~Tema1();

		void CreateMeshes();

		void Init() override;

		void draw_crosshair(float inclinare_tinta, float translateY);

		void draw_arrow(float deltaTimeSeconds, float translateY);

		void draw_bow(float translateX, float translateY);

		void draw_baloons(float angularStep);

		void draw_shurikens(float angularStep, float translateY);

	private:
		void draw_personaj(GLint x0, GLint y0, int latura_patrat, float angularStep, float deltaTimeSeconds);
		void draw_game_over();
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

	protected:
		glm::mat3 modelMatrix;
		float translateX, translateY;
		float scaleX, scaleY;
		float angularStep;
};
