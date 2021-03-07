#include "Tema1.h"

#include <vector>
#include <iostream>
#include <math.h>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"

using namespace std;

char difficulty[10] = "easy";
int numar_baloane_rosii = 10;	// numarul de baloane rosii care se spawneaza (setam -1 daca vrem sa nu apara niciunul)
int numar_baloane_galbene = 10;	// numarul de baloane galbene care se spawneaza (setam -1 daca vrem sa nu apara niciunul)
int numar_baloane_mov = 10;	// numarul de baloane galbene care se spawneaza (setam -1 daca vrem sa nu apara niciunul)
int numar_shurikenuri = 10;	// numarul de shurikenuri care apar
int counter = 0;
int viteza_shurikenuri = 50;
float speed_baloane_rosii[3001];
float speed_baloane_galbene[3001];
float speed_baloane_mov[3001];
float theta;	// unghiul arcului care face parte din bow
int show_arrow = false;		// variabila care ne spune cand afisam sageata dupa ce tragem
int show_ballons = false;	// variabila care ne spune daca dorim sa afisam sau nu baloanele
int show_shuriken = false;		// variabila care ne spune daca dorim sa afisam sau nu shurikenurile
float copie_angular = 0;		// de eliminat
float viteze_proiectile[100];		// vector in care stocam viteza fiecarei sageti
float OY_proiectile[100] = { 0 };	// vector in care stocam pozitia pe axa OY a fiecarei sageti
float OX_proiectile[100] = { 0 };   // vector in care stocam pozitia pe axa OX a fiecarei sageti
float OX_shuriken[100] = { 0 };		// vector in care stocam pozitia pe axa OX a fiecarui shuriken
float OY_shuriken[100] = { 0 };		// vector in care stocam pozitia pe axa OY a fiecarui shuriken
float inclinare_tinta = 0;	// variabila in care stocam inclinarea unei sageti fata de axa OX, practic panta
float latura_shuriken = 50;	// cat are latura un shuriken de pe mid
float latura_shuriken2 = 50; // cat are latura un shuriken de pe top
float latura_shuriken3 = 50; // cat are latura un shuriken de pe bot
int OK = 0;		// semafor care ne spune daca un shuriken de pe mid a fost lovit de o sageata
int OK2 = 0;	// semafor care ne spune daca un shuriken de pe top a fost lovit de o sageata
int OK3 = 0;	// semafor care ne spune daca un shuriken de pe bot a fost lovit de o sageata
float laturi_shuriken_mid[5001];	// vector in care stocam latura fiecarui shuriken de pe mid
float laturi_shuriken_top[5001];	// vector in care stocam latura fiecarui shuriken de pe top
float laturi_shuriken_bot[5001];	// vector in care stocam latura fiecarui shuriken de pe bot
vector<float> baloane_red_x(numar_baloane_rosii * 300 + 1);
vector<float> baloane_red_y(numar_baloane_rosii * 300 + 1);
vector<float> baloane_yellow_x(numar_baloane_galbene * 300 + 1);
vector<float> baloane_yellow_y(numar_baloane_galbene * 300 + 1);
vector<float> baloane_mov_x(numar_baloane_mov * 300 + 1);
vector<float> baloane_mov_y(numar_baloane_mov * 300 + 1);
//float baloane_red_x[3001];		// vector in care stocam pozitia pe axa OX a fiecarui balon rosu
//float baloane_red_y[3001];		// vector in care stocam pozitia pe axa OY a fiecarui balon rosu
//float baloane_yellow_x[3001];	// vector in care stocam pozitia pe axa OX a fiecarui balon galben
//float baloane_yellow_y[3001];	// vector in care stocam pozitia pe axa OY a fiecarui balon galben
int dead_shuriken_id_mid = 0;	// pastram id-ul shurikenul care a fost distrus de pe mid
int dead_shuriken_id_top = 0;	// pastram id-ul shurikenul care a fost distrus de pe top
int dead_shuriken_id_bot = 0;	// pastram id-ul shurikenul care a fost distrus de pe bot
int dead_red_ballon = 0;	// pastram id-ul balonului rosu care a fost distrus
int dead_yellow_ballon = 0;	// pastram id-ul balonului rosu care a fost distrus
int dead_mov_ballon = 0;
float arrow_speed = 1;
float speed2[3001];	// viteza cu care se misca linia de la baloanele galbene
float speed3[3001];	// viteza cu care se misca linia de la baloanele rosii
float speed4[3001];
int dead_shuriken_ids_mid[5001];	// vector in care stocam daca un shuriken de pe mid este distrus sau nu
int dead_shuriken_ids_bot[5001];	// vector in care stocam daca un shuriken de pe bot este distrus sau nu
int dead_shuriken_ids_top[5001];	// vector in care stocam daca un shuriken de pe top este distrus sau nu
int previous_score = 0;	// scorul anterior
int score = 0;	// scorul curent
int rosu_vechi = -1;	// id-ul balonului rosu care a fost distrus anterior
int galben_vechi = -1;	// id-ul balonului galben care a fost distrus anterior
int mov_vechi = -1;
int life_count_prev = 3;	// numarul de vieti anterior
int life_count = 3;	// numarul de vieti curent
int shuriken_mid_vechi = -1;	// id-ul shurikenului mid care a fost distrus anterior
int shuriken_top_vechi = -1;	// id-ul shurikenului top care a fost distrus anterior
int shuriken_bot_vechi = -1;	// id-ul shurikenului bot care a fost distrus anterior
int dead_arrows[101] = { 0 };	// vector in care stocam care sageata a fost distrusa
int fix_orientation[101] = { 0 };	// vector in care stocam daca o sageata a fost "trasa" sau nu, astfel daca vrem sa tragem o sageata in sus si dupa una in jos sa nu se modifice inclinarea unei sageti in functie de alta
float inclinare_proiectile[101] = { 0 };

