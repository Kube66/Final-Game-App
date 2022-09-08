#include <raylib.h> //includes libararies
#include <iostream>
#include <conio.h>
#include <chrono>
#include <thread>
#include <cstdint>
#include <ctime>
#include <cstdlib>
#include <string>
#include <fstream>


#define SCREEN_WIDTH 1080 // defining screen height
#define SCREEN_HEIGHT 720 // defining screen width
#define CHARACTER_SPEED .5f // character speed
#define GOAL_LINE1 100 // goal line location
#define GOAL_LINE2 980 // goal 2 line location
#define MOVE_TIME 2000 // enemy moves in millsec


using std::cout; // define variables
Vector2 playerpos{SCREEN_WIDTH - 1060, SCREEN_HEIGHT / 2}; // player posistion
int Difficulty = -1;
int score = -10;

struct enemy //enemy items
{
	Vector2 position;
	Vector2 direction;
	static constexpr float enemySpeed = CHARACTER_SPEED / 3.5f; //const enemy speed 
	bool boundsCheck(); // check for out of bounds
	void BoundsResolution(); //action for aout of bounds
	int indit;

}enemies[10]{}; // array of enemies;

struct button // create button
{

	Rectangle rect;
	bool isMouseOver();
};

enum struct screenType  //union enemytype
{
	startScreen, gamePlayScreen, endScreen
};

screenType screenState = screenType::startScreen; //screen type

uint64_t moveClock = 0; // keeps track of time, for each enemy movmenemt change timmmer
void updateplayer(uint64_t);
void drawGoalLine();
void spawnpoints();
bool checkcolide();
void changeEnemyDirections();
void moveEnimeies(uint64_t);
void drawenemies();
void DrawCharacter();
void startGameLoop();
void MainLoop();
bool endGameLoop();
void resetchar();




int main() //main function loop
{
	bool shouldExit = false;
	while (!shouldExit)
	{
		startGameLoop();
		MainLoop();
		shouldExit = endGameLoop();// end program
	}
	
}

void MainLoop() // main game loop
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Willson Run"); // screen paramiters
	SetWindowState(FLAG_VSYNC_HINT); //set window
	SetTargetFPS(60); //frame limit
	spawnpoints(); // enemy spawn
	srand(time(0)); //run time
	uint64_t delta = 0;

	Image backG = LoadImage("Textures/otherciderun.png");// background level 1
	Texture2D background = LoadTextureFromImage(backG);
	UnloadImage(backG);

	Image backG2 = LoadImage("Textures/othercide2.png");// background level 2-4
	Texture2D background2 = LoadTextureFromImage(backG2);
	UnloadImage(backG2);

	Image backG3 = LoadImage("Textures/maiden.png");// background level 5
	Texture2D background3 = LoadTextureFromImage(backG3);
	UnloadImage(backG3);

	Image image2 = LoadImage("Textures/BrightSoul.png"); //player level 1
	ImageResize(&image2, 45, 45);                  // resize image
	Texture2D texturePl1 = LoadTextureFromImage(image2); // Image converted to texture
	UnloadImage(image2);							// close image

	Image image3 = LoadImage("Textures/light.png"); //player level 2-4
	ImageResize(&image3, 45, 45);                  
	Texture2D texturePl2 = LoadTextureFromImage(image3);
	UnloadImage(image3);

	Image image4 = LoadImage("Textures/lost soul.png"); //player level 5
	ImageResize(&image4, 45, 45);                  
	Texture2D texturePl3 = LoadTextureFromImage(image4); 
	UnloadImage(image4);

	Image level1 = LoadImage("Textures/Scavenger.png"); //  enemy level 1
	ImageResize(&level1, 60, 60);								
	Texture2D textureEnemy1 = LoadTextureFromImage(level1);         
	UnloadImage(level1);

	Image level2 = LoadImage("Textures/othercide hunter.png"); //  enemy level 2
	ImageResize(&level2, 60, 60);
	Texture2D textureEnemy2 = LoadTextureFromImage(level2);
	UnloadImage(level2);

	Image level3 = LoadImage("Textures/blo.png"); //  enemy level 3
	ImageResize(&level3, 60, 60);
	Texture2D textureEnemy3 = LoadTextureFromImage(level3);
	UnloadImage(level3);

	Image level4 = LoadImage("Textures/rav.png");//  enemy level 4
	ImageResize(&level4, 60, 60);
	Texture2D textureEnemy4 = LoadTextureFromImage(level4);
	UnloadImage(level4);

	Image level5 = LoadImage("Textures/suff.png"); //  enemy level 5
	ImageResize(&level5, 60, 60);
	Texture2D textureEnemy5 = LoadTextureFromImage(level5);
	UnloadImage(level5);



	while (!WindowShouldClose()) //main game functuin
	{
		const auto start = std::chrono::steady_clock::now(); //set time and include all functions
		updateplayer(delta);
		if (checkcolide()) // gameover
		{
			CloseWindow();
			return;
		}
		changeEnemyDirections();
		moveEnimeies(delta);
		BeginDrawing();
		ClearBackground({ 0, 0, 0, 255 }); //background color

		if (Difficulty >= 20)
			DrawTexture(background3, 0, 0, WHITE); //changes background per dificulty change
		else if (Difficulty >= 5)
			DrawTexture(background2, 0, 0, WHITE);
		else
			DrawTexture(background, 0, 0, WHITE);
	
		for (auto enemy : enemies) // for all enemies texture draw
		{
			if (Difficulty >= 20)
				DrawTexture(textureEnemy5, enemy.position.x -30, enemy.position.y -30, WHITE);

			else if (Difficulty >= 15)
				DrawTexture(textureEnemy4, enemy.position.x -30, enemy.position.y -30, WHITE);

			else if (Difficulty >= 10)
				DrawTexture(textureEnemy3, enemy.position.x -30, enemy.position.y -30, WHITE);

			else if (Difficulty >= 5)
				DrawTexture(textureEnemy2, enemy.position.x -30, enemy.position.y -30, WHITE);

			else
				DrawTexture(textureEnemy1, enemy.position.x - 30, enemy.position.y - 30, WHITE);
		

		}
		
		if (Difficulty >= 20)
			DrawTexture(texturePl3, playerpos.x, playerpos.y, WHITE);// draw player new dificulty 5
			
		else if (Difficulty >= 5)
			DrawTexture(texturePl2, playerpos.x, playerpos.y, WHITE);// draw player new dificulty 2

		else 
			DrawTexture(texturePl1, playerpos.x, playerpos.y, WHITE);// draw player 
		DrawFPS(10, 10);
		drawGoalLine();
		EndDrawing();
		

		const auto end = std::chrono::steady_clock::now(); // end time
		delta = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); //use time start and fin to keep smooth playback
		moveClock += delta;


	}
	UnloadTexture(textureEnemy1); // when apllication terminated close texture files 
	UnloadTexture(textureEnemy2);
	UnloadTexture(textureEnemy3);
	UnloadTexture(textureEnemy4);
	UnloadTexture(textureEnemy5);
	UnloadTexture(background);
	UnloadTexture(background2);
	UnloadTexture(background3);
	UnloadTexture(texturePl1);
	UnloadTexture(texturePl2);
	UnloadTexture(texturePl3);
	
}

