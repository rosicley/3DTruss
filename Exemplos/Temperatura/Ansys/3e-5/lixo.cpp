
    // matrix<double, column_major> J(3 * nodes_.size(), 3 * nodes_.size());
    // vector<int> ipiv(3 * nodes_.size());
    // J = MassMatrix();
    // boost::numeric::bindings::lapack::getrf(J, ipiv);
    // boost::numeric::bindings::lapack::getri(J, ipiv, boost::numeric::bindings::lapack::optimal_workspace());
    // J = prod(J, Hessian());
    // for (int i = 0; i < 3 * nodes_.size(); i++)
    // {
    //     if (boundaryConditions_[i] == 1) //quando =1 é porque o deslocamento naquela direção está sendo
    //     {
    //         for (int k = 0; k < 3 * nodes_.size(); k++)
    //         {
    //             J(i, k) = 0.0;
    //             J(k, i) = 0.0;
    //         }
    //         J(i, i) = 1.0;
    //     }
    // }
    //_______

    // boost::numeric::bindings::lapack::geev(J, eigenvalues, vl, vr, boost::numeric::bindings::lapack::optimal_workspace());
    // boost::numeric::bindings::lapack::geev(J, eigenvalues, vl, vr, work);

    //________
    // vector<double> eigenvalues(3 * nodes_.size());
    // vector<double> wi(3 * nodes_.size());
    // matrix<double, column_major> vl(3 * nodes_.size(), 3 * nodes_.size()),vr(3 * nodes_.size(), 3 * nodes_.size());
    // boost::numeric::bindings::lapack::geev('N','V',J,eigenvalues,wi,vl,vr,boost::numeric::bindings::lapack::optimal_workspace());

    // for(int i=0;i<3*nodes_.size();i++)
    // {
    //     eigenvalues(i)=sqrt(eigenvalues(i));
    // }