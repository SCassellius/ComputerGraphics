/***
 Assignment-2: Rotating a Cube in 3-Dimensional Space

 Name: Wong, Alex (Please write your name in Last Name, First Name format)

 Collaborators: Doe, John; Doe, Jane
 ** Note: although the assignment should be completed individually
 you may speak with classmates on high level algorithmic concepts. Please
 list their names in this section

 Project Summary: A short paragraph (3-4 sentences) describing the work you
 did for the project.
 ***/

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#pragma GCC diagnostic pop

#include <math.h>
#include <vector>
#include <iostream>
using namespace std;


float theta = 0.0;
int runPrintArray = 0;

// Converts degrees to radians for rotation
float deg2rad(float d) {
    return (d*M_PI)/180.0;
}

// Converts a vector to an array
GLfloat* vector2array(vector<GLfloat> vec) {
    GLfloat* arr = new GLfloat[vec.size()];
    for (int i = 0; i < vec.size(); i++) {
        arr[i] = vec[i];
    }
    return arr;
}

void printArray(vector<GLfloat> array) {
  if(runPrintArray < 1){
    for(int i = 0; i < array.size(); i++){
      cout << array[i] << " ";
    }
    runPrintArray++;
  }
}

// Converts Cartesian coordinates to homogeneous coordinates  (Step 1)
vector<GLfloat> to_homogenous_coord(vector<GLfloat> cartesian_coords) {
    vector<GLfloat> result;
    for (int i = 0; i < cartesian_coords.size(); i++) {
        result.push_back(cartesian_coords[i]);
        if ((i + 1) % 3 == 0) {
            result.push_back(1);
        }
    }
    return result;
}

// Converts Cartesian coordinates to homogeneous coordinates
vector<GLfloat> to_cartesian_coord(vector<GLfloat> homogenous_coords) {
    vector<GLfloat> result;
    for (int i = 0; i < homogenous_coords.size(); i++) {
        if ((i + 1) % 4 != 0) {
            result.push_back(homogenous_coords[i]);
        }
    }
    return result;
}

// Definition of a rotation matrix about the x-axis theta degrees
vector<GLfloat> rotation_matrix_x (float theta) {
    vector<GLfloat> rotate_mat_x;
    GLfloat thetaInRadians = deg2rad(theta);
    GLfloat cosVal = cos(thetaInRadians);
    GLfloat sinVal = sin(thetaInRadians);
    rotate_mat_x.push_back(1);
    rotate_mat_x.push_back(0);
    rotate_mat_x.push_back(0);
    rotate_mat_x.push_back(0);
    rotate_mat_x.push_back(0);
    rotate_mat_x.push_back(cosVal);
    rotate_mat_x.push_back(sinVal * -1);
    rotate_mat_x.push_back(0);
    rotate_mat_x.push_back(0);
    rotate_mat_x.push_back(sinVal);
    rotate_mat_x.push_back(cosVal);
    rotate_mat_x.push_back(0);
    rotate_mat_x.push_back(0);
    rotate_mat_x.push_back(0);
    rotate_mat_x.push_back(0);
    rotate_mat_x.push_back(1);
    return rotate_mat_x;
}

// Definition of a rotation matrix along the y-axis by theta degrees
vector<GLfloat> rotation_matrix_y (float theta) {
    vector<GLfloat> rotate_mat_y;
    GLfloat thetaInRadians = deg2rad(theta);
    GLfloat cosVal = cos(thetaInRadians);
    GLfloat sinVal = sin(thetaInRadians);
    rotate_mat_y.push_back(cosVal);
    rotate_mat_y.push_back(0);
    rotate_mat_y.push_back(sinVal * -1);
    rotate_mat_y.push_back(0);
    rotate_mat_y.push_back(0);
    rotate_mat_y.push_back(1);
    rotate_mat_y.push_back(0);
    rotate_mat_y.push_back(0);
    rotate_mat_y.push_back(sinVal);
    rotate_mat_y.push_back(0);
    rotate_mat_y.push_back(cosVal);
    rotate_mat_y.push_back(0);
    rotate_mat_y.push_back(0);
    rotate_mat_y.push_back(0);
    rotate_mat_y.push_back(0);
    rotate_mat_y.push_back(1);
    return rotate_mat_y;
}

// Definition of a rotation matrix along the z-axis by theta degrees
vector<GLfloat> rotation_matrix_z (float theta) {
    vector<GLfloat> rotate_mat_z;
    GLfloat thetaInRadians = deg2rad(theta);
    GLfloat cosVal = cos(thetaInRadians);
    GLfloat sinVal = sin(thetaInRadians);
    rotate_mat_z.push_back(cosVal);
    rotate_mat_z.push_back(sinVal * -1);
    rotate_mat_z.push_back(0);
    rotate_mat_z.push_back(0);
    rotate_mat_z.push_back(sinVal);
    rotate_mat_z.push_back(cosVal);
    rotate_mat_z.push_back(0);
    rotate_mat_z.push_back(0);
    rotate_mat_z.push_back(0);
    rotate_mat_z.push_back(0);
    rotate_mat_z.push_back(1);
    rotate_mat_z.push_back(0);
    rotate_mat_z.push_back(0);
    rotate_mat_z.push_back(0);
    rotate_mat_z.push_back(0);
    rotate_mat_z.push_back(1);
    return rotate_mat_z;
}