[[nodiscard]]
int clamp(int min, int max, int toClamp) //filteres out variables not within paramaters
{
	if (toClamp <= min) 
		return min;
	if (toClamp >= max) 
		return max;
	return toClamp;
}

void startGameLoop() // title screen
{

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Willson Run Start"); // screen paramiters
	SetWindowState(FLAG_VSYNC_HINT); //set window
	SetTargetFPS(60);

	Image backG4 = LoadImage("Textures/mother.png");// background start
	Texture2D background4 = LoadTextureFromImage(backG4);
	UnloadImage(backG4);

	while (!WindowShouldClose())
	{
	int Key = GetKeyPressed();
		BeginDrawing();
		ClearBackground(BLACK);
		DrawTexture(background4, 40, 0, WHITE);
		DrawText("Press any button to Start", 200, 100, 50, PURPLE);
		DrawText("Press ESC to Exit", 300, 400, 50, PURPLE);
		SetExitKey(KEY_NULL);
		if (Key == KeyboardKey::KEY_ESCAPE) // close program
		{
			exit(0);
			CloseWindow();
		}

		if (Key) // if true or not zero
		{
			CloseWindow();
			return;
		}
		EndDrawing();
		
	}
	UnloadTexture(background4);
}

void fileOpen() // opens score text file that shows most recent score
{
	FILE* Score;
	char G[255];
	const char* SC = TextFormat("Most Recent score :\n Score = %d \n", score);
	fopen_s(&Score,"Score.txt", "w");
	fputs(SC, Score);
	fclose(Score);
	
}

