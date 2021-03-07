#include "Tema1.h"
///#include "Laborator2.h"

#include <vector>
#include <iostream>
#include <math.h>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"
//#include <Component/SimpleScene.cpp>

using namespace std;

// GAME SETTINGS

int show_shuriken = true;		// variabila care ne spune daca dorim sa afisam sau nu shurikenurile
int show_ballons = true;	// variabila care ne spune daca dorim sa afisam sau nu baloanele
bool draw_personaj_status = true; // variabila care ne spune daca dorim sa afisam sau nu personajul
char difficulty[10] = "easy";	// putem alege dintre easy,medium si hard


int numar_baloane_rosii = 20;	// numarul de baloane rosii care se spawneaza (setam -1 daca vrem sa nu apara niciunul)
int numar_baloane_galbene = 20;	// numarul de baloane galbene care se spawneaza (setam -1 daca vrem sa nu apara niciunul)
int numar_baloane_mov = 5;	// numarul de baloane mov care se spawneaza (setam -1 daca vrem sa nu apara niciunul)
int numar_shurikenuri = 20;	// numarul de shurikenuri care apar
int counter = 0;

// variabile globale arc + sageata
float viteze_proiectile[100];		// vector in care stocam viteza fiecarei sageti
float OY_proiectile[100] = { 0 };	// vector in care stocam pozitia pe axa OY a fiecarei sageti
float OX_proiectile[100] = { 0 };   // vector in care stocam pozitia pe axa OX a fiecarei sageti
float inclinare_tinta = 0;	// variabila in care stocam inclinarea unei sageti fata de axa OX, practic panta
int dead_arrows[101] = { 0 };	// vector in care stocam care sageata a fost distrusa
int fix_orientation[101] = { 0 };	// vector in care stocam daca o sageata a fost "trasa" sau nu, astfel daca vrem sa tragem o sageata in sus si dupa una in jos sa nu se modifice inclinarea unei sageti in functie de alta
float inclinare_proiectile[101] = { 0 };
float theta;	// unghiul arcului care face parte din bow
int show_arrow = false;		// variabila care ne spune cand afisam sageata dupa ce tragem

// variabile globale shurikenuri
int viteza_shurikenuri = 50;
float OX_shuriken[100] = { 0 };		// vector in care stocam pozitia pe axa OX a fiecarui shuriken
float OY_shuriken[100] = { 0 };		// vector in care stocam pozitia pe axa OY a fiecarui shuriken
float latura_shuriken = 50;	// cat are latura un shuriken de pe mid
float latura_shuriken2 = 50; // cat are latura un shuriken de pe top
float latura_shuriken3 = 50; // cat are latura un shuriken de pe bot
int OK = 0;		// semafor care ne spune daca un shuriken de pe mid a fost lovit de o sageata
int OK2 = 0;	// semafor care ne spune daca un shuriken de pe top a fost lovit de o sageata
int OK3 = 0;	// semafor care ne spune daca un shuriken de pe bot a fost lovit de o sageata
vector <float> laturi_shuriken_mid(500 * numar_shurikenuri + 1); // vector in care stocam latura fiecarui shuriken de pe mid
vector <float> laturi_shuriken_top(500 * numar_shurikenuri + 1); // vector in care stocam latura fiecarui shuriken de pe top
vector <float> laturi_shuriken_bot(500 * numar_shurikenuri + 1); // vector in care stocam latura fiecarui shuriken de pe bot
vector <float> dead_shuriken_ids_mid(500 * numar_shurikenuri + 1); // vector in care stocam daca un shuriken de pe mid este distrus sau nu
vector <float> dead_shuriken_ids_bot(500 * numar_shurikenuri + 1); // vector in care stocam daca un shuriken de pe bot este distrus sau nu
vector <float> dead_shuriken_ids_top(500 * numar_shurikenuri + 1); // vector in care stocam daca un shuriken de pe top este distrus sau nu
int dead_shuriken_id_mid = 0;	// pastram id-ul shurikenul care a fost distrus de pe mid
int dead_shuriken_id_top = 0;	// pastram id-ul shurikenul care a fost distrus de pe top
int dead_shuriken_id_bot = 0;	// pastram id-ul shurikenul care a fost distrus de pe bot
int shuriken_mid_vechi = -1;	// id-ul shurikenului mid care a fost distrus anterior
int shuriken_top_vechi = -1;	// id-ul shurikenului top care a fost distrus anterior
int shuriken_bot_vechi = -1;	// id-ul shurikenului bot care a fost distrus anterior

// variabile globale baloane
vector<float> baloane_red_x(numar_baloane_rosii * 300 + 1);	// coordonata pe OX a unui balon rosu
vector<float> baloane_red_y(numar_baloane_rosii * 300 + 1); // coordonata pe OY a unui balon rosu
vector<float> baloane_yellow_x(numar_baloane_galbene * 300 + 1); // coordonata pe OX a unui balon galben
vector<float> baloane_yellow_y(numar_baloane_galbene * 300 + 1); // coordonata pe OY a unui balon galben
vector<float> baloane_mov_x(numar_baloane_mov * 600 + 1); // coordonata pe OX a unui balon mov
vector<float> baloane_mov_y(numar_baloane_mov * 600 + 1); // coordonata pe OY a unui balon mov
vector <float> speed_baloane_rosii(numar_baloane_rosii * 300 + 1); // viteza unui balon rosu
vector <float> speed_baloane_galbene(numar_baloane_galbene * 300 + 1); // viteza unui balon galben
vector <float> speed_baloane_mov(numar_baloane_mov * 600 + 1); // viteza unui balon mov
vector <float> speed2(numar_baloane_rosii * 300 + 1);	// viteza cu care se misca linia de la baloanele galbene
vector <float> speed3(numar_baloane_galbene * 300 + 1);	// viteza cu care se misca linia de la baloanele rosii
vector <float> speed4(numar_baloane_mov * 600 + 1); // viteza cu care se misca linia de la baloanele mov
int dead_red_ballon = 0;	// pastram id-ul balonului rosu care a fost distrus anterior
int dead_yellow_ballon = 0;	// pastram id-ul balonului galben care a fost distrus anterior
int dead_mov_ballon = 0;	// pastram id-ul balonului mov care a fost distrus anterior
int rosu_vechi = -1;	// id-ul balonului rosu care a fost distrus anterior
int galben_vechi = -1;	// id-ul balonului galben care a fost distrus anterior
int mov_vechi = -1; // id-ul balonului mov care a fost distrus anterior

// variabile globale status joc
float arrow_speed = 1;
int previous_score = 0;	// scorul anterior
int score = 0;	// scorul curent
int life_count_prev = 3;	// numarul de vieti anterior
int life_count = 3;	// numarul de vieti curent

// data personaj
int dead_personaj_arrows[101] = { 0 };
int happy_contor = 1; // variabila prin care ne dam seama ce expresie faciala are personajul nostru
int speed_personaj = 60; // viteza cu care se deplaseaza personajul
int semafor = false; // variabila care ne ajuta sa tinem personajul in aria vizibila pe ecran
float copie_angular_2 = 0;
bool semafor_game_over = false; // variabila care ne spune cand s-a terminat jocul pentru a nu spama in consola mesajele cu scor si "GAME OVER!"

float copie_angular = 0;		// de eliminat
int counter_game_over = 200;

float viteza_s = 2;	// variabila cu care crestem viteza sagetii
float viteza_sageata[101] = { 0 };	// vector in care stocam viteza fiecarei sageti
int indice_sageti = 0;	
float latura_powerbar = 0; // lungimea powerbarului
int contor_cercuri = -1; 
float mouse_l1 = 0;	// variabila utilizata pentru a afla tangenta
float mouse_l2 = 0; // variabila utilizata pentru a afla tangenta

