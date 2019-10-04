#include "Truss.h"


std::vector<Node*> Truss::getNodes()
{
    return nodes_;
}

vector<int> Truss::BoundaryCondition()
{
    vector<double> boundary (3*nodes_.size(),0.0);


    for(int i=0; i<3*nodes_.size(); i++)
    {
        boundary[i] = boundaryConditions_[i];
    }

    return boundary;
}


std::vector<Element*> Truss::getElements()
{
    return elements_;
}

std::vector<Material*> Truss::getMaterial()
{
    return materials_;
}

//void addDisplacementBoundaryCondition();

//void addLoadBoundaryCondition();

void Truss::addMaterial(const int& index,
                        const double& young)
{
    Material* mat = new Material(index, young);
    materials_.push_back(mat);
}

void Truss::addNode(const int& index, const std::vector<double>& initialCoordinate)
{
    Node* n = new Node(index, initialCoordinate);
    nodes_.push_back(n);
}

void Truss::addElement(const int& index,
                const std::vector<int>& connection,
                const int& material,
                const double& area)
{
    std::vector<Node*> nodes;
    nodes.push_back(nodes_[connection[0]]);
    nodes.push_back(nodes_[connection[1]]);
    Element* el = new Element (index, nodes, materials_[material], area);
    elements_.push_back(el);
}


vector<double> Truss::InternalForces()
{
    int cont = elements_.size();
    vector<double> internalForces_ (3*nodes_.size(), 0.0);

    for(int i=0; i<cont; i++)
    {
        std::vector<double> force = elements_[i]->ForceConec();

        int indexInitialNode = elements_[i]->getConnection()[0]->getIndex();
        int indexEndNode = elements_[i]->getConnection()[1]->getIndex();

        for(int id=0; id<3; id++)
        {
            internalForces_[3*indexInitialNode+id] += force[id];
        }

        for(int id=0; id<3; id++)
        {
            internalForces_[3*indexEndNode+id] += force[3+id];
        }
    }
    return internalForces_;
}

vector<double> Truss::ExternalForces()
{
    vector<double> externalForces (3*nodes_.size(),0.0);


    for(int i=0; i<3*nodes_.size(); i++)
    {
        externalForces[i] = externalForces_[i];
    }

    return externalForces;
}

matrix<double> Truss::Hessian()
{
    matrix<double> hessian (3*nodes_.size(), 3*nodes_.size(), 0.0);
    
    for(int i=0; i<elements_.size(); i++)
    {
        bounded_matrix<double, 6, 6> localHessian = elements_[i]->localHessian();
        int indexInitialNode = elements_[i]->getConnection()[0]->getIndex();
        int indexEndNode = elements_[i]->getConnection()[1]->getIndex();
        for(int ij=0; ij<3; ij++)
        {
            for(int ik=0; ik<3; ik++)
            {
                hessian(indexInitialNode*3+ij, indexInitialNode*3+ik) += localHessian(ij, ik);

                hessian(indexInitialNode*3+ij, indexEndNode*3+ik) += localHessian(ij, ik+3);

                hessian(indexEndNode*3+ik, indexInitialNode*3+ij) += localHessian(ij+3, ik);

                hessian(indexEndNode*3+ij, indexEndNode*3+ik) += localHessian(ij+3, ik+3);
            }
        }
    }

    for(int i=0; i<3*nodes_.size(); i++)
    {
        if(boundaryConditions_[i]==1) //quando =1 é porque o deslocamento naquela direção está sendo
        {
            for(int k=0; k<3*nodes_.size(); k++)
            {
                hessian(i, k) = 0.0;
                hessian(k, i) = 0.0;
            }
            hessian(i, i) = 1.0;
        }
    }

    return hessian;
}