// Perform matrix multiplication for A(the rotation matrix, 4x4) and B(the 24x4 homogenous points)
vector<GLfloat> mat_mult(vector<GLfloat> A, vector<GLfloat> B) {
    vector<GLfloat> result;
    GLfloat temp = 0;
    for(int k = 0; k < 24; k++){
      vector<GLfloat> point;
      for(int l = 0; l < 4; l++){
        point.push_back(B[(k * 4) + l]);
      }
      for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
          temp = temp + (A[(4 * i) + j] * point[j]);
        }
        result.push_back(temp);
        temp = 0;
      }
    }

    return result;
}

void setup() {
    // Enable the vertex array functionality
    glEnableClientState(GL_VERTEX_ARRAY);
    // Enable the color array functionality (so we can specify a color for each vertex)
    glEnableClientState(GL_COLOR_ARRAY);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    // Set up some default base color
    glColor3f(0.5, 0.5, 0.5);
    // Set up white background
    glClearColor(1.0, 1.0, 1.0, 0.0);
}

void init_camera() {
    // Camera parameters
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define a 50 degree field of view, 1:1 aspect ratio, near and far planes at 3 and 7
    gluPerspective(50.0, 1.0, 2.0, 10.0);
    // Position camera at (2, 3, 5), attention at (0, 0, 0), up at (0, 1, 0)
    gluLookAt(2.0, 3.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void display_func() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // World model parameters
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    vector<GLfloat> points = {
        // Front plane
        +1.0,   +1.0,   +1.0,
        -1.0,   +1.0,   +1.0,
        -1.0,   -1.0,   +1.0,
        +1.0,   -1.0,   +1.0,
        // Back plane
        +1.0,   +1.0,   -1.0,
        -1.0,   +1.0,   -1.0,
        -1.0,   -1.0,   -1.0,
        +1.0,   -1.0,   -1.0,
        // Right
        +1.0,   +1.0,   -1.0,
        +1.0,   +1.0,   +1.0,
        +1.0,   -1.0,   +1.0,
        +1.0,   -1.0,   -1.0,
        // Left
        -1.0,   +1.0,   -1.0,
        -1.0,   +1.0,   +1.0,
        -1.0,   -1.0,   +1.0,
        -1.0,   -1.0,   -1.0,
        // Top
        +1.0,   +1.0,   +1.0,
        -1.0,   +1.0,   +1.0,
        -1.0,   +1.0,   -1.0,
        +1.0,   +1.0,   -1.0,
        // Bottom
        +1.0,   -1.0,   +1.0,
        -1.0,   -1.0,   +1.0,
        -1.0,   -1.0,   -1.0,
        +1.0,   -1.0,   -1.0,
    };

    GLfloat colors[] = {
        // Front plane
        1.0,    0.0,    0.0,
        1.0,    0.0,    0.0,
        1.0,    0.0,    0.0,
        1.0,    0.0,    0.0,
        // Back plane
        0.0,    1.0,    0.0,
        0.0,    1.0,    0.0,
        0.0,    1.0,    0.0,
        0.0,    1.0,    0.0,
        // Right
        0.0,    0.0,    1.0,
        0.0,    0.0,    1.0,
        0.0,    0.0,    1.0,
        0.0,    0.0,    1.0,
        // Left
        1.0,    1.0,    0.0,
        1.0,    1.0,    0.0,
        1.0,    1.0,    0.0,
        1.0,    1.0,    0.0,
        // Top
        1.0,    0.0,    1.0,
        1.0,    0.0,    1.0,
        1.0,    0.0,    1.0,
        1.0,    0.0,    1.0,
        // Bottom
        0.0,    1.0,    1.0,
        0.0,    1.0,    1.0,
        0.0,    1.0,    1.0,
        0.0,    1.0,    1.0,
    };


    // TODO: Apply rotation(s) to the set of points

    points = to_homogenous_coord(points);
    points = mat_mult(rotation_matrix_x(theta), points);
    points = mat_mult(rotation_matrix_y(theta), points);
    points = mat_mult(rotation_matrix_z(theta), points);
    points = to_cartesian_coord(points);


    GLfloat* vertices = vector2array(points);

    glVertexPointer(3,          // 3 components (x, y, z)
                    GL_FLOAT,   // Vertex type is GL_FLOAT
                    0,          // Start position in referenced memory
                    vertices);  // Pointer to memory location to read from

    //pass the color pointer
    glColorPointer(3,           // 3 components (r, g, b)
                   GL_FLOAT,    // Vertex type is GL_FLOAT
                   0,           // Start position in referenced memory
                   colors);     // Pointer to memory location to read from

    // Draw quad point planes: each 4 vertices
    glDrawArrays(GL_QUADS, 0, 4*6);

    glFlush();			//Finish rendering
    glutSwapBuffers();
}

void idle_func() {
    theta = theta+0.003;
    display_func();
}

int main (int argc, char **argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    // Create a window with rendering context and everything else we need
    glutCreateWindow("Assignment 2");

    setup();
    init_camera();

    // Set up our display function
    glutDisplayFunc(display_func);
    glutIdleFunc(idle_func);
    // Render our world
    glutMainLoop();
    return 0;
}
