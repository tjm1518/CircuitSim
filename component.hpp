#ifndef component_hpp
#define component_hpp

#include <string>
#include <functional>

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
class Resistor:Component{ //A linear component such as a resistor, capacitor, inductor or non-dependant source
public:
	double val; //the value of the component in SI units. In sources this is the DC offset.
    Resistor(string uName, int id, Node& right, Node& left, double val);
};
Resistor::Resistor(string uName, int id, Node& right, Node& left, double val){
    this->cName = "Resistor";
    this->uName = uName;
    this->id = id;
    this->right = &right;
    this->left = &left;
    this->val = val;
}
class Source:Component{ //Only voltage sources here, I heard that current kills
public:
	double DCOffset; //For use in case of DC bias point check only. 500€ fine for misuse.
	function<double(double)> waveform; //Functional programmers represent
	Source(function<double(double)> f, double offset);
	Source(double offset);
};
Source::Source(function<double(double)> f, double offset){
	this->DCOffset = offset;
	this->waveform = f;
}
Source::Source(double offset){
	this->DCOffset = offset;
	this->waveform = [](double d) {return offset;};
}

class Sim{ //Currently unused struct for toring the type of simulations. Potentially worth merging with SimParams. Structs DC and Tran inherit from this.
public:
    string simType;
	vector<Source> Sources;
	vector<Resistor> Resistors;
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
    this->start = start;
    this->end = end;
    this->timeStep = timeStep;
    this->steps = ((start-end)/timeStep);
}
Tran::Tran(double start, double end, int steps){
    this->start = start;
}


#endif
