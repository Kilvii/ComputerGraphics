#include <windows.h>
#include <GL/gl.h>
#include "GL/glu.h"
#include "GL/glut.h"
#include <iostream>

using namespace std;

float const xmax = 10, xmin = -1, ymax = 10, ymin = -1, delta = 0.5, width = 660, height = 660;

class Point {
	public: double x, y;
	};

int const n = 3;
Point p[n];

void draw_border(void) {
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(xmin, xmax, ymin, ymax);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glLineWidth(1);
	glLineStipple(1, 0xAAa1);
	glEnable(GL_LINE_STIPPLE);
	glColor3f(0.0, 0.0, 0.0);
	for (int i = ceil(xmin); i <= floor(xmax) - 1; i++) {
		glBegin(GL_LINES);
		glVertex2f(i, ymin + delta);
		glVertex2f(i, ymax - delta);
		glEnd();
	}
	for (int i = ceil(ymin) + 1; i <= floor(ymax) - 1; i++) {
		glBegin(GL_LINES);
		glVertex2f(xmin + delta, i);
		glVertex2f(xmax - delta, i);
		glEnd();

	}
	glDisable(GL_LINE_STIPPLE);

	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex2f(xmin + delta, 0.0);
	glVertex2f(xmax - delta, 0.0);
	glVertex2f(0.0, ymin + delta);
	glVertex2f(0.0, ymax - delta);
	glEnd();
	glFlush();
}
float PSP(Point a, Point b, Point c) {
	return ((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x));
}
void display(void)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_LINES);
	glColor3ub(0, 0, 0);
	glVertex2f(-1, 0);
	glVertex2f(10, 0);
	glVertex2f(0, -1);
	glVertex2f(0, 10);
	glEnd();

	for (int i = 0; i < 11; i += 1)
	{
		glPointSize(5);
		glEnable(GL_POINT_SMOOTH);
		glColor3ub(0, 0, 0);
		glBegin(GL_POINTS);
		glVertex2f(i - 1, 0);
		glPointSize(5);
		glEnd();
	}
	for (int j = 0; j < 11; j += 1)
	{
		glPointSize(5);
		glEnable(GL_POINT_SMOOTH);
		glColor3ub(0, 0, 0);
		glBegin(GL_POINTS);
		glVertex2f(0, j - 1);
		glEnd();
	}
	glColor3ub(0, 0, 0);
	glBegin(GL_POINTS);
	glEnable(GL_POINT_SMOOTH);
	glVertex2f(p[0].x, p[0].y);
	glVertex2f(p[1].x, p[1].y);
	glVertex2f(p[2].x, p[2].y);
	glEnd();

	glBegin(GL_LINES);
	glColor3ub(250, 0, 0);
	glVertex2f(p[0].x, p[0].y);
	glVertex2f(p[1].x, p[1].y);
	glColor3ub(0, 0, 250);
	glVertex2f(p[1].x, p[1].y);
	glVertex2f(p[2].x, p[2].y);
	glEnd();

	glFlush();
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "rus");
	cout << "Введите координаты точки B:" << endl;
	cin >> p[1].x; cin >> p[1].y;
	cout << "Введите координаты точки A:" << endl;
	cin >> p[0].x; cin >> p[0].y;
	cout << "Введите координаты точки C:" << endl;
	cin >> p[2].x; cin >> p[2].y;
	cout << "------------------------" << endl;

	int x1, x2, y1, y2;
	float fi,turn,s;
	x1 = p[0].x - p[1].x;
	y1 = p[0].y - p[1].y;
	x2 = p[2].x - p[1].x;
	y2 = p[2].y - p[1].y;

	fi = ((x1 * x2 + y1 * y2) / (sqrt(x1 * x1 + y1 * y1) * sqrt(x2 * x2 + y2 * y2)));

	s = PSP(p[0], p[1], p[2]);
	cout <<"Значение псевдоскаляра: "<< s << endl;
	cout << "Значения косинуса угла между векторами: " << fi << endl;
	//////////////////////////
	if (s == 0) {
		cout << "Векторы коллинеарны=> ";
		if (fi > 0) {
			cout << "Векторы сонаправленные.Угол между ними равен 0°" << endl;
		}
		else {
			cout << "Векторы противоположно направленные.Угол между ними равен 180°" << endl;
		}

	}
	///////////////////////////
	else if (s != 0) {
		cout << "Кратчайший поворот осуществляется: ";
		if (s > 0) {
			cout << "Против часовой стрелки " << endl;
		}
		else if (s < 0) {
			cout << "По часовой стрелки " << endl;
		}
	}
	///////////////////////////
	if (fi == 0) {
		cout << "Угол между векторами прямой" << endl;
	}
	else if (((fi > 0) && (s>0)) || ((fi > 0 && (s < 0)))) {
		cout << "Угол между векторами острый" << endl;
	}
	else if (((fi < 0) && (s<0)) || ((fi<0 && (s>0)))){
		cout << "Угол между векторами тупой" << endl;
	}
	//////////////////////////
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(660, 660);
	glutInitWindowPosition(250, 50);
	glutCreateWindow("Lines and corners");
	draw_border();
	glutDisplayFunc(display);
	glShadeModel(GL_FLAT);
	glutMainLoop();
	return 0;
}