bool endGameLoop() // game over screen
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Willson Run Game Over"); // screen paramiters
	SetWindowState(FLAG_VSYNC_HINT); //set window
	SetTargetFPS(60);

	Image backG5 = LoadImage("Textures/beg.png");// background start
	Texture2D background5 = LoadTextureFromImage(backG5);
	UnloadImage(backG5);

	button B1,B2; //creates button
	B1.rect = {.x = 150, .y = 400, .width = 350, .height = 50};
	B2.rect = {.x = 600, .y = 400, .width = 400, .height = 50};
	const char* SC = TextFormat("Score = %d", score);
	Color B1color = DARKPURPLE;
	Color B2color = DARKPURPLE;
	fileOpen();
	while (!WindowShouldClose()) // buttons 
	{
		
		BeginDrawing();
		ClearBackground(BLACK);
		DrawTexture(background5, 40, -60, WHITE);
		if (B1.isMouseOver()) // close program
		{
			B1color = PURPLE;
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				
				CloseWindow();
				return true;
			}

		}
		else B1color = DARKPURPLE;

		if (B2.isMouseOver()) // continue game
		{
			B2color = PURPLE;
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				CloseWindow();
				resetchar();
				return false;
			}
		}
		else B2color = DARKPURPLE;

		DrawRectangle(600, 400, 400, 50, B2color); // visuals for buttons
		DrawRectangle(150, 400, 350, 50, B1color);
		DrawText("GAME OVER", 250, 200, 100, RED);
		DrawText(SC, 250, 300, 100, RED);
		DrawText("Press to Retry", 600, 400, 50, BLUE);
		DrawText("Press to Exit", 150, 400, 50, RED);
		EndDrawing();
	}
	UnloadTexture(background5);
}

bool button::isMouseOver() // detects if mouse is over button
{
	Vector2 Mousep = GetMousePosition();
	return( Mousep.x >= rect.x && 
			Mousep.x <= (rect.x + rect.width) &&
			Mousep.y >= rect.y &&
			Mousep.y <= (rect.y + rect.height)
			);
}

void resetchar() // when game continues reset all atributtes
{
	playerpos.x = SCREEN_WIDTH - 1060; // player pos
	playerpos.y = SCREEN_HEIGHT / 2;
	score = 0; // reset score
	Difficulty = 0; // reset diff
	spawnpoints(); // respwan enemies
	return;
}

void updateplayer(uint64_t delta)  //player controls
{
	if (IsKeyDown(KEY_A)) // move left
	{
		playerpos.x -= delta * CHARACTER_SPEED;
	}
	if (IsKeyDown(KEY_W)) // move up
	{
		playerpos.y -= delta * CHARACTER_SPEED;
	}
	if (IsKeyDown(KEY_S)) // move right
	{
		playerpos.y += delta * CHARACTER_SPEED;
	}
	if (IsKeyDown(KEY_D)) // move down
	{
		playerpos.x += delta * CHARACTER_SPEED;
	}
	playerpos.y = clamp(0, SCREEN_HEIGHT-30, playerpos.y); // keeps player on screen
	playerpos.x = clamp(0, SCREEN_WIDTH-30, playerpos.x); // keeps player on screen
}


void drawGoalLine() //goal
{
	const char* SC = TextFormat("Score = %d", score);
	static bool check = false; // checks status of goal

	if (playerpos.x < GOAL_LINE1 && check == false) // cross left line checkpoint
	{
		DrawRectangle(GOAL_LINE1, 0, 10, 1080, GREEN);
		DrawRectangle(GOAL_LINE2, 0, 10, 1080, RED);
		check = true;
		Difficulty += 1;
		score += 10;
		
	}
		else if (playerpos.x < GOAL_LINE1 && check == true) // if no checkpoint do nothing
	{
		DrawRectangle(GOAL_LINE1, 0, 10, 1080, WHITE);
		DrawRectangle(GOAL_LINE2, 0, 10, 1080, RED);
	}
	if (GOAL_LINE1 < playerpos.x < GOAL_LINE2 && check == true) //info between checkpoints
	{
		DrawRectangle(GOAL_LINE1, 0, 10, 1080, WHITE);
		DrawRectangle(GOAL_LINE2, 0, 10, 1080, RED);
	
	}
	if (GOAL_LINE1 < playerpos.x < GOAL_LINE2 && check == false) //info between checkpoints
	{
		DrawRectangle(GOAL_LINE1, 0, 10, 1080, RED);
		DrawRectangle(GOAL_LINE2, 0, 10, 1080, WHITE);

	}
	if (playerpos.x > GOAL_LINE2 && check == true) // cross right line checkpoint
	{
		DrawRectangle(GOAL_LINE1, 0, 10, 1080, RED);
		DrawRectangle(GOAL_LINE2, 0, 10, 1080, WHITE);
		check = false;
		Difficulty += 1;
		score += 10;
	}
		else if (playerpos.x > GOAL_LINE2 && check == false) // if no checkpoint do nothing
	{
		DrawRectangle(GOAL_LINE1, 0, 10, 1080, RED);
		DrawRectangle(GOAL_LINE2, 0, 10, 1080, WHITE);
	}
	
	DrawText(SC, 115, 10, 25, RED);
	
}