int dead_personaj_arrows[101] = { 0 };
int happy_contor = 1;
int speed_personaj = 60;
int semafor = false;
float copie_angular_2 = 0;
bool semafor_game_over = false;
bool draw_personaj_status = false;

//int destroy_yellow_ballons = false;
//int destroy_red_ballons = false;
//int raza = 100, xc, yc, latura = 1280;
//float random_number;
//int semafor = false;
//float copie = 0;
//int OK_nou = 0;
//float player_speed = 1;
//float baloane_yellow_x = 50;
//float baloane_yellow_y = 75;
//float baloane_red_x = 50;
//float baloane_red_y = 75;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}


void Tema1::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	glm::vec3 corner = glm::vec3(0, 0, 0);
	float squareSide = 100;

	// compute coordinates of square center
	float cx = corner.x + squareSide / 2;
	float cy = corner.y + squareSide / 2;
	
	// initialize tx and ty (the translation steps)
	translateX = 0;
	translateY = 0;

	// initialize sx and sy (the scale factors)
	scaleX = 1;
	scaleY = 1;
	
	// initialize angularStep
	angularStep = 0;
	

	Mesh* square1 = Object2D::CreateSquare("square1", corner, squareSide, glm::vec3(1, 0, 0), true);
	AddMeshToList(square1);
	
	Mesh* square2 = Object2D::CreateSquare("square2", corner, squareSide, glm::vec3(0, 1, 0));
	AddMeshToList(square2);

	Mesh* square3 = Object2D::CreateSquare("square3", corner, squareSide, glm::vec3(0, 0, 1));
	AddMeshToList(square3);

	for (int i = 0; i < 5001; i++)
	{
		laturi_shuriken_mid[i] = 50;
		laturi_shuriken_top[i] = 50;
		laturi_shuriken_bot[i] = 50;
	}
	for (int i = 0; i < numar_baloane_rosii * 300 + 1; i++)
	{
		baloane_red_x[i] = 50;
		baloane_red_y[i] = 75;
		if (strcmp(difficulty, "easy") == 0)
		{
			speed3[i] = 80;
			speed_baloane_rosii[i] = 80;
		}
		if (strcmp(difficulty, "medium") == 0)
		{
			speed3[i] = 100;
			speed_baloane_rosii[i] = 100;
		}
		if (strcmp(difficulty, "hard") == 0)
		{
			speed3[i] = 120;
			speed_baloane_rosii[i] = 120;
		}
		
	}
	for (int i = 0; i < numar_baloane_galbene * 300 + 1; i++)		
	{
		baloane_yellow_y[i] = 75;
		baloane_yellow_x[i] = 50;
		if (strcmp(difficulty, "easy") == 0)
		{
			speed2[i] = 100;
			speed_baloane_galbene[i] = 100;
		}
		if (strcmp(difficulty, "medium") == 0)
		{
			speed2[i] = 120;
			speed_baloane_galbene[i] = 120;
		}
		if (strcmp(difficulty, "hard") == 0)
		{
			speed2[i] = 140;
			speed_baloane_galbene[i] = 140;
		}

	}
	for (int i = 0; i < numar_baloane_mov * 300 + 1; i++)
	{
		baloane_mov_y[i] = 75;
		baloane_mov_x[i] = 50;
		if (strcmp(difficulty, "easy") == 0)
		{
			speed4[i] = 100;
			speed_baloane_mov[i] = 100;
		}
		if (strcmp(difficulty, "medium") == 0)
		{
			speed4[i] = 120;
			speed_baloane_mov[i] = 120;
		}
		if (strcmp(difficulty, "hard") == 0)
		{
			speed4[i] = 140;
			speed_baloane_mov[i] = 140;
		}
		//speed4[i] = 100;
		//speed_baloane_mov[i] = 100;

	}
	for (int i = 0; i < 5001; i++)
	{
		dead_shuriken_ids_mid[i] = -1;
		dead_shuriken_ids_top[i] = -1;
		dead_shuriken_ids_bot[i] = -1;
	}

	if (strcmp(difficulty, "easy") == 0)
	{
		cout << endl;
		cout << "DIFFICULTY : EASY" << endl;
		viteza_shurikenuri = 60;
	}
	if (strcmp(difficulty, "medium") == 0)
	{
		cout << endl;
		cout << "DIFFICULTY : MEDIUM" << endl;
		viteza_shurikenuri = 100;
	}
	if (strcmp(difficulty, "hard") == 0)
	{
		cout << endl;
		cout << "DIFFICULTY : HARD" << endl;
		viteza_shurikenuri = 140;
	}

	cout << endl;
	cout << "--------------------------------------------------------------------------------------\n";
	cout << "Press K to show shurikens. Press B to show ballons." << endl;
	cout << "Press 1,2,3 to change arrow's speed." << endl;
	cout << "--------------------------------------------------------------------------------------\n";
	cout << "Life count : 3" << endl;
	cout << "Arrow speed : " << arrow_speed << endl;
	cout << "Player speed : " << "to be implemented" << endl;
	cout << "--------------------------------------------------------------------------------------\n";
	cout << "SCORE: 0\n";
}