// variabile utilizate pentru a calcula coliziunea dintre o elipsa si punct, dintr-o elipsa si alta elipsa, etc
float raza_elipsa_ox; 
float raza_elipsa_oy;
float origine_balon_x;
float origine_balon_y;
float OY_Punct; 
float OX_Punct; 
int latura_powerbar_boss = 100;
bool show_bonus_crosshair = false;


Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}


void Tema1::CreateMeshes()
{
	Mesh* triunghi = new Mesh("triunghi");
	vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(0,1,0), glm::vec3(0,1,1)),
		VertexFormat(glm::vec3(1,1,0), glm::vec3(0,1,1)),
		VertexFormat(glm::vec3(1,0,0), glm::vec3(0,1,1))
	};
	vector <unsigned short> indices =
	{
		0,1,2
	};
	triunghi->SetDrawMode(GL_TRIANGLES);
	triunghi->InitFromData(vertices, indices);
	AddMeshToList(triunghi);

	Mesh* triunghi2 = new Mesh("triunghi2");
	vector<VertexFormat> vertices2
	{
		VertexFormat(glm::vec3(0,0,0), glm::vec3(0,1,1)),
		VertexFormat(glm::vec3(1,1,0), glm::vec3(0,1,1)),
		VertexFormat(glm::vec3(1,0,0), glm::vec3(0,1,1))
	};
	vector <unsigned short> indices2 =
	{
		0,1,2
	};
	triunghi2->SetDrawMode(GL_TRIANGLES);
	triunghi2->InitFromData(vertices2, indices2);
	AddMeshToList(triunghi2);

	Mesh* triunghi3 = new Mesh("triunghi3");
	vector<VertexFormat> vertices3
	{
		VertexFormat(glm::vec3(0,0,0), glm::vec3(0,1,1)),
		VertexFormat(glm::vec3(0,1,0), glm::vec3(0,1,1)),
		VertexFormat(glm::vec3(1,1,0), glm::vec3(0,1,1))
	};
	vector <unsigned short> indices3 =
	{
		0,1,2
	};
	triunghi3->SetDrawMode(GL_TRIANGLES);
	triunghi3->InitFromData(vertices3, indices3);
	AddMeshToList(triunghi3);

	Mesh* triunghi4 = new Mesh("triunghi4");
	vector<VertexFormat> vertices4
	{
		VertexFormat(glm::vec3(0,1,0), glm::vec3(0,1,1)),
		VertexFormat(glm::vec3(0,0,0), glm::vec3(0,1,1)),
		VertexFormat(glm::vec3(1,0,0), glm::vec3(0,1,1))
	};
	vector <unsigned short> indices4 =
	{
		0,1,2
	};
	triunghi4->SetDrawMode(GL_TRIANGLES);
	triunghi4->InitFromData(vertices4, indices4);
	AddMeshToList(triunghi4);

	Mesh* linie1 = new Mesh("linie1");
	vector<VertexFormat> vertices5
	{
		VertexFormat(glm::vec3(0,0,0), glm::vec3(1,1,1)),
		VertexFormat(glm::vec3(1,1,0), glm::vec3(1,1,1)),
	};
	vector <unsigned short> indices5 =
	{
		0,1
	};
	linie1->SetDrawMode(GL_LINES);
	linie1->InitFromData(vertices5, indices5);
	AddMeshToList(linie1);

	Mesh* powerbar = new Mesh("powerbar");
	vector<VertexFormat> vertices6
	{
		VertexFormat(glm::vec3(0,0,0), glm::vec3(1,1,1)),
		VertexFormat(glm::vec3(0,1,0), glm::vec3(1,1,1)),
		VertexFormat(glm::vec3(1,1,0), glm::vec3(1,1,1)),
		VertexFormat(glm::vec3(1,0,0), glm::vec3(1,1,1)),
	};
	vector <unsigned short> indices6 =
	{
		0,1,2,3
	};
	powerbar->SetDrawMode(GL_QUADS);
	powerbar->InitFromData(vertices6, indices6);
	AddMeshToList(powerbar);

	Mesh* varf_sageata = new Mesh("varf_sageata");
	vector<VertexFormat> vertices7
	{
		VertexFormat(glm::vec3(0.5f,0.5f,0), glm::vec3(1,1,1)),
		VertexFormat(glm::vec3(0,1,0), glm::vec3(1,1,1)),
		VertexFormat(glm::vec3(0,0,0), glm::vec3(1,1,1)),
	};
	vector <unsigned short> indices7 =
	{
		0,1,2
	};
	varf_sageata->SetDrawMode(GL_TRIANGLES);
	varf_sageata->InitFromData(vertices7, indices7);
	AddMeshToList(varf_sageata);

	Mesh* linie_arc = new Mesh("linie_arc");
	vector<VertexFormat> vertices8
	{
		VertexFormat(glm::vec3(0,0,0), glm::vec3(1,1,1)),
		VertexFormat(glm::vec3(0,1,0), glm::vec3(1,1,1)),
	};
	vector <unsigned short> indices8 =
	{
		0,1
	};
	linie_arc->SetDrawMode(GL_LINES);
	linie_arc->InitFromData(vertices8, indices8);
	AddMeshToList(linie_arc);

	Mesh* game_over_screen = new Mesh("game_over_screen");
	vector<VertexFormat> vertices9
	{
		VertexFormat(glm::vec3(0,0,0), glm::vec3(1,1,1)),
		VertexFormat(glm::vec3(0,1,0), glm::vec3(1,1,1)),
		VertexFormat(glm::vec3(1,1,0), glm::vec3(1,1,1)),
		VertexFormat(glm::vec3(1,0,0), glm::vec3(1,1,1)),
	};
	vector <unsigned short> indices9 =
	{
		0,1,2,3
	};
	game_over_screen->SetDrawMode(GL_QUADS);
	game_over_screen->InitFromData(vertices9, indices9);
	AddMeshToList(game_over_screen);

	Mesh* g_letter = new Mesh("g_letter");
	vector<VertexFormat> vertices10
	{
		VertexFormat(glm::vec3(550,500,0), glm::vec3(0,0,0)),
		VertexFormat(glm::vec3(500,500,0), glm::vec3(0,0,0)),
		VertexFormat(glm::vec3(500,500,0), glm::vec3(0,0,0)),
		VertexFormat(glm::vec3(500,400,0), glm::vec3(0,0,0)),
		VertexFormat(glm::vec3(500,400,0), glm::vec3(0,0,0)),
		VertexFormat(glm::vec3(550,400,0), glm::vec3(0,0,0)),
		VertexFormat(glm::vec3(550,400,0), glm::vec3(0,0,0)),
		VertexFormat(glm::vec3(550,445,0), glm::vec3(0,0,0)),
		VertexFormat(glm::vec3(550,445,0), glm::vec3(0,0,0)),
		VertexFormat(glm::vec3(515,445,0), glm::vec3(0,0,0)),
	};
	vector <unsigned short> indices10 =
	{
		0,1,2,3,4,5,6,7,8,9
	};
	g_letter->SetDrawMode(GL_LINES);
	g_letter->InitFromData(vertices10, indices10);
	AddMeshToList(g_letter);

	Mesh* lower_key = new Mesh("lower_key");
	vector<VertexFormat> vertices11
	{
		VertexFormat(glm::vec3(750,500,0), glm::vec3(0,0,0)),
		VertexFormat(glm::vec3(700,450,0), glm::vec3(0,0,0)),
		VertexFormat(glm::vec3(700,450,0), glm::vec3(0,0,0)),
		VertexFormat(glm::vec3(750,400,0), glm::vec3(0,0,0)),
	};
	vector <unsigned short> indices11 =
	{
		0,1,2,3
	};
	lower_key->SetDrawMode(GL_LINES);
	lower_key->InitFromData(vertices11, indices11);
	AddMeshToList(lower_key);

	Mesh* three_key = new Mesh("three_key");
	vector<VertexFormat> vertices12
	{
		VertexFormat(glm::vec3(775,500,0), glm::vec3(0,0,0)),
		VertexFormat(glm::vec3(825,500,0), glm::vec3(0,0,0)),
		VertexFormat(glm::vec3(825,500,0), glm::vec3(0,0,0)),
		VertexFormat(glm::vec3(825,450,0), glm::vec3(0,0,0)),
		VertexFormat(glm::vec3(825,450,0), glm::vec3(0,0,0)),
		VertexFormat(glm::vec3(775,450,0), glm::vec3(0,0,0)),
		VertexFormat(glm::vec3(825,450,0), glm::vec3(0,0,0)),
		VertexFormat(glm::vec3(825,400,0), glm::vec3(0,0,0)),
		VertexFormat(glm::vec3(825,400,0), glm::vec3(0,0,0)),
		VertexFormat(glm::vec3(775,400,0), glm::vec3(0,0,0)),
	};
	vector <unsigned short> indices12 =
	{
		0,1,2,3,4,5,6,7,8,9
	};
	three_key->SetDrawMode(GL_LINES);
	three_key->InitFromData(vertices12, indices12);
	AddMeshToList(three_key);

	Mesh* polilinie_galben = new Mesh("polilinie_galben");
	vector<VertexFormat> vertices13
	{
		VertexFormat(glm::vec3(660,200,0), glm::vec3(1,1,0)),
		VertexFormat(glm::vec3(620,150,0), glm::vec3(1,1,0)),
		VertexFormat(glm::vec3(620,150,0), glm::vec3(1,1,0)),
		VertexFormat(glm::vec3(640,140,0), glm::vec3(1,1,0)),
		VertexFormat(glm::vec3(640,140,0), glm::vec3(1,1,0)),
		VertexFormat(glm::vec3(620,130,0), glm::vec3(1,1,0)),
		VertexFormat(glm::vec3(620,130,0), glm::vec3(1,1,0)),
		VertexFormat(glm::vec3(640,110,0), glm::vec3(1,1,0)),
	};
	vector <unsigned short> indices13 =
	{
		0,1,2,3,4,5,6,7
	};
	polilinie_galben->SetDrawMode(GL_LINES);
	polilinie_galben->InitFromData(vertices13, indices13);
	AddMeshToList(polilinie_galben);

	Mesh* polilinie_rosu = new Mesh("polilinie_rosu");
	vector<VertexFormat> vertices14
	{
		VertexFormat(glm::vec3(520,450,0), glm::vec3(1,0,0)),
		VertexFormat(glm::vec3(480,400,0), glm::vec3(1,0,0)),
		VertexFormat(glm::vec3(480,400,0), glm::vec3(1,0,0)),
		VertexFormat(glm::vec3(500,390,0), glm::vec3(1,0,0)),
		VertexFormat(glm::vec3(500,390,0), glm::vec3(1,0,0)),
		VertexFormat(glm::vec3(480,380,0), glm::vec3(1,0,0)),
		VertexFormat(glm::vec3(480,380,0), glm::vec3(1,0,0)),
		VertexFormat(glm::vec3(500,360,0), glm::vec3(1,0,0)),
	};
	vector <unsigned short> indices14 =
	{
		0,1,2,3,4,5,6,7
	};
	polilinie_rosu->SetDrawMode(GL_LINES);
	polilinie_rosu->InitFromData(vertices14, indices14);
	AddMeshToList(polilinie_rosu);

	Mesh* polilinie_mov= new Mesh("polilinie_mov");
	vector<VertexFormat> vertices15
	{
		VertexFormat(glm::vec3(920,450,0), glm::vec3(1,0,1)),
		VertexFormat(glm::vec3(880,400,0), glm::vec3(1,0,1)),
		VertexFormat(glm::vec3(880,400,0), glm::vec3(1,0,1)),
		VertexFormat(glm::vec3(900,390,0), glm::vec3(1,0,1)),
		VertexFormat(glm::vec3(900,390,0), glm::vec3(1,0,1)),
		VertexFormat(glm::vec3(880,380,0), glm::vec3(1,0,1)),
		VertexFormat(glm::vec3(880,380,0), glm::vec3(1,0,1)),
		VertexFormat(glm::vec3(900,360,0), glm::vec3(1,0,1)),
	};
	vector <unsigned short> indices15 =
	{
		0,1,2,3,4,5,6,7
	};
	polilinie_mov->SetDrawMode(GL_LINES);
	polilinie_mov->InitFromData(vertices15, indices15);
	AddMeshToList(polilinie_mov);


	Mesh* body_personaj = new Mesh("body_personaj");
	vector<VertexFormat> vertices16
	{
		VertexFormat(glm::vec3(0,0,0), glm::vec3(0.5,1,0)),
		VertexFormat(glm::vec3(0,1,0), glm::vec3(0.5,1,0)),
		VertexFormat(glm::vec3(1,1,0), glm::vec3(0.5,1,0)),
		VertexFormat(glm::vec3(1,0,0), glm::vec3(0.5,1,0)),
	};
	vector <unsigned short> indices16 =
	{
		0,1,2,3
	};
	body_personaj->SetDrawMode(GL_LINE_LOOP);
	body_personaj->InitFromData(vertices16, indices16);
	AddMeshToList(body_personaj);

	Mesh* mouth_personaj = new Mesh("mouth_personaj");
	vector<VertexFormat> vertices17
	{
		VertexFormat(glm::vec3(0,0,0), glm::vec3(0.5,1,0)),
		VertexFormat(glm::vec3(1,0,0), glm::vec3(0.5,1,0)),
	};
	vector <unsigned short> indices17 =
	{
		0,1
	};
	mouth_personaj->SetDrawMode(GL_LINES);
	mouth_personaj->InitFromData(vertices17, indices17);
	AddMeshToList(mouth_personaj);

	Mesh* cross_eye_personaj = new Mesh("cross_eye_personaj");
	vector<VertexFormat> vertices18
	{
		VertexFormat(glm::vec3(0,0,0), glm::vec3(0.5,1,0)),
		VertexFormat(glm::vec3(1,0,0), glm::vec3(0.5,1,0)),
	};
	vector <unsigned short> indices18 =
	{
		0,1
	};
	cross_eye_personaj->SetDrawMode(GL_LINES);
	cross_eye_personaj->InitFromData(vertices18, indices18);
	AddMeshToList(cross_eye_personaj);

	Mesh* body_personaj_2 = new Mesh("body_personaj_2");
	vector<VertexFormat> vertices19
	{
		VertexFormat(glm::vec3(0,0,0), glm::vec3(1,1,0)),
		VertexFormat(glm::vec3(0,1,0), glm::vec3(1,1,0)),
		VertexFormat(glm::vec3(1,1,0), glm::vec3(1,1,0)),
		VertexFormat(glm::vec3(1,0,0), glm::vec3(1,1,0)),
	};
	vector <unsigned short> indices19 =
	{
		0,1,2,3
	};
	body_personaj_2->SetDrawMode(GL_LINE_LOOP);
	body_personaj_2->InitFromData(vertices19, indices19);
	AddMeshToList(body_personaj_2);

	Mesh* mouth_personaj_2 = new Mesh("mouth_personaj_2");
	vector<VertexFormat> vertices20
	{
		VertexFormat(glm::vec3(0,0,0), glm::vec3(1,1,0)),
		VertexFormat(glm::vec3(1,0,0), glm::vec3(1,1,0)),
	};
	vector <unsigned short> indices20 =
	{
		0,1
	};
	mouth_personaj_2->SetDrawMode(GL_LINES);
	mouth_personaj_2->InitFromData(vertices20, indices20);
	AddMeshToList(mouth_personaj_2);

	Mesh* cross_eye_personaj_2 = new Mesh("cross_eye_personaj_2");
	vector<VertexFormat> vertices21
	{
		VertexFormat(glm::vec3(0,0,0), glm::vec3(1,1,0)),
		VertexFormat(glm::vec3(1,0,0), glm::vec3(1,1,0)),
	};
	vector <unsigned short> indices21 =
	{
		0,1
	};
	cross_eye_personaj_2->SetDrawMode(GL_LINES);
	cross_eye_personaj_2->InitFromData(vertices21, indices21);
	AddMeshToList(cross_eye_personaj_2);

	Mesh* body_personaj_3 = new Mesh("body_personaj_3");
	vector<VertexFormat> vertices22
	{
		VertexFormat(glm::vec3(0,0,0), glm::vec3(1,0,0)),
		VertexFormat(glm::vec3(0,1,0), glm::vec3(1,0,0)),
		VertexFormat(glm::vec3(1,1,0), glm::vec3(1,0,0)),
		VertexFormat(glm::vec3(1,0,0), glm::vec3(1,0,0)),
	};
	vector <unsigned short> indices22 =
	{
		0,1,2,3
	};
	body_personaj_3->SetDrawMode(GL_LINE_LOOP);
	body_personaj_3->InitFromData(vertices22, indices22);
	AddMeshToList(body_personaj_3);

	Mesh* mouth_personaj_3 = new Mesh("mouth_personaj_3");
	vector<VertexFormat> vertices23
	{
		VertexFormat(glm::vec3(0,0,0), glm::vec3(1,0,0)),
		VertexFormat(glm::vec3(1,0,0), glm::vec3(1,0,0)),
	};
	vector <unsigned short> indices23 =
	{
		0,1
	};
	mouth_personaj_3->SetDrawMode(GL_LINES);
	mouth_personaj_3->InitFromData(vertices23, indices23);
	AddMeshToList(mouth_personaj_3);

	Mesh* cross_eye_personaj_3 = new Mesh("cross_eye_personaj_3");
	vector<VertexFormat> vertices24
	{
		VertexFormat(glm::vec3(0,0,0), glm::vec3(1,0,0)),
		VertexFormat(glm::vec3(1,0,0), glm::vec3(1,0,0)),
	};
	vector <unsigned short> indices24 =
	{
		0,1
	};
	cross_eye_personaj_3->SetDrawMode(GL_LINES);
	cross_eye_personaj_3->InitFromData(vertices24, indices24);
	AddMeshToList(cross_eye_personaj_3);

	Mesh* shuriken = new Mesh("shuriken");
	vector<VertexFormat> vertices25
	{
		VertexFormat(glm::vec3(0,0,0), glm::vec3(1,0,0)),
		VertexFormat(glm::vec3(0,1,0), glm::vec3(1,0,0)),
		VertexFormat(glm::vec3(-1,1,0), glm::vec3(1,0,0)),
		VertexFormat(glm::vec3(0,0,0), glm::vec3(1,0,0)),
		VertexFormat(glm::vec3(1,1,0), glm::vec3(1,0,0)),
		VertexFormat(glm::vec3(1,0,0), glm::vec3(1,0,0)),
		VertexFormat(glm::vec3(0,0,0), glm::vec3(1,0,0)),
		VertexFormat(glm::vec3(1,-1,0), glm::vec3(1,0,0)),
		VertexFormat(glm::vec3(0,-1,0), glm::vec3(1,0,0)),
		VertexFormat(glm::vec3(0,0,0), glm::vec3(1,0,0)),
		VertexFormat(glm::vec3(-1,0,0), glm::vec3(1,0,0)),
		VertexFormat(glm::vec3(-1,-1,0), glm::vec3(1,0,0)),
	};
	vector <unsigned short> indices25 =
	{
		0,1,2,3,4,5,6,7,8,9,10,11
	};
	shuriken->SetDrawMode(GL_TRIANGLES);
	shuriken->InitFromData(vertices25, indices25);
	AddMeshToList(shuriken);


	float theta;
	Mesh* circle_rosu = new Mesh("circle_rosu");
	vector<VertexFormat> vertices26;
		for (float r = 0; r < 360; r ++)
		{
			theta = r * 3.142 / 180;
			contor_cercuri++;
			vertices26.push_back(VertexFormat(glm::vec3(0 + cos(theta), 0 + sin(theta), 0), glm::vec3(1, 0, 0)));
		}

		vector <unsigned short> indices26;
		for (float r = 0; r < contor_cercuri; r++)
		{
			indices26.push_back(r);
		}
	circle_rosu->SetDrawMode(GL_POLYGON);
	circle_rosu->InitFromData(vertices26, indices26);
	AddMeshToList(circle_rosu);

	contor_cercuri = -1;
	theta = 0;
	Mesh* circle_galben = new Mesh("circle_galben");
	vector<VertexFormat> vertices27;
	for (float r = 0; r < 360; r++)
	{
		theta = r * 3.142 / 180;
		contor_cercuri++;
		vertices27.push_back(VertexFormat(glm::vec3(0 + cos(theta), 0 + sin(theta), 0), glm::vec3(1, 1, 0)));
	}

	vector <unsigned short> indices27;
	for (float r = 0; r < contor_cercuri; r++)
	{
		indices27.push_back(r);
	}
	circle_galben->SetDrawMode(GL_POLYGON);
	circle_galben->InitFromData(vertices27, indices27);
	AddMeshToList(circle_galben);

	contor_cercuri = -1;
	theta = 0;
	Mesh* circle_mov = new Mesh("circle_mov");
	vector<VertexFormat> vertices28;
	for (float r = 0; r < 360; r++)
	{
		theta = r * 3.142 / 180;
		contor_cercuri++;
		vertices28.push_back(VertexFormat(glm::vec3(0 + cos(theta), 0 + sin(theta), 0), glm::vec3(1, 0, 1)));
	}

	vector <unsigned short> indices28;
	for (float r = 0; r < contor_cercuri; r++)
	{
		indices28.push_back(r);
	}
	circle_mov->SetDrawMode(GL_POLYGON);
	circle_mov->InitFromData(vertices28, indices28);
	AddMeshToList(circle_mov);

	contor_cercuri = -1;
	theta = 0;
	Mesh* semicerc_arc = new Mesh("semicerc_arc");
	vector<VertexFormat> vertices29;
	for (float r = 0; r < 180; r++)
	{
		theta = r * 3.142 / 180;
		contor_cercuri++;
		vertices29.push_back(VertexFormat(glm::vec3(0 + cos(theta), 0 + sin(theta), 0), glm::vec3(1, 1, 1)));
	}

	vector <unsigned short> indices29;
	for (float r = 0; r < contor_cercuri; r++)
	{
		indices29.push_back(r);
	}
	semicerc_arc->SetDrawMode(GL_LINE_LOOP);
	semicerc_arc->InitFromData(vertices29, indices29);
	AddMeshToList(semicerc_arc);

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

	CreateMeshes();

	// initializam tot ce tine de shurikenuri
	for (int i = 0; i < 500 * numar_shurikenuri + 1; i++)
	{
		laturi_shuriken_mid[i] = 50;
		laturi_shuriken_top[i] = 50;
		laturi_shuriken_bot[i] = 50;
		dead_shuriken_ids_mid[i] = -1;
		dead_shuriken_ids_top[i] = -1;
		dead_shuriken_ids_bot[i] = -1;
	}
	// initializam tot ce tine de baloanele rosii
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
	// initializam tot ce tine de baloanele galbene
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
	// pentru bonus am creat si baloane mov care odata lovite adauga o viata in plus jucatorului
	for (int i = 0; i < numar_baloane_mov * 600 + 1; i++)
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
	}

	/*if (strcmp(difficulty, "easy") == 0)
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
	}*/

	cout << endl;
	cout << "                                   INSTRUCTIONS                                       \n";
	cout << "--------------------------------------------------------------------------------------\n";
	cout << "Press K to enable/disable shurikens." << endl;
	cout << "Press B to enable/disable ballons." << endl;
	cout << "Press P to enable/disable personaj" << endl;
	cout << "Press 1,2,3 to change arrow's speed." << endl;
	cout << "                                   GAME SETTINGS                                      \n";
	cout << "--------------------------------------------------------------------------------------\n";
	cout << "Life count : 3" << endl;
	cout << "DIFFICULTY : " << difficulty << endl;
	cout << "                                    GAME STATUS                                       \n";
	cout << "--------------------------------------------------------------------------------------\n";
	cout << "SCORE: 0\n"; 
}

