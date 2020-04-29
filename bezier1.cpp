#include<stdio.h>
#include<iostream>
#include"line.h"
#include"circle.h"
#include"helper.h"
#include <GL/glut.h>
#include"bezier1.h"

int pts=0;// Count of points on the screen
double coordX[100]; // X coordinates of all points on the screen
double coordY[100]; // Y coordinates of all points on the screen

int calcPts[2];//to store the intermediate result of beizerPt function (a single calculated point - a pair of values x and y)

int plotPts=0;// Count of Bezier curve points
int plotPtsX[1500]; // X coordinates of bezier curve points
int plotPtsY[1500]; // Y coordinates of bezier curve points

bool isComputing = false;// Flag to determine if the bezier curve points are ready to be drawn or in progress of computation

bool isDragging = false; // Flag to determine if the user is currently dragging the pointer on the screen
int dragPoint=-1;// Stores the index of the point if it is being dragged by the user


/**
 * @brief De-Castlejau algorithm's recusrive implementation to determine a point on the Beizer curve according to the paramter values within [0,1]
 * 
 * @param count The number of points on the screen
 * @param arrX Pointer to an array of X coordinates of all the points
 * @param arrY Pointer to an array of Y coordinates of all the points
 * @param param Parameter t between [0,1]
 */
void Bezier::bezierPt(int count, double *arrX, double *arrY, double param){
    if(count==1){
        calcPts[0]=(int)arrX[0];
        calcPts[1]=(int)arrY[0];
    }
    else{
        double aX[count-1];
        double aY[count-1];
        for(int i=0;i<=count-2;i++){
            aX[i] = (1-param)*arrX[i]+param*arrX[i+1];
            aY[i] = (1-param)*arrY[i]+param*arrY[i+1];
        }
        bezierPt(count-1, aX, aY, param);
    }
}

/**
 * @brief De-Castlejau algorithm to determine ALL the points point on the Beizer curve for 1000 parametric points
 */
void Bezier::computeBezierCurve(){
    // std::cout<<"Started computing Beizer Curve"<<std::endl;
    plotPts = 0;
    if(pts<2)
        return;
    isComputing =  true;
    for(double i=0; i<=1;i+=0.001){
        bezierPt(pts, coordX, coordY, i);
        if(plotPts==0 || plotPtsX[plotPts]!=*calcPts || plotPtsY[plotPts]!=*(calcPts+1)){
            plotPtsX[plotPts] = *calcPts;
            plotPtsY[plotPts++] = *(calcPts+1);
        }
    }
    // std::cout<<"Finished computing Beizer Curve"<<std::endl;
    isComputing =  false; 
}

/**
 * @brief Adds a new point on the screen provided there are no other points in its vicinity
 * 
 * @param x X-coordinate on the screen where a new point has to be added
 * @param y Y-coordinate on the screen where a new point has to be added
 */
void Bezier::addNewPoint(int x, int y){
    for(int i=0;i<pts;i++){
        if(Helper::distanceBetweenPts(coordX[i],coordY[i],x,y)<15){
            printf("A point is already present in the vicinity\n");
            return;
        }
    }
    coordX[pts]= x;
    coordY[pts++]= y;
    computeBezierCurve();
    glutPostRedisplay();
}

/**
 * @brief Removes a point on the screen if there exists
 * 
 * @param x X-coordinate on the screen where the point has to be removed
 * @param y Y-coordinate on the screen where the point has to be removed
 */
void Bezier::removePoint(int x, int y){
    for(int i=0;i<pts;i++){
        if(Helper::distanceBetweenPts(coordX[i],coordY[i],x,y)<10){
            for(int j=i;j<pts-1;j++){
                coordX[j]=coordX[j+1];
                coordY[j]=coordY[j+1];
            }
            pts--;
            computeBezierCurve();
            glutPostRedisplay();
            return;
        }
    }
    printf("No point selected\n");
}

void mouseClickCallback(int button, int state, int x, int y){
    y = Helper::WINDOW_HEIGHT -y;
    if(button == GLUT_LEFT_BUTTON && state==GLUT_DOWN){
        for(int i=0;i<pts;i++){
            if(Helper::distanceBetweenPts(coordX[i],coordY[i],x,y)<10){
                dragPoint = i;
                return;
            }
        }
    } else if(button == GLUT_LEFT_BUTTON && state==GLUT_UP) {
        if(!isDragging){
            Bezier bezier;
            bezier.addNewPoint(x, y);
        }
        isDragging = false;
        dragPoint = -1;
    } else if(button == GLUT_RIGHT_BUTTON && state==GLUT_DOWN){
        Bezier bezier;
        bezier.removePoint(x,y);
    }
}

void mouseMoveCallback(int x, int y){
    y = Helper::WINDOW_HEIGHT -y;
    isDragging = true;
    if(dragPoint==-1)
        return;
    coordX[dragPoint]=x;
    coordY[dragPoint]=y;
    Bezier bezier;
    bezier.computeBezierCurve();
    glutPostRedisplay();
}

void draw(){
    Helper::clearScreen();
    Circle circle;
    Line line;
    for(int i=0;i<pts;i++){
        glColor3f(1.0,1.0,1.0); 
        circle.drawCircle(coordX[i],coordY[i], 10);
        glColor3f(1.0, 0.0, 0.0);
        line.drawLine(coordX[i]-15, coordY[i], coordX[i]+15, coordY[i]);
        line.drawLine(coordX[i], coordY[i]-15, coordX[i], coordY[i]+15);
    }

    if(!isComputing){
        glBegin(GL_POINTS);
        glColor3f(0.0, 1.0, 0.0);
        for(int i=0;i<plotPts;i++)
            Helper::plotPoint(plotPtsX[i], plotPtsY[i]);
        glEnd();
    }
    glFlush();
}


/**
 * @brief 
 * This function creates an interactive window using OpenGL and allows user to add, move, delete points and view the bezier curve through those points
 * @param argc Pointer to total number of arguments
 * @param argv The actual arguments to be passed to the function
 */
void Bezier::initBezierCurve(int *argc, char **argv){
    Helper::createWindow(argc, argv);
    glutDisplayFunc(draw);
    glutMouseFunc(mouseClickCallback);
    glutMotionFunc(mouseMoveCallback);
    glutMainLoop();
}
