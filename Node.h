#pragma once
#include<iostream>
#include<vector>

class Node
{
public:
	Node();
	
	Node(const int& index,
		 const std::vector<double>& initialCoordinate);  //(nº do nó, {x1, x2,x3})
    
	~Node();
   

    int getIndex();
	
	std::vector<double> getInitialCoordinate();
	
	std::vector<double> getCurrentCoordinate(); 

	// std::vector<double> InternalForce(); //VERIFICAR SE VOU USAR

	// std::vector<double> ExternalForce();


    void setIndex(const int& index); 
	
	void setInitialCoordinate(const std::vector<double>& initialCoordinate);
	
	void setCurrentCoordinate(const std::vector<double>& currentCoordinate);
    
	
private:
	int index_;

	std::vector<double> initialCoordinate_; 

    std::vector<double> currentCoordinate_; 
	
	std::vector<double> internalForce_; //3xnº de nós

	std::vector<double> externalForce_;
};