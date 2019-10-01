#include "Node.h"

Node::Node(){}

Node::Node(const int& index,
           const std::vector<double>& initialCoordinate)
{
    index_=index;
    initialCoordinate_=initialCoordinate;
    currentCoordinate_=initialCoordinate;
}

Node::~Node(){}


int Node::getIndex()
{
    return index_;
}

std::vector<double> Node::getInitialCoordinate()
{
    return initialCoordinate_;
}

std::vector<double> Node::getCurrentCoordinate()
{
    return currentCoordinate_;
}

// std::vector<double> Node::InternalForce()
// {
//     return internalForce_;
// }

// std::vector<double> Node::ExternalForce()
// {
//     return externalForce_;
// }


void Node::setIndex(const int& index)
{
    index_=index;
}

void Node::setInitialCoordinate(const std::vector<double>& initialCoordinate)
{
    initialCoordinate_=initialCoordinate;
}

void Node::setCurrentCoordinate(const std::vector<double>& currentCoordinate)
{
    currentCoordinate_=currentCoordinate;
}