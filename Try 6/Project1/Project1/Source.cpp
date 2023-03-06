#include <iostream>
#include <windows.h>
#include <gl/GL.h>
#include <gl/glut.h>
#include <cmath>
#include <vector>
#define _USE_MATH_DEFINES
using namespace std;
int w = 550, h = 550, tcheck = 0;
bool check = true;
float mx = 10.5, minx = -0.5;
bool block = 0;
float const xmax = 10, xmin = -1, ymax = 10, ymin = -1, delta = 0.5;
int const width = 600, height = 600;
float const scaleX = width / (xmax - xmin), scaleY = height / (ymax - ymin);
bool IsClosed = false;
float const eps = 0.1;
bool flag = true;
float y_max = 0;


class Point {
public:
	float x, y;

	Point() { x = 0; y = 0; };
	Point(float x_, float y_) {
		x = x_;
		y = y_;
	};
	void set(float x1, float y1) {
		x = x1;
		y = y1;
	}
	Point operator - (Point b) {
		Point c;
		c.x = x - b.x;
		c.y = y - b.y;
		return c;
	}
	bool operator < (int a) {
		int temp = sqrt(x * x + y * y);
		return temp < a;
	}
	bool operator != (Point b) {
		bool condition = (x != b.x) && (y != b.y);
		return condition;
	}
	bool operator == (Point b) {
		bool condition = (x == b.x) && (y == b.y);
		return condition;
	}
	bool operator > (Point b) {
		bool condition = (x > b.x) && (y > b.y);
		return condition;
	}
	bool operator < (Point b) {
		bool condition = (x < b.x) && (y < b.y);
		return condition;
	}
	bool operator >= (Point b) {
		bool condition = (x >= b.x) && (y >= b.y);
		return condition;
	}
	bool operator <= (Point b) {
		bool condition = (x <= b.x) && (y <= b.y);
		return condition;
	}
	float modul() {
		return sqrt(x * x + y * y);
	}

	;
};

class Vect {
public:
	float x, y;

	Vect(float x1, float y1) : x(x1), y(y1) {}

	Vect() : x(0), y(0) {}

	Vect(Point a, Point b) : x(b.x - a.x), y(b.y - a.y) {}

	void set(float x1, float y1) {
		x = x1;
		y = y1;
	}
	Vect operator - (Vect b) {
		Vect c;
		c.x = x - b.x;
		c.y = y - b.y;
		return c;
	}
	bool operator < (int a) {
		int temp = sqrt(x * x + y * y);
		return temp < a;
	}
	bool operator != (Vect b) {
		bool condition = (x != b.x) && (y != b.y);
		return condition;
	}
	bool operator == (Vect b) {
		bool condition = (x == b.x) && (y == b.y);
		return condition;
	}
	bool operator > (Vect b) {
		bool condition = (x > b.x) && (y > b.y);
		return condition;
	}
	bool operator < (Vect b) {
		bool condition = (x < b.x) && (y < b.y);
		return condition;
	}
	bool operator >= (Vect b) {
		bool condition = (x >= b.x) && (y >= b.y);
		return condition;
	}
	bool operator <= (Vect b) {
		bool condition = (x <= b.x) && (y <= b.y);
		return condition;
	}
	float modul() {
		return sqrt(x * x + y * y);
	};
};

vector <Point> p, v, vec;

void init(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(xmin, xmax, ymin, ymax);
}

void draw_border() {
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0, 0.0, 0.0);
	glLineWidth(1);
	glLineStipple(1, 0xAAA);
	glEnable(GL_LINE_STIPPLE);
	glBegin(GL_LINES);
	for (int i = ceil(xmin); i <= floor(xmax) - 1; i++) {
		glVertex2f(i, ymin + delta);
		glVertex2f(i, ymax - delta);
	}
	for (int i = ceil(ymin); i <= floor(ymax) - 1; i++) {
		glVertex2f(xmin + delta, i);
		glVertex2f(xmax - delta, i);
	}
	glEnd();
	glDisable(GL_LINE_STIPPLE);
	glColor3f(0.0, 0.0, 0.0);
	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex2f(xmin + delta, 0.0);
	glVertex2f(xmax - delta, 0.0);
	glVertex2f(0.0, ymin + delta);
	glVertex2f(0.0, ymax - delta);
	glEnd();
}


