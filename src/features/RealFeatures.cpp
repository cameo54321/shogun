#include <assert.h>
#include "RealFeatures.h"

CRealFeatures::CRealFeatures() : CFeatures(), num_vectors(0), num_features(0), feature_matrix(NULL)
{
}

CRealFeatures::~CRealFeatures()
{
  delete[] feature_matrix;
}

/// get feature vector for sample num
REAL* CRealFeatures::get_feature_vector(long num, long &len, bool &free)
{
  //fprintf(stderr, "RealFeat.get_feature_vector(%ld) [%ld];\n", num, num_vectors) ;
  len=num_features ; 
  assert(num<num_vectors) ;
  
  if (feature_matrix)
    {
      free=false ;
      return &feature_matrix[num*num_features];
    } 
  else
    {
      free=true ;
      REAL* feat=compute_feature_vector(num, len) ;
      if (preproc)
	{
	  REAL* feat2 = preproc->apply_to_feature_vector(feat, len);
	  delete[] feat ;
	  return feat2 ;
	}
      return feat ;
    }
}

void CRealFeatures::free_feature_vector(REAL* feat, bool free)
{
  if (free)
    delete[] feat ;
} 

/// get the pointer to the feature matrix
REAL* CRealFeatures::get_feature_matrix(long &num_feat, long &num_vec)
{
  num_feat=num_features;
  num_vec=num_vectors;
  return feature_matrix;
}

/// preproc feature_matrix
/*bool CRealFeatures::preproc_feature_matrix()
{
  if (preproc)
    preproc->preproc_feature(this);
}*/