int Truss::solveProblem()
{
    double normInitialCoordinate=0.0;

    // std::stringstream text;
	// text << name_ << "-forçaXdeslocamento.txt";
	// std::ofstream file(text.str());
    
    for(int i=0; i<nodes_.size(); i++)
    {
        std::vector<double> initialCoordinate = nodes_[i]->getInitialCoordinate();
        normInitialCoordinate += initialCoordinate[0]*initialCoordinate[0] + initialCoordinate[1]*initialCoordinate[1] + initialCoordinate[2]*initialCoordinate[2];
    }

    for(int loadStep=0; loadStep<=numberOfSteps_; loadStep++)
    {

        std::cout << "------------------------- LOAD STEP = "
                  << loadStep << " -------------------------\n";

        vector<double> dexternalForces = (loadStep)*ExternalForces()/numberOfSteps_;

        for(int interation = 0; interation < 10; interation++) //definir o máximo de interações por passo de carga
        {
            vector<int> c(3*nodes_.size(),0.0);
            vector<double> g (3*nodes_.size(),0.0), deltaY (3*nodes_.size(),0.0);
            g = InternalForces() - dexternalForces;
            
            for(int i=0; i<3*nodes_.size(); i++)
            {
                if(boundaryConditions_[i]==1) //quando =1 é porque o deslocamento naquela direção está sendo
                {
                    g[i]=0.0;
                }
            }
                    
            matrix<double, column_major> hessian = Hessian();

            deltaY = -g;

            boost::numeric::bindings::lapack::gesv(hessian, c, deltaY);

            double normDeltaY=0.0;

            for(int ih=0; ih<nodes_.size(); ih++) //loop para atualizar as coordenadas dos nós
            {
            int index = nodes_[ih]->getIndex();
            std::vector<double> currentCoordinate = nodes_[ih]->getCurrentCoordinate();

            normDeltaY += deltaY[3*index]*deltaY[3*index] + deltaY[3*index+1]*deltaY[3*index+1] + deltaY[3*index+2]*deltaY[3*index+2];

            currentCoordinate[0] += deltaY[3*index];
            currentCoordinate[1] += deltaY[3*index+1];
            currentCoordinate[2] += deltaY[3*index+2];

            nodes_[ih]->setCurrentCoordinate(currentCoordinate);
            }

            std::cout << "Iteration = " << interation 
                      << "   x Norm = " << std::scientific << sqrt(normDeltaY/normInitialCoordinate) 
                      << std::endl;

            if(sqrt(normDeltaY/normInitialCoordinate) <= tolerance_)
                break;

        }
        
    exportToParaview(loadStep);

    //file << nodes_[6]->getCurrentCoordinate()[1]-nodes_[6]->getInitialCoordinate()[1] << " " << dexternalForces[18] << std::endl;

    }
}


void Truss::exportToParaview(const int& loadstep)
{
	std::stringstream text;
	text << name_ << loadstep << ".vtu";
	std::ofstream file(text.str());

	//header
	file << "<?xml version=\"1.0\"?>" << "\n"
         << "<VTKFile type=\"UnstructuredGrid\">" << "\n"
		 << "  <UnstructuredGrid>" << "\n"
         << "  <Piece NumberOfPoints=\"" << nodes_.size()
         << "\"  NumberOfCells=\"" << elements_.size()
         << "\">" << "\n";
	//nodal coordinates
	file << "    <Points>" << "\n"
         << "      <DataArray type=\"Float64\" "
         << "NumberOfComponents=\"3\" format=\"ascii\">" << "\n";

    for (Node* n : nodes_)
	{
		file << n->getCurrentCoordinate()[0] << " " << n->getCurrentCoordinate()[1] << " " << n->getCurrentCoordinate()[2] << "\n";
	}


    file << "      </DataArray>" << "\n"
         << "    </Points>" << "\n";
	//element connectivity
	file << "    <Cells>" << "\n"
         << "      <DataArray type=\"Int32\" "
         << "Name=\"connectivity\" format=\"ascii\">" << "\n";

    for (Element* el : elements_)
	{

        file << el->getConnection()[0]->getIndex() << " " << el->getConnection()[1]->getIndex() << "\n";

	}


	file << "      </DataArray>" << "\n";
	//offsets
	file << "      <DataArray type=\"Int32\""
		 << " Name=\"offsets\" format=\"ascii\">" << "\n";
	int aux = 0;

    for (Element* el : elements_)
	{
		int n = el->getConnection().size();
		aux += n;
		file << aux << "\n";
	}


	file << "      </DataArray>" << "\n";
	//elements type
	file << "      <DataArray type=\"UInt8\" Name=\"types\" "
		 << "format=\"ascii\">" << "\n";

    for (Element* el : elements_)
	{
		file << 4 << "\n";
	}
	

    file << "      </DataArray>" << "\n"
		 << "    </Cells>" << "\n";
	//nodal results
	file << "    <PointData>" <<"\n";
	file << "      <DataArray type=\"Float64\" NumberOfComponents=\"3\" "
		<< "Name=\"Displacement\" format=\"ascii\">" << "\n";

	for (Node* n: nodes_)
	{
		file << n->getCurrentCoordinate()[0] - n->getInitialCoordinate()[0] << " "
             << n->getCurrentCoordinate()[1] - n->getInitialCoordinate()[1] << " "
			 << n->getCurrentCoordinate()[2] - n->getInitialCoordinate()[2] << "\n";
	}
	
    // file << "      </DataArray> " << "\n";
	// file << "      <DataArray type=\"Float64\" NumberOfComponents=\"2\" "
	// 	<< "Name=\"Velocity\" format=\"ascii\">" << "\n";

	// for (Node* n: nodes_)
	// {
	// 	file << n->getCurrentVelocity()(0) << " " << n->getCurrentVelocity()(1) << "\n";
	// }
	// file << "      </DataArray> " << "\n";
	// file << "      <DataArray type=\"Float64\" NumberOfComponents=\"2\" "
	// 	<< "Name=\"Acceleration\" format=\"ascii\">" << "\n";

	// for (Node* n: nodes_)
	// {
	// 	file << n->getCurrentAcceleration()(0) << " " << n->getCurrentAcceleration()(1) << "\n";
	// }
	file << "      </DataArray> " << "\n";
	file << "    </PointData>" << "\n";
	//elemental results
	file << "    <CellData>" << "\n";

	file << "    </CellData>" << "\n";
	//footnote
	file << "  </Piece>" << "\n"
		<< "  </UnstructuredGrid>" << "\n"
		<< "</VTKFile>" << "\n";
}

