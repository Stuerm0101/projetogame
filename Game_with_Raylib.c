


#include "raylib.h"


#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Some Defines
//----------------------------------------------------------------------------------
#define NUM_SHOOTS 50
#define NUM_MAX_ENEMIES 100
#define THE_WAVE 100
#define Gold 

typedef enum { FIRST = 0, SECOND, THIRD } EnemyWave;

typedef struct Player{
    Rectangle rec;
    Vector2 speed;
    Color color;
} Player;

typedef struct Player2{
    Rectangle rec;
    Vector2 speed;
    Color color;
} Player2;


typedef struct Enemy{
    Rectangle rec;
    Vector2 speed;
    bool active;
    Color color;
} Enemy;

typedef struct Shoot{
    Rectangle rec;
    Vector2 speed;
    bool active;
    Color color;
} Shoot;

typedef struct Shoot2{
    Rectangle rec;
    Vector2 speed;
    bool active;
    Color color;
} Shoot2;

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------
static const int screenWidth = 800;
static const int screenHeight = 450;

static bool gameOver = false;
static bool gameOver1 = false;
static bool gameOver2 = false;
static bool pause =  false;
static int score = 0;
static int score2 = 0;
static bool victory = false;
static bool start = false;
static Player player = { 0 };
static Player2 player2 = { 0 };
static Enemy enemy[NUM_MAX_ENEMIES] = { 0 };
static Shoot shoot[NUM_SHOOTS] = { 0 };
static Shoot2 shoot2[NUM_SHOOTS] = { 0 };
static EnemyWave wave = { 0 };

static int shootRate = 0;
static int shootRate2 = 0;


static int activeEnemies = 0;
static int enemiesKill = 0;


//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static void InitGame(void);         // A base de todas as coisas
static void UpdateGame(void);       // Maioria das funcionalidades
static void DrawGame(void);         // Desenha as coisas
static void UnloadGame(void);       // carrega
static void UpdateDrawFrame(void);


int main(void)
{

    InitWindow(screenWidth, screenHeight, "ASTEROID RUSH");
    InitAudioDevice();
    Sound laserSound = LoadSound("Audiolaser.wav");
    Sound musicGame = LoadSound("Musicplay.mp3");
    



    InitGame();
    

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);

    // Loop principal
    

    while (!WindowShouldClose())
    { 
     
        if (!start) main_menu();
        PauseSound(musicGame);
        UpdateDrawFrame();
        // Verifique se o jogador pressionou a tecla "space"
        if (IsKeyPressed(KEY_SPACE))
        {
            // Reproduza o som do efeito sonoro
            PlaySound(laserSound);
        }
         // Verifique se o jogador pressionou a tecla "L"
        if (IsKeyPressed(KEY_L))
        {
            // Reproduza o som do efeito sonoro
            PlaySound(laserSound);
        }
        
        if (IsKeyPressed(KEY_P))
        {
            // Reproduza o som do efeito sonoro
            StopSound(laserSound);
        }
        if (gameOver)
        {
            StopSound(laserSound);
        }
       
    }
#endif

    UnloadGame();    // Carregar as coisas (texturas, sons, modelos...)
    UnloadSound(laserSound);
    UnloadSound(musicGame);
    CloseAudioDevice();
    // Feche a janela e encerre o jogo
    CloseWindow();
        
        return 0;
}

void InitGame(void)
{
    // Variaveis principais do jogo //
    gameOver1 = false;
    gameOver2 = false;
    victory = false;
    activeEnemies = THE_WAVE;
    enemiesKill = 0;
    score = 0;
    score2= 0;
    
    
    // Caracteristicas dos jogadores //
    player.rec.x =  20;
    player.rec.y = 50;
    player.rec.width = 20;
    player.rec.height = 20;
    player.speed.x = 5;
    player.speed.y = 5;
    player.color = GOLD;

    player2.rec.x =  20;
    player2.rec.y = 50;
    player2.rec.width = 20;
    player2.rec.height = 20;
    player2.speed.x = 5;
    player2.speed.y = 5;
    player2.color = RED;

    // Gerar os bixo ruim
    for (int i = 0; i < NUM_MAX_ENEMIES; i++)
    {
        enemy[i].rec.width = 10;
        enemy[i].rec.height = 10;
        enemy[i].rec.x = GetRandomValue(screenWidth, screenWidth + 1000);
        enemy[i].rec.y = GetRandomValue(0, screenHeight - enemy[i].rec.height);
        enemy[i].speed.x = 5;
        enemy[i].speed.y = 5;
        enemy[i].active = true;
        enemy[i].color = GRAY;
    }

    // Atirar
    for (int i = 0; i < NUM_SHOOTS; i++)
    {
        shoot[i].rec.x = player.rec.x;
        shoot[i].rec.y = player.rec.y + player.rec.height/4;
         
        shoot[i].rec.width = 10;
        shoot[i].rec.height = 5;
        shoot[i].speed.x = 7;
        shoot[i].speed.y = 0;
        shoot[i].active = false;
        shoot[i].color = MAROON;
    }

    for (int i = 0; i < NUM_SHOOTS; i++)
    {   
        shoot2[i].rec.x = player2.rec.x;
        shoot2[i].rec.y = player2.rec.y + player.rec.height/4;
        
        shoot2[i].rec.width = 10;
        shoot2[i].rec.height = 5;
        shoot2[i].speed.x = 7;
        shoot2[i].speed.y = 0;
        shoot2[i].active = false;
        shoot2[i].color = BLUE;
    }

}


