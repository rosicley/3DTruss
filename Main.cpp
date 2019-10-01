#include<iostream>
#include "Element.h"
#include "Material.h"
#include "Truss.h"

int main()
{

Truss* Ex = new Truss;

Ex->readInput("TorreEngastada.txt");

std::cout<<Ex->BoundaryCondition()<<std::endl;

std::cout<<Ex->ExternalForces()<<std::endl;

return 0;
}