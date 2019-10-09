#pragma once
#include <iostream>

class Material
{
public:
    Material();

    Material(const int &index,
             const double &young,
             const double &density);

    ~Material();

    int getIndex();

    double getYoung();

    double getDensity();


    // void setIndex(const int& index);

    // void setYoung(const double& young);


private:
    int index_;

    double young_;

    double density_;
};
