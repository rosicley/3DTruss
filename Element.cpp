#include "Element.h"

Element::Element() {}

Element::Element(const int &index,
                 const std::vector<Node *> &connection,
                 Material *material,
                 const double &area)
{
    index_ = index;
    connection_ = connection;
    material_ = material;
    area_ = area;
    deltaTemp_ = 0.0;
}

Element::~Element() {}

int Element::getIndex()
{
    return index_;
}

std::vector<Node *> Element::getConnection()
{
    return connection_;
}

Material *Element::getMaterial()
{
    return material_;
}

double Element::getArea()
{
    return area_;
}

double Element::getDeltaTemp()
{
    return deltaTemp_;
}

double Element::InitialLength()
{
    std::vector<double> initialNode = connection_[0]->getInitialCoordinate();
    std::vector<double> endNode = connection_[1]->getInitialCoordinate();
    double length = 0.0;

    for (int i = 0; i < 3; i++)
    {
        length += (endNode[i] - initialNode[i]) * (endNode[i] - initialNode[i]);
    }

    length = sqrt(length);
    return length;
}

double Element::CurrentLength()
{
    std::vector<double> initialNode = connection_[0]->getCurrentCoordinate();
    std::vector<double> endNode = connection_[1]->getCurrentCoordinate();
    double length = 0.0;

    for (int i = 0; i < 3; i++)
    {
        length += (endNode[i] - initialNode[i]) * (endNode[i] - initialNode[i]);
    }

    length = sqrt(length);
    return length;
}

double Element::PiolaStress()
{
    double green = 0.5 * ((CurrentLength() * CurrentLength() - InitialLength() * InitialLength()) / (InitialLength() * InitialLength()));
    double s;

    if (green <= getMaterial()->getPlastStrain())
    {
        s = green * (getMaterial()->getYoung());
    }

    if (green > getMaterial()->getPlastStrain())
    {
        s = getMaterial()->getPlastStrain() * (getMaterial()->getYoung()) + (green - getMaterial()->getPlastStrain()) * getMaterial()->getHardeningModulus();
    }

    return s;
}

std::vector<double> Element::InternalForce()
{
    std::vector<double> initialNode = connection_[0]->getCurrentCoordinate();
    std::vector<double> endNode = connection_[1]->getCurrentCoordinate();
    std::vector<double> forceConec_;

    for (int i = 0; i < 2; i++)
    {
        for (int ih = 0; ih < 3; ih++)
        {
            double force = getArea() * PiolaStress() * pow(-1, i + 1) * (endNode[ih] - initialNode[ih]) / InitialLength();
            forceConec_.push_back(force);
        }
    }
    return forceConec_;
}

std::vector<double> Element::TemperatureForce(const int &numberOfSteps, const int &currentStep)
{
    std::vector<double> initialNode = connection_[0]->getCurrentCoordinate();
    std::vector<double> endNode = connection_[1]->getCurrentCoordinate();
    std::vector<double> forceConec_;
    double young;
    double green = 0.5 * ((CurrentLength() * CurrentLength() - InitialLength() * InitialLength()) / (InitialLength() * InitialLength()));

    if (green <= getMaterial()->getPlastStrain())
    {
        young = getMaterial()->getYoung();
    }

    if (green > getMaterial()->getPlastStrain())
    {
        young = getMaterial()->getHardeningModulus();
    }

    double ftemp = getArea() * young * getMaterial()->getExpansionCoef() * (getDeltaTemp() * currentStep / numberOfSteps);

    for (int i = 0; i < 2; i++)
    {
        for (int ih = 0; ih < 3; ih++)
        {
            double force = ftemp * pow(-1, i + 1) * (endNode[ih] - initialNode[ih]) / InitialLength();
            forceConec_.push_back(force);
        }
    }

    return forceConec_;
}

bounded_matrix<double, 6, 6> Element::localHessian()
{
    double young;
    double green = 0.5 * ((CurrentLength() * CurrentLength() - InitialLength() * InitialLength()) / (InitialLength() * InitialLength()));

    if (green <= getMaterial()->getPlastStrain())
    {
        young = getMaterial()->getYoung();
    }

    if (green > getMaterial()->getPlastStrain())
    {
        young = getMaterial()->getHardeningModulus();
    }

    std::vector<double> initialNode = connection_[0]->getCurrentCoordinate();
    std::vector<double> endNode = connection_[1]->getCurrentCoordinate();
    bounded_matrix<double, 6, 6> hessian;

    for (int beta = 0; beta < 2; beta++)
    {
        for (int alfa = 0; alfa < 2; alfa++)
        {
            for (int i = 0; i < 3; i++)
            {
                for (int k = 0; k < 3; k++)
                {
                    int l = 3 * beta + i;
                    int m = 3 * alfa + k;
                    double de_dy = pow(-1, beta + 1) * pow(-1, alfa + 1) * getArea() * young * (endNode[i] - initialNode[i]) * (endNode[k] - initialNode[k]) / (pow(InitialLength(), 3));
                    double d2edy2 = 0.0;

                    if (i == k)
                    {
                        d2edy2 = pow(-1, beta + 1) * pow(-1, alfa + 1) * getArea() / InitialLength() * PiolaStress();
                    }
                    hessian(l, m) = de_dy + d2edy2;
                }
            }
        }
    }
    return hessian;
}

bounded_matrix<double, 6, 6> Element::localTemperatureHessian(const int &numberOfSteps, const int &currentStep)
{
    double young;
    double green = 0.5 * ((CurrentLength() * CurrentLength() - InitialLength() * InitialLength()) / (InitialLength() * InitialLength()));

    if (green <= getMaterial()->getPlastStrain())
    {
        young = getMaterial()->getYoung();
    }

    if (green > getMaterial()->getPlastStrain())
    {
        young = getMaterial()->getHardeningModulus();
    }

    double ftemp = young * (getMaterial()->getExpansionCoef()) * (getDeltaTemp() * currentStep / numberOfSteps);

    std::vector<double> initialNode = connection_[0]->getCurrentCoordinate();
    std::vector<double> endNode = connection_[1]->getCurrentCoordinate();
    bounded_matrix<double, 6, 6> hessian;

    for (int beta = 0; beta < 2; beta++)
    {
        for (int alfa = 0; alfa < 2; alfa++)
        {
            for (int i = 0; i < 3; i++)
            {
                for (int k = 0; k < 3; k++)
                {
                    int l = 3 * beta + i;
                    int m = 3 * alfa + k;
                    double d2edy2 = 0.0;

                    if (i == k)
                    {
                        d2edy2 = pow(-1, beta + 1) * pow(-1, alfa + 1) * getArea() / InitialLength() * ftemp;
                    }
                    hessian(l, m) = d2edy2;
                }
            }
        }
    }
    return hessian;
}

bounded_matrix<double, 6, 6> Element::localMassMatrix()
{
    identity_matrix<double> identity(6);
    bounded_matrix<double, 6, 6> mass;
    double partial = getArea() * (getMaterial()->getDensity()) * InitialLength() / 2;

    mass = partial * identity;

    return mass;
}

void Element::setIndex(const int &index)
{
    index_ = index;
}

void Element::setConnection(const std::vector<Node *> &connection)
{
    connection_ = connection;
}

void Element::setMaterial(Material *material)
{
    material_ = material;
}

void Element::setArea(const double &area)
{
    area_ = area;
}

void Element::setDeltaTemp(const double &deltaTemp)
{
    deltaTemp_ = deltaTemp;
}
