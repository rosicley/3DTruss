#include "Material.h"

Material::Material(){}

Material::Material(const int& index, 
                   const double& young)                   
{
    index_=index;
    young_=young;
    
}

Material::~Material(){}


int Material::getIndex()
{
    return index_;
}

double Material::getYoung()
{
    return young_;
}


void Material::setIndex(const int& index)
{
    index_=index;
}

void Material::setYoung(const double& young)
{
    young_=young;
}

