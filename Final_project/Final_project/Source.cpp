#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

const int high = 20;
const int width = 10;

int field[high][width] = { 0 };

struct Point
{
	int x;
	int y;
} a[4], b[4], next[4],shadow[4];

int figures[7][4] =
{
	{0,2,4,6}, // I
	{2,4,5,7}, // Z
	{3,5,4,6}, // S
	{3,5,4,7}, // T
	{3,5,6,7}, // L
	{2,3,5,7}, // J
	{2,3,4,5}, // O
};

bool check()
{
	for (int i = 0; i < 4; i++)
		if (a[i].x < 0 || a[i].x >= width || a[i].y >= high) return 0;
		else if (field[a[i].y][a[i].x]) return 0;

	return 1;
};


int main()
{
	srand(time(0));

	RenderWindow window(VideoMode(640, 720), "The Game!");

	Texture t1, t2, t3, t4;
	t1.loadFromFile("./tiles_DIY.png");
	t2.loadFromFile("./cell.png");
	t3.loadFromFile("./tiles_shadow.png");
	t4.loadFromFile("./next_box.png");

	Sprite s(t1),cell(t2),box(t4);

	int dx = 0; 
	int rotate = 0; 
	int block_now = 0,block_next = 0;
	int colorNum = 1,colorNum_next = 1;
	float timer = 0, delay = 0.9;
	////////
	int score = 0;
	int remove_counter = 0;
	int Level = 0;
	int Level_counter = 1;
	bool drop = 0;			

	Clock clock;

	printf("Level %d\n", Level);

	block_now = rand() % 7;
	colorNum = 1 + block_now;

	for (int i = 0; i < 4; i++)
	{
		a[i].x = 3 + (figures[block_now][i] / 2);
		a[i].y = figures[block_now][i] % 2;
	}

	block_next = rand() % 7;
	colorNum_next = 1 + block_next;

	for (int i = 0; i < 4; i++)
	{
		next[i].x = figures[block_next][i] / 2;
		next[i].y = figures[block_next][i] % 2;
	}

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();

			if (e.type == Event::KeyPressed)
				if (e.key.code == Keyboard::K) rotate = 1;
				else if (e.key.code == Keyboard::J) rotate = 2;
				else if (e.key.code == Keyboard::A) dx = -1;
				else if (e.key.code == Keyboard::D) dx = 1;
				else if (e.key.code == Keyboard::Space) drop = true;
		}

		if (block_now == 6) rotate = 0;

		if (Keyboard::isKeyPressed(Keyboard::S)) delay = 0.05;
		else delay = 1.0 - 0.1 * Level;

		//// <- Move -> ///
		for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].x += dx; }
		dx = 0;							//
		if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];
		if (drop)delay = 0;				//

		//////Rotate//////
		if (rotate == 1)
		{
			Point p = a[1]; //center of rotation
			for (int i = 0; i < 4; i++)
			{
				int x = a[i].y - p.y;
				int y = a[i].x - p.x;
				a[i].x = p.x - x;
				a[i].y = p.y + y;
			}

			for (int i = 0; i < 4; i++)
			{
				if (a[i].x < 0) for (int j = 0; j < 4; j++) a[j].x++;
				else if (a[i].x >= width) for (int j = 0; j < 4; j++) a[j].x--;
				else if (a[i].y < 0) for (int j = 0; j < 4; j++) a[j].y++;
				else if (field[a[i].y][a[i].x]) for (int j = 0; j < 4; j++) a[j] = b[j];
			}
			rotate = 0;
		}

		if (rotate == 2)			//
		{
			Point p = a[1]; //center of rotation
			for (int i = 0; i < 4; i++)
			{
				int x = a[i].y - p.y;
				int y = a[i].x - p.x;
				a[i].x = p.x + x;
				a[i].y = p.y - y;
			}

			for (int i = 0; i < 4; i++)
			{
				if (a[i].x < 0) for (int j = 0; j < 4; j++) a[j].x++;
				else if (a[i].x >= width) for (int j = 0; j < 4; j++) a[j].x--;
				else if (a[i].y < 0) for (int j = 0; j < 4; j++) a[j].y++;
				else if (field[a[i].y][a[i].x]) for (int j = 0; j < 4; j++) a[j] = b[j];
			}
			rotate = 0;
		}

		////score////
		switch (remove_counter)
		{

		case 1:
			printf("You fisished a single !\n");
			score = score + 40 * (Level + 1);
			break;
		case 2:
			printf("You fisished a double !!\n");
			score = score + 100 * (Level + 1);
			break;
		case 3:
			printf("You fisished a triple !!!\n");
			score = score + 300 * (Level + 1);
			break;
		case 4:
			printf("You fisished a Tetris !!!!\n");
			score = score + 1200 * (Level + 1);
			break;
		}

		if (remove_counter != 0)
		{
			printf("now score: %d\n", score);
			Level_counter += remove_counter;
			remove_counter = 0;
		}

		///////Tick//////
		if (timer > delay)
		{
			for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].y += 1; }

			if (!check())
			{
				for (int i = 0; i < 4; i++) field[b[i].y][b[i].x] = colorNum;

				block_now = block_next;
				colorNum = 1 + block_now;

				for (int i = 0; i < 4; i++)
				{
					a[i].x = 3 + (figures[block_now][i] / 2);
					a[i].y = figures[block_now][i] % 2;
				}

				block_next = rand() % 7;
				colorNum_next = 1 + block_next;

				for (int i = 0; i < 4; i++)
				{
					next[i].x = figures[block_next][i] / 2;
					next[i].y = figures[block_next][i] % 2;
				}

				if (block_now == 0) for (int i = 0; i < 4; i++) a[i].y++;

				////Level////
				if (Level_counter % 30 == 0 && Level < 9)
				{
					Level++;
					printf("\nLevel %d\n\n", Level);
				}

				drop = false;			//
				delay = 1.0 - 0.1 * Level;
			}
			timer = 0;
		}

		///////check lines//////////
		int k = high - 1;
		for (int i = high - 1; i > 0; i--)
		{
			int count = 0;
			for (int j = 0; j < width; j++)
			{
				if (field[i][j]) count++;
				field[k][j] = field[i][j];
			}
			if (count < width) k--;
			else if (count == width)
				remove_counter++;
				
		}

		/////////draw//////////
		window.clear(Color::Black);


		////background////
		unsigned char CELL_SIZE = 36;
		unsigned char COLUMNS = 10;
		unsigned char ROWS = 20;
		unsigned char SCREEN_RESIZE = 4;

		//unsigned short FRAME_DURATION = 16667;

		for (unsigned char i = 0; i < COLUMNS; i++)
		{
			for (unsigned char j = 0; j < ROWS; j++)
			{
				cell.setPosition(CELL_SIZE*i, CELL_SIZE*j);
				window.draw(cell);
			}
		}

		box.setPosition(400, 30);
		window.draw(box);
		

		////blocks////
		for (int i = 0; i < high; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (field[i][j] == 0) continue;
				s.setTextureRect(IntRect(field[i][j] * 36, 0, 36, 36));
				s.setPosition(j * 36, i * 36);
				window.draw(s);
			}
		}

		for (int i = 1; i < 5; i++)
		{
			s.setTextureRect(IntRect(colorNum * 36, 0, 36, 36));
			s.setPosition(a[i].x * 36, a[i].y * 36);
			window.draw(s);
		}


		for (int i = 0; i < 4; i++)
		{
			s.setTextureRect(IntRect(colorNum_next * 36, 0, 36, 36));
			s.setPosition(next[i].x * 36, next[i].y * 36);
			s.move(400, 30);
			window.draw(s);
		}

		window.display();
	}

	//system("pause");	
	return 0;
}