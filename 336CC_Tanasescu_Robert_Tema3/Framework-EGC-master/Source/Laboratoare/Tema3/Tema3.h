#pragma once
#include <Component/SimpleScene.h>
#include "LabCamera.h"
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>

class Tema3 : public SimpleScene
{
	public:
		Tema3();
		~Tema3();

		Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices);
		void Init() override;

	private:
		void FrameStart() override;
		void DrawPlayer(float deltaTimeSeconds);
		void DrawPlatforms();
		void VerificareCadere();
		void DrawStatusBar();
		void FuelManagement();
		void SpeedManagement();
		void Update(float deltaTimeSeconds) override;
		void RenderLightMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, const glm::vec3& lightpos);
		void FrameEnd() override;

		void RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix) override;
		void RenderSpecialMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix , const float ceva);
		void RenderTextureMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1);
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
		std::unordered_map<std::string, Texture2D*> mapTextures;

		int FOV = 80;
		int left1 = -1;
		int right1 = 1;
		int top1 = 0;
		int bot1 = -1;

		// variabile care defineste cu viteza se vor misca platformele
		float viteza_platforme = 1;

		int numar_seturi_platforme = 10000;

		int player_position = 2;	// pozitia player-ului in scena ( 1 = stanga, 2 = mijloc, 3 = dreapta)
		int camera_position = 0;	// daca este 0 avem camera third person, daca este 1 este first person
		int camera_position_firstperson = 2;	// pozitia player-ului in scena in first person ( 1 = stanga, 2 = mijloc, 3 = dreapta)
		int jumping_state = 0;	// variabila care ne spune daca playerul a initiat saritura
		int contor_saritura = 0;	// variabila pe care o folosim pentru a contoriza cat a stat playerul in "aer"

		int i = 0; int j = 0;

		int random_generate_platforms[10000] = { 0 };		// fiecare set de platforme va avea atribuit un numar de la 1 la 8 , reprezentand tipul de set de coloane pe care
		// il vor reprezenta
		//vector<int> random_generate_platforms(numar_seturi_platforme, 0);

		int showed_platforms[20] = { 0 };

		float pozitie_z_platforme[10000] = { 0 };
		//vector<float> pozitie_platforme(numar_seturi_platforme, 0);


		int contor_pozitie_sfera = 0;	// contor care ne spune la al catelea set din vector se afla sfera la momentul actual

		int debug_boxes = 0;	// variabila pentru a activa debug boxes

		float z_platforma[10000] = { 0 };	// vector in care stocam lungimea pe z a fiecarei platforme

		float distanta_platforma[10000] = { 0 };	// vectoram in care stocam lungimea dintre platforma curenta si urmatoarea pe OZ

		int platforma_parcursa[10000] = { 0 };	// vector utilizat pentru a retine care platforme au 

		int previous_player_position = 2;	// variabila in care retinem pozitia anterioara a player-ului , ne ajuta pentru a initia animatiile de miscare stanga dreapta

		float viteza_bila = 0;
		float viteza_bila_orizontala = 0; // viteza cu care se va misca bila stanga dreapta in cadrul animatiei
		int finished_move = 0;	// variabila care ne spune cand s-a terminat animatia de schimbare a directiei pe OX

		float viteza_bila_verticala = 0; // viteza cu care sare bila

		int is_player_dead = 0;	// variabila care ne spune daca player-ul este mort sau nu


		int infinite_jump = 0;	// contor pentru a tine minte cat timp dureaza saritura bilei


		int previous_player_speed = 0;	// viteza anterioara a player-ului



		// variabile powerup platforme


		int speed_powerup = 0;	// 0 = speed powerup nu este activat, 1 = speed powerup este activat



		int special_platform_indices[10000] = { 0 };	// indicele platformei speciale din fiecare set, 1 = platforma din stanga, 2 = platforma din mijloc, 3 = platforma din dreapta

		int special_platform_type[10000] = { 0 }; // 1 = rosu, 2 = galben , 3 = portocaliu , 4 = verde 


		// variabile powerup platforme end



		// variabile pentru spatiul dintre coloane
		float spatiu_platforme_coloane[10000] = { 0 };	// spatiul dintre platforme pe OZ

		float spatiu_platfomre_vector_random[3];	// vector in care stocam posibilele dimensiuni pentru spatiile dintre coloane

		// changing speed

		int change_speed = 0;	// viteza cu care se deplaseaza player-ul 

		int platforme_parcurse[10000][4];	// matrice in care stocam care platforme au fost atinse de catre player

		// variabile globale pentru combustibil

		float fuel = 100;	// variabila in care stocam combustibilul , 0 sau mai mic ca 0 inseamna game over

		int take_fuel_platform_set[10000] = { 0 };	// vector in care stocam platformele galbene

		int add_fuel_platform_set[10000] = { 0 };	// vector in care stocam platformele verzi

		int take_fuel = 0;	// variabila care ne spune daca player-ul va pierde din fuel

		int add_fuel = 0;	// variabila care ne spune daca player-ul va castiga fuel


		int previous_contor_pozitie_sfera = 0;	// variabila in care stocam ultimul set de platforme parcurse de player

		int speed_platform_effect = 0;	// variabila care ne spune daca este activ efectul unei platforme portocalii


		int contor_fuel = 0;	// un contor care in momentul cand ajunge la o valoare divizibila cu 20 scade scade din combustibilul curent

		int Semafor_speed_powerup = 0;	// variabila care ne spune daca este activ efectul unei platforme portocalii

		float factor_deformare = 0;

		int deformare_on = 0;

		int contor_deformare = 0;

		// variabile globale pentru bara de fuel

		float latura_powerbar = fuel;	// latura din UI care ne spune cat combustibil mai avem

		int previous_change_speed = -1;	// vechea viteza a player-ului

		int finished_dead = 0;



		glm::vec3 lightPosition;
		glm::vec3 lightDirection;
		unsigned int materialShininess;
		float materialKd;
		float materialKs;

		int isSpotlight;
};
