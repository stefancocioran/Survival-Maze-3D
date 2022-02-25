#include "lab_m1/Tema2/Tema2.h"
#include "lab_m1/Tema2/MazeGenerator.h"
#include "lab_m1/Tema2/object3D.h"
#include "lab_m1/Tema2/object2D.h"
#include "lab_m1/Tema2/transform3D.h"
#include <string>
#include <set>
#include <math.h>


using namespace std;
using namespace m1;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
	cameraSpeed = 3.0f;

	rotationAngle = 0;
	rotationUpDown = 0;

	N = 10, M = 10;

	float wallLength = 3.f;
	float playersScaleToCell = 8 * 4.6f;


	float wallWidth = wallLength / 24;
	float wallHeight = wallLength / 2;


	float bodyPartSize = wallLength / playersScaleToCell;

	limbSpace = bodyPartSize * 0.2f;
	eyeDistance = limbSpace * 1.5f;

	legDimensions.x = bodyPartSize;
	legDimensions.y = bodyPartSize * 3;
	legDimensions.z = bodyPartSize;

	shoulderDimensions.x = bodyPartSize + limbSpace;
	shoulderDimensions.y = bodyPartSize;
	shoulderDimensions.z = bodyPartSize;

	armDimensions.x = bodyPartSize;
	armDimensions.y = bodyPartSize * 2;
	armDimensions.z = bodyPartSize;

	headDimensions.x = bodyPartSize * 1.75f;
	headDimensions.y = bodyPartSize * 1.25f;
	headDimensions.z = bodyPartSize;

	hairDimensions.x = headDimensions.x;
	hairDimensions.y = bodyPartSize * 0.5f;
	hairDimensions.z = headDimensions.z;

	eyeDimensions.x = bodyPartSize * 0.2f;
	eyeDimensions.y = bodyPartSize * 0.2f;
	eyeDimensions.z = bodyPartSize * 0.2f;

	pupilDimensions.x = bodyPartSize * 0.2f;
	pupilDimensions.y = bodyPartSize * 0.2f;
	pupilDimensions.z = bodyPartSize * 0.2f;

	bodyDimensions.x = legDimensions.x * 2 + limbSpace;
	bodyDimensions.y = bodyPartSize * 3;
	bodyDimensions.z = bodyPartSize;

	wallDimensions.x = wallLength;
	wallDimensions.y = wallHeight;
	wallDimensions.z = wallWidth;

	healthbarLength = 7.5f * bodyPartSize / 75;
	healthbarWidth = 1.75f * bodyPartSize / 75;

	healthbars = 5;		// healthbar scale   
	lives = healthbars; // initial health
	timeLeft = timerSeconds;

	playerWidth = shoulderDimensions.x * 2 + bodyDimensions.x;
	playerHeight = legDimensions.y + bodyDimensions.y + headDimensions.y + hairDimensions.y;

	patrolDistance = wallDimensions.x - wallDimensions.z - bodyDimensions.z / 2 - playerWidth / 2 - (playerWidth / 2 - bodyDimensions.z / 2);
	centerOffset = glm::vec3(wallDimensions.x / 2 - shoulderDimensions.x, 0, wallDimensions.x / 2 - bodyDimensions.z);

	set<pair<int, int>> mazeWallsJoints = MazeGenerator::GenerateMaze(N, M);
	CreateMaze(N, M, mazeWallsJoints);

	float blockSize = wallLength;

	isThirdPerson = true;

	fov = 50.f;
	znear = 0.01f;
	zfar = 200.0f;

	left = -5.f;
	right = 5.f;
	bottom = -5.f;
	top = 5.f;

	nextFire = 0;

	cameraFirst = new implemented::GameCamera();
	cameraThird = new implemented::GameCamera();
	cameraMovement = new implemented::GameCamera();

	cameraFirst->Set(glm::vec3(playerX + bodyDimensions.x / 2, playerHeight - hairDimensions.y - headDimensions.y / 2, playerZ + headDimensions.z + 0.2f),
		glm::vec3(playerX + bodyDimensions.x / 2, playerHeight - hairDimensions.y - headDimensions.y / 2, playerZ + headDimensions.z + 0.5f),
		glm::vec3(0, 1, 0));

	cameraFirst->distanceToTarget = 0.01f;

	cameraThird->Set(glm::vec3(playerX + bodyDimensions.x / 2, playerHeight - hairDimensions.y - headDimensions.y / 2, playerZ - 1.f),
		glm::vec3(playerX + bodyDimensions.x / 2, playerHeight - hairDimensions.y - headDimensions.y / 2, playerZ + 0.5f),
		glm::vec3(0, 1, 0));


	cameraThird->distanceToTarget = 1 + headDimensions.z / 2;

	cameraMovement->Set(glm::vec3(playerX + bodyDimensions.x / 2, playerHeight - hairDimensions.y - headDimensions.y / 2, playerZ + headDimensions.z / 2),
		glm::vec3(playerX + bodyDimensions.x / 2, playerHeight - hairDimensions.y - headDimensions.y / 2, playerZ + headDimensions.z + 0.5f),
		glm::vec3(0, 1, 0));

	cameraMain = cameraThird;


	Mesh* leg = object3D::Box("leg", glm::vec3(0), legDimensions.x, legDimensions.z, legDimensions.y, pantsColor);
	AddMeshToList(leg);

	Mesh* armPlayer = object3D::Box("armPlayer", glm::vec3(0), armDimensions.x, armDimensions.z, armDimensions.y, skinColor);
	AddMeshToList(armPlayer);

	Mesh* armEnemy = object3D::Box("armEnemy", glm::vec3(0), armDimensions.x, armDimensions.z, armDimensions.y, skinColorEnemy);
	AddMeshToList(armEnemy);

	Mesh* eyePlayer = object3D::Box("eyePlayer", glm::vec3(0), eyeDimensions.x, eyeDimensions.z, eyeDimensions.y, glm::vec3(1));
	AddMeshToList(eyePlayer);

	Mesh* eyeEnemy = object3D::Box("eyeEnemy", glm::vec3(0), eyeDimensions.x, eyeDimensions.z, eyeDimensions.y, glm::vec3(0));
	AddMeshToList(eyeEnemy);

	Mesh* pupilPlayer = object3D::Box("pupilPlayer", glm::vec3(0), pupilDimensions.x, pupilDimensions.z, pupilDimensions.y, eyeColor);
	AddMeshToList(pupilPlayer);

	Mesh* pupilEnemy = object3D::Box("pupilEnemy", glm::vec3(0), pupilDimensions.x, pupilDimensions.z, pupilDimensions.y, eyeColorEnemy);
	AddMeshToList(pupilEnemy);

	Mesh* headPlayer = object3D::Box("headPlayer", glm::vec3(0), headDimensions.x, headDimensions.z, headDimensions.y, skinColor);
	AddMeshToList(headPlayer);

	Mesh* headEnemy = object3D::Box("headEnemy", glm::vec3(0), headDimensions.x, headDimensions.z, headDimensions.y, skinColorEnemy);
	AddMeshToList(headEnemy);

	Mesh* hair = object3D::Box("hair", glm::vec3(0), hairDimensions.x, hairDimensions.z, hairDimensions.y, hairColor);
	AddMeshToList(hair);

	Mesh* body = object3D::Box("body", glm::vec3(0), bodyDimensions.x, bodyDimensions.z, bodyDimensions.y, shirtColor);
	AddMeshToList(body);

	Mesh* shoulder = object3D::Box("shoulder", glm::vec3(0), shoulderDimensions.x, shoulderDimensions.z, shoulderDimensions.y, shirtColor);
	AddMeshToList(shoulder);

	Mesh* wallMesh = object3D::Box("wall", glm::vec3(0), wallLength, wallWidth, wallHeight, wallColor);
	AddMeshToList(wallMesh);

	// Create healthbar 
	Mesh* healthbarEdge = object2D::Rectangle("healthbarEdge", glm::vec3(0), healthbarLength, healthbarWidth, healthColor);
	AddMeshToList(healthbarEdge);
	Mesh* healthbar = object2D::Rectangle("healthbar", glm::vec3(0), healthbarLength, healthbarWidth, healthColor, true);
	AddMeshToList(healthbar);

	Mesh* timerEdge = object2D::Rectangle("timerEdge", glm::vec3(0), healthbarLength, healthbarWidth, timeColor);
	AddMeshToList(timerEdge);
	Mesh* timer = object2D::Rectangle("timer", glm::vec3(0), healthbarLength, healthbarWidth, timeColor, true);
	AddMeshToList(timer);


	// Floor
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(0, 0,  0), floorColor),
			VertexFormat(glm::vec3(M * blockSize, 0,  0), floorColor),
			VertexFormat(glm::vec3(0, 0, N * blockSize), floorColor),
			VertexFormat(glm::vec3(M * blockSize, 0,  N * blockSize), floorColor)
		};

		vector<unsigned int> indices =
		{
			0, 3, 1,
			0, 2, 3
		};


		meshes["floor"] = new Mesh("floor");
		meshes["floor"]->InitFromData(vertices, indices);
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}


	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader* shader = new Shader("LabShader");
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, znear, zfar);
}


