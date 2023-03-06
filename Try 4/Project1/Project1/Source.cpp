#include <windows.h>
#include "GL/gl.h"
#include "GL/glu.h"
#include <GL/glut.h>
#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

float const xmax = 10, xmin = -1, ymax = 10, ymin = -1, delta = 0.5;
int const widht = 600, height = 600;
float const scaleX = widht / (xmax - xmin), scaleY = height / (ymax - ymin);
bool isClose = false;
bool check = true;
float const eps = 0.1;
bool idea = false;
bool prov = false;

class Point {
public:float x, y;
	  Point() {
		  x = 0; y = 0;
	  }
	  Point(float x_, float y_) {
		  x = x_; y = y_;
	  }
};

float Bypass(Point a, Point b) {
	if ((a.x * b.y - b.x * a.y) > 0) {
		return 1;
	}
	else if ((a.x * b.y - b.x * a.y) == 0) {
		return 0;
	}
	else {
		return -1;
	}
}

float Cross(Point a, Point b, Point c) {
	if (((a.x - b.x) * (c.y - b.y) - (a.y - b.y) * (c.x - b.x)) > 0) {
		return 1;
	}
	else if (((a.x - b.x) * (c.y - b.y) - (a.y - b.y) * (c.x - b.x)) == 0) {
		return 0;
	}
	else {
		return -1;
	}
}

float Length(Point a, Point b)
{
	return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
}

Point convertCoordinate(float x, float y) {
	Point result = Point(x / scaleX + xmin, ymax - y / scaleY);
	return result;
}

vector<Point>v;
vector<Point>v2;
vector<Point>vcheck;

float Angle(Point a, Point b, Point c, Point d) {
	Point first(b.x - a.x, b.y - a.y);
	Point second(d.x - c.x, d.y - c.y);
	int dir;
	if (Bypass(first, second) < 0) {
		dir = -1;
	}
	else {
		dir = 1;
	}
	return dir * (acos((first.x * second.x + first.y * second.y) / (sqrt(first.x * first.x + first.y * first.y) * sqrt(second.x * second.x + second.y * second.y))) / (M_PI / 180));
}

int insidePoint(Point a)
{
	float angles = 0;
	for (int j = 1; j < v.size(); j++)
	{
		angles += Angle(a, v[j], a, v[j - 1]);
	}
	cout << angles << endl;
	if (abs(angles) >= 180) {
		return 1;
	}
	else
		return 0;
}


void init(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(xmin, xmax, ymin, ymax);
}

void draw_border(void);

