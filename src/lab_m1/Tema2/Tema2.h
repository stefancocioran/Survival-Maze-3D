#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/GameCamera.h"
#include <set>
#include <string>

using namespace std;

namespace m1
{
	class Tema2 : public gfxc::SimpleScene
	{
	public:
		struct wall
		{
			float x, z;
			string direction;
		};

		struct powerup
		{
			int type;
			glm::vec3 color;
			float cx, cy, cz;
		};

		struct enemy {
			float xPos;
			float zPos;
			float cx;
			float cy;
			float cz;
			float distanceX;
			float distanceZ;
			float phase;
			int cell;
			int hit = 0;
			float time = 0;
		};

		class Projectile {
		public:
			Projectile()
			{
				position = glm::vec3(0);
				forward = glm::vec3(0);
			}

			Projectile(const glm::vec3& position, const glm::vec3& center, const float spawnTime)
			{
				Set(position, center, spawnTime);
			}

			~Projectile()
			{ }

			// Update projectile
			void Set(const glm::vec3& position, const glm::vec3& direction, const float spawnTime)
			{
				this->position = position;
				forward = glm::normalize(direction - position);
				this->spawnTime = spawnTime;
			}


			void MoveForward(float distance)
			{
				glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
				position += dir * distance;
			}

			void TranslateForward(float distance)
			{
				// Translate the camera using the `forward` vector
				position += forward * distance;
			}

		public:
			glm::vec3 position;
			glm::vec3 forward;
			float spawnTime;
		};

	public:
		Tema2();
		~Tema2();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;
		void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3& color = glm::vec3(0), float explosion = 0);
		void CreateMaze(int N, int M, set<pair<int, int>> mazeWalls);
		void InitializeVariables();
		void CreateMeshes();
		bool ProjectileWallCollision(glm::vec3 sphere, float radius, wall wall);
		bool ProjectileFloorCollision(glm::vec3 sphere, float radius);
		bool ProjectileEnemyCollision(glm::vec3 sphere, float radius, enemy enemy);
		bool PlayerEnemyCollision(glm::vec3 playerPos, enemy enemy);
		bool PlayerWallCollision(glm::vec3 playerPos, wall wall);
		bool PlayerPowerUpCollision(glm::vec3 playerPos, glm::vec3 sphere, float radius);
		bool EscapedMaze();
		void GameWon();
		void GameOver();
		void DrawScene(float deltaTimeSeconds);
		void DrawPlayer();
		void DrawEnemies();
		void DrawWalls();
		void DrawProjectiles(float deltaTimeSeconds);
		void DrawHealthTime();
		void DrawPowerups();
		void CheckCollisions();
		void UpdateEnemies(float deltaTimeSeconds);

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

	protected:
		implemented::GameCamera* cameraMain;
		implemented::GameCamera* cameraFirst;
		implemented::GameCamera* cameraThird;
		implemented::GameCamera* cameraMovement;

		glm::mat4 projectionMatrix;
		bool isThirdPerson;

		float top, bottom, left, right;
		float fov;
		float znear, zfar;
		bool perspective = true;

		glm::vec3 centerOffset;
		glm::vec3 crosshair;

		vector<wall> innerWalls;
		vector<wall> outerWalls;
		vector<enemy> enemies;
		vector<Projectile>projectiles;
		vector<powerup> powerups;
		vector<int> occupiedCells;
		vector<int> occupiedPowerups;

		float eyeDistance;
		float limbSpace;
		float playerScale;
		float cameraSpeed;
		float playerX, playerZ;

		int N, M;
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;
		float playerHeight;
		float rotationAngle;
		float patrolDistance;
		float playerWidth;
		bool playerUpdate = true;
		float rotationUpDown;
		float projectileLife = 1; // seconds

		float projectileRadius = 0.05 / 4;
		float translateX = 0, translateY = 0, translateZ = 0;

		float healthbars, healthbarLength, healthbarWidth;
		float lives;
		float timerSeconds = 90;
		float timeLeft;
		float enemyDeathTimer = .1f;
		bool fireProjectile = false;
		double nextFire;
		wall exitWall;

		double fireCooldown = 1.f;
		double rapidfireTimer = 0;
		double speedTimer = 0;
		double bonusCountdown = 5.f;

		double normalSpeed = 3.f;

		double powerupFireCooldown = 0.2f;
		double normalFireCooldown = 1.f;

		// Dimensions
		glm::vec3 wallDimensions;
		glm::vec3 legDimensions;
		glm::vec3 bodyDimensions;
		glm::vec3 shoulderDimensions;
		glm::vec3 armDimensions;
		glm::vec3 headDimensions;
		glm::vec3 hairDimensions;
		glm::vec3 eyeDimensions;
		glm::vec3 pupilDimensions;

		// Mesh colors
		glm::vec3 roomColor = glm::vec3(0.211f, 0.780f, 0.949f);
		glm::vec3 shirtColor = glm::vec3(0.0078f, 0.6313, 0.7137f);
		glm::vec3 eyeColor = glm::vec3(0.0078f, 0.6313, 0.7137f);
		glm::vec3 hairColor = glm::vec3(0.3725f, 0.2078f, 0.0431f);
		glm::vec3 pantsColor = glm::vec3(0.2823f, 0.2313f, 0.6509f);
		glm::vec3 skinColor = glm::vec3(0.7568f, 0.6431f, 0.498f);
		glm::vec3 wallColor = glm::vec3(0.388f, 0.356f, 0.313f);
		glm::vec3 floorColor = glm::vec3(0.368f, 0.615f, 0.203f);
		glm::vec3 skinColorEnemy = glm::vec3(0.2156f, 0.37254f, 0.16078f);
		glm::vec3 eyeColorEnemy = glm::vec3(0.9686f, 0.01176f, 0);
		glm::vec3 healthColor = glm::vec3(0.79f, 0.011f, 0.011f);
		glm::vec3 timeColor = glm::vec3(0.866f, 0.9176f, 0.9568f);
	};
}   // namespace m1