void Tema2::CreateMaze(int N, int M, set<pair<int, int>> mazeWallsJoints) {

	int playerStartingCell = M / 2 + M * (N / 2);
	playerX = wallDimensions.x * (playerStartingCell % M) + centerOffset.x;
	playerZ = wallDimensions.x * (playerStartingCell / M) + centerOffset.z;

	occupiedCells.push_back(playerStartingCell);
	occupiedPowerups.push_back(playerStartingCell);

	for (int i = 0; i <= N + M; ++i) {

		int cell = rand() % (N * M);
		int type = rand() % 3;

		bool done = false;
		while (std::find(occupiedPowerups.begin(), occupiedPowerups.end(), cell) != occupiedPowerups.end()) {
			cell = rand() % (N * M);
		}

		glm::vec3 color;

		switch (type) {
		case 0:
			color = glm::vec3(0, 1, 0);
			break;
		case 1:
			color = glm::vec3(1, 0, 0);
			break;
		case 2:
			color = glm::vec3(0, 0, 1);
			break;
		}

		powerup newPowerup{
			type,
			color,
			wallDimensions.x / 2 + wallDimensions.x * (cell % M),
			0,
			wallDimensions.x / 2 + wallDimensions.x * (cell / M)
		};

		powerups.push_back(newPowerup);
		occupiedPowerups.push_back(cell);

	}

	for (int i = 0; i <= (N + M) * 1.5; ++i)
	{
		int cell = rand() % (N * M);
		while (std::find(occupiedCells.begin(), occupiedCells.end(), cell) != occupiedCells.end()) {
			cell = rand() % (N * M);
		}

		occupiedCells.push_back(cell);

		enemy newEnemy{
			wallDimensions.x * (cell % M) + wallDimensions.z + shoulderDimensions.x + (bodyDimensions.z / 2 - bodyDimensions.x / 2),
			wallDimensions.x * (cell / M) + wallDimensions.z / 2 + (playerWidth / 2 - bodyDimensions.z / 2),
			wallDimensions.x * (cell % M) + wallDimensions.z + shoulderDimensions.x + (bodyDimensions.z / 2 - bodyDimensions.x / 2) + playerWidth / 2,
			playerHeight / 2,
			wallDimensions.x * (cell / M) + wallDimensions.z / 2 + (playerWidth / 2 - bodyDimensions.z / 2) + bodyDimensions.z / 2,
			0,
			0,
			0,
			cell
		};

		enemies.push_back(newEnemy);
	}


	// create inner walls
	for (pair<int, int> pair : mazeWallsJoints) {
		int col = pair.first % M;
		int row = pair.first / M;

		float x = col * wallDimensions.x;
		float z = row * wallDimensions.x;

		string direction;
		if (abs(pair.first - pair.second) > 1) {
			direction = "side";
			z += wallDimensions.x;
		}
		else {
			x += wallDimensions.x;
			z += wallDimensions.x;

			direction = "front";
		}

		wall newWall = { x, z, direction };
		innerWalls.push_back(newWall);
	}

	// create outer walls
	for (int i = 0; i < M; ++i) {
		wall outerDown = { i * wallDimensions.x, 0, "side" };
		outerWalls.push_back(outerDown);
		wall outerUp = { i * wallDimensions.x, N * wallDimensions.x, "side" };
		outerWalls.push_back(outerUp);
	}

	for (int i = 1; i <= N; ++i) {
		wall outerRight = { 0, i * wallDimensions.x, "front" };
		outerWalls.push_back(outerRight);
		wall outerLeft = { M * wallDimensions.x, i * wallDimensions.x, "front" };
		outerWalls.push_back(outerLeft);
	}

	int index = rand() % (outerWalls.size() - 1);
	exitWall = outerWalls.at(index);
	outerWalls.erase(outerWalls.begin() + index);
}

