#ifndef UNICODE
#define UNICODE
#endif 

#define SECOND_TIMER 1000/60

#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include <random>
#include <vector>
#include <windows.h>
#include <omp.h>

const int GRID_SIZE = 32;
const int PLANT_SIZE = 500;
const int ANIMAL_SIZE = 500;
const int NUM_PLANT = 320;
const int NUM_ANIMAL = 240;
const int PLANT_GROWTH_FACTOR = 2;
const int ANIMAL_GROWTH_FACTOR = 20;
const int ANIMAL_EAT_RATE = 50;
const int PLANT_EAT_RATE = 5;
const int PLANT_FOOD = 10;
const int PLANT_DEATH_THRESHOLD = 300;
const double PLANT_PROCREATE_FACTOR = 1.1;
const int ANIMAL_DEATH_THRESHOLD = 150;
const double ANIMAL_PROCREATE_FACTOR = 1.5; 
const int WINDOW_WIDTH = 512;
const int WINDOW_HEIGHT = 512;

int animal_procreates = 0;
int plant_procreates = 0;

HBRUSH h_brush;

std::ofstream file;

class GridBox
{
private:
	int x, y;
	int r, g, b;
	int plant_food;
	int width, height;

	bool plant;
	bool animal;

public:
	GridBox(int x_loc, int y_loc)
	{
		r = 0;
		g = 0;
		b = 0;
		x = x_loc;
		y = y_loc;
		plant_food = 0;
		plant = false;
		animal = false;
		width = WINDOW_WIDTH / GRID_SIZE;
		height = (WINDOW_HEIGHT) / GRID_SIZE;
	}
	int get_height()
	{
		return height;
	}
	bool set_plant(bool p)
	{
		if (plant == false && p == true)
		{
			plant = true;
			return true;
		}
		else if (plant == true && p == false)
		{
			plant = p;
			return true;
		}
		else if (plant == true && p == true)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	bool set_animal(bool a)
	{
		if (animal == false && a == true)
		{
			animal = a;
			return true;
		}
		else if (animal == true && a == false)
		{
			animal = a;
			return true;
		}
		else if (animal == true && a == true)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	int get_food()
	{
		return plant_food;
	}

	void add_food(int f)
	{
		plant_food += f;
		if (plant_food < 0)
		{
			plant_food = 0;
		}
	}

	int eat_food()
	{
		int to_return = 0;
		if (plant_food < PLANT_EAT_RATE)
		{
			to_return = plant_food;
			plant_food = 0;
		}
		else
		{
			plant_food -= PLANT_EAT_RATE;
			to_return = PLANT_EAT_RATE;
		}

		if (plant_food > 255)
		{
			b = 255;
		}
		else
		{
			b = plant_food;
		}

		return to_return;
	}

	int get_x()
	{
		return x;
	}

	int get_y()
	{
		return y;
	}

	void draw(HDC hDC);

	~GridBox()
	{
		plant = 0;
		animal = 0;
	}
};

class Plant
{
private:
	int id;
	int x, y;
	int north, south, east, west;
	int size;
	bool alive;
	GridBox* boxes;

public:
	Plant(GridBox* b, int i_d)
	{
		id = i_d;
		x = rand() % GRID_SIZE;
		y = rand() % GRID_SIZE;
		size = rand() % PLANT_SIZE;
		boxes = b;
		alive = true;

		while (!boxes[y * GRID_SIZE + x].set_plant(true))
		{
			x = rand() % GRID_SIZE;
			y = rand() % GRID_SIZE;
		}

		north = (GRID_SIZE + y - 1) % GRID_SIZE;
		south = (GRID_SIZE + y + 1) % GRID_SIZE;
		east = (GRID_SIZE + x + 1) % GRID_SIZE;
		west = (GRID_SIZE + x - 1) % GRID_SIZE;
	}

	bool is_alive()
	{
		return alive;
	}

	void eat();

	int get_x()
	{
		return x;
	}

	int get_y()
	{
		return y;
	}

	int get_size()
	{
		return size;
	}

	int chomp();

	void procreate();

	void die();

	void print();
	
	void draw();

	~Plant()
	{
		boxes = 0;
	}
};

class Animal
{
private:
	int id;
	int x, y;
	int north, south, east, west;
	int size;
	bool alive;
	GridBox* boxes;

public:
	Animal(GridBox* b, int i_d)
	{
		id = i_d;
		x = rand() % GRID_SIZE;
		y = rand() % GRID_SIZE;
		size = rand() % ANIMAL_SIZE;
		boxes = b;
		alive = true;

		while (!boxes[y * GRID_SIZE + x].set_animal(true))
		{
			x = rand() % GRID_SIZE;
			y = rand() % GRID_SIZE;
		}

		north = (GRID_SIZE + y - 1) % GRID_SIZE;
		south = (GRID_SIZE + y + 1) % GRID_SIZE;
		east = (GRID_SIZE + x + 1) % GRID_SIZE;
		west = (GRID_SIZE + x - 1) % GRID_SIZE;
	}

	bool is_alive()
	{
		return alive;
	}

	void move();

	void procreate();

	void die();

	void print();

	int get_size()
	{
		return size;
	}

	void draw();

	~Animal()
	{
		boxes = 0;
	}
};

std::vector<Plant> plants;
std::vector<Animal> animals;
GridBox* boxes;

int count = 0;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	srand(time(0));



	file.open("whatever.txt");
	animals.reserve(NUM_ANIMAL * 2);
	plants.reserve(NUM_PLANT * 2);
	boxes = (GridBox*)malloc(sizeof(GridBox) * GRID_SIZE * GRID_SIZE);

	for (int i = 0; i < GRID_SIZE; i++)
	{
		for (int j = 0; j < GRID_SIZE; j++)
		{
			boxes[j * GRID_SIZE + i] = GridBox(i, j);
			boxes[j * GRID_SIZE + i].add_food(rand() % PLANT_FOOD);
		}
	}

	for (int i = 0; i < NUM_PLANT; i++)
	{
		plants.push_back(Plant(boxes, i));
	}



	for (int i = 0; i < NUM_ANIMAL; i++)
	{
		animals.push_back(Animal(boxes, i));
	}

	// Register the window class.
	const wchar_t CLASS_NAME[] = L"ProducerConsumer class";

	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));

	RegisterClass(&wc);

	// Create the window.

	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"Producers////Consumers",    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

										// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT,
		WINDOW_WIDTH,
		(boxes[0].get_height() + 2) * GRID_SIZE - 25,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (hwnd == NULL)
	{
		file.close();
		free(boxes);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	// Run the message loop.
	SetTimer(hwnd, SECOND_TIMER, SECOND_TIMER, NULL);

	MSG msg = {};
	BOOL message;
	while (message = GetMessage(&msg, NULL, 0, 0) != 0)
	{
		if (message == -1)
		{
			file << "error close.";
			file.close();
			free(boxes);
			return 0;
		}

		TranslateMessage(&msg);
		
		DispatchMessage(&msg);
	}

	file.close();
	free(boxes);

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	{
		//PlaySound(TEXT("Kick 005 Deep.wav"), NULL, SND_FILENAME);
#pragma omp parallel section
		{
			count++;
			for (std::vector<Plant>::iterator it = plants.begin(); it != plants.end(); it++)
			{
				if (!it->is_alive())
				{
					it = plants.erase(it);
				}

				if (it == plants.end())
				{
					break;
				}

				it->eat();

			}
		}
#pragma omp parallel section
		{
			for (std::vector<Animal>::iterator it = animals.begin(); it != animals.end(); it++)
			{

				if (!it->is_alive())
				{

					it = animals.erase(it);
				}

				if (it == animals.end())
				{

					break;
				}

				it->move();

			}
		}

		for (int j = 0; j < animal_procreates; j++)
		{
			animals.push_back(Animal(boxes, animals.size() + 1));
		}

		for (int j = 0; j < plant_procreates; j++)
		{
			plants.push_back(Plant(boxes, plants.size() + 1));
		}

		animal_procreates = 0;
		plant_procreates = 0;

		//InvalidateRect(hwnd, NULL, FALSE);


		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++)
		{
			boxes[i].draw(hdc);
		}


		EndPaint(hwnd, &ps);

		break;
	}
	

	case WM_TIMER:
		if (wParam == SECOND_TIMER)
		{
			InvalidateRect(hwnd, NULL, FALSE);   // invalidate whole window
		}
		break;

	return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void GridBox::draw(HDC hdc)
{

	if (plant)
	{
		g = 255;
	}
	else
	{
		g = 0;
	}

	if (animal)
	{
		r = 255;
	}
	else
	{
		r = 0;
	}

	if (plant_food > 255)
	{
		b = 255;
	}
	else
	{
		b = plant_food;
	}

	SetDCPenColor(hdc, RGB(r, g, b));
	h_brush = CreateSolidBrush(RGB(r,g,b));
	SelectObject(hdc, h_brush);
	Rectangle(hdc, x * width + 1.0, y * height + 1.0, x * width + width - 2.0, y * height + height - 2.0);
	DeleteObject(h_brush);


}

