#include "MouseParser.h"

namespace sim {

MouseParser::MouseParser() {
    pugi::xml_parse_result result=doc.load_file("C:/Users/Mitchell/Documents/GitHub/Micromouse/two/winport/sim/dataFiles/myRobot.xml");
    if(!result) {
		std::cout << "Error" << std::endl;
		std::cout << "Description: " << result.description() << std::endl;
    }
}

std::vector<std::pair<float, float> > MouseParser::getShape() {
    pugi::xml_node shape=doc.child("rob").child("shape").child("point");
    std::vector<std::pair<float, float> > points;
    std::pair<float, float> point;
    
    //Iterate though all members of shape
    for(pugi::xml_node_iterator it=shape.begin(); it!=shape.end(); ++it) {
		if(*(it->name())=='x') {
            point.first=atof(it->child_value()); //Implicit conversion 
        }
        else if(*(it->name())=='y') {
            point.second=atof(it->child_value()); //Implicit conversion
            points.push_back(point); //Add point to end of vector
        }
    }

    return points;
}


float MouseParser::getHeight() {
    return atof(doc.child("rob").child("height").child_value()); //Implicit conversion
}
 
std::vector<std::vector<float> > MouseParser::getWheels() {
    pugi::xml_node wheels=doc.child("rob").child("wheels").child("pointanddirection");
    std::vector<std::vector<float> > wheelSpecs;
    std::vector<float> specs;

    for(pugi::xml_node_iterator it=wheels.begin(); it!=wheels.end(); ++it) {
        if(*(it->name())=='x') {
            specs.push_back(atof(it->child_value())); //Implicit conversion
        }
        else if(*(it->name())=='y') {
            specs.push_back(atof(it->child_value())); //Implicit conversion
        }
        else if(*(it->name())=='d') {
            specs.push_back(atof(it->child_value())); //Implicit conversion
            wheelSpecs.push_back(specs);
            specs.clear();
        }    
    }
    
    return wheelSpecs;   
}

float MouseParser::getTorque() {
    return atof(doc.child("rob").child("wheels").child("torque").child_value());
}

float MouseParser::getDiameter() {
    return atof(doc.child("rob").child("wheels").child("diameter").child_value());
}

std::vector<std::vector<float> > MouseParser::getSensors() {
    pugi::xml_node sensors=doc.child("rob").child("sensors").child("pointanddirection");
    std::vector<std::vector<float> > sensorSpecs;
    std::vector<float> specs;

    for(pugi::xml_node_iterator it=sensors.begin(); it!=sensors.end(); ++it) {
        if(*(it->name())=='x') {
            specs.push_back(atof(it->child_value())); //Implicit conversion
        }
        else if(*(it->name())=='y') {
            specs.push_back(atof(it->child_value())); //Implicit conversion
        }
        else if(*(it->name())=='d') {
            specs.push_back(atof(it->child_value())); //Implicit conversion
            sensorSpecs.push_back(specs);
            specs.clear();
        }
    }

    return sensorSpecs;

}

}
