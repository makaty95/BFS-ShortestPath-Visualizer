#include <iostream>
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include <vector>
#include <queue>
#include <chrono>
#include <random>
#include <cstdlib>
#include "UIME.h"
using namespace std;
using namespace sf;

/*notes
* /----------------BUGS--------------------//
* program crashes as vector out of bounds (random times) [fixed]
* fix clicking on any button on open , the program crashes.. [fixed]
* /----------------FEATURES--------------------//
* add text UI : to show number of cells scanned, shortest path cells count, clean cells [done]
* adding label class [done]
* Complete Button class [done]
* Mouse cursor to hand when button havoring [done]
* panel class for UI [done]
* add option to generate a random maze [done]
* modify button class to add member function changeSize() [done]
* make color boxes as button and remove onMouse function (decrease overhead) [done]
* add checkBox class [done]
* add check box for diagonal moving [done]
* complete allignment functions
*/

/////////////////////////////////Utility///////////////////////////////////////

const Color graphTypeColors[4] = { Color::Black, Color::Green, Color::Red, Color::White };
const enum class GraphType { WALL, START, END, NONE };
GraphType toDraw = GraphType::NONE;

/////////////////////////////Prototypes///////////////////////////////////////////
void showRecords(int records[4], Label& visited_cells_num, Label& tot_cells_num, Label& clean_cells_num, Label& shortest_path_num);

////////////////////////////////////////////////////////////////////////





class Cell : public UIComponent, public RectangleShape {
private:

public:


	Vector2i gridIndex;
	Vector2i parentIndex = { -1, -1 };
	bool isVisited = 0;
	GraphType type = GraphType::NONE;

	virtual bool isMouseContained(RenderWindow& window) {

		if (this->getGlobalBounds().contains({ (float)Mouse::getPosition(window).x, (float)Mouse::getPosition(window).y })) {
			return 1;
		}
		return 0;
	}

	Cell(float length, const Color color) {
		this->setColor(color);
		this->setLength(length);
	}

	void setColor(const Color& color) {
		this->setFillColor(color);
	}

	void setLength(float length) {
		this->setSize(Vector2f(length, length));
	}

	void setVisited(bool state) {
		if (state == 1) {
			this->setFillColor(Color::Yellow);
			//this->setColor(Color::Yellow);
		}
		else {
			this->setFillColor(Color::White);
		}
		isVisited = state;
	}

	void changeGraphType(GraphType _type) {
		this->type = _type;
		this->setFillColor(graphTypeColors[(int)_type]);

	}
};

class Grid {

private:
	int dx[8] = { 1,0,-1,0,1,-1,1,-1 };
	int dy[8] = { 0,1,0,-1,1,1,-1,-1 };
	int width;
	int height;
	float badding;
	vector<vector<Cell>> grid;
	int records[4] = {};
	queue<Cell>q;
	Vector2i startPoint = { -1, -1 }, endPoint = { -1,-1 };

public:

	bool BFS_ON = 0;
	bool diagonalMovement;


