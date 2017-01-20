#ifndef falsenearestH
#define falsenearestH


#include "nearest.h"


/*
	The false nearest neighbour algorithm is described in
    o  Matthew B Kennel, Reggie Brown, and Henry D. I. Abarbanel
    o  Determining embedding dimension for phase-space reconstruction
       using a geometrical construction
    o  Physical Review A, Volume 45, Number 6, 15 March 1992, pp 3403-3411
*/


class FalseNNExcept : public ExCatch {};

class InvalidFalseNNSettings : public FalseNNExcept
{
public:
	InvalidFalseNNSettings(void);
};





class FalseNearest
{
private:
   const RawData& source_data;
   const uint lags;
   vector<valarray_fp> distances;
   vector<valarray_fp> R_D_plus_one;
   vector<valarray_fp> A_tol_D_plus_one;

public:
   FalseNearest(const RawData& sd, uint l);
   valarray_fp FalseNeighbours(fp R_tol, fp A_tol);

   friend ostream& operator<<(ostream& os, const FalseNearest& fnn);
};


#endif