void UpdateGame(void)
{
    
    
    
    activeEnemies = THE_WAVE;
    wave = THIRD;
    if (!gameOver)
    {
    if (IsKeyPressed('P')) pause = !pause;
    

    if (!pause)
        
    {
    if (!gameOver1){
            // Movimento do jogador 1
            if (IsKeyDown(KEY_RIGHT)) player.rec.x += player.speed.x;
            if (IsKeyDown(KEY_LEFT)) player.rec.x -= player.speed.x;
            if (IsKeyDown(KEY_UP)) player.rec.y -= player.speed.y;
            if (IsKeyDown(KEY_DOWN)) player.rec.y += player.speed.y;
            
    }
    if (!gameOver2)
    {
            //Movimento do jogador 2
            if (IsKeyDown('D')) player2.rec.x += player2.speed.x;
            if (IsKeyDown('A')) player2.rec.x -= player2.speed.x;
            if (IsKeyDown('W')) player2.rec.y -= player2.speed.y;
            if (IsKeyDown('S')) player2.rec.y += player2.speed.y;
            
    }
            // Colisao como inimigo
            for (int i = 0; i < activeEnemies; i++)
            {
                if (CheckCollisionRecs(player.rec, enemy[i].rec)) gameOver1=true;
                if (CheckCollisionRecs(player2.rec, enemy[i].rec)) gameOver2 = true;
                if (gameOver1==true && gameOver2==true) gameOver=true ;              
            }
             // Onde vai spawnar os inimigo
            for (int i = 0; i < activeEnemies; i++)
            {
                if (enemy[i].active)
                {
                    enemy[i].rec.x -= enemy[i].speed.x;

                    if (enemy[i].rec.x < 0)
                    {
                        enemy[i].rec.x = GetRandomValue(screenWidth, screenWidth + 1000);
                        enemy[i].rec.y = GetRandomValue(0, screenHeight - enemy[i].rec.height);
                    }
                }
            }

            // Nao sair da tela
            if (player.rec.x <= 0) player.rec.x = 0;
            if (player.rec.x + player.rec.width >= screenWidth) player.rec.x = screenWidth - player.rec.width;
            if (player.rec.y <= 0) player.rec.y = 0;
            if (player.rec.y + player.rec.height >= screenHeight) player.rec.y = screenHeight - player.rec.height;

            if (player2.rec.x <= 0) player2.rec.x = 0;
            if (player2.rec.x + player2.rec.width >= screenWidth) player2.rec.x = screenWidth - player2.rec.width;
            if (player2.rec.y <= 0) player2.rec.y = 0;
            if (player2.rec.y + player2.rec.height >= screenHeight) player2.rec.y = screenHeight - player2.rec.height;





            // Começar a atirar jogador 1 //
            if (IsKeyDown(KEY_L))
            {
                shootRate += 5;

                for (int i = 0; i < NUM_SHOOTS; i++)
                {
                    if (!shoot[i].active && shootRate %20 == 0)
                    {
                        shoot[i].rec.x = player.rec.x;
                        shoot[i].rec.y = player.rec.y + player.rec.height/4;
                        shoot[i].active = true;
                        break;
                    }
                }
            }
            //Começar a atirar jogador 2 //
            if (IsKeyDown(KEY_SPACE))
            {
                shootRate2 += 5;

                for (int i = 0; i < NUM_SHOOTS; i++)
                {
                    if (!shoot2[i].active && shootRate2 %20 == 0)
                    {
                        shoot2[i].rec.x = player2.rec.x;
                        shoot2[i].rec.y = player2.rec.y + player2.rec.height/4;
                        shoot2[i].active = true;
                        break;
                    }
                }
            }
           bala();
        }
    } else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            InitGame();
            gameOver = false;
            gameOver1 = false;
            gameOver2 = false;
        }
    }
}

