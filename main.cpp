#include<stdio.h>
#include<iostream>
#include"line.h"
#include"circle.h"
#include "helper.h"
#include <GL/glut.h>

int pts=0;
double coordX[100];
double coordY[100];

int calcPts[2];//to store the intermediate result of beizerPt function (a single calculated point - a pair of values x and y)

int plotPts=0;
int plotPtsX[1500];
int plotPtsY[1500];

bool isComputing = false;

void beizerPt(int count, double *arrX, double *arrY, double param){
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
        beizerPt(count-1, aX, aY, param);
    }
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


void computeBeizerCurve(){
    isComputing =  true;
    
    std::cout<<"Started computing Beizer Curve"<<std::endl;
    plotPts = 0;
    for(double i=0; i<=1;i+=0.001){
        beizerPt(pts, coordX, coordY, i);
        if(plotPts==0 || plotPtsX[plotPts]!=*calcPts || plotPtsY[plotPts]!=*(calcPts+1)){
            plotPtsX[plotPts] = *calcPts;
            plotPtsY[plotPts++] = *(calcPts+1);
        }
    }
    std::cout<<"Finished computing Beizer Curve"<<std::endl;
    isComputing =  false; 
}

int main(int argc, char **argv){
    std::cout<<"Computer Graphics Assignment 3 "<<std::endl;
    std::cout<<"Team members - "<<std::endl;
    std::cout<<"Utkarsh Grover , 2017A7PS1428H "<<std::endl;
    std::cout<<"Prateek Agarwal , 2017A7PS0075H"<<std::endl;
    std::cout<<std::endl;

    coordX[0]= 400;
    coordY[0]= 400;
    coordX[1]= 600;
    coordY[1]= 400;
    coordX[2]= 600;
    coordY[2]= 600;
    pts=3;

    computeBeizerCurve();

    Helper::createWindow(&argc, argv);
    glutDisplayFunc(draw);
    glutMainLoop();
}

