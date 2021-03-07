#include "Tema2.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

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

float factor_deformare =  0 ;

int deformare_on = 0;

int contor_deformare = 0;

// variabile globale pentru bara de fuel

float latura_powerbar = fuel;	// latura din UI care ne spune cat combustibil mai avem

int previous_change_speed = -1;	// vechea viteza a player-ului


Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

void Tema2::Init()
{
	renderCameraTarget = false;

	camera = new Tema::Camera();
	camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("rectangle");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "screen_quad.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}
	angularStep = 0;
	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);


	srand(time(NULL));

	distanta_platforma[0] = 1.25f;

	spatiu_platfomre_vector_random[0] = 0;
	spatiu_platfomre_vector_random[1] = 0.25f;
	spatiu_platfomre_vector_random[2] = 0.5f;

	for (int i = 0; i < 10000; i++)
	{
		spatiu_platforme_coloane[i] = 1;
	}

	for (int i = 0; i < 10000; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			platforme_parcurse[i][j] = 0;
		}
	}
	//srand(1);
}

void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

int finished_dead = 0;

void Tema2::DrawPlayer(float deltaTimeSeconds)
{
	
	if (is_player_dead == 1)
	{
		//cout << "SALUTTTTTTT" << endl;

		//cout << viteza_bila << endl;
		glm::mat4 modelMatrix4 = glm::mat4(1);
		if (player_position == 1)
		{
			if (previous_player_position == 2 && viteza_bila_orizontala < 1)
			{
				viteza_bila_orizontala += 0.1f;
				if (0 - viteza_bila_orizontala > -1)
				{
					modelMatrix4 = glm::translate(modelMatrix4, glm::vec3(0 - viteza_bila_orizontala, 1.5f, 1.5f));
				}
			}
			if (viteza_bila_orizontala >= 1 || previous_player_position == 1)
			{
				previous_player_position = 1;
				viteza_bila_orizontala = 0;
				modelMatrix4 = glm::translate(modelMatrix4, glm::vec3(-1, 1.5f, 1.5f));
				finished_dead = 2;
			}
		}
		if (player_position == 2)
		{

			if (previous_player_position == 1 && viteza_bila_orizontala < 1)
			{
				viteza_bila_orizontala += 0.1f;
				if (-1 + viteza_bila_orizontala < 0)
				{
					modelMatrix4 = glm::translate(modelMatrix4, glm::vec3(-1 + viteza_bila_orizontala, 1.5f, 1.5f));
				}
			}

			if (previous_player_position == 3 && viteza_bila_orizontala < 1)
			{
				viteza_bila_orizontala += 0.1f;
				if (1 - viteza_bila_orizontala > 0)
				{
					modelMatrix4 = glm::translate(modelMatrix4, glm::vec3(1 - viteza_bila_orizontala, 1.5f, 1.5f));
				}
			}

			if (viteza_bila_orizontala >= 1 || previous_player_position == 2)
			{
				previous_player_position = 2;
				viteza_bila_orizontala = 0;
				modelMatrix4 = glm::translate(modelMatrix4, glm::vec3(0, 1.5f, 1.5f));
				finished_dead = 2;
			}




			//modelMatrix4 = glm::translate(modelMatrix4, glm::vec3(0, 1.5f, 1.5f));
		}
		if (player_position == 3)
		{
			if (previous_player_position == 2 && viteza_bila_orizontala < 1)
			{
				viteza_bila_orizontala += 0.1f;
				if (0 - viteza_bila_orizontala > -1)
				{
					modelMatrix4 = glm::translate(modelMatrix4, glm::vec3(0 + viteza_bila_orizontala, 1.5f, 1.5f));
				}
			}
			if (viteza_bila_orizontala >= 1 || previous_player_position == 3)
			{
				previous_player_position = 3;
				viteza_bila_orizontala = 0;
				modelMatrix4 = glm::translate(modelMatrix4, glm::vec3(1, 1.5f, 1.5f));
				finished_dead = 2;
			}
		}
		if (finished_dead == 2)
		{
			viteza_bila -= 0.05f;
			//cout << "POZITIA CURENTA ESTE " << contor_pozitie_sfera << " RANDOM GENERATE PLATFORMS" << random_generate_platforms[contor_pozitie_sfera] << endl;
		}
		modelMatrix4 = glm::translate(modelMatrix4, glm::vec3(0, viteza_bila, 0));
		modelMatrix4 = glm::scale(modelMatrix4, glm::vec3(0.5f));
		if (camera_position == 0)
		{
			if (speed_powerup == 0 && contor_deformare == 0)
				RenderMesh(meshes["sphere"], shaders["VertexColor2"], modelMatrix4);
			if (speed_powerup > 0 || contor_deformare > 0)
				RenderSpecialMesh(meshes["sphere"], shaders["Noise"], modelMatrix4, factor_deformare);
			//RenderMesh(meshes["sphere"], shaders["VertexColor2"], modelMatrix4); 
		}
		
		if (1.5f + viteza_bila < -0.5f)
		{
			exit(0);
		}
	}
	
	if (is_player_dead == 0)
	{

		glm::mat4 modelMatrixPlayer = glm::mat4(1);

		if (player_position == 1)
		{
			//cout << "VITEZA BILA ESTE" << viteza_bila_orizontala << endl;
			if (previous_player_position == 2 && viteza_bila_orizontala < 1)
			{
				viteza_bila_orizontala += 0.1f;
				if (0 - viteza_bila_orizontala > -1)
				{
					modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0 - viteza_bila_orizontala, 1.5f, 1.5f));
				}
			}
			if (viteza_bila_orizontala >= 1 || previous_player_position == 1)
			{
				previous_player_position = 1;
				viteza_bila_orizontala = 0;
				modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(-1, 1.5f, 1.5f));
			}
			
			if (jumping_state == 1)
			{
				//cout << "SALUT" << endl;
				viteza_bila_verticala += 0.1f;
				modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0, viteza_bila_verticala, 0));
				if (1.5f + viteza_bila_verticala >= 2.5f)
				{
					jumping_state = 2;
					viteza_bila_verticala = 0;
					infinite_jump = 1;
					modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0, 2.5f, 0));
				}
			}
			if (infinite_jump >= 1 && infinite_jump < 50)
			{
				//cout << infinite_jump << endl;
				modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0, 1, 0));
				infinite_jump++;
			}
			if (jumping_state == 2 && infinite_jump == 50)
			{
				viteza_bila_verticala += 0.1f;
				modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0, 1.5f-viteza_bila_verticala, 0));
				if (2.5f - viteza_bila_verticala <= 1.5f)
				{
					jumping_state = 0;
					viteza_bila_verticala = 0;
				}
			}
			if (jumping_state == 0)
			{
				modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0, 0, 0));
			}
			modelMatrixPlayer = glm::scale(modelMatrixPlayer, glm::vec3(0.5f));
			if (camera_position == 0)
			{
				if (speed_powerup == 0 && contor_deformare == 0)
					RenderMesh(meshes["sphere"], shaders["VertexColor2"], modelMatrixPlayer);
				if (speed_powerup > 0 || contor_deformare > 0)
					RenderSpecialMesh(meshes["sphere"], shaders["Noise"], modelMatrixPlayer, factor_deformare);
				//RenderMesh(meshes["sphere"], shaders["VertexColor2"], modelMatrixPlayer);
			}
				
		}
		else if (player_position == 2)
		{
			if (previous_player_position == 1 && viteza_bila_orizontala < 1)
			{
				viteza_bila_orizontala += 0.1f;
				if (-1 + viteza_bila_orizontala < 0)
				{
					modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(-1 + viteza_bila_orizontala, 1.5f, 1.5f));
				}
			}

			if (previous_player_position == 3 && viteza_bila_orizontala < 1)
			{
				viteza_bila_orizontala += 0.1f;
				if (1 - viteza_bila_orizontala > 0)
				{
					modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(1 - viteza_bila_orizontala, 1.5f, 1.5f));
				}
			}
						
			if (viteza_bila_orizontala >= 1 || previous_player_position == 2)
			{
				previous_player_position = 2;
				viteza_bila_orizontala = 0;
				modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0, 1.5f, 1.5f));
			}

			if (jumping_state == 1)
			{
				//cout << "SALUT" << endl;
				viteza_bila_verticala += 0.1f;
				modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0, viteza_bila_verticala, 0));
				if (1.5f + viteza_bila_verticala >= 2.5f)
				{
					jumping_state = 2;
					viteza_bila_verticala = 0;
					infinite_jump = 1;
					modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0, 2.5f, 0));
				}
			}
			if (infinite_jump >= 1 && infinite_jump < 50)
			{
				//cout << infinite_jump << endl;
				modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0, 1, 0));
				infinite_jump++;
			}
			if (jumping_state == 2 && infinite_jump == 50)
			{
				//cout << "VITEZA ESTE" << viteza_bila_verticala << endl;
				viteza_bila_verticala += 0.1f;
				modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0, 1.5f - viteza_bila_verticala, 0));
				if (2.5f - viteza_bila_verticala <= 1.5f)
				{
					jumping_state = 0;
					viteza_bila_verticala = 0;
				}
			}
			if (jumping_state == 0)
			{
				modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0, 0, 0));
			}


			modelMatrixPlayer = glm::scale(modelMatrixPlayer, glm::vec3(0.5f));
			if (camera_position == 0)
			{
				if (speed_powerup == 0 && contor_deformare == 0)
					RenderMesh(meshes["sphere"], shaders["VertexColor2"], modelMatrixPlayer);
				if(speed_powerup > 0 || contor_deformare > 0)
					RenderSpecialMesh(meshes["sphere"], shaders["Noise"], modelMatrixPlayer, factor_deformare);
			}
		}
		else if (player_position == 3)
		{
			if (previous_player_position == 2 && viteza_bila_orizontala < 1)
			{
				viteza_bila_orizontala += 0.1f;
				if (0 + viteza_bila_orizontala < 1)
				{
					modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0 + viteza_bila_orizontala, 1.5f, 1.5f));
				}
			}
			if (viteza_bila_orizontala >= 1 || previous_player_position == 3)
			{
				previous_player_position = 3;
				viteza_bila_orizontala = 0;
				modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(1, 1.5f, 1.5f));
			}
			if (jumping_state == 1)
			{
				//cout << "SALUT" << endl;
				viteza_bila_verticala += 0.1f;
				modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0, viteza_bila_verticala, 0));
				if (1.5f + viteza_bila_verticala >= 2.5f)
				{
					jumping_state = 2;
					viteza_bila_verticala = 0;
					infinite_jump = 1;
					modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0, 2.5f, 0));
				}
			}
			if (infinite_jump >= 1 && infinite_jump < 50)
			{
				//cout << infinite_jump << endl;
				modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0, 1, 0));
				infinite_jump++;
			}
			if (jumping_state == 2 && infinite_jump == 50)
			{
				//cout << "VITEZA ESTE" << viteza_bila_verticala << endl;
				viteza_bila_verticala += 0.1f;
				modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0, 1.5f - viteza_bila_verticala, 0));
				if (2.5f - viteza_bila_verticala <= 1.5f)
				{
					jumping_state = 0;
					viteza_bila_verticala = 0;
				}
			}
			if (jumping_state == 0)
			{
				modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0, 0, 0));
			}
			modelMatrixPlayer = glm::scale(modelMatrixPlayer, glm::vec3(0.5f));
			if (camera_position == 0)
			{
				if (speed_powerup == 0 && contor_deformare == 0)
					RenderMesh(meshes["sphere"], shaders["VertexColor2"], modelMatrixPlayer);
				if (speed_powerup > 0 || contor_deformare > 0)
					RenderSpecialMesh(meshes["sphere"], shaders["Noise"], modelMatrixPlayer, factor_deformare);
				//RenderMesh(meshes["sphere"], shaders["VertexColor2"], modelMatrixPlayer);
			}
		}

	}
}