void Tema2::FrameStart()
{
	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(roomColor.x, roomColor.y, roomColor.z, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// Sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

// Collisions

// Sphere - BOX
bool Tema2::ProjectileFloorCollision(glm::vec3 sphere, float radius) {
	// get box closest point to sphere center by clamping

	float maxX = M * wallDimensions.x, maxZ = N * wallDimensions.x;

	float x = max(-maxX, min(sphere.x, maxX));
	float y = max(-10.f, min(sphere.y, 0.f));
	float z = max(-maxZ, min(sphere.z, maxZ));


	// this is the same as isPointInsideSphere
	float distance = sqrt((x - sphere.x) * (x - sphere.x) +
		(y - sphere.y) * (y - sphere.y) +
		(z - sphere.z) * (z - sphere.z));

	return distance < radius;
}


// Sphere - BOX
bool Tema2::ProjectileWallCollision(glm::vec3 sphere, float radius, wall wall) {
	// get box closest point to sphere center by clamping

	float minX, maxX, minZ, maxZ;

	if (wall.direction == "side") {
		minZ = wall.z - wallDimensions.z / 2;
		maxZ = wall.z + wallDimensions.z / 2;
		minX = wall.x;
		maxX = wall.x + wallDimensions.x;
	}
	else {
		minZ = wall.z - wallDimensions.x;
		maxZ = wall.z;
		minX = wall.x - wallDimensions.z / 2;
		maxX = wall.x + wallDimensions.z / 2;
	}


	float x = max(minX, min(sphere.x, maxX));
	float y = max(0.f, min(sphere.y, wallDimensions.y));
	float z = max(minZ, min(sphere.z, maxZ));


	// this is the same as isPointInsideSphere
	float distance = sqrt((x - sphere.x) * (x - sphere.x) +
		(y - sphere.y) * (y - sphere.y) +
		(z - sphere.z) * (z - sphere.z));

	return distance < radius;
}

bool Tema2::ProjectileEnemyCollision(glm::vec3 sphere, float radius, enemy enemy) {
	// get box closest point to sphere center by clamping

	float minX, maxX, minZ, maxZ;

	if (enemy.phase == 0 || enemy.phase == 2) {
		minZ = enemy.cz - bodyDimensions.z / 2;
		maxZ = enemy.cz + bodyDimensions.z / 2;
		minX = enemy.cx - playerWidth / 2;
		maxX = enemy.cx + playerWidth / 2;
	}
	else {
		minZ = enemy.cz - playerWidth / 2;
		maxZ = enemy.cz + playerWidth / 2;
		minX = enemy.cx - bodyDimensions.z / 2;
		maxX = enemy.cx + bodyDimensions.z / 2;
	}

	float x = max(minX, min(sphere.x, maxX));
	float y = max(0.f, min(sphere.y, playerHeight));
	float z = max(minZ, min(sphere.z, maxZ));

	// this is the same as isPointInsideSphere
	float distance = sqrt((x - sphere.x) * (x - sphere.x) +
		(y - sphere.y) * (y - sphere.y) +
		(z - sphere.z) * (z - sphere.z));

	return distance < radius;
}

bool Tema2::PlayerPowerUpCollision(glm::vec3 playerPos, glm::vec3 sphere, float radius) {
	// get box closest point to sphere center by clamping

	float playerMinX = playerPos.x - bodyDimensions.x, playerMaxX = playerPos.x + bodyDimensions.x;
	float playerMinZ = playerPos.z - bodyDimensions.x, playerMaxZ = playerPos.z + bodyDimensions.x;

	float x = max(playerMinX, min(sphere.x, playerMaxX));
	float y = max(0.f, min(sphere.y, playerHeight));
	float z = max(playerMinZ, min(sphere.z, playerMaxZ));

	// this is the same as isPointInsideSphere
	float distance = sqrt((x - sphere.x) * (x - sphere.x) +
		(y - sphere.y) * (y - sphere.y) +
		(z - sphere.z) * (z - sphere.z));

	return distance < radius;
}


bool Tema2::PlayerEnemyCollision(glm::vec3 playerPos, enemy enemy) {
	// get box closest point to sphere center by clamping

	float enemyMinX, enemyMaxX, enemyMinZ, enemyMaxZ;
	float playerMinX = playerPos.x - bodyDimensions.x, playerMaxX = playerPos.x + bodyDimensions.x;
	float playerMinZ = playerPos.z - bodyDimensions.x, playerMaxZ = playerPos.z + bodyDimensions.x;

	if (enemy.phase == 0 || enemy.phase == 2) {
		enemyMinZ = enemy.cz - bodyDimensions.z / 2;
		enemyMaxZ = enemy.cz + bodyDimensions.z / 2;
		enemyMinX = enemy.cx - playerWidth / 2;
		enemyMaxX = enemy.cx + playerWidth / 2;
	}
	else {
		enemyMinZ = enemy.cz - playerWidth / 2;
		enemyMaxZ = enemy.cz + playerWidth / 2;
		enemyMinX = enemy.cx - bodyDimensions.z / 2;
		enemyMaxX = enemy.cx + bodyDimensions.z / 2;
	}

	return (playerMinX <= enemyMaxX && playerMaxX >= enemyMinX) &&
		(playerMinZ <= enemyMaxZ && playerMaxZ >= enemyMinZ);
}


bool Tema2::PlayerWallCollision(glm::vec3 playerPos, wall wall) {
	// get box closest point to sphere center by clamping

	float wallMinX, wallMaxX, wallMinZ, wallMaxZ;
	float playerMinX = playerPos.x - bodyDimensions.x, playerMaxX = playerPos.x + bodyDimensions.x;
	float playerMinZ = playerPos.z - bodyDimensions.x, playerMaxZ = playerPos.z + bodyDimensions.x;

	if (wall.direction == "side") {
		wallMinZ = wall.z - wallDimensions.z / 2;
		wallMaxZ = wall.z + wallDimensions.z / 2;
		wallMinX = wall.x;
		wallMaxX = wall.x + wallDimensions.x;
	}
	else {
		wallMinZ = wall.z - wallDimensions.x;
		wallMaxZ = wall.z;
		wallMinX = wall.x - wallDimensions.z / 2;
		wallMaxX = wall.x + wallDimensions.z / 2;
	}

	return (playerMinX <= wallMaxX && playerMaxX >= wallMinX) &&
		(0.f <= wallDimensions.y && playerHeight >= 0) &&
		(playerMinZ <= wallMaxZ && playerMaxZ >= wallMinZ);
}


bool Tema2::EscapedMaze() {

	if (exitWall.direction == "side") {
		if (exitWall.x + wallDimensions.x > cameraFirst->position.x && cameraFirst->position.x > exitWall.x) {
			if ((cameraFirst->position.z < 0 - bodyDimensions.x / 2) || (cameraFirst->position.z > N * wallDimensions.x + bodyDimensions.x / 2)) {
				return true;
			}
		}
	}
	else {
		if (exitWall.z > cameraFirst->position.z && cameraFirst->position.z > exitWall.z - wallDimensions.x) {
			if (cameraFirst->position.x < 0 - bodyDimensions.x / 2 || (cameraFirst->position.x > M * wallDimensions.x + bodyDimensions.x / 2)) {
				return true;
			}
		}
	}

	return false;
}


void Tema2::DrawPlayer() {

	playerX = cameraMovement->position.x - bodyDimensions.x / 2;
	playerZ = cameraMovement->position.z - headDimensions.z / 2;

	glm::vec3 offset = glm::vec3(playerX, 0, playerZ);
	glm::mat4 modelMatrix = glm::mat4(1), bodyMatrix = glm::mat4(1);

	bodyMatrix = glm::translate(glm::mat4(1), offset + glm::vec3(0, legDimensions.y, 0));

	bodyMatrix = glm::translate(bodyMatrix, glm::vec3(bodyDimensions.x / 2, bodyDimensions.y / 2, bodyDimensions.z / 2));
	bodyMatrix *= transform3D::RotateOY(rotationAngle);
	bodyMatrix = glm::translate(bodyMatrix, glm::vec3(-bodyDimensions.x / 2, -bodyDimensions.y / 2, -bodyDimensions.z / 2));

	RenderSimpleMesh(meshes["body"], shaders["VertexColor"], bodyMatrix);
	modelMatrix = bodyMatrix * glm::translate(glm::mat4(1), glm::vec3(0, -legDimensions.y, 0));
	RenderSimpleMesh(meshes["leg"], shaders["VertexColor"], modelMatrix);

	modelMatrix = bodyMatrix * glm::translate(glm::mat4(1), glm::vec3(legDimensions.x + limbSpace, -legDimensions.y, 0));
	RenderSimpleMesh(meshes["leg"], shaders["VertexColor"], modelMatrix);

	modelMatrix = bodyMatrix * glm::translate(glm::mat4(1), glm::vec3(-shoulderDimensions.x, bodyDimensions.y - shoulderDimensions.y, 0));
	RenderSimpleMesh(meshes["shoulder"], shaders["VertexColor"], modelMatrix);

	modelMatrix = bodyMatrix * glm::translate(glm::mat4(1), glm::vec3(bodyDimensions.x, bodyDimensions.y - shoulderDimensions.y, 0));
	RenderSimpleMesh(meshes["shoulder"], shaders["VertexColor"], modelMatrix);

	modelMatrix = bodyMatrix * glm::translate(glm::mat4(1), glm::vec3(-armDimensions.x - limbSpace, bodyDimensions.y - shoulderDimensions.y - armDimensions.y, 0));
	RenderSimpleMesh(meshes["armPlayer"], shaders["VertexColor"], modelMatrix);

	modelMatrix = bodyMatrix * glm::translate(glm::mat4(1), glm::vec3(bodyDimensions.x + limbSpace, bodyDimensions.y - shoulderDimensions.y - armDimensions.y, 0));
	RenderSimpleMesh(meshes["armPlayer"], shaders["VertexColor"], modelMatrix);

	modelMatrix = bodyMatrix * glm::translate(glm::mat4(1), glm::vec3((bodyDimensions.x - headDimensions.x) / 2, bodyDimensions.y, 0));
	RenderSimpleMesh(meshes["headPlayer"], shaders["VertexColor"], modelMatrix);

	modelMatrix = bodyMatrix * glm::translate(glm::mat4(1), glm::vec3((bodyDimensions.x - headDimensions.x) / 2, bodyDimensions.y + headDimensions.y, 0));
	RenderSimpleMesh(meshes["hair"], shaders["VertexColor"], modelMatrix);

	modelMatrix = bodyMatrix * glm::translate(glm::mat4(1), glm::vec3((bodyDimensions.x - headDimensions.x) / 2 + eyeDistance, bodyDimensions.y + headDimensions.y / 2, headDimensions.z - eyeDimensions.z + 0.0001f));
	RenderSimpleMesh(meshes["eyePlayer"], shaders["VertexColor"], modelMatrix);

	modelMatrix = bodyMatrix * glm::translate(glm::mat4(1), glm::vec3(bodyDimensions.x / 2 + headDimensions.x / 2 - eyeDistance - eyeDimensions.x, bodyDimensions.y + headDimensions.y / 2, headDimensions.z - eyeDimensions.z + 0.0001f));
	RenderSimpleMesh(meshes["eyePlayer"], shaders["VertexColor"], modelMatrix);

	modelMatrix = bodyMatrix * glm::translate(glm::mat4(1), glm::vec3((bodyDimensions.x - headDimensions.x) / 2 + eyeDistance + pupilDimensions.x, bodyDimensions.y + headDimensions.y / 2, headDimensions.z - eyeDimensions.z + 0.0001f));
	RenderSimpleMesh(meshes["pupilPlayer"], shaders["VertexColor"], modelMatrix);

	modelMatrix = bodyMatrix * glm::translate(glm::mat4(1), glm::vec3(bodyDimensions.x / 2 + headDimensions.x / 2 - eyeDistance - eyeDimensions.x - pupilDimensions.x, bodyDimensions.y + headDimensions.y / 2, headDimensions.z - eyeDimensions.z + 0.0001f));
	RenderSimpleMesh(meshes["pupilPlayer"], shaders["VertexColor"], modelMatrix);
}

void Tema2::DrawHealthTime() {

	glm::mat4 cameraMatrix, modelMatrix;
	cameraMatrix = glm::translate(glm::mat4(1), cameraMain->position);
	cameraMatrix *= transform3D::RotateOY(rotationAngle);
	cameraMatrix *= transform3D::RotateOX(-rotationUpDown);

	modelMatrix = cameraMatrix * glm::translate(glm::mat4(1), glm::vec3(-0.045 - healthbarLength / 2, -healthbarWidth / 2 + 0.022, 0.05f));
	modelMatrix *= transform3D::Scale(timeLeft * 1 / timerSeconds, 1, 1);
	RenderSimpleMesh(meshes["timer"], shaders["VertexColor"], modelMatrix);

	modelMatrix = cameraMatrix * glm::translate(glm::mat4(1), glm::vec3(-0.045 - healthbarLength / 2, -healthbarWidth / 2 + 0.025, 0.05f));
	modelMatrix *= transform3D::Scale(lives * 1 / healthbars, 1, 1);
	RenderSimpleMesh(meshes["healthbar"], shaders["VertexColor"], modelMatrix);

	modelMatrix = cameraMatrix * glm::translate(glm::mat4(1), glm::vec3(-0.045 - healthbarLength / 2, -healthbarWidth / 2 + 0.025, 0.05f));
	RenderSimpleMesh(meshes["healthbarEdge"], shaders["VertexColor"], modelMatrix);
}


void Tema2::DrawEnemies() {
	for (enemy& enemy : enemies) {
		glm::vec3 offset = glm::vec3(enemy.xPos, 0, enemy.zPos);
		glm::mat4 modelMatrix = glm::mat4(1), bodyMatrix = glm::mat4(1);

		bodyMatrix = glm::translate(glm::mat4(1), offset + glm::vec3(0, legDimensions.y, 0));
		bodyMatrix = glm::translate(bodyMatrix, glm::vec3(bodyDimensions.x / 2, bodyDimensions.y / 2, bodyDimensions.z / 2));
		bodyMatrix *= transform3D::RotateOY(enemy.phase * RADIANS(90.f));
		bodyMatrix = glm::translate(bodyMatrix, glm::vec3(-bodyDimensions.x / 2, -bodyDimensions.y / 2, -bodyDimensions.z / 2));

		RenderSimpleMesh(meshes["body"], shaders["LabShader"], bodyMatrix, shirtColor, enemy.hit);

		modelMatrix = bodyMatrix * glm::translate(glm::mat4(1), glm::vec3(0, -legDimensions.y, 0));
		RenderSimpleMesh(meshes["leg"], shaders["LabShader"], modelMatrix, pantsColor, enemy.hit);

		modelMatrix = bodyMatrix * glm::translate(glm::mat4(1), glm::vec3(legDimensions.x + limbSpace, -legDimensions.y, 0));
		RenderSimpleMesh(meshes["leg"], shaders["LabShader"], modelMatrix, pantsColor, enemy.hit);

		modelMatrix = bodyMatrix * glm::translate(glm::mat4(1), glm::vec3(-shoulderDimensions.x, bodyDimensions.y - shoulderDimensions.y, 0));
		RenderSimpleMesh(meshes["shoulder"], shaders["LabShader"], modelMatrix, shirtColor, enemy.hit);

		modelMatrix = bodyMatrix * glm::translate(glm::mat4(1), glm::vec3(bodyDimensions.x, bodyDimensions.y - shoulderDimensions.y, 0));
		RenderSimpleMesh(meshes["shoulder"], shaders["LabShader"], modelMatrix, shirtColor, enemy.hit);

		modelMatrix = bodyMatrix * glm::translate(glm::mat4(1), glm::vec3(-armDimensions.x - limbSpace, bodyDimensions.y - shoulderDimensions.y - armDimensions.y, 0));
		RenderSimpleMesh(meshes["armEnemy"], shaders["LabShader"], modelMatrix, skinColorEnemy, enemy.hit);

		modelMatrix = bodyMatrix * glm::translate(glm::mat4(1), glm::vec3(bodyDimensions.x + limbSpace, bodyDimensions.y - shoulderDimensions.y - armDimensions.y, 0));
		RenderSimpleMesh(meshes["armEnemy"], shaders["LabShader"], modelMatrix, skinColorEnemy, enemy.hit);

		modelMatrix = bodyMatrix * glm::translate(glm::mat4(1), glm::vec3((bodyDimensions.x - headDimensions.x) / 2, bodyDimensions.y, 0));
		RenderSimpleMesh(meshes["headEnemy"], shaders["LabShader"], modelMatrix, skinColorEnemy, enemy.hit);

		modelMatrix = bodyMatrix * glm::translate(glm::mat4(1), glm::vec3((bodyDimensions.x - headDimensions.x) / 2 + eyeDistance, bodyDimensions.y + headDimensions.y / 2, headDimensions.z - eyeDimensions.z + 0.0001f));
		RenderSimpleMesh(meshes["eyeEnemy"], shaders["LabShader"], modelMatrix, glm::vec3(0), enemy.hit);

		modelMatrix = bodyMatrix * glm::translate(glm::mat4(1), glm::vec3(bodyDimensions.x / 2 + headDimensions.x / 2 - eyeDistance - eyeDimensions.x, bodyDimensions.y + headDimensions.y / 2, headDimensions.z - eyeDimensions.z + 0.0001f));
		RenderSimpleMesh(meshes["eyeEnemy"], shaders["LabShader"], modelMatrix, glm::vec3(0), enemy.hit);

		modelMatrix = bodyMatrix * glm::translate(glm::mat4(1), glm::vec3((bodyDimensions.x - headDimensions.x) / 2 + eyeDistance + pupilDimensions.x, bodyDimensions.y + headDimensions.y / 2, headDimensions.z - eyeDimensions.z + 0.0001f));
		RenderSimpleMesh(meshes["pupilEnemy"], shaders["LabShader"], modelMatrix, eyeColorEnemy, enemy.hit);

		modelMatrix = bodyMatrix * glm::translate(glm::mat4(1), glm::vec3(bodyDimensions.x / 2 + headDimensions.x / 2 - eyeDistance - eyeDimensions.x - pupilDimensions.x, bodyDimensions.y + headDimensions.y / 2, headDimensions.z - eyeDimensions.z + 0.0001f));
		RenderSimpleMesh(meshes["pupilEnemy"], shaders["LabShader"], modelMatrix, eyeColorEnemy, enemy.hit);
	}
}


void Tema2::UpdateEnemies(float deltaTimeSeconds) {
	{
		//enemy update position
		for (enemy& enemy : enemies) {
			int speed = 1.2f;

			if (enemy.hit == 3) {
				enemy.time += deltaTimeSeconds;
			}

			if (enemy.distanceX == 0 && enemy.distanceZ == 0) {
				enemy.phase = 0;
			}
			else if (enemy.distanceX == 0 && enemy.distanceZ == patrolDistance) {
				enemy.phase = 1;
			}
			else if (enemy.distanceX == patrolDistance && enemy.distanceZ == patrolDistance) {
				enemy.phase = 2;
			}
			else if (enemy.distanceX == 0 && enemy.distanceZ == patrolDistance) {
				enemy.phase = 3;
			}

			if (enemy.phase == 0) {
				if (enemy.distanceZ + deltaTimeSeconds * speed >= patrolDistance) {
					enemy.phase = 1;
				}
				else {
					enemy.zPos += deltaTimeSeconds * speed;
					enemy.cz += deltaTimeSeconds * speed;
					enemy.distanceZ += deltaTimeSeconds * speed;
				}
			}
			else if (enemy.phase == 1) {
				if (enemy.distanceX + deltaTimeSeconds * speed >= patrolDistance) {
					enemy.phase = 2;
				}
				else {
					enemy.xPos += deltaTimeSeconds * speed;
					enemy.cx += deltaTimeSeconds * speed;
					enemy.distanceX += deltaTimeSeconds * speed;
				}
			}
			else if (enemy.phase == 2) {
				if (enemy.distanceZ - deltaTimeSeconds * speed <= 0) {
					enemy.phase = 3;
				}
				else {
					enemy.zPos -= deltaTimeSeconds * speed;
					enemy.cz -= deltaTimeSeconds * speed;
					enemy.distanceZ -= deltaTimeSeconds * speed;
				}
			}
			else if (enemy.phase == 3) {
				if (enemy.distanceX - deltaTimeSeconds * speed <= 0) {
					enemy.phase = 0;
				}
				else {
					enemy.xPos -= deltaTimeSeconds * speed;
					enemy.cx -= deltaTimeSeconds * speed;
					enemy.distanceX -= deltaTimeSeconds * speed;
				}
			}
		}
	}
}

void Tema2::DrawWalls() {
	for (wall wall : innerWalls) {
		glm::mat4 modelMatrix = glm::mat4(1);
		if (wall.direction == "side") {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(wall.x, 0, wall.z - wallDimensions.z / 2));
		}
		else {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(wall.x, 0, wall.z));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(90.f), glm::vec3(0, 1, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -wallDimensions.z / 2));
		}
		RenderSimpleMesh(meshes["wall"], shaders["VertexColor"], modelMatrix);
	}

	for (wall wall : outerWalls) {
		glm::mat4 modelMatrix = glm::mat4(1);
		if (wall.direction == "side") {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(wall.x, 0, wall.z - wallDimensions.z / 2));
		}
		else {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(wall.x, 0, wall.z));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(90.f), glm::vec3(0, 1, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -wallDimensions.z / 2));
		}
		RenderSimpleMesh(meshes["wall"], shaders["VertexColor"], modelMatrix);
	}
}

