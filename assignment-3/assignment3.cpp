/***
 Assignment-3: Geometric Modeling of a Scene

 Name: Cassellius, Seth

 Collaborators: Wong, Alex; Shingledecker, Bennett
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

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <string>
#include <iostream>
using namespace std;

// If a float is < EPSILON or > -EPSILON then it should be 0
float EPSILON = 0.000001;
// theta is the angle to rotate the scene
float THETA = 0.0;
// Vector placeholders for the scene and color array
vector<GLfloat> SCENE;
vector<GLfloat> COLOR;

/**************************************************
 *  Rectangular Prisms via Hierarchical Modeling  *
 *                                                *
 *  using planes as building blocks, build a unit *
 *  cube with transformations that will serve as  *
 *  a primitive for modeling objects in the scene *
 *                                                *
 *************************************************/

 // Converts Cartesian coordinates to homogeneous coordinates
 vector<GLfloat> to_homogeneous_coord(vector<GLfloat> cartesian_coords) {
     vector<GLfloat> result;
     for (int i = 0; i < cartesian_coords.size(); i++) {
         result.push_back(cartesian_coords[i]);
         if ((i+1) % 3 == 0) {
             result.push_back(1.0);
         }
     }
     return result;
 }

// Initializes a square plane of unit lengths
vector<GLfloat> init_plane() {
    vector<GLfloat> vertices = {
        +0.5,   +0.5,   +0.0,
        -0.5,   +0.5,   +0.0,
        -0.5,   -0.5,   +0.0,
        +0.5,   -0.5,   +0.0
    };
    return vertices;
}

// Converts degrees to radians for rotation
float deg2rad(float d) {
    return (d*M_PI) / 180.0;
}

// Converts a vector to an array
GLfloat* vector2array(vector<GLfloat> vec) {
    GLfloat* arr = new GLfloat[vec.size()];
    for (int i = 0; i < vec.size(); i++) {
        arr[i] = vec[i];
    }
    return arr;
}

// Converts Cartesian coordinates to homogeneous coordinates
vector<GLfloat> to_cartesian_coord(vector<GLfloat> homogeneous_coords) {
    vector<GLfloat> result;
    for (int i = 0; i < homogeneous_coords.size(); i++) {
        if ((i+1) % 4 == 0) {
            continue;
        } else {
            result.push_back(homogeneous_coords[i]);
        }
    }
    return result;
}

// Definition of a translation matrix
vector<GLfloat> translation_matrix (float dx, float dy, float dz) {
    vector<GLfloat> translate_mat;
    translate_mat.push_back(1);
    translate_mat.push_back(0);
    translate_mat.push_back(0);
    translate_mat.push_back((GLfloat)dx);
    translate_mat.push_back(0);
    translate_mat.push_back(1);
    translate_mat.push_back(0);
    translate_mat.push_back((GLfloat)dy);
    translate_mat.push_back(0);
    translate_mat.push_back(0);
    translate_mat.push_back(1);
    translate_mat.push_back((GLfloat)dz);
    translate_mat.push_back(0);
    translate_mat.push_back(0);
    translate_mat.push_back(0);
    translate_mat.push_back(1);
    return translate_mat;
}