void draw_crosshair(float inclinare_tinta, float translateY)
{
	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
	glVertex2f(10, 300 + translateY * 60);
	glVertex2f(300, 300 + inclinare_tinta * 60 + translateY * 60);
	glEnd();
}

void draw_arrow(float deltaTimeSeconds,float translateY)
{
	glColor3f(1, 1, 1);
	for (int r = 1; r < 100; r++)
	{
		if (counter >= r)
		{
			if (OY_proiectile[r] == 0)
				OY_proiectile[r] = 300 + translateY * 60;
			viteze_proiectile[r] += deltaTimeSeconds * arrow_speed;
			OX_proiectile[r] = 300 + viteze_proiectile[r] * 100;
			if (show_arrow == true && counter >= r && dead_arrows[r] != 1)
			{
				if (fix_orientation[r] == 0)
				{
					fix_orientation[r] = 1;
					inclinare_proiectile[r] = inclinare_tinta;

				}
				glBegin(GL_LINES);
				glVertex2f(10 + viteze_proiectile[r] * 100, OY_proiectile[r] + viteze_proiectile[r] * (inclinare_proiectile[r] * 60 / 290) * 100);
				glVertex2f(300 + viteze_proiectile[r] * 100, OY_proiectile[r] + inclinare_proiectile[r] * 60 + viteze_proiectile[r] * (inclinare_proiectile[r] * 60 / 290) * 100);
				glEnd();
				if (inclinare_proiectile[r] == 0)	// ultima modificare 2
				{
					glBegin(GL_TRIANGLES);
					glVertex2f(300 + viteze_proiectile[r] * 100, OY_proiectile[r] + inclinare_proiectile[r] * 60 + viteze_proiectile[r] * (inclinare_proiectile[r] * 60 / 290) * 100);
					glVertex2f(250 + viteze_proiectile[r] * 100, OY_proiectile[r] + 50 + inclinare_proiectile[r] * 60 + viteze_proiectile[r] * (inclinare_proiectile[r] * 60 / 290) * 100);
					glVertex2f(250 + viteze_proiectile[r] * 100, OY_proiectile[r] - 50 + inclinare_proiectile[r] * 60 + viteze_proiectile[r] * (inclinare_proiectile[r] * 60 / 290) * 100);
					glEnd();
				}
			}
		}
	}
}

void draw_bow(float translateX, float translateY)
{
	glColor3f(0, 1, 1);
	glBegin(GL_LINES);
	glVertex2f(5 + translateX * 60 , 100 + translateY * 60);
	glVertex2f(5 + translateX * 60 , 500 + translateY * 60);
	glEnd();
	float radius = 200;
	float twoPI = 2 * 3.14159;

	glBegin(GL_POINTS);

	for (float i = 0.0; i <= twoPI / 2; i += 0.001)
		glVertex2f(5 + (sin(i) * radius) , 300 + (cos(i) * radius) + translateY * 60);

	glEnd();
	glFlush();
}