	// grid constructor
	Grid(int width, int height, float cellLength, float badding, Vector2f position) {
		
		diagonalMovement = 1;

		this->setDimensions(width, height);
		grid = vector<vector<Cell>>(height, vector<Cell>(width, Cell(cellLength, Color::White)));
		this->badding = badding;

		Vector2f current = position;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {

				grid[i][j].setPosition(current);

				grid[i][j].gridIndex = { i,j };
				current.x += (badding + cellLength);
			}

			// shifting the drawing position after each row
			current.x = position.x;
			current.y += (badding + cellLength);
		}

	}

	int getQSize() {
		return q.size();
	}

	void randomFill() {

		// clearing the old grid 
		restartGrid();

		// engine for random number generating
		default_random_engine engine(time(0));
		uniform_int_distribution<unsigned int> randomInt(0, 2);

		// restarting start and end points
		if (startPoint.x != -1) {
			grid[this->startPoint.x][this->startPoint.y].changeGraphType(GraphType::NONE);
			startPoint = { -1, -1 };
		}
		if (endPoint.x != -1) {
			grid[this->endPoint.x][this->endPoint.y].changeGraphType(GraphType::NONE);
			endPoint = { -1, -1 };
		}

		// filling the grid randomly 
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				int r = randomInt(engine);
				if (r == 0 or r == 1) { // empty cell
					grid[i][j].changeGraphType(GraphType::NONE);
				}
				else {
					// wall cell
					grid[i][j].changeGraphType(GraphType::WALL);
				}
			}
		}


	}

	void restartGrid() {
		records[0] = records[1] = records[2] = records[3] = 0; // record again :)
		BFS_ON = 0; // avoiding bugs

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (grid[i][j].isVisited) {
					grid[i][j].setVisited(0);
					grid[i][j].parentIndex = { -1, -1 };

					// to avoid removing start and end points
					if ((i != startPoint.x or j != startPoint.y) and (i != endPoint.x or j != endPoint.y)) {
						grid[i][j].type = GraphType::NONE;
					}
				}
			}
		}

		while (!q.empty()) {
			q.pop();
		}

		if (startPoint.x != -1) {
			grid[startPoint.x][startPoint.y].setVisited(0);
			grid[startPoint.x][startPoint.y].setColor(Color::Green);

			grid[endPoint.x][endPoint.y].setVisited(0);
			grid[endPoint.x][endPoint.y].setColor(Color::Red);

		}


	}

	void setDimensions(int width, int height) {
		this->height = height;
		this->width = width;
	}

	void drawGrid(RenderWindow& window) {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				window.draw(grid[i][j]);
			}
		}
	}

	void clear() {

		// clearing old grid
		restartGrid();

		// removing the start and end points
		startPoint = endPoint = { -1, -1 };

		// grid reset
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				grid[i][j].changeGraphType(GraphType::NONE);
			}
		}

	}



	void checkDrawingShapes(RenderWindow& window) {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (grid[i][j].isMouseContained(window)) {
					// changing draw mode for mouse to whatever mode we choosed.

					if (toDraw == GraphType::START) {
						if (startPoint.x == -1) {
							grid[i][j].changeGraphType(toDraw);
						}
						else {
							grid[startPoint.x][startPoint.y].changeGraphType(GraphType::NONE);
							grid[i][j].changeGraphType(toDraw);
						}
						startPoint = { i, j };
					}
					else if (toDraw == GraphType::END) {
						if (endPoint.x == -1) {
							grid[i][j].changeGraphType(toDraw);
						}
						else {
							grid[endPoint.x][endPoint.y].changeGraphType(GraphType::NONE);
							grid[i][j].changeGraphType(toDraw);
						}
						endPoint = { i, j };
					}
					else {
						grid[i][j].changeGraphType(toDraw);
					}
				}
			}
		}
	}

	void getParents(Cell cell) {
		if (cell.type == GraphType::START) {
			return;
		}
		records[3]++;
		int x = cell.gridIndex.x;
		int y = cell.gridIndex.y;
		grid[x][y].setColor(Color::Cyan);
		getParents(grid[cell.parentIndex.x][cell.parentIndex.y]);
	}

	int* getRecords() { // returns an array having: visited, clean, total pushed cells
		records[1] = (width * height) - records[2];
		return records;
	}



	void BFS() {


		if (q.empty()) {
			if (startPoint.x == -1) {
				cout << "no start point detected EXITING......" << endl;
				BFS_ON = 0;
				return;
			}
			int x = startPoint.x;
			int y = startPoint.y;
			q.push({ grid[x][y] });
			records[2]++; // a cell was pushed
			grid[x][y].isVisited = 1;
			BFS_ON = 0;
		}
		
		
		while (!q.empty()) {
			auto node = q.front();
			q.pop();
			int x = node.gridIndex.x;
			int y = node.gridIndex.y;
			records[0]++; // a cell was visited


			if (x != startPoint.x or y != startPoint.y) {
				// if not the start node
				grid[x][y].setColor(Color::Yellow);
			}

			int N = (diagonalMovement ? 8 : 4);
			for (int i = 0; i < N; i++) {
				int nx = x + dx[i];
				int ny = y + dy[i];
				if (nx >= 0 and ny >= 0 and nx < height and ny < width and !grid[nx][ny].isVisited and grid[nx][ny].type != GraphType::WALL) {
					BFS_ON = 1;
					if (grid[nx][ny].type == GraphType::END) {
						while (!q.empty()) {
							q.pop();
						}
						grid[nx][ny].isVisited = 1;
						grid[nx][ny].setColor(Color::Magenta);
						records[0]++; // a cell was visited
						records[2]++;
						getParents(grid[x][y]);
						BFS_ON = 0;
						return;
					}
					grid[nx][ny].parentIndex = { x,y };
					grid[nx][ny].setVisited(1);
					q.push(grid[nx][ny]);
					records[2]++; // a cell was pushed into the stack
					grid[nx][ny].setColor(Color(122, 122, 12));

				}
			}
			if (BFS_ON == 0) {
				// no path found
				records[3] = -1;
			}
			return;
		}

	}

};


