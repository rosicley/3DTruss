#include<iostream>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/bindings/lapack/computational/getrf.hpp>
#include <boost/numeric/bindings/lapack/computational/getri.hpp>
#include <boost/numeric/bindings/ublas/matrix.hpp>
#include <boost/numeric/bindings/ublas/vector.hpp>

using namespace boost::numeric::ublas;

int main()
{

matrix<double, column_major> A (4,4);
vector<int> ipiv(4);

A(0,0)=1.0;
A(0,1)=2.0;
A(0,2)=5.0;
A(0,3)=0.0;
A(1,0)=3.0;
A(1,1)=7.0;
A(1,2)=1.0;
A(1,3)=5.0;
A(2,0)=3.0;
A(2,1)=8.0;
A(2,2)=10.0;
A(3,3)=9.0;
A(3,0)=3.0;
A(3,1)=18.0;
A(3,2)=0.0;
A(3,3)=9.0;

for(int i=0; i<4; i++)
{
    for(int j=0; j<4; j++)
    {
        std::cout<<A(i,j)<< " ";
    }

    std::cout<<std::endl;
}
std::cout<<std::endl;

boost::numeric::bindings::lapack::getrf(A,ipiv);

for(int i=0; i<4; i++)
{
    for(int j=0; j<4; j++)
    {
        std::cout<<A(i,j)<< " ";
    }

    std::cout<<std::endl;
}
std::cout<<std::endl;
boost::numeric::bindings::lapack::getri(A,ipiv);

for(int i=0; i<4; i++)
{
    for(int j=0; j<4; j++)
    {
        std::cout<<A(i,j)<< " ";
    }

    std::cout<<std::endl;
}

return 0;
}