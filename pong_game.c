#include <raylib.h>
/*#include <stdio.h>*/
#include <raymath.h>

typedef struct Ball {
    Vector2 position;
    Vector2 velocity;
    Color color;
    int size;
} Ball;

typedef struct Paddle {
    Vector2 position;
    Vector2 size;
    Color color;
    int point;
} Paddle;

int main(void)
{
    const int window_width = 800, window_height = 450;

    InitWindow(window_width, window_height, "Pong game");

    Ball ball = {0};
    ball.position = (Vector2) {window_width/2.0f, window_height/2.0f};
    ball.velocity = (Vector2) {300, 300};
    ball.color = RED;
    ball.size = 10;

    Paddle paddle_left = {0};
    paddle_left.position = (Vector2){50, window_height/2.0f};
    paddle_left.size = (Vector2){10, 100};
    paddle_left.color = BLACK;
    paddle_left.point = 0;

    Paddle paddle_right = {0};
    paddle_right.position = (Vector2){window_width - 50, window_height/2.0f};
    paddle_right.size = (Vector2){10, 100};
    paddle_right.color = BLACK;
    paddle_right.point = 0;

    SetTargetFPS(60);
    SetExitKey(KEY_ESCAPE);
    
    bool is_collision = false;

    while (!WindowShouldClose())
    {
        float frame_count = GetFrameTime();

        ball.position.x += ball.velocity.x * frame_count;
        ball.position.y += ball.velocity.y * frame_count;

        /*if (ball.position.x >= (window_width - ball.size) || ball.position.x <= ball.size)*/
        /*{*/
        /*    ball.velocity.x = -ball.velocity.x;*/
        /*}*/

        if (ball.position.y >= (window_height - ball.size) || ball.position.y <= ball.size)
        {
            ball.velocity.y = -ball.velocity.y;
        }

        if (IsKeyDown(KEY_S) && paddle_left.position.y <= (window_height - paddle_left.size.y))
        {
            paddle_left.position.y += 400 * frame_count;
        }

        else if (IsKeyDown(KEY_W) && paddle_left.position.y >= 0)
        {
            paddle_left.position.y -= 400 * frame_count;
        }

        if (IsKeyDown(KEY_DOWN) && paddle_right.position.y <= (window_height - paddle_right.size.y))
        {
            paddle_right.position.y += 400 * frame_count;
        }

        else if (IsKeyDown(KEY_UP) && paddle_right.position.y >= 0)
        {
            paddle_right.position.y -= 400 * frame_count;
        }

        if (CheckCollisionCircleRec(ball.position , ball.size, (Rectangle){paddle_right.position.x, 
            paddle_right.position.y, paddle_right.size.x, paddle_right.size.y}) 
        || CheckCollisionCircleRec(ball.position , ball.size, (Rectangle){paddle_left.position.x, 
            paddle_left.position.y, paddle_left.size.x, paddle_left.size.y}) )
        {
            is_collision = true;
            ball.velocity.x = -ball.velocity.x;
        }
        else if (ball.position.x <= (paddle_left.position.x - 30)) // Paddle left misses ball
        {
            ball.position.x = window_width/2.0f;
            ball.position.y = window_height/2.0f;
            paddle_left.point ++;
        } 
        else if (ball.position.x >= (paddle_right.position.x + 30)) // Paddle right misses ball
        {
            ball.position.x = window_width/2.0f;
            ball.position.y = window_height/2.0f;
            paddle_right.point ++;
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        if (is_collision)
        {
            DrawText("Collision", 0, 0, 20, RED);
            is_collision = false;
        }
        DrawText(TextFormat("Player A: %d", paddle_left.point), window_width/2.0f - 150, 0, 20, RED);
        DrawText(TextFormat("Player B: %d", paddle_right.point), window_width/2.0f, 0, 20, RED);

        DrawCircleV(ball.position, ball.size, ball.color);
        
        DrawRectangleV(paddle_left.position, paddle_left.size, paddle_left.color);
        DrawRectangleV(paddle_right.position, paddle_right.size, paddle_right.color);

        EndDrawing();
    }
    CloseWindow();

    return 0;
}

