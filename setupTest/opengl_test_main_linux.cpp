#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
void display() {
  // Do nothing for now
}
int main(int argc, char *argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Test Window Linux");
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}
