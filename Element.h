#pragma once
#include<iostream>
#include<vector>
#include "Node.h"
#include "Material.h"
#include<math.h>
#include <boost/numeric/ublas/matrix.hpp>


using namespace boost::numeric::ublas;


class Element
{
public:
    Element();

    Element(const int& index, 
            const std::vector<Node*>& connection, 
            Material* material, 
            const double& area); 
    
    ~Element();
    
    
    int getIndex();

    std::vector<Node*> getConnection();

    Material* getMaterial();

    double getArea();

    double InitialLength();

    double CurrentLength();

    double PiolaStress();

    std::vector<double> ForceConec();

    bounded_matrix<double, 6, 6> localHessian();


    void setIndex(const int& index);

    void setConnection(const std::vector<Node*>& connection);

    void setMaterial(Material* material);

    void setArea(const double& area);
   

private:
    int index_;

    std::vector<Node*> connection_;

    Material* material_;
    
    double area_;  
};