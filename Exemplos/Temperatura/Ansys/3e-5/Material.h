#pragma once
#include <iostream>

class Material
{
public:
    Material();

    Material(const int &index,
             const double &young,
             const double &plastStrain,
             const double &hardeningModulus,
             const double &density,
             const double &expansionCoef);

    ~Material();

    int getIndex();

    double getYoung();

    double getDensity();

    double getPlastStrain();

    double getHardeningModulus();

    double getExpansionCoef();

    // void setIndex(const int& index);

    // void setYoung(const double& young);


private:
    int index_;

    double young_;

    double density_;

    double plastStrain_;

    double hardeningModulus_;

    double expansionCoef_;
};
