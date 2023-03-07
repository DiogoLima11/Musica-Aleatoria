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

    InitWindow(screenWidth, screenHeight, "Baygo Music");

    InitAudioDevice();              // Initialize audio device

    Music music[2] = {LoadMusicStream("Musica1.mp3"),
                      LoadMusicStream("Musica2.mp3")};

    for(musica = 0; musica <2 ; musica++)
    PlayMusicStream(music[musica]);


    float timePlayed = 0.0f;        // Time played normalized [0.0f..1.0f]
    bool pause = false;
    bool mouseProximo = false;
    bool mouseAnterior = false;
               // Music playing paused

    SetTargetFPS(60);               // Set our game to run at 30 frames-per-second
    musica = 0;

    Rectangle BotaoProxima = { 608, 370, 162, 50 };
    Rectangle BotaoVoltar = { 30, 370, 162, 50 };


    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        if (CheckCollisionPointRec(GetMousePosition(), BotaoProxima))
            {
                SetMouseCursor(4);
                if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                mouseProximo = true;
                mouseAnterior = false;
                musica ++;
                UpdateMusicStream(music[musica]);
                SeekMusicStream(music [musica], 0.1);
                printf("%d",GetMusicTimePlayed(music[musica]));
                }
            }
            else
                SetMouseCursor(1);

        if(CheckCollisionPointRec(GetMousePosition(), BotaoVoltar))
        {
            SetMouseCursor(4);
            if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
              mouseProximo = false;
              mouseAnterior = true;
              musica --;
            }
        }
        else
            if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                mouseProximo = false;
                mouseAnterior = false;
                SetMouseCursor(1);
                }

    if (GetMusicTimePlayed(music[musica]) >= GetMusicTimeLength(music[musica]) - 0.05 && musica < 2)
            musica++;
    else
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

            ClearBackground(RAYWHITE);

            DrawText("TOCA MUSICA ALEATORIO!", 255, 150, 20, BLACK);

            DrawRectangle(200, 200, 400, 12, LIGHTGRAY);
            DrawRectangle(200, 200, (int)(timePlayed*400.0f), 12, BLACK);
            DrawRectangleLines(200, 200, 400, 12, GRAY);

            DrawText("APERTE ESPACO PARA PAUSAR A MUSICA", 215, 250, 20, BLACK);
            DrawText("APERTE R PARA RENICIAR A MUSICA", 235, 280, 20, BLACK);

            DrawRectangleRec(BotaoProxima, WHITE);
            DrawRectangleLines((int)BotaoProxima.x, (int)BotaoProxima.y, (int)BotaoProxima.width, (int)BotaoProxima.height, BLACK);
            DrawText("Proxima", (int)BotaoProxima.x + 5, (int)BotaoProxima.y + 8, 40, BLACK);

            DrawRectangleRec(BotaoVoltar, WHITE);
            DrawRectangleLines((int)BotaoVoltar.x, (int)BotaoVoltar.y, (int)BotaoVoltar.width, (int)BotaoVoltar.height, BLACK);
            DrawText("Voltar", (int)BotaoVoltar.x + 15, (int)BotaoVoltar.y + 8, 40, BLACK);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadMusicStream(music[musica]);   // Unload music stream buffers from RAM

    CloseAudioDevice();         // Close audio device (music streaming is automatically stopped)

    CloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

