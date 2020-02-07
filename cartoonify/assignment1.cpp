/***
 assignment1.cpp
 Assignment-1: Cartoonify

 Name: Wong, Alex (Please write your name in Last Name, First Name format)

 Collaborators: Doe, John; Doe, Jane
 ** Note: although the assignment should be completed individually
 you may speak with classmates on high level algorithmic concepts. Please
 list their names in this section

 Project Summary: A short paragraph (3-4 sentences) describing the work you
 did for the project: e.g. did you use the Chaikin's or Bezier's algorithm?
 Did you take an iterative or recursive approach?
 ***/


#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <vector>
#include <iostream>
using namespace std;

class Vertex {
    GLfloat x, y;
public:
    Vertex(GLfloat, GLfloat);
    GLfloat get_y() { return y; };
    GLfloat get_x() { return x; };
};

Vertex::Vertex(GLfloat X, GLfloat Y) {
    x = X;
    y = Y;
}

vector<Vertex> testCurve;
vector<Vertex> head;

void setup() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    testCurve.insert(testCurve.begin()+testCurve.size()/2, Vertex(-0.8, 0.8));
    testCurve.push_back(Vertex(0.0, 0.0));
    testCurve.push_back(Vertex(0.8, 0.8));

    head.push_back(Vertex(-.85,0));
    head.push_back(Vertex(-.8,.4));
    head.push_back(Vertex(-.58,.69));
    head.push_back(Vertex(0,.92));
    head.push_back(Vertex(.63,.69));
    head.push_back(Vertex(.8,0));
    head.push_back(Vertex(.6,-.64));
    head.push_back(Vertex(0,-.87));
    head.push_back(Vertex(-.6,-.75));
    head.push_back(Vertex(-.8,-.6));
    head.push_back(Vertex(-.85,0));
}

Vertex midpoint(Vertex p1, Vertex p2) {
  GLfloat x = (p1.get_x() + p2.get_x()) / 2;
  GLfloat y = (p1.get_y() + p2.get_y()) / 2;
  return Vertex(x, y);
}

vector<Vertex> generate_points(vector<Vertex> control_points) {
    vector<Vertex> points;
    points.insert(points.begin() + points.size() / 2, control_points[0]);
    points.insert(points.begin() + points.size() / 2, control_points[control_points.size()-1]);
    vector<Vertex> midpointsTemp = control_points;
    for(;;){
      vector<Vertex> temp;
      for(int i = 0; i < midpointsTemp.size() - 1; i++){
        temp.push_back(midpoint(midpointsTemp[i], midpointsTemp[i + 1]));
      }
      if(temp.size() >= 2){
        points.insert(points.begin() + points.size() / 2, temp[0]);
        points.insert(points.begin() + points.size() / 2, temp[temp.size()-1]);
      }else{
        points.insert(points.begin() + points.size() / 2, temp[0]);
        break;
      }
      midpointsTemp = temp;
    }

    return points;
}


void draw_curve(vector<Vertex> control_points, int n_iter) {

  for(int i = 0; i < n_iter; i++){
    control_points = generate_points(control_points);
  }


  glBegin(GL_LINES);
  int counter = 0;
  for(int i = 0; i < control_points.size(); i++){
    glVertex2f(control_points[i].get_x(), control_points[i].get_y());
    counter++;
    if(counter == 2){
      i--;
      counter = 0;
    }
  }

  glEnd();

  glPointSize(5.0f);

  glColor3f(1.0f, 0.0f, 0.0f);

  glBegin(GL_POINTS);
  for(int i = 0; i < control_points.size(); i++){
    glVertex2f(control_points[i].get_x(), control_points[i].get_y());
  }

  glEnd();



}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Set our color to black (R, G, B)
    glColor3f(0.0f, 0.0f, 0.0f);

    glLineWidth(10.0f);

    draw_curve(head,5);

    glutSwapBuffers();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(2000, 2000); // Set your own window size
    glutCreateWindow("Assignment 1");
    setup();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
