#include "raylib.h"

int main(void) {
    const int W = 900, H = 600;
    InitWindow(W, H, "Pong");
    SetTargetFPS(60);

    Rectangle player = { 30, H / 2 - 50, 15, 100 };
    Rectangle enemy = { W - 45, H / 2 - 50, 15, 100 };
    Vector2 ball = { W / 2, H / 2 };
    Vector2 vel = { 6, 4 };
    float radius = 10;
    int playerScore = 0, enemyScore = 0;

    while (!WindowShouldClose()) {
        // player: W / S
        if (IsKeyDown(KEY_W)) player.y -= 7;
        if (IsKeyDown(KEY_S)) player.y += 7;
        if (player.y < 0) player.y = 0;
        if (player.y + player.height > H) player.y = H - player.height;

        // enemy AI: chase the ball, a bit slower than the ball moves
        float center = enemy.y + enemy.height / 2;
        if (center < ball.y - 10) enemy.y += 3.5f;
        else if (center > ball.y + 10) enemy.y -= 3.5f;
        if (enemy.y < 0) enemy.y = 0;
        if (enemy.y + enemy.height > H) enemy.y = H - enemy.height;

        // ball
        ball.x += vel.x;
        ball.y += vel.y;
        if (ball.y < radius || ball.y > H - radius) vel.y *= -1;

        if (CheckCollisionCircleRec(ball, radius, player) && vel.x < 0) vel.x *= -1.1f;
        if (vel.x > 14) vel.x = 14;
        if (CheckCollisionCircleRec(ball, radius, enemy) && vel.x > 0) vel.x *= -1.1f;
        if (vel.x < -14) vel.x = -14;

        // scoring: ball left the screen
        if (ball.x < 0 || ball.x > W) {
            if (ball.x < 0) enemyScore++;
            else playerScore++;
            ball = (Vector2){ W / 2, H / 2 };
            vel.x = GetRandomValue(0, 1) ? 6 : -6;
            vel.y = GetRandomValue(0, 1) ? 4 : -4;
        }

        BeginDrawing();
        ClearBackground(YELLOW);
        for (int y = 0; y < H; y += 30) DrawRectangle(W / 2 - 2, y, 4, 15, GRAY);
        DrawRectangleRec(player, DARKGREEN);
        DrawRectangleRec(enemy, DARKBROWN);
        DrawCircleV(ball, radius, BLACK);
        DrawText(TextFormat("%d", playerScore), W / 4, 20, 60, BLACK);
        DrawText(TextFormat("%d", enemyScore), 3 * W / 4, 20, 60, BLACK);
        if (playerScore == 5 || enemyScore == 5) {
            vel.x =0;
            vel.y = 0;
            const char* msg = (playerScore == 5) ? "Player Wins!" : "Computer Wins!";
            DrawText(msg, (W - MeasureText(msg, 60)) / 2, H / 2 - 60, 60, DARKGRAY);

            const char* hint = "Press SPACE to restart";
            DrawText(hint, (W - MeasureText(hint, 30)) / 2, H / 2 + 20, 30, DARKGRAY);
            if (IsKeyPressed(KEY_SPACE)) {
                playerScore = 0;
                enemyScore = 0;
                vel.x = 6;
                vel.y = 4;
            }
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}