Vector2i screenSize({1280, 720});
int main() {

	RenderWindow window(VideoMode(screenSize.x, screenSize.y), "BFS Visualizer (Alpha)");
	window.setFramerateLimit(90);
	std::chrono::microseconds bfs_duration(0);

	Font font;
	if (!font.loadFromFile("stuff/fonts/speedy.ttf")) {
		cout << "Error loading Font Exiting..\n";
		return 0;
	}

	// UI


	// panels
	Panel dash_board(Vector2f(500.f, 600.f), Color(16, 155, 100), Vector2f(770.f, 10.f));

	// buttons
	Button start_bfs_btn(font, Color::Black, Color::White, Vector2f(10.f, 10.f), "Start BFS");
	Button reset_bfs_btn(font, Color::Black, Color::White, Vector2f(10.f, 60.f), "Reset");
	Button fillRandomCells_btn(font, Color::Black, Color::White, Vector2f(10.f, 110.f), "Fill Random Cells");
	Button clearGrid_btn(font, Color::White, Color::Black, Vector2f(10.f, 160.f), "Clear Grid");
	Button drawWall_btn(font, Color::White, Color::Black, { 10.f, 10.f }, "Wall"); drawWall_btn.setTextSize(13); 
	Button drawStart_btn(font, Color::White, Color::Green, { 70.f, 10.f}, "Start"); drawStart_btn.setTextSize(13); 
	Button drawEnd_btn(font, Color::White, Color::Red, { 130.f, 10.f}, "End"); drawEnd_btn.setTextSize(13); 
	Button drawNone_btn(font, Color::Black, Color::White, { 190.f, 10.f }, "Erase"); drawNone_btn.setTextSize(13);

	
	// resizing buttons [resizing system needs some optimizations :) ]
	drawWall_btn.setSize({ 50.f, 50.f });
	drawStart_btn.setSize({ 50.f, 50.f });
	drawEnd_btn.setSize({ 50.f, 50.f });
	drawNone_btn.setSize({ 50.f, 50.f });

	fillRandomCells_btn.setSize({ 400, 40 });
	start_bfs_btn.setSize({ 400, 40 });
	reset_bfs_btn.setSize({ 400, 40 });
	clearGrid_btn.setSize({ 400, 40 });

	drawWall_btn.allignText(Allignement::CENTER);
	drawNone_btn.allignText(Allignement::CENTER);
	drawStart_btn.allignText(Allignement::CENTER);
	drawEnd_btn.allignText(Allignement::CENTER);

	fillRandomCells_btn.allignText(Allignement::CENTER);
	start_bfs_btn.allignText(Allignement::CENTER);
	reset_bfs_btn.allignText(Allignement::CENTER);
	clearGrid_btn.allignText(Allignement::CENTER);

	/*
	*  ===---===
	*  ===---===
	2x + text_x = btn_x
	2y + text_y = btn_y;
	
	-> x = (btn_x - text_x)/2;
	-> y = (btn_y - text_y)/2
	void centertext(){
		vector2f = getText
	}
	*/


	// colors changing
	clearGrid_btn.setOnHavorColor(Color(150, 10, 20));
	clearGrid_btn.setOnClickColor(Color::Red);

	drawEnd_btn.setOnClickColor(Color(120, 10, 10));
	drawEnd_btn.setOnHavorColor(Color(200, 10, 10));

	drawStart_btn.setOnClickColor(Color(0, 150, 0));
	drawStart_btn.setOnHavorColor(Color(10, 200, 10));

	drawWall_btn.setOnClickColor(Color(80, 80, 80));
	drawWall_btn.setOnHavorColor(Color(50, 50, 50));

	drawNone_btn.setOnClickColor(Color(100, 100, 100));




	// labels
	Label visited_cells(font, { 10.f, 430.f }, "Visited cells:");
	Label tot_cells(font, { 10.f, 470.f }, "Total pushed cells:");
	Label clean_cells(font, { 10.f, 510.f }, "Clean cells:");
	Label shortest_path(font, { 10.f, 550.f }, "Shortest path:");
	Label visited_cells_num(font, { 200.f, 430.f }, "0");
	Label tot_cells_num(font, { 250.f, 470.f }, "0");
	Label clean_cells_num(font, { 200.f, 510.f }, "0");
	Label shortest_path_num(font, { 200.f, 550.f }, "0");


	// add buttons to the panel
	dash_board.addComponent(start_bfs_btn);
	dash_board.addComponent(reset_bfs_btn);
	dash_board.addComponent(fillRandomCells_btn);
	dash_board.addComponent(clearGrid_btn);

	// add labels to the panel
	dash_board.addComponent(visited_cells);
	dash_board.addComponent(tot_cells);
	dash_board.addComponent(clean_cells);
	dash_board.addComponent(visited_cells_num);
	dash_board.addComponent(tot_cells_num);
	dash_board.addComponent(clean_cells_num);
	dash_board.addComponent(shortest_path);
	dash_board.addComponent(shortest_path_num);

	
	// checkBoxes
	CheckBox checkDiagonal("Diagonal movement", font, { 10.f, 400.f });
	checkDiagonal.setScale(1.4);
	dash_board.addComponent(checkDiagonal);
	
	//GRID
	Grid grid(50, 40, 14.f, 1.f, Vector2f(10.f, 100.f));

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}

			
		}

		// Show records
		showRecords(grid.getRecords(), visited_cells_num, tot_cells_num, clean_cells_num, shortest_path_num);

		////////////////////////////////////////////UI behaviuor detection///////////////////////////////////////////////////////////
		// Buttons behaviour
		start_bfs_btn.behaviour(window);
		reset_bfs_btn.behaviour(window);
		fillRandomCells_btn.behaviour(window);
		clearGrid_btn.behaviour(window);
		drawWall_btn.behaviour(window);
		drawStart_btn.behaviour(window);
		drawEnd_btn.behaviour(window);
		drawNone_btn.behaviour(window);

		// checkboxes behaviour
		checkDiagonal.behaviour(window);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		//////////////////////////////////////////////// Mouse clicks ////////////////////////////////////////////////////////////////////
		if (Mouse::isButtonPressed(Mouse::Left)) {
			grid.checkDrawingShapes(window);
		}

		if (drawWall_btn.isClicked(window)) {
			toDraw = GraphType::WALL;
		}
		else if (drawNone_btn.isClicked(window)) {
			toDraw = GraphType::NONE;
		}
		else if (drawStart_btn.isClicked(window)) {
			toDraw = GraphType::START;
		}
		else if (drawEnd_btn.isClicked(window)) {
			toDraw = GraphType::END;
		} 
		else if (!grid.BFS_ON and start_bfs_btn.isClicked(window)) {
			grid.restartGrid();
			grid.BFS_ON = 1;
		}
		else if (reset_bfs_btn.isClicked(window)) {
			grid.restartGrid();
		}
		else if (fillRandomCells_btn.isClicked(window)) {
			grid.randomFill();
			
		}
		else if (clearGrid_btn.isClicked(window)) {
			grid.clear();
		}
		else if (checkDiagonal.isClicked(window)) {
			grid.diagonalMovement = (checkDiagonal.isChecked() ? true : false);
			cout << "diagonal movement: " << grid.diagonalMovement << endl;
		}

		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		if (grid.BFS_ON) {
			grid.BFS();
		}

		///////////////////////////////////////////////////////Drawing objects/////////////////////////////////////////////////////////////
		window.clear(Color(30, 120, 90));


		// Drawing the grid
		grid.drawGrid(window);

		// panels
		window.draw(dash_board);

		// labels
		window.draw(tot_cells);
		window.draw(visited_cells);
		window.draw(clean_cells);
		window.draw(tot_cells_num);
		window.draw(visited_cells_num);
		window.draw(clean_cells_num);
		window.draw(shortest_path);
		window.draw(shortest_path_num);

		//Buttons
		window.draw(reset_bfs_btn);
		window.draw(start_bfs_btn);
		window.draw(fillRandomCells_btn);
		window.draw(clearGrid_btn);
		window.draw(drawWall_btn);
		window.draw(drawNone_btn);
		window.draw(drawStart_btn);
		window.draw(drawEnd_btn);

		//CheckBoxes
		window.draw(checkDiagonal);

		window.display();
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}

}

void showRecords(int records[4], Label& visited_cells_num, Label& tot_cells_num, Label& clean_cells_num, Label& shortest_path_num) {
	visited_cells_num.setValue(records[0]);
	clean_cells_num.setValue(records[1]);
	tot_cells_num.setValue(records[2]);

	if (records[3] != -1) {
		shortest_path_num.setValue(records[3]);
	}
	else {
		shortest_path_num.setString("no path found");
		
	}
	

}

