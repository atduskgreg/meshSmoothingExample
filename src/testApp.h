#pragma once

#include "ofMain.h"

typedef map<ofIndexType, vector<ofIndexType> > VertexGraph;

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    ofMesh mesh;
    ofEasyCam cam;
    ofLight light;
    
    ofMesh performSmooth(ofMesh &m, float relaxationFactor);
    

    VertexGraph connectivityGraph;
    VertexGraph buildGraph(ofMesh &m);
    
    void smoothMesh(ofMesh & m, int iterations=10);

};