void Truss::readInput(const std::string& read)
{
    std::ifstream file(read);
    std::string line;

    file >> name_;

    std::getline(file, line); std::getline(file, line);

    int nmaterial, nnode, nelement, nforce, nboundary;

    file >> nmaterial >> nnode >> nelement >> nforce >> nboundary >> numberOfSteps_ >> tolerance_;

    std::getline(file, line); std::getline(file, line); std::getline(file, line); std::getline(file, line);

    for(int i=0; i<nmaterial; i++)
    {
        int index;
        double young;

        file >> index >> young;

        addMaterial(index, young);

        std::getline(file, line);
    }

    std::getline(file, line); std::getline(file, line); std::getline(file, line);

    for(int i=0; i<nnode; i++)
    {
        int index;
        double x1, x2, x3;

        file >> index >> x1 >> x2 >> x3;

        addNode(index, {x1, x2, x3});

        std::getline(file, line);
    }

    std::getline(file, line); std::getline(file, line); std::getline(file, line);

    for(int i=0; i<nelement; i++)
    {
        int index, initialNode, endNode, material;
        double area;

        file >> index >> initialNode >> endNode >> material >> area;

        addElement(index, {initialNode, endNode}, material, area);

        std::getline(file, line);
    }

    std::getline(file, line); std::getline(file, line); std::getline(file, line);

    int cont = nodes_.size();
    boundaryConditions_.reserve(3*cont);
    externalForces_.reserve(3*cont);

    for(int i=0; i<(3*cont); i++)
    {
        externalForces_[i]=0.0;
        boundaryConditions_[i]=0;
    }

    for(int i=0; i<nforce; i++)
    {
        int index;
        double x1, x2, x3;

        file >> index >> x1 >> x2 >> x3;

        externalForces_[3*index]=x1;
        externalForces_[3*index+1]=x2;
        externalForces_[3*index+2]=x3;

        std::getline(file, line);
    }

    std::getline(file, line); std::getline(file, line); std::getline(file, line);

    for(int i=0; i<nboundary; i++)
    {
        int index;
        double x1, x2, x3;

        file >> index >> x1 >> x2 >> x3;

        boundaryConditions_[3*index]=x1;
        boundaryConditions_[3*index+1]=x2;
        boundaryConditions_[3*index+2]=x3;

        std::getline(file, line);
    }

    solveProblem();
}

