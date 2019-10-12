#include "Material.h"

Material::Material() {}

Material::Material(const int &index,
                   const double &young,
                   const double &plastStrain,
                   const double &hardeningModulus,
                   const double &density)
{
    index_ = index;
    young_ = young;
    plastStrain_=plastStrain;
    hardeningModulus_=hardeningModulus;
    density_ = density;
}

Material::~Material() {}

int Material::getIndex()
{
    return index_;
}

double Material::getYoung()
{
    return young_;
}

double Material::getDensity()
{
    return density_;
}

double Material::getPlastStrain()
{
    return plastStrain_;
}

double Material::getHardeningModulus()
{
    return hardeningModulus_;
}

// void Material::setIndex(const int& index)
// {
//     index_=index;
// }

// void Material::setYoung(const double& young)
// {
//     young_=young;
// }
