#include "RandMat.h"

RandMat::RandMat() {}

void RandMat::init(int dim, int is_mb)
{
	mat_GF2 mat;
	int hw;
	mat.SetDims(dim, dim);
	this->dim = dim;
	do {
		for (int i = 0; i < dim; i++)
	    {
	    	do
	    	{
	    		hw = 0;
				for (int j = 0; j < dim; j++)
				{
		            mat[i][j] = random_GF2();
		            if (mat[i][j] == 1)
			            hw += 1;
		    	}
	    	} while (hw == 1);
	    }
	    this->mat = mat;
	    if (is_mb == 0) {
	    	return;
	    }
	} while (determinant(mat) == 0);

    this->invMat = inv(mat);
}

ostream& operator<<(ostream& stream, const RandMat& rMat)
{
	cout << "Dim=" << rMat.dim << endl;
	for (int i = 0; i < rMat.dim; i++)
    {
		for (int j = 0; j < rMat.dim; j++)
		{
			cout << rMat.mat[i][j] << " ";
		}
		cout << endl;
	}

	cout << endl;

	if (determinant(rMat.mat) == 0)
		return stream;

	for (int i = 0; i < rMat.dim; i++)
    {
		for (int j = 0; j < rMat.dim; j++)
		{
			cout << rMat.invMat[i][j] << " ";
		}
		cout << endl;
	}

	return stream;
}