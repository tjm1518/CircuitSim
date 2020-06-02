#ifndef component_hpp
#define component_hpp

#include <string>
#include <functional>
#include <vector>

using namespace std;

class Node{ //A node. As the nodes are numbered 0 or from N001 to N999 we can give them a unique integer ID directly from the CIR file
public:
	int ID; //Used as the key for the right and left component maps
	double voltage;
	Node(int id);
	Node();
};
Node::Node(int id){ //Constructor for a node where there is a nonzero voltage, ie not the reference node
	this->ID = id;
}
Node::Node(){ //Constructorfor either an empty node or the reference node
	this->ID=0;
}

class Component{
public:
    string cName; //The component name ie "Resistor", "Capacitor" etc.
    string uName; //The name of the component as in the CIR file ie "R1", "Vin" etc.
    int id; //The unique (between components of the same type) identifier for the component.
    Node* right; //The node to the "right" of this component. This is the cathode/positive pin of polar components.
    Node* left; //The node to the "left" of this component. This is the anode/negative pin of polar components.
};
class ValuedComponent:Component{ //A linear component such as a resistor, capacitor, inductor or non-dependant source
public:
	double val; //the value of the component in SI units. In sources this is the DC offset.
    ValuedComponent Resistor(string uName, int id, Node& right, Node& left, double val){
		ValuedComponent rtn;
	    rtn.cName = 'R';
	    rtn.uName = uName;
	    rtn.id = id;
	    rtn.right = &right;
	    rtn.left = &left;
	    rtn.val = val;
		return rtn;
	}
};
class Source:Component{ //Only voltage sources here, I heard that current kills
public:
	bool vORc;
	double DCOffset;
	function<double(double)> waveform; //use 'waveform(time);' to run function
	Source(function<double(double)> f, double offset, bool b);
	Source(double offset, bool b);
};
Source::Source(function<double(double)> f, double offset, bool b){
	this->DCOffset = offset;
	this->waveform = f;
	this->vORc = b;
}
Source::Source(double offset, bool b){
	this->DCOffset = offset;
	this->waveform = [offset](double d) {return offset;};
	this->vORc = b;
}

class Sim{ //Currently unused struct for toring the type of simulations. Potentially worth merging with SimParams. Structs DC and Tran inherit from this.
public:
    string simType;
	vector<Source> Sources;
	vector<ValuedComponent> Resistors;
	vector<Node> Nodes;
};
class DC : Sim{
    DC();
};
DC::DC(){
    this->simType = "DC";
}
class Tran:Sim{
    double timeStep;
    double start;
    double end;
    int steps;
    Tran(double start, double end, double timeStep);
    Tran(double start, double end, int steps);
};
Tran::Tran(double start, double end, double timeStep){
	this->simType = "Tran";
    this->start = start;
    this->end = end;
    this->timeStep = timeStep;
    this->steps = ((start-end)/timeStep);
}
Tran::Tran(double start, double end, int steps){
	this->simType = "Tran";
    this->start = start;
	this->end = end;
	this->steps = steps;
	this->timeStep = (end-start)/steps;
}

#endif