void Tema1::draw_crosshair(float inclinare_tinta, float translateY)
{
	// desenam powerbarul care ne indica viteza sagetii
	RenderMesh2D(meshes["powerbar"], shaders["VertexColor"], Transform2D::Translate(10, 100 + translateY * 60) * Transform2D::Scale(latura_powerbar, 25));

	// desenam o linia care reprezinta practic de unde va pleca sageata
	if (show_bonus_crosshair == true)
	{
		RenderMesh2D(meshes["linie1"], shaders["VertexColor"], Transform2D::Translate(10, 200 + translateY * 60) * Transform2D::Rotate(atan(mouse_l2 / mouse_l1)) * Transform2D::Scale(290, inclinare_tinta * 60));
	}
	else
	{
		RenderMesh2D(meshes["linie1"], shaders["VertexColor"], Transform2D::Translate(10, 200 + translateY * 60) * Transform2D::Scale(290, inclinare_tinta * 60));
	}
}

void Tema1::draw_arrow(float deltaTimeSeconds,float translateY)
{
	// desenam sagetile rand pe rand in momentul in care sunt trase
	for (int r = 1; r < 100; r++)
	{
		if (counter >= r)
		{
			if (OY_proiectile[r] == 0)
				OY_proiectile[r] = 300 + translateY * 60;
			viteze_proiectile[r] += deltaTimeSeconds * viteza_sageata[r];
			OX_proiectile[r] = 300 + viteze_proiectile[r] * 100;
			if (show_arrow == true && counter >= r && dead_arrows[r] != 1)
			{
				if (fix_orientation[r] == 0)
				{
					fix_orientation[r] = 1;
					inclinare_proiectile[r] = inclinare_tinta;

				}
				// desenam linia care face parte din sageata
				RenderMesh2D(meshes["linie1"], shaders["VertexColor"], Transform2D::Translate(10 + viteze_proiectile[r] * 100 , OY_proiectile[r] - 100 + viteze_proiectile[r] * (inclinare_proiectile[r] * 60 / 290) * 100) * Transform2D::Scale(290, inclinare_proiectile[r] * 60) );
				//RenderMesh2D(meshes["linie1"], shaders["VertexColor"], Transform2D::Translate(10 + viteze_proiectile[r] * 100, OY_proiectile[r] - 100 + viteze_proiectile[r] * (inclinare_proiectile[r] * 60 / 290) * 100) * Transform2D::Scale(290, 0));

				if (inclinare_proiectile[r] == 0)	
				{
					// desenam triunghiul care face parte din sageata
					RenderMesh2D(meshes["varf_sageata"], shaders["VertexColor"], Transform2D::Translate(275 + viteze_proiectile[r] * 100, OY_proiectile[r] - 150 + inclinare_proiectile[r] * 60 + viteze_proiectile[r] * (inclinare_proiectile[r] * 60 / 290) * 100) * Transform2D::Scale(75, 100));
				}
			}
		}
	}
}

