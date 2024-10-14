#include "raylib.h"
#include <map>
#include <string>

constexpr int WIDTH = 400, HEIGHT = 600,
PIXEL_SIZE = 4, STATES = 3, NEIGHBOURS = 1,
CELL_COUNT = WIDTH / PIXEL_SIZE;

Color dea [] = {
	Color{ 0x13, 0x13, 0x2d, 0xff },
	Color{ 0x28, 0x30, 0x57, 0xff },
	Color{ 0x2a, 0x0f, 0x00, 0xff },
	Color{ 0x4f, 0x36, 0x18, 0xff },
	Color{ 0x51, 0x4c, 0x62, 0xff },
	Color{ 0x85, 0x38, 0x26, 0xff } };

Color colors [] = { RED, YELLOW, BLUE, GREEN, VIOLET, MAGENTA };

int y = 0;
char row[CELL_COUNT];
std::map<std::string, char> rules;

Texture2D texture;
Image img;

void addRules(std::string str, int cnt) {
	if (cnt == 0) {
		char c = GetRandomValue(0, STATES - 1) + 48;
		rules[str] = c;
		return;
	}

	for (char d = 0; d < STATES; d++) {
		addRules(str + (char)(d + 48), cnt - 1);
	}
}

void createRules() {

	for (int x = 0; x < CELL_COUNT; x++) {
		row[x] = GetRandomValue(0, STATES - 1) + 48;
	}

	addRules("", 2 * NEIGHBOURS + 1);

	y = 0;
}

void update() {
	
	int idx;
	std::string str;
	char nrow[CELL_COUNT];

	for (int j = 0; j < CELL_COUNT; j++) {
		str = "";
		for (int i = -NEIGHBOURS; i <= NEIGHBOURS; i++) {
			idx = ((j + i) + CELL_COUNT) % CELL_COUNT;
			str += row[idx];
		}
		nrow[j] = rules[str];
	}

	memcpy(row, nrow, CELL_COUNT);
}

void drawRow() { 
	float z = 360.f / STATES;

	for (int x = 0; x < CELL_COUNT; x++) {
		//float h = z * (row[x] - 48);
		//ImageDrawRectangle(&img, x * PIXEL_SIZE, y * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE, ColorFromHSV(h, 1, 1));
		ImageDrawRectangle(&img, x * PIXEL_SIZE, y * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE, colors[row[x] - 48]);
	}
}

int main() {

	InitWindow(WIDTH, HEIGHT, "1D Cellular Automata");

	SetTargetFPS(60);

	img = GenImageColor(WIDTH, HEIGHT, BLACK);

	ClearBackground(BLACK);

	createRules();

	while (!WindowShouldClose()) {

		drawRow();

		texture = LoadTextureFromImage(img);

		BeginDrawing();

		DrawTexture(texture, 0, 0, WHITE);

		EndDrawing();

		UnloadTexture(texture);

		update();
		
		if (++y >= (HEIGHT / PIXEL_SIZE)) {
			createRules();
		}
	}

	UnloadTexture(texture);
	UnloadImage(img);

	CloseWindow();

	return 0;
}