#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#define BOARD_SIZE 8
#define TILE_SIZE 42
#define TILE_TYPES 5
#define SCORE_FONT_SIZE 32

const char TILE_CHARS[TILE_TYPES] = { '$', '&', '%', '#', '@' };

char board[BOARD_SIZE][BOARD_SIZE]; // 2d-array for the board
bool matched[BOARD_SIZE][BOARD_SIZE];

Vector2 grid_origin;
Texture2D background;
Vector2 hovered_tile = { -1, -1 };
Vector2 selected_tile = { -1, -1 };

int score = 200;
Font score_font;

char random_tile() {
	return TILE_CHARS[rand() % TILE_TYPES];
}

bool find_matches() {
	bool found = false;
	for (int y=0; y<BOARD_SIZE; y++) {
		for (int x=0; x<BOARD_SIZE; x++) {
			matched[y][x] = false;
		}
	}

	for (int y = 0; y < BOARD_SIZE; y++) {
		for (int x = 0; x < BOARD_SIZE - 2; x++) { 
			char tile = board[y][x];
			if (tile == board[y][x+1] &&
			    tile == board[y][x+2]) {
				matched[y][x] = matched[y][x+1] = matched[y][x+2] = true;
				// update score
				score += 10;

				found = true;
			}
		}
	}

	for (int x = 0; x < BOARD_SIZE; x++) {
		for (int y = 0; y < BOARD_SIZE - 2; y++) { 
			char tile = board[y][x];
			if (tile == board[y+1][x] &&
			    tile == board[y+2][x]) {
				matched[y][x] = matched[y+1][x] = matched[y+2][x] = true;
				// update score
				score += 10;

				found = true;
			}
		}
	}

	return found;
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
	Vector2 mouse = { 0, 0 };

	// main game loop
	while (!WindowShouldClose()) {

		// update game logic

		mouse = GetMousePosition();

		// calculate which tile (if any) is being hovered over
		// or clicked on
		Vector2 grid_offset = {
			mouse.x - grid_origin.x,
			mouse.y - grid_origin.y
		};

		if (grid_offset.x < 0 || grid_offset.y < 0 || grid_offset.x >= BOARD_SIZE*TILE_SIZE || grid_offset.y >= BOARD_SIZE*TILE_SIZE) {
			hovered_tile.x = -1; // we will only draw the highlight if this value >= 0
			hovered_tile.y = -1;
		}
		else {
			hovered_tile.x = (int) (grid_offset.x / TILE_SIZE);
			hovered_tile.y = (int) (grid_offset.y / TILE_SIZE);

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				selected_tile.x = hovered_tile.x;
				selected_tile.y = hovered_tile.y;
			}
		}

		// find matches and update score
		find_matches();

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
					20, 
					1, 
					matched[y][x] ? GREEN : WHITE 
				);
			}
		}

		// Draw hovered-over tile
		if (hovered_tile.x >= 0) {
			DrawRectangleLinesEx(
				(Rectangle) {
					grid_origin.x + (hovered_tile.x) * TILE_SIZE,
					grid_origin.y + (hovered_tile.y) * TILE_SIZE,
					TILE_SIZE,
					TILE_SIZE
				},
				2,
				WHITE
			);
		}

		// Draw selected (clicked) tile
		if (selected_tile.x >= 0) {
			DrawRectangleLinesEx(
				(Rectangle) {
					grid_origin.x + (selected_tile.x) * TILE_SIZE,
					grid_origin.y + (selected_tile.y) * TILE_SIZE,
					TILE_SIZE,
					TILE_SIZE
				},
				2,
				YELLOW
			);
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
	UnloadFont(score_font);

	// raylib function
	CloseWindow();
}


