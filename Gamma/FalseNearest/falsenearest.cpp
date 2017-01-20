#include "falsenearest.h"


InvalidFalseNNSettings :: InvalidFalseNNSettings(void)
{
	message << "Invalid settings for 'False Nearest Neighbours' algorithm.";
}




FalseNearest :: FalseNearest(const RawData& sd, uint l)
              : source_data(sd), lags(l)
{
   if (source_data.Inputs() != 1 || source_data.Outputs() != 0 ||
       source_data.Series() != 1 || source_data.Vectors() <= lags) throw InvalidFalseNNSettings();

   distances.resize(lags);
   R_D_plus_one.resize(lags);
   A_tol_D_plus_one.resize(lags);
   for (uint i = 0; i < lags; i++) {
      distances[i].resize(sd.Vectors() - (i+1));
      R_D_plus_one[i].resize(sd.Vectors() - (i+1));
      A_tol_D_plus_one[i].resize(sd.Vectors() - (i+1));
   }

   vector<FormatType> ft(1);

   for (uint i = 0; i < lags; i++) {
		fp x_bar = fp_ZERO;
      ft[0] = FormatType(TS, i+1, 1);
      Data data(source_data, ft);
      kdTree kdtree(data, 10);
      for (uint j = 0; j < data.Size()-1; j++) {
      	x_bar += data[j].Output_Vector(0);
		}
      x_bar /= data.Size()-1;

		fp R_A = fp_ZERO;
      for (uint j = 0; j < data.Size()-1; j++) {
			R_A += Raise(data[j].Output_Vector(0) - x_bar, 2);
		}
		R_A /= data.Size()-1;
      R_A = Dissim(R_A);

      for (uint j = 0; j < data.Size()-1; j++) {
         Nearest nn(data[j], kdtree, 1);
		   const near_points& neighbour = nn.neighbours(0);
	   	const valarray_fp& output1 = data[j].Output_Vector();
       	const valarray_fp& output2 = data[neighbour.index_list[0]].Output_Vector();
       	const fp numerator = fabs(output1[0] - output2[0]);
         const fp Dd = Dissim(neighbour.distance); // sqrt the squared distance
         const fp S = numerator / Dd;
         distances[i][j] = S;
         R_D_plus_one[i][j] = Dissim(neighbour.distance + Raise(numerator, 2));
         A_tol_D_plus_one[i][j] = R_D_plus_one[i][j] / R_A;
      }
   }
}




valarray_fp FalseNearest :: FalseNeighbours(fp R_tol, fp A_tol)
{
   valarray_fp tolerances(distances.size());
   for (uint i = 0; i < distances.size(); i++) {
      uint sum = 0;
      for (uint j = 0; j < distances[i].size(); j++)
         if (distances[i][j] > R_tol || A_tol_D_plus_one[i][j] > A_tol) sum++;
      tolerances[i] = (fp)sum / distances[i].size();
   }
   return tolerances;
}




ostream& operator<<(ostream& os, const FalseNearest& fnn)
{
	fp average_distance;
 	for (uint i = 0; i < fnn.distances.size(); i++) {
   	average_distance = 0;
		for (uint j = 0; j < fnn.distances[i].size(); j++)
			average_distance += fnn.distances[i][j];
		average_distance /= fnn.distances[i].size();
      os << "Lag = " << (i+1) << ", average distance = " << average_distance << endl; 
   }
   return os;
}