void Tema1::draw_bow(float translateX, float translateY)
{
	// desenam linia care face parte din arc
	RenderMesh2D(meshes["linie_arc"], shaders["VertexColor"], Transform2D::Translate(5, 0 + translateY * 60) * Transform2D::Scale(0, 400));

	// desenam arcul de cerc
	RenderMesh2D(meshes["semicerc_arc"], shaders["VertexColor"], Transform2D::Translate(0 , 200 + translateY * 60) * Transform2D::Rotate(2*M_PI - M_PI/2)  * Transform2D::Scale(200, 200));

	
}

void Tema1::draw_baloons(float angularStep)
{
	if (show_ballons == true)
	{
		// baloane galbene
		for (int j = (numar_baloane_galbene - 1) * 300; j >= 0; j = j - 300)
		{
			// coliziune baloane galbene cu o sageata
			for (int r = 1; r < 100; r++)
			{

				// tratare coliziune balon galben cu sageata
				OY_Punct = OY_proiectile[r] - 100 + inclinare_proiectile[r] * 60 + viteze_proiectile[r] * (inclinare_proiectile[r] * 60 / 290) * 100;
				OX_Punct = OX_proiectile[r];
				raza_elipsa_ox = baloane_yellow_x[r];
				raza_elipsa_oy = baloane_yellow_y[r];
				origine_balon_x = 640;
				origine_balon_y = 250 - 100 - j + angularStep * speed_baloane_galbene[j];
				if ((pow((OX_Punct - origine_balon_x), 2) / pow( baloane_yellow_x[r], 2)) + (pow((OY_Punct - origine_balon_y), 2) / pow(baloane_yellow_y[r], 2)) <= 1)
				{
					dead_yellow_ballon = j;
					if (galben_vechi != j)
					{
						score = score - 100;
						galben_vechi = j;
					}
				}
			}
			// desenam elipsa
			RenderMesh2D(meshes["circle_galben"], shaders["VertexColor"], Transform2D::Translate(640 , 250 - 100 - j + angularStep * speed_baloane_galbene[j]) * Transform2D::Scale(baloane_yellow_x[j], baloane_yellow_y[j]));

			// desenam polilinia de sub fiecare balon
			RenderMesh2D(meshes["polilinie_galben"], shaders["VertexColor"], Transform2D::Translate(0, -100 - j + angularStep * speed2[j]));

		}
		// baloanele rosii
		for (int j = (numar_baloane_rosii - 1 ) * 300; j >= 0; j = j - 300)
		{
			for (int r = 1; r < 100; r++)
			{
				// tratare coliziune balon rosu cu sageata
				OY_Punct = OY_proiectile[r] - 100 + inclinare_proiectile[r] * 60 + viteze_proiectile[r] * (inclinare_proiectile[r] * 60 / 290) * 100;
				OX_Punct = OX_proiectile[r];
				raza_elipsa_ox = baloane_yellow_x[r];
				raza_elipsa_oy = baloane_yellow_y[r];
				origine_balon_x = 500;
				origine_balon_y = 500 - 100 - j + angularStep * speed_baloane_rosii[j];
				if ((pow((OX_Punct - origine_balon_x), 2) / pow(baloane_yellow_x[r], 2)) + (pow((OY_Punct - origine_balon_y), 2) / pow(baloane_yellow_y[r], 2)) <= 1)
				{
					dead_red_ballon = j;
					if (rosu_vechi != j)
					{
						score = score + 100;
						rosu_vechi = j;
					}
				}
			}
			// desenam elipsa
			RenderMesh2D(meshes["circle_rosu"], shaders["VertexColor"], Transform2D::Translate(500, 500 - 100 - j + angularStep * speed_baloane_rosii[j]) * Transform2D::Scale(baloane_red_x[j], baloane_red_y[j]));
			// desenam polilinia
			RenderMesh2D(meshes["polilinie_rosu"], shaders["VertexColor"], Transform2D::Translate(0, -100 - j + angularStep * speed3[j]));

		}
		//  baloanele mov
		for (int j = (numar_baloane_mov - 1) * 600; j >= 0; j = j - 600)
		{
			for (int r = 1; r < 100; r++)
			{
				
				// tratare coliziune sageata arc mov
				OY_Punct = OY_proiectile[r] - 100 + inclinare_proiectile[r] * 60 + viteze_proiectile[r] * (inclinare_proiectile[r] * 60 / 290) * 100;
				OX_Punct = OX_proiectile[r];
				raza_elipsa_ox = baloane_yellow_x[r];
				raza_elipsa_oy = baloane_yellow_y[r];
				origine_balon_x = 900;
				origine_balon_y = 500 - 100 - j + angularStep * speed_baloane_mov[j];
				if ((pow((OX_Punct - origine_balon_x), 2) / pow(baloane_yellow_x[r], 2)) + (pow((OY_Punct - origine_balon_y), 2) / pow(baloane_yellow_y[r], 2)) <= 1)
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
			// desenam elipsa
			RenderMesh2D(meshes["circle_mov"], shaders["VertexColor"], Transform2D::Translate(900, 500 - 100 - j + angularStep * speed_baloane_mov[j])* Transform2D::Scale(baloane_mov_x[j], baloane_mov_y[j]));

			// desenam polilinia de sub fiecare balon
			RenderMesh2D(meshes["polilinie_mov"], shaders["VertexColor"], Transform2D::Translate(0, -100 - j + angularStep * speed4[j]));
			

		}
	}
}

void Tema1::draw_shurikens(float angularStep,float translateY)
{
	if (show_shuriken == true)
	{
		for (int i = (numar_shurikenuri - 1) * 500; i >= 0; i = i - 500)
		{

			OX_shuriken[1] = 1100 + i - angularStep * viteza_shurikenuri;
			OY_shuriken[1] = 260;
			for (int r = 1; r < 100; r++)
			{
				// tratare coliziuni sageata cu shuriken mid
				OY_Punct = OY_proiectile[r] - 100 + inclinare_proiectile[r] * 60 + viteze_proiectile[r] * (inclinare_proiectile[r] * 60 / 290) * 100;
				OX_Punct = OX_proiectile[r];
				raza_elipsa_ox = 50;
				raza_elipsa_oy = 50;
				origine_balon_x = 1100 + i - angularStep * viteza_shurikenuri;
				origine_balon_y = 260 ;
				if ( (pow((OX_Punct - origine_balon_x), 2) / pow(laturi_shuriken_mid[i], 2)) + (pow((OY_Punct - origine_balon_y), 2) / pow(laturi_shuriken_mid[i], 2)) <= 1 && dead_arrows[r] != 1 && dead_shuriken_ids_mid[i] != i)
				{
					// aici practic tinem minte care shuriken din vectorul de shurikenuri de pe mid a fost lovit si il facem sa se micsoreze si marim scorul
					OK = 1;
					dead_shuriken_id_mid = i;
					dead_shuriken_ids_mid[i] = i;
					dead_arrows[r] = 1;
					score = score + 100;
				}
				// tratare coliziuni sageata cu shuriken top
				OY_Punct = OY_proiectile[r] - 100 + inclinare_proiectile[r] * 60 + viteze_proiectile[r] * (inclinare_proiectile[r] * 60 / 290) * 100;
				OX_Punct = OX_proiectile[r];
				raza_elipsa_ox = 50;
				raza_elipsa_oy = 50;
				origine_balon_x = 1100 + i - angularStep * viteza_shurikenuri;
				origine_balon_y = 460;
				if ((pow((OX_Punct - origine_balon_x), 2) / pow(laturi_shuriken_top[i], 2)) + (pow((OY_Punct - origine_balon_y), 2) / pow(laturi_shuriken_top[i], 2)) <= 1 && dead_arrows[r] != 1 && dead_shuriken_ids_top[i] != i)
				{
					// aici practic tinem minte care shuriken din vectorul de shurikenuri de pe top a fost lovit si il facem sa se micsoreze si marim scorul
					OK2 = 1;
					dead_shuriken_id_top = i;
					dead_shuriken_ids_top[i] = i;
					dead_arrows[r] = 1;
					score = score + 100;
				}
				// tratare coliziuni sageata cu shuriken bot
				OY_Punct = OY_proiectile[r] - 100 + inclinare_proiectile[r] * 60 + viteze_proiectile[r] * (inclinare_proiectile[r] * 60 / 290) * 100;
				OX_Punct = OX_proiectile[r];
				raza_elipsa_ox = 50;
				raza_elipsa_oy = 50;
				origine_balon_x = 1100 + i - angularStep * viteza_shurikenuri;
				origine_balon_y = 60;
				if ((pow((OX_Punct - origine_balon_x), 2) / pow(laturi_shuriken_bot[i], 2)) + (pow((OY_Punct - origine_balon_y), 2) / pow(laturi_shuriken_bot[i], 2)) <= 1 && dead_arrows[r] != 1 && dead_shuriken_ids_bot[i] != i)
				{
					// aici practic tinem minte care shuriken din vectorul de shurikenuri de pe bot a fost lovit si il facem sa se micsoreze si marim scorul
					OK3 = 1;
					dead_shuriken_id_bot = i;
					dead_shuriken_ids_bot[i] = i;
					dead_arrows[r] = 1;
					score = score + 100;
				}
				OY_Punct = 260;
				OX_Punct = OX_shuriken[1] - laturi_shuriken_mid[r];
				raza_elipsa_ox = 200;
				raza_elipsa_oy = 200;
				origine_balon_x = 0;
				origine_balon_y = 200 + translateY * 60;
				// daca arcul este atins de un shuriken de pe mid / coliziune arc shuriken mid
				if ((pow((OX_Punct - origine_balon_x), 2) / pow(raza_elipsa_ox, 2)) + (pow((OY_Punct - origine_balon_y), 2) / pow(raza_elipsa_oy, 2)) <= 1 && dead_shuriken_ids_mid[i] != i)
				{
					// daca arcul a fost atins de un shuriken tinem minte acel shuriken ca sa nu scadem viata mai mult decat este nevoie
					if (shuriken_mid_vechi != i)
					{
						life_count--;
						shuriken_mid_vechi = i;
					}
				}
				// daca arcul este atins de un shuriken de pe bot / coliziune arc shuriken bot
				OY_Punct = 60;
				OX_Punct = OX_shuriken[1] - laturi_shuriken_mid[r];
				raza_elipsa_ox = 200;
				raza_elipsa_oy = 200;
				origine_balon_x = 0;
				origine_balon_y = 200 + translateY * 60;
				if ((pow((OX_Punct - origine_balon_x), 2) / pow(raza_elipsa_ox, 2)) + (pow((OY_Punct - origine_balon_y), 2) / pow(raza_elipsa_oy, 2)) <= 1 && dead_shuriken_ids_bot[i] != i)
				{
					// daca arcul a fost atins de un shuriken tinem minte acel shuriken ca sa nu scadem viata mai mult decat este nevoie
					if (shuriken_bot_vechi != i)
					{
						life_count--;
						shuriken_bot_vechi = i;
					}
				}
				// daca arcul este atins de un shuriken de pe top / coliziune arc shuriken top
				OY_Punct = 460;
				OX_Punct = OX_shuriken[1] - laturi_shuriken_mid[r];
				raza_elipsa_ox = 200;
				raza_elipsa_oy = 200;
				origine_balon_x = 0;
				origine_balon_y = 200 + translateY * 60;
				if ((pow((OX_Punct - origine_balon_x), 2) / pow(raza_elipsa_ox, 2)) + (pow((OY_Punct - origine_balon_y), 2) / pow(raza_elipsa_oy, 2)) <= 1 && dead_shuriken_ids_top[i] != i)
				{
					// daca arcul a fost atins de un shuriken tinem minte acel shuriken ca sa nu scadem viata mai mult decat este nevoie
					if (shuriken_top_vechi != i)
					{
						life_count--;
						shuriken_top_vechi = i;
					}
				}
			}

			// desenez pe ecran shurikenurile de pe mid
			
			RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], Transform2D::Translate(1100 + i - angularStep * viteza_shurikenuri, 260) * Transform2D::Rotate(angularStep) * Transform2D::Scale(laturi_shuriken_mid[i], laturi_shuriken_mid[i]));

			// desenez pe ecran shurikenurile de pe top
			
			RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], Transform2D::Translate(1100 + i - angularStep * viteza_shurikenuri, 460) * Transform2D::Rotate(angularStep) * Transform2D::Scale(laturi_shuriken_top[i], laturi_shuriken_top[i]));

			// desenez pe ecran shurikenurile de pe bot
			
			RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], Transform2D::Translate(1100 + i - angularStep * viteza_shurikenuri, 60)* Transform2D::Rotate(angularStep)* Transform2D::Scale(laturi_shuriken_bot[i], laturi_shuriken_bot[i]));

		}
		// daca un shuriken de pe mid a fost atins de sageata atunci ii scadem treptat latura si dam impresia ca se micsoreaza
		if (laturi_shuriken_mid[dead_shuriken_id_mid] > 0 && OK == 1)
			laturi_shuriken_mid[dead_shuriken_id_mid]--;
		// daca un shuriken de pe top a fost atins de sageata atunci ii scadem treptat latura si dam impresia ca se micsoreaza
		if (laturi_shuriken_top[dead_shuriken_id_top] > 0 && OK2 == 1)
			laturi_shuriken_top[dead_shuriken_id_top]--;
		// daca un shuriken de pe bot a fost atins de sageata atunci ii scadem treptat latura si dam impresia ca se micsoreaza
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
		cout << "YOUR SCORE WAS : " << score << endl;
		semafor_game_over = true;
		//exit(0);
	}
}