void Tema2::DrawProjectiles(float deltaTimeSeconds) {
	for (Projectile& proj : projectiles) {
		proj.TranslateForward(deltaTimeSeconds * 5.5f);
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= transform3D::Translate(proj.position.x, proj.position.y - 0.05f, proj.position.z);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(projectileRadius * 2));
		RenderSimpleMesh(meshes["sphere"], shaders["VertexColor"], modelMatrix);
	}
}

void Tema2::DrawPowerups() {
	glm::mat4 modelMatrix;
	for (powerup& powerup : powerups) {
		powerup.cy = 0.25 + abs(sin(2 * Engine::GetElapsedTime())) / 10;
		modelMatrix = glm::translate(glm::mat4(1), glm::vec3(powerup.cx, powerup.cy, powerup.cz));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f));
		RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, powerup.color, 0);
	}
}

void Tema2::CheckCollisions() {
	int count = 0;
	for (Projectile& proj : projectiles) {
		bool hit = false;

		if (Engine::GetElapsedTime() - proj.spawnTime > projectileLife) {
			projectiles.erase(projectiles.begin() + count);
			continue;
		}

		for (wall outerWall : outerWalls) {
			if (ProjectileWallCollision(proj.position, projectileRadius * 2, outerWall)) {
				projectiles.erase(projectiles.begin() + count);
				hit = true;
				break;
			}
		}

		if (!hit)
			for (wall innerWall : innerWalls) {
				if (ProjectileWallCollision(proj.position, projectileRadius * 2, innerWall)) {
					projectiles.erase(projectiles.begin() + count);
					hit = true;
					break;
				}
			}

		if (!hit && ProjectileFloorCollision(proj.position, projectileRadius * 2)) {
			hit = true;
			projectiles.erase(projectiles.begin() + count);
		}

		for (enemy& enemy : enemies) {
			if (ProjectileEnemyCollision(proj.position, projectileRadius * 2, enemy)) {
				projectiles.erase(projectiles.begin() + count);
				enemy.hit++;
				hit = true;;
				break;
			}
		}

		if (!hit)
			count++;
	}


	count = 0;
	for (enemy& enemy : enemies) {
		if (enemy.time >= enemyDeathTimer) {
			enemies.erase(enemies.begin() + count);
			break;
		}

		if (PlayerEnemyCollision(glm::vec3(cameraFirst->position.x, 0, cameraFirst->position.z), enemy)) {
			enemies.erase(enemies.begin() + count);
			lives--;
			break;
		}
		count++;
	}

	count = 0;
	for (powerup& powerup : powerups) {
		if (PlayerPowerUpCollision(glm::vec3(cameraFirst->position.x, 0, cameraFirst->position.z), glm::vec3(powerup.cx, powerup.cy, powerup.cz), 0.15f)) {
			// apply bonus, remove powerUp;
			switch (powerup.type) {
			case 0:
				// HEAL
				if (lives != healthbars)
					lives++;
				break;
			case 1:
				// INCREASED FIRE RATE
				rapidfireTimer = Engine::GetElapsedTime();
				nextFire = 0;
				fireCooldown = powerupFireCooldown;
				break;
			case 2:
				// BONUS MOVEMENT SPEED
				speedTimer = Engine::GetElapsedTime();
				cameraSpeed = normalSpeed * 1.5f;
				break;
			}

			powerups.erase(powerups.begin() + count);
			break;
		}
		count++;
	}
}


