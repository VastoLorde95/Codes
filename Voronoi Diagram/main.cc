#include<bits/stdc++.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "lib/Voronoi.h"
#include "lib/VPoint.h"

#define foreach(it, v) for(auto it=(v).begin(); it != (v).end(); ++it)

void display (void);
void onEF (int n);
void reshape (int width, int height);

Voronoi * v;        // the container for the Voronoi diagram
Vertices * ver;     // Voronoi vertices
Edges * edg;        // edges of the Voronoi diagram

const double w = 100000;    // the bounding box has dimensions w x w

int main (int argc, char **argv){
    v = new Voronoi();
    ver = new Vertices();

    srand(time(NULL));
    
    // 4 co-circular points gives an error
//    ver->push_back(new VPoint(2500, 2500)); 
//    ver->push_back(new VPoint(2500, 7500)); 
//    ver->push_back(new VPoint(7500, 2500)); 
//    ver->push_back(new VPoint(7500, 7500)); 

    // generate N random Voronoi sites
    // search space is large enough such that points will be in general position with high probability.
    const int N = 20;
    for(int i = 0; i < N; i++)
        ver->push_back(new VPoint( w * (double)rand()/(double)RAND_MAX , w * (double)rand()/(double)RAND_MAX )); 

    edg = v->GetEdges(ver, w, w);

    // assert that all edges were constructed properly, i.e., have both start and end points defined
    foreach(it, *edg) assert((*it)->start and (*it)->end);

    // end of main function (for all practical purposes)
    
    








    
    
    
    
    
    
    
    
    // code below plots the Voronoi diagram
    // plot the Voronoi diagram using GLUT
    
    glutInit(&argc, argv);                     // Initialize GLUT
    glutInitDisplayMode (GLUT_SINGLE);         // Set up a basic display buffer (only single buffered for now)
    glutInitWindowSize (600, 600);             // Set the width and height of the window
    glutInitWindowPosition (100, 100);         // Set the position of the window
    glutCreateWindow ("Voronoi Diagram");      // Set the title for the window
    
    glutTimerFunc(100, onEF, 0);
    glutDisplayFunc(display);                  // Tell GLUT to use the method "display" for rendering

    glutReshapeFunc(reshape);                  // Tell GLUT to use the method "reshape" for reshaping

    glutMainLoop();                            // Enter GLUT's main loop
    
    return 0;
}

void drawVoronoi(){
    edg = v->GetEdges(ver, w, w);
    foreach(it, *ver){
        glBegin(GL_QUADS);
        glVertex2f(-1 + 2 * (*it)->x / w - 0.01,  -1 + 2 * (*it)->y / w - 0.01);
        glVertex2f(-1 + 2 * (*it)->x / w + 0.01,  -1 + 2 * (*it)->y / w - 0.01);
        glVertex2f(-1 + 2 * (*it)->x / w + 0.01,  -1 + 2 * (*it)->y / w + 0.01);
        glVertex2f(-1 + 2 * (*it)->x / w - 0.01,  -1 + 2 * (*it)->y / w + 0.01);
        glEnd();
    }

    foreach(it, *edg){
        glBegin(GL_LINES);
        glVertex2f(-1 + 2 * (*it)->start->x / w,  -1 + 2 * (*it)->start->y / w);
        glVertex2f(-1 + 2 * (*it)->end->x / w, -1 + 2 * (*it)->end->y / w);
        glEnd();
    }
}

void display(){
    glLoadIdentity();
    glTranslatef(0, 0, -5); 
    glFlush();
}

void onEF(int n){
    glutTimerFunc(20, onEF, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
    drawVoronoi();
    glutSwapBuffers();
}

void reshape(int width, int height){
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(22.5, (GLfloat) width / (GLfloat) height, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}