void Tema1::draw_personaj(GLint x0, GLint y0, int latura_patrat, float angularStep,float deltaTimeSeconds)
{
	// pentru bonus mi-am definit un personaj in partea dreapta a ecranului care se misca sus jos si care are o linie de viata si in momentul
	// in care i se aplica damage acesta isi schimba expresia faciala, cand personajul este omorat jucatorul primeste la scor o valoare mare
	if (draw_personaj_status == true && latura_powerbar_boss > 0 )
	{
		// pastram o copie pentru angular astfel cand ii schimbam directa de deplasare totul sa fie smooth
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
		// coordonatele patratului
		GLint x1 = x0;
		GLint y1 = y0 + latura_patrat;
		GLint x2 = x0 + latura_patrat;
		GLint y2 = y0 + latura_patrat;
		GLint x3 = x0 + latura_patrat;
		GLint y3 = y0;
		// desenam linia de viata a personajului
		RenderMesh2D(meshes["powerbar"], shaders["VertexColor"], Transform2D::Translate(x0, y0 + 25) * Transform2D::Scale(latura_powerbar_boss, 5));
		// expresia faciala 1
		if (happy_contor % 3 == 1)
		{
			RenderMesh2D(meshes["body_personaj"], shaders["VertexColor"], Transform2D::Translate(x0, y0 - 100) * Transform2D::Scale(latura_patrat, latura_patrat));
			RenderMesh2D(meshes["mouth_personaj"], shaders["VertexColor"], Transform2D::Translate(x0 + latura_patrat / 4, y0 - 3 * latura_patrat / 4) * Transform2D::Scale(latura_patrat / 2, 0));
			RenderMesh2D(meshes["cross_eye_personaj"], shaders["VertexColor"], Transform2D::Translate(x0 + latura_patrat / 8, y0 - latura_patrat / 4) * Transform2D::Scale(latura_patrat / 4, 0));
			RenderMesh2D(meshes["cross_eye_personaj"], shaders["VertexColor"], Transform2D::Translate(x0 + 5 * latura_patrat / 8, y0 - latura_patrat / 4) * Transform2D::Scale(latura_patrat / 4, 0));
		}
		// expresia faciala 2
		if (happy_contor % 3 == 2)
		{
			RenderMesh2D(meshes["body_personaj_2"], shaders["VertexColor"], Transform2D::Translate(x0, y0 - 100) * Transform2D::Scale(latura_patrat, latura_patrat));
			RenderMesh2D(meshes["mouth_personaj_2"], shaders["VertexColor"], Transform2D::Translate(x0 + latura_patrat / 4, y0 - 3 * latura_patrat / 4) * Transform2D::Scale(latura_patrat / 2, 0));
			RenderMesh2D(meshes["mouth_personaj_2"], shaders["VertexColor"], Transform2D::Translate(x0 + latura_patrat / 4, y0 - 3 * latura_patrat / 4) * Transform2D::Rotate(M_PI / 2) * Transform2D::Scale(latura_patrat / 8, 0));
			RenderMesh2D(meshes["mouth_personaj_2"], shaders["VertexColor"], Transform2D::Translate(x0 + latura_patrat / 4, y0 -  3 * latura_patrat / 4 + latura_patrat / 8) * Transform2D::Scale(latura_patrat / 2, 0));
			RenderMesh2D(meshes["mouth_personaj_2"], shaders["VertexColor"], Transform2D::Translate(x0 + 3* latura_patrat / 4, y0 - 3 * latura_patrat / 4) * Transform2D::Rotate(M_PI / 2) * Transform2D::Scale(latura_patrat / 8, 0));
			RenderMesh2D(meshes["cross_eye_personaj_2"], shaders["VertexColor"], Transform2D::Translate(x0 + latura_patrat / 8, y0 - latura_patrat / 4) * Transform2D::Scale(latura_patrat / 4, 0));
			RenderMesh2D(meshes["cross_eye_personaj_2"], shaders["VertexColor"], Transform2D::Translate(x0 + 5 * latura_patrat / 8, y0 - latura_patrat / 4) * Transform2D::Translate(latura_patrat / 8, 0) * Transform2D::Rotate(M_PI / 4) * Transform2D::Translate(-latura_patrat / 8, 0) * Transform2D::Scale(latura_patrat / 4, 0));
			RenderMesh2D(meshes["cross_eye_personaj_2"], shaders["VertexColor"], Transform2D::Translate(x0 + 5 * latura_patrat / 8, y0 - latura_patrat / 4) * Transform2D::Translate(latura_patrat / 8, 0) * Transform2D::Rotate(2 * M_PI - M_PI / 4) * Transform2D::Translate(-latura_patrat / 8, 0) * Transform2D::Scale(latura_patrat / 4, 0));
		}
		// expresia faciala 3
		if (happy_contor % 3 == 0)
		{
			RenderMesh2D(meshes["body_personaj_3"], shaders["VertexColor"], Transform2D::Translate(x0, y0 - 100) * Transform2D::Scale(latura_patrat, latura_patrat));
			RenderMesh2D(meshes["mouth_personaj_3"], shaders["VertexColor"], Transform2D::Translate(x0 + latura_patrat / 4, y0 - 3 * latura_patrat / 4) * Transform2D::Scale(latura_patrat / 2, 0));
			RenderMesh2D(meshes["cross_eye_personaj_3"], shaders["VertexColor"], Transform2D::Translate(x0 + latura_patrat / 8, y0 - latura_patrat / 4) * Transform2D::Translate(latura_patrat / 8, 0) * Transform2D::Rotate(M_PI / 4) * Transform2D::Translate(-latura_patrat / 8, 0) * Transform2D::Scale(latura_patrat / 4, 0));
			RenderMesh2D(meshes["cross_eye_personaj_3"], shaders["VertexColor"], Transform2D::Translate(x0 + latura_patrat / 8, y0 - latura_patrat / 4) * Transform2D::Translate(latura_patrat / 8, 0) * Transform2D::Rotate(2 * M_PI - M_PI / 4) * Transform2D::Translate(-latura_patrat / 8, 0) * Transform2D::Scale(latura_patrat / 4, 0));
			RenderMesh2D(meshes["cross_eye_personaj_3"], shaders["VertexColor"], Transform2D::Translate(x0 + 5 * latura_patrat / 8, y0 - latura_patrat / 4) * Transform2D::Translate(latura_patrat / 8, 0) * Transform2D::Rotate(M_PI / 4) * Transform2D::Translate(-latura_patrat / 8, 0) * Transform2D::Scale(latura_patrat / 4, 0));
			RenderMesh2D(meshes["cross_eye_personaj_3"], shaders["VertexColor"], Transform2D::Translate(x0 + 5 * latura_patrat / 8, y0 - latura_patrat / 4) * Transform2D::Translate(latura_patrat / 8, 0) * Transform2D::Rotate(2 * M_PI - M_PI / 4) * Transform2D::Translate(-latura_patrat / 8, 0) * Transform2D::Scale(latura_patrat / 4, 0));
		}
		// aici tratam coliziunea dintre sageata si personaj
		for (int r = 1; r < 100; r++)
		{

			if (OY_proiectile[r] + inclinare_proiectile[r] * 60 + viteze_proiectile[r] * (inclinare_proiectile[r] * 60 / 290) * 100 < y1 && OY_proiectile[r] + inclinare_proiectile[r] * 60 + viteze_proiectile[r] * (inclinare_proiectile[r] * 60 / 290) * 100 > y0 && OX_proiectile[r] <= x3 && OX_proiectile[r] >= x0)
			{
				if (dead_personaj_arrows[r] != 1)
				{
					happy_contor++;
					dead_personaj_arrows[r] = 1;
					latura_powerbar_boss -= 10;
					if (latura_powerbar_boss <= 0)
					{
						cout << "WELL DONE! YOU KILLED THE BOSS!" << endl;
						score = score + 1000;
					}
				}
			}
		}
	}
}