void Tema2::DrawPlatforms()
{
		
	for (int i = 0; i < numar_seturi_platforme; i++)
	{
		//pozitie_z_platforme[i] = angularStep - i * 1.25f;
		//pozitie_z_platforme[i] = angularStep - distanta_platforma[i];

		if (random_generate_platforms[i] == 0)
		{
			if (i == 0 || i == 1 || i == 2)
			{
				random_generate_platforms[i] = 1;
				//random_generate_platforms[i] = 1;
			}
			if (i > 2)
			{
				random_generate_platforms[i] = rand() % 9;
				//random_generate_platforms[i] = 1;

				//cout << random_generate_platforms[i] << endl;
				//random_generate_platforms[i] = 5;
				//cout << "PLATFORME GENERATE " << random_generate_platforms[i] << endl;
				//random_generate_platforms[i] = 4;
			}
			
		}

		if (z_platforma[i] == 0)
		{
			//z_platforma[i] = rand() % 3 ;
			z_platforma[i] = rand() % 5;
		}

		

		if (random_generate_platforms[i] == 1)
		{
			if (i > 0)
			{
				if (spatiu_platforme_coloane[i] == 1)
				{
					spatiu_platforme_coloane[i] = spatiu_platfomre_vector_random[rand() % 3];
				}
				//distanta_platforma[i] = distanta_platforma[i - 1] + ( (z_platforma[i - 1] / 2) + 0.25f + (z_platforma[i] / 2) );
				distanta_platforma[i] = distanta_platforma[i - 1] + ((z_platforma[i - 1] / 2) + spatiu_platforme_coloane[i] + (z_platforma[i] / 2));
			}
			
			pozitie_z_platforme[i] = angularStep  - distanta_platforma[i];
			

			if (pozitie_z_platforme[i] < 5 && pozitie_z_platforme[i] > -10)
			{

				glm::mat4 modelMatrix = glm::mat4(1);
				if (camera_position == 0)
					modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0 + angularStep - distanta_platforma[i]));
				if (camera_position == 1)
				{
					modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1.5f, 0 + angularStep  - distanta_platforma[i]));
					if (contor_saritura != 0)
					{
						//cout << "SUNT AICI" << endl;
						modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.25f, 0));
					}
				}
				modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 0.25f, z_platforma[i]));


				if (player_position == 2 && contor_pozitie_sfera == i && jumping_state == 0)
				{
					//cout << "CONTOR POZITIE SFERA este " << contor_pozitie_sfera << endl;
 					RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
				}
				else
				{
					if (platforme_parcurse[i][2] == 1)
					{
						RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix);
					}
					if (platforme_parcurse[i][2] == 0)
					{
						RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix);
					}
				}

				glm::mat4 modelMatrix2 = glm::mat4(1);
				if (camera_position == 0)
					modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(0 + 1.25f, 1, 0 + angularStep  - distanta_platforma[i]));
				if (camera_position == 1)
				{
					modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(0 + 1.25f, 1.5f, 0 + angularStep  - distanta_platforma[i]));
					if (contor_saritura != 0)
					{
						modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(0, -0.25f, 0));
					}
				}
				modelMatrix2 = glm::scale(modelMatrix2, glm::vec3(1, 0.25f, z_platforma[i]));

				if (player_position == 3 && contor_pozitie_sfera == i && jumping_state == 0)
				{
					RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix2);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
				}
				else
				{
					if (platforme_parcurse[i][3] == 1)
					{
						//cout << "AICI ESTEE   " << player_position << endl;
						RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix2);
					}
					if (platforme_parcurse[i][3] == 0)
					{
						RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix2);
					}
					//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix2);
				}

				glm::mat4 modelMatrix3 = glm::mat4(1);
				if (camera_position == 0)
					modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(0 - 1.25f, 1, 0 + angularStep  - distanta_platforma[i]));
				if (camera_position == 1)
				{
					modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(0 - 1.25f, 1.5f, 0 + angularStep  - distanta_platforma[i]));
					if (contor_saritura != 0)
					{
						modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(0, -0.25f, 0));
					}
				}
				modelMatrix3 = glm::scale(modelMatrix3, glm::vec3(1, 0.25f, z_platforma[i]));
				if (player_position == 1 && contor_pozitie_sfera == i && jumping_state == 0)
				{
					RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix3);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
				}
				else
				{
					if (platforme_parcurse[i][1] == 1)
					{
						RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix3);
					}
					if (platforme_parcurse[i][1] == 0)
					{
						RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix3);
					}
					//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix3);
				}
			}
		}


		if (random_generate_platforms[i] == 2)
		{
			if (i > 0)
			{
				if (spatiu_platforme_coloane[i] == 1)
				{
					spatiu_platforme_coloane[i] = spatiu_platfomre_vector_random[rand() % 3];
				}
				distanta_platforma[i] = distanta_platforma[i - 1] + ((z_platforma[i - 1] / 2) + spatiu_platforme_coloane[i] + (z_platforma[i] / 2));
			}
			
			pozitie_z_platforme[i] = angularStep * viteza_platforme - distanta_platforma[i];
			
			if (pozitie_z_platforme[i] < 5 && pozitie_z_platforme[i] > -10)
			{

				glm::mat4 modelMatrix = glm::mat4(1);
				if (camera_position == 0)
					modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0 + angularStep * viteza_platforme - distanta_platforma[i]));
				if (camera_position == 1)
				{
					modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1.5f, 0 + angularStep * viteza_platforme - distanta_platforma[i]));
					if (contor_saritura != 0)
					{
						modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.25f, 0));
					}
				}
				modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 0.25f, z_platforma[i]));
				if (player_position == 2 && contor_pozitie_sfera == i && jumping_state == 0)
				{
					RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
				}
				else
				{
					if (platforme_parcurse[i][2] == 1)
					{
						RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix);
					}
					if (platforme_parcurse[i][2] == 0)
					{
						RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix);
					}
					//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix);
				}



				glm::mat4 modelMatrix2 = glm::mat4(1);
				if (camera_position == 0)
					modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(0 + 1.25f, 1, 0 + angularStep * viteza_platforme - distanta_platforma[i]));
				if (camera_position == 1)
				{
					modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(0 + 1.25f, 1.5f, 0 + angularStep * viteza_platforme - distanta_platforma[i]));
					if (contor_saritura != 0)
					{
						modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(0, -0.25f, 0));
					}
				}
				modelMatrix2 = glm::scale(modelMatrix2, glm::vec3(1, 0.25f, z_platforma[i]));
				if (player_position == 3 && contor_pozitie_sfera == i && jumping_state == 0)
				{
					RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix2);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
				}
				else
				{
					if (platforme_parcurse[i][3] == 1)
					{
						RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix2);
					}
					if (platforme_parcurse[i][3] == 0)
					{
						RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix2);
					}
					//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix2);
				}




				if (debug_boxes == 1)
				{
					glm::mat4 modelMatrix3 = glm::mat4(1);
					if (camera_position == 0)
						modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(0 - 1.25f, 1, 0 + angularStep * viteza_platforme - distanta_platforma[i]));
					if (camera_position == 1)
					{
						modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(0 - 1.25f, 1.5f, 0 + angularStep * viteza_platforme - distanta_platforma[i]));
						if (contor_saritura != 0)
						{
							modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(0, -0.25f, 0));
						}
					}
					modelMatrix3 = glm::scale(modelMatrix3, glm::vec3(1, 0.25f, z_platforma[i]));
					RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix3);
				}
			}
		}

		if (random_generate_platforms[i] == 3)
		{

			if (i > 0)
			{
				if (spatiu_platforme_coloane[i] == 1)
				{
					spatiu_platforme_coloane[i] = spatiu_platfomre_vector_random[rand() % 3];
				}
				distanta_platforma[i] = distanta_platforma[i - 1] + ((z_platforma[i - 1] / 2) + spatiu_platforme_coloane[i] + (z_platforma[i] / 2));
			}


			pozitie_z_platforme[i] = angularStep * viteza_platforme - distanta_platforma[i];

			if (pozitie_z_platforme[i] < 5 && pozitie_z_platforme[i] > -10)
			{


				if (debug_boxes == 1)
				{
					glm::mat4 modelMatrix = glm::mat4(1);
					if (camera_position == 0)
						modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0 + angularStep * viteza_platforme - distanta_platforma[i]));
					if (camera_position == 1)
					{
						modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1.5f, 0 + angularStep * viteza_platforme - distanta_platforma[i]));
						if (contor_saritura != 0)
						{
							modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.25f, 0));
						}
					}
					modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 0.25f, z_platforma[i]));
					RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
				}

				glm::mat4 modelMatrix2 = glm::mat4(1);
				if (camera_position == 0)
					modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(0 + 1.25f, 1, 0 + angularStep * viteza_platforme - distanta_platforma[i]));
				if (camera_position == 1)
				{
					modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(0 + 1.25f, 1.5f, 0 + angularStep * viteza_platforme - distanta_platforma[i]));
					if (contor_saritura != 0)
					{
						modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(0, -0.25f, 0));
					}
				}
				modelMatrix2 = glm::scale(modelMatrix2, glm::vec3(1, 0.25f, z_platforma[i]));
				if (player_position == 3 && contor_pozitie_sfera == i && jumping_state == 0)
				{
					RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix2);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
				}
				else
				{
					if (platforme_parcurse[i][3] == 1)
					{
						RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix2);
					}
					if (platforme_parcurse[i][3] == 0)
					{
						RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix2);
					}
					//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix2);
				}



				glm::mat4 modelMatrix3 = glm::mat4(1);
				if (camera_position == 0)
					modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(0 - 1.25f, 1, 0 + angularStep * viteza_platforme - distanta_platforma[i]));
				if (camera_position == 1)
				{
					modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(0 - 1.25f, 1.5f, 0 + angularStep * viteza_platforme - distanta_platforma[i]));
					if (contor_saritura != 0)
					{
						modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(0, -0.25f, 0));
					}
				}
				modelMatrix3 = glm::scale(modelMatrix3, glm::vec3(1, 0.25f, z_platforma[i]));
				if (player_position == 1 && contor_pozitie_sfera == i && jumping_state == 0)
				{
					RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix3);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
				}
				else
				{
					if (platforme_parcurse[i][1] == 1)
					{
						RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix3);
					}
					if (platforme_parcurse[i][1] == 0)
					{
						RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix3);
					}
					//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix3);
				}
			}
		}

		if (random_generate_platforms[i] == 4)
		{

			if (i > 0)
			{
				if (spatiu_platforme_coloane[i] == 1)
				{
					spatiu_platforme_coloane[i] = spatiu_platfomre_vector_random[rand() % 3];
				}
				distanta_platforma[i] = distanta_platforma[i - 1] + ((z_platforma[i - 1] / 2) + spatiu_platforme_coloane[i] + (z_platforma[i] / 2));
			}


			pozitie_z_platforme[i] = angularStep * viteza_platforme - distanta_platforma[i];

			if (pozitie_z_platforme[i] < 5 && pozitie_z_platforme[i] > -10)
			{

				glm::mat4 modelMatrix = glm::mat4(1);
				if (camera_position == 0)
					modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0 + angularStep * viteza_platforme - distanta_platforma[i]));
				if (camera_position == 1)
				{
					modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1.5f, 0 + angularStep * viteza_platforme - distanta_platforma[i]));
					if (contor_saritura != 0)
					{
						modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.25f, 0));
					}
				}
				modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 0.25f, z_platforma[i]));
				if (player_position == 2 && contor_pozitie_sfera == i && jumping_state == 0)
				{
					RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
				}
				else
				{
					if (platforme_parcurse[i][2] == 1)
					{
						RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix);
					}
					if (platforme_parcurse[i][2] == 0)
					{
						RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix);
					}
					//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix);
				}


				if (debug_boxes == 1)
				{
					glm::mat4 modelMatrix2 = glm::mat4(1);
					if (camera_position == 0)
						modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(0 + 1.25f, 1, 0 + angularStep * viteza_platforme - distanta_platforma[i]));
					if (camera_position == 1)
					{
						modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(0 + 1.25f, 1.5f, 0 + angularStep * viteza_platforme - distanta_platforma[i]));
						if (jumping_state == 1)
						{
							modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(0, -0.25f, 0));
						}
					}
					modelMatrix2 = glm::scale(modelMatrix2, glm::vec3(1, 0.25f, z_platforma[i]));
					RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix2);
				}


				glm::mat4 modelMatrix3 = glm::mat4(1);
				if (camera_position == 0)
					modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(0 - 1.25f, 1, 0 + angularStep * viteza_platforme - distanta_platforma[i]));
				if (camera_position == 1)
				{
					modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(0 - 1.25f, 1.5f, 0 + angularStep * viteza_platforme - distanta_platforma[i]));
					if (contor_saritura != 0)
					{
						modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(0, -0.25f, 0));
					}
				}
				modelMatrix3 = glm::scale(modelMatrix3, glm::vec3(1, 0.25f, z_platforma[i]));
				if (player_position == 1 && contor_pozitie_sfera == i && jumping_state == 0)
				{
					RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix3);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
				}
				else
				{
					if (platforme_parcurse[i][1] == 1)
					{
						RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix3);
					}
					if (platforme_parcurse[i][1] == 0)
					{
						RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix3);
					}
					//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix3);
				}
			}

		}

		if (random_generate_platforms[i] == 5)
		{
			
			if(special_platform_indices[i] == 0)
				special_platform_indices[i] = (rand() % 3) + 1;

			if (special_platform_type[i] == 0)
				special_platform_type[i] = (rand() % 4) + 1;
			
			
			if (i > 0)
			{
				if (spatiu_platforme_coloane[i] == 1)
				{
					spatiu_platforme_coloane[i] = spatiu_platfomre_vector_random[rand() % 3];
				}
				//distanta_platforma[i] = distanta_platforma[i - 1] + ( (z_platforma[i - 1] / 2) + 0.25f + (z_platforma[i] / 2) );
				distanta_platforma[i] = distanta_platforma[i - 1] + ((z_platforma[i - 1] / 2) + spatiu_platforme_coloane[i] + (z_platforma[i] / 2));
			}
			pozitie_z_platforme[i] = angularStep - distanta_platforma[i];
			if (pozitie_z_platforme[i] < 5 && pozitie_z_platforme[i] > -10)
			{
				glm::mat4 modelMatrix = glm::mat4(1);
				if (camera_position == 0)
					modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0 + angularStep - distanta_platforma[i]));
				if (camera_position == 1)
				{
					modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1.5f, 0 + angularStep - distanta_platforma[i]));
					if (contor_saritura != 0)
					{
					modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.25f, 0));
					}
				}
				modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 0.25f, z_platforma[i]));


				if (player_position == 2 && contor_pozitie_sfera == i && jumping_state == 0)
				{
					RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
					if (special_platform_indices[i] == 2 && special_platform_type[i] == 1 && jumping_state == 0)
					{
						cout << "YOU ARE DEAD" << endl;
						exit(0);
					}
					if (special_platform_indices[i] == 2 && special_platform_type[i] == 2 && jumping_state == 0)
					{
						//cout << "YOU TOUCHED YELLOW PLATFORM" << endl;
						if (take_fuel_platform_set[i] == 0)
						{
							take_fuel = 1;
							take_fuel_platform_set[i] = 1;
						}
					}
					if (special_platform_indices[i] == 2 && special_platform_type[i] == 3 && jumping_state == 0)
					{
						speed_powerup = 1;
						speed_platform_effect = 1;
					}
					if (special_platform_indices[i] == 2 && special_platform_type[i] == 4 && jumping_state == 0)
					{
						//cout << "YOU TOUCHED GREEN PLATFORM" << endl;
						if (add_fuel_platform_set[i] == 0)
						{
							add_fuel = 1;
							add_fuel_platform_set[i] = 1;
						}
					}
					
				}
				else
				{
					if (special_platform_indices[i] == 2 && special_platform_type[i] == 1)
					{
						RenderMesh(meshes["box"], shaders["RedPlatform"], modelMatrix);
					}
					if (special_platform_indices[i] == 2 && special_platform_type[i] == 2)
					{
						RenderMesh(meshes["box"], shaders["YellowPlatform"], modelMatrix);
					}
					if (special_platform_indices[i] == 2 && special_platform_type[i] == 3)
					{
						RenderMesh(meshes["box"], shaders["HitPlatformShader"], modelMatrix);
					}
					if (special_platform_indices[i] == 2 && special_platform_type[i] == 4)
					{
						RenderMesh(meshes["box"], shaders["GreenPlatform"], modelMatrix);
					}
					else
					{
						if (platforme_parcurse[i][2] == 1)
						{
							RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix);
						}
						if (platforme_parcurse[i][2] == 0)
						{
							RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix);
						}
						//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix);
					}
				}
				glm::mat4 modelMatrix2 = glm::mat4(1);	
				if (camera_position == 0)
					modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(0 + 1.25f, 1, 0 + angularStep - distanta_platforma[i]));
				if (camera_position == 1)
				{
					modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(0 + 1.25f, 1.5f, 0 + angularStep - distanta_platforma[i]));
					if (contor_saritura != 0)
					{
						modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(0, -0.25f, 0));
					}
				}
				modelMatrix2 = glm::scale(modelMatrix2, glm::vec3(1, 0.25f, z_platforma[i]));

				if (player_position == 3 && contor_pozitie_sfera == i && jumping_state == 0)
				{
					RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix2);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
					if (special_platform_indices[i] == 3 && special_platform_type[i] == 1 && jumping_state == 0)
					{
						cout << "YOU ARE DEAD" << endl;
						exit(0);
					}
					if (special_platform_indices[i] == 3 && special_platform_type[i] == 2 && jumping_state == 0)
					{
						//cout << "YOU TOUCHED YELLOW PLATFORM" << endl;
						if (take_fuel_platform_set[i] == 0)
						{
							take_fuel = 1;
							take_fuel_platform_set[i] = 1;
						}
					}
					if (special_platform_indices[i] == 3 && special_platform_type[i] == 3 && jumping_state == 0)
					{
						speed_powerup = 1;
						speed_platform_effect = 1;
					}
					if (special_platform_indices[i] == 3 && special_platform_type[i] == 4 && jumping_state == 0)
					{
						//cout << "YOU TOUCHED GREEN PLATFORM" << endl;
						if (add_fuel_platform_set[i] == 0)
						{
							add_fuel = 1;
							add_fuel_platform_set[i] = 1;
						}
					}
				}
				else
				{
					if (special_platform_indices[i] == 3 && special_platform_type[i] == 1)
					{
						RenderMesh(meshes["box"], shaders["RedPlatform"], modelMatrix2);
					}
					if (special_platform_indices[i] == 3 && special_platform_type[i] == 2)
					{
						RenderMesh(meshes["box"], shaders["YellowPlatform"], modelMatrix2);
					}
					if (special_platform_indices[i] == 3 && special_platform_type[i] == 3)
					{
						RenderMesh(meshes["box"], shaders["HitPlatformShader"], modelMatrix2);
					}
					if (special_platform_indices[i] == 3 && special_platform_type[i] == 4)
					{
						RenderMesh(meshes["box"], shaders["GreenPlatform"], modelMatrix2);
					}
					else
					{
						if (platforme_parcurse[i][3] == 1)
						{
							RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix2);
						}
						if (platforme_parcurse[i][3] == 0)
						{
							RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix2);
						}
						//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix2);
					}
				}

				glm::mat4 modelMatrix3 = glm::mat4(1);
				if (camera_position == 0)
				modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(0 - 1.25f, 1, 0 + angularStep - distanta_platforma[i]));
				if (camera_position == 1)
				{
					modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(0 - 1.25f, 1.5f, 0 + angularStep - distanta_platforma[i]));
					if (contor_saritura != 0)
					{
						modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(0, -0.25f, 0));
					}
				}
				modelMatrix3 = glm::scale(modelMatrix3, glm::vec3(1, 0.25f, z_platforma[i]));
				if (player_position == 1 && contor_pozitie_sfera == i && jumping_state == 0)
				{
					RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix3);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
					if (special_platform_indices[i] == 1 && special_platform_type[i] == 1 && jumping_state == 0)
					{
						cout << "YOU ARE DEAD" << endl;
						exit(0);
					}
					if (special_platform_indices[i] == 1 && special_platform_type[i] == 2 && jumping_state == 0)
					{
						//cout << "YOU TOUCHED YELLOW PLATFORM" << endl;
						if (take_fuel_platform_set[i] == 0)
						{
							take_fuel = 1;
							take_fuel_platform_set[i] = 1;
						}
					}
					if (special_platform_indices[i] == 1 && special_platform_type[i] == 3 && jumping_state == 0)
					{
						speed_powerup = 1;
						speed_platform_effect = 1;
					}
					if (special_platform_indices[i] == 1 && special_platform_type[i] == 4 && jumping_state == 0)
					{
						//cout << "YOU TOUCHED GREEN PLATFORM" << endl;
						if (add_fuel_platform_set[i] == 0)
						{
							add_fuel = 1;
							add_fuel_platform_set[i] = 1;
						}
					}
				}
				else
				{
					if (special_platform_indices[i] == 1 && special_platform_type[i] == 1)
					{
						RenderMesh(meshes["box"], shaders["RedPlatform"], modelMatrix3);
					}
					if (special_platform_indices[i] == 1 && special_platform_type[i] == 2)
					{
						RenderMesh(meshes["box"], shaders["YellowPlatform"], modelMatrix3);
					}
					if (special_platform_indices[i] == 1 && special_platform_type[i] == 3)
					{
						RenderMesh(meshes["box"], shaders["HitPlatformShader"], modelMatrix3);
					}
					if (special_platform_indices[i] == 1 && special_platform_type[i] == 4)
					{
						RenderMesh(meshes["box"], shaders["GreenPlatform"], modelMatrix3);
					}
					else
					{
						if (platforme_parcurse[i][1] == 1)
						{
							RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix3);
						}
						if (platforme_parcurse[i][1] == 0)
						{
							RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix3);
						}
						//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix3);
					}
				}
			}
		}


		if (random_generate_platforms[i] == 6)
		{
			if (special_platform_indices[i] == 0)
				special_platform_indices[i] = (rand() % 3) + 1;

			if (special_platform_type[i] == 0)
				special_platform_type[i] = (rand() % 4) + 1;
			
			
			
			if (i > 0)
			{
				if (spatiu_platforme_coloane[i] == 1)
				{
					spatiu_platforme_coloane[i] = spatiu_platfomre_vector_random[rand() % 3];
				}
				distanta_platforma[i] = distanta_platforma[i - 1] + ((z_platforma[i - 1] / 2) + spatiu_platforme_coloane[i] + (z_platforma[i] / 2));
			}

			pozitie_z_platforme[i] = angularStep * viteza_platforme - distanta_platforma[i];

			if (pozitie_z_platforme[i] < 5 && pozitie_z_platforme[i] > -10)
			{

				glm::mat4 modelMatrix = glm::mat4(1);
				if (camera_position == 0)
					modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0 + angularStep * viteza_platforme - distanta_platforma[i]));
				if (camera_position == 1)
				{
					modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1.5f, 0 + angularStep * viteza_platforme - distanta_platforma[i]));
					if (contor_saritura != 0)
					{
						modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.25f, 0));
					}
				}
				modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 0.25f, z_platforma[i]));
				if (player_position == 2 && contor_pozitie_sfera == i && jumping_state == 0)
				{
					RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
					if (special_platform_indices[i] == 2 && special_platform_type[i] == 1 && jumping_state == 0)
					{
						cout << "YOU ARE DEAD" << endl;
						exit(0);
					}
					if (special_platform_indices[i] == 2 && special_platform_type[i] == 2 && jumping_state == 0)
					{
						//cout << "YOU TOUCHED YELLOW PLATFORM" << endl;
						if (take_fuel_platform_set[i] == 0)
						{
							take_fuel = 1;
							take_fuel_platform_set[i] = 1;
						}
					}
					if (special_platform_indices[i] == 2 && special_platform_type[i] == 3 && jumping_state == 0)
					{
						speed_powerup = 1;
						speed_platform_effect = 1;
					}
					if (special_platform_indices[i] == 2 && special_platform_type[i] == 4 && jumping_state == 0)
					{
						//cout << "YOU TOUCHED GREEN PLATFORM" << endl;
						if (add_fuel_platform_set[i] == 0)
						{
							add_fuel = 1;
							add_fuel_platform_set[i] = 1;
						}
					}
					
				}
				else
				{
					if (special_platform_indices[i] == 2 && special_platform_type[i] == 1)
					{
						RenderMesh(meshes["box"], shaders["RedPlatform"], modelMatrix);
					}
					if (special_platform_indices[i] == 2 && special_platform_type[i] == 2)
					{
						RenderMesh(meshes["box"], shaders["YellowPlatform"], modelMatrix);
					}
					if (special_platform_indices[i] == 2 && special_platform_type[i] == 3)
					{
						RenderMesh(meshes["box"], shaders["HitPlatformShader"], modelMatrix);
					}
					if (special_platform_indices[i] == 2 && special_platform_type[i] == 4)
					{
						RenderMesh(meshes["box"], shaders["GreenPlatform"], modelMatrix);
					}
					else
					{
						if (platforme_parcurse[i][2] == 1)
						{
							RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix);
						}
						if (platforme_parcurse[i][2] == 0)
						{
							RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix);
						}
						//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix);
					}
					//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix);
				}



				glm::mat4 modelMatrix2 = glm::mat4(1);
				if (camera_position == 0)
					modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(0 + 1.25f, 1, 0 + angularStep * viteza_platforme - distanta_platforma[i]));
				if (camera_position == 1)
				{
					modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(0 + 1.25f, 1.5f, 0 + angularStep * viteza_platforme - distanta_platforma[i]));
					if (contor_saritura != 0)
					{
						modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(0, -0.25f, 0));
					}
				}
				modelMatrix2 = glm::scale(modelMatrix2, glm::vec3(1, 0.25f, z_platforma[i]));
				if (player_position == 3 && contor_pozitie_sfera == i && jumping_state == 0)
				{
					RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix2);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
					if (special_platform_indices[i] == 3 && special_platform_type[i] == 1 && jumping_state == 0)
					{
						cout << "YOU ARE DEAD" << endl;
						exit(0);
					}
					if (special_platform_indices[i] == 3 && special_platform_type[i] == 2 && jumping_state == 0)
					{
						//cout << "YOU TOUCHED YELLOW PLATFORM" << endl;
						if (take_fuel_platform_set[i] == 0)
						{
							take_fuel = 1;
							take_fuel_platform_set[i] = 1;
						}
					}
					if (special_platform_indices[i] == 3 && special_platform_type[i] == 3 && jumping_state == 0)
					{
						speed_powerup = 1;
						speed_platform_effect = 1;
					}
					if (special_platform_indices[i] == 3 && special_platform_type[i] == 4 && jumping_state == 0)
					{
						//cout << "YOU TOUCHED GREEN PLATFORM" << endl;
						if (add_fuel_platform_set[i] == 0)
						{
							add_fuel = 1;
							add_fuel_platform_set[i] = 1;
						}
					}
				}
				else
				{
					if (special_platform_indices[i] == 3 && special_platform_type[i] == 1)
					{
						RenderMesh(meshes["box"], shaders["RedPlatform"], modelMatrix2);
					}
					if (special_platform_indices[i] == 3 && special_platform_type[i] == 2)
					{
						RenderMesh(meshes["box"], shaders["YellowPlatform"], modelMatrix2);
					}
					if (special_platform_indices[i] == 3 && special_platform_type[i] == 3)
					{
						RenderMesh(meshes["box"], shaders["HitPlatformShader"], modelMatrix2);
					}
					if (special_platform_indices[i] == 3 && special_platform_type[i] == 4)
					{
						RenderMesh(meshes["box"], shaders["GreenPlatform"], modelMatrix2);
					}
					else
					{
						if (platforme_parcurse[i][3] == 1)
						{
							RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix2);
						}
						if (platforme_parcurse[i][3] == 0)
						{
							RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix2);
						}
						//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix2);
					}
					//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix2);
				}

			}
		}


		if (random_generate_platforms[i] == 7)
		{

			if (special_platform_indices[i] == 0)
				special_platform_indices[i] = (rand() % 3) + 1;

			if (special_platform_type[i] == 0)
				special_platform_type[i] = (rand() % 4) + 1;
			
			
			if (i > 0)
			{
				if (spatiu_platforme_coloane[i] == 1)
				{
					spatiu_platforme_coloane[i] = spatiu_platfomre_vector_random[rand() % 3];
				}
				distanta_platforma[i] = distanta_platforma[i - 1] + ((z_platforma[i - 1] / 2) + spatiu_platforme_coloane[i] + (z_platforma[i] / 2));
			}


			pozitie_z_platforme[i] = angularStep * viteza_platforme - distanta_platforma[i];

			if (pozitie_z_platforme[i] < 5 && pozitie_z_platforme[i] > -10)
			{

				glm::mat4 modelMatrix2 = glm::mat4(1);
				if (camera_position == 0)
					modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(0 + 1.25f, 1, 0 + angularStep * viteza_platforme - distanta_platforma[i]));
				if (camera_position == 1)
				{
					modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(0 + 1.25f, 1.5f, 0 + angularStep * viteza_platforme - distanta_platforma[i]));
					if (contor_saritura != 0)
					{
						modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(0, -0.25f, 0));
					}
				}
				modelMatrix2 = glm::scale(modelMatrix2, glm::vec3(1, 0.25f, z_platforma[i]));
				if (player_position == 3 && contor_pozitie_sfera == i && jumping_state == 0)
				{
					RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix2);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
					if (special_platform_indices[i] == 3 && special_platform_type[i] == 1 && jumping_state == 0)
					{
						cout << "YOU ARE DEAD" << endl;
						exit(0);
					}
					if (special_platform_indices[i] == 3 && special_platform_type[i] == 2 && jumping_state == 0)
					{
						//cout << "YOU TOUCHED YELLOW PLATFORM" << endl;
						if (take_fuel_platform_set[i] == 0)
						{
							take_fuel = 1;
							take_fuel_platform_set[i] = 1;
						}
					}
					if (special_platform_indices[i] == 3 && special_platform_type[i] == 3 && jumping_state == 0)
					{
						speed_powerup = 1;
						speed_platform_effect = 1;
					}
					if (special_platform_indices[i] == 3 && special_platform_type[i] == 4 && jumping_state == 0)
					{
						//cout << "YOU TOUCHED GREEN PLATFORM" << endl;
						if (add_fuel_platform_set[i] == 0)
						{
							add_fuel = 1;
							add_fuel_platform_set[i] = 1;
						}
					}
				}
				else
				{
					if (special_platform_indices[i] == 3 && special_platform_type[i] == 1)
					{
						RenderMesh(meshes["box"], shaders["RedPlatform"], modelMatrix2);
					}
					if (special_platform_indices[i] == 3 && special_platform_type[i] == 2)
					{
						RenderMesh(meshes["box"], shaders["YellowPlatform"], modelMatrix2);
					}
					if (special_platform_indices[i] == 3 && special_platform_type[i] == 3)
					{
						RenderMesh(meshes["box"], shaders["HitPlatformShader"], modelMatrix2);
					}
					if (special_platform_indices[i] == 3 && special_platform_type[i] == 4)
					{
						RenderMesh(meshes["box"], shaders["GreenPlatform"], modelMatrix2);
					}
					else
					{
						if (platforme_parcurse[i][3] == 1)
						{
							RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix2);
						}
						if (platforme_parcurse[i][3] == 0)
						{
							RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix2);
						}
						//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix2);
					}
					//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix2);
				}



				glm::mat4 modelMatrix3 = glm::mat4(1);
				if (camera_position == 0)
					modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(0 - 1.25f, 1, 0 + angularStep * viteza_platforme - distanta_platforma[i]));
				if (camera_position == 1)
				{
					modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(0 - 1.25f, 1.5f, 0 + angularStep * viteza_platforme - distanta_platforma[i]));
					if (contor_saritura != 0)
					{
						modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(0, -0.25f, 0));
					}
				}
				modelMatrix3 = glm::scale(modelMatrix3, glm::vec3(1, 0.25f, z_platforma[i]));
				if (player_position == 1 && contor_pozitie_sfera == i && jumping_state == 0)
				{
					RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix3);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
					if (special_platform_indices[i] == 1 && special_platform_type[i] == 1 && jumping_state == 0)
					{
						cout << "YOU ARE DEAD" << endl;
						exit(0);
					}
					if (special_platform_indices[i] == 1 && special_platform_type[i] == 2 && jumping_state == 0)
					{
						//cout << "YOU TOUCHED YELLOW PLATFORM" << endl;
						if (take_fuel_platform_set[i] == 0)
						{
							take_fuel = 1;
							take_fuel_platform_set[i] = 1;
						}
					}
					if (special_platform_indices[i] == 1 && special_platform_type[i] == 3 && jumping_state == 0)
					{
						speed_powerup = 1;
						speed_platform_effect = 1;
					}
					if (special_platform_indices[i] == 1 && special_platform_type[i] == 4 && jumping_state == 0)
					{
						//cout << "YOU TOUCHED GREEN PLATFORM" << endl;
						if (add_fuel_platform_set[i] == 0)
						{
							add_fuel = 1;
							add_fuel_platform_set[i] = 1;
						}
					}
				}
				else
				{
					if (special_platform_indices[i] == 1 && special_platform_type[i] == 1)
					{
						RenderMesh(meshes["box"], shaders["RedPlatform"], modelMatrix3);
					}
					if (special_platform_indices[i] == 1 && special_platform_type[i] == 2)
					{
						RenderMesh(meshes["box"], shaders["YellowPlatform"], modelMatrix3);
					}
					if (special_platform_indices[i] == 1 && special_platform_type[i] == 3)
					{
						RenderMesh(meshes["box"], shaders["HitPlatformShader"], modelMatrix3);
					}
					if (special_platform_indices[i] == 1 && special_platform_type[i] == 4)
					{
						RenderMesh(meshes["box"], shaders["GreenPlatform"], modelMatrix3);
					}
					else
					{
						if (platforme_parcurse[i][1] == 1)
						{
							RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix3);
						}
						if (platforme_parcurse[i][1] == 0)
						{
							RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix3);
						}
						//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix3);
					}
					//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix3);
				}
			}
		}

		if (random_generate_platforms[i] == 8)
		{

			if (special_platform_indices[i] == 0)
				special_platform_indices[i] = (rand() % 3) + 1;

			if (special_platform_type[i] == 0)
				special_platform_type[i] = (rand() % 4) + 1;
			
			
			if (i > 0)
			{
				if (spatiu_platforme_coloane[i] == 1)
				{
					spatiu_platforme_coloane[i] = spatiu_platfomre_vector_random[rand() % 3];
				}
				distanta_platforma[i] = distanta_platforma[i - 1] + ((z_platforma[i - 1] / 2) + spatiu_platforme_coloane[i] + (z_platforma[i] / 2));
			}


			pozitie_z_platforme[i] = angularStep * viteza_platforme - distanta_platforma[i];

			if (pozitie_z_platforme[i] < 5 && pozitie_z_platforme[i] > -10)
			{

				glm::mat4 modelMatrix = glm::mat4(1);
				if (camera_position == 0)
					modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0 + angularStep * viteza_platforme - distanta_platforma[i]));
				if (camera_position == 1)
				{
					modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1.5f, 0 + angularStep * viteza_platforme - distanta_platforma[i]));
					if (contor_saritura != 0)
					{
						modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.25f, 0));
					}
				}
				modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 0.25f, z_platforma[i]));
				if (player_position == 2 && contor_pozitie_sfera == i && jumping_state == 0)
				{
					RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
					if (special_platform_indices[i] == 2 && special_platform_type[i] == 1 && jumping_state == 0)
					{
						cout << "YOU ARE DEAD" << endl;
						exit(0);
					}
					if (special_platform_indices[i] == 2 && special_platform_type[i] == 2 && jumping_state == 0)
					{
						//cout << "YOU TOUCHED YELLOW PLATFORM" << endl;
						if (take_fuel_platform_set[i] == 0)
						{
							take_fuel = 1;
							take_fuel_platform_set[i] = 1;
						}
					}
					if (special_platform_indices[i] == 2 && special_platform_type[i] == 3 && jumping_state == 0)
					{
						speed_powerup = 1;
						speed_platform_effect = 1;
					}
					if (special_platform_indices[i] == 2 && special_platform_type[i] == 4 && jumping_state == 0)
					{
						//cout << "YOU TOUCHED GREEN PLATFORM" << endl;
						if (add_fuel_platform_set[i] == 0)
						{
							add_fuel = 1;
							add_fuel_platform_set[i] = 1;
						}
					}
				}
				else
				{
					if (special_platform_indices[i] == 2 && special_platform_type[i] == 1)
					{
						RenderMesh(meshes["box"], shaders["RedPlatform"], modelMatrix);
					}
					if (special_platform_indices[i] == 2 && special_platform_type[i] == 2)
					{
						RenderMesh(meshes["box"], shaders["YellowPlatform"], modelMatrix);
					}
					if (special_platform_indices[i] == 2 && special_platform_type[i] == 3)
					{
						RenderMesh(meshes["box"], shaders["HitPlatformShader"], modelMatrix);
					}
					if (special_platform_indices[i] == 2 && special_platform_type[i] == 4)
					{
						RenderMesh(meshes["box"], shaders["GreenPlatform"], modelMatrix);
					}
					else
					{
						if (platforme_parcurse[i][2] == 1)
						{
							RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix);
						}
						if (platforme_parcurse[i][2] == 0)
						{
							RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix);
						}
						//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix);
					}
					//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix);
				}


				glm::mat4 modelMatrix3 = glm::mat4(1);
				if (camera_position == 0)
					modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(0 - 1.25f, 1, 0 + angularStep * viteza_platforme - distanta_platforma[i]));
				if (camera_position == 1)
				{
					modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(0 - 1.25f, 1.5f, 0 + angularStep * viteza_platforme - distanta_platforma[i]));
					if (contor_saritura != 0)
					{
						modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(0, -0.25f, 0));
					}
				}
				modelMatrix3 = glm::scale(modelMatrix3, glm::vec3(1, 0.25f, z_platforma[i]));
				if (player_position == 1 && contor_pozitie_sfera == i && jumping_state == 0)
				{
					RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix3);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
					if (special_platform_indices[i] == 1 && special_platform_type[i] == 1 && jumping_state == 0)
					{
						cout << "YOU ARE DEAD" << endl;
						exit(0);
					}
					if (special_platform_indices[i] == 1 && special_platform_type[i] == 2 && jumping_state == 0)
					{
						//cout << "YOU TOUCHED YELLOW PLATFORM" << endl;
						if (take_fuel_platform_set[i] == 0)
						{
							take_fuel = 1;
							take_fuel_platform_set[i] = 1;
						}
					}
					if (special_platform_indices[i] == 1 && special_platform_type[i] == 3 && jumping_state == 0)
					{
						speed_powerup = 1;
						speed_platform_effect = 1;
					}
					if (special_platform_indices[i] == 1 && special_platform_type[i] == 4 && jumping_state == 0)
					{
						//cout << "YOU TOUCHED GREEN PLATFORM" << endl;
						if (add_fuel_platform_set[i] == 0)
						{
							add_fuel = 1;
							add_fuel_platform_set[i] = 1;
						}
					}
				}
				else
				{
					if (special_platform_indices[i] == 1 && special_platform_type[i] == 1)
					{
						RenderMesh(meshes["box"], shaders["RedPlatform"], modelMatrix3);
					}
					if (special_platform_indices[i] == 1 && special_platform_type[i] == 2)
					{
						RenderMesh(meshes["box"], shaders["YellowPlatform"], modelMatrix3);
					}
					if (special_platform_indices[i] == 1 && special_platform_type[i] == 3)
					{
						RenderMesh(meshes["box"], shaders["HitPlatformShader"], modelMatrix3);
					}
					if (special_platform_indices[i] == 1 && special_platform_type[i] == 4)
					{
						RenderMesh(meshes["box"], shaders["GreenPlatform"], modelMatrix3);
					}
					else
					{
						if (platforme_parcurse[i][1] == 1)
						{
							RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix3);
						}
						if (platforme_parcurse[i][1] == 0)
						{
							RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix3);
						}
						//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix3);
					}
					//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix3);
				}
			}

		}

	}
}