void Tema2::GameWon() {
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
		"   #      #  #######  #     #      #             #  #######  #       #   # \n"
		"    #    #   #     #  #     #      #             #  #     #  # #     #   # \n"
		"     # #     #     #  #     #      #      #      #  #     #  #  #    #   # \n"
		"      #      #     #  #     #      #     # #     #  #     #  #   #   #   # \n"
		"      #      #     #  #     #       #   #   #   #   #     #  #    #  #   # \n"
		"      #      #     #  #     #        # #     # #    #     #  #     # #     \n"
		"      #      #######  #######         #       #     #######  #      ##   # \n\n");
	window->Close();
}


void Tema2::GameOver() {
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
		"   #######     #    #     # #######      ####### #     # ####### ######    # \n"
		"   #     #    # #   ##   ## #            #     # #     # #       #     #   # \n"
		"   #         #   #  # # # # #            #     # #     # #       #     #   # \n"
		"   #  ####  #     # #  #  # #####        #     # #     # #####   ######    # \n"
		"   #     #  ####### #     # #            #     #  #   #  #       #   #     # \n"
		"   #     #  #     # #     # #            #     #   # #   #       #    #      \n"
		"   #######  #     # #     # #######      #######    #    ####### #     #   # \n\n");
	window->Close();
}


