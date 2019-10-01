#pragma once
#include<iostream>

class Material
{
public:
    Material();
    
    Material(const int& index, 
             const double& young); 

    ~Material();

    int getIndex();

    double getYoung();


    void setIndex(const int& index);

    void setYoung(const double& young);


private:
    int index_;

    double young_;
};

