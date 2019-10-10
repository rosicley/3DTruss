#include "Node.h"

Node::Node() {}

Node::Node(const int &index,
           const std::vector<double> &initialCoordinate)
{
    index_ = index;
    initialCoordinate_ = initialCoordinate;
    pastCoordinate_ = initialCoordinate;
    currentCoordinate_ = initialCoordinate;
    pastVelocity_.reserve(3);
    pastAcceleration_.reserve(3);
    currentVelocity_.reserve(3);
    currentAcceleration_.reserve(3);
    pastVelocity_ = {0.0, 0.0, 0.0};
    pastAcceleration_ = {0.0, 0.0, 0.0};
    currentVelocity_ = {0.0, 0.0, 0.0};
    currentAcceleration_ = {0.0, 0.0, 0.0};
}

Node::~Node() {}

int Node::getIndex()
{
    return index_;
}

std::vector<double> Node::getInitialCoordinate()
{
    return initialCoordinate_;
}

std::vector<double> Node::getPastCoordinate()
{
    return pastCoordinate_;
}

std::vector<double> Node::getPastVelocity()
{
    return pastVelocity_;
}

std::vector<double> Node::getPastAcceleration()
{
    return pastAcceleration_;
}

std::vector<double> Node::getCurrentCoordinate()
{
    return currentCoordinate_;
}

std::vector<double> Node::getCurrentVelocity()
{
    return currentVelocity_;
}

std::vector<double> Node::getCurrentAcceleration()
{
    return currentAcceleration_;
}

void Node::setPastCoordinate(const std::vector<double> &pastCoordinate)
{
    pastCoordinate_ = pastCoordinate;
}

void Node::setPastVelocity(const std::vector<double> &pastVelocity)
{
    pastVelocity_ = pastVelocity;
}

void Node::setPastAcceleration(const std::vector<double> &pastAcceleration)
{
    pastAcceleration_ = pastAcceleration;
}

void Node::setCurrentCoordinate(const std::vector<double> &currentCoordinate)
{
    currentCoordinate_ = currentCoordinate;
}

void Node::setCurrentVelocity(const std::vector<double> &currentVelocity)
{
    currentVelocity_ = currentVelocity;
}

void Node::setCurrentAcceleration(const std::vector<double> &currentAcceleration)
{
    currentAcceleration_ = currentAcceleration;
}