void Plant::eat()
{
	//print();
	file << "Plant # " << id << " eat --";
	int food_need = PLANT_GROWTH_FACTOR;

	int food = boxes[y * GRID_SIZE + x].eat_food();
	food += boxes[north * GRID_SIZE + x].eat_food();
	food += boxes[south * GRID_SIZE + x].eat_food();
	food += boxes[y * GRID_SIZE + east].eat_food();
	food += boxes[y * GRID_SIZE + west].eat_food();
	food += boxes[north * GRID_SIZE + east].eat_food();
	food += boxes[north * GRID_SIZE + west].eat_food();
	food += boxes[south * GRID_SIZE + east].eat_food();
	food += boxes[south * GRID_SIZE + west].eat_food();

	//std::cout << food << std::endl;

	size += food - food_need;
	if (size <= PLANT_DEATH_THRESHOLD)
	{
		die();
	}

	if (size >= PLANT_SIZE * PLANT_PROCREATE_FACTOR)
	{
		procreate();
	}



}

void Plant::procreate()
{
	if (plants.size() + plant_procreates > GRID_SIZE * (GRID_SIZE - GRID_SIZE / 16))
	{
		return;
	}
	plant_procreates++;
	size /= PLANT_PROCREATE_FACTOR;
}

int Plant::chomp()
{
	int to_return = 0;
	if (size < ANIMAL_EAT_RATE)
	{
		to_return = size;
		size = 0;
	}
	else
	{
		to_return = ANIMAL_EAT_RATE;
		size -= ANIMAL_EAT_RATE;
	}
	return to_return;
}

