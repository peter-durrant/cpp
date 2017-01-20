#ifndef embeddingH
#define embeddingH


#include "gamma.h"


class Embedding
{
protected:
	ostream& output_stream;

public:
	Embedding(ostream& os);
};




class Full_Embedding : public Embedding
{
private:
	const Data& data;
   const uint pmax;
   const uint high_moments;

public:
	Full_Embedding(const Data& d, const uint p, const uint num_m, ostream& os);
};



class River_Embedding : public Embedding
{
private:
	const Data& data;
   const uint pmax;
   const uint high_moments;

public:
	River_Embedding(const Data& d, const uint p, const uint num_m, ostream& os);
};


#endif