void display(void)
{
	glColor3f(0.5, 0.5, 0.5);
	glEnable(GL_POINT_SMOOTH);

	glPointSize(8);
	if ((v.size() > 2) && (abs(v[v.size() - 1].x - v[v.size() - 3].x) < eps) && (abs(v[v.size() - 1].y - v[v.size() - 3].y) < eps)) {
		v.pop_back();
	}
	else {
		if ((v.size() > 0) && !isClose) {
			glBegin(GL_POINTS);
			glColor3f(0.0, 0.0, 0.0);
			glVertex2f(v[v.size() - 1].x, v[v.size() - 1].y);
			glEnd();
		}
		if ((v.size() > 1) && !isClose) {
			glBegin(GL_LINES);
			glColor3f(0.0, 0.0, 0.0);
			glVertex2f(v[v.size() - 2].x, v[v.size() - 2].y);
			glVertex2f(v[v.size() - 1].x, v[v.size() - 1].y);
			glEnd();
		}
	}
	if ((v.size() == 2) && ((v[1].x == v[0].x) && (v[1].y == v[0].y))) {
		cout << "Данное нечто не является полигоном." << endl;
		isClose = true;
	}
	else if (!isClose && (v.size() > 1) && (abs(v[v.size() - 1].x - v[0].x) < eps) && (abs(v[v.size() - 1].y - v[0].y) < eps))
	{
		cout << "Вы замкнули полигон.Дальнейшее рисование невозможно. \n" << endl;
		isClose = true;
		prov = true;
		v[v.size() - 1].x = v[0].x;
		v[v.size() - 1].y = v[0].y;
	}
	if (prov == true && idea == false) {
		int Rive = 1;
		for (int i = 0; i < v.size() - 2; i++)
		{
			for (int j = i + 3; j < v.size(); j++)
			{
				if (Cross(v[i + 1], v[i], v[j - 1]) * Cross(v[i + 1], v[i], v[j]) <= 0 && Cross(v[j], v[j - 1], v[i + 1]) * Cross(v[j], v[j - 1], v[i]) < 0)
				{
					Rive = -1;
					break;
				}
			}
		}

		if (Rive == 1)
		{
			idea = true;
			cout << "Полигон не имеет самопересечений." << endl;

		}
		else
		{
			cout << "Полигон имеет самопересечение(-я). Нарисуйте полигон снова" << endl;
			init();
			draw_border();
			v.clear();
			isClose = false;
			prov = false;
		}
	}
	if ((isClose == true) && (vcheck.size() > 0)) {
		Point prov;
		prov.x = vcheck[vcheck.size() - 1].x;
		prov.y = vcheck[vcheck.size() - 1].y;
		int key = 0;
		for (int k = 1; k < v.size(); k++) {
			if ((prov.x == v[k].x && prov.y == v[k].y) || (Length(vcheck[vcheck.size() - 1], v[k-1]) + Length(vcheck[vcheck.size() - 1], v[k]) == Length(v[k], v[k-1]))) {
				key += 1;
			}
		}
		if (key == 0) {
			if (insidePoint(prov) == 1) {
				key += 1;
			}
			else if (insidePoint(prov) == 0) {
				key += 0;
			}
		}
		if (key > 0) {
			glBegin(GL_POINTS);
			glColor3f(0, 1, 0);
			glVertex2f(vcheck[vcheck.size() - 1].x, vcheck[vcheck.size() - 1].y);
			glEnd();
		}
		else {
			glBegin(GL_POINTS);
			glColor3f(1, 0, 0);
			glVertex2f(vcheck[vcheck.size() - 1].x, vcheck[vcheck.size() - 1].y);
			glEnd();
		}
	}
	glFlush();
}

void mouse(int button, int state, int x, int y) {
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
		Point p = convertCoordinate(x, y);
		v.push_back(p);
		if (isClose == true) {
			vcheck.push_back(p);
			v.pop_back();
		}
		display();

	}
}

void keyboard(unsigned char key, int x, int y)
{
	if (key == 'z') {
		if (isClose == false)
		{
			int t;
			Point o;
			cout << "Ввести координаты вершины? 1 - да, 2 - нет." << endl;
			cin >> t;
			if (t == 1)
			{
				cout << "Введите координаты: " << endl;
				cin >> o.x >> o.y;
				while (o.x > 10 || o.x < -1 || o.y > 10 || o.y < -1)
				{
					cout << "Ошибка в координатах. Введите координаты ещё раз: " << endl;
					cin >> o.x >> o.y;
				}
				v.push_back(o);
				display();
			}
		}
		else
		{
			int t;
			Point o;
			cout << "Ввести координаты точки для проверки? 1 - да, 2 - нет." << endl;
			cin >> t;
			if (t == 1)
			{
				cout << "Введите координаты: " << endl;
				cin >> o.x >> o.y;
				while (o.x > 10 || o.x < -1 || o.y > 10 || o.y < -1)
				{
					cout << "Ошибка в координатах. Введите координаты ещё раз: " << endl;
					cin >> o.x >> o.y;
				}
				vcheck.push_back(o);
				display();
			}
		}
	}
}

void draw_border(void) {
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

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "rus");
	glutInit(&argc, argv);
	int n;
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(widht, height);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("God, bless me");
	init();
	draw_border();
	glutDisplayFunc(display);
	cout << "Выберите способ ввода: " << endl;
	cout << "1 - мышкой" << endl;
	cout << "2 - клавиатурой" << endl;
	cin >> n;
	if (n == 1) {
		glutDisplayFunc(display);
		glutMouseFunc(mouse);
	}
	else
	{
		glutDisplayFunc(display);
		glutKeyboardFunc(keyboard);
	}
	glutMainLoop();
	return 0;
}