// Draw game (one frame)
void DrawGame(void)
{
    BeginDrawing();

        ClearBackground(RAYWHITE);

        if (!gameOver)
        {
            DrawRectangleRec(player.rec, player.color);
            DrawRectangleRec(player2.rec, player2.color);


            for (int i = 0; i < activeEnemies; i++)
            {
                if (enemy[i].active) DrawRectangleRec(enemy[i].rec, enemy[i].color);
            }

            for (int i = 0; i < NUM_SHOOTS; i++)
            {
                if (shoot2[i].active) DrawRectangleRec(shoot2[i].rec, shoot2[i].color);
                if (shoot[i].active) DrawRectangleRec(shoot[i].rec, shoot[i].color);

            }

            DrawText(TextFormat("Player 1: %04i", score), 20, 20, 30, GRAY);
            DrawText(TextFormat("Player 2: %04i", score2), 350, 20, 30, BLACK);

            if (pause) DrawText("JOGO PAUSADO", screenWidth/2 - MeasureText("JOGO PAUSADO", 40)/2, screenHeight/2 - 40, 40, GRAY);
        }else {
            DrawText("GAME OVER", GetScreenWidth()/2 - MeasureText("GAME OVER", 20)/2, GetScreenHeight()/2 - 50, 20, RED);
            DrawText("PRESSIONE [ENTER] PARA RECOMEÇAR", GetScreenWidth()/2 - MeasureText("PRESSIONE [ENTER] PARA RECOMEÇAR", 20)/2, GetScreenHeight()/2 - 10, 20, BLACK);
            if (score > score2) DrawText("Vitoria do PLAYER 1",260,20,30,BLACK);
            if (score2 > score) DrawText("Vitoria do PLAYER 2",260,20,30,BLACK);}
    EndDrawing();
}

// Unload game variables
void UnloadGame(void)
{
    
    // TODO: Unload all dynamic loaded data (textures, sounds, models...)
}

void UpdateDrawFrame(void)
{
    UpdateGame();
    DrawGame();
}
void main_menu()
{
    while (!start)
    {
    BeginDrawing();
    DrawText("ASTEROID RUSH", GetScreenWidth()/2 - MeasureText("ASTEROID RUSH", 20)/2, GetScreenHeight()/2 - 50, 20, RED);
    DrawText("PRESSIONE [ENTER] PARA COMEÇAR", GetScreenWidth()/2 - MeasureText("PRESSIONE [ENTER] PARA COMEÇAR", 20)/2, GetScreenHeight()/2 - 10, 20, GOLD);
    DrawText("APERTE Q PARA JOGAR APENAS UM PLAYER",80,20,30,GRAY);
    EndDrawing();
    if (IsKeyPressed(KEY_ENTER)) start = true;
    if (IsKeyPressed(KEY_Q))
    {
    start= true;
    gameOver1=true;
    }
    
    }
}
void bala(void){
            // Tiro
            for (int i = 0; i < NUM_SHOOTS; i++)
            {
            if (shoot[i].active)
            {
            // Movimento do tiro
             shoot[i].rec.x += shoot[i].speed.x;

                 // morte dos bixo
               for (int j = 0; j < activeEnemies; j++)
                 {
                  if (enemy[j].active)
                     {
                        if (CheckCollisionRecs(shoot[i].rec, enemy[j].rec))
                          {
                              shoot[i].active = false;
                              enemy[j].rec.x = GetRandomValue(screenWidth, screenWidth + 1000);
                              enemy[j].rec.y = GetRandomValue(0, screenHeight - enemy[j].rec.height);
                              shootRate = 0;
                              enemiesKill++;
                              score += 100;
                            }

                            if (shoot[i].rec.x + shoot[i].rec.width >= screenWidth)
                            {
                                shoot[i].active = false;
                                shootRate = 0;
                            }
                        }
                    }
                }
        }
        for (int i = 0; i < NUM_SHOOTS; i++)
            {
            if (shoot2[i].active)
            {
            // Movimento do tiro
             shoot2[i].rec.x += shoot2[i].speed.x;

                 // morte dos bixo
               for (int j = 0; j < activeEnemies; j++)
                 {
                  if (enemy[j].active)
                     {
                        if (CheckCollisionRecs(shoot2[i].rec, enemy[j].rec))
                          {
                              shoot2[i].active = false;
                              enemy[j].rec.x = GetRandomValue(screenWidth, screenWidth + 1000);
                              enemy[j].rec.y = GetRandomValue(0, screenHeight - enemy[j].rec.height);
                              shootRate2 = 0;
                              enemiesKill++;
                              score2 += 100;
                            }

                            if (shoot2[i].rec.x + shoot2[i].rec.width >= screenWidth)
                            {
                                shoot2[i].active = false;
                                shootRate2 = 0;
                            }
                        }
                    }
                }
        }
        
        }