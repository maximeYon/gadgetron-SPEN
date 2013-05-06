/** \file cuNonCartesianKtSenseOperator.h
    \brief Non-Cartesian kt-Sense operator, GPU based.
*/

#pragma once

#include "cuNonCartesianSenseOperator.h"

namespace Gadgetron{

  template<class REAL, unsigned int D>
  class EXPORTGPUPMRI cuNonCartesianKtSenseOperator : public cuNonCartesianSenseOperator<REAL,D>
  {
    
  public:
    
    typedef typename uintd<D>::Type _uintd;
    typedef typename reald<REAL,D>::Type _reald;
    
    cuNonCartesianKtSenseOperator() : cuNonCartesianSenseOperator<REAL,D>() {}
    virtual ~cuNonCartesianKtSenseOperator() {}
    
    virtual void mult_M( cuNDArray< complext<REAL> >* in, cuNDArray< complext<REAL> >* out, bool accumulate = false );
    virtual void mult_MH( cuNDArray< complext<REAL> >* in, cuNDArray< complext<REAL> >* out, bool accumulate = false );
    
    virtual boost::shared_ptr< linearOperator<cuNDArray< complext<REAL>  > > > clone(){
      return linearOperator< cuNDArray<complext<REAL> > >::clone(this);
    }  
  };
}