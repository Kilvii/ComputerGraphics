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
bool prov = false; \
float radius;

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
vector<Point>vup;
vector<Point>vdown;
vector<Point>vleft;
vector<Point>vright;

void init(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(xmin, xmax, ymin, ymax);
}

void draw_border(void);

void peresech(double x, double y, double k, double b, double r)
{ 
	double d = pow(-2 * x - 2 * b * k - 2 * y * k, 2) - 4 * (1 + k * k) * (b * b + y * y + x * x + 2 * b * y - r * r);

	
	if (d < 0)
	{
		cout <<"\n\nПрямая и окружность не пересекаются";
	}
	else
	{
		
		double x1 = ((-(2 * k * b - 2 * x - 2 * y * k) - sqrt(d)) / (2 + 2 * k * k));
		double x2 = ((-(2 * k * b - 2 * x - 2 * y * k) + sqrt(d)) / (2 + 2 * k * k));
		double y1 = k * x1 + b;
		double y2 = k * x2 + b;

		cout << "\n\nПрямая и окружность пересекаются в точках:\n";
		cout << x1 << ' ' << y1 << '\n' << x2 << ' ' << y2 << '\n';
		glBegin(GL_POINTS);
		glColor3f(1.0, 0.0, 0.0);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
		glEnd();
	}
}

void display(void)
{
	glColor3f(0.5, 0.5, 0.5);
	glEnable(GL_POINT_SMOOTH);
	
	// << "Точки ставятся по часовой" << endl;
	glPointSize(8);
	if ((v.size() > 0) && !isClose) {
			glBegin(GL_POINTS);
			glColor3f(0.0, 0.0, 0.0);
			glVertex2f(v[v.size() - 1].x, v[v.size() - 1].y);
			glEnd();
	}
	if (v.size() ==2) {
		//glBegin(GL_LINES);
		//glColor3f(0.0, 0.0, 0.0);
		//glVertex2f(v[v.size() - 2].x, v[v.size() - 2].y);
		//glVertex2f(v[v.size() - 1].x, v[v.size() - 1].y);
		//glEnd();
		Point Centre;
		Centre.x = (v[v.size() - 2].x + v[v.size() - 1].x) / 2;
		Centre.y = (v[v.size() - 2].y + v[v.size() - 1].y) / 2;
		glBegin(GL_POINTS);
		glColor3f(0.0, 0.0, 0.0);
		glVertex2f(Centre.x, Centre.y);
		glEnd();
		float r = sqrt((v[1].x - v[0].x) * (v[1].x - v[0].x) + (v[1].y - v[0].y) * (v[1].y - v[0].y)) / 2;
		cout << "rad= " << r << endl;
		double step = 2 *	M_PI / 30;
		Point A1(((v[v.size() - 2].x + v[v.size() - 1].x) / 2) + r, ((v[v.size() - 2].y + v[v.size() - 1].y) / 2));
		Point A2;
		glBegin(GL_LINE_LOOP);
		for (double f = 0; f <= 2 * M_PI; f += step) {
			A2 = Point(((v[v.size() - 2].x + v[v.size() - 1].x) / 2) + r * cos(f), ((v[v.size() - 2].y + v[v.size() - 1].y) / 2) + r * sin(f));
			glVertex2f(A1.x, A1.y);
			glVertex2f(A2.x, A2.y);
			A1 = A2;
		}
		glEnd();
	}
	if (v.size() == 4) {
		float b = v[v.size() - 2].y - (v[v.size() - 2].x * ((v[v.size() - 1].y - v[v.size() - 2].y) / (v[v.size() - 1].x - v[v.size() - 2].x)));
		cout << "b= " << b << endl;
		float k = (v[v.size() - 1].y - v[v.size() - 2].y) / (v[v.size() - 1].x - v[v.size() - 2].x);
		cout << "k= " << k << endl;
		float x1 = (ymin-b)/k;
		float x2 = (ymax-b)/k;
		glBegin(GL_LINES);
		glColor3f(0.0, 0.0, 0.0);
		glVertex2f(x1, ymin);
		cout << "x1= " << x1<<endl;
		glVertex2f(x2,ymax);
		cout << "x2= " << x2;
		glEnd();
		peresech(((v[v.size() - 2].x + v[v.size() - 1].x) / 2), ((v[v.size() - 2].y + v[v.size() - 1].y) / 2) ,k,b,radius);
	}
		
	
		
		/*for (int i = 0; i < 3; i++) {
			Point P1;
			P1.x = v[0].x - dlinB/2 ;
			P1.y = v[0].y + (dlinA);
			v.push_back(P1);
			Point P2;
			P2.x = v[1].x - dlinB / 2;
			P2.y = v[1].y + (dlinA );
			v.push_back(P2);
			Point P3;
			P3.x = v[2].x - dlinB / 2;
			P3.y = v[2].y + (dlinA );
			v.push_back(P3);
			Point P4;
			P4.x = v[3].x - dlinB / 2;
			P4.y = v[3].y + (dlinA );
			v.push_back(P4);
			glColor3ub(0, 0, 0);
			glBegin(GL_POINTS);
			glVertex2f(P1.x, P1.y);
			glVertex2f(P2.x, P2.y);
			glVertex2f(P3.x, P3.y);
			glVertex2f(P4.x, P4.y);
			glEnd();
			glBegin(GL_QUADS);
			glVertex2f(P1.x, P1.y);
			glVertex2f(P2.x, P2.y);
			glVertex2f(P3.x, P3.y);
			glVertex2f(P4.x, P4.y);
			glEnd();
			i++;
		}
		for (int i = 0; i < 3; i++) {
			Point P1;
			P1.x = v[0].x - dlinB-1;
			P1.y = v[0].y - (dlinA);
			v.push_back(P1);
			Point P2;
			P2.x = v[1].x - dlinB-1;
			P2.y = v[1].y - (dlinA);
			v.push_back(P2);
			Point P3;
			P3.x = v[2].x - dlinB-1;
			P3.y = v[2].y - (dlinA);
			v.push_back(P3);
			Point P4;
			P4.x = v[3].x - dlinB-1;
			P4.y = v[3].y - (dlinA);
			v.push_back(P4);
			glColor3ub(0, 0, 0);
			glBegin(GL_POINTS);
			glVertex2f(P1.x, P1.y);
			glVertex2f(P2.x, P2.y);
			glVertex2f(P3.x, P3.y);
			glVertex2f(P4.x, P4.y);
			glEnd();
			glBegin(GL_QUADS);
			glVertex2f(P1.x, P1.y);
			glVertex2f(P2.x, P2.y);
			glVertex2f(P3.x, P3.y);
			glVertex2f(P4.x, P4.y);
			glEnd();
			i++;
		} */


	glFlush();
}

void mouse(int button, int state, int x, int y) {
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
		Point p = convertCoordinate(x, y);
		v.push_back(p);
		if (isClose == true) {
			//vcheck.push_back(p);
			//v.pop_back();
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
	int n;
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

