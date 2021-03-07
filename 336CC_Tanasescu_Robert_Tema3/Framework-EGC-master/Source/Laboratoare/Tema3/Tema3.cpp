#include "Tema3.h"
#include <vector>
#include <string>
#include <iostream>
#include <Core/Engine.h>

using namespace std;

int pickups_location_id[10000];
int pickups_location_id_position[10000];
float pickups_z[10000];
int pickups_status[10000];
int pickups_is_picked[10000];

float copie_angular_2 = 0;
bool semafor = false;
float y_pickups = 1.75f;
float x_pickups = 0;

int pickups_model_id[10000];

float score = 0;
float previous_score = -1;

int change_platforms = 0;

float latura_score = 0;

int decor_type_left[10000];
int decor_type_right[10000];

int box_location_id[10000];
int box_location_id_position[10000];

float invulnerability_time = 300;

int player_skin = 1;



Tema3::Tema3()
{
}

Tema3::~Tema3()
{
}

Mesh* Tema3::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices)
{
	unsigned int VAO;
	// TODO: Create the VAO and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// TODO: Create the VBO and bind it
	//unsigned int VBO = 0;
	GLuint VBO_ID;			// ID-ul (nume sau referinta) buffer-ului ce va fi cerut de la GPU
	glGenBuffers(1, &VBO_ID);	// se genereaza ID-ul (numele) bufferului

	// TODO: Send vertices data into the VBO buffer

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// TODO: Crete the IBO and bind it
	//unsigned int IBO = 0;
	GLuint IBO_ID;
	glGenBuffers(1, &IBO_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// TODO: Send indices data into the IBO buffer

	// ========================================================================
	// This section describes how the GPU Shader Vertex Shader program receives data
	// It will be learned later, when GLSL shaders will be introduced
	// For the moment just think that each property value from our vertex format needs to be send to a certain channel
	// in order to know how to receive it in the GLSL vertex shader

	// set vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// set vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	// set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	// set vertex color attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	// ========================================================================

	// TODO: Unbind the VAO
	glBindVertexArray(0);

	// Check for OpenGL errors
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	meshes[name] = new Mesh(name);
	meshes[name]->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	return meshes[name];
}

void Tema3::Init()
{
	renderCameraTarget = false;

	camera = new Tema::Camera();
	camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

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
		pickups_z[i] = -1;
		pickups_location_id_position[i] = rand() % 3 + 1;
		pickups_location_id[i] = 0;
		pickups_status[i] = 1;
		pickups_is_picked[i] = 0;
		pickups_model_id[i] = rand() % 3;	// 3
		decor_type_left[i] = rand() % 5;
		decor_type_right[i] = rand() % 4;
	}

	for (int z = 0; z < 10; z++)
	{
		box_location_id_position[z] = -5;
		box_location_id[z] = -5;
	}
	for (int z = 10; z < 10000; z++)
	{
		box_location_id_position[z] = rand() % 3 + 1;
		box_location_id[z] = 0;
	}

	for (int i = 0; i < 10000; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			platforme_parcurse[i][j] = 0;
		}
	}

	vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(0, 0,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 0,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0, 2,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 2,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0, 0,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 0,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0, 2,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 2,  0), glm::vec3(0, 1, 1)),

		VertexFormat(glm::vec3(0, 2,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 2,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 2,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0, 2,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(1, 3.75f,  1), glm::vec3(0, 1, 1)),
	};

	vector<unsigned short> indices =
	{
		0, 1, 2,	// indices for first triangle
		1, 3, 2,	// indices for second triangle
		2, 3, 7,
		2, 7, 6,
		1, 7, 3,
		1, 5, 7,
		6, 7, 4,
		7, 5, 4,
		0, 4, 1,
		1, 4, 5,
		2, 6, 4,
		0, 2, 4,

		8,9,12,
		9,10,12,
		10,11,12,
		8,11,12
	};

	vector<VertexFormat> vertices2
	{
		VertexFormat(glm::vec3(0, 2,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 2,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 2,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0, 2,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(1, 3,  1), glm::vec3(0, 1, 1)),
	};

	vector<unsigned short> indices2 =
	{
		0,1,4,
		1,2,4,
		2,3,4,
		0,3,4
	};
	
	vector<VertexFormat> vertices3
	{
		VertexFormat(glm::vec3(0, 0,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 0,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0, 2,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 2,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0, 0,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 0,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0, 2,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 2,  0), glm::vec3(0, 1, 1)),

		VertexFormat(glm::vec3(0, 3.25f,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 3.25f,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 3.25f,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0, 3.25f,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(1, 1.5f,  1), glm::vec3(0, 1, 1)),
	};

	vector<unsigned short> indices3 =
	{
		0, 1, 2,	// indices for first triangle
		1, 3, 2,	// indices for second triangle
		2, 3, 7,
		2, 7, 6,
		1, 7, 3,
		1, 5, 7,
		6, 7, 4,
		7, 5, 4,
		0, 4, 1,
		1, 4, 5,
		2, 6, 4,
		0, 2, 4,

		8,9,12,
		9,10,12,
		10,11,12,
		8,11,12
	};

	vector<VertexFormat> vertices4
	{
		VertexFormat(glm::vec3(0 - 4, 0,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 0,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0 - 4, 2,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 2,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0 - 4, 0,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 0,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0 - 4, 2,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2 , 2,  0), glm::vec3(0, 1, 1)),
		
		VertexFormat(glm::vec3(0.5f - 4, 2,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0.5f - 4, 2,  1.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0 - 4, 2,  1.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0 - 4, 3,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0.5f - 4, 3,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0.5f - 4 , 3,  1.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0 - 4, 3,  1.5f), glm::vec3(0, 1, 1)),

		VertexFormat(glm::vec3(0 - 4, 2,  0.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0.5f - 4, 2,  0.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0.5f - 4, 2,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0 - 4, 3,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0 - 4, 3,  0.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0.5f - 4, 3,  0.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0.5f - 4, 3,  0), glm::vec3(0, 1, 1)),
	
		//22
		VertexFormat(glm::vec3(1.5f, 2,  0), glm::vec3(0, 1, 1)),
		//23
		VertexFormat(glm::vec3(1.5f, 2,  0.5f), glm::vec3(0, 1, 1)),
		//23
		VertexFormat(glm::vec3(2, 2,  0.5f), glm::vec3(0, 1, 1)),
		//25
		VertexFormat(glm::vec3(1.5f, 3,  0), glm::vec3(0, 1, 1)),
		//26
		VertexFormat(glm::vec3(1.5f, 3,  0.5f), glm::vec3(0, 1, 1)),
		//27
		VertexFormat(glm::vec3(2, 3,  0.5f), glm::vec3(0, 1, 1)),
		//28
		VertexFormat(glm::vec3(2, 3,  0), glm::vec3(0, 1, 1)),
		

		VertexFormat(glm::vec3(2, 2,  1.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(1.5f, 2,  1.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(1.5f, 2,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 3,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 3,  1.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(1.5f, 3,  1.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(1.5f, 3,  2), glm::vec3(0, 1, 1)),
	
		VertexFormat(glm::vec3(0 - 4, 3,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 3,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0 - 4, 3.5f,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 3.5f,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0 - 4, 3,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 3,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0 - 4, 3.5f,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 3.5f,  0), glm::vec3(0, 1, 1)),
	};

	vector<unsigned short> indices4 =
	{
		0, 1, 2,	// indices for first triangle
		1, 3, 2,	// indices for second triangle
		2, 3, 7,
		2, 7, 6,
		1, 7, 3,
		1, 5, 7,
		6, 7, 4,
		7, 5, 4,
		0, 4, 1,
		1, 4, 5,
		2, 6, 4,
		0, 2, 4,

		2,8,11,
		8,12,11,
		10,12,13,
		11,13,14,
		8,13,12,
		8,9,13,
		14,13,10,
		13,9,10,
		2,10,8,
		8,10,9,
		11,14,10,
		2,11,10,

		15,16,19,
		16,20,19,
		19,20,21,
		19,21,18,
		16,21,20,
		16,17,21,
		18,21,6,
		21,17,6,
		15,6,3,
		16,6,17,
		19,18,6,
		15,19,6,


		23,24,26,
		24,27,26,
		26,27,28,
		26,28,25,
		24,28,27,
		24,7,28,
		25,28,22,
		28,7,22,
		23,22,24,
		24,22,7,
		26,25,22,
		23,26,22,

		31,3,35,
		3,32,35,
		35,32,33,
		35,33,34,
		3,33,32,
		3,29,33,
		34,33,30,
		33,29,30,
		31,30,3,
		3,30,29,
		35,34,30,
		31,35,30,

		36,37,38,
		37,39,38,
		38,39,43,
		38,43,42,
		37,43,39,
		37,41,43,
		42,43,40,
		43,41,40,
		36,40,37,
		37,40,41,
		38,42,40,
		36,38,40

	};

	

	vector<VertexFormat> vertices5
	{
		VertexFormat(glm::vec3(0 - 4, 0,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 0,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0 - 4, 2,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 2,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0 - 4, 0,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 0,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0 - 4, 2,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 2,  0), glm::vec3(0, 1, 1)),

		VertexFormat(glm::vec3(0.5f - 4, 2,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0.5f - 4, 2,  1.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0 - 4, 2,  1.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0 - 4, 3,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0.5f - 4, 3,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0.5f - 4, 3,  1.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0 - 4, 3,  1.5f), glm::vec3(0, 1, 1)),

		VertexFormat(glm::vec3(0 - 4, 2,  0.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0.5f - 4, 2,  0.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0.5f - 4, 2,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0 - 4, 3,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0 - 4, 3,  0.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0.5f - 4, 3,  0.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0.5f - 4, 3,  0), glm::vec3(0, 1, 1)),

		//22
		VertexFormat(glm::vec3(1.5f, 2,  0), glm::vec3(0, 1, 1)),
		//23
		VertexFormat(glm::vec3(1.5f, 2,  0.5f), glm::vec3(0, 1, 1)),
		//23
		VertexFormat(glm::vec3(2, 2,  0.5f), glm::vec3(0, 1, 1)),
		//25
		VertexFormat(glm::vec3(1.5f, 3,  0), glm::vec3(0, 1, 1)),
		//26
		VertexFormat(glm::vec3(1.5f, 3,  0.5f), glm::vec3(0, 1, 1)),
		//27
		VertexFormat(glm::vec3(2, 3,  0.5f), glm::vec3(0, 1, 1)),
		//28
		VertexFormat(glm::vec3(2, 3,  0), glm::vec3(0, 1, 1)),


		VertexFormat(glm::vec3(2, 2,  1.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(1.5f, 2,  1.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(1.5f, 2,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 3,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 3,  1.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(1.5f, 3,  1.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(1.5f, 3,  2), glm::vec3(0, 1, 1)),

		VertexFormat(glm::vec3(0 - 4, 3,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 3,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 3,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0 - 4, 3,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(1 - 2, 4,  1 ), glm::vec3(0, 1, 1)),
	};

	vector<unsigned short> indices5 =
	{
		0, 1, 2,	// indices for first triangle
		1, 3, 2,	// indices for second triangle
		2, 3, 7,
		2, 7, 6,
		1, 7, 3,
		1, 5, 7,
		6, 7, 4,
		7, 5, 4,
		0, 4, 1,
		1, 4, 5,
		2, 6, 4,
		0, 2, 4,

		2,8,11,
		8,12,11,
		10,12,13,
		11,13,14,
		8,13,12,
		8,9,13,
		14,13,10,
		13,9,10,
		2,10,8,
		8,10,9,
		11,14,10,
		2,11,10,

		15,16,19,
		16,20,19,
		19,20,21,
		19,21,18,
		16,21,20,
		16,17,21,
		18,21,6,
		21,17,6,
		15,6,3,
		16,6,17,
		19,18,6,
		15,19,6,


		23,24,26,
		24,27,26,
		26,27,28,
		26,28,25,
		24,28,27,
		24,7,28,
		25,28,22,
		28,7,22,
		23,22,24,
		24,22,7,
		26,25,22,
		23,26,22,

		31,3,35,
		3,32,35,
		35,32,33,
		35,33,34,
		3,33,32,
		3,29,33,
		34,33,30,
		33,29,30,
		31,30,3,
		3,30,29,
		35,34,30,
		31,35,30,

		36,37,40,
		37,38,40,
		38,39,40,
		36,39,40,

	};

	vector<VertexFormat> vertices6
	{
		VertexFormat(glm::vec3(0, 0,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2 + 4, 0,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0, 2,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2 + 4, 2,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0, 0,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2 + 4, 0,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0, 2,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2 + 4 , 2,  0), glm::vec3(0, 1, 1)),

		VertexFormat(glm::vec3(0.5f, 2,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0.5f, 2,  1.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0, 2,  1.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0, 3,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0.5f, 3,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0.5f, 3,  1.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0, 3,  1.5f), glm::vec3(0, 1, 1)),

		VertexFormat(glm::vec3(0, 2,  0.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0.5f, 2,  0.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0.5f, 2,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0, 3,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0, 3,  0.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0.5f, 3,  0.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0.5f, 3,  0), glm::vec3(0, 1, 1)),

		//22
		VertexFormat(glm::vec3(1.5f + 4, 2,  0), glm::vec3(0, 1, 1)),
		//23
		VertexFormat(glm::vec3(1.5f + 4, 2,  0.5f), glm::vec3(0, 1, 1)),
		//23
		VertexFormat(glm::vec3(2 + 4, 2,  0.5f), glm::vec3(0, 1, 1)),
		//25
		VertexFormat(glm::vec3(1.5f + 4, 3,  0), glm::vec3(0, 1, 1)),
		//26
		VertexFormat(glm::vec3(1.5f + 4, 3,  0.5f), glm::vec3(0, 1, 1)),
		//27
		VertexFormat(glm::vec3(2 + 4, 3,  0.5f), glm::vec3(0, 1, 1)),
		//28
		VertexFormat(glm::vec3(2 + 4, 3,  0), glm::vec3(0, 1, 1)),


		VertexFormat(glm::vec3(2 + 4, 2,  1.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(1.5f + 4, 2,  1.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(1.5f + 4, 2,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2 + 4, 3,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2 + 4, 3,  1.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(1.5f + 4, 3,  1.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(1.5f + 4, 3,  2), glm::vec3(0, 1, 1)),

		VertexFormat(glm::vec3(0, 3,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2 + 4, 3,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0, 3.5f,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2 + 4, 3.5f,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0, 3,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2 + 4, 3,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0, 3.5f,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2 + 4, 3.5f,  0), glm::vec3(0, 1, 1)),
	};

	vector<unsigned short> indices6 =
	{
		0, 1, 2,	// indices for first triangle
		1, 3, 2,	// indices for second triangle
		2, 3, 7,
		2, 7, 6,
		1, 7, 3,
		1, 5, 7,
		6, 7, 4,
		7, 5, 4,
		0, 4, 1,
		1, 4, 5,
		2, 6, 4,
		0, 2, 4,

		2,8,11,
		8,12,11,
		10,12,13,
		11,13,14,
		8,13,12,
		8,9,13,
		14,13,10,
		13,9,10,
		2,10,8,
		8,10,9,
		11,14,10,
		2,11,10,

		15,16,19,
		16,20,19,
		19,20,21,
		19,21,18,
		16,21,20,
		16,17,21,
		18,21,6,
		21,17,6,
		15,6,3,
		16,6,17,
		19,18,6,
		15,19,6,


		23,24,26,
		24,27,26,
		26,27,28,
		26,28,25,
		24,28,27,
		24,7,28,
		25,28,22,
		28,7,22,
		23,22,24,
		24,22,7,
		26,25,22,
		23,26,22,

		31,3,35,
		3,32,35,
		35,32,33,
		35,33,34,
		3,33,32,
		3,29,33,
		34,33,30,
		33,29,30,
		31,30,3,
		3,30,29,
		35,34,30,
		31,35,30,

		36,37,38,
		37,39,38,
		38,39,43,
		38,43,42,
		37,43,39,
		37,41,43,
		42,43,40,
		43,41,40,
		36,40,37,
		37,40,41,
		38,42,40,
		36,38,40

	};

	vector<VertexFormat> vertices7
	{
		VertexFormat(glm::vec3(0 - 4, 0,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 0,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0 - 4, 2,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 2,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0, 0,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 0,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0, 2,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 2,  0), glm::vec3(0, 1, 1)),

		VertexFormat(glm::vec3(0.5f, 2,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0.5f, 2,  1.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0, 2,  1.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0, 3,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0.5f, 3,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0.5f, 3,  1.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0, 3,  1.5f), glm::vec3(0, 1, 1)),

		VertexFormat(glm::vec3(0, 2,  0.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0.5f, 2,  0.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0.5f, 2,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0, 3,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0, 3,  0.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0.5f, 3,  0.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0.5f, 3,  0), glm::vec3(0, 1, 1)),

		//22
		VertexFormat(glm::vec3(1.5f, 2,  0), glm::vec3(0, 1, 1)),
		//23
		VertexFormat(glm::vec3(1.5f, 2,  0.5f), glm::vec3(0, 1, 1)),
		//23
		VertexFormat(glm::vec3(2, 2,  0.5f), glm::vec3(0, 1, 1)),
		//25
		VertexFormat(glm::vec3(1.5f, 3,  0), glm::vec3(0, 1, 1)),
		//26
		VertexFormat(glm::vec3(1.5f, 3,  0.5f), glm::vec3(0, 1, 1)),
		//27
		VertexFormat(glm::vec3(2, 3,  0.5f), glm::vec3(0, 1, 1)),
		//28
		VertexFormat(glm::vec3(2, 3,  0), glm::vec3(0, 1, 1)),


		VertexFormat(glm::vec3(2, 2,  1.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(1.5f, 2,  1.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(1.5f, 2,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 3,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 3,  1.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(1.5f, 3,  1.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(1.5f, 3,  2), glm::vec3(0, 1, 1)),

		VertexFormat(glm::vec3(0, 3,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 3,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2, 3,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0, 3,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(1, 4,  1), glm::vec3(0, 1, 1)),
	};

	vector<unsigned short> indices7 =
	{
		0, 1, 2,	// indices for first triangle
		1, 3, 2,	// indices for second triangle
		2, 3, 7,
		2, 7, 6,
		1, 7, 3,
		1, 5, 7,
		6, 7, 4,
		7, 5, 4,
		0, 4, 1,
		1, 4, 5,
		2, 6, 4,
		0, 2, 4,

		2,8,11,
		8,12,11,
		10,12,13,
		11,13,14,
		8,13,12,
		8,9,13,
		14,13,10,
		13,9,10,
		2,10,8,
		8,10,9,
		11,14,10,
		2,11,10,

		15,16,19,
		16,20,19,
		19,20,21,
		19,21,18,
		16,21,20,
		16,17,21,
		18,21,6,
		21,17,6,
		15,6,3,
		16,6,17,
		19,18,6,
		15,19,6,


		23,24,26,
		24,27,26,
		26,27,28,
		26,28,25,
		24,28,27,
		24,7,28,
		25,28,22,
		28,7,22,
		23,22,24,
		24,22,7,
		26,25,22,
		23,26,22,

		31,3,35,
		3,32,35,
		35,32,33,
		35,33,34,
		3,33,32,
		3,29,33,
		34,33,30,
		33,29,30,
		31,30,3,
		3,30,29,
		35,34,30,
		31,35,30,

		36,37,40,
		37,38,40,
		38,39,40,
		36,39,40,

	};

	vector<VertexFormat> vertices8
	{
		VertexFormat(glm::vec3(0, 0,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2 + 4, 0,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0 , 2,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2 + 4, 2,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0 , 0,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2 + 4, 0,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0 , 2,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2 + 4, 2,  0), glm::vec3(0, 1, 1)),

		VertexFormat(glm::vec3(0.5f, 2,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0.5f, 2,  1.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0, 2,  1.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0, 3,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0.5f, 3,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0.5f, 3,  1.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0, 3,  1.5f), glm::vec3(0, 1, 1)),

		VertexFormat(glm::vec3(0, 2,  0.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0.5f, 2,  0.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0.5f, 2,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0, 3,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0, 3,  0.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0.5f, 3,  0.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0.5f, 3,  0), glm::vec3(0, 1, 1)),

		//22
		VertexFormat(glm::vec3(1.5f + 4, 2,  0), glm::vec3(0, 1, 1)),
		//23
		VertexFormat(glm::vec3(1.5f + 4, 2,  0.5f), glm::vec3(0, 1, 1)),
		//23
		VertexFormat(glm::vec3(2 + 4, 2,  0.5f), glm::vec3(0, 1, 1)),
		//25
		VertexFormat(glm::vec3(1.5f + 4, 3,  0), glm::vec3(0, 1, 1)),
		//26
		VertexFormat(glm::vec3(1.5f + 4, 3,  0.5f), glm::vec3(0, 1, 1)),
		//27
		VertexFormat(glm::vec3(2 + 4, 3,  0.5f), glm::vec3(0, 1, 1)),
		//28
		VertexFormat(glm::vec3(2 + 4, 3,  0), glm::vec3(0, 1, 1)),


		VertexFormat(glm::vec3(2 + 4, 2,  1.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(1.5f + 4, 2,  1.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(1.5f + 4, 2,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2 + 4, 3,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2 + 4, 3,  1.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(1.5f + 4, 3,  1.5f), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(1.5f + 4, 3,  2), glm::vec3(0, 1, 1)),

		VertexFormat(glm::vec3(0, 3,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2 + 4, 3,  2), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(2 + 4, 3,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(0, 3,  0), glm::vec3(0, 1, 1)),
		VertexFormat(glm::vec3(1 + 2, 4,  1), glm::vec3(0, 1, 1)),
	};

	vector<unsigned short> indices8 =
	{
		0, 1, 2,	// indices for first triangle
		1, 3, 2,	// indices for second triangle
		2, 3, 7,
		2, 7, 6,
		1, 7, 3,
		1, 5, 7,
		6, 7, 4,
		7, 5, 4,
		0, 4, 1,
		1, 4, 5,
		2, 6, 4,
		0, 2, 4,

		2,8,11,
		8,12,11,
		10,12,13,
		11,13,14,
		8,13,12,
		8,9,13,
		14,13,10,
		13,9,10,
		2,10,8,
		8,10,9,
		11,14,10,
		2,11,10,

		15,16,19,
		16,20,19,
		19,20,21,
		19,21,18,
		16,21,20,
		16,17,21,
		18,21,6,
		21,17,6,
		15,6,3,
		16,6,17,
		19,18,6,
		15,19,6,


		23,24,26,
		24,27,26,
		26,27,28,
		26,28,25,
		24,28,27,
		24,7,28,
		25,28,22,
		28,7,22,
		23,22,24,
		24,22,7,
		26,25,22,
		23,26,22,

		31,3,35,
		3,32,35,
		35,32,33,
		35,33,34,
		3,33,32,
		3,29,33,
		34,33,30,
		33,29,30,
		31,30,3,
		3,30,29,
		35,34,30,
		31,35,30,

		36,37,40,
		37,38,40,
		38,39,40,
		36,39,40,

	};

	vector<glm::vec3> vertices9
	{
		glm::vec3(0,   0, 0),	// Top Right
		glm::vec3(2,  0, 0),	// Bottom Right
		glm::vec3(1,  1, 1),	// Top Left

		glm::vec3(2,   0, 0),	// Top Right
		glm::vec3(2,  0, 2),	// Bottom Right
		glm::vec3(1,  1, 1),	// Top Left

		glm::vec3(0,   0, 2),	// Top Right
		glm::vec3(2,  0, 2),	// Bottom Right
		glm::vec3(1,  1, 1),	// Top Left

		glm::vec3(0,   0, 0),	// Top Right
		glm::vec3(0,  0, 2),	// Bottom Right
		glm::vec3(1,  1, 1),	// Top Left
	};

	vector<glm::vec2> textureindices9
	{
		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 0)


	};

	vector<glm::vec3> normals2
	{
		glm::vec3(0, 1, 1),
		glm::vec3(1, 0, 1),
		glm::vec3(0, 1, 0)
	};

	vector<unsigned short> indices9 =
	{
		0, 1, 2,
		3,4,5,
		6,7,8,
		9,10,11
	};

	vector<glm::vec3> vertices10
	{
		glm::vec3(0,  0, 0),	
		glm::vec3(0,  2, 0),	
		glm::vec3(2,  2, 0),
		glm::vec3(2, 0 , 0),

		glm::vec3(0,  0, 0),
		glm::vec3(0,  2, 0),
		glm::vec3(0,  2, 2),
		glm::vec3(0, 0 , 2),

		glm::vec3(0,0,2),
		glm::vec3(0,2,2),
		glm::vec3(2,2,2),
		glm::vec3(2,0,2),

		glm::vec3(2,0,2),
		glm::vec3(2,2,2),
		glm::vec3(2,2,0),
		glm::vec3(2,0,0),

		glm::vec3(0,   4, 0),	// Top Right
		glm::vec3(2,  4, 0),	// Bottom Right
		glm::vec3(1,  2, 1),	// Top Left

		glm::vec3(2,   4, 0),	// Top Right
		glm::vec3(2,  4, 2),	// Bottom Right
		glm::vec3(1,  2, 1),	// Top Left

		glm::vec3(0,  4, 2),	// Top Right
		glm::vec3(2,  4, 2),	// Bottom Right
		glm::vec3(1,  2, 1),	// Top Left

		glm::vec3(0,   4, 0),	// Top Right
		glm::vec3(0,  4, 2),	// Bottom Right
		glm::vec3(1,  2, 1),	// Top Left

		glm::vec3(0,2,0),
		glm::vec3(2,2,0),
		glm::vec3(2,2,2),
		glm::vec3(0,2,2),

		glm::vec3(0,4,0),
		glm::vec3(2,4,0),
		glm::vec3(2,4,2),
		glm::vec3(0,4,2),
		
	};

	vector<glm::vec2> textureindices10
	{
		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),


		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

	};

	vector<glm::vec3> normals3
	{
		glm::vec3(0, 1, 1),
		glm::vec3(1, 0, 1),
		glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0),

	};

	vector<unsigned short> indices10 =
	{
		0, 1, 3,
		1,2,3,

		4,5,6,
		6,7,4,

		8,9,10,
		10,11,8,

		12,13,14,
		14,15,12,

		16,17,18,

		19,20,21,

		22,23,24,

		25,26,27,

		28,29,30,
		30,31,28,

		32,33,34,
		34,35,32
	};

	vector<glm::vec3> vertices11
	{
		glm::vec3(0,  0, 0),
		glm::vec3(0,  2, 0),
		glm::vec3(2,  2, 0),
		glm::vec3(2, 0 , 0),

		glm::vec3(0,  0, 0),
		glm::vec3(0,  2, 0),
		glm::vec3(0,  2, 2),
		glm::vec3(0, 0 , 2),

		glm::vec3(0,0,2),
		glm::vec3(0,2,2),
		glm::vec3(2,2,2),
		glm::vec3(2,0,2),

		glm::vec3(2,0,2),
		glm::vec3(2,2,2),
		glm::vec3(2,2,0),
		glm::vec3(2,0,0),

		glm::vec3(0,   2, 0),	// Top Right
		glm::vec3(2,  2, 0),	// Bottom Right
		glm::vec3(1,  4, 1),	// Top Left

		glm::vec3(2,   2, 0),	// Top Right
		glm::vec3(2,  2, 2),	// Bottom Right
		glm::vec3(1,  4, 1),	// Top Left

		glm::vec3(0,   2, 2),	// Top Right
		glm::vec3(2,  2, 2),	// Bottom Right
		glm::vec3(1,  4, 1),	// Top Left

		glm::vec3(0,  2, 0),	// Top Right
		glm::vec3(0,  2, 2),	// Bottom Right
		glm::vec3(1,  4, 1),	// Top Left


	};

	vector<glm::vec2> textureindices11
	{
		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),


		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),


	};

	vector<glm::vec3> normals4
	{
		glm::vec3(0, 1, 1),
		glm::vec3(1, 0, 1),
		glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0),

	};

	vector<unsigned short> indices11 =
	{
		0, 1, 3,
		1,2,3,

		4,5,6,
		6,7,4,

		8,9,10,
		10,11,8,

		12,13,14,
		14,15,12,

		16,17,18,

		19,20,21,

		22,23,24,

		25,26,27,


	};

	vector<glm::vec3> vertices12
	{
		glm::vec3(0,  0, 0),
		glm::vec3(0,  2, 0),
		glm::vec3(4,  2, 0),
		glm::vec3(4, 0 , 0),

		glm::vec3(0,  0, 2 ),
		glm::vec3(0,  2, 2),
		glm::vec3(4,  2, 2),
		glm::vec3(4, 0 , 2),

		glm::vec3(0,  0, 0),
		glm::vec3(0,  0, 2),
		glm::vec3(0,  2, 2),
		glm::vec3(0, 2 , 0),

		glm::vec3(4,  0, 0),
		glm::vec3(4,  0, 2),
		glm::vec3(4,  2, 2),
		glm::vec3(4, 2 , 0),

		glm::vec3(0,  2, 0),
		glm::vec3(0,  2, 2),
		glm::vec3(4,  2, 2),
		glm::vec3(4, 2 , 0),

		// bordura1

		glm::vec3(3.5f,  2, 2),
		glm::vec3(3.5f,  3.5f, 2),
		glm::vec3(4,  3.5f, 2),
		glm::vec3(4,  2, 2),

		glm::vec3(3.5f,  2, 1.5f),
		glm::vec3(3.5f,  3.5f, 1.5f),
		glm::vec3(4,  3.5f, 1.5f),
		glm::vec3(4,  2, 1.5f),

		glm::vec3(3.5f,  2, 1.5f),
		glm::vec3(3.5f,  3.5f, 1.5f),
		glm::vec3(3.5f,  3.5f, 2),
		glm::vec3(3.5f,  2, 2),

		glm::vec3(4,  2, 1.5f),
		glm::vec3(4,  3.5f, 1.5f),
		glm::vec3(4,  3.5f, 2),
		glm::vec3(4,  2, 2),

		//bordura2

		glm::vec3(0,  2, 2),
		glm::vec3(0,  3.5f, 2),
		glm::vec3(0.5f,  3.5f, 2),
		glm::vec3(0.5f,  2, 2),

		glm::vec3(0,  2, 1.5f),
		glm::vec3(0,  3.5f, 1.5f),
		glm::vec3(0.5f,  3.5f, 1.5f),
		glm::vec3(0.5f,  2, 1.5f),

		glm::vec3(0,  2, 1.5f),
		glm::vec3(0,  3.5f, 1.5f),
		glm::vec3(0, 3.5f, 2),
		glm::vec3(0,  2, 2),

		glm::vec3(0.5f,  2, 1.5f),
		glm::vec3(0.5f,  3.5f, 1.5f),
		glm::vec3(0.5f,  3.5f, 2),
		glm::vec3(0.5f,  2, 2),

		// bordura3

		glm::vec3(0,  2, 0.5f),
		glm::vec3(0,  3.5f, 0.5f),
		glm::vec3(0.5f,  3.5f, 0.5f),
		glm::vec3(0.5f,  2, 0.5f),

		glm::vec3(0,  2, 0),
		glm::vec3(0,  3.5f, 0),
		glm::vec3(0.5f,  3.5f, 0),
		glm::vec3(0.5f,  2, 0),

		glm::vec3(0,  2, 0),
		glm::vec3(0,  3.5f, 0),
		glm::vec3(0,  3.5f, 0.5f),
		glm::vec3(0,  2, 0.5f),

		glm::vec3(0.5f,  2, 0),
		glm::vec3(0.5f,  3.5f, 0),
		glm::vec3(0.5f,  3.5f, 0.5f),
		glm::vec3(0.5f,  2, 0.5f),

		// bordura4

		glm::vec3(3.5f,  2, 0.5f),
		glm::vec3(3.5f,  3.5f, 0.5f),
		glm::vec3(4,  3.5f, 0.5f),
		glm::vec3(4,  2, 0.5f),

		glm::vec3(3.5f,  2, 0),
		glm::vec3(3.5f,  3.5f, 0),
		glm::vec3(4,  3.5f, 0),
		glm::vec3(4,  2, 0),

		glm::vec3(3.5f,  2, 0),
		glm::vec3(3.5f,  3.5f, 0),
		glm::vec3(3.5f,  3.5f, 0.5f),
		glm::vec3(3.5f,  2, 0.5f),

		glm::vec3(4,  2, 0),
		glm::vec3(4,  3.5f, 0),
		glm::vec3(4,  3.5f, 0.5f),
		glm::vec3(4,  2, 0.5f),

		// roof

			glm::vec3(0, 3.5f, 0),
			glm::vec3(0, 4, 0),
			glm::vec3(4, 4, 0),
			glm::vec3(4, 3.5f, 0),

			glm::vec3(0, 3.5f, 2),
			glm::vec3(0, 4, 2),
			glm::vec3(4, 4, 2),
			glm::vec3(4, 3.5f, 2),

			glm::vec3(0, 3.5f, 0),
			glm::vec3(0, 3.5f, 2),
			glm::vec3(0, 4, 2),
			glm::vec3(0, 4, 0),

			glm::vec3(4, 3.5f, 0),
			glm::vec3(4, 3.5f, 2),
			glm::vec3(4, 4, 2),
			glm::vec3(4, 4, 0),

			glm::vec3(0, 4, 0),
			glm::vec3(0, 4, 2),
			glm::vec3(4, 4, 2),
			glm::vec3(4, 4, 0),

	};

	vector<glm::vec2> textureindices12
	{
		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		// bordura2

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),


		// bordura3

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		// bordura4

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		// roof

			glm::vec2(0, 0),
			glm::vec2(0, 1),
			glm::vec2(1, 1),
			glm::vec2(1, 0),

			glm::vec2(0, 0),
			glm::vec2(0, 1),
			glm::vec2(1, 1),
			glm::vec2(1, 0),

			glm::vec2(0, 0),
			glm::vec2(0, 1),
			glm::vec2(1, 1),
			glm::vec2(1, 0),

			glm::vec2(0, 0),
			glm::vec2(0, 1),
			glm::vec2(1, 1),
			glm::vec2(1, 0),

			glm::vec2(0, 0),
			glm::vec2(0, 1),
			glm::vec2(1, 1),
			glm::vec2(1, 0),
	};

	vector<glm::vec3> normals5
	{
		glm::vec3(0, 1, 1),
		glm::vec3(1, 0, 1),
		glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0),

	};

	vector<unsigned short> indices12 =
	{
		0, 1, 3,
		1,2,3,

		4,5,6,
		6,7,4,

		8,9,10,
		10,11,8,

		12,13,14,
		14,15,12,

		16,17,18,
		18,19,16,

		20,21,22,
		22,23,20,

		24,25,26,
		26,27,24,

		28,29,30,
		30,31,28,

		32,33,34,
		34,35,32,

		36,37,38,
		38,39,36,

		40,41,42,
		42,43,40,

		44,45,46,
		46,47,44,

		48,49,50,
		50,51,48,

		52,53,54,
		54,55,52,

		56,57,58,
		58,59,56,

		60,61,62,
		62,63,60,

		64,65,66,
		66,67,65,


		68,69,70,
		70,71,68,

		72,73,74,
		74,75,72,

		76,77,78,
		78,79,76,

		80,81,82,
		82,83,80,


		84,85,87,
		85,86,87,

		88,89,90,
		90,91,88,

		92,93,94,
		94,95,92,

		96,97,98,
		98,99,96,

		100,101,102,
		102,103,100,



	};


	vector<glm::vec3> vertices13
	{
		glm::vec3(0,  0, 0),
		glm::vec3(0,  2, 0),
		glm::vec3(4,  2, 0),
		glm::vec3(4, 0 , 0),

		glm::vec3(0,  0, 2),
		glm::vec3(0,  2, 2),
		glm::vec3(4,  2, 2),
		glm::vec3(4, 0 , 2),

		glm::vec3(0,  0, 0),
		glm::vec3(0,  0, 2),
		glm::vec3(0,  2, 2),
		glm::vec3(0, 2 , 0),

		glm::vec3(4,  0, 0),
		glm::vec3(4,  0, 2),
		glm::vec3(4,  2, 2),
		glm::vec3(4, 2 , 0),

		glm::vec3(0,  2, 0),
		glm::vec3(0,  2, 2),
		glm::vec3(4,  2, 2),
		glm::vec3(4, 2 , 0),

		// bordura1

		glm::vec3(3.5f,  2, 2),
		glm::vec3(3.5f,  3.5f, 2),
		glm::vec3(4,  3.5f, 2),
		glm::vec3(4,  2, 2),

		glm::vec3(3.5f,  2, 1.5f),
		glm::vec3(3.5f,  3.5f, 1.5f),
		glm::vec3(4,  3.5f, 1.5f),
		glm::vec3(4,  2, 1.5f),

		glm::vec3(3.5f,  2, 1.5f),
		glm::vec3(3.5f,  3.5f, 1.5f),
		glm::vec3(3.5f,  3.5f, 2),
		glm::vec3(3.5f,  2, 2),

		glm::vec3(4,  2, 1.5f),
		glm::vec3(4,  3.5f, 1.5f),
		glm::vec3(4,  3.5f, 2),
		glm::vec3(4,  2, 2),

		//bordura2

		glm::vec3(0,  2, 2),
		glm::vec3(0,  3.5f, 2),
		glm::vec3(0.5f,  3.5f, 2),
		glm::vec3(0.5f,  2, 2),

		glm::vec3(0,  2, 1.5f),
		glm::vec3(0,  3.5f, 1.5f),
		glm::vec3(0.5f,  3.5f, 1.5f),
		glm::vec3(0.5f,  2, 1.5f),

		glm::vec3(0,  2, 1.5f),
		glm::vec3(0,  3.5f, 1.5f),
		glm::vec3(0, 3.5f, 2),
		glm::vec3(0,  2, 2),

		glm::vec3(0.5f,  2, 1.5f),
		glm::vec3(0.5f,  3.5f, 1.5f),
		glm::vec3(0.5f,  3.5f, 2),
		glm::vec3(0.5f,  2, 2),

		// bordura3

		glm::vec3(0,  2, 0.5f),
		glm::vec3(0,  3.5f, 0.5f),
		glm::vec3(0.5f,  3.5f, 0.5f),
		glm::vec3(0.5f,  2, 0.5f),

		glm::vec3(0,  2, 0),
		glm::vec3(0,  3.5f, 0),
		glm::vec3(0.5f,  3.5f, 0),
		glm::vec3(0.5f,  2, 0),

		glm::vec3(0,  2, 0),
		glm::vec3(0,  3.5f, 0),
		glm::vec3(0,  3.5f, 0.5f),
		glm::vec3(0,  2, 0.5f),

		glm::vec3(0.5f,  2, 0),
		glm::vec3(0.5f,  3.5f, 0),
		glm::vec3(0.5f,  3.5f, 0.5f),
		glm::vec3(0.5f,  2, 0.5f),

		// bordura4

		glm::vec3(3.5f,  2, 0.5f),
		glm::vec3(3.5f,  3.5f, 0.5f),
		glm::vec3(4,  3.5f, 0.5f),
		glm::vec3(4,  2, 0.5f),

		glm::vec3(3.5f,  2, 0),
		glm::vec3(3.5f,  3.5f, 0),
		glm::vec3(4,  3.5f, 0),
		glm::vec3(4,  2, 0),

		glm::vec3(3.5f,  2, 0),
		glm::vec3(3.5f,  3.5f, 0),
		glm::vec3(3.5f,  3.5f, 0.5f),
		glm::vec3(3.5f,  2, 0.5f),

		glm::vec3(4,  2, 0),
		glm::vec3(4,  3.5f, 0),
		glm::vec3(4,  3.5f, 0.5f),
		glm::vec3(4,  2, 0.5f),

		// roof

			glm::vec3(0, 3.5f, 0),
			glm::vec3(0, 4, 0),
			glm::vec3(4, 4, 0),
			glm::vec3(4, 3.5f, 0),

			glm::vec3(0, 3.5f, 2),
			glm::vec3(0, 4, 2),
			glm::vec3(4, 4, 2),
			glm::vec3(4, 3.5f, 2),

			glm::vec3(0, 3.5f, 0),
			glm::vec3(0, 3.5f, 2),
			glm::vec3(0, 4, 2),
			glm::vec3(0, 4, 0),

			glm::vec3(4, 3.5f, 0),
			glm::vec3(4, 3.5f, 2),
			glm::vec3(4, 4, 2),
			glm::vec3(4, 4, 0),

			glm::vec3(0, 4, 0),
			glm::vec3(0, 4, 2),
			glm::vec3(4, 4, 2),
			glm::vec3(4, 4, 0),
			
			// roof2

			glm::vec3(0, 4, 2),
			glm::vec3(4, 4, 2),
			glm::vec3(2, 5, 1),

			glm::vec3(0, 4, 0),
			glm::vec3(4, 4, 0),
			glm::vec3(2, 5, 1),

			glm::vec3(0, 4, 0),
			glm::vec3(0, 4, 2),
			glm::vec3(2, 5, 1),

			glm::vec3(4, 4, 0),
			glm::vec3(4, 4, 2),
			glm::vec3(2, 5, 1),
			

	};

	vector<glm::vec2> textureindices13
	{
		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		// bordura2

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),


		// bordura3

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		// bordura4

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		// roof

			glm::vec2(0, 0),
			glm::vec2(0, 1),
			glm::vec2(1, 1),
			glm::vec2(1, 0),

			glm::vec2(0, 0),
			glm::vec2(0, 1),
			glm::vec2(1, 1),
			glm::vec2(1, 0),

			glm::vec2(0, 0),
			glm::vec2(0, 1),
			glm::vec2(1, 1),
			glm::vec2(1, 0),

			glm::vec2(0, 0),
			glm::vec2(0, 1),
			glm::vec2(1, 1),
			glm::vec2(1, 0),

			glm::vec2(0, 0),
			glm::vec2(0, 1),
			glm::vec2(1, 1),
			glm::vec2(1, 0),

		// roof2

			glm::vec2(0, 0),
			glm::vec2(0, 1),
			glm::vec2(1, 1),

			glm::vec2(0, 0),
			glm::vec2(0, 1),
			glm::vec2(1, 1),

			glm::vec2(0, 0),
			glm::vec2(0, 1),
			glm::vec2(1, 1),

			glm::vec2(0, 0),
			glm::vec2(0, 1),
			glm::vec2(1, 1),

	};

	vector<glm::vec3> normals6
	{
		glm::vec3(0, 1, 1),
		glm::vec3(1, 0, 1),
		glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0),

	};

	vector<unsigned short> indices13 =
	{
		0, 1, 3,
		1,2,3,

		4,5,6,
		6,7,4,

		8,9,10,
		10,11,8,

		12,13,14,
		14,15,12,

		16,17,18,
		18,19,16,

		20,21,22,
		22,23,20,

		24,25,26,
		26,27,24,

		28,29,30,
		30,31,28,

		32,33,34,
		34,35,32,

		36,37,38,
		38,39,36,

		40,41,42,
		42,43,40,

		44,45,46,
		46,47,44,

		48,49,50,
		50,51,48,

		52,53,54,
		54,55,52,

		56,57,58,
		58,59,56,

		60,61,62,
		62,63,60,

		64,65,66,
		66,67,65,


		68,69,70,
		70,71,68,

		72,73,74,
		74,75,72,

		76,77,78,
		78,79,76,

		80,81,82,
		82,83,80,


		84,85,87,
		85,86,87,

		88,89,90,
		90,91,88,

		92,93,94,
		94,95,92,

		96,97,98,
		98,99,96,

		100,101,102,
		102,103,100,

		104,105,106,

		107,108,109,

		110,111,112,

		113,114,115


	};

	vector<glm::vec3> vertices14
	{
		glm::vec3(0,  0, 0),
		glm::vec3(0,  5, 0),
		glm::vec3(1,  5, 0),
		glm::vec3(1,  0, 0),


		glm::vec3(0,  0, 1),
		glm::vec3(0,  5, 1),
		glm::vec3(1,  5, 1),
		glm::vec3(1,  0, 1),

		glm::vec3(0,  0, 0),
		glm::vec3(0,  5, 0),
		glm::vec3(0,  5, 1),
		glm::vec3(0,  0, 1),

		glm::vec3(1,  0, 0),
		glm::vec3(1,  5, 0),
		glm::vec3(1,  5, 1),
		glm::vec3(1,  0, 1),

		glm::vec3(0,  5, 1),
		glm::vec3(0,  6, 1),
		glm::vec3(14,  6, 1),
		glm::vec3(14,  5, 1),

		glm::vec3(0,  5, 0),
		glm::vec3(0,  6, 0),
		glm::vec3(14,  6, 0),
		glm::vec3(14,  5, 0),

		glm::vec3(0,  5, 0),
		glm::vec3(0,  6, 0),
		glm::vec3(0,  6, 1),
		glm::vec3(0,  5, 1),

		glm::vec3(14,  5, 0),
		glm::vec3(14,  6, 0),
		glm::vec3(14,  6, 1),
		glm::vec3(14,  5, 1),

		glm::vec3(13,  0, 1),
		glm::vec3(13,  5, 1),
		glm::vec3(14,  5, 1),
		glm::vec3(14,  0, 1),

		glm::vec3(13,  0, 0),
		glm::vec3(13,  5, 0),
		glm::vec3(14,  5, 0),
		glm::vec3(14,  0, 0),

		glm::vec3(13,  0, 0),
		glm::vec3(13,  5, 0),
		glm::vec3(13,  5, 1),
		glm::vec3(13,  0, 1),

		glm::vec3(14,  0, 0),
		glm::vec3(14,  5, 0),
		glm::vec3(14,  5, 1),
		glm::vec3(14,  0, 1),

		glm::vec3(0,  6, 0),
		glm::vec3(0,  6, 1),
		glm::vec3(14,  6, 1),
		glm::vec3(14,  6, 0),
		
		glm::vec3(0,  5, 0),
		glm::vec3(0,  5, 1),
		glm::vec3(14,  5, 1),
		glm::vec3(14,  5, 0),

	};

	vector<glm::vec2> textureindices14
	{
		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 0),
		glm::vec2(1, 1),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),

		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),
		

	};

	vector<glm::vec3> normals7
	{
		glm::vec3(0, 1, 1),
		glm::vec3(1, 0, 1),
		glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0),

	};

	vector<unsigned short> indices14 =
	{
		
		  

		4,5,6,
		6,7,4,

		8,9,10,
		10,11,8,

		12,13,14,
		14,15,12,

		16,17,18,
		18,19,16,

		20,21,22,
		22,23,20,

		24,25,26,
		26,27,24,

		28,29,30,
		30,31,28,

		32,33,34,
		34,35,32,

		36,37,38,
		38,39,36,

		40,41,42,
		42,43,40,

		44,45,46,
		46,47,44,

		48,49,50,
		50,51,48,

		52,53,54,
		54,55,52
	};



	meshes["cube1"] = new Mesh("generated cube 1");
	meshes["cube1"]->InitFromData(vertices, indices);

	Mesh* cube = CreateMesh("piramida1", vertices, indices);

	meshes["cube2"] = new Mesh("generated cube 2");
	meshes["cube2"]->InitFromData(vertices2, indices2);

	Mesh* cube2 = CreateMesh("piramida2", vertices2, indices2);

	meshes["cube3"] = new Mesh("generated cube 3");
	meshes["cube3"]->InitFromData(vertices3, indices3);

	Mesh* cube3 = CreateMesh("piramida3", vertices3, indices3);

	meshes["cube4"] = new Mesh("generated cube 4");
	meshes["cube4"]->InitFromData(vertices4, indices4);

	Mesh* cube4 = CreateMesh("piramida4", vertices4, indices4);

	meshes["cube5"] = new Mesh("generated cube 5");
	meshes["cube5"]->InitFromData(vertices5, indices5);

	Mesh* cube5 = CreateMesh("piramida5", vertices5, indices5);

	meshes["cube6"] = new Mesh("generated cube 6");
	meshes["cube6"]->InitFromData(vertices6, indices6);

	Mesh* cube6 = CreateMesh("piramida6", vertices6, indices6);

	meshes["cube7"] = new Mesh("generated cube 7");
	meshes["cube7"]->InitFromData(vertices8, indices8);

	Mesh* cube7 = CreateMesh("piramida7", vertices8, indices8);

	Mesh* mesh9 = new Mesh("piramida_texturata");
	mesh9->InitFromData(vertices9, normals2, textureindices9, indices9);
	meshes[mesh9->GetMeshID()] = mesh9;

	Mesh* mesh10 = new Mesh("textura2");
	mesh10->InitFromData(vertices10, normals3, textureindices10, indices10);
	meshes[mesh10->GetMeshID()] = mesh10;

	Mesh* mesh11 = new Mesh("textura3");
	mesh11->InitFromData(vertices11, normals4, textureindices11, indices11);
	meshes[mesh11->GetMeshID()] = mesh11;

	Mesh* mesh12 = new Mesh("textura4");
	mesh12->InitFromData(vertices12, normals5, textureindices12, indices12);
	meshes[mesh12->GetMeshID()] = mesh12;

	Mesh* mesh13 = new Mesh("textura5");
	mesh13->InitFromData(vertices13, normals6, textureindices13, indices13);
	meshes[mesh13->GetMeshID()] = mesh13;

	Mesh* mesh14 = new Mesh("textura6");
	mesh14->InitFromData(vertices14, normals7, textureindices14, indices14);
	meshes[mesh14->GetMeshID()] = mesh14;

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
		Mesh* mesh = new Mesh("plane");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "plane50.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	

	// LOADING TEXTURES 

	const string textureLoc = "Source/Laboratoare/Tema3/Textures/";

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "crate.jpg").c_str(), GL_REPEAT);
		mapTextures["crate"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "sand.jpg").c_str(), GL_REPEAT);
		mapTextures["sand"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "piramida.jpg").c_str(), GL_REPEAT);
		mapTextures["piramida"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "water.png").c_str(), GL_REPEAT);
		mapTextures["water"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "earth.png").c_str(), GL_REPEAT);
		mapTextures["earth"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "redsmile.png").c_str(), GL_REPEAT);
		mapTextures["redsmile"] = texture;
	}

	/*{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "moon1.jpg").c_str(), GL_REPEAT);
		mapTextures["moon1"] = texture;
	}*/

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "moon2.jpg").c_str(), GL_REPEAT);
		mapTextures["moon2"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "8ball.png").c_str(), GL_REPEAT);
		mapTextures["8ball"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "badcrate.jpg").c_str(), GL_REPEAT);
		mapTextures["cub2"] = texture;
	}


	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "XP.png").c_str(), GL_REPEAT);
		mapTextures["XP"] = texture;
	}
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "windowslogo.png").c_str(), GL_REPEAT);
		mapTextures["windowslogo"] = texture;
	}


	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "redcrate.jpg").c_str(), GL_REPEAT);
		mapTextures["redcrate"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "yellowcrate.jpg").c_str(), GL_REPEAT);
		mapTextures["yellowcrate"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "movcrate.jpg").c_str(), GL_REPEAT);
		mapTextures["movcrate"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "greencrate.jpg").c_str(), GL_REPEAT);
		mapTextures["greencrate"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "orangecrate.jpg").c_str(), GL_REPEAT);
		mapTextures["orangecrate"] = texture;
	}

	// Create a simple quad
	{
		vector<glm::vec3> vertices
		{
			glm::vec3(0.5f,   0.5f, 0.0f),	// Top Right
			glm::vec3(0.5f,  -0.5f, 0.0f),	// Bottom Right
			glm::vec3(-0.5f, -0.5f, 0.0f),	// Bottom Left
			glm::vec3(-0.5f,  0.5f, 0.0f),	// Top Left
		};

		vector<glm::vec3> normals
		{
			glm::vec3(0, 1, 1),
			glm::vec3(1, 0, 1),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0)
		};

		// TODO : Complete texture coordinates for the square
		vector<glm::vec2> textureCoords
		{
			glm::vec2(0, 0),
			glm::vec2(0, 1),
			glm::vec2(1, 1),
			glm::vec2(1, 0)

		};

		vector<unsigned short> indices =
		{
			0, 1, 3,
			1, 2, 3
		};

		Mesh* mesh = new Mesh("square");
		mesh->InitFromData(vertices, normals, textureCoords, indices);
		meshes[mesh->GetMeshID()] = mesh;
	}

	// LOADING SHADERS

	{
		Shader* shader = new Shader("ShaderLab9");
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Shader* shader = new Shader("VertexColor2");
		shader->AddShader(RESOURCE_PATH::SHADERS + "MVP.Texture.VS.glsl", GL_VERTEX_SHADER);
		shader->AddShader(RESOURCE_PATH::SHADERS + "VertexColor2.FS.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Shader* shader = new Shader("VertexColor3");
		shader->AddShader(RESOURCE_PATH::SHADERS + "MVP.Texture.VS.glsl", GL_VERTEX_SHADER);
		shader->AddShader(RESOURCE_PATH::SHADERS + "VertexColor3.FS.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Shader* shader = new Shader("Noise");
		shader->AddShader(RESOURCE_PATH::SHADERS + "Noise.VS.glsl", GL_VERTEX_SHADER);
		shader->AddShader(RESOURCE_PATH::SHADERS + "Noise.FS.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Shader* shader = new Shader("UI");
		shader->AddShader(RESOURCE_PATH::SHADERS + "UI.VS.glsl", GL_VERTEX_SHADER);
		shader->AddShader(RESOURCE_PATH::SHADERS + "UI.FS.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Shader* shader = new Shader("UI2");
		shader->AddShader(RESOURCE_PATH::SHADERS + "UI2.VS.glsl", GL_VERTEX_SHADER);
		shader->AddShader(RESOURCE_PATH::SHADERS + "UI2.FS.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Shader* shader = new Shader("UI3");
		shader->AddShader(RESOURCE_PATH::SHADERS + "UI2.VS.glsl", GL_VERTEX_SHADER);
		shader->AddShader(RESOURCE_PATH::SHADERS + "UI3.FS.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Shader* shader = new Shader("SpeedPowerup");
		shader->AddShader(RESOURCE_PATH::SHADERS + "UI.VS.glsl", GL_VERTEX_SHADER);
		shader->AddShader(RESOURCE_PATH::SHADERS + "SpeedPowerup.FS.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Shader* shader = new Shader("SpeedPowerup2");
		shader->AddShader(RESOURCE_PATH::SHADERS + "UI.VS.glsl", GL_VERTEX_SHADER);
		shader->AddShader(RESOURCE_PATH::SHADERS + "SpeedPowerup2.FS.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Shader* shader = new Shader("Light");
		shader->AddShader(RESOURCE_PATH::SHADERS + "VertexShaderLight.glsl", GL_VERTEX_SHADER);
		shader->AddShader(RESOURCE_PATH::SHADERS + "FragmentShaderLight.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	//Light & material properties
	{
		lightPosition = glm::vec3(-21.5f , 14, -58);
		lightDirection = glm::vec3(0, 0, -1);
		materialShininess = 60;
		materialKd = 30;
		materialKs = 30;
		isSpotlight = 0;
	}

}

void Tema3::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema3::DrawPlayer(float deltaTimeSeconds)
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
			{
				if (player_skin == 1)
					RenderTextureMesh(meshes["sphere"], shaders["ShaderLab9"], modelMatrix4, mapTextures["earth"]);
				if (player_skin == 2)
					RenderTextureMesh(meshes["sphere"], shaders["ShaderLab9"], modelMatrix4, mapTextures["piramida"]);
				if (player_skin == 3)
					RenderTextureMesh(meshes["sphere"], shaders["ShaderLab9"], modelMatrix4, mapTextures["8ball"]);
			}
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
				
				if (1.5f + viteza_bila_verticala >= 2.5f)
				{
					jumping_state = 2;
					viteza_bila_verticala = 0;
					infinite_jump = 1;
					//modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0, 2.5f, 0));
				}
				modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0, viteza_bila_verticala, 0));
			}
			if (infinite_jump >= 1 && infinite_jump < 50)
			{
				//cout << infinite_jump << endl;
				modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0, 1, 0));
				if (infinite_jump == 49)
					modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0, -1, 0));
				infinite_jump++;
			}
			if (jumping_state == 2 && infinite_jump == 50)
			{
				viteza_bila_verticala += 0.1f;
				modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0, 1 - viteza_bila_verticala, 0));
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
			if(jumping_state == 0)
			modelMatrixPlayer = glm::rotate(modelMatrixPlayer, RADIANS(-angularStep * 60), glm::vec3(1, 0, 0));
			if (camera_position == 0)
			{
				if (speed_powerup == 0 && contor_deformare == 0)
				{
					if (player_skin == 1)
						RenderTextureMesh(meshes["sphere"], shaders["ShaderLab9"], modelMatrixPlayer, mapTextures["earth"]);
					if (player_skin == 2)
						RenderTextureMesh(meshes["sphere"], shaders["ShaderLab9"], modelMatrixPlayer, mapTextures["piramida"]);
					if (player_skin == 3)
						RenderTextureMesh(meshes["sphere"], shaders["ShaderLab9"], modelMatrixPlayer, mapTextures["8ball"]);
				}
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

				if (1.5f + viteza_bila_verticala >= 2.5f)
				{
					jumping_state = 2;
					viteza_bila_verticala = 0;
					infinite_jump = 1;
					//modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0, 2.5f, 0));
				}
				modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0, viteza_bila_verticala, 0));
			}
			if (infinite_jump >= 1 && infinite_jump < 50)
			{
				//cout << infinite_jump << endl;
				modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0, 1, 0));
				if (infinite_jump == 49)
					modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0, -1, 0));
				infinite_jump++;
			}
			if (jumping_state == 2 && infinite_jump == 50)
			{
				//cout << "VITEZA ESTE" << viteza_bila_verticala << endl;
				viteza_bila_verticala += 0.1f;
				modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0, 1 - viteza_bila_verticala, 0));
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
			if(jumping_state == 0)
			modelMatrixPlayer = glm::rotate(modelMatrixPlayer, RADIANS(- angularStep * 60),glm::vec3(1,0,0));
			if (camera_position == 0)
			{
				if (speed_powerup == 0 && contor_deformare == 0)
				{
					if (player_skin == 1)
						RenderTextureMesh(meshes["sphere"], shaders["ShaderLab9"], modelMatrixPlayer, mapTextures["earth"]);
					if (player_skin == 2)
						RenderTextureMesh(meshes["sphere"], shaders["ShaderLab9"], modelMatrixPlayer, mapTextures["piramida"]);
					if (player_skin == 3)
						RenderTextureMesh(meshes["sphere"], shaders["ShaderLab9"], modelMatrixPlayer, mapTextures["8ball"]);
				}
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
				
				if (1.5f + viteza_bila_verticala >= 2.5f)
				{
					jumping_state = 2;
					viteza_bila_verticala = 0;
					infinite_jump = 1;
					//modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0, 2.5f, 0));
				}
				modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0, viteza_bila_verticala, 0));
			}
			if (infinite_jump >= 1 && infinite_jump < 50)
			{
				//cout << infinite_jump << endl;
				modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0, 1, 0));
				if(infinite_jump == 49)
				modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0, -1, 0));
				infinite_jump++;
			}
			if (jumping_state == 2 && infinite_jump == 50)
			{
				//cout << "VITEZA ESTE" << viteza_bila_verticala << endl;
				viteza_bila_verticala += 0.1f;
				modelMatrixPlayer = glm::translate(modelMatrixPlayer, glm::vec3(0, 1 - viteza_bila_verticala, 0));
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
			if (jumping_state == 0)
			modelMatrixPlayer = glm::rotate(modelMatrixPlayer, RADIANS(-angularStep * 60), glm::vec3(1, 0, 0));
			if (camera_position == 0)
			{
				if (speed_powerup == 0 && contor_deformare == 0)
				{
					if (player_skin == 1)
						RenderTextureMesh(meshes["sphere"], shaders["ShaderLab9"], modelMatrixPlayer, mapTextures["earth"]);
					if (player_skin == 2)
						RenderTextureMesh(meshes["sphere"], shaders["ShaderLab9"], modelMatrixPlayer, mapTextures["piramida"]);
					if (player_skin == 3)
						RenderTextureMesh(meshes["sphere"], shaders["ShaderLab9"], modelMatrixPlayer, mapTextures["8ball"]);
				}
				if (speed_powerup > 0 || contor_deformare > 0)
					RenderSpecialMesh(meshes["sphere"], shaders["Noise"], modelMatrixPlayer, factor_deformare);
				//RenderMesh(meshes["sphere"], shaders["VertexColor2"], modelMatrixPlayer);
			}
		}

	}
}