void Tema1::draw_game_over()
{
	// pentru bonus am creat un ecran care apare atunci cand jucatorul este mort si dupa 3,4 secunde jocul se inchide automat
	// pe ecran ar trebui afisata culoarea alba si sa scrie "GG <3" :D
	RenderMesh2D(meshes["g_letter"], shaders["VertexColor"], glm::mat3(1));
	RenderMesh2D(meshes["g_letter"], shaders["VertexColor"], Transform2D::Translate(100,0));
	RenderMesh2D(meshes["lower_key"], shaders["VertexColor"], glm::mat3(1));
	RenderMesh2D(meshes["three_key"], shaders["VertexColor"], glm::mat3(1));
	RenderMesh2D(meshes["game_over_screen"], shaders["VertexColor"], Transform2D::Scale(1280, 720));
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


	//RenderMesh2D(meshes["linie1"], shaders["VertexColor"], Transform2D::Rotate(30) * Transform2D::Translate(50 , 50) * Transform2D::Scale(20,30) );

	previous_score = score;
	life_count_prev = life_count;
	if (semafor_game_over == true)
	{
		draw_game_over();
		counter_game_over--;
		if (counter_game_over == 0)
		{
			exit(0);
		}
	}

	modelMatrix = glm::mat3(1);
	//// ultima modficiare
	modelMatrix *= Transform2D::Translate(0, -100);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
	
	draw_personaj(1100, 100, 100, angularStep, deltaTimeSeconds);
	
	draw_baloons(angularStep);

	draw_shurikens(angularStep, translateY);

	draw_crosshair(inclinare_tinta, translateY);

	draw_bow(translateX, translateY);

	draw_arrow(deltaTimeSeconds, translateY);

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
	// daca apasam continuu sageata in dreapta marim viteza arcului
	if (window->KeyHold(GLFW_KEY_RIGHT) == 1)
	{
		viteza_s = viteza_s + 0.05;
		latura_powerbar++;
	}
	// daca apasam continuu click stanga marim viteza arcului
	if (window->MouseHold(GLFW_MOUSE_BUTTON_1) == 1)
	{
		viteza_s = viteza_s + 0.05;
		latura_powerbar++;
	}
}