void Tema2::DrawScene(float deltaTimeSeconds) {
	if (isThirdPerson) {
		cameraMain = cameraThird;
	}
	else {
		cameraMain = cameraFirst;
	}

	if (Engine::GetElapsedTime() > rapidfireTimer + bonusCountdown) {
		fireCooldown = normalFireCooldown;
	}

	if (Engine::GetElapsedTime() > speedTimer + bonusCountdown) {
		cameraSpeed = normalSpeed;
	}

	timeLeft -= deltaTimeSeconds;

	DrawPlayer();

	UpdateEnemies(deltaTimeSeconds);

	DrawEnemies();

	DrawWalls();

	DrawHealthTime();

	DrawPowerups();

	// Draw crosshair
	if (!isThirdPerson) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, cameraFirst->GetTargetPosition());
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.0001f));
		RenderSimpleMesh(meshes["sphere"], shaders["VertexColor"], modelMatrix);
	}

	crosshair = cameraFirst->GetTargetPosition();

	// Fire projectile
	if (fireProjectile && Engine::GetElapsedTime() > nextFire)
	{
		nextFire = Engine::GetElapsedTime() + fireCooldown;

		Projectile newProjectile(cameraFirst->position, crosshair, Engine::GetElapsedTime());
		projectiles.push_back(newProjectile);
	}

	fireProjectile = false;

	CheckCollisions();

	DrawProjectiles(deltaTimeSeconds);

	// Draw Floor
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		RenderSimpleMesh(meshes["floor"], shaders["VertexColor"], modelMatrix);
	}
}


