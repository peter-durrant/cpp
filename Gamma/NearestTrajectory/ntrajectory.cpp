#include "ntrajectory.h"

/*
NearestTrajectory :: NearestTrajectory(uint q, const kdTree& t, uint p)
                   : Nearest(q, t, p), trajectory_points(p)
{
}




void NearestTrajectory :: Add_Point(uint index)
{
	const IOVector& bucket_point = kdtree[index];
	const fp distance = Distance(query_point, bucket_point); // squared furthest distance

   if (distance == fp_ZERO) {	// zero nearest neighbour
   	zero_nn_list.push_back(index);
      return;
   }

   if (distance > furthest_distance) return;

	uint i = 0;
   uint j = near_neighbours.size();
   while (i < j && distance > near_neighbours[i]->distance) i++;

	if (distance == near_neighbours[i]->distance) // equal nearest neighbour
   	near_neighbours[i]->index_list.push_back(index);
   else { // insert point into list
		j--;
      delete near_neighbours[j];
   	while (j > i) {
	      near_neighbours[j] = near_neighbours[j-1];
   	   j--;
	   }
		near_neighbours[i] = new near_points(distance, index);
   	furthest_distance = near_neighbours[near_neighbours.size()-1]->distance;
   }
}




void NearestTrajectory :: Search(const node* root)
{
	if (root->Terminal()) {
		for (uint i = 0; i < root->index_list.size(); i++)
         Add_Point(root->index_list[i]);
      return;
   }

   const uint d = root->partition_key; // partition key
   const fp p = kdtree[root->median].Input_Vector()[d];	// partition value

	const valarray_fp& query_input = query_point.Input_Vector();
   if (query_input[d] < p) { // recurse on nearest child
		fp temp = upper_bound[d];
		upper_bound[d] = p;
		Search(root->left);
		upper_bound[d] = temp;

		temp = lower_bound[d];
		lower_bound[d] = p;
		if (Bounds_Overlap_Ball()) Search(root->right);
      lower_bound[d] = temp; 
   } else {
		fp temp = lower_bound[d];
		lower_bound[d] = p;
		Search(root->right);
		lower_bound[d] = temp;

		temp = upper_bound[d];
		upper_bound[d] = p;
		if (Bounds_Overlap_Ball()) Search(root->left);
		upper_bound[d] = temp;
   }
}
*/
