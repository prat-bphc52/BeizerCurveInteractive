#ifndef BEZIER1_H
#define BEZIER1_H

class Bezier{
    private:
    void bezierPt(int, double*, double*, double);

    public:
    static void initBezierCurve(int* , char**);
    void computeBezierCurve();
    void addNewPoint(int, int);
    void removePoint(int, int);
};

#endif