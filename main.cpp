#include "raylib.h"
#include <stdio.h>

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    int telas = 0;
    int musica = 0;
    int Qmusicas = 5;

    InitWindow(screenWidth, screenHeight, "SpotSaco");

    InitAudioDevice();              // Initialize audio device

    Texture2D capaFundo = LoadTexture("SpotSaco2.png");
    Music music[Qmusicas] = {LoadMusicStream("Is This Love.mp3"),
                      LoadMusicStream("Meca Cereja.mp3"),
                      LoadMusicStream("Noite Fria.mp3"),
                      LoadMusicStream("Scarsito.mp3"),
                      LoadMusicStream("Eu Quero So Voce.mp3")};

    for(musica = 0; musica < Qmusicas ; musica++)
    PlayMusicStream(music[musica]);

    printf("\n[ESPACO] PAUSA A MUSICA\n");
    printf("[R] RENICIA A MUSICA\n");

    float timePlayed = 0.0f;        // Time played normalized [0.0f..1.0f]
    bool pause = false;
    bool mouseProximo = false;
    bool mouseAnterior = false;
    bool click = true;
               // Music playing paused

    SetTargetFPS(60);               // Set our game to run at 30 frames-per-second
    musica = 0;

    Rectangle BotaoProxima = { 608, 370, 162, 50 };
    Rectangle BotaoVoltar = { 30, 370, 162, 50 };


    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        SetMusicVolume(music[musica], 2);
        if (CheckCollisionPointRec(GetMousePosition(), BotaoProxima))
            {
                SetMouseCursor(4);
                if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && click == true && musica < Qmusicas -1)
                {
                    musica++;
                    SeekMusicStream(music[musica],0);
                    click = false;
                }
            }
             else if (CheckCollisionPointRec(GetMousePosition(), BotaoVoltar))
            {
                SetMouseCursor(4);
                if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && click == true && musica > 0)
                {
                    musica--;
                    SeekMusicStream(music[musica],0);
                    click = false;
                }
            }
            else
                SetMouseCursor(1);


            if(IsMouseButtonUp(MOUSE_BUTTON_LEFT))
                click = true;

    if (GetMusicTimePlayed(music[musica]) >= GetMusicTimeLength(music[musica]) - 0.01 && musica < Qmusicas -1)
            musica++;
    if (GetMusicTimePlayed(music[musica]) >= GetMusicTimeLength(music[musica]) - 0.01 && musica > Qmusicas -1)
        musica = 0;

        // Update
        //----------------------------------------------------------------------------------
        UpdateMusicStream(music[musica]);   // Update music buffer with new stream data

        // Restart music playing (stop and play)
        if (IsKeyPressed(KEY_R))
        {
            StopMusicStream(music[musica]);
            PlayMusicStream(music[musica]);
        }

        // Pause/Resume music playing
        if (IsKeyPressed(KEY_SPACE))
        {
            pause = !pause;

            if (pause) PauseMusicStream(music[musica]);
            else ResumeMusicStream(music[musica]);
        }

        // Get normalized time played for current music stream
        timePlayed = GetMusicTimePlayed(music[musica])/GetMusicTimeLength(music[musica]);

        if (timePlayed > 1.0f) timePlayed = 1.0f;   // Make sure time played is no longer than music
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            DrawTexture(capaFundo,0,0,WHITE);

            DrawRectangle(200, 390, (int)(timePlayed*400.0f), 12, BLACK);
            DrawRectangleLines(200, 390, 400, 12, BLACK);

            DrawText("Proxima", (int)BotaoProxima.x + 5, (int)BotaoProxima.y + 8, 40, BLACK);
            DrawText("Voltar", (int)BotaoVoltar.x + 15, (int)BotaoVoltar.y + 8, 40, BLACK);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    for(musica = 0; musica < Qmusicas ; musica++)
    UnloadMusicStream(music[musica]);   // Unload music stream buffers from RAM
    UnloadTexture(capaFundo);

    CloseAudioDevice();         // Close audio device (music streaming is automatically stopped)

    CloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