void Tema3::DrawPlatforms()
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

		if (change_platforms == 1)
		{
			for (int j = contor_pozitie_sfera + 1; j < contor_pozitie_sfera + 10 ; j++)
			{
				if (random_generate_platforms[j] == 1)
					random_generate_platforms[j] = 5;
				if (random_generate_platforms[j] == 2)
					random_generate_platforms[j] = 6;
				if (random_generate_platforms[j] == 3)
					random_generate_platforms[j] = 7;
				if (random_generate_platforms[j] == 4)
					random_generate_platforms[j] = 8;
				special_platform_indices[j] = 4;
				special_platform_type[j] = 4;
			}
			change_platforms = 0;
		}

		if (z_platforma[i] == 0)
		{
			//z_platforma[i] = rand() % 3 ;
			z_platforma[i] = rand() % 5;
		}

		if (pickups_location_id[i] == 0 && i % 5 == 0)
		{
			pickups_location_id[i] = 1;
		}

		if (box_location_id[i] == 0 && i % 3 == 0 && i != 0)
		{
			box_location_id[i] = 1;
		}
		

		if (random_generate_platforms[i] == 1)
		{
			glm::mat4 modelMatrixPickup = glm::mat4(1);
			glm::mat4 obstacle = glm::mat4(1);
			
			
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

				if (box_location_id[i] == 1)
				{
					//cout << "SAL" << endl;
					if (box_location_id_position[i] == 1)
					{
						if (camera_position == 0)
							obstacle = glm::translate(obstacle, glm::vec3(0 - 1.25f, 1.5f, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							obstacle = glm::translate(obstacle, glm::vec3(0 - 1.25f, 1.5f + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								obstacle = glm::translate(obstacle, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					else if (box_location_id_position[i] == 2)
					{
						if (camera_position == 0)
							obstacle = glm::translate(obstacle, glm::vec3(0, 1.5f, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							obstacle = glm::translate(obstacle, glm::vec3(0, 1.5f + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								obstacle = glm::translate(obstacle, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					else if (box_location_id_position[i] == 3)
					{
						if (camera_position == 0)
							obstacle = glm::translate(obstacle, glm::vec3(0 + 1.25f, 1.5f, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							obstacle = glm::translate(obstacle, glm::vec3(0 + 1.25f, 1.5f + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								obstacle = glm::translate(obstacle, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					obstacle = glm::scale(obstacle, glm::vec3(0.75f));
					RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], obstacle, mapTextures["cub2"]);
					if (player_position == box_location_id_position[i] && angularStep - distanta_platforma[i] >= 1.0f && angularStep - distanta_platforma[i] <= 1.7f && jumping_state == 0 && is_player_dead == 0)
					{
						if(invulnerability_time <= 0)
						exit(0);
					}
				}
				
				
				
				if (pickups_location_id[i] == 1)
				{
					if (pickups_location_id_position[i] == 1)
					{
						if(camera_position == 0)
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0 - 1.25f, y_pickups, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0 - 1.25f, y_pickups + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					else if (pickups_location_id_position[i] == 2)
					{
						if (camera_position == 0)
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, y_pickups, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, y_pickups + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					else if (pickups_location_id_position[i] == 3)
					{
						if (camera_position == 0)
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0 + 1.25f, y_pickups, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0 + 1.25f, y_pickups + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					modelMatrixPickup = glm::scale(modelMatrixPickup, glm::vec3(0.5f));
					modelMatrixPickup = glm::rotate(modelMatrixPickup, RADIANS(180), GetSceneCamera()->transform->GetLocalOYVector());
					if (pickups_status[i] == 1)
					{
						if(pickups_model_id[i] == 0)
							RenderTextureMesh(meshes["square"], shaders["ShaderLab9"], modelMatrixPickup, mapTextures["redsmile"]);
						else if(pickups_model_id[i] == 1)
							RenderTextureMesh(meshes["square"], shaders["ShaderLab9"], modelMatrixPickup, mapTextures["XP"]);
						else if (pickups_model_id[i] == 2)
							RenderTextureMesh(meshes["square"], shaders["ShaderLab9"], modelMatrixPickup, mapTextures["windowslogo"]);
					}
					if (player_position == pickups_location_id_position[i] && angularStep - distanta_platforma[i] >= 1.3f && angularStep-distanta_platforma[i] <= 1.7f && pickups_status[i] == 1 && jumping_state == 0 && is_player_dead == 0)
					{
						pickups_status[i] = 0;
						score += 10;
					}

				}
				
				
				
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


				if (player_position == 2 && contor_pozitie_sfera == i && jumping_state == 0 && is_player_dead == 0)
				{
					//cout << "CONTOR POZITIE SFERA este " << contor_pozitie_sfera << endl;
 					//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix);
					RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix, mapTextures["movcrate"]);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
				}
				else
				{
					if (platforme_parcurse[i][2] == 1)
					{
						//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix, mapTextures["movcrate"]);
					}
					if (platforme_parcurse[i][2] == 0)
					{
						//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix, mapTextures["crate"]);
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

				if (player_position == 3 && contor_pozitie_sfera == i && jumping_state == 0 && is_player_dead == 0)
				{
					//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix2);
					RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix2, mapTextures["movcrate"]);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
				}
				else
				{
					if (platforme_parcurse[i][3] == 1)
					{
						//cout << "AICI ESTEE   " << player_position << endl;
						//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix2);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix2, mapTextures["movcrate"]);
					}
					if (platforme_parcurse[i][3] == 0)
					{
						//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix2);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix2, mapTextures["crate"]);
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
				if (player_position == 1 && contor_pozitie_sfera == i && jumping_state == 0 && is_player_dead == 0)
				{
					//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix3);
					RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix3, mapTextures["movcrate"]);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
				}
				else
				{
					if (platforme_parcurse[i][1] == 1)
					{
						//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix3);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix3, mapTextures["movcrate"]);
					}
					if (platforme_parcurse[i][1] == 0)
					{
						//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix3);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix3, mapTextures["crate"]);
					}
					//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix3);
				}
			}
		}


		if (random_generate_platforms[i] == 2)
		{
			glm::mat4 modelMatrixPickup = glm::mat4(1);
			glm::mat4 obstacle = glm::mat4(1);
			
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

				if (box_location_id[i] == 1 && box_location_id_position[i] != 1)
				{
					/*if (box_location_id_position[i] == 1)
					{
						if (camera_position == 0)
							obstacle = glm::translate(obstacle, glm::vec3(0 - 1.25f, 1.5f, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							obstacle = glm::translate(obstacle, glm::vec3(0 - 1.25f, 1.5f + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								obstacle = glm::translate(obstacle, glm::vec3(0, -0.25f, 0));
							}
						}
					}*/
					if (box_location_id_position[i] == 2)
					{
						if (camera_position == 0)
							obstacle = glm::translate(obstacle, glm::vec3(0, 1.5f, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							obstacle = glm::translate(obstacle, glm::vec3(0, 1.5f + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								obstacle = glm::translate(obstacle, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					else if (box_location_id_position[i] == 3)
					{
						if (camera_position == 0)
							obstacle = glm::translate(obstacle, glm::vec3(0 + 1.25f, 1.5f, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							obstacle = glm::translate(obstacle, glm::vec3(0 + 1.25f, 1.5f + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								obstacle = glm::translate(obstacle, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					obstacle = glm::scale(obstacle, glm::vec3(0.75f));
					RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], obstacle, mapTextures["cub2"]);
					if (player_position == box_location_id_position[i] && angularStep - distanta_platforma[i] >= 1.0f && angularStep - distanta_platforma[i] <= 1.7f && jumping_state == 0 && is_player_dead == 0)
					{
						if(invulnerability_time <= 0)
						exit(0);
					}
				}
				
				
				
				
				
				if (pickups_location_id[i] == 1 && pickups_location_id_position[i] != 1)
				{
					/*if (pickups_location_id_position[i] == 1)
					{
						if (camera_position == 0)
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0 - 1.25f, y_pickups, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0 - 1.25f, y_pickups + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, -0.25f, 0));
							}
						}
					}*/
					if (pickups_location_id_position[i] == 2)
					{
						if (camera_position == 0)
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, y_pickups, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, y_pickups + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					else if (pickups_location_id_position[i] == 3)
					{
						if (camera_position == 0)
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0 + 1.25f, y_pickups, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0 + 1.25f, y_pickups + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					modelMatrixPickup = glm::scale(modelMatrixPickup, glm::vec3(0.5f));
					modelMatrixPickup = glm::rotate(modelMatrixPickup, RADIANS(180), GetSceneCamera()->transform->GetLocalOYVector());
					if (pickups_status[i] == 1)
					{
						if (pickups_model_id[i] == 0)
							RenderTextureMesh(meshes["square"], shaders["ShaderLab9"], modelMatrixPickup, mapTextures["redsmile"]);
						else if (pickups_model_id[i] == 1)
							RenderTextureMesh(meshes["square"], shaders["ShaderLab9"], modelMatrixPickup, mapTextures["XP"]);
						else if (pickups_model_id[i] == 2)
							RenderTextureMesh(meshes["square"], shaders["ShaderLab9"], modelMatrixPickup, mapTextures["windowslogo"]);
					}
					if (player_position == pickups_location_id_position[i] && angularStep - distanta_platforma[i] >= 1.3f && angularStep - distanta_platforma[i] <= 1.7f && pickups_status[i] == 1 && jumping_state == 0 && is_player_dead == 0)
					{
						pickups_status[i] = 0;
						score += 10;
					}

				}
				
				
				
				
				
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
				if (player_position == 2 && contor_pozitie_sfera == i && jumping_state == 0 && is_player_dead == 0)
				{
					//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix);
					RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix, mapTextures["movcrate"]);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
				}
				else
				{
					if (platforme_parcurse[i][2] == 1)
					{
						//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix, mapTextures["movcrate"]);
					}
					if (platforme_parcurse[i][2] == 0)
					{
						//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix, mapTextures["crate"]);
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
				if (player_position == 3 && contor_pozitie_sfera == i && jumping_state == 0 && is_player_dead == 0)
				{
					//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix2);
					RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix2, mapTextures["movcrate"]);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
				}
				else
				{
					if (platforme_parcurse[i][3] == 1)
					{
						//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix2);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix2, mapTextures["movcrate"]);
					}
					if (platforme_parcurse[i][3] == 0)
					{
						//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix2);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix2, mapTextures["crate"]);
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
			glm::mat4 modelMatrixPickup = glm::mat4(1);
			glm::mat4 obstacle = glm::mat4(1);

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
				
				if (box_location_id[i] == 1 && box_location_id_position[i] != 2)
				{
					//cout << "SAL" << endl;
					if (box_location_id_position[i] == 1)
					{
						if (camera_position == 0)
							obstacle = glm::translate(obstacle, glm::vec3(0 - 1.25f, 1.5f, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							obstacle = glm::translate(obstacle, glm::vec3(0 - 1.25f, 1.5f + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								obstacle = glm::translate(obstacle, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					else if (box_location_id_position[i] == 2)
					{
						if (camera_position == 0)
							obstacle = glm::translate(obstacle, glm::vec3(0, 1.5f, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							obstacle = glm::translate(obstacle, glm::vec3(0, 1.5f + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								obstacle = glm::translate(obstacle, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					else if (box_location_id_position[i] == 3)
					{
						if (camera_position == 0)
							obstacle = glm::translate(obstacle, glm::vec3(0 + 1.25f, 1.5f, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							obstacle = glm::translate(obstacle, glm::vec3(0 + 1.25f, 1.5f + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								obstacle = glm::translate(obstacle, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					obstacle = glm::scale(obstacle, glm::vec3(0.75f));
					RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], obstacle, mapTextures["cub2"]);
					if (player_position == box_location_id_position[i] && angularStep - distanta_platforma[i] >= 1.0f && angularStep - distanta_platforma[i] <= 1.7f && jumping_state == 0 && is_player_dead == 0)
					{
						if (invulnerability_time <= 0)
							exit(0);
					}
				}
				
				
				
				
				
				
				
				if (pickups_location_id[i] == 1 && pickups_location_id_position[i] != 2)
				{
					if (pickups_location_id_position[i] == 1)
					{
						if (camera_position == 0)
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0 - 1.25f, y_pickups, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0 - 1.25f, y_pickups + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					/*else if (pickups_location_id_position[i] == 2)
					{
						if (camera_position == 0)
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, y_pickups, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, y_pickups + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, -0.25f, 0));
							}
						}
					}*/
					else if (pickups_location_id_position[i] == 3)
					{
						if (camera_position == 0)
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0 + 1.25f, y_pickups, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0 + 1.25f, y_pickups + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					modelMatrixPickup = glm::scale(modelMatrixPickup, glm::vec3(0.5f));
					modelMatrixPickup = glm::rotate(modelMatrixPickup, RADIANS(180), GetSceneCamera()->transform->GetLocalOYVector());
					if (pickups_status[i] == 1)
					{
						if (pickups_model_id[i] == 0)
							RenderTextureMesh(meshes["square"], shaders["ShaderLab9"], modelMatrixPickup, mapTextures["redsmile"]);
						else if (pickups_model_id[i] == 1)
							RenderTextureMesh(meshes["square"], shaders["ShaderLab9"], modelMatrixPickup, mapTextures["XP"]);
						else if (pickups_model_id[i] == 2)
							RenderTextureMesh(meshes["square"], shaders["ShaderLab9"], modelMatrixPickup, mapTextures["windowslogo"]);
					}
					if (player_position == pickups_location_id_position[i] && angularStep - distanta_platforma[i] >= 1.3f && angularStep - distanta_platforma[i] <= 1.7f && pickups_status[i] == 1 && jumping_state == 0 && is_player_dead == 0)
					{
						pickups_status[i] = 0;
						score += 10;
					}

				}




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
				if (player_position == 3 && contor_pozitie_sfera == i && jumping_state == 0 && is_player_dead == 0)
				{
					//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix2);
					RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix2, mapTextures["movcrate"]);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
				}
				else
				{
					if (platforme_parcurse[i][3] == 1)
					{
						//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix2);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix2, mapTextures["movcrate"]);
					}
					if (platforme_parcurse[i][3] == 0)
					{
						//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix2);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix2, mapTextures["crate"]);
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
				if (player_position == 1 && contor_pozitie_sfera == i && jumping_state == 0 && is_player_dead == 0)
				{
					//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix3);
					RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix3, mapTextures["movcrate"]);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
				}
				else
				{
					if (platforme_parcurse[i][1] == 1)
					{
						//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix3);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix3, mapTextures["movcrate"]);
					}
					if (platforme_parcurse[i][1] == 0)
					{
						//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix3);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix3, mapTextures["crate"]);
					}
					//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix3);
				}
			}
		}

		if (random_generate_platforms[i] == 4)
		{

			glm::mat4 modelMatrixPickup = glm::mat4(1);
			glm::mat4 obstacle = glm::mat4(1);

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
				if (box_location_id[i] == 1 && box_location_id_position[i] != 3)
				{
					//cout << "SAL" << endl;
					if (box_location_id_position[i] == 1)
					{
						if (camera_position == 0)
							obstacle = glm::translate(obstacle, glm::vec3(0 - 1.25f, 1.5f, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							obstacle = glm::translate(obstacle, glm::vec3(0 - 1.25f, 1.5f + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								obstacle = glm::translate(obstacle, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					else if (box_location_id_position[i] == 2)
					{
						if (camera_position == 0)
							obstacle = glm::translate(obstacle, glm::vec3(0, 1.5f, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							obstacle = glm::translate(obstacle, glm::vec3(0, 1.5f + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								obstacle = glm::translate(obstacle, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					else if (box_location_id_position[i] == 3)
					{
						if (camera_position == 0)
							obstacle = glm::translate(obstacle, glm::vec3(0 + 1.25f, 1.5f, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							obstacle = glm::translate(obstacle, glm::vec3(0 + 1.25f, 1.5f + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								obstacle = glm::translate(obstacle, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					obstacle = glm::scale(obstacle, glm::vec3(0.75f));
					RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], obstacle, mapTextures["cub2"]);
					if (player_position == box_location_id_position[i] && angularStep - distanta_platforma[i] >= 1.0f && angularStep - distanta_platforma[i] <= 1.7f && jumping_state == 0 && is_player_dead == 0)
					{
						if (invulnerability_time <= 0)
							exit(0);
					}
				}



				if (pickups_location_id[i] == 1 && pickups_location_id_position[i] != 3)
				{
					if (pickups_location_id_position[i] == 1)
					{
						if (camera_position == 0)
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0 - 1.25f, y_pickups, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0 - 1.25f, y_pickups + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					else if (pickups_location_id_position[i] == 2)
					{
						if (camera_position == 0)
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, y_pickups, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, y_pickups + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					/*else if (pickups_location_id_position[i] == 3)
					{
						if (camera_position == 0)
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0 + 1.25f, y_pickups, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0 + 1.25f, y_pickups + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, -0.25f, 0));
							}
						}
					}*/
					modelMatrixPickup = glm::scale(modelMatrixPickup, glm::vec3(0.5f));
					modelMatrixPickup = glm::rotate(modelMatrixPickup, RADIANS(180), GetSceneCamera()->transform->GetLocalOYVector());
					if (pickups_status[i] == 1)
					{
						if (pickups_model_id[i] == 0)
							RenderTextureMesh(meshes["square"], shaders["ShaderLab9"], modelMatrixPickup, mapTextures["redsmile"]);
						else if (pickups_model_id[i] == 1)
							RenderTextureMesh(meshes["square"], shaders["ShaderLab9"], modelMatrixPickup, mapTextures["XP"]);
						else if (pickups_model_id[i] == 1)
							RenderTextureMesh(meshes["square"], shaders["ShaderLab9"], modelMatrixPickup, mapTextures["windowslogo"]);
					}
					if (player_position == pickups_location_id_position[i] && angularStep - distanta_platforma[i] >= 1.3f && angularStep - distanta_platforma[i] <= 1.7f && pickups_status[i] == 1 && jumping_state == 0 && is_player_dead == 0)
					{
						pickups_status[i] = 0;
						score += 10;
					}

				}




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
				if (player_position == 2 && contor_pozitie_sfera == i && jumping_state == 0 && is_player_dead == 0)
				{
					//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix);
					RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix, mapTextures["movcrate"]);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
				}
				else
				{
					if (platforme_parcurse[i][2] == 1)
					{
						//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix, mapTextures["movcrate"]);
					}
					if (platforme_parcurse[i][2] == 0)
					{
						//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix, mapTextures["crate"]);
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
				if (player_position == 1 && contor_pozitie_sfera == i && jumping_state == 0 && is_player_dead == 0)
				{
					//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix3);
					RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix3, mapTextures["movcrate"]);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
				}
				else
				{
					if (platforme_parcurse[i][1] == 1)
					{
						//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix3);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix3, mapTextures["movcrate"]);
					}
					if (platforme_parcurse[i][1] == 0)
					{
						//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix3);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix3, mapTextures["crate"]);
					}
					//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix3);
				}
			}

		}

		if (random_generate_platforms[i] == 5)
		{
			
			glm::mat4 modelMatrixPickup = glm::mat4(1);
			glm::mat4 obstacle = glm::mat4(1);

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
				

				if (box_location_id[i] == 1)
				{
					//cout << "SAL" << endl;
					if (box_location_id_position[i] == 1)
					{
						if (camera_position == 0)
							obstacle = glm::translate(obstacle, glm::vec3(0 - 1.25f, 1.5f, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							obstacle = glm::translate(obstacle, glm::vec3(0 - 1.25f, 1.5f + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								obstacle = glm::translate(obstacle, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					else if (box_location_id_position[i] == 2)
					{
						if (camera_position == 0)
							obstacle = glm::translate(obstacle, glm::vec3(0, 1.5f, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							obstacle = glm::translate(obstacle, glm::vec3(0, 1.5f + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								obstacle = glm::translate(obstacle, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					else if (box_location_id_position[i] == 3)
					{
						if (camera_position == 0)
							obstacle = glm::translate(obstacle, glm::vec3(0 + 1.25f, 1.5f, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							obstacle = glm::translate(obstacle, glm::vec3(0 + 1.25f, 1.5f + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								obstacle = glm::translate(obstacle, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					obstacle = glm::scale(obstacle, glm::vec3(0.75f));
					RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], obstacle, mapTextures["cub2"]);
					if (player_position == box_location_id_position[i] && angularStep - distanta_platforma[i] >= 1.0f && angularStep - distanta_platforma[i] <= 1.7f && jumping_state == 0 && is_player_dead == 0)
					{
						if (invulnerability_time <= 0)
							exit(0);
					}
				}




				if (pickups_location_id[i] == 1)
				{
					if (pickups_location_id_position[i] == 1)
					{
						if (camera_position == 0)
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0 - 1.25f, y_pickups, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0 - 1.25f, y_pickups + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					else if (pickups_location_id_position[i] == 2)
					{
						if (camera_position == 0)
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, y_pickups, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, y_pickups + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					else if (pickups_location_id_position[i] == 3)
					{
						if (camera_position == 0)
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0 + 1.25f, y_pickups, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0 + 1.25f, y_pickups + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					modelMatrixPickup = glm::scale(modelMatrixPickup, glm::vec3(0.5f));
					modelMatrixPickup = glm::rotate(modelMatrixPickup, RADIANS(180), GetSceneCamera()->transform->GetLocalOYVector());
					if (pickups_status[i] == 1)
					{
						if (pickups_model_id[i] == 0)
							RenderTextureMesh(meshes["square"], shaders["ShaderLab9"], modelMatrixPickup, mapTextures["redsmile"]);
						else if (pickups_model_id[i] == 1)
							RenderTextureMesh(meshes["square"], shaders["ShaderLab9"], modelMatrixPickup, mapTextures["XP"]);
						else if (pickups_model_id[i] == 2)
							RenderTextureMesh(meshes["square"], shaders["ShaderLab9"], modelMatrixPickup, mapTextures["windowslogo"]);
					}
					if (player_position == pickups_location_id_position[i] && angularStep - distanta_platforma[i] >= 1.3f && angularStep - distanta_platforma[i] <= 1.7f && pickups_status[i] == 1 && jumping_state == 0 && is_player_dead == 0)
					{
						pickups_status[i] = 0;
						score += 10;
					}

				}
				
				
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


				if (player_position == 2 && contor_pozitie_sfera == i && jumping_state == 0 && is_player_dead == 0)
				{
					//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix);
					RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix, mapTextures["movcrate"]);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
					if ( (special_platform_indices[i] == 2 || special_platform_indices[i] == 4) && special_platform_type[i] == 1 && jumping_state == 0)
					{
						cout << "YOU ARE DEAD" << endl;
						exit(0);
					}
					if ((special_platform_indices[i] == 2 || special_platform_indices[i] == 4) && special_platform_type[i] == 2 && jumping_state == 0)
					{
						//cout << "YOU TOUCHED YELLOW PLATFORM" << endl;
						if (take_fuel_platform_set[i] == 0)
						{
							take_fuel = 1;
							take_fuel_platform_set[i] = 1;
						}
					}
					if ((special_platform_indices[i] == 2 || special_platform_indices[i] == 4) && special_platform_type[i] == 3 && jumping_state == 0)
					{
						speed_powerup = 1;
						speed_platform_effect = 1;
					}
					if ((special_platform_indices[i] == 2 || special_platform_indices[i] == 4) && special_platform_type[i] == 4 && jumping_state == 0)
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
					if ((special_platform_indices[i] == 2 || special_platform_indices[i] == 4) && special_platform_type[i] == 1)
					{
						//RenderMesh(meshes["box"], shaders["RedPlatform"], modelMatrix);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix, mapTextures["redcrate"]);
					}
					if ((special_platform_indices[i] == 2 || special_platform_indices[i] == 4) && special_platform_type[i] == 2)
					{
						//RenderMesh(meshes["box"], shaders["YellowPlatform"], modelMatrix);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix, mapTextures["yellowcrate"]);
					}
					if ((special_platform_indices[i] == 2 || special_platform_indices[i] == 4) && special_platform_type[i] == 3)
					{
						//RenderMesh(meshes["box"], shaders["HitPlatformShader"], modelMatrix);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix, mapTextures["orangecrate"]);
					}
					if ((special_platform_indices[i] == 2 || special_platform_indices[i] == 4) && special_platform_type[i] == 4)
					{
						//RenderMesh(meshes["box"], shaders["GreenPlatform"], modelMatrix);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix, mapTextures["greencrate"]);
					}
					else
					{
						if (platforme_parcurse[i][2] == 1)
						{
							//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix);
							RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix, mapTextures["movcrate"]);
						}
						if (platforme_parcurse[i][2] == 0)
						{
							//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix);
							RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix, mapTextures["crate"]);
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

				if (player_position == 3 && contor_pozitie_sfera == i && jumping_state == 0 && is_player_dead == 0)
				{
					//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix2);
					RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix2, mapTextures["movcrate"]);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
					if ((special_platform_indices[i] == 3 || special_platform_indices[i] == 4) && special_platform_type[i] == 1 && jumping_state == 0)
					{
						cout << "YOU ARE DEAD" << endl;
						exit(0);
					}
					if ((special_platform_indices[i] == 3 || special_platform_indices[i] == 4) && special_platform_type[i] == 2 && jumping_state == 0)
					{
						//cout << "YOU TOUCHED YELLOW PLATFORM" << endl;
						if (take_fuel_platform_set[i] == 0)
						{
							take_fuel = 1;
							take_fuel_platform_set[i] = 1;
						}
					}
					if ((special_platform_indices[i] == 3 || special_platform_indices[i] == 4) && special_platform_type[i] == 3 && jumping_state == 0)
					{
						speed_powerup = 1;
						speed_platform_effect = 1;
					}
					if ((special_platform_indices[i] == 3 || special_platform_indices[i] == 4) && special_platform_type[i] == 4 && jumping_state == 0)
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
					if ((special_platform_indices[i] == 3 || special_platform_indices[i] == 4) && special_platform_type[i] == 1)
					{
						//RenderMesh(meshes["box"], shaders["RedPlatform"], modelMatrix2);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix2, mapTextures["redcrate"]);
					}
					if ((special_platform_indices[i] == 3 || special_platform_indices[i] == 4) && special_platform_type[i] == 2)
					{
						//RenderMesh(meshes["box"], shaders["YellowPlatform"], modelMatrix2);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix2, mapTextures["yellowcrate"]);
					}
					if ((special_platform_indices[i] == 3 || special_platform_indices[i] == 4) && special_platform_type[i] == 3)
					{
						//RenderMesh(meshes["box"], shaders["HitPlatformShader"], modelMatrix2);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix2, mapTextures["orangecrate"]);
					}
					if ((special_platform_indices[i] == 3 || special_platform_indices[i] == 4) && special_platform_type[i] == 4)
					{
						//RenderMesh(meshes["box"], shaders["GreenPlatform"], modelMatrix2);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix2, mapTextures["greencrate"]);
					}
					else
					{
						if (platforme_parcurse[i][3] == 1)
						{
							//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix2);
							RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix2, mapTextures["movcrate"]);
						}
						if (platforme_parcurse[i][3] == 0)
						{
							//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix2);
							RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix2, mapTextures["crate"]);
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
				if (player_position == 1 && contor_pozitie_sfera == i && jumping_state == 0 && is_player_dead == 0)
				{
					//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix3);
					RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix3, mapTextures["movcrate"]);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
					if ((special_platform_indices[i] == 1 || special_platform_indices[i] == 4) && special_platform_type[i] == 1 && jumping_state == 0)
					{
						cout << "YOU ARE DEAD" << endl;
						exit(0);
					}
					if ((special_platform_indices[i] == 1 || special_platform_indices[i] == 4) && special_platform_type[i] == 2 && jumping_state == 0)
					{
						//cout << "YOU TOUCHED YELLOW PLATFORM" << endl;
						if (take_fuel_platform_set[i] == 0)
						{
							take_fuel = 1;
							take_fuel_platform_set[i] = 1;
						}
					}
					if ((special_platform_indices[i] == 1 || special_platform_indices[i] == 4) && special_platform_type[i] == 3 && jumping_state == 0)
					{
						speed_powerup = 1;
						speed_platform_effect = 1;
					}
					if ((special_platform_indices[i] == 1 || special_platform_indices[i] == 4) && special_platform_type[i] == 4 && jumping_state == 0)
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
					if ((special_platform_indices[i] == 1 || special_platform_indices[i] == 4) && special_platform_type[i] == 1)
					{
						//RenderMesh(meshes["box"], shaders["RedPlatform"], modelMatrix3);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix3, mapTextures["redcrate"]);
					}
					if ((special_platform_indices[i] == 1 || special_platform_indices[i] == 4) && special_platform_type[i] == 2)
					{
						//RenderMesh(meshes["box"], shaders["YellowPlatform"], modelMatrix3);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix3, mapTextures["yellowcrate"]);
					}
					if ((special_platform_indices[i] == 1 || special_platform_indices[i] == 4) && special_platform_type[i] == 3)
					{
						//RenderMesh(meshes["box"], shaders["HitPlatformShader"], modelMatrix3);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix3, mapTextures["orangecrate"]);
					}
					if ((special_platform_indices[i] == 1 || special_platform_indices[i] == 4) && special_platform_type[i] == 4)
					{
						//RenderMesh(meshes["box"], shaders["GreenPlatform"], modelMatrix3);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix3, mapTextures["greencrate"]);
					}
					else
					{
						if (platforme_parcurse[i][1] == 1)
						{
							//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix3);
							RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix3, mapTextures["movcrate"]);
						}
						if (platforme_parcurse[i][1] == 0)
						{
							//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix3);
							RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix3, mapTextures["crate"]);
						}
						//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix3);
					}
				}
			}
		}


		if (random_generate_platforms[i] == 6)
		{
			glm::mat4 modelMatrixPickup = glm::mat4(1);
			glm::mat4 obstacle = glm::mat4(1);
			
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
				
				if (box_location_id[i] == 1 && box_location_id_position[i] != 1)
				{
					//cout << "SAL" << endl;
					if (box_location_id_position[i] == 1)
					{
						if (camera_position == 0)
							obstacle = glm::translate(obstacle, glm::vec3(0 - 1.25f, 1.5f, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							obstacle = glm::translate(obstacle, glm::vec3(0 - 1.25f, 1.5f + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								obstacle = glm::translate(obstacle, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					else if (box_location_id_position[i] == 2)
					{
						if (camera_position == 0)
							obstacle = glm::translate(obstacle, glm::vec3(0, 1.5f, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							obstacle = glm::translate(obstacle, glm::vec3(0, 1.5f + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								obstacle = glm::translate(obstacle, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					else if (box_location_id_position[i] == 3)
					{
						if (camera_position == 0)
							obstacle = glm::translate(obstacle, glm::vec3(0 + 1.25f, 1.5f, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							obstacle = glm::translate(obstacle, glm::vec3(0 + 1.25f, 1.5f + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								obstacle = glm::translate(obstacle, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					obstacle = glm::scale(obstacle, glm::vec3(0.75f));
					RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], obstacle, mapTextures["cub2"]);
					if (player_position == box_location_id_position[i] && angularStep - distanta_platforma[i] >= 1.0f && angularStep - distanta_platforma[i] <= 1.7f && jumping_state == 0 && is_player_dead == 0)
					{
						if (invulnerability_time <= 0)
							exit(0);
					}
				}

				
				
				if (pickups_location_id[i] == 1 && pickups_location_id_position[i] != 1)
				{
					/*if (pickups_location_id_position[i] == 1)
					{
						if (camera_position == 0)
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0 - 1.25f, y_pickups, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0 - 1.25f, y_pickups + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, -0.25f, 0));
							}
						}
					}*/
					if (pickups_location_id_position[i] == 2)
					{
						if (camera_position == 0)
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, y_pickups, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, y_pickups + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					else if (pickups_location_id_position[i] == 3)
					{
						if (camera_position == 0)
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0 + 1.25f, y_pickups, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0 + 1.25f, y_pickups + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					modelMatrixPickup = glm::scale(modelMatrixPickup, glm::vec3(0.5f));
					modelMatrixPickup = glm::rotate(modelMatrixPickup, RADIANS(180), GetSceneCamera()->transform->GetLocalOYVector());
					if (pickups_status[i] == 1)
					{
						if (pickups_model_id[i] == 0)
							RenderTextureMesh(meshes["square"], shaders["ShaderLab9"], modelMatrixPickup, mapTextures["redsmile"]);
						else if (pickups_model_id[i] == 1)
							RenderTextureMesh(meshes["square"], shaders["ShaderLab9"], modelMatrixPickup, mapTextures["XP"]);
						else if (pickups_model_id[i] == 2)
							RenderTextureMesh(meshes["square"], shaders["ShaderLab9"], modelMatrixPickup, mapTextures["windowslogo"]);
					}
					if (player_position == pickups_location_id_position[i] && angularStep - distanta_platforma[i] >= 1.3f && angularStep - distanta_platforma[i] <= 1.7f && pickups_status[i] == 1 && jumping_state == 0 && is_player_dead == 0)
					{
						pickups_status[i] = 0;
						score += 10;
					}

				}


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
				if (player_position == 2 && contor_pozitie_sfera == i && jumping_state == 0 && is_player_dead == 0)
				{
					//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix);
					RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix, mapTextures["movcrate"]);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
					if ((special_platform_indices[i] == 2 || special_platform_indices[i] == 4) && special_platform_type[i] == 1 && jumping_state == 0)
					{
						cout << "YOU ARE DEAD" << endl;
						exit(0);
					}
					if ((special_platform_indices[i] == 2 || special_platform_indices[i] == 4) && special_platform_type[i] == 2 && jumping_state == 0)
					{
						//cout << "YOU TOUCHED YELLOW PLATFORM" << endl;
						if (take_fuel_platform_set[i] == 0)
						{
							take_fuel = 1;
							take_fuel_platform_set[i] = 1;
						}
					}
					if ((special_platform_indices[i] == 2 || special_platform_indices[i] == 4) && special_platform_type[i] == 3 && jumping_state == 0)
					{
						speed_powerup = 1;
						speed_platform_effect = 1;
					}
					if ((special_platform_indices[i] == 2 || special_platform_indices[i] == 4) && special_platform_type[i] == 4 && jumping_state == 0)
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
					if ((special_platform_indices[i] == 2 || special_platform_indices[i] == 4) && special_platform_type[i] == 1)
					{
						//RenderMesh(meshes["box"], shaders["RedPlatform"], modelMatrix);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix, mapTextures["redcrate"]);
					}
					if ((special_platform_indices[i] == 2 || special_platform_indices[i] == 4) && special_platform_type[i] == 2)
					{
						//RenderMesh(meshes["box"], shaders["YellowPlatform"], modelMatrix);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix, mapTextures["yellowcrate"]);
					}
					if ((special_platform_indices[i] == 2 || special_platform_indices[i] == 4) && special_platform_type[i] == 3)
					{
						//RenderMesh(meshes["box"], shaders["HitPlatformShader"], modelMatrix);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix, mapTextures["orangecrate"]);
					}
					if ((special_platform_indices[i] == 2 || special_platform_indices[i] == 4) && special_platform_type[i] == 4)
					{
						//RenderMesh(meshes["box"], shaders["GreenPlatform"], modelMatrix);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix, mapTextures["greencrate"]);
					}
					else
					{
						if (platforme_parcurse[i][2] == 1)
						{
							//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix);
							RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix, mapTextures["movcrate"]);
						}
						if (platforme_parcurse[i][2] == 0)
						{
							//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix);
							RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix, mapTextures["crate"]);
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
				if (player_position == 3 && contor_pozitie_sfera == i && jumping_state == 0 && is_player_dead == 0)
				{
					//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix2);
					RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix2, mapTextures["movcrate"]);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
					if ((special_platform_indices[i] == 3 || special_platform_indices[i] == 4) && special_platform_type[i] == 1 && jumping_state == 0)
					{
						cout << "YOU ARE DEAD" << endl;
						exit(0);
					}
					if ((special_platform_indices[i] == 3 || special_platform_indices[i] == 4) && special_platform_type[i] == 2 && jumping_state == 0)
					{
						//cout << "YOU TOUCHED YELLOW PLATFORM" << endl;
						if (take_fuel_platform_set[i] == 0)
						{
							take_fuel = 1;
							take_fuel_platform_set[i] = 1;
						}
					}
					if ((special_platform_indices[i] == 3 || special_platform_indices[i] == 4) && special_platform_type[i] == 3 && jumping_state == 0)
					{
						speed_powerup = 1;
						speed_platform_effect = 1;
					}
					if ((special_platform_indices[i] == 3 || special_platform_indices[i] == 4) && special_platform_type[i] == 4 && jumping_state == 0)
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
					if ((special_platform_indices[i] == 3 || special_platform_indices[i] == 4) && special_platform_type[i] == 1)
					{
						//RenderMesh(meshes["box"], shaders["RedPlatform"], modelMatrix2);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix2, mapTextures["redcrate"]);
					}
					if ((special_platform_indices[i] == 3 || special_platform_indices[i] == 4) && special_platform_type[i] == 2)
					{
						//RenderMesh(meshes["box"], shaders["YellowPlatform"], modelMatrix2);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix2, mapTextures["yellowcrate"]);
					}
					if ((special_platform_indices[i] == 3 || special_platform_indices[i] == 4) && special_platform_type[i] == 3)
					{
						//RenderMesh(meshes["box"], shaders["HitPlatformShader"], modelMatrix2);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix2, mapTextures["orangecrate"]);
					}
					if ((special_platform_indices[i] == 3 || special_platform_indices[i] == 4) && special_platform_type[i] == 4)
					{
						//RenderMesh(meshes["box"], shaders["GreenPlatform"], modelMatrix2);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix2, mapTextures["greencrate"]);
					}
					else
					{
						if (platforme_parcurse[i][3] == 1)
						{
							//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix2);
							RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix2, mapTextures["movcrate"]);
						}
						if (platforme_parcurse[i][3] == 0)
						{
							//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix2);
							RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix2, mapTextures["crate"]);
						}
						//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix2);
					}
					//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix2);
				}

			}
		}


		if (random_generate_platforms[i] == 7)
		{
			glm::mat4 modelMatrixPickup = glm::mat4(1);
			glm::mat4 obstacle = glm::mat4(1);


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
				
				if (box_location_id[i] == 1 && box_location_id_position[i] != 2)
				{
					//cout << "SAL" << endl;
					if (box_location_id_position[i] == 1)
					{
						if (camera_position == 0)
							obstacle = glm::translate(obstacle, glm::vec3(0 - 1.25f, 1.5f, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							obstacle = glm::translate(obstacle, glm::vec3(0 - 1.25f, 1.5f + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								obstacle = glm::translate(obstacle, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					else if (box_location_id_position[i] == 2)
					{
						if (camera_position == 0)
							obstacle = glm::translate(obstacle, glm::vec3(0, 1.5f, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							obstacle = glm::translate(obstacle, glm::vec3(0, 1.5f + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								obstacle = glm::translate(obstacle, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					else if (box_location_id_position[i] == 3)
					{
						if (camera_position == 0)
							obstacle = glm::translate(obstacle, glm::vec3(0 + 1.25f, 1.5f, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							obstacle = glm::translate(obstacle, glm::vec3(0 + 1.25f, 1.5f + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								obstacle = glm::translate(obstacle, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					obstacle = glm::scale(obstacle, glm::vec3(0.75f));
					RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], obstacle, mapTextures["cub2"]);
					if (player_position == box_location_id_position[i] && angularStep - distanta_platforma[i] >= 1.0f && angularStep - distanta_platforma[i] <= 1.7f && jumping_state == 0 && is_player_dead == 0)
					{
						if (invulnerability_time <= 0)
							exit(0);
					}
				}
				
				
				
				
				
				if (pickups_location_id[i] == 1 && pickups_location_id_position[i] != 2)
				{
					if (pickups_location_id_position[i] == 1)
					{
						if (camera_position == 0)
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0 - 1.25f, y_pickups, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0 - 1.25f, y_pickups + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					/*else if (pickups_location_id_position[i] == 2)
					{
						if (camera_position == 0)
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, y_pickups, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, y_pickups + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, -0.25f, 0));
							}
						}
					}*/
					else if (pickups_location_id_position[i] == 3)
					{
						if (camera_position == 0)
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0 + 1.25f, y_pickups, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0 + 1.25f, y_pickups + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					modelMatrixPickup = glm::scale(modelMatrixPickup, glm::vec3(0.5f));
					modelMatrixPickup = glm::rotate(modelMatrixPickup, RADIANS(180), GetSceneCamera()->transform->GetLocalOYVector());
					if (pickups_status[i] == 1)
					{
						if (pickups_model_id[i] == 0)
							RenderTextureMesh(meshes["square"], shaders["ShaderLab9"], modelMatrixPickup, mapTextures["redsmile"]);
						else if (pickups_model_id[i] == 1)
							RenderTextureMesh(meshes["square"], shaders["ShaderLab9"], modelMatrixPickup, mapTextures["XP"]);
						else if (pickups_model_id[i] == 2)
							RenderTextureMesh(meshes["square"], shaders["ShaderLab9"], modelMatrixPickup, mapTextures["windowslogo"]);
					}
					if (player_position == pickups_location_id_position[i] && angularStep - distanta_platforma[i] >= 1.3f && angularStep - distanta_platforma[i] <= 1.7f && pickups_status[i] == 1 && jumping_state == 0 && is_player_dead == 0)
					{
						pickups_status[i] = 0;
						score += 10;
					}

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
				if (player_position == 3 && contor_pozitie_sfera == i && jumping_state == 0 && is_player_dead == 0)
				{
					//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix2);
					RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix2, mapTextures["movcrate"]);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
					if ((special_platform_indices[i] == 3 || special_platform_indices[i] == 4) && special_platform_type[i] == 1 && jumping_state == 0)
					{
						cout << "YOU ARE DEAD" << endl;
						exit(0);
					}
					if ((special_platform_indices[i] == 3 || special_platform_indices[i] == 4) && special_platform_type[i] == 2 && jumping_state == 0)
					{
						//cout << "YOU TOUCHED YELLOW PLATFORM" << endl;
						if (take_fuel_platform_set[i] == 0)
						{
							take_fuel = 1;
							take_fuel_platform_set[i] = 1;
						}
					}
					if ((special_platform_indices[i] == 3 || special_platform_indices[i] == 4) && special_platform_type[i] == 3 && jumping_state == 0)
					{
						speed_powerup = 1;
						speed_platform_effect = 1;
					}
					if ((special_platform_indices[i] == 3 || special_platform_indices[i] == 4) && special_platform_type[i] == 4 && jumping_state == 0)
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
					if ((special_platform_indices[i] == 3 || special_platform_indices[i] == 4) && special_platform_type[i] == 1)
					{
						//RenderMesh(meshes["box"], shaders["RedPlatform"], modelMatrix2);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix2, mapTextures["redcrate"]);
					}
					if ((special_platform_indices[i] == 3 || special_platform_indices[i] == 4) && special_platform_type[i] == 2)
					{
						//RenderMesh(meshes["box"], shaders["YellowPlatform"], modelMatrix2);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix2, mapTextures["yellowcrate"]);
					}
					if ((special_platform_indices[i] == 3 || special_platform_indices[i] == 4) && special_platform_type[i] == 3)
					{
						//RenderMesh(meshes["box"], shaders["HitPlatformShader"], modelMatrix2);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix2, mapTextures["orangecrate"]);
					}
					if ((special_platform_indices[i] == 3 || special_platform_indices[i] == 4) && special_platform_type[i] == 4)
					{
						//RenderMesh(meshes["box"], shaders["GreenPlatform"], modelMatrix2);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix2, mapTextures["greencrate"]);
					}
					else
					{
						if (platforme_parcurse[i][3] == 1)
						{
							//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix2);
							RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix2, mapTextures["movcrate"]);
						}
						if (platforme_parcurse[i][3] == 0)
						{
							//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix2);
							RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix2, mapTextures["crate"]);
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
				if (player_position == 1 && contor_pozitie_sfera == i && jumping_state == 0 && is_player_dead == 0)
				{
					//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix3);
					RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix3, mapTextures["movcrate"]);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
					if ((special_platform_indices[i] == 1 || special_platform_indices[i] == 4) && special_platform_type[i] == 1 && jumping_state == 0)
					{
						cout << "YOU ARE DEAD" << endl;
						exit(0);
					}
					if ((special_platform_indices[i] == 1 || special_platform_indices[i] == 4) && special_platform_type[i] == 2 && jumping_state == 0)
					{
						//cout << "YOU TOUCHED YELLOW PLATFORM" << endl;
						if (take_fuel_platform_set[i] == 0)
						{
							take_fuel = 1;
							take_fuel_platform_set[i] = 1;
						}
					}
					if ((special_platform_indices[i] == 1 || special_platform_indices[i] == 4) && special_platform_type[i] == 3 && jumping_state == 0)
					{
						speed_powerup = 1;
						speed_platform_effect = 1;
					}
					if ((special_platform_indices[i] == 1 || special_platform_indices[i] == 4) && special_platform_type[i] == 4 && jumping_state == 0)
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
					if ((special_platform_indices[i] == 1 || special_platform_indices[i] == 4) && special_platform_type[i] == 1)
					{
						//RenderMesh(meshes["box"], shaders["RedPlatform"], modelMatrix3);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix3, mapTextures["redcrate"]);
					}
					if ((special_platform_indices[i] == 1 || special_platform_indices[i] == 4) && special_platform_type[i] == 2)
					{
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix3, mapTextures["yellowcrate"]);
						//RenderMesh(meshes["box"], shaders["YellowPlatform"], modelMatrix3);
					}
					if ((special_platform_indices[i] == 1 || special_platform_indices[i] == 4) && special_platform_type[i] == 3)
					{
						//RenderMesh(meshes["box"], shaders["HitPlatformShader"], modelMatrix3);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix3, mapTextures["orangecrate"]);
					}
					if ((special_platform_indices[i] == 1 || special_platform_indices[i] == 4) && special_platform_type[i] == 4)
					{
						//RenderMesh(meshes["box"], shaders["GreenPlatform"], modelMatrix3);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix3, mapTextures["greencrate"]);
					}
					else
					{
						if (platforme_parcurse[i][1] == 1)
						{
							//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix3);
							RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix3, mapTextures["movcrate"]);
						}
						if (platforme_parcurse[i][1] == 0)
						{
							//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix3);
							RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix3, mapTextures["crate"]);
						}
						//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix3);
					}
					//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix3);
				}
			}
		}

		if (random_generate_platforms[i] == 8)
		{
			glm::mat4 modelMatrixPickup = glm::mat4(1);
			glm::mat4 obstacle = glm::mat4(1);


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
				
				if (box_location_id[i] == 1 && box_location_id_position[i] != 3)
				{
					//cout << "SAL" << endl;
					if (box_location_id_position[i] == 1)
					{
						if (camera_position == 0)
							obstacle = glm::translate(obstacle, glm::vec3(0 - 1.25f, 1.5f, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							obstacle = glm::translate(obstacle, glm::vec3(0 - 1.25f, 1.5f + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								obstacle = glm::translate(obstacle, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					else if (box_location_id_position[i] == 2)
					{
						if (camera_position == 0)
							obstacle = glm::translate(obstacle, glm::vec3(0, 1.5f, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							obstacle = glm::translate(obstacle, glm::vec3(0, 1.5f + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								obstacle = glm::translate(obstacle, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					else if (box_location_id_position[i] == 3)
					{
						if (camera_position == 0)
							obstacle = glm::translate(obstacle, glm::vec3(0 + 1.25f, 1.5f, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							obstacle = glm::translate(obstacle, glm::vec3(0 + 1.25f, 1.5f + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								obstacle = glm::translate(obstacle, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					obstacle = glm::scale(obstacle, glm::vec3(0.75f));
					RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], obstacle, mapTextures["cub2"]);
					if (player_position == box_location_id_position[i] && angularStep - distanta_platforma[i] >= 1.0f && angularStep - distanta_platforma[i] <= 1.7f && jumping_state == 0 && is_player_dead == 0)
					{
						if (invulnerability_time <= 0)
							exit(0);
					}
				}
				
				
				
				if (pickups_location_id[i] == 1 && pickups_location_id_position[i] != 3)
				{
					if (pickups_location_id_position[i] == 1)
					{
						if (camera_position == 0)
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0 - 1.25f, y_pickups, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0 - 1.25f, y_pickups + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					else if (pickups_location_id_position[i] == 2)
					{
						if (camera_position == 0)
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, y_pickups, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, y_pickups + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, -0.25f, 0));
							}
						}
					}
					/*else if (pickups_location_id_position[i] == 3)
					{
						if (camera_position == 0)
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0 + 1.25f, y_pickups, 0 + angularStep - distanta_platforma[i]));
						if (camera_position == 1)
						{
							modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0 + 1.25f, y_pickups + 0.5f, 0 + angularStep - distanta_platforma[i]));
							if (contor_saritura != 0)
							{
								modelMatrixPickup = glm::translate(modelMatrixPickup, glm::vec3(0, -0.25f, 0));
							}
						}
					}*/
					modelMatrixPickup = glm::scale(modelMatrixPickup, glm::vec3(0.5f));
					modelMatrixPickup = glm::rotate(modelMatrixPickup, RADIANS(180), GetSceneCamera()->transform->GetLocalOYVector());
					if (pickups_status[i] == 1)
					{
						if (pickups_model_id[i] == 0)
							RenderTextureMesh(meshes["square"], shaders["ShaderLab9"], modelMatrixPickup, mapTextures["redsmile"]);
						else if (pickups_model_id[i] == 1)
							RenderTextureMesh(meshes["square"], shaders["ShaderLab9"], modelMatrixPickup, mapTextures["XP"]);
						else if (pickups_model_id[i] == 2)
							RenderTextureMesh(meshes["square"], shaders["ShaderLab9"], modelMatrixPickup, mapTextures["windowslogo"]);
					}
					if (player_position == pickups_location_id_position[i] && angularStep - distanta_platforma[i] >= 1.3f && angularStep - distanta_platforma[i] <= 1.7f && pickups_status[i] == 1 && jumping_state == 0 && is_player_dead == 0)
					{
						pickups_status[i] = 0;
						score += 10;
					}

				}

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
				if (player_position == 2 && contor_pozitie_sfera == i && jumping_state == 0 && is_player_dead == 0)
				{
					//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix);
					RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix, mapTextures["movcrate"]);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
					if ((special_platform_indices[i] == 2 || special_platform_indices[i] == 4) && special_platform_type[i] == 1 && jumping_state == 0)
					{
						cout << "YOU ARE DEAD" << endl;
						exit(0);
					}
					if ((special_platform_indices[i] == 2 || special_platform_indices[i] == 4) && special_platform_type[i] == 2 && jumping_state == 0)
					{
						//cout << "YOU TOUCHED YELLOW PLATFORM" << endl;
						if (take_fuel_platform_set[i] == 0)
						{
							take_fuel = 1;
							take_fuel_platform_set[i] = 1;
						}
					}
					if ((special_platform_indices[i] == 2 || special_platform_indices[i] == 4) && special_platform_type[i] == 3 && jumping_state == 0)
					{
						speed_powerup = 1;
						speed_platform_effect = 1;
					}
					if ((special_platform_indices[i] == 2 || special_platform_indices[i] == 4) && special_platform_type[i] == 4 && jumping_state == 0)
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
					if ((special_platform_indices[i] == 2 || special_platform_indices[i] == 4) && special_platform_type[i] == 1)
					{
						//RenderMesh(meshes["box"], shaders["RedPlatform"], modelMatrix);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix, mapTextures["redcrate"]);
					}
					if ((special_platform_indices[i] == 2 || special_platform_indices[i] == 4) && special_platform_type[i] == 2)
					{
						//RenderMesh(meshes["box"], shaders["YellowPlatform"], modelMatrix);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix, mapTextures["yellowcrate"]);
					}
					if ((special_platform_indices[i] == 2 || special_platform_indices[i] == 4) && special_platform_type[i] == 3)
					{
						//RenderMesh(meshes["box"], shaders["HitPlatformShader"], modelMatrix);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix, mapTextures["orangecrate"]);
					}
					if ((special_platform_indices[i] == 2 || special_platform_indices[i] == 4) && special_platform_type[i] == 4)
					{
						//RenderMesh(meshes["box"], shaders["GreenPlatform"], modelMatrix);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix, mapTextures["greencrate"]);
					}
					else
					{
						if (platforme_parcurse[i][2] == 1)
						{
							//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix);
							RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix, mapTextures["movcrate"]);
						}
						if (platforme_parcurse[i][2] == 0)
						{
							//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix);
							RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix, mapTextures["crate"]);
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
				if (player_position == 1 && contor_pozitie_sfera == i && jumping_state == 0 && is_player_dead == 0)
				{
					//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix3);
					RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix3, mapTextures["movcrate"]);
					platforme_parcurse[contor_pozitie_sfera][player_position] = 1;
					if ((special_platform_indices[i] == 1 || special_platform_indices[i] == 4) && special_platform_type[i] == 1 && jumping_state == 0)
					{
						cout << "YOU ARE DEAD" << endl;
						exit(0);
					}
					if ((special_platform_indices[i] == 1 || special_platform_indices[i] == 4) && special_platform_type[i] == 2 && jumping_state == 0)
					{
						//cout << "YOU TOUCHED YELLOW PLATFORM" << endl;
						if (take_fuel_platform_set[i] == 0)
						{
							take_fuel = 1;
							take_fuel_platform_set[i] = 1;
						}
					}
					if ((special_platform_indices[i] == 1 || special_platform_indices[i] == 4) && special_platform_type[i] == 3 && jumping_state == 0)
					{
						speed_powerup = 1;
						speed_platform_effect = 1;
					}
					if ((special_platform_indices[i] == 1 || special_platform_indices[i] == 4) && special_platform_type[i] == 4 && jumping_state == 0)
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
					if ((special_platform_indices[i] == 1 || special_platform_indices[i] == 4) && special_platform_type[i] == 1)
					{
						//RenderMesh(meshes["box"], shaders["RedPlatform"], modelMatrix3);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix3, mapTextures["redcrate"]);
					}
					if ((special_platform_indices[i] == 1 || special_platform_indices[i] == 4) && special_platform_type[i] == 2)
					{
						//RenderMesh(meshes["box"], shaders["YellowPlatform"], modelMatrix3);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix3, mapTextures["yellowcrate"]);
					}
					if ((special_platform_indices[i] == 1 || special_platform_indices[i] == 4) && special_platform_type[i] == 3)
					{
						//RenderMesh(meshes["box"], shaders["HitPlatformShader"], modelMatrix3);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix3, mapTextures["orangecrate"]);
					}
					if ((special_platform_indices[i] == 1 || special_platform_indices[i] == 4) && special_platform_type[i] == 4)
					{
						//RenderMesh(meshes["box"], shaders["GreenPlatform"], modelMatrix3);
						RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix3, mapTextures["greencrate"]);
					}
					else
					{
						if (platforme_parcurse[i][1] == 1)
						{
							//RenderMesh(meshes["box"], shaders["MovPlatform"], modelMatrix3);
							RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix3, mapTextures["movcrate"]);
						}
						if (platforme_parcurse[i][1] == 0)
						{
							//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix3);
							RenderTextureMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix3, mapTextures["crate"]);
						}
						//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix3);
					}
					//RenderMesh(meshes["box"], shaders["PlatformShader"], modelMatrix3);
				}
			}

		}

	}
}

void Tema3::VerificareCadere()
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

void Tema3::DrawStatusBar()
{
	// incepem desenarea UI-ului

	latura_powerbar = fuel / 200;

	latura_score = score / 200;


		glm::mat4 modelMatrix44 = glm::mat4(1);
		modelMatrix44 = glm::translate(modelMatrix44, glm::vec3(0.45f, 0.5f, 0));
		modelMatrix44 = glm::translate(modelMatrix44, glm::vec3(0.5f * latura_score, 0, 0));
		modelMatrix44 = glm::scale(modelMatrix44, glm::vec3(latura_score / 2, 0.1f / 2, 0));
		//modelMatrix4 = glm::scale(modelMatrix4, glm::vec3(0.5f, 1, 0));
		if (speed_platform_effect == 0)
			RenderMesh(meshes["rectangle"], shaders["UI3"], modelMatrix44);
		if (speed_platform_effect == 1)
			RenderMesh(meshes["rectangle"], shaders["SpeedPowerup2"], modelMatrix44);

		glm::mat4 modelMatrix55 = glm::mat4(1);
		modelMatrix55 = glm::translate(modelMatrix55, glm::vec3(0.45f, 0.5f, 0));
		modelMatrix55 = glm::translate(modelMatrix55, glm::vec3(0.25f, 0, 0));
		modelMatrix55 = glm::scale(modelMatrix55, glm::vec3(0.5f / 2, 0.1f / 2, 0));
		RenderMesh(meshes["rectangle"], shaders["UI2"], modelMatrix55);



	
	if (latura_powerbar > 0)
	{
		glm::mat4 modelMatrix4 = glm::mat4(1);
		modelMatrix4 = glm::translate(modelMatrix4, glm::vec3(0.45f, 0.9f, 0));
		modelMatrix4 = glm::translate(modelMatrix4, glm::vec3(0.5f * latura_powerbar, 0, 0));
		modelMatrix4 = glm::scale(modelMatrix4, glm::vec3(latura_powerbar / 2, 0.1f / 2, 0));
		//modelMatrix4 = glm::scale(modelMatrix4, glm::vec3(0.5f, 1, 0));
		if (speed_platform_effect == 0)
			RenderMesh(meshes["rectangle"], shaders["UI"], modelMatrix4);
		if (speed_platform_effect == 1)
			RenderMesh(meshes["rectangle"], shaders["SpeedPowerup2"], modelMatrix4);

	}

	glm::mat4 modelMatrix5 = glm::mat4(1);
	modelMatrix5 = glm::translate(modelMatrix5, glm::vec3(0.45f, 0.9f, 0));
	modelMatrix5 = glm::translate(modelMatrix5, glm::vec3(0.25f, 0, 0));
	modelMatrix5 = glm::scale(modelMatrix5, glm::vec3(0.5f / 2, 0.1f / 2, 0));
	RenderMesh(meshes["rectangle"], shaders["UI2"], modelMatrix5);


	if (change_speed == 0)
	{
		glm::mat4 modelMatrix7 = glm::mat4(1);
		modelMatrix7 = glm::translate(modelMatrix7, glm::vec3(0.45f, 0.7f, 0));
		modelMatrix7 = glm::translate(modelMatrix7, glm::vec3(0.5f * 0.125f, 0, 0));
		modelMatrix7 = glm::scale(modelMatrix7, glm::vec3(0.125f / 2, 0.1f / 2, 0));
		RenderMesh(meshes["rectangle"], shaders["SpeedPowerup"], modelMatrix7);
	}


	if (change_speed == 1)
	{
		glm::mat4 modelMatrix7 = glm::mat4(1);
		modelMatrix7 = glm::translate(modelMatrix7, glm::vec3(0.45f, 0.7f, 0));
		modelMatrix7 = glm::translate(modelMatrix7, glm::vec3(0.5f * 0.26f, 0, 0));
		modelMatrix7 = glm::scale(modelMatrix7, glm::vec3(0.26f / 2, 0.1f / 2, 0));
		RenderMesh(meshes["rectangle"], shaders["SpeedPowerup"], modelMatrix7);
	}

	if (change_speed == 2)
	{
		glm::mat4 modelMatrix7 = glm::mat4(1);
		modelMatrix7 = glm::translate(modelMatrix7, glm::vec3(0.45f, 0.7f, 0));
		modelMatrix7 = glm::translate(modelMatrix7, glm::vec3(0.5f * 0.375f, 0, 0));
		modelMatrix7 = glm::scale(modelMatrix7, glm::vec3(0.375f / 2, 0.1f / 2, 0));
		RenderMesh(meshes["rectangle"], shaders["SpeedPowerup"], modelMatrix7);
	}

	if (change_speed == 3)
	{
		glm::mat4 modelMatrix7 = glm::mat4(1);
		modelMatrix7 = glm::translate(modelMatrix7, glm::vec3(0.45f, 0.7f, 0));
		modelMatrix7 = glm::translate(modelMatrix7, glm::vec3(0.5f * 0.5f, 0, 0));
		modelMatrix7 = glm::scale(modelMatrix7, glm::vec3(0.5f / 2, 0.1f / 2, 0));
		RenderMesh(meshes["rectangle"], shaders["SpeedPowerup"], modelMatrix7);
	}

	if (change_speed != 4)
		previous_change_speed = change_speed;


	if (change_speed == 4)
	{

		if (previous_change_speed == 0)
		{
			glm::mat4 modelMatrix7 = glm::mat4(1);
			modelMatrix7 = glm::translate(modelMatrix7, glm::vec3(0.45f, 0.7f, 0));
			modelMatrix7 = glm::translate(modelMatrix7, glm::vec3(0.5f * 0.125f, 0, 0));
			modelMatrix7 = glm::scale(modelMatrix7, glm::vec3(0.125f / 2, 0.1f / 2, 0));
			RenderMesh(meshes["rectangle"], shaders["SpeedPowerup2"], modelMatrix7);
		}
		if (previous_change_speed == 1)
		{
			glm::mat4 modelMatrix7 = glm::mat4(1);
			modelMatrix7 = glm::translate(modelMatrix7, glm::vec3(0.45f, 0.7f, 0));
			modelMatrix7 = glm::translate(modelMatrix7, glm::vec3(0.5f * 0.26f, 0, 0));
			modelMatrix7 = glm::scale(modelMatrix7, glm::vec3(0.26f / 2, 0.1f / 2, 0));
			RenderMesh(meshes["rectangle"], shaders["SpeedPowerup2"], modelMatrix7);
		}
		if (previous_change_speed == 2)
		{
			glm::mat4 modelMatrix7 = glm::mat4(1);
			modelMatrix7 = glm::translate(modelMatrix7, glm::vec3(0.45f, 0.7f, 0));
			modelMatrix7 = glm::translate(modelMatrix7, glm::vec3(0.5f * 0.375f, 0, 0));
			modelMatrix7 = glm::scale(modelMatrix7, glm::vec3(0.375f / 2, 0.1f / 2, 0));
			RenderMesh(meshes["rectangle"], shaders["SpeedPowerup2"], modelMatrix7);
		}
		if (previous_change_speed == 3)
		{
			glm::mat4 modelMatrix7 = glm::mat4(1);
			modelMatrix7 = glm::translate(modelMatrix7, glm::vec3(0.45f, 0.7f, 0));
			modelMatrix7 = glm::translate(modelMatrix7, glm::vec3(0.5f * 0.5f, 0, 0));
			modelMatrix7 = glm::scale(modelMatrix7, glm::vec3(0.5f / 2, 0.1f / 2, 0));
			RenderMesh(meshes["rectangle"], shaders["SpeedPowerup2"], modelMatrix7);
		}

	}

	glm::mat4 modelMatrix6 = glm::mat4(1);
	modelMatrix6 = glm::translate(modelMatrix6, glm::vec3(0.45f, 0.7f, 0));
	modelMatrix6 = glm::translate(modelMatrix6, glm::vec3(0.25f, 0, 0));
	modelMatrix6 = glm::scale(modelMatrix6, glm::vec3(0.5f / 2, 0.1f / 2, 0));
	RenderMesh(meshes["rectangle"], shaders["UI2"], modelMatrix6);

	// Terminam desenarea UI-ului
}

void Tema3::FuelManagement()
{
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
}

void Tema3::SpeedManagement()
{
	
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
}

void Tema3::Update(float deltaTimeSeconds)
{
		
	invulnerability_time--;

	//cout << "INVUL : " << invulnerability_time << endl;

	// Render Moon

	glm::mat4 modelMatrix99 = glm::mat4(1);
	modelMatrix99 = glm::translate(modelMatrix99, glm::vec3(-8 , 6.5f, -20));
	modelMatrix99 = glm::scale(modelMatrix99, glm::vec3(3));
	RenderTextureMesh(meshes["sphere"], shaders["ShaderLab9"], modelMatrix99, mapTextures["moon2"]);
	
	// Render background piramids
	
	glm::mat4 modelMatrix68 = glm::mat4(1);
	modelMatrix68 = glm::translate(modelMatrix68, glm::vec3(-12, 1, -20));
	modelMatrix68 = glm::scale(modelMatrix68, glm::vec3(1.5f));
	RenderTextureMesh(meshes["piramida_texturata"], shaders["ShaderLab9"], modelMatrix68, mapTextures["piramida"]);

	glm::mat4 modelMatrix70 = glm::mat4(1);
	modelMatrix70 = glm::translate(modelMatrix70, glm::vec3(-18, 1, -20));
	modelMatrix70 = glm::scale(modelMatrix70, glm::vec3(1.5f));
	RenderTextureMesh(meshes["piramida_texturata"], shaders["ShaderLab9"], modelMatrix70, mapTextures["piramida"]);

	glm::mat4 modelMatrix71 = glm::mat4(1);
	modelMatrix71 = glm::translate(modelMatrix71, glm::vec3(-24, 1, -20));
	modelMatrix71 = glm::scale(modelMatrix71, glm::vec3(1.5f));
	RenderTextureMesh(meshes["piramida_texturata"], shaders["ShaderLab9"], modelMatrix71, mapTextures["piramida"]);

	glm::mat4 modelMatrix98 = glm::mat4(1);
	modelMatrix98 = glm::translate(modelMatrix98, glm::vec3(8, 1, -20));
	modelMatrix98 = glm::scale(modelMatrix98, glm::vec3(1.5f));
	RenderTextureMesh(meshes["piramida_texturata"], shaders["ShaderLab9"], modelMatrix98, mapTextures["piramida"]);

	glm::mat4 modelMatrix101 = glm::mat4(1);
	modelMatrix101 = glm::translate(modelMatrix101, glm::vec3(14, 1, -20));
	modelMatrix101 = glm::scale(modelMatrix101, glm::vec3(1.5f));
	RenderTextureMesh(meshes["piramida_texturata"], shaders["ShaderLab9"], modelMatrix101, mapTextures["piramida"]);

	glm::mat4 modelMatrix102 = glm::mat4(1);
	modelMatrix102 = glm::translate(modelMatrix102, glm::vec3(20, 1, -20));
	modelMatrix102 = glm::scale(modelMatrix102, glm::vec3(1.5f));
	RenderTextureMesh(meshes["piramida_texturata"], shaders["ShaderLab9"], modelMatrix102, mapTextures["piramida"]);

	
	// Render sand ground
	{
		glm::mat4 modelMatrix88 = glm::mat4(1);
		modelMatrix88 = glm::translate(modelMatrix88, glm::vec3(0, 0.75f, 0));
		modelMatrix88 = glm::scale(modelMatrix88, glm::vec3(1.5f));
		RenderTextureMesh(meshes["plane"], shaders["ShaderLab9"], modelMatrix88, mapTextures["sand"]);
	}

	// Render light background
	{
		glm::mat4 modelMatrix8888 = glm::mat4(1);
		modelMatrix8888 = glm::translate(modelMatrix8888, glm::vec3(-10, 0.75f, -60));
		modelMatrix8888 = glm::scale(modelMatrix8888, glm::vec3(5));
		modelMatrix8888 = glm::rotate(modelMatrix8888,RADIANS(90), glm::vec3(1, 0, 0));
		RenderLightMesh(meshes["plane"], shaders["Light"], modelMatrix8888, glm::vec3(1, 1, 1), lightPosition);
	}

	// Render water ground
	{
		glm::mat4 modelMatrix1000 = glm::mat4(1);
		modelMatrix1000 = glm::translate(modelMatrix1000, glm::vec3(0, 0.77f, 0));
		modelMatrix1000 = glm::scale(modelMatrix1000, glm::vec3(0.1015f,0.25f,1.5f));
		RenderTextureMesh(meshes["plane"], shaders["ShaderLab9"], modelMatrix1000, mapTextures["water"]);
	}
	
	// Render left and right props
		
	for (int j = 1; j < 100; j++)
	{
		glm::mat4 modelMatrix66 = glm::mat4(1);
		modelMatrix66 = glm::translate(modelMatrix66, glm::vec3(-5, -0.5f, -15 * j + angularStep));
		if (-15 * j + angularStep > -50 && -15 * j + angularStep < 3)
		{
			if (decor_type_left[j] == 0)
				RenderTextureMesh(meshes["textura2"], shaders["ShaderLab9"], modelMatrix66, mapTextures["piramida"]);
			else if (decor_type_left[j] == 1)
				RenderTextureMesh(meshes["textura3"], shaders["ShaderLab9"], modelMatrix66, mapTextures["piramida"]);
			else if (decor_type_left[j] == 2)
			{
				modelMatrix66 = glm::translate(modelMatrix66, glm::vec3(-2,-0.25f,0));
				RenderTextureMesh(meshes["textura4"], shaders["ShaderLab9"], modelMatrix66, mapTextures["piramida"]);
			}
			else if (decor_type_left[j] == 3)
			{
				modelMatrix66 = glm::translate(modelMatrix66, glm::vec3(-2, -0.25f, 0));
				RenderTextureMesh(meshes["textura5"], shaders["ShaderLab9"], modelMatrix66, mapTextures["piramida"]);
			}
			else if (decor_type_left[j] == 4)
			{
				modelMatrix66 = glm::translate(modelMatrix66, glm::vec3(-2, -0.25f, 0));
				RenderTextureMesh(meshes["textura6"], shaders["ShaderLab9"], modelMatrix66, mapTextures["piramida"]);
			}
		}

		if (decor_type_left[j] != 4)
		{
			glm::mat4 modelMatrix77 = glm::mat4(1);
			modelMatrix77 = glm::translate(modelMatrix77, glm::vec3(5, -0.5f, -15 * j + angularStep));
			if (-15 * j + angularStep > -50 && -15 * j + angularStep < 3)
			{
				if (decor_type_right[j] == 0)
				{
					modelMatrix77 = glm::translate(modelMatrix77, glm::vec3(-1.5f, -0.25f, 0));
					RenderTextureMesh(meshes["textura2"], shaders["ShaderLab9"], modelMatrix77, mapTextures["piramida"]);
				}
				else if (decor_type_right[j] == 1)
				{
					modelMatrix77 = glm::translate(modelMatrix77, glm::vec3(-1.5f, -0.25f, 0));
					RenderTextureMesh(meshes["textura3"], shaders["ShaderLab9"], modelMatrix77, mapTextures["piramida"]);
				}
				else if (decor_type_right[j] == 2)
				{
					modelMatrix77 = glm::translate(modelMatrix77, glm::vec3(-1.5f, -0.25f, 0));
					RenderTextureMesh(meshes["textura4"], shaders["ShaderLab9"], modelMatrix77, mapTextures["piramida"]);
				}
				else if (decor_type_right[j] == 3)
				{
					modelMatrix77 = glm::translate(modelMatrix77, glm::vec3(-1.5f, -0.25f, 0));
					RenderTextureMesh(meshes["textura5"], shaders["ShaderLab9"], modelMatrix77, mapTextures["piramida"]);
				}
			}
		}
	}
	// Render the point light in the scene
	
	glm::mat4 modelMatrix66 = glm::mat4(1);
	modelMatrix66 = glm::translate(modelMatrix66, glm::vec3(0, 0.01f, 0));
	modelMatrix66 = glm::scale(modelMatrix66, glm::vec3(0.25f));
	RenderLightMesh(meshes["plane"], shaders["Light"], modelMatrix66, glm::vec3(1,1,0), lightPosition);
	
	angularStep += deltaTimeSeconds;

	factor_deformare += 5 * deltaTimeSeconds;
	
	if (y_pickups <= 1.5f)
		semafor = false;
	if (y_pickups >= 1.75f)
		semafor = true;
	if (semafor == false)
	{
		y_pickups = y_pickups + 0.005f;
	}
	if (semafor == true)
	{
		y_pickups = y_pickups - 0.005f;
	}

	if (score == 100)
	{
		score = 0;
		change_platforms = 1;
	}

	previous_score = score;

	DrawStatusBar();

	FuelManagement();

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
		angularStep += 0.25f;
	}

	if (change_speed == -1)
	{
		angularStep -= 0.01f;
	}

	SpeedManagement();

	DrawPlayer(deltaTimeSeconds);

	DrawPlatforms();
		
	VerificareCadere();

}

void Tema3::FrameEnd()
{
	//DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema3::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
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

void Tema3::RenderSpecialMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix,  float factor_deformare)
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

void Tema3::RenderTextureMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	shader->Use();
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	if (texture1)
	{
		//TODO : activate texture location 0
		glActiveTexture(GL_TEXTURE0);
		//TODO : Bind the texture1 ID
		glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
		//TODO : Send texture uniform value
		glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
	}

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema3::RenderLightMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, const glm::vec3& lightpos)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Set shader uniforms for light & material properties
	// TODO: Set light position uniform
	int light_position = glGetUniformLocation(shader->program, "light_position");
	glUniform3f(light_position, lightpos.x, lightpos.y, lightpos.z);

	int light_direction = glGetUniformLocation(shader->program, "light_direction");
	glUniform3f(light_direction, lightDirection.x, lightDirection.y, lightDirection.z);

	// TODO: Set eye position (camera position) uniform
	glm::vec3 eyePosition = GetSceneCamera()->transform->GetWorldPosition();
	int eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

	// TODO: Set material property uniforms (shininess, kd, ks, object color) 
	int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(material_shininess, materialShininess);

	int material_kd = glGetUniformLocation(shader->program, "material_kd");
	glUniform1f(material_kd, materialKd);

	int material_ks = glGetUniformLocation(shader->program, "material_ks");
	glUniform1f(material_ks, materialKs);

	int object_color = glGetUniformLocation(shader->program, "object_color");
	glUniform3f(object_color, color.r, color.g, color.b);

	int spotLight = glGetUniformLocation(shader->program, "spotLight");
	glUniform1i(spotLight, isSpotlight);



	shader->Use();
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	//if (texture1)
	//{
	//	//TODO : activate texture location 0
	//	glActiveTexture(GL_TEXTURE0);
	//	//TODO : Bind the texture1 ID
	//	glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
	//	//TODO : Send texture uniform value
	//	glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
	//}

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Tema3::OnInputUpdate(float deltaTime, int mods)
{
	// move the camera only if MOUSE_RIGHT button is pressed
	
	float speed = 20;
	
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float cameraSpeed = 2.0f;

		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 right = GetSceneCamera()->transform->GetLocalOXVector();
		glm::vec3 forward = GetSceneCamera()->transform->GetLocalOZVector();
		forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));

		//// Control light position using on W, A, S, D, E, Q
		if (window->KeyHold(GLFW_KEY_UP)) lightPosition -= forward * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_LEFT)) lightPosition -= right * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_DOWN)) lightPosition += forward * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_RIGHT)) lightPosition += right * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_M)) lightPosition += up * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_N)) lightPosition -= up * deltaTime * speed;
		
		
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

void Tema3::OnKeyPress(int key, int mods)
{
	// add key press event
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

	if (key == GLFW_KEY_F) {
		if (isSpotlight == 0)
			isSpotlight = 1;
		else if (isSpotlight == 1)
			isSpotlight = 0;
	}

	if (key == GLFW_KEY_P) 
	{
		camera->TranslateUpword(5);
		camera->TranslateForward(-5);
		camera->RotateFirstPerson_OX(RADIANS(-30));
		//camera->RotateFirstPerson_OY(90);
	}
	if (key == GLFW_KEY_O)
	{
		camera->TranslateUpword(1);
		camera->TranslateForward(-3);
		camera->TranslateRight(-5);
		camera->RotateFirstPerson_OY(RADIANS(-40));
	}
	if (key == GLFW_KEY_1)
	{
		player_skin = 1;
	}
	if (key == GLFW_KEY_2)
	{
		player_skin = 2;
	}
	if (key == GLFW_KEY_3)
	{
		player_skin = 3;
	}
}

void Tema3::OnKeyRelease(int key, int mods)
{

}

void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
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

void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema3::OnWindowResize(int width, int height)
{
}
