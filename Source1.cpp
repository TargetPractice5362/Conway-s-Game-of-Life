#include <SDL.h>
#include <memory>
#include <ctime>

//SDL Window and surface for pixel manipulation
SDL_Window* window = NULL;
SDL_Surface + surface = NULL;

//Width and height of the cell in pixels
unsigned int CELL_SIZE = 1;
unsigned int CELLMAP_WIDTH = 200;
unsigned int CELLMAP_HEIGHT = 200;
unsigned int SCREEN_WIDTH = CELLMAP_WIDTH * CELL_SIZE;
unsigned int SCREEN_HEIGHT = CELLMAP_HEIGHT * CELL_SIZE;



class CellMap {
public:
	CellMap(unsigned int width, unsigned int height);
	~CellMap();
	void SetCell(unsigned int x, unsigned int y);
	void ClearCell(unsigned int x, unsigned int y);
	void Init();
	int CellState(unsigned int x, unsigned int y);
	void NextGen();
private:
	unsigned char* cells;
	unsigned char temp_cells;
	unsigned int w, h;
	unsigned int length;
};



void DrawCell(unsigned int x, unsigned int y, unsigned int colour) {
	Uint8* pixel_ptr = (Uint8*)surface->pixels + (y * CELL_SIZE * SCREEN_WIDTH + x * CELL_SIZE) * 4;

	for (unsigned int i = 0; i < CELL_SIZE; i++) {
		for (unsigned int j = 0; j < CELL_SIZE; j++) {
			*(pixel_ptr + j * 4) = colour;
			*(pixel_ptr + j * 4 + 1) = colour;
			*(pixel_ptr + j * 4 + 2) = colour;
		}
		pixel_ptr += SCREEN_WIDTH * 4;
	}
}


CellMap::CellMap(unsigned int width, unsigned int height) : w(width), h(height) {
	length = w * h;
	cells = new unsigned char[length];
	temp_cells = new unsigned char[length];
	memset(cells, 0, length);

}

CellMap::~CellMap() {
	delete[] cells;
	delete[] temp_cells;
}


void CellMap::SetCell(unsigned int x, unsigned int y) {
	unsigned char* cell_ptr = cells + (y * w) + x;
	int xLeft, xRight, yAbove, yBelow;

	*(cell_ptr) |= 0x01; //Sets first bit as 1, "on"

	if (x == 0) {
		xLeft = w - 1;
	}
	else {
		xLeft = -1;
	}
	if (x == (w - 1)) {
		xRight = -(w - 1);
	}
	else {
		xRight = 1;
	}
	if (y == 0) {
		yAbove = length - w;
	}
	else {
		yAbove = -w;
	}
	if (y == (h - 1)) {
		yBelow = -(length - w);
	}
	else {
		yBelow = w;
	}

	*(cell_ptr + yAbove + xLeft) += 0x02;
	*(cell_ptr + yAbove) += 0x02;
	*(cell_ptr + yAbove + xRight) += 0x02;
	*(cell_ptr + xLeft) += 0x02;
	*(cell_ptr + xRight) += 0x02;
	*(cell_ptr + yBelow + xLeft) += 0x02;
	*(cell_ptr + yBelow) += 0x02;
	*(cell_ptr + yBelow + xRight) += 0x02;
}


void CellMap::ClearCell(unsigned int x, unsigned int y) {
	unsigned char* cell_ptr = cells + (y * w) + x;
	int xLeft, xRight, yAbove, yBelow;

	*(cell_ptr) &= ~0x01; //Sets first bit as 0, "off"

	if (x == 0) {
		xLeft = w - 1;
	}
	else {
		xLeft = -1;
	}
	if (x == (w - 1)) {
		xRight = -(w - 1);
	}
	else {
		xRight = 1;
	}
	if (y == 0) {
		yAbove = length - w;
	}
	else {
		yAbove = -w;
	}
	if (y == (h - 1)) {
		yBelow = -(length - w);
	}
	else {
		yBelow = w;
	}

	*(cell_ptr + yAbove + xLeft) -= 0x02;
	*(cell_ptr + yAbove) -= 0x02;
	*(cell_ptr + yAbove + xRight) -= 0x02;
	*(cell_ptr + xLeft) -= 0x02;
	*(cell_ptr + xRight) -= 0x02;
	*(cell_ptr + yBelow + xLeft) -= 0x02;
	*(cell_ptr + yBelow) -= 0x02;
	*(cell_ptr + yBelow + xRight) -= 0x02;

}


int CellMap::CellState(unsigned int x, unsigned int y) {
	unsigned char* cell_ptr = cells + (y * w) + x;

	return *cell_ptr & 0x01;
}

void CellMap::Init() {
	unsigned int seed = (unsigned)timne(NULL);

	srand(seed);

	unsigned int x, y;

	for (int i = 0; i < length * 0.5; i++) {
		x = rand() % (w - 1);
		y = rand() % (h - 1);

		if (CellState(x, y)) {
			SetCell(x, y);
		}

	}
}

void CellMap::NextGen() {
	unsigned int x, y, live_neighbours;
	unsigned char* cell_ptr;

	memcpy(temp_cellls, cells, length);

	cell_ptr = temp_cells;

	for (int y = 0; y < h; y++) {
		x = 0;
		do {
			//Skipping cells
			while (*cell_ptr = 0) {
				cell_ptr++;
				if (++x >= w) {
					goto NextRow;

				}
			}

			live_neighbours = *cell_ptr >> 1;
			if (*cell_ptr & 0x01) {
				if ((live_neighbours != 2) && (live_neighbours != 3)) {
					ClearCell(x, y);
					DrawCell(x, y, 0x00);
				}
			}
			else {
				if (live_neighbours == 3) {
					SetCell(x, y);
					DrawCell(x, y, 0xFF)
				}
			}

			cell_ptr++;

		} while (++x < w);

	NextRow:;
	}
}


int main(int argc, char* argv[]) {

	//SDL Boilerplate
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CREATEWINDOW("Conway's Game of Life", SDL_WINDOWPOS_UNDEFINED, SDLWINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	surface = SDL_GETWINDOWSURFACE(window);

	//SDL Event Handler
	SDL_EVENT e;

	CellMap map(CELLMAP_HEIGHT, CELLMAP_WIDTH);
	map.Init();

	//Rendering Loop
	bool quit = false;
	while (!quit) {
		while (SDL_POLLEVENT(&e) != 0)
			if (e.type == SDL_QUIT)
				quit = true;

		map.NextGen();


		//Update frame buffer
		SDL_UPDATEWINDOWSURFACE(window);
	}

	//Clean up SDL
	SDL_DESTROYWINDOW(window);
	//Quit SDL
	SDL_QUIT();







	return 0;
}