void Plant::die()
{
	boxes[y * GRID_SIZE + x].add_food(size);
	size = 0;
	boxes[y * GRID_SIZE + x].set_plant(false);
	alive = false;
}

void Animal::move()
{
	//print();
	file << "Animal # " << id << " moves -- ";
	int move_dir = rand() % 5;
	int new_index = 0;
	int new_x = x;
	int new_y = y;

	if (move_dir == 0)
	{
		new_index = north * GRID_SIZE + x;
		new_y = north;
	}
	else if (move_dir == 1)
	{
		new_index = south * GRID_SIZE + x;
		new_y = south;
	}
	else if (move_dir == 2)
	{
		new_index = y * GRID_SIZE + east;
		new_x = east;
	}
	else if (move_dir == 3)
	{
		new_index = y * GRID_SIZE + west;
		new_x = west;
	}
	else
	{
		new_index = y * GRID_SIZE + x;
		boxes[new_index].set_animal(false);
	}


	int count = 0;
	while (!boxes[new_index].set_animal(true))
	{
		move_dir = rand() % 5;
		new_index = 0;
		new_x = x;
		new_y = y;



		if (move_dir == 0)
		{
			new_index = north * GRID_SIZE + x;
			new_y = north;
		}
		else if (move_dir == 1)
		{
			new_index = south * GRID_SIZE + x;
			new_y = south;
		}
		else if (move_dir == 2)
		{
			new_index = y * GRID_SIZE + east;
			new_x = east;
		}
		else if (move_dir == 3)
		{
			new_index = y * GRID_SIZE + west;
			new_x = west;
		}
		else
		{
			new_index = y * GRID_SIZE + x;
			boxes[new_index].set_animal(false);
		}

		count++;
	}


	if (move_dir != 4)
	{
		boxes[y * GRID_SIZE + x].set_animal(false);
	}

	boxes[y * GRID_SIZE + x].add_food(ANIMAL_GROWTH_FACTOR);
	size -= ANIMAL_GROWTH_FACTOR;

	x = new_x;
	y = new_y;

	north = (GRID_SIZE + y - 1) % GRID_SIZE;
	south = (GRID_SIZE + y + 1) % GRID_SIZE;
	east = (GRID_SIZE + x + 1) % GRID_SIZE;
	west = (GRID_SIZE + x - 1) % GRID_SIZE;

	Plant* p = 0;

	for (std::vector<Plant>::iterator it = plants.begin(); it != plants.end(); it++)
	{
		if (x == it->get_x() && y == it->get_y())
		{
			p = &(*it);
		}
	}

	if (p != 0)
	{
		size += p->chomp();
	}


	if (size <= ANIMAL_DEATH_THRESHOLD)
	{
		die();
	}

	if (size > ANIMAL_SIZE * ANIMAL_PROCREATE_FACTOR)
	{
		procreate();
	}

}

void Animal::procreate()
{
	if (animals.size() + animal_procreates > GRID_SIZE * (GRID_SIZE - GRID_SIZE / 16))
	{
		return;
	}
	animal_procreates++;
	size /= ANIMAL_PROCREATE_FACTOR;

}

void Animal::die()
{
	boxes[y * GRID_SIZE + x].add_food(size);
	size = 0;
	boxes[y * GRID_SIZE + x].set_animal(false);

	alive = false;
}

void Plant::print()
{
	std::cout << "Plant #" << id << std::endl;
	std::cout << "Size: " << size << "   Status: " << alive << std::endl << std::endl;
}

void Animal::print()
{
	std::cout << "Animal #" << id << std::endl;
	std::cout << "Size: " << size << "   Status: " << alive << std::endl << std::endl;
}