void draw_baloons(float angularStep)
{
	if (show_ballons == true)
	{
		// desenam baloanele galbene
		//for (int j = 3000; j >= 0; j = j - 300)
		for (int j = (numar_baloane_galbene - 1) * 300; j >= 0; j = j - 300)
		{
			// coliziune baloane galbene
			for (int r = 1; r < 100; r++)
			{
				if (OY_proiectile[r] + inclinare_proiectile[r] * 60 + viteze_proiectile[r] * (inclinare_proiectile[r] * 60 / 290) * 100 < (250 - j + 75 + angularStep * speed_baloane_galbene[j]) && OY_proiectile[r] + inclinare_proiectile[r] * 60 + viteze_proiectile[r] * (inclinare_proiectile[r] * 60 / 290) * 100 > (250 - j - 75 + angularStep * speed_baloane_galbene[j]) && OX_proiectile[r] <= 665 && OX_proiectile[r] >= 590)
				{
					dead_yellow_ballon = j;
					if (galben_vechi != j)
					{
						score = score + 100;
						galben_vechi = j;
					}
				}
			}
			glColor3f(1, 6, 0);
			glBegin(GL_POLYGON);
			for (int i = 0; i < 360; i++)
			{
				theta = i * 3.142 / 180;
				glVertex2f(640 + baloane_yellow_x[j] * cos(theta), 250 - j + baloane_yellow_y[j] * sin(theta) + angularStep * speed_baloane_galbene[j]);
			}
			glEnd();
			/*glBegin(GL_LINES);
			glVertex2f(640, 200 - j + angularStep * speed2[j]);
			glVertex2f(640, 125 - j + angularStep * speed2[j]);
			glEnd();*/
			glBegin(GL_LINES);
			glVertex2f(660, 200 - j + angularStep * speed2[j]);
			glVertex2f(620, 150 - j + angularStep * speed2[j]);
			glEnd();
			glBegin(GL_LINES);
			glVertex2f(620, 150 - j + angularStep * speed2[j]);
			glVertex2f(640, 140 - j + angularStep * speed2[j]);
			glEnd();
			glBegin(GL_LINES);
			glVertex2f(640, 140 - j + angularStep * speed2[j]);
			glVertex2f(620, 130 - j + angularStep * speed2[j]);
			glEnd();
			glBegin(GL_LINES);
			glVertex2f(620, 130 - j + angularStep * speed2[j]);
			glVertex2f(640, 110 - j + angularStep * speed2[j]);
			glEnd();
		}
		// desenam baloanele rosii
		for (int j = (numar_baloane_rosii - 1 ) * 300; j >= 0; j = j - 300)
		{
			for (int r = 1; r < 100; r++)
			{
				if (OY_proiectile[r] + inclinare_proiectile[r] * 60 + viteze_proiectile[r] * (inclinare_proiectile[r] * 60 / 290) * 100 < (500 - j + 75 + angularStep * speed_baloane_rosii[j]) && OY_proiectile[r] + inclinare_proiectile[r] * 60 + viteze_proiectile[r] * (inclinare_proiectile[r] * 60 / 290) * 100 > (500 - j - 75 + angularStep * speed_baloane_rosii[j]) && OX_proiectile[r] <= 525 && OX_proiectile[r] >= 450)
				{
					dead_red_ballon = j;
					if (rosu_vechi != j)
					{
						score = score - 100;
						rosu_vechi = j;
					}

				}
			}
			glColor3f(1, 0, 0);
			glBegin(GL_POLYGON);
			for (int i = 0; i < 360; i++)
			{
				theta = i * 3.142 / 180;
				glVertex2f(500 + baloane_red_x[j] * cos(theta), 500 - j + baloane_red_y[j] * sin(theta) + angularStep * speed_baloane_rosii[j]);
			}
			glEnd();
			glBegin(GL_LINES);
			glVertex2f(520, 450 - j + angularStep * speed3[j]);
			glVertex2f(480, 400 - j + angularStep * speed3[j]);
			glEnd();
			glBegin(GL_LINES);
			glVertex2f(480, 400 - j + angularStep * speed3[j]);
			glVertex2f(500, 390 - j + angularStep * speed3[j]);
			glEnd();
			glBegin(GL_LINES);
			glVertex2f(500, 390 - j + angularStep * speed3[j]);
			glVertex2f(480, 380 - j + angularStep * speed3[j]);
			glEnd();
			glBegin(GL_LINES);
			glVertex2f(480, 380 - j + angularStep * speed3[j]);
			glVertex2f(500, 360 - j + angularStep * speed3[j]);
			glEnd();
		}
		// desenam baloanele mov
		for (int j = (numar_baloane_mov - 1) * 300; j >= 0; j = j - 600)
		{
			for (int r = 1; r < 100; r++)
			{
				if (OY_proiectile[r] + inclinare_proiectile[r] * 60 + viteze_proiectile[r] * (inclinare_proiectile[r] * 60 / 290) * 100 < (500 - j + 75 + angularStep * speed_baloane_mov[j]) && OY_proiectile[r] + inclinare_proiectile[r] * 60 + viteze_proiectile[r] * (inclinare_proiectile[r] * 60 / 290) * 100 > (500 - j - 75 + angularStep * speed_baloane_mov[j]) && OX_proiectile[r] <= 925 && OX_proiectile[r] >= 850)
				{
					dead_mov_ballon = j;
					if (mov_vechi != j)
					{
						cout << "EXTRA LIFE!!!" << endl;
						life_count++;
						mov_vechi = j;
					}

				}
			}
			glColor3f(1, 0, 1);
			glBegin(GL_POLYGON);
			for (int i = 0; i < 360; i++)
			{
				theta = i * 3.142 / 180;
				glVertex2f(900 + baloane_mov_x[j] * cos(theta), 500 - j + baloane_mov_y[j] * sin(theta) + angularStep * speed_baloane_mov[j]);
			}
			glEnd();
			glBegin(GL_LINES);
			glVertex2f(920, 450 - j + angularStep * speed4[j]);
			glVertex2f(880, 400 - j + angularStep * speed4[j]);
			glEnd();
			glBegin(GL_LINES);
			glVertex2f(880, 400 - j + angularStep * speed4[j]);
			glVertex2f(900, 390 - j + angularStep * speed4[j]);
			glEnd();
			glBegin(GL_LINES);
			glVertex2f(900, 390 - j + angularStep * speed4[j]);
			glVertex2f(880, 380 - j + angularStep * speed4[j]);
			glEnd();
			glBegin(GL_LINES);
			glVertex2f(880, 380 - j + angularStep * speed4[j]);
			glVertex2f(900, 360 - j + angularStep * speed4[j]);
			glEnd();
		}
	}
}

