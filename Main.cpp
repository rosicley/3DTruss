#include<iostream>
#include "Element.h"
#include "Material.h"
#include "Truss.h"

int main()
{

Truss* Ex = new Truss;

Ex->readInput("TorreEngastada.txt");

return 0;
}