int ByPass(Point a, Point b)
{
	float flag = a.x * b.y - a.y * b.x;
	if (flag > 0) return 1;
	else if (flag < 0) return -1;
	else return 0;
}

int PSP(Point a, Point b, Point c)
{
	float flag = (a.x - b.x) * (c.y - b.y) - (a.y - b.y) * (c.x - b.x);
	if (flag < 0) return -1;
	else if (flag > 0) return 1;
	else return 0;
}

float PSPvec(Vect a, Vect b) {
	return a.x * b.y - a.y * b.x;
}

float SPvec(Vect a, Vect b) {
	return a.x * b.x + a.y * b.y;
}

float SP(Point a, Point b) {
	return a.x * b.x + a.y * b.y;
}

Point convertCoordinate(float x, float y) {
	Point result = Point(x / scaleX + xmin, ymax - y / scaleY);
	return result;
}

void polygon()
{
	glColor3f(0.0, 0.0, 0.0);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(10);
	if ((v.size() > 2) && (abs(v[v.size() - 1].x - v[v.size() - 3].x) < eps) && (abs(v[v.size() - 1].y - v[v.size() - 3].y) < eps)) {
		v.pop_back();
	}
	else {
		if ((v.size() > 0) && !IsClosed) {
			glBegin(GL_POINTS);
			glVertex2f(v[v.size() - 1].x, v[v.size() - 1].y);
			glEnd();
		}
		if ((v.size() > 1) && !IsClosed) {
			glBegin(GL_LINES);
			glVertex2f(v[v.size() - 2].x, v[v.size() - 2].y);
			glVertex2f(v[v.size() - 1].x, v[v.size() - 1].y);
			glEnd();
		}
	}

	if ((v.size() == 2) && ((v[1].x == v[0].x) && (v[1].y == v[0].y))) {
		v.pop_back();
	}

	else if ((IsClosed == false) && (v.size() > 1) && (abs(v[v.size() - 1].x - v[0].x) < eps) && (abs(v[v.size() - 1].y - v[0].y) < eps))
	{
		cout << "Вы замкнули полигон \n" << endl;
		IsClosed = true;
		tcheck = 1;
		v[v.size() - 1].x = v[0].x;
		v[v.size() - 1].y = v[0].y;

		for (int i = 2; i < v.size(); i++)
		{
			if ((abs(v[i].x - v[i - 2].x) < eps) && (abs(v[i - 2].y - v[i].y) < eps))
			{
				check = false;
			}
		}
		if (check == false) {
			cout << "Вы нарисовали не полигон, попробуйте еще раз, запустив программу заново." << endl;
			init();
			draw_border();
			v.clear();
			vec.clear();
			IsClosed = false;
			tcheck = 0;
		}
		if (check == true)
		{
			for (int i = 0; i < v.size() - 1; i++)
			{
				Point temp(v[i + 1].x - v[i].x, v[i + 1].y - v[i].y);
				vec.push_back(temp);
				cout << vec[i].x << " " << vec[i].y << endl;
			}


			int Rive = 1;
			for (int i = 0; i < v.size() - 2; i++)
			{
				for (int j = i + 3; j < v.size(); j++)
				{
					if ((PSP(v[i + 1], v[i], v[j - 1]) * PSP(v[i + 1], v[i], v[j]) <= 0) && (PSP(v[j], v[j - 1], v[i + 1]) * PSP(v[j], v[j - 1], v[i]) < 0))
					{
						Rive = 0;
						break;
					}

				}
			}

			if (Rive == 1)
			{
				int dir = 1;
				cout << "1)Полигон без самопересечений." << endl;
				int a = ByPass(vec[0], vec[vec.size() - 1]);
				for (int i = 0; i < vec.size() - 1; i++)
				{
					if (ByPass(vec[i + 1], vec[i]) != a)
					{
						dir = -1;
						break;
					}
				}
				if (dir == 1)
				{
					if (a == 1) {
						vector <Point> temp;
						for (int i = v.size() - 1; i >= 0; i--)
							temp.push_back(v[i]);
						v = temp;
						temp.clear();
					}
					check = 1;

				}
				else {
					cout << "2)Полигон вогнутый.Перерисуйте полигон" << endl;
					init();
					draw_border();
					v.clear();
					vec.clear();
					IsClosed = false;
					tcheck = 0;
				}
			}
			else
			{
				cout << "Полигон имеет самопересечение(-я). Нарисуйте полигон снова" << endl;
				init();
				draw_border();
				v.clear();
				vec.clear();
				IsClosed = false;
				tcheck = 0;
			}
		}

	}
	glFlush();


}