void draw_shurikens(float angularStep,float translateY)
{
	if (show_shuriken == true)
	{
		//for (int i = 5000; i >= 0; i = i - 500)
		for (int i = 500 * numar_shurikenuri; i >= 0; i = i - 500)
		{

			OX_shuriken[1] = 1100 + i - angularStep * viteza_shurikenuri;
			OY_shuriken[1] = 360;
			for (int r = 1; r < 100; r++)
			{
				// tratare coliziuni shurikenuri de pe mid

				// ultima modificare la coliziuni inloc de 360+50 era 460 etc


				if (OY_proiectile[r] + inclinare_proiectile[r] * 60 + viteze_proiectile[r] * (inclinare_proiectile[r] * 60 / 290) * 100 < 360 + 50 && OY_proiectile[r] + inclinare_proiectile[r] * 60 + viteze_proiectile[r] * (inclinare_proiectile[r] * 60 / 290) * 100 > 360-50 && OX_proiectile[r] <= OX_shuriken[1] && OX_proiectile[r] >= OX_shuriken[1] - latura_shuriken && dead_arrows[r] != 1 && dead_shuriken_ids_mid[i] != i)
				{
					OK = 1;
					dead_shuriken_id_mid = i;
					dead_shuriken_ids_mid[i] = i;
					dead_arrows[r] = 1;
				}
				// tratare coliziuni shurikenuri de pe top
				if (OY_proiectile[r] + inclinare_proiectile[r] * 60 + viteze_proiectile[r] * (inclinare_proiectile[r] * 60 / 290) * 100 < 560+50 && OY_proiectile[r] + inclinare_proiectile[r] * 60 + viteze_proiectile[r] * (inclinare_proiectile[r] * 60 / 290) * 100 > 560 -50 && OX_proiectile[r] <= OX_shuriken[1] && OX_proiectile[r] >= OX_shuriken[1] - latura_shuriken && dead_arrows[r] != 1 && dead_shuriken_ids_top[i] != i)
				{
					OK2 = 1;
					dead_shuriken_id_top = i;
					dead_shuriken_ids_top[i] = i;
					dead_arrows[r] = 1;
				}
				// tratare coliziuni shurikenuri de pe bot
				if (OY_proiectile[r] + inclinare_proiectile[r] * 60 + viteze_proiectile[r] * (inclinare_proiectile[r] * 60 / 290) * 100 < 160 + 50 && OY_proiectile[r] + inclinare_proiectile[r] * 60 + viteze_proiectile[r] * (inclinare_proiectile[r] * 60 / 290) * 100 > 160-50 && OX_proiectile[r] <= OX_shuriken[1] && OX_proiectile[r] >= OX_shuriken[1] - latura_shuriken && dead_arrows[r] != 1 && dead_shuriken_ids_bot[i] != i)
				{
					OK3 = 1;
					dead_shuriken_id_bot = i;
					dead_shuriken_ids_bot[i] = i;
					dead_arrows[r] = 1;
				}
				// daca arcul este atins de un shuriken de pe mid
				if (OX_shuriken[1] - laturi_shuriken_mid[r] > 5 && OX_shuriken[1] - laturi_shuriken_mid[r] < 205 && 360 + 50 > 100 + translateY * 60 && 360 - 50 < 500 + translateY * 60 && dead_shuriken_ids_mid[i] != i)
				{
					//exit(0);
					if (shuriken_mid_vechi != i)
					{
						life_count--;
						shuriken_mid_vechi = i;
					}
				}
				// daca arcul este atins de un shuriken de pe bot
				if (OX_shuriken[1] - laturi_shuriken_mid[r] > 5 && OX_shuriken[1] - laturi_shuriken_mid[r] < 205 && 210 > 100 + translateY * 60 && 110 < 500 + translateY * 60 && dead_shuriken_ids_bot[i] != i && latura_shuriken3 != 0)
				{
					//exit(0);
					if (shuriken_bot_vechi != i)
					{
						life_count--;
						shuriken_bot_vechi = i;
					}
				}
				// daca arcul este atins de un shuriken de pe top
				if (OX_shuriken[1] - laturi_shuriken_mid[r] > 5 && OX_shuriken[1] - laturi_shuriken_mid[r] < 205 && 610 > 100 + translateY * 60 && 510 < 500 + translateY * 60 && dead_shuriken_ids_top[i] != i && latura_shuriken2 != 0)
				{
					//exit(0);
					if (shuriken_top_vechi != i)
					{
						life_count--;
						shuriken_top_vechi = i;
					}
				}
			}

			// desenez pe ecran shurikenurile de pe mid
			glBegin(GL_TRIANGLES);
			glColor3f(0, 0.5f, 0);
			glVertex2f(1100 + i - angularStep * viteza_shurikenuri, 360);
			glVertex2f(1100 + i - angularStep * viteza_shurikenuri, 360 + laturi_shuriken_mid[i]);
			glVertex2f(1100 - laturi_shuriken_mid[i] + i - angularStep * viteza_shurikenuri, 360 + laturi_shuriken_mid[i]);
			glVertex2f(1100 + i - angularStep * viteza_shurikenuri, 360);
			glVertex2f(1100 + laturi_shuriken_mid[i] + i - angularStep * viteza_shurikenuri, 360 + laturi_shuriken_mid[i]);
			glVertex2f(1100 + laturi_shuriken_mid[i] + i - angularStep * viteza_shurikenuri, 360);
			glVertex2f(1100 + i - angularStep * viteza_shurikenuri, 360);
			glVertex2f(1100 - laturi_shuriken_mid[i] + i - angularStep * viteza_shurikenuri, 360);
			glVertex2f(1100 - laturi_shuriken_mid[i] + i - angularStep * viteza_shurikenuri, 360 - laturi_shuriken_mid[i]);
			glVertex2f(1100 + i - angularStep * viteza_shurikenuri, 360);
			glVertex2f(1100 + i - angularStep * viteza_shurikenuri, 360 - laturi_shuriken_mid[i]);
			glVertex2f(1100 + laturi_shuriken_mid[i] + i - angularStep * viteza_shurikenuri, 360 - laturi_shuriken_mid[i]);
			glEnd();

			// desenez pe ecran shurikenurile de pe top
			glBegin(GL_TRIANGLES);
			glColor3f(0, 0.5f, 0);
			glVertex2f(1100 + i - angularStep * viteza_shurikenuri, 560);
			glVertex2f(1100 + i - angularStep * viteza_shurikenuri, 560 + laturi_shuriken_top[i]);
			glVertex2f(1100 - laturi_shuriken_top[i] + i - angularStep * viteza_shurikenuri, 560 + laturi_shuriken_top[i]);
			glVertex2f(1100 + i - angularStep * viteza_shurikenuri, 560);
			glVertex2f(1100 + laturi_shuriken_top[i] + i - angularStep * viteza_shurikenuri, 560 + laturi_shuriken_top[i]);
			glVertex2f(1100 + laturi_shuriken_top[i] + i - angularStep * viteza_shurikenuri, 560);
			glVertex2f(1100 + i - angularStep * viteza_shurikenuri, 560);
			glVertex2f(1100 - laturi_shuriken_top[i] + i - angularStep * viteza_shurikenuri, 560);
			glVertex2f(1100 - laturi_shuriken_top[i] + i - angularStep * viteza_shurikenuri, 560 - laturi_shuriken_top[i]);
			glVertex2f(1100 + i - angularStep * viteza_shurikenuri, 560);
			glVertex2f(1100 + i - angularStep * viteza_shurikenuri, 560 - laturi_shuriken_top[i]);
			glVertex2f(1100 + laturi_shuriken_top[i] + i - angularStep * viteza_shurikenuri, 560 - laturi_shuriken_top[i]);
			glEnd();

			// desenez pe ecran shurikenurile de pe bot
			glBegin(GL_TRIANGLES);
			glColor3f(0, 0.5f, 0);
			glVertex2f(1100 + i - angularStep * viteza_shurikenuri, 160);
			glVertex2f(1100 + i - angularStep * viteza_shurikenuri, 160 + laturi_shuriken_bot[i]);
			glVertex2f(1100 - laturi_shuriken_bot[i] + i - angularStep * viteza_shurikenuri, 160 + laturi_shuriken_bot[i]);
			glVertex2f(1100 + i - angularStep * viteza_shurikenuri, 160);
			glVertex2f(1100 + laturi_shuriken_bot[i] + i - angularStep * viteza_shurikenuri, 160 + laturi_shuriken_bot[i]);
			glVertex2f(1100 + laturi_shuriken_bot[i] + i - angularStep * viteza_shurikenuri, 160);
			glVertex2f(1100 + i - angularStep * viteza_shurikenuri, 160);
			glVertex2f(1100 - laturi_shuriken_bot[i] + i - angularStep * viteza_shurikenuri, 160);
			glVertex2f(1100 - laturi_shuriken_bot[i] + i - angularStep * viteza_shurikenuri, 160 - laturi_shuriken_bot[i]);
			glVertex2f(1100 + i - angularStep * viteza_shurikenuri, 160);
			glVertex2f(1100 + i - angularStep * viteza_shurikenuri, 160 - laturi_shuriken_bot[i]);
			glVertex2f(1100 + laturi_shuriken_bot[i] + i - angularStep * viteza_shurikenuri, 160 - laturi_shuriken_bot[i]);
			glEnd();
		}
		if (laturi_shuriken_mid[dead_shuriken_id_mid] > 0 && OK == 1)
			laturi_shuriken_mid[dead_shuriken_id_mid]--;
		if (laturi_shuriken_top[dead_shuriken_id_top] > 0 && OK2 == 1)
			laturi_shuriken_top[dead_shuriken_id_top]--;
		if (laturi_shuriken_bot[dead_shuriken_id_bot] > 0 && OK3 == 1)
			laturi_shuriken_bot[dead_shuriken_id_bot]--;
	}
}

