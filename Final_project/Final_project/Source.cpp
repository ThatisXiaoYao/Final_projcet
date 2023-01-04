#include <SFML/Graphics.hpp>
#include <time.h>
#include <math.h>

using namespace sf;

const int high = 20;
const int width = 10;

int field[high][width] = { 0 };

struct Point
{
	int x;
	int y;
} a[4], b[4], next[4], shadow[4];

int figures[7][4] =
{
	{0,2,4,6}, // I
	{2,4,5,7}, // Z
	{3,5,4,6}, // S
	{3,4,5,7}, // T
	{3,6,5,7}, // L
	{2,3,5,7}, // J
	{2,3,4,5}, // O
};

bool check(Point A[])
{
	for (int i = 0; i < 4; i++)
		if (A[i].x < 0 || A[i].x >= width || A[i].y >= high) return 0;
		else if (field[A[i].y][A[i].x]) return 0;

	return 1;
};


int main()
{
	srand(time(0));

	RenderWindow window(VideoMode(720, 720), "The Game!");

	Texture t1, t2, t3, t4, t5, t6, t7;
	t1.loadFromFile("./tiles_DIY.png");
	t2.loadFromFile("./cell.png");
	t3.loadFromFile("./tiles_shadow.png");
	t4.loadFromFile("./next_box.png");
	t5.loadFromFile("./Score_bord.png");
	t6.loadFromFile("./Number.png");
	t7.loadFromFile("./Flag.png");

	Sprite s(t1),cell(t2),s_(t3),box(t4), Score_bord(t5),Number(t6),Flag(t7);

	int dx = 0; 
	int rotate = 0; 
	int block_now = 0,block_next = 0;
	int colorNum = 1,colorNum_next = 1;
	float timer = 0, delay = 0.9;
	////////
	int Hold = 0;
	int score = 0;
	int score_array[10] = {0};
	int remove_counter = 0;
	int Level = 0;
	int Level_counter = 0;
	bool drop = false;			
	bool move_ = false;
	bool Latch = false;
	bool score_chage= false;

	Clock clock;

	printf("Level %d\n", Level);

	block_now = rand() % 7;
	colorNum = 1 + block_now;

	for (int i = 0; i < 4; i++)
	{
		a[i].x = 3 + (figures[block_now][i] / 2);
		a[i].y = figures[block_now][i] % 2;
		shadow[i].x = a[i].x;
		shadow[i].y = a[i].y;
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
				if (e.key.code == Keyboard::K || e.key.code == Keyboard::W) rotate = 1;
				else if (e.key.code == Keyboard::J) rotate = 2;
				else if (e.key.code == Keyboard::A) dx = -1;
				else if (e.key.code == Keyboard::D) dx = 1;
				else if (e.key.code == Keyboard::Space) drop = true;
				else if (e.key.code == Keyboard::X) Hold = 1;
		}

		if (block_now == 6) rotate = 0;

		if (Keyboard::isKeyPressed(Keyboard::S)) delay = 0.05;
		else delay = 1.0 - 0.1 * Level;

		//// <- Move -> ///
		for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].x += dx; move_ = 1;}
		dx = 0;							//

		for (int i = 0; i < 4; i++) { shadow[i].x = a[i].x; shadow[i].y = a[i].y; }

		if (!check(a)) for (int i = 0; i < 4; i++) a[i] = b[i];
		if (drop)delay = 0;				//

		//////Rotate//////
		if (rotate == 1)
		{
			Point p = a[2]; //center of rotation
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

			if (block_now == 0)
			{
				for (int i = 0; i < 4; i++)
				{
					if (a[i].x < 0) for (int j = 0; j < 4; j++) a[j].x++;
					else if (a[i].x >= width) for (int j = 0; j < 4; j++) a[j].x--;
					else if (a[i].y < 0) for (int j = 0; j < 4; j++) a[j].y++;
					else if (field[a[i].y][a[i].x]) for (int j = 0; j < 4; j++) a[j] = b[j];
				}
			}

			for(int i = 0; i < 4; i++) { shadow[i].x = a[i].x; shadow[i].y = a[i].y; }

			move_ = true;
			rotate = 0;
		}

		if (rotate == 2)			//
		{
			Point p = a[2]; //center of rotation
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

			for (int i = 0; i < 4; i++) { shadow[i].x = a[i].x; shadow[i].y = a[i].y; }
			
			move_ = true;
			rotate = 0;
		}

		////Hold////
		if (Hold == 1 && !Latch)
		{
			for (int i = 0; i < 4; i++)
			{
				a[i].x = (figures[block_now][i] / 2);
				a[i].y = figures[block_now][i] % 2;
			}

			int hold;
			hold= block_now;
			block_now = block_next;
			block_next = hold;

			colorNum = 1 + block_now;
			colorNum_next = 1 + block_next;

			Point hold_p[4];
			for (int i = 0; i < 4; i++)
			{
				hold_p[i] = a[i];
				a[i] = next[i];
				next[i] = hold_p[i];

				a[i].x += 3;

				shadow[i] = a[i];
			}
			Latch = true;
		}

		///////Tick//////
		if (timer > delay)
		{
			for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].y += 1; }

			if (!check(a))
			{
				for (int i = 0; i < 4; i++) field[b[i].y][b[i].x] = colorNum;

				block_now = block_next;
				colorNum = 1 + block_now;

				for (int i = 0; i < 4; i++)
				{
					a[i].x = 3 + (figures[block_now][i] / 2);
					a[i].y = figures[block_now][i] % 2;

					shadow[i].x = 3 + (figures[block_now][i] / 2);
					shadow[i].y = figures[block_now][i] % 2;
				}

				block_next = rand() % 7;
				colorNum_next = 1 + block_next;

				for (int i = 0; i < 4; i++)
				{
					next[i].x = figures[block_next][i] / 2;
					next[i].y = figures[block_next][i] % 2;
				}

				Hold = 0;
				Latch = false;
				drop = false;			//
				delay = 1.0 - 0.1 * Level;
			}
			timer = 0;
		}

		////shadow/////
		if (move_)
		{
			for (int i = 0; i < 4; i++) { shadow[i].x = a[i].x; shadow[i].y = a[i].y; }

			for (int i = 0; i < 20; i++)
			{
				for (int j = 0; j < 4; j++) b[j]= shadow[j];

				for (int j = 0; j < 4; j++) shadow[j].y ++;

				if (!check(shadow))
				{
					for (int j = 0; j < 4; j++) shadow[j] = b[j];
					break;
				}
			}
			move_ = false;
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


		////Level  &  score////
		if (remove_counter != 0)
		{
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

			unsigned long long score_ = score;

			for (int i = 9; i > 0; i--)
			{
				score_array[i] = score_%10;
				score_ = score_ / 10;
			}

			score_chage = true;

			printf("now score: %d\n", score);
			
			for (int i = 0; i < remove_counter; i++)
			{
				Level_counter ++;
				if (Level_counter % 10 == 0 && Level < 9)
				{
					Level++;
					printf("\nLevel %d\n\n", Level);
				}
			}
			remove_counter = 0;
		}



		/////////draw//////////
		window.clear(Color::Black);


		////background////
		unsigned char CELL_SIZE = 36;
		unsigned char COLUMNS = 10;
		unsigned char ROWS = 20;
		unsigned char SCREEN_RESIZE = 4;

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

		Flag.setTextureRect(IntRect(Hold * 15, 0, 15, 15));
		Flag.setPosition(410, 40);
		window.draw(Flag);

		Score_bord.setPosition(375, 570);
		window.draw(Score_bord);

		Number.setTextureRect(IntRect(Level * 15, 0, 15, 25));
		Number.setPosition(480, 595);
		window.draw(Number);

		for (int i = 0; i < 10; i++)
		{
			Number.setTextureRect(IntRect(score_array[i] * 15, 0, 15, 25));
			Number.setPosition(480 + i * 15, 655);
			window.draw(Number);
		}

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

		for (int i = 0; i < 4; i++)
		{
			s_.setPosition(shadow[i].x * 36, shadow[i].y * 36);
			window.draw(s_);
		}

		for (int i = 0; i < 4; i++)
		{
			s.setTextureRect(IntRect(colorNum * 36, 0, 36, 36));
			s.setPosition(a[i].x * 36, a[i].y * 36);
			window.draw(s);
		}

		for (int i = 0; i < 4; i++)
		{
			s.setTextureRect(IntRect(colorNum_next * 36, 0, 36, 36));
			s.setPosition(next[i].x * 36, next[i].y * 36);

			switch (block_next)
			{
			case 0:
				s.move(438, 120);
				break;
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
				s.move(420, 120);
				break;
			case 6:
				s.move(438, 120);
				break;
			}

			window.draw(s);
		}

		window.display();
	}
	return 0;
}