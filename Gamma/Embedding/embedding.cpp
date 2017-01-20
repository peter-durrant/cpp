#include "embedding.h"
#include "mask.h"

Embedding :: Embedding(ostream& os)
			  : output_stream(os)
{
}



Full_Embedding :: Full_Embedding(const Data& d, const uint p, const uint num_m, ostream& os)
				    : Embedding(os), data(d), pmax(p), high_moments(num_m)
{
	Mask mask(data.Inputs());
//   mask.Set_Mask("00000000001");

	while (!mask.Empty()) {
      Data embed_data(data, mask);
		Gamma gamma(embed_data, pmax, high_moments);
		output_stream << mask << "," << gamma << endl;
		mask.Decrement();
   }
}



River_Embedding :: River_Embedding(const Data& d, const uint p, const uint num_m, ostream& os)
				     : Embedding(os), data(d), pmax(p), high_moments(num_m)
{
cout << "1" << endl;
	Mask mask(data.Inputs());
cout << "2" << endl;
//00000000000000000000
//00000000000000000000
//00000000000000000000
//00000000000000000000
//00000000000000000000
//00000000000000000000
//00000000001000000000
//00000000000000000000
//00000000000000000000
//00000000000000000000
//00000000000000000000
//00000000000000000000
//00000000000000000000
//00000000000000000000
//00000000000000000000
//00000000000000000000
//00000000000000000000
//	mask.Set_Mask("111111111111111111111111111111111111111111111111111111111111111111111111111");
//	Data embed_data(data, mask);
cout << "3" << endl;
//	Gamma gamma(embed_data, pmax, high_moments);
cout << "4" << endl;
//	output_stream << mask << "," << gamma << endl;


	for (unsigned int j = 0; j < 16; j++) {
		for (unsigned int i = 0; i < 20; i++) {
			stringstream m;
	      for (unsigned int k = 0; k < j; k++) {
				m << "00000000000000000000";
      	}
         for (unsigned int k = 0; k < i; k++) {
				m << "0";
     	   }
         m << "1";
         for (unsigned int k = i+1; k < 20; k++) {
				m << "0";
         }
	      for (unsigned int k = j+1; k < 16; k++) {
				m << "00000000000000000000";
      	}
			m << "00000000000000000000";
         cout << m << endl;
		   mask.Set_Mask(m.str().c_str());
	      Data embed_data(data, mask);

			Gamma gamma(embed_data, pmax, high_moments);

//			os << embed_data << endl;

			output_stream << mask << "," << gamma << endl;
		}
   }
}