void Tema2::VerificareCadere()
{
	
	if (pozitie_z_platforme[contor_pozitie_sfera] - (z_platforma[contor_pozitie_sfera] / 2) < 1.5f < pozitie_z_platforme[contor_pozitie_sfera] + (z_platforma[contor_pozitie_sfera] / 2))
	{
		//if (platforma_parcursa[contor_pozitie_sfera] == 0)
		if(jumping_state == 0)
		{
			//cout << "PLATFORMA NR ESTE " << contor_pozitie_sfera << endl;
			if (player_position == 1)
			{
				if (random_generate_platforms[contor_pozitie_sfera] == 2)
				{
					is_player_dead = 1;
					if (camera_position == 1)
					{
						exit(0);
					}
					//exit(0);
					//cout << random_generate_platforms[contor_pozitie_sfera] << endl;
				}
				if (random_generate_platforms[contor_pozitie_sfera] == 6)
				{
					is_player_dead = 1;
					if (camera_position == 1)
					{
						exit(0);
					}
					//exit(0);
					//cout << random_generate_platforms[contor_pozitie_sfera] << endl;
				}
			}
			if (player_position == 2)
			{
				if (random_generate_platforms[contor_pozitie_sfera] == 3)
				{
					is_player_dead = 1;
					if (camera_position == 1)
					{
						exit(0);
					}
					//exit(0);
						//cout << random_generate_platforms[contor_pozitie_sfera] << endl;
				}
				if (random_generate_platforms[contor_pozitie_sfera] == 7)
				{
					is_player_dead = 1;
					if (camera_position == 1)
					{
						exit(0);
					}
					//exit(0);
					//cout << random_generate_platforms[contor_pozitie_sfera] << endl;
				}
			}
			if (player_position == 3)
			{
				if (random_generate_platforms[contor_pozitie_sfera] == 4)
				{
					is_player_dead = 1;
					if (camera_position == 1)
					{
						exit(0);
					}
					//exit(0);
					//cout << random_generate_platforms[contor_pozitie_sfera] << endl;
				}
				if (random_generate_platforms[contor_pozitie_sfera] == 8)
				{
					is_player_dead = 1;
					if (camera_position == 1)
					{
						exit(0);
					}
					//exit(0);
					//cout << random_generate_platforms[contor_pozitie_sfera] << endl;
				}
			}
			//platforma_parcursa[contor_pozitie_sfera] = 1;
		}
	}
	if (pozitie_z_platforme[contor_pozitie_sfera] - (z_platforma[contor_pozitie_sfera] / 2) > 1.5f)
	{
		contor_pozitie_sfera++;
	}
}

