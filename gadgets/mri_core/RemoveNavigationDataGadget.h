#ifndef RemoveNavigationDataGadget_H_
#define RemoveNavigationDataGadget_H_

#include "Gadget.h"
#include "hoNDArray.h"
#include "gadgetron_mricore_export.h"

#include <ismrmrd/ismrmrd.h>
#include <complex>

namespace Gadgetron{

class EXPORTGADGETSMRICORE RemoveNavigationDataGadget :
  public Gadget2< ISMRMRD::AcquisitionHeader, hoNDArray< std::complex<float> > >
    {
    public:
      GADGET_DECLARE(RemoveNavigationDataGadget);
      
      RemoveNavigationDataGadget();
      virtual ~RemoveNavigationDataGadget();
      
      virtual int process_config(ACE_Message_Block* mb);
      virtual int process(GadgetContainerMessage< ISMRMRD::AcquisitionHeader >* m1,			  GadgetContainerMessage< hoNDArray< std::complex<float> > > * m2);
      
    protected:
          
    };
}
#endif /* RemoveNavigationDataGadget_H_ */
