#include<iostream>
#include "Element.h"
#include "Material.h"
#include "Truss.h"

int main()
{

Truss* Ex = new Truss;

Ex->readInput("Exemplo2.txt","Static"); //Static or Dynamic

return 0;
}