void print_score()
{
	// daca scorul s-a schimbat il afisam, facem aceasta verificare pentru a nu face spam in consola la fiecare apelare a functiei update
	if (previous_score != score & semafor_game_over != true)
		cout << "SCORE: " << score << "\n";
}

void print_current_life()
{
	// daca numarul de vieti ramase s-a schimbat il afisam, facem aceasta verificare pentru a nu face spam in consola la fiecare apelare a functiei update
	if (life_count != life_count_prev & semafor_game_over != true)
		cout << "Life count: " << life_count << endl;
}

void check_game_over()
{
	// daca numarul de vieti este mai mic sau egal cu 0 oprim jocul si afisam mesajul clasic "Game over"
	if (life_count <= 0 & semafor_game_over != true)
	{
		cout << "GAME OVER!" << endl;
		semafor_game_over = true;
		//exit(0);
	}
}

void draw_personaj(GLint x0, GLint y0, int latura_patrat, float angularStep,float deltaTimeSeconds)
{
	if (draw_personaj_status == true)
	{

		if (copie_angular_2 <= 0)
			semafor = false;
		if (copie_angular_2 >= 10)
			semafor = true;
		if (semafor == false)
		{
			copie_angular_2 += deltaTimeSeconds;
			y0 = y0 + copie_angular_2 * 60;
		}
		if (semafor == true)
		{
			copie_angular_2 -= deltaTimeSeconds;
			y0 = y0 + copie_angular_2 * 60;;
		}

		GLint x1 = x0;
		GLint y1 = y0 + latura_patrat;
		GLint x2 = x0 + latura_patrat;
		GLint y2 = y0 + latura_patrat;
		GLint x3 = x0 + latura_patrat;
		GLint y3 = y0;
		if (happy_contor % 4 == 1)
		{
			glColor3f(0.5, 1.0, 0.0);
		}
		else if (happy_contor % 4 == 2)
		{
			glColor3f(1.0, 1.0, 0.0);
		}
		else if (happy_contor % 4 == 2)
		{
			glColor3f(1.0, 0.0, 0.0);
		}
		else
		{
			glColor3f(1.0, 0.0, 0.0);
		}
		glBegin(GL_LINE_LOOP);
		glVertex2i(x0, y0);
		glVertex2i(x1, y1);
		glVertex2i(x2, y2);
		glVertex2i(x3, y3);
		glEnd();
		glFlush();
		float radius = 10;
		float twoPI = 2 * 3.14159;
		
		if (happy_contor % 4 == 0)
		{
			glBegin(GL_LINES);

			glVertex2f(x0 + latura_patrat / 8, y0 + latura_patrat - latura_patrat / 8);
			glVertex2f(x0 + 3 * latura_patrat / 8, y0 + latura_patrat - 3 * latura_patrat / 8);
			glVertex2f(x0 + 3 * latura_patrat / 8, y0 + latura_patrat - latura_patrat / 8);
			glVertex2f(x0 + latura_patrat / 8, y0 + latura_patrat - 3 * latura_patrat / 8);

			glVertex2f(x0 + latura_patrat / 8 + latura_patrat / 2, y0 + latura_patrat - latura_patrat / 8);
			glVertex2f(x0 + 3 * latura_patrat / 8 + latura_patrat / 2, y0 + latura_patrat - 3 * latura_patrat / 8);
			glVertex2f(x0 + 3 * latura_patrat / 8 + latura_patrat / 2, y0 + latura_patrat - latura_patrat / 8);
			glVertex2f(x0 + latura_patrat / 8 + latura_patrat / 2, y0 + latura_patrat - 3 * latura_patrat / 8);

			glEnd();
		}
		else
		{
			glBegin(GL_POINTS);

			for (float i = 0.0; i <= twoPI; i += 0.001)
				glVertex2f(x0 + latura_patrat / 4 + (sin(i) * radius), y0 + 3 * latura_patrat / 4 + (cos(i) * radius));

			glEnd();
			glFlush();

			glBegin(GL_POINTS);

			for (float i = 0.0; i <= twoPI; i += 0.001)
				glVertex2f(x0 + 3 * latura_patrat / 4 + (sin(i) * radius), y0 + 3 * latura_patrat / 4 + (cos(i) * radius));

			glEnd();
			glFlush();
		}

		for (int r = 1; r < 100; r++)

		{

			if (OY_proiectile[r] + inclinare_proiectile[r] * 60 + viteze_proiectile[r] * (inclinare_proiectile[r] * 60 / 290) * 100 < y1 && OY_proiectile[r] + inclinare_proiectile[r] * 60 + viteze_proiectile[r] * (inclinare_proiectile[r] * 60 / 290) * 100 > y0 && OX_proiectile[r] <= x3 && OX_proiectile[r] >= x0)
			{
				if (dead_personaj_arrows[r] != 1)
				{
					happy_contor++;
					dead_personaj_arrows[r] = 1;
				}
			}
		}
		if (happy_contor % 4 == 1)
		{
			// happy face
			glBegin(GL_LINES);
			glVertex2f(x0 + latura_patrat / 2 - 20, y0 + latura_patrat / 2);
			glVertex2f(x0 + latura_patrat - (latura_patrat / 2 - 20), y0 + latura_patrat / 2);
			glEnd();
			glFlush();
			glBegin(GL_POINTS);

			for (float i = twoPI / 4; i <= 3 * twoPI / 4; i += 0.001)
				glVertex2f(x0 + latura_patrat / 2 + (sin(i) * 20), y0 + latura_patrat / 2 + (cos(i) * 20));
			glEnd();
			glFlush();
		}
		if (happy_contor % 4 == 2)
		{
			// sad face
			glBegin(GL_LINES);
			glVertex2f(x0 + latura_patrat / 2 - 20, y0 + latura_patrat / 2 - 20);
			glVertex2f(x0 + latura_patrat - (latura_patrat / 2 - 20), y0 + latura_patrat / 2 - 20);
			glEnd();
			glFlush();
			glBegin(GL_POINTS);

			for (float i = twoPI * 3 / 4; i <= twoPI * 5 / 4; i += 0.001)
				glVertex2f(x0 + latura_patrat / 2 + (sin(i) * 20), y0 - 20 + latura_patrat / 2 + (cos(i) * 20));
			glEnd();
			glFlush();
		}
		if (happy_contor % 4 == 3)
		{
			// no expression
			glBegin(GL_LINES);
			glVertex2f(x0 + latura_patrat / 2 - 20, y0 + latura_patrat / 2 - 20);
			glVertex2f(x0 + latura_patrat - (latura_patrat / 2 - 20), y0 + latura_patrat / 2 - 20);
			glEnd();
			glFlush();
		}
		if (happy_contor % 4 == 0)
		{
			// no expression
			glBegin(GL_LINES);
			glVertex2f(x0 + latura_patrat / 2 - 20, y0 + latura_patrat / 2 - 20);
			glVertex2f(x0 + latura_patrat - (latura_patrat / 2 - 20), y0 + latura_patrat / 2 - 20);
			glEnd();
			glFlush();
		}
	}
}