void Tema2::Update(float deltaTimeSeconds)
{
	angularStep += deltaTimeSeconds;

	factor_deformare += 5 * deltaTimeSeconds;

	latura_powerbar = fuel/200;

	
	//glm::mat4 modelMatrix10 = glm::mat4(1);
	//modelMatrix10 = glm::translate(modelMatrix10, glm::vec3(0.45f, 0.9f, 0));
	////modelMatrix10 = glm::scale(modelMatrix10, glm::vec3(0.5f / 2, 0.1f / 2, 0));
	//RenderSpecialMesh(meshes["sphere"], shaders["Noise"], modelMatrix10, ceva);



	// incepem desenarea UI-ului

	if (latura_powerbar > 0)
	{
			glm::mat4 modelMatrix4 = glm::mat4(1);
			modelMatrix4 = glm::translate(modelMatrix4, glm::vec3(0.45f, 0.9f, 0));
			modelMatrix4 = glm::translate(modelMatrix4, glm::vec3(0.5f * latura_powerbar, 0, 0));
			modelMatrix4 = glm::scale(modelMatrix4, glm::vec3(latura_powerbar/2, 0.1f/2, 0));
			//modelMatrix4 = glm::scale(modelMatrix4, glm::vec3(0.5f, 1, 0));
			if(speed_platform_effect == 0)
				RenderMesh(meshes["rectangle"], shaders["UI"], modelMatrix4);
			if(speed_platform_effect == 1)
				RenderMesh(meshes["rectangle"], shaders["SpeedPowerup2"], modelMatrix4);

	}

	glm::mat4 modelMatrix5 = glm::mat4(1);
	modelMatrix5 = glm::translate(modelMatrix5, glm::vec3(0.45f, 0.9f, 0));
	modelMatrix5 = glm::translate(modelMatrix5, glm::vec3(0.25f, 0, 0));
	modelMatrix5 = glm::scale(modelMatrix5, glm::vec3(0.5f/2, 0.1f/2, 0));
	RenderMesh(meshes["rectangle"], shaders["UI2"], modelMatrix5);

	
	if (change_speed == 0)
	{
		glm::mat4 modelMatrix7 = glm::mat4(1);
		modelMatrix7 = glm::translate(modelMatrix7, glm::vec3(0.45f, 0.7f, 0));
		modelMatrix7 = glm::translate(modelMatrix7, glm::vec3(0.5f * 0.125f, 0, 0));
		modelMatrix7 = glm::scale(modelMatrix7, glm::vec3(0.125f/2, 0.1f/2, 0));
		RenderMesh(meshes["rectangle"], shaders["SpeedPowerup"], modelMatrix7);
	}

	
	if (change_speed == 1)
	{
		glm::mat4 modelMatrix7 = glm::mat4(1);
		modelMatrix7 = glm::translate(modelMatrix7, glm::vec3(0.45f, 0.7f, 0));
		modelMatrix7 = glm::translate(modelMatrix7, glm::vec3(0.5f * 0.26f, 0, 0));
		modelMatrix7 = glm::scale(modelMatrix7, glm::vec3(0.26f/2, 0.1f/2, 0));
		RenderMesh(meshes["rectangle"], shaders["SpeedPowerup"], modelMatrix7);
	}

	if (change_speed == 2)
	{
		glm::mat4 modelMatrix7 = glm::mat4(1);
		modelMatrix7 = glm::translate(modelMatrix7, glm::vec3(0.45f, 0.7f, 0));
		modelMatrix7 = glm::translate(modelMatrix7, glm::vec3(0.5f * 0.375f, 0, 0));
		modelMatrix7 = glm::scale(modelMatrix7, glm::vec3(0.375f/2, 0.1f/2, 0));
		RenderMesh(meshes["rectangle"], shaders["SpeedPowerup"], modelMatrix7);
	}

	if (change_speed == 3)
	{
		glm::mat4 modelMatrix7 = glm::mat4(1);
		modelMatrix7 = glm::translate(modelMatrix7, glm::vec3(0.45f, 0.7f, 0));
		modelMatrix7 = glm::translate(modelMatrix7, glm::vec3(0.5f * 0.5f, 0, 0));
		modelMatrix7 = glm::scale(modelMatrix7, glm::vec3(0.5f/2, 0.1f/2, 0));
		RenderMesh(meshes["rectangle"], shaders["SpeedPowerup"], modelMatrix7);
	}

	if(change_speed != 4)
		previous_change_speed = change_speed;


	if (change_speed == 4)
	{
		
		if (previous_change_speed == 0)
		{
			glm::mat4 modelMatrix7 = glm::mat4(1);
			modelMatrix7 = glm::translate(modelMatrix7, glm::vec3(0.45f, 0.7f, 0));
			modelMatrix7 = glm::translate(modelMatrix7, glm::vec3(0.5f * 0.125f, 0, 0));
			modelMatrix7 = glm::scale(modelMatrix7, glm::vec3(0.125f/2, 0.1f/2, 0));
			RenderMesh(meshes["rectangle"], shaders["SpeedPowerup2"], modelMatrix7);
		}
		if (previous_change_speed == 1)
		{
			glm::mat4 modelMatrix7 = glm::mat4(1);
			modelMatrix7 = glm::translate(modelMatrix7, glm::vec3(0.45f, 0.7f, 0));
			modelMatrix7 = glm::translate(modelMatrix7, glm::vec3(0.5f * 0.26f, 0, 0));
			modelMatrix7 = glm::scale(modelMatrix7, glm::vec3(0.26f/2, 0.1f/2, 0));
			RenderMesh(meshes["rectangle"], shaders["SpeedPowerup2"], modelMatrix7);
		}
		if (previous_change_speed == 2)
		{
			glm::mat4 modelMatrix7 = glm::mat4(1);
			modelMatrix7 = glm::translate(modelMatrix7, glm::vec3(0.45f, 0.7f, 0));
			modelMatrix7 = glm::translate(modelMatrix7, glm::vec3(0.5f * 0.375f, 0, 0));
			modelMatrix7 = glm::scale(modelMatrix7, glm::vec3(0.375f/2, 0.1f/2, 0));
			RenderMesh(meshes["rectangle"], shaders["SpeedPowerup2"], modelMatrix7);
		}
		if(previous_change_speed == 3)
		{
			glm::mat4 modelMatrix7 = glm::mat4(1);
			modelMatrix7 = glm::translate(modelMatrix7, glm::vec3(0.45f, 0.7f, 0));
			modelMatrix7 = glm::translate(modelMatrix7, glm::vec3(0.5f * 0.5f, 0, 0));
			modelMatrix7 = glm::scale(modelMatrix7, glm::vec3(0.5f/2, 0.1f/2, 0));
			RenderMesh(meshes["rectangle"], shaders["SpeedPowerup2"], modelMatrix7);
		}
		
	}
	
	glm::mat4 modelMatrix6 = glm::mat4(1);
	modelMatrix6 = glm::translate(modelMatrix6, glm::vec3(0.45f, 0.7f, 0));
	modelMatrix6 = glm::translate(modelMatrix6, glm::vec3(0.25f, 0, 0));
	modelMatrix6 = glm::scale(modelMatrix6, glm::vec3(0.5f/2, 0.1f/2, 0));
	RenderMesh(meshes["rectangle"], shaders["UI2"], modelMatrix6);

	// Terminam desenarea UI-ului
	
	// scadem combustibilul in functie de ce viteza are player-ul

	contor_fuel++;

	if (change_speed == 1 && contor_fuel % 20 == 0)
	{
		fuel = fuel - 0.25f;
	}
	if (change_speed == 2 && contor_fuel % 20 == 0)
	{
		fuel = fuel - 0.5f;
	}
	if (change_speed == 3 && contor_fuel % 20 == 0)
	{
		fuel = fuel - 1.5f;
	}

	//cout << "FUEL ESTE  " << fuel << endl;

	// Incrementam contorul de saritura pana ajunge la 70, adica tinem bila in pozitia jump pana se ajunge la 70

	if (contor_saritura > 0)
	{
		contor_saritura++;
	}

	if (contor_saritura == 70)
	{
		contor_saritura = 0;
	}

	// Daca player-ul a atins o platforma galbena scadem o valoare random din fuel

	if (take_fuel == 1)
	{
		fuel = fuel - (rand() % 30);	// ultima modificare
		take_fuel = 0;

		contor_deformare = 1;
	}

	if (contor_deformare > 0)
	{
		contor_deformare++; 
		if (contor_deformare == 200)
		{
			contor_deformare = 0;
		}
	}

	// Daca player-ul a atins o platforma verde adaugam o valoare random la fuel

	if (add_fuel == 1)
	{
		fuel = fuel + (rand() % 50);
		if (fuel > 100)
		{
			fuel = 100;
		}
		add_fuel = 0;
		contor_deformare = 1;
	}
	
	if (contor_deformare > 0)
	{
		contor_deformare++;
		if (contor_deformare == 200)
		{
			contor_deformare = 0;
		}
	}


	// Daca fuel ajunge la o valoare mai mica sau egala cu 0 jocul se termina

	if (fuel <= 0)
	{
		cout << "AI RAMAS FARA COMBUSTIBIL! JOCUL S-A TERMINAT! " << endl;
		exit(0);
	}

	if (change_speed == 1)
	{
		angularStep += 0.05f;
	}

	if (change_speed == 2)
	{
		angularStep += 0.1f;
	}

	if (change_speed == 3)
	{
		angularStep += 0.15f;
	}

	if (change_speed == 4)
	{
		angularStep += 0.2f;
	}

	if (change_speed == -1)
	{
		angularStep -= 0.01f;
	}
	
	// Daca jucatorul atinge o platforma portocalie, ii limitam posibilitatea de a schimba viteza si il blocam la o viteza mai mare pana cand speed powerup este 100

	if (speed_powerup > 0)
	{
		if (Semafor_speed_powerup == 0)
		{
			previous_player_speed = change_speed;
			Semafor_speed_powerup = 1;
		}
		speed_powerup++;
		change_speed = 4;
	}
	if (speed_powerup == 100)
	{
		speed_powerup = 0;
		speed_platform_effect = 0;
		Semafor_speed_powerup = 0;
		change_speed = previous_player_speed;
	}

	// Desenam player-ul, platformele, si verificam daca acesta a cazut de pe platforma

	DrawPlayer(deltaTimeSeconds);

	DrawPlatforms();
		
	VerificareCadere();

}

void Tema2::FrameEnd()
{
	DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
	if (!mesh || !shader || !shader->program)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	mesh->Render();
}

void Tema2::RenderSpecialMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix,  float factor_deformare)
{
	if (!mesh || !shader || !shader->program)
		return;

	
	// render an object using the specified shader and the specified position
	shader->Use();
	int position = glGetUniformLocation(shader->program, "factor_deformare");
	glUniform1f(position, factor_deformare);
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	mesh->Render();
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	// move the camera only if MOUSE_RIGHT button is pressed
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float cameraSpeed = 2.0f;

		if (window->KeyHold(GLFW_KEY_W)) {
			// TODO : translate the camera forward
			camera->MoveForward(0.5f * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			// TODO : translate the camera to the left
			camera->TranslateRight(-0.5f * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			// TODO : translate the camera backwards
			camera->MoveForward(-0.5f * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			// TODO : translate the camera to the right
			camera->TranslateRight(0.5f * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_Q)) {
			// TODO : translate the camera down
			camera->TranslateUpword(-0.5f * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_E)) {
			// TODO : translate the camera up
			camera->TranslateUpword(0.5f * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_UP)) {
			FOV = FOV + FOV * deltaTime;
		}
		if (window->KeyHold(GLFW_KEY_DOWN)) {
			FOV = FOV - FOV * deltaTime;
		}
		if (window->KeyHold(GLFW_KEY_K)) {
			top1 = top1 + top1 * deltaTime;
		}
	}
}

void Tema2::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_T)
	{
		renderCameraTarget = !renderCameraTarget;
	}
	if (key == GLFW_KEY_P)
	{
		projectionMatrix = glm::perspective(FOV, 1, -1, 1);
	}
	if (key == GLFW_KEY_O)
	{
		projectionMatrix = glm::ortho(left1,right1,bot1,top1,  -5,  5);
	}
	if (key == GLFW_KEY_A)
	{
		//previous_player_position = player_position;
		if (camera_position == 1 && (player_position == 2 || player_position == 3))
			camera->TranslateRight(-1.25f);
		if (player_position == 2 || player_position == 3)
		{
			player_position--;
		}
		
	}
	if (key == GLFW_KEY_D)
	{
		if (camera_position == 1 && (player_position == 1 || player_position == 2))
		{
			camera->TranslateRight(1.25f);
		}
		if (player_position == 1 || player_position == 2)
		{
			player_position++;
		}
		
	}
	if (key == GLFW_KEY_C)
	{
		if (camera_position == 0)
		{
			camera_position = 1;
		}
		else
		{
			camera_position = 0;
		}
			
		camera->stateCamera();

		if (player_position == 1 && camera_position == 0)
		{
			camera->TranslateRight(+1.25f);
		}

		if (player_position == 3 && camera_position == 0)
		{
			camera->TranslateRight(-1.25f);
		}
		if (player_position == 1 && camera_position == 1)
		{
			camera->TranslateRight(-1.25f);
		}
		if (player_position == 3 && camera_position == 1)
		{
			camera->TranslateRight(+1.25f);
		}
	}
	if (key == GLFW_KEY_SPACE)
	{
		if (jumping_state == 0)
		{
			jumping_state = 1;
			contor_saritura = 1;
		}
		if (jumping_state == 0 && camera_position == 1)
		{
			jumping_state == 1;
			contor_saritura = 1;
		}
		
	}
	if (key == GLFW_KEY_R)
	{
		jumping_state = 1;
	}
	if (key == GLFW_KEY_0)
	{
		if (debug_boxes == 0)
			debug_boxes = 1;
		else
			debug_boxes = 0;
	}
	if (key == GLFW_KEY_W)
	{
		if (speed_platform_effect == 0)
		{
			if (change_speed < 3)
			{
				change_speed++;
			}
		}
	}
	if (key == GLFW_KEY_S)
	{
		if (speed_platform_effect == 0)
		{
			if (change_speed > 0)
			{
				change_speed--;
			}
		}
	}
	
}

void Tema2::OnKeyRelease(int key, int mods)
{
	// add key release event
	/*if (key == GLFW_KEY_SPACE)
	{
		infinite_jump = 0;
	}*/
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event

	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

		if (window->GetSpecialKeyState() == 0) {
			renderCameraTarget = false;
			// TODO : rotate the camera in First-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed
			camera->RotateFirstPerson_OX(-sensivityOX * deltaY);
			camera->RotateFirstPerson_OY(-sensivityOY * deltaX);
		}

		if (window->GetSpecialKeyState() && GLFW_MOD_CONTROL) {
			renderCameraTarget = true;
			// TODO : rotate the camera in Third-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed
			camera->RotateThirdPerson_OX(-sensivityOX * deltaY);
			camera->RotateThirdPerson_OY(-sensivityOY * deltaX);
		}

	}
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}