void changeEnemyDirections() //changes enemy movement
 {
 //make sure enough time has passed before we perform this function
	if (moveClock > MOVE_TIME)
		moveClock = 0;
	else return;
	
	for (auto& enemy : enemies) // for all enemies
	{
		auto rng = rand() % 4; // random number generator between 0-3
		
		if (rng < 2)//rand picked up or down (rng is either 0 or 1 here)
		{
			enemy.direction.y = rng ? 1 : -1;//map the y value to either -1 or 1
			enemy.direction.x = 0;//make it so the enemy wont move in the left and right direction also
		}
		else//rand picked left or right (rng is either 2 or 3 here)
		{
			enemy.direction.x = (rng == 2) ? -1 : 1;//map the x value to -1 or 1
			enemy.direction.y = 0;//make it so the enemy wont move in the up and down direction also
		}
		
	}
}


void moveEnimeies(uint64_t deltaTime) //moves enemies
{
	// scale the normalized direction vectors up turing into velocity vec.
	for (auto& enemy : enemies)
	{

		if (Difficulty >= 20) // increase movement speed on dificulty 
		{
			enemy.position.x += (enemy.direction.x * deltaTime * enemy::enemySpeed*2.3);// moves enemy in x direction
			enemy.position.y += (enemy.direction.y * deltaTime * enemy::enemySpeed*2.3);// moves enemy in y direction
		}

		else if (Difficulty >= 15) // increase movement speed on dificulty 
		{
			enemy.position.x += (enemy.direction.x * deltaTime * enemy::enemySpeed * 2.0);// moves enemy in x direction
			enemy.position.y += (enemy.direction.y * deltaTime * enemy::enemySpeed * 2.0);// moves enemy in y direction
		}

		else if (Difficulty >= 10) // increase movement speed on dificulty 
		{
			enemy.position.x += (enemy.direction.x * deltaTime * enemy::enemySpeed * 1.7);// moves enemy in x direction
			enemy.position.y += (enemy.direction.y * deltaTime * enemy::enemySpeed * 1.7);// moves enemy in y direction
		}

		else if (Difficulty >= 5) // increase movement speed on dificulty 
		{
			enemy.position.x += (enemy.direction.x * deltaTime * enemy::enemySpeed * 1.5);// moves enemy in x direction
			enemy.position.y += (enemy.direction.y * deltaTime * enemy::enemySpeed * 1.5);// moves enemy in y direction
		}

		else
		{
			enemy.position.x += (enemy.direction.x * deltaTime * enemy::enemySpeed);// moves enemy in x direction
			enemy.position.y += (enemy.direction.y * deltaTime * enemy::enemySpeed);// moves enemy in y direction
		}

		if (enemy.boundsCheck()) // check for out of bounds
		{
			enemy.BoundsResolution();// bounce them back
		}
	}
}

void spawnpoints() // initializes randomiazes enemy locations
{
	int x =0;
	for (auto& enemy : enemies)// all enemies
	{
		enemy.position.x = (GOAL_LINE1 + 40) + rand() % ((GOAL_LINE2 - GOAL_LINE1)); // prevents from spwaing in safe zone 
		enemy.position.y = rand() % (SCREEN_HEIGHT -40); // limits hight for spawning
		enemy.indit = x ++ ;
	}
}

bool areCirclesColidding(Vector2 p0, Vector2 p1, float r0, float r1) //if checks if objects collide
{
	Vector2 length;
	float lengthsqrd;
	float areaSum;
	

	length.x = p1.x - p0.x; //distance from x values
	length.y = p1.y - p0.y; // distance from y values 
	lengthsqrd = (length.x * length.x) + (length.y * length.y); //x, y cordanates and their distance from each other
	areaSum = r0 * r1; // circle radius
	

	return (lengthsqrd < areaSum);// detecs if distance is overlaping
}

bool enemy::boundsCheck() // checks for position in reguards to bounds
{
	
	return (position.x <= GOAL_LINE1 || position.x >= GOAL_LINE2 || position.y>= SCREEN_HEIGHT || position.y <= 0);
	
}

void enemy::BoundsResolution() // if on edge of bounds, reverese directions
{ 
	direction.x *=-1;
	direction.y *= -1;
	if (position.x == GOAL_LINE1 || position.x == GOAL_LINE2 || position.y == SCREEN_HEIGHT || position.y == 0)
	{
		position.x = SCREEN_WIDTH / 2;
		position.y = SCREEN_HEIGHT / 2;
	}
	
}

bool checkcolide() //if collide function activates return the function has collided
{
	for (const auto enemy : enemies)
	{
		if (areCirclesColidding(enemy.position, playerpos, 45, 45))
			return true;
	}
	return false;
} 