void draw_game_over()
{
		glBegin(GL_LINES);
		glColor3f(0, 0, 0);
		glVertex2f(550, 500);
		glVertex2f(500, 500);
		glVertex2f(500, 500 );
		glVertex2f(500, 400);
		glVertex2f(500, 400);
		glVertex2f(550, 400 );
		glVertex2f(550, 400 );
		glVertex2f(550, 445 );
		glVertex2f(550, 445 );
		glVertex2f(515, 445 );

		glVertex2f(550 + 100, 500 );
		glVertex2f(500 + 100, 500 );
		glVertex2f(500 + 100, 500 );
		glVertex2f(500 + 100, 400 );
		glVertex2f(500 + 100, 400 );
		glVertex2f(550 + 100, 400 );
		glVertex2f(550 + 100, 400 );
		glVertex2f(550 + 100, 445 );
		glVertex2f(550 + 100, 445);
		glVertex2f(515 + 100, 445 );

		glVertex2f(650 + 100, 500);
		glVertex2f(650 + 50, 450);
		glVertex2f(650 + 50, 450);
		glVertex2f(650 + 100, 400);

		glVertex2f(750 + 100 - 75, 500);
		glVertex2f(750 + 150 - 75, 500);
		glVertex2f(750 + 150 - 75, 500);
		glVertex2f(750 + 150 - 75, 450);
		glVertex2f(750 + 150 - 75, 450);
		glVertex2f(750 + 100 - 75, 450);
		glVertex2f(750 + 150 - 75, 450);
		glVertex2f(750 + 150 - 75, 400);
		glVertex2f(750 + 150 - 75, 400);
		glVertex2f(750 + 100 - 75, 400);
		glEnd();
		glBegin(GL_QUADS);
		glColor3f(1.0, 1.0, 1.0);
		glVertex2f(0, 0);
		glVertex2f(1280, 0);
		glVertex2f(1280, 820);
		glVertex2f(0, 820);
		glEnd();
}