int Kback(Point& a, Point& b) {
	Vect ab(a, b);
	float t_a = 0, t_b = 1;
	for (int i = 0; i < v.size() - 1; i++) {
		Vect N(v[i], v[i + 1]);
		N.set(N.y, -N.x);
		if (SPvec(N, ab) < 0) t_a = max(t_a, SPvec(N, Vect(a, v[i])) / SPvec(N, ab));
		if (SPvec(N, ab) > 0) t_b = min(t_b, SPvec(N, Vect(a, v[i])) / SPvec(N, ab));
		if (t_a > t_b) {
			return 0;
		}
		if (SPvec(N, ab) == 0) {
			if (PSPvec(Vect(v[i], v[i + 1]), Vect(v[i], a)) < 0
				&& PSPvec(Vect(v[i], v[i + 1]), Vect(v[i], b)) < 0) {
				return 0;
			}
		}
	}
	float x1, y1, x2, y2;
	x1 = a.x + t_a * (b.x - a.x);
	y1 = a.y + t_a * (b.y - a.y);
	x2 = a.x + t_b * (b.x - a.x);
	y2 = a.y + t_b * (b.y - a.y);
	a.set(x1, y1);
	b.set(x2, y2);
	return 1;
}

void display() {
	glPointSize(7);
	int i = p.size() - 1;
	glEnable(GL_POINT_SMOOTH);
	if (p.size() > 0) {
		glBegin(GL_POINTS);
		glVertex2f(p[i].x, p[i].y);
		glEnd();
	}
	if (p.size() > 1 && (i % 2) != 0) {
		Point a = p[i - 1], b = p[i];
		switch (Kback(a, b)) {
		case 0: {
			glEnable(GL_LINE_STIPPLE);
			glColor3f(0.0, 0.0, 0.0);
			glLineStipple(4, 0xAAAA);
			glBegin(GL_LINES);
			glVertex2f(p[i - 1].x, p[i - 1].y);
			glVertex2f(p[i].x, p[i].y);
			glEnd();
			glDisable(GL_LINE_STIPPLE);
			break;
		}
		case 1: {
			glBegin(GL_LINES);
			glVertex2f(a.x, a.y);
			glVertex2f(b.x, b.y);
			glEnd();
			glEnable(GL_LINE_STIPPLE);
			glBegin(GL_LINES);
			glColor3f(0.0, 0.0, 0.0);
			glLineStipple(4, 0xAAAA);
			glVertex2f(p[i - 1].x, p[i - 1].y);
			glVertex2f(a.x, a.y);
			glVertex2f(p[i].x, p[i].y);
			glVertex2f(b.x, b.y);
			glEnd();
			glDisable(GL_LINE_STIPPLE);
			break;
		}
		}
	}
	glFlush();
}
void mouse(int button, int state, int x, int y) {
	Point temp;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		switch (tcheck) {
		case 0: {
			temp = convertCoordinate(x, y);
			if (v.size() > 0 && temp.x == v[v.size() - 1].x && temp.y == v[v.size() - 1].y)return;
			v.push_back(temp);
			polygon();
			break;
		}
		case 1: {
			temp = convertCoordinate(x, y);
			if (p.size() > 0 && temp.x == p[p.size() - 1].x && temp.y == p[p.size() - 1].y)return;
			p.push_back(temp);
			display();
			break;
		}
		}
	}
}

int main(int argc, char** argv) {
	setlocale(LC_ALL, "rus");
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("God, bless me! ");
	init();
	draw_border();
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMainLoop();
	return 0;
}