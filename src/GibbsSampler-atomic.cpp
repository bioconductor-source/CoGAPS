#include "GibbsSampler.h"

// ********* METHODS TO GO BETWEEN ATOMIC SPACE AND MATRIX  ********************
unsigned int GibbsSampler::getRow(char matrix_label , unsigned int iBin) {
    switch (matrix_label) {
        case 'A': { // A - horizontal addressing implicit
            return (floor(iBin / _nFactor));
        }

        case 'P': { // P - vertical addressing implicit
            return iBin % _nFactor;
        }
    }

    // EJF dummy return to avoid warnings
    return iBin;
}

unsigned int GibbsSampler::getCol(char matrix_label , unsigned int iBin) {
    switch (matrix_label) {
        case 'A': { // A - horizontal addressing implicit
            return iBin % _nFactor;
        }

        case 'P': { // P - vertical addressing implicit
            return floor(iBin / _nFactor);
        }
    }

    // EJF dummy return to avoid warnings
    return iBin;
}


unsigned int GibbsSampler::getTotNumAtoms(char matrix_label) {
    switch (matrix_label) {
        case 'A': {
            return _AAtomicdomain.numAtoms();
        }

        case 'P': {
            return _PAtomicdomain.numAtoms();
        }
    }

    // EJF dummy return to avoid warnings
    return 0;
}

vector <vector <vector <double> > > GibbsSampler::getNormedMatrices()
{
    vector <vector <double> > AMatrixNormed;
    AMatrixNormed.resize(_nRow, vector<double>(_nFactor, 0.0));
    vector <vector <double> > PMatrixNormed;
    PMatrixNormed.resize(_nFactor, vector<double>(_nCol, 0.0));
    vector<double> k(_nFactor);  // normalized vector

    // compute the normalization vector
    for (int m = 0; m < _nFactor; ++m)
    {
        k[m] = 0.;

        for (int n = 0; n < _nCol; ++n)
        {
            k[m] += _PMatrix(m,n);
        }

        if (k[m] == 0) // when the whole row of P is zero, then don't do anything
        {
            k[m] = 1.0;
        }
    }

    for (int m = 0; m < _nRow; ++m)
    {
        for (int n = 0; n < _nFactor; ++n)
        {
            AMatrixNormed[m][n] = _AMatrix(m,n) * k[n];
        }
    }

    for (int m = 0; m < _nFactor; ++m)
    {
        for (int n = 0; n < _nCol; ++n)
        {
            PMatrixNormed[m][n] = _PMatrix(m,n) / k[m];
        }
    }

    vector <vector <vector <double> > > NormedMatrices;
    NormedMatrices.push_back(AMatrixNormed);
    NormedMatrices.push_back(PMatrixNormed);
    return NormedMatrices;
}

// ======CoGAPS Test methods============

// Get the full atomic domain from the atomic space
/*std::map <unsigned long long, double> GibbsSampler::getAtomicDomain(char matrix_label)
{
    std::map <uint64_t, double> zero;

    if (matrix_label == 'A')
    {
        return _AAtomicdomain.getDomain();
    }
    else if (matrix_label == 'P')
    {
        return _PAtomicdomain.getDomain();
    }
    return zero;
}*/

// Manually calculate the matrix A from the atomic space passed in.
/*vector <vector <double> > GibbsSampler::createSampleAMat(map <unsigned long long, double> ADomain) {
    // Make that a matrix
    vector <vector <double> > SampleAMatrix;
    SampleAMatrix.resize(_nRow);

    for (int i = 0; i < _nRow; i++) {
        SampleAMatrix[i].resize(_nFactor, 0);
    }

    //Copy the parsed domain
    map<unsigned long long, double>::const_iterator iter;

    for (iter = ADomain.begin(); iter != ADomain.end(); iter++) {
        unsigned int theBin = _AAtomicdomain.getBin(iter->first);
        // Put the mass in the bin in the matrix
        unsigned int theRow = getRow('A', theBin);
        unsigned int theCol = getCol('A', theBin);
        SampleAMatrix[theRow][theCol] += iter->second;
    }

    return SampleAMatrix;
}

// Manually calculate the matrix P from the atomic space passed in.
vector <vector <double> > GibbsSampler::createSamplePMat(map <unsigned long long, double> PDomain) {
    // Make that a matrix
    vector <vector <double> > SamplePMatrix;
    SamplePMatrix.resize(_nFactor);

    for (int i = 0; i < _nFactor; i++) {
        SamplePMatrix[i].resize(_nCol, 0);
    }

    //Copy the parsed domain
    map<unsigned long long, double>::const_iterator iter;

    for (iter = PDomain.begin(); iter != PDomain.end(); iter++) {
        unsigned int theBin = _PAtomicdomain.getBin(iter->first);
        // Put the mass in the bin in the matrix
        unsigned int theRow = getRow('P', theBin);
        unsigned int theCol = getCol('P', theBin);
        SamplePMatrix[theRow][theCol] += iter->second;
    }

    return SamplePMatrix;
}
*/
// Manually calculate the chi squared value based on the 2 matrices passed in
double GibbsSampler::ManualCalcChiSqu(vector <vector <double> > SampleAMat, vector <vector <double> > SamplePMat) {
    Matrix SampleAMatrix(SampleAMat);
    Matrix SamplePMatrix(SamplePMat);
    return GAPSNorm::calChi2(_DMatrix, _SMatrix, SampleAMatrix,
        SamplePMatrix, _nFactor);
}