void Tema2::Update(float deltaTimeSeconds)
{
	DrawScene(deltaTimeSeconds);

	if (EscapedMaze()) {
		GameWon();
	}

	if (lives == 0 || timeLeft <= 0) {
		GameOver();
	}
}


void Tema2::FrameEnd()
{
}


void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3& color, float explosion)
{
	if (!mesh || !shader || !shader->program)
		return;

	// Render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(cameraMain->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glUniform3f(glGetUniformLocation(shader->program, "object_color"), color.x, color.y, color.z);
	glUniform1f(glGetUniformLocation(shader->program, "explosion"), explosion);

	GLint timeLocation = glGetUniformLocation(shader->GetProgramID(), "Time");
	glUniform1f(timeLocation, (GLfloat)Engine::GetElapsedTime());
	mesh->Render();
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	// move the camera

	if (window->KeyHold(GLFW_KEY_W)) {
		// Translate the camera forward
		implemented::GameCamera cameraAux = *cameraFirst;
		cameraAux.MoveForward(cameraSpeed * deltaTime);
		bool move = true;
		for (wall wall : innerWalls) {
			if (PlayerWallCollision(glm::vec3(cameraAux.position.x, 0, cameraAux.position.z), wall)) {
				move = false;
				break;
			}
		}
		if (move)
			for (wall wall : outerWalls) {
				if (PlayerWallCollision(glm::vec3(cameraAux.position.x, 0, cameraAux.position.z), wall)) {
					move = false;
					break;
				}
			}
		if (move) {
			cameraFirst->MoveForward(cameraSpeed * deltaTime);
			cameraThird->MoveForward(cameraSpeed * deltaTime);
			cameraMovement->MoveForward(cameraSpeed * deltaTime);
		}
	}
	else if (window->KeyHold(GLFW_KEY_S)) {
		// Translate the camera backward
		implemented::GameCamera cameraAux = *cameraFirst;
		cameraAux.MoveForward(-cameraSpeed * deltaTime);
		bool move = true;
		for (wall wall : innerWalls) {
			if (PlayerWallCollision(glm::vec3(cameraAux.position.x, 0, cameraAux.position.z), wall)) {
				move = false;
				break;
			}
		}
		if (move)
			for (wall wall : outerWalls) {
				if (PlayerWallCollision(glm::vec3(cameraAux.position.x, 0, cameraAux.position.z), wall)) {
					move = false;
					break;
				}
			}
		if (move) {
			cameraFirst->MoveForward(-cameraSpeed * deltaTime);
			cameraThird->MoveForward(-cameraSpeed * deltaTime);
			cameraMovement->MoveForward(-cameraSpeed * deltaTime);
		}
	}

	if (window->KeyHold(GLFW_KEY_A)) {
		// Translate the camera to the left
		implemented::GameCamera cameraAux = *cameraFirst;
		cameraAux.TranslateRight(-cameraSpeed * deltaTime);
		bool move = true;
		for (wall wall : innerWalls) {
			if (PlayerWallCollision(glm::vec3(cameraAux.position.x, 0, cameraAux.position.z), wall)) {
				move = false;
				break;
			}
		}
		if (move)
			for (wall wall : outerWalls) {
				if (PlayerWallCollision(glm::vec3(cameraAux.position.x, 0, cameraAux.position.z), wall)) {
					move = false;
					break;
				}
			}
		if (move) {
			cameraFirst->TranslateRight(-cameraSpeed * deltaTime);
			cameraThird->TranslateRight(-cameraSpeed * deltaTime);
			cameraMovement->TranslateRight(-cameraSpeed * deltaTime);
		}
	}
	else if (window->KeyHold(GLFW_KEY_D)) {
		// Translate the camera to the right
		implemented::GameCamera cameraAux = *cameraFirst;
		cameraAux.TranslateRight(cameraSpeed * deltaTime);
		bool move = true;
		for (wall wall : innerWalls) {
			if (PlayerWallCollision(glm::vec3(cameraAux.position.x, 0, cameraAux.position.z), wall)) {
				move = false;
				break;
			}
		}
		if (move)
			for (wall wall : outerWalls) {
				if (PlayerWallCollision(glm::vec3(cameraAux.position.x, 0, cameraAux.position.z), wall)) {
					move = false;
					break;
				}
			}
		if (move) {
			cameraFirst->TranslateRight(cameraSpeed * deltaTime);
			cameraThird->TranslateRight(cameraSpeed * deltaTime);
			cameraMovement->TranslateRight(cameraSpeed * deltaTime);
		}
	}

	if (window->KeyHold(GLFW_KEY_Q)) {
		// Translate the camera downward
		cameraFirst->TranslateUpward(cameraSpeed * deltaTime);
		cameraThird->TranslateUpward(cameraSpeed * deltaTime);
		cameraMovement->TranslateUpward(cameraSpeed * deltaTime);
	}
	else if (window->KeyHold(GLFW_KEY_E)) {
		// Translate the camera upward
		cameraFirst->TranslateUpward(-cameraSpeed * deltaTime);
		cameraThird->TranslateUpward(-cameraSpeed * deltaTime);
		cameraMovement->TranslateUpward(-cameraSpeed * deltaTime);
	}

	// Shoot projectile (only first person)
	if (window->KeyHold(GLFW_KEY_SPACE) && !isThirdPerson) {
		fireProjectile = true;
	}

	// Change projection parameters
	if (window->KeyHold(GLFW_KEY_F))
	{
		fov += deltaTime * cameraSpeed;

		if (perspective)
		{
			projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, znear, zfar);
		}
	}
	else if (window->KeyHold(GLFW_KEY_G))
	{
		fov -= deltaTime * cameraSpeed;

		if (perspective)
		{
			projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, znear, zfar);
		}
	}
}


