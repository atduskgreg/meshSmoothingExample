#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    // this should be a ply with each vertex only once
    // and indices listed for all triangles
    mesh.load(ofToDataPath("my_mesh.ply"));
 
    glEnable(GL_DEPTH_TEST);  
    light.enable();  
    
    cout << "num verts: " << mesh.getVertices().size() << endl;
    cout << "num indices: " << mesh.getIndices().size() << endl;
    
    cout << "building connectivity graph" << endl;
    connectivityGraph = buildGraph(mesh);
    cout << "connectivity graph complete" << endl;
    cout << "graph size: " << connectivityGraph.size() << endl;
    cout << "first node has " << connectivityGraph[0].size() << " connections" << endl;

    int i = 10;
    cout << "performing smooth with " << i << "iterations" << endl;
    smoothMesh(mesh, i);
    cout << "smooth complete" << endl;
                    
}

//--------------------------------------------------------------
void testApp::update(){

}

// build a DAG representing the connectivity
// between vertices in a mesh
// assumes vertices are inserted as triangles
// i.e. each triplet of vertices is treated as the corners
// of a single triangle
VertexGraph testApp::buildGraph(ofMesh &m){
    typedef VertexGraph::iterator I;

    VertexGraph result;
    
    // iterate through the indices in sets of three
    // adding the neighbor relationships for the 
    // indices in each triangle
    for(int tri = 0; tri < m.getIndices().size(); tri+=3){
        ofIndexType v1 = m.getIndices()[tri];
        vector<ofIndexType> n1;
        // 0->1
        // 0->2
        n1.push_back(m.getIndices()[tri+1]);
        n1.push_back(m.getIndices()[tri+2]);
        
        std::pair<I,bool> const& r=result.insert(VertexGraph::value_type(v1, n1));
        
        // we already have neighbors for this index
        // so append these new ones
        if(!r.second){
            vector<ofIndexType> toInsert = r.first->second;
            toInsert.push_back(n1.at(0));
            toInsert.push_back(n1.at(1));
            result.insert(VertexGraph::value_type(v1, toInsert));
        } 
        
        // 1->0
        // 1->2
        ofIndexType v2= m.getIndices()[tri+1];
        vector<ofIndexType> n2;
        n2.push_back(m.getIndices()[tri+0]);
        n2.push_back(m.getIndices()[tri+2]);
        
        std::pair<I,bool> const& r2=result.insert(VertexGraph::value_type(v2, n2));
       
        if(!r2.second){
            vector<ofIndexType> toInsert = r.first->second;
            toInsert.push_back(n2.at(0));
            toInsert.push_back(n2.at(1));
            result.insert(VertexGraph::value_type(v2, toInsert));
        } 
        
        // 2->0
        // 2->1
        ofIndexType v3= m.getIndices()[tri+2];
        vector<ofIndexType> n3;
        n3.push_back(m.getIndices()[tri+0]);
        n3.push_back(m.getIndices()[tri+1]);
        
        std::pair<I,bool> const& r3=result.insert(VertexGraph::value_type(v3, n3));
        
        // we already have neighbors for this index
        if(!r3.second){
            vector<ofIndexType> toInsert = r.first->second;
            toInsert.push_back(n3.at(0));
            toInsert.push_back(n3.at(1));
            result.insert(VertexGraph::value_type(v3, toInsert));
        } 
        
        
    }
    
    return result;
}

// laplacian smooth for vertices of an ofMesh with indices
// algorithm adapted from here: http://accu.org/index.php/journals/1526

ofMesh testApp::performSmooth(ofMesh &m, float relaxationFactor){
    ofMesh result;
    for(int i = 0; i < m.getVertices().size(); i++){
        
        ofVec3f vert = m.getVertices().at(i);

        
        // get adjacent vertices
        vector<ofIndexType> neighbors = connectivityGraph[i];
        for(int n = 0; n < neighbors.size(); n++ ){
            ofIndexType neighborIndex = neighbors.at(n);
            ofVec3f offset = m.getVertices().at(neighborIndex) -  m.getVertices().at(i);
            offset *= relaxationFactor / neighbors.size();
            vert += offset;
        }
        
        result.addVertex(vert);
        
        /*
         for(
         std::vector<int>::const_iterator
         jt=neighbours.begin(),
         jend=neighbours.end(); jt!=jend; ++jt)
         {
         
         
           Vector3d offset =
           nodes[*jt].position -
           nodes[i].position;
           offset *= m_lambda / neighbours.size();
           newPositions[i] += offset;
         }
         */
    }
    return result;
}


void testApp::smoothMesh(ofMesh & m, int iterations){
    vector<ofIndexType> originalIndices = m.getIndices();
    
    for(int i = 0; i < iterations; i++){
        cout << "performing iteration " << i << endl;
        m = performSmooth(m, 0.5);
    }
    
    m.addIndices(originalIndices);
}


//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    //ofEnableLighting();
    
    ofSetColor(255);
    cam.begin();
    mesh.drawWireframe();
    cam.end();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key == ' '){
        cout << "saving file" << endl;
         mesh.save(ofToDataPath("smoothed_gnome.ply"));
        cout << "file saved" << endl;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}