#include "helper.h"
#include<iostream>
#include<GL/glut.h>
#include<string>
#include<cmath>

void Helper::plotPoint(int x,int y){
    glVertex2i(x,y);
}

void Helper::clearScreen(){
    glClearColor(0,0,0,0); //sets the colour buffer to black
    glClear(GL_COLOR_BUFFER_BIT);//the background colour is now set to black (which was the default one too , so yeah..)

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,WINDOW_WIDTH,0,WINDOW_HEIGHT);

    glColor3f(1.0,1.0,1.0); 
    glPointSize(1);
}

void Helper::createWindow(int *argc, char **argv){
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB ); //we dont want indexed colours 
    glutInitWindowSize(WINDOW_WIDTH,WINDOW_HEIGHT);
    // glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH )- WINDOW_WIDTH)/ 2, (glutGet(GLUT_SCREEN_HEIGHT) - WINDOW_HEIGHT) / 2 );
    glutCreateWindow("Display Window");
}

/**
 * @brief Calculates the distance between two coordinates
 * 
 * @param x1 The X coordinate of 1st point
 * @param y1 The Y coordinate of 1st point
 * @param x2 The X coordinate of 2nd point
 * @param y2 The Y coordinate of 2nd point
 * @return double 
 * The distane in double between the given points
 */
double Helper::distanceBetweenPts(int x1, int y1, int x2, int y2){
    return sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}