void Tema1::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
	// TODO: update steps for translation, rotation, scale, in order to create animations
	angularStep += deltaTimeSeconds;
	previous_score = score;
	life_count_prev = life_count;
	if (semafor_game_over == true)
	{
		draw_game_over();
	}

	draw_crosshair(inclinare_tinta, translateY);

	draw_arrow(deltaTimeSeconds, translateY);

	draw_personaj(1100, 100, 100, angularStep, deltaTimeSeconds);

	modelMatrix = glm::mat3(1);
	// ultima modficiare
	modelMatrix *= Transform2D::Translate(0, -100);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	draw_bow(translateX, translateY);

	draw_baloons(angularStep);

	draw_shurikens(angularStep, translateY);

	if (dead_yellow_ballon != 0)
	{
		if(baloane_yellow_x[dead_yellow_ballon] > 0)
			baloane_yellow_x[dead_yellow_ballon]--;
		if(baloane_yellow_y[dead_yellow_ballon] > 0)
			baloane_yellow_y[dead_yellow_ballon]--;
		speed2[dead_yellow_ballon]--;
	}

	if (dead_red_ballon != 0)
	{
		if (baloane_red_x[dead_red_ballon] > 0)
			baloane_red_x[dead_red_ballon]--;
		if (baloane_red_y[dead_red_ballon] > 0)
			baloane_red_y[dead_red_ballon]--;
		speed3[dead_red_ballon]--;
	}

	if (dead_mov_ballon != 0)
	{
		if (baloane_mov_x[dead_mov_ballon] > 0)
			baloane_mov_x[dead_mov_ballon]--;
		if (baloane_mov_y[dead_mov_ballon] > 0)
			baloane_mov_y[dead_mov_ballon]--;
		speed4[dead_mov_ballon]--;
	}

	print_score();

	print_current_life();

	check_game_over();

}

void Tema1::FrameEnd()
{

}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	// daca apasam continuu tasta W arcul se va misca pe axa OY in sus
	if (window->KeyHold(GLFW_KEY_W) == 1)
	{
		translateY = translateY + deltaTime;
	}
	// daca apasam continuu tasta W arcul se va misca pe axa OY in jos
	if (window->KeyHold(GLFW_KEY_S) == 1)
	{
		translateY = translateY - deltaTime;
	}
	// daca apasam continuu sageata in sus putem schimba inclinarea sagetii
	if (window->KeyHold(GLFW_KEY_UP) == 1)
	{
		inclinare_tinta = inclinare_tinta + deltaTime;
	}
	// daca apasam continuu sageata in jos putem schimba inclinarea sagetii
	if (window->KeyHold(GLFW_KEY_DOWN) == 1)
	{
		inclinare_tinta = inclinare_tinta - deltaTime;
	}
}

void Tema1::OnKeyPress(int key, int mods)
{
	// add key press event
	// daca apasam sageata dreapta tragem cu arcul
	if (key == GLFW_KEY_RIGHT)
	{
		show_arrow = true;
		counter++;
		copie_angular = 0.001;
	}
	// daca apasam tasta Q iesim din joc
	if (key == GLFW_KEY_Q)
	{
		exit(0);
	}
	// daca apasam tasta B afisam baloanele
	if (key == GLFW_KEY_B)
	{
		if (show_ballons == false)
			show_ballons = true;
		else
			show_ballons = false;
	}
	// daca apasam tasta K afisam shurikenurile
	if (key == GLFW_KEY_K)
	{
		if (show_shuriken == false)
			show_shuriken = true;
		else
			show_shuriken = false;
	}
	// vitezele sagetii
	if (key == GLFW_KEY_1)
	{
		arrow_speed = 1;
	}
	if (key == GLFW_KEY_2)
	{
		arrow_speed = 2;
	}
	if (key == GLFW_KEY_3)
	{
		arrow_speed = 3;
	}
	if (key == GLFW_KEY_P)
	{
		draw_personaj_status = true;
	}
}

void Tema1::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event

	// daca apasam click stanga tragem cu arcul
	if (button == GLFW_MOUSE_BUTTON_2)
	{
		show_arrow = true;
		counter++;
		copie_angular = 0.001;
	}

}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}