// Definition of a scaling matrix
vector<GLfloat> scaling_matrix (float sx, float sy, float sz) {
    vector<GLfloat> scale_mat;
    scale_mat.push_back((GLfloat)sx);
    scale_mat.push_back(0);
    scale_mat.push_back(0);
    scale_mat.push_back(0);
    scale_mat.push_back(0);
    scale_mat.push_back((GLfloat)sy);
    scale_mat.push_back(0);
    scale_mat.push_back(0);
    scale_mat.push_back(0);
    scale_mat.push_back(0);
    scale_mat.push_back((GLfloat)sz);
    scale_mat.push_back(0);
    scale_mat.push_back(0);
    scale_mat.push_back(0);
    scale_mat.push_back(0);
    scale_mat.push_back(1);
    return scale_mat;
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


// Definition of a rotation matrix about the y-axis by theta degrees
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


// Definition of a rotation matrix about the z-axis by theta degrees
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

// Perform matrix multiplication for A B
vector<GLfloat> mat_mult(vector<GLfloat> A, vector<GLfloat> B) {
    vector<GLfloat> result;
    for (int i = 0; i < B.size(); i += 4) {
        for (int j = 0; j < A.size(); j += 4) {
            GLfloat dot_product = 0.0;
            for (int k = 0; k < 4; k++){
                GLfloat value = A[j+k] * B[i+k];
                if (value < EPSILON && value > -1.0*EPSILON) {
                  value = 0.0;
                }
                dot_product += value;
            }
            result.push_back(dot_product);
        }
    }
    return result;
}

//append one vector into another
vector<GLfloat> push_back_helper(vector<GLfloat> from, vector<GLfloat> to){
  vector<GLfloat> result;
  for(int i = 0; i < to.size(); i++){
    result.push_back(to.at(i));
  }
  for(int i = 0; i < from.size(); i++){
    result.push_back(from.at(i));
  }
  return result;
}

//duplicate a vector
vector<GLfloat> copy_vector(vector<GLfloat> source){
  vector<GLfloat> copy;
    for(int i = 0; i < source.size(); i++){
      copy.push_back(source.at(i));
    }
  return copy;
}

//Builds a unit cube centered at the origin
vector<GLfloat> build_cube() {
    vector<GLfloat> result;
    vector<GLfloat> bottom = to_homogeneous_coord(init_plane());
    vector<GLfloat> top = to_homogeneous_coord(init_plane());
    vector<GLfloat> front = to_homogeneous_coord(init_plane());
    vector<GLfloat> back = to_homogeneous_coord(init_plane());
    vector<GLfloat> right = to_homogeneous_coord(init_plane());
    vector<GLfloat> left = to_homogeneous_coord(init_plane());

    back = mat_mult(translation_matrix(0.0,0.0,-0.5), back);
    front = mat_mult(translation_matrix(0.0,0.0,0.5), front);
    bottom = mat_mult(rotation_matrix_x(90), bottom);
    top = mat_mult(translation_matrix(0.0, 0.5, 0.0), bottom);
    bottom = mat_mult(translation_matrix(0.0, -0.5, 0.0), bottom);
    left = mat_mult(rotation_matrix_y(90), left);
    right = mat_mult(translation_matrix(0.5, 0.0, 0.0), left);
    left = mat_mult(translation_matrix(-0.5, 0.0, 0.0), left);
    result = push_back_helper(front, result);
    result = push_back_helper(back, result);
    result = push_back_helper(left, result);
    result = push_back_helper(right, result);
    result = push_back_helper(top, result);
    result = push_back_helper(bottom, result);
    return result;
}

// vector<GLfloat> build_cube() {
//     vector<GLfloat> result;
//     vector<GLfloat> a_0 = init_plane();
//     a_0 = to_homogeneous_coord(a_0);
//     vector<GLfloat> a_1 = mat_mult(translation_matrix(0.0, 0.0, 0.5), a_0);
//     push_back_helper(a_1, result);
//     vector<GLfloat> a_2 = mat_mult(translation_matrix(0.0, 0.0, -0.5), mat_mult(rotation_matrix_y(180), a_0));
//     push_back_helper(a_2, result);
//     vector<GLfloat> a_3 = mat_mult(translation_matrix(-0.5, 0.0, 0.0), mat_mult(rotation_matrix_x(90), a_0));
//     push_back_helper(a_3, result);
//     return result;
// }

/**************************************************
 *            Camera and World Modeling           *
 *                                                *
 *  create a scene by applying transformations to *
 *  the objects built from planes and position    *
 *  the camera to view the scene by setting       *
 *  the projection/viewing matrices               *
 *                                                *
 *************************************************/

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
    gluPerspective(50.0, 1.0, 2.0, 10.0);
    gluLookAt(2.0, 6.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

}

// Construct the scene using objects built from cubes/prisms
vector<GLfloat> init_scene() {
    vector<GLfloat> scene;
    scene = push_back_helper(build_cube(), scene);
    return scene;
}

// Construct the color mapping of the scene
vector<GLfloat> init_color(vector<GLfloat> scene) {
    vector<GLfloat> colors;
    for (int i = 0; i < scene.size(); i++) {
        colors.push_back(static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    }
    return colors;
}

void display_func() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // TODO: Rotate the scene using the scene vector
    vector<GLfloat> scene = to_cartesian_coord(SCENE);
    //scene = mat_mult(rotation_matrix_y(THETA), scene);

    GLfloat* scene_vertices = vector2array(scene);
    GLfloat* color_vertices = vector2array(COLOR);
    // Pass the scene vertex pointer
    glVertexPointer(3,                // 3 components (x, y, z)
                    GL_FLOAT,         // Vertex type is GL_FLOAT
                    0,                // Start position in referenced memory
                    scene_vertices);  // Pointer to memory location to read from    // Pass the color vertex pointer
    glColorPointer(3,                   // 3 components (r, g, b)
                   GL_FLOAT,            // Vertex type is GL_FLOAT
                   0,                   // Start position in referenced memory
                   color_vertices);     // Pointer to memory location to read from    // Draw quad point planes: each 4 vertices
    glDrawArrays(GL_QUADS, 0, scene.size()/3.0);
    glFlush();			//Finish rendering
    glutSwapBuffers();
}

void idle_func() {
    THETA = THETA + 0.03;
    display_func();
}

int main (int argc, char **argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1920, 1080);
    // Create a window with rendering context and everything else we need
    glutCreateWindow("Assignment 3");

    setup();
    init_camera();
    // Setting global variables SCENE and COLOR with actual values
    SCENE = init_scene();
    COLOR = init_color(SCENE);

    // Set up our display function
    glutDisplayFunc(display_func);
    glutIdleFunc(idle_func);
    // Render our world
    glutMainLoop();

    // Remember to call "delete" on your dynmically allocated arrays
    // such that you don't suffer from memory leaks. e.g.
    // delete arr;

    return 0;
}