void Tema2::OnKeyPress(int key, int mods)
{
	// Add key press event
	if (key == GLFW_KEY_T)
	{
		isThirdPerson = !isThirdPerson;
	}
	// Switch projections
	if (key == GLFW_KEY_O)
	{
		perspective = false;
		projectionMatrix = glm::ortho(left, right, bottom, top, znear, zfar);
	}

	if (key == GLFW_KEY_P)
	{
		perspective = true;
		projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, znear, zfar);
	}

	if (key == GLFW_KEY_LEFT_CONTROL)
	{
		isThirdPerson = !isThirdPerson;
	}
}


void Tema2::OnKeyRelease(int key, int mods)
{
	// Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// Add mouse move event

	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		if (!isThirdPerson) {
			cameraMain = cameraFirst;
			// Rotate the camera in first-person mode around
			// OX and OY using `deltaX` and `deltaY`. Used the sensitivity
			// variables for setting up the rotation speed

			cameraFirst->MoveForward(-headDimensions.z / 2 - 0.2f);
			cameraFirst->RotateFirstPerson_OY(-deltaX * sensivityOX);
			cameraFirst->MoveForward(headDimensions.z / 2 + 0.2f);

			cameraThird->RotateThirdPerson_OY(-deltaX * sensivityOX);
			cameraMovement->RotateFirstPerson_OY(-deltaX * sensivityOX);

			if (rotationUpDown - deltaY * sensivityOY < RADIANS(45.f) && rotationUpDown - deltaY * sensivityOY > RADIANS(-45.f)) {
				cameraFirst->RotateFirstPerson_OX(-deltaY * sensivityOY);
				cameraThird->RotateThirdPerson_OX(-deltaY * sensivityOY);
				cameraMovement->RotateFirstPerson_OX(-deltaY * sensivityOY);
				rotationUpDown -= deltaY * sensivityOY;
			}
		}
		else {
			isThirdPerson = true;
			cameraMain = cameraThird;
			// Rotate the camera in third-person mode around
			// OX and OY using `deltaX` and `deltaY`. Used the sensitivity
			// variables for setting up the rotation speed.

			cameraFirst->MoveForward(-headDimensions.z / 2 - 0.2f);
			cameraFirst->RotateFirstPerson_OY(-deltaX * sensivityOX);
			cameraFirst->MoveForward(headDimensions.z / 2 + 0.2f);

			cameraThird->RotateThirdPerson_OY(-deltaX * sensivityOX);
			cameraMovement->RotateFirstPerson_OY(-deltaX * sensivityOX);

			if (rotationUpDown - deltaY * sensivityOY < RADIANS(45.f) && rotationUpDown - deltaY * sensivityOY > RADIANS(-45.f)) {
				cameraFirst->RotateFirstPerson_OX(-deltaY * sensivityOY);
				cameraThird->RotateThirdPerson_OX(-deltaY * sensivityOY);
				cameraMovement->RotateFirstPerson_OX(-deltaY * sensivityOY);
				rotationUpDown -= deltaY * sensivityOY;
			}
		}
		rotationAngle -= deltaX * sensivityOX;
	}

}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
