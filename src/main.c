#include "raylib.h"
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#define BOARD_SIZE 8
#define TILE_SIZE 42
#define TILE_TYPES 5
#define SCORE_FONT_SIZE 32

const char tile_chars[TILE_TYPES] = { '$', '&', '%', '#', '@' };

char board[BOARD_SIZE][BOARD_SIZE]; // 2d-array for the board

Vector2 grid_origin;
Texture2D background;

int score = 200;
Font score_font;

char random_tile() {
	return tile_chars[rand() % TILE_TYPES];
}

void init_board() {
	for (int y=0; y<BOARD_SIZE; y++) {
		for (int x=0; x<BOARD_SIZE; x++) {
			board[y][x] = random_tile();
		}
	}

	int grid_width = BOARD_SIZE * TILE_SIZE;
	int grid_height = BOARD_SIZE * TILE_SIZE;

	grid_origin = (Vector2) {
		(GetScreenWidth() - grid_width) / 2,
		(GetScreenHeight() - grid_height) / 2
	};
}

int main(void) {
	const int screen_width = 800;
	const int screen_height = 450;

	// raylib function: initialize 
	InitWindow(screen_width, screen_height, "Match-3 with Raylib");

	SetTargetFPS(60);
	srand(time(NULL)); // seed with current time for true random

	background = LoadTexture("assets/night-sky800x480.png");
	score_font = LoadFontEx("assets/04b03.TTF", SCORE_FONT_SIZE, NULL, 0);

	init_board();

	// main game loop
	while (!WindowShouldClose()) {

		// update game logic

		BeginDrawing();
		ClearBackground(BLACK);

		DrawTexturePro(
			background,
			(Rectangle) {
				0, 0, background.width, background.height
			},
			(Rectangle) {
				0, 0, GetScreenWidth(), GetScreenHeight()
			},
			(Vector2) { 0, 0 },
			0.0f,
			WHITE
		);

		for (int y=0; y<BOARD_SIZE; y++) {
			for (int x=0; x<BOARD_SIZE; x++) {
				Rectangle rect = {
					grid_origin.x + x * TILE_SIZE, // x coord
					grid_origin.y + y * TILE_SIZE, // y coord
					TILE_SIZE,	   // width
					TILE_SIZE      // height
				};

				DrawRectangleLinesEx(rect, 1, DARKGRAY);

				DrawTextEx(
					GetFontDefault(), // font
					TextFormat("%c", board[y][x]), // text to draw
					(Vector2) { 
						rect.x + 14, 
						rect.y + 10 
					}, // x, y coords
					20, 1, WHITE // respectively: fontSize, spacing, tint
				);
			}
		}

		DrawTextEx(
			score_font,
			TextFormat("SCORE: %d", score),
			(Vector2) {
				20, 20
			},
			SCORE_FONT_SIZE,
			1.0f,
			YELLOW
		);
		// DrawText(TextFormat("スコア: %d", score), 20, 20, 24, YELLOW);

		EndDrawing();
	}

	UnloadTexture(background);

	// raylib function
	CloseWindow();
}


