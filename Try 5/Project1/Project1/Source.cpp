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
vector<float>P;
vector<float>Q;
Point vec3, vec4;
Point Pmin, Pmax;

void init(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(xmin, xmax, ymin, ymax);
}

void display(void)
{
	glColor3f(0.5, 0.5, 0.5);
	glEnable(GL_POINT_SMOOTH);

	glPointSize(8);
	if ((v.size() > 1) && (abs(v[v.size() - 1].x - v[v.size() - 2].x) < eps) && (abs(v[v.size() - 1].y - v[v.size() - 2].y) < eps)) {
		v.pop_back();
	}
	if (v.size() > 0) {
		glBegin(GL_POINTS);
		glColor3f(0.0, 0.0, 0.0);
		glVertex2f(v[v.size() - 1].x, v[v.size() - 1].y);
		glEnd();
	}
	if (v.size() == 2 && v.size()!=0) {
		glBegin(GL_POINTS);
		glColor3f(0.0, 0.0, 0.0);
		glVertex2f(v[v.size() - 1].x, v[v.size() - 1].y);
		glEnd();
		vec3.x = v[v.size() - 2].x;
		vec3.y = v[v.size() - 1].y;
		vec4.x = v[v.size() - 1].x;
		vec4.y = v[v.size() - 2].y;
		v.push_back(vec3);
		v.push_back(vec4);
		swap(v[1], v[2]);
		glBegin(GL_POINTS);
		glColor3f(0.0, 0.0, 0.0);
		glVertex2f(v[1].x, v[1].y);
		glEnd();
		glBegin(GL_POINTS);
		glColor3f(0.0, 0.0, 0.0);
		glVertex2f(v[3].x, v[3].y);
		glEnd();
		glBegin(GL_LINE_LOOP);
		glVertex2f(v[0].x, v[0].y);
		glVertex2f(v[1].x, v[1].y);
		glVertex2f(v[2].x, v[2].y);
		glVertex2f(v[3].x, v[3].y);
		glEnd();
		Pmin.x = v[0].x;
		Pmin.y = v[0].y;
		Pmax.x = v[2].x;
		Pmax.y = v[2].y;

		cout << "min X: " << Pmin.x << endl;
		cout << "max X: " << Pmax.x << endl;
		cout << "min Y: " << Pmin.y << endl;
		cout << "min Y: " << Pmax.y << endl;
		isClose = true;
		}
	if (v2.size() > 0 && isClose == true) {
		glBegin(GL_POINTS);
		glColor3f(0.0, 0.0, 0.0);
		glVertex2f(v2[v2.size() - 1].x, v2[v2.size() - 1].y);
		glEnd();
	}
	if (v2.size() % 2 == 0 && isClose==true && v2.size() != 0){
		bool flag = false;
		cout << "1-ая точка отрезка: " << "(" << v2[v2.size() - 2].x << " : " << v2[v2.size() - 2].y << ")" << endl;
		cout << "2-ая точка отрезка: " << "(" << v2[v2.size() - 1].x << " : " << v2[v2.size() - 1].y << ")" << endl;
		P.push_back(-(v2[v2.size() - 1].x - v2[v2.size() - 2].x));
		P.push_back((v2[v2.size() - 1].x - v2[v2.size() - 2].x));
		P.push_back(-(v2[v2.size() - 1].y - v2[v2.size() - 2].y));
		P.push_back((v2[v2.size() - 1].y - v2[v2.size() - 2].y));

		Q.push_back(v2[v2.size() - 2].x - Pmin.x);
		Q.push_back(Pmax.x - v2[v2.size() - 2].x);
		Q.push_back(v2[v2.size() - 2].y - Pmin.y);
		Q.push_back(Pmax.y - v2[v2.size() - 2].y);


		float t0 = 0, t1 = 1;
		for (int i = 0; i < 4; i++)
		{
			if (P[i] > 0)
			{
				t1 = min(t1, Q[i] / P[i]);
			}
			else if (P[i] < 0)
			{
				t0 = max(t0, Q[i] / P[i]);
			}
			else if (P[i] == 0)
			{
				if (Q[i] >= 0)
				{
					continue;
				}
				else
				{
					glEnable(GL_LINE_STIPPLE);
					glColor3f(0.0, 0.0, 0.0);
					glLineStipple(4, 0xAAAA);
					glBegin(GL_LINES);
					glVertex2f(v2[v2.size() - 2].x, v2[v2.size() - 2].y);
					glVertex2f(v2[v2.size() - 1].x, v2[v2.size() - 1].y);
					glEnd();
					glDisable(GL_LINE_STIPPLE);
					flag = true;
					break;
				}
			}
			
		}
		P.pop_back();
		P.pop_back();
		P.pop_back();
		P.pop_back();
		Q.pop_back();
		Q.pop_back();
		Q.pop_back();
		Q.pop_back();
		if (flag == false)
		{
			if (t0 >= 0 && t0 <= 1 && t1 >= 0 && t1 <= 1 && t0 < t1)
			{
				Point new1, new2;
				new1.x = v2[v2.size() - 2].x + (v2[v2.size() - 1].x - v2[v2.size() - 2].x) * t0;
				new1.y = v2[v2.size() - 2].y + (v2[v2.size() - 1].y - v2[v2.size() - 2].y) * t0;
				new2.x = v2[v2.size() - 2].x + (v2[v2.size() - 1].x - v2[v2.size() - 2].x) * t1;
				new2.y = v2[v2.size() - 2].y + (v2[v2.size() - 1].y - v2[v2.size() - 2].y) * t1;
				glBegin(GL_LINES);
				glColor3f(0.0, 0.0, 0.0);
				glVertex2f(new1.x, new1.y);
				glVertex2f(new2.x, new2.y);
				glEnd();


				/*glBegin(GL_LINES);
				glColor3f(0.0, 0.0, 0.0);
				glVertex2f(v2[v2.size() - 2].x, v2[v2.size() - 2].y);
				glVertex2f(v2[v2.size() - 1].x, v2[v2.size() - 1].y);
				glEnd();*/
				

				glEnable(GL_LINE_STIPPLE);
				glColor3ub(0, 0, 0);
				glLineStipple(4, 0xAAAA);
				glBegin(GL_LINES);
				glVertex2f(v2[v2.size() - 2].x, v2[v2.size() - 2].y);
				glVertex2f(new1.x, new1.y);
				glEnd();
				
				glColor3ub(0, 0, 0);
				glLineWidth(4);
				glBegin(GL_LINES);
				glVertex2f(v2[v2.size() - 1].x, v2[v2.size() - 1].y);
				glVertex2f(new2.x, new2.y);
				glEnd();
				
				glDisable(GL_LINE_STIPPLE);
			}
			else
			{
				glEnable(GL_LINE_STIPPLE);
				glLineStipple(4, 0xAAAA);
				glBegin(GL_LINES);
				glColor3f(0.0, 0.0, 0.0);
				glVertex2f(v2[v2.size() - 2].x, v2[v2.size() - 2].y);
				glVertex2f(v2[v2.size() - 1].x, v2[v2.size() - 1].y);
				glEnd();
				glDisable(GL_LINE_STIPPLE);
				
			}
		}
		cout << "------------------------" << endl;
	}
	

	glFlush();
}

void mouse(int button, int state, int x, int y) {
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
		Point p = convertCoordinate(x, y);
			v.push_back(p);
		if (isClose == true) {
			v2.push_back(p);
			v.pop_back();
		}
		display();

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
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(widht, height);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("God, bless me");
	init();
	draw_border();
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMainLoop();
	return 0;
}

