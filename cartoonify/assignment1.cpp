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

void setup() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    testCurve.insert(testCurve.begin()+testCurve.size()/2, Vertex(-0.1, 0.1));
    testCurve.push_back(Vertex(-0.1, -0.1));
    testCurve.push_back(Vertex(0.1, -0.1));
}

vector<Vertex> generate_points(vector<Vertex> control_points, int n_iter) {
    vector<Vertex> points;
    points.insert(points.begin() + points.size() / 2, control_points[0]));
    points.insert(points.begin() + points.size() / 2, control_points[control_points.size()-1]);
    for(int j = 0; j < n_iter; j++){
      vector<Vertex> temp;
      for(int i = 0; i < control_points.size() - 1; i++){
        temp.pushback(midpoint(control_points[i], control_points[i + 1]));
      }
      if(temp.size() >= 2){
        points.insert(points.begin() + points.size() / 2, temp[0]));
        points.insert(points.begin() + points.size() / 2, temp.size()-1]);
      }else{
        points.insert(points.begin() + points.size() / 2, temp[0]));
      }
    }

    return points;
}

Vertex midpoint(Vertex p1, Vertex p2){
  GLfloat x = (p1.get_x() + p2.get_x()) / 2;
  GLfloat y = (p1.get_y() + p2.get_y()) / 2;
  return Vertex(x, y);
}

void draw_curve(vector<Vertex> control_points) {

  glBegin(GL_POINTS);
  generate_points(control_points);
  glEnd();

}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Set our color to black (R, G, B)
    glColor3f(0.0f, 0.0f, 0.0f);

    // TODO:
    // Draw cartoon

    glutSwapBuffers();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(800, 600); // Set your own window size
    glutCreateWindow("Assignment 1");
    setup();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
