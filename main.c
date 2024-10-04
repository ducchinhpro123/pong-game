#include <raylib.h>
#include <raymath.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

typedef struct Ball
{
    Vector2 position;
    Vector2 velocity;
    Color color;
    int size;
} Ball;

typedef struct Paddle
{
    Vector2 position;
    Vector2 size;
    Color color;
    int point;
} Paddle;

void paddle_movement(Paddle *paddle_left, Paddle *paddle_right, float frame_count)
{
    if (IsKeyDown(KEY_S) && paddle_left->position.y <= (SCREEN_HEIGHT - paddle_left->size.y))
        paddle_left->position.y += 400 * frame_count;

    else if (IsKeyDown(KEY_W) && paddle_left->position.y >= 0)
        paddle_left->position.y -= 400 * frame_count;

    if (IsKeyDown(KEY_DOWN) && paddle_right->position.y <= (SCREEN_HEIGHT - paddle_right->size.y))
        paddle_right->position.y += 400 * frame_count;

    else if (IsKeyDown(KEY_UP) && paddle_right->position.y >= 0)
        paddle_right->position.y -= 400 * frame_count;
}

void resolve_collision(Ball *ball, Paddle *paddle_left, Paddle *paddle_right, float frame_count)
{
    // Wall collision
    if (ball->position.y >= (SCREEN_HEIGHT - ball->size) || ball->position.y <= ball->size)
        ball->velocity.y = -ball->velocity.y;

    paddle_movement(paddle_left, paddle_right, frame_count);
}

bool check_ball_paddle_collision(Ball *ball, Paddle *paddle)
{
    return CheckCollisionCircleRec(
        ball->position, ball->size,
        (Rectangle){paddle->position.x, paddle->position.y, paddle->size.x, paddle->size.y});
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong game");

    Ball ball = {0};
    ball.position = (Vector2){SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
    ball.velocity = (Vector2){300, 300};
    ball.color = RED;
    ball.size = 20;

    Paddle paddle_left = {0};
    paddle_left.position = (Vector2){50, SCREEN_HEIGHT / 2.0f};
    paddle_left.size = (Vector2){10, 100};
    paddle_left.color = BLACK;
    paddle_left.point = 0;

    Paddle paddle_right = {0};
    paddle_right.position = (Vector2){SCREEN_WIDTH - 50, SCREEN_HEIGHT / 2.0f};
    paddle_right.size = (Vector2){10, 100};
    paddle_right.color = BLACK;
    paddle_right.point = 0;

    SetTargetFPS(60);
    SetExitKey(KEY_ESCAPE);

    /*bool is_collision = false;*/

    while (!WindowShouldClose())
    {
        float frame_count = GetFrameTime();

        ball.position.x += ball.velocity.x * frame_count;
        ball.position.y += ball.velocity.y * frame_count;

        resolve_collision(&ball, &paddle_left, &paddle_right, frame_count);

        if (check_ball_paddle_collision(&ball, &paddle_left) || check_ball_paddle_collision(&ball, &paddle_right))
        {
            /*is_collision = true;*/
            ball.velocity.x = -ball.velocity.x;
        }
        else if (ball.position.x <= (paddle_left.position.x - 30))  // Paddle left misses ball
        {
            ball.position = (Vector2){SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f};
            paddle_left.point++;
        }
        else if (ball.position.x >= (paddle_right.position.x + 30))  // Paddle right misses ball
        {
            ball.position = (Vector2){SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f};
            paddle_right.point++;
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText(TextFormat("Player A: %d", paddle_left.point), SCREEN_WIDTH / 2.0f - 150, 0, 20,
                 RED);
        DrawText(TextFormat("Player B: %d", paddle_right.point), SCREEN_WIDTH / 2.0f, 0, 20, RED);

        DrawCircleV(ball.position, ball.size, ball.color);

        DrawRectangleV(paddle_left.position, paddle_left.size, paddle_left.color);
        DrawRectangleV(paddle_right.position, paddle_right.size, paddle_right.color);

        EndDrawing();
    }
    CloseWindow();

    return 0;
}
