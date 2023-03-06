#include <windows.h>
#include "GL/gl.h"
#include "GL/glu.h"
#include <GL/glut.h>
#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include <limits>

using namespace std;

float const xmax = 10, xmin = -1, ymax = 10, ymin = -1, delta = 0.5;
int const widht = 600, height = 600;
float const scaleX = widht / (xmax - xmin), scaleY = height / (ymax - ymin);
bool isClose = false;
bool check = true;
bool prov = false;
bool run = false;
bool idea = false;
float const eps = 0.1;
float epsi = 0.01;
float e1 = 0, e2 = 1;
float inf = e2 / e1;


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

vector<Point>v;

float Cross(Point a, Point b, Point c) {
	cout << "ffafa" << (a.x - b.x) * (c.y - b.y) - (a.y - b.y) * (c.x - b.x) << endl;
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

float Angle(Point a, Point b, Point c, Point d) {
	Point ab(b.x - a.x, b.y - a.y);
	Point cd(d.x - c.x, d.y - c.y);
	int dir;
	if (Bypass(ab, cd) == false) dir = -1;
	else dir = 1;
	return dir * acos((ab.x * cd.x + ab.y * cd.y) / (sqrt(ab.x * ab.x + ab.y * ab.y) * sqrt(cd.x * cd.x + cd.y * cd.y))) / (M_PI / 180);
}

int insidePoint(Point a)
{
	float angles = 0;
	int COCK = 0;
	for (int j = 1; j < v.size(); j++)
	{
		angles += Angle(a, v[j], a, v[j - 1]);
	}
	if (abs(angles) >= 180) {
		return 1;
	}
	else
		return 0;
}


vector<Point>v2;
vector<Point>vcheck;
vector <Point> NewPolygon;

void init(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(xmin, xmax, ymin, ymax);
}

void draw_border(void);

void CutPolygon() {
	int dir = -Cross(vcheck[vcheck.size() - 3], vcheck[vcheck.size() - 2], vcheck[vcheck.size() - 1]);
	cout << "dir" << dir <<  endl;
		float k, b;
	int kcheck = 0;
	int chiric = 0;
	for (int i = 1; i < v.size(); i++)
	{
		kcheck = 0;
		chiric = 0;
		cout << "Cross" << Cross(vcheck[0], vcheck[1], v[i - 1]) << endl;
		if (Cross(vcheck[0], vcheck[1], v[i - 1]) == dir || Cross(vcheck[0], vcheck[1], v[i - 1]) == 0)
		{
			if (NewPolygon.size() > 0)
			{
				if ((abs(NewPolygon[NewPolygon.size() - 1].x - v[i - 1].x) > epsi && abs(NewPolygon[NewPolygon.size() - 1].y - v[i - 1].y) > epsi) || (abs(NewPolygon[NewPolygon.size() - 1].x - v[i - 1].x) == 0 && abs(NewPolygon[NewPolygon.size() - 1].y - v[i - 1].y) > epsi) || (abs(NewPolygon[NewPolygon.size() - 1].x - v[i - 1].x) > epsi && abs(NewPolygon[NewPolygon.size() - 1].y - v[i - 1].y) == 0))
					NewPolygon.push_back(v[i - 1]);
			}
			else NewPolygon.push_back(v[i - 1]);
		}
		if (vcheck[0].y == vcheck[1].y && vcheck[0].y != 0) { k = 0; chiric++; }
		else  k = ((vcheck[vcheck.size() - 2].y - vcheck[vcheck.size() - 3].y) / (vcheck[vcheck.size() - 2].x - vcheck[vcheck.size() - 3].x));
		if (vcheck[0].x == vcheck[1].x && vcheck[0].x != 0)
		{
			b = vcheck[0].x;
			kcheck++;

		}
		else  b = vcheck[0].y - k * vcheck[0].x;
		float b1, k1;
		Point FToPayRespect;
		if (v[i - 1].y == v[i].y && v[i - 1].y != 0) { k1 = 0; chiric += 2; }
		else k1 = (v[i].y - v[i - 1].y) / (v[i].x - v[i - 1].x);
		if (v[i].x == v[i - 1].x && v[i].x != 0)
		{
			kcheck += 2;
			b1 = v[i].x;
		}
		else  b1 = v[i].y - k1 * v[i].x;
		if (kcheck == 1 && chiric == 2) {
			FToPayRespect.x = b;
			FToPayRespect.y = b1;
		}
		else if (kcheck == 2 && chiric == 1) {
			FToPayRespect.x = b1;
			FToPayRespect.y = b;
		}
		else {
			FToPayRespect.x = (b1 - b) / (k - k1);
			FToPayRespect.y = (k * (b1 - b)) / (k - k1) + b;
		}

		float Dlin1 = Length(FToPayRespect, v[i - 1]);

		float Dlin2 = Length(FToPayRespect, v[i]);

		float DlinObsh = Length(v[i], v[i - 1]);

		float DlinSum = Dlin1 + Dlin2;

		float DlinRazn = round(round(DlinObsh) - round(DlinSum));



		if (DlinRazn == 0) {

			NewPolygon.push_back(FToPayRespect);
		}
	}


	cout << "--------------------------------------------" << endl;



	vcheck.clear();
	if (NewPolygon.size() > 0)
	{
		v.clear();
		v.push_back(NewPolygon[0]);
		int m = 0;
		if (NewPolygon.size() > 1)
			for (int w = 1; w < NewPolygon.size(); w++) {
				v.push_back(NewPolygon[w]);
				m++;
				if (v[m - 1].x == v[m].x && v[m - 1].y == v[m].y)
				{
					v.pop_back(); m--;
				}

			}
	}
	for (int pik = 0; pik < v.size(); pik++) {
		cout << v[pik].x << " " << v[pik].y << " Newbie" << endl;
	}
	NewPolygon.clear();
	init();
	draw_border();
	if (v.size() > 0) {
		for (int i = 1; i < v.size(); i++) {
			glBegin(GL_POINTS);
			glVertex2f(v[i - 1].x, v[i - 1].y);
			glVertex2f(v[i].x, v[i].y);
			glEnd();
			glBegin(GL_LINES);
			glVertex2f(v[i - 1].x, v[i - 1].y);
			glVertex2f(v[i].x, v[i].y);
			glEnd();
		}
		glBegin(GL_POINTS);
		glVertex2f(v[0].x, v[0].y);
		glVertex2f(v[v.size() - 1].x, v[v.size() - 1].y);
		glEnd();
		glBegin(GL_LINES);
		glVertex2f(v[0].x, v[0].y);
		glVertex2f(v[v.size() - 1].x, v[v.size() - 1].y);
		glEnd();

		v.push_back(v[0]);
	}

}


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
		cout << "Данное нечно нельзя отсечь." << endl;
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
	if (prov == true && idea == false)
	{
		for (int i = 0; i < v.size() - 1; i++)
		{
			v2.push_back(Point());
			v2[i].x = v[i + 1].x - v[i].x;
			v2[i].y = v[i + 1].y - v[i].y;
		}

		int Strive = 1;
		for (int i = 0; i < v.size() - 2; i++)
		{
			for (int j = i + 3; j < v.size(); j++)
			{
				if (Cross(v[i + 1], v[i], v[j - 1]) * Cross(v[i + 1], v[i], v[j]) <= 0 && Cross(v[j], v[j - 1], v[i + 1]) * Cross(v[j], v[j - 1], v[i]) < 0)
				{
					Strive = -1;
					break;
				}
			}
		}

		if (Strive == 1)
		{
			int Test = 1;
			int q = Bypass(v2[0], v2[v2.size() - 1]);
			for (int i = 0; i < v2.size() - 1; i++)
			{
				if (Bypass(v2[i + 1], v2[i]) != q)
				{
					Test = -1;
					break;
				}
			}
			if (Test == 1)
			{
				cout << "Полигон выпуклый." << endl;
				idea = true;
			}
			else if (idea == false) {
				cout << "Полигон вогнутый.Перерисуйте полигон." << endl;
				init();
				draw_border();
				v.clear();
				v2.clear();
				isClose = false;
				prov = false;
			}
		}
		else if (idea == false)
		{
			cout << "Полигон имеет самопересечение(-я). Перерисуйте полигон." << endl;
			init();
			draw_border();
			v.clear();
			v2.clear();
			isClose = false;
			prov = false;
		}
	}
	if ((isClose == true) && (vcheck.size() > 0) && (v.size() > 3)) {
		if (vcheck.size() % 3 != 0 && vcheck.size() % 2 != 0)
		{
			glBegin(GL_POINTS);
			glColor3f(255, 0, 0);
			glVertex2f(vcheck[vcheck.size() - 1].x, vcheck[vcheck.size() - 1].y);
			glEnd();
		}
		if (vcheck.size() % 2 == 0 && vcheck.size() % 3 != 0) {
			glBegin(GL_POINTS);
			glColor3f(255, 0, 0);
			glVertex2f(vcheck[vcheck.size() - 1].x, vcheck[vcheck.size() - 1].y);
			glEnd();

			glBegin(GL_LINES);
			glColor3f(0.0, 0.0, 0.0);
			glVertex2f(vcheck[vcheck.size() - 2].x, vcheck[vcheck.size() - 2].y);
			glVertex2f(vcheck[vcheck.size() - 1].x, vcheck[vcheck.size() - 1].y);
			glEnd();
		}
		if (vcheck.size() % 3 == 0) {
			if (Cross(vcheck[0], vcheck[1], vcheck[2]) != 0)
				CutPolygon();
			else vcheck.pop_back();
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
			cout << "Ввести координаты точки отсекающей прямой? 1 - да, 2 - нет." << endl;
			cin >> t;
			if ((v.size() == 3) && isClose == true) {
				cout << "Данное нечто нельзя отсечь." << endl;
				t == 2;
			}
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