void Tema1::OnKeyPress(int key, int mods)
{
	// add key press event
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
	// daca apasam tasta P afisam personajul
	if (key == GLFW_KEY_P)
	{
		if (draw_personaj_status == false)
			draw_personaj_status = true;
		else
			draw_personaj_status = false;
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
	// daca apasam M putem configura ca crosshairul sa urmareasca pointerul mouseului
	if (key == GLFW_KEY_M)
	{
		if (show_bonus_crosshair == false)
			show_bonus_crosshair = true;
		else
			show_bonus_crosshair = false;
	}
	
}

void Tema1::OnKeyRelease(int key, int mods)
{
	// add key release event
	// daca dam drumul la sageata in dreapta lansam arcul
	if (key == GLFW_KEY_RIGHT)
	{
		indice_sageti++;
		viteza_sageata[indice_sageti] = viteza_s;
		viteza_s = 2;
		show_arrow = true;
		counter++;
		copie_angular = 0.001;
		latura_powerbar = 0;
	}
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
	mouse_l1 = 290;
	mouse_l2 = 720 - mouseY - 200;
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event

	// daca apasam click stanga tragem cu arcul
	// aici era pentru bonus dar nu am reusit sa duc pana la capat
	/*if (button == GLFW_MOUSE_BUTTON_2)
	{
		mouse_l1 = 290;
		mouse_l2 = 720 - mouseY - 200;
	}*/

}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event

	// daca dam drumul la click stanga tragem sageata
	if (button == GLFW_MOUSE_BUTTON_2)
	{
		indice_sageti++;
		viteza_sageata[indice_sageti] = viteza_s;
		viteza_s = 2;
		show_arrow = true;
		counter++;
		copie_angular = 0.001;
		latura_powerbar = 0;
	}
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}
