#pragma once
#include <iostream>
#include <vector>

class Node
{
public:
	Node();

	Node(const int &index,
		 const std::vector<double> &initialCoordinate); //(nº do nó, {x1, x2,x3})

	~Node();

	int getIndex();

	std::vector<double> getInitialCoordinate();

	std::vector<double> getPastCoordinate();

	std::vector<double> getPastVelocity();

	std::vector<double> getPastAcceleration();

	std::vector<double> getCurrentCoordinate();

	std::vector<double> getCurrentVelocity();

	std::vector<double> getCurrentAcceleration();

	void setPastCoordinate(const std::vector<double> &pastCoordinate);

	void setPastVelocity(const std::vector<double> &pastVelocity);

	void setPastAcceleration(const std::vector<double> &pastAcceleration);

	void setCurrentCoordinate(const std::vector<double> &currentCoordinate);

	void setCurrentVelocity(const std::vector<double> &currentVelocity);

	void setCurrentAcceleration(const std::vector<double> &currentAcceleration);

private:
	int index_;

	std::vector<double> initialCoordinate_;

	std::vector<double> pastCoordinate_;

	std::vector<double> pastVelocity_;

	std::vector<double> pastAcceleration_;

	std::vector<double> currentCoordinate_;

	std::vector<double> currentVelocity_;

	std::vector<double> currentAcceleration_;
};