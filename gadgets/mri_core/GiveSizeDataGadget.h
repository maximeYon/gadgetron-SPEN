#ifndef GiveSizeDataGadget_H_
#define GiveSizeDataGadget_H_

#include "Gadget.h"
#include "hoNDArray.h"
#include "gadgetron_mricore_export.h"

#include <ismrmrd/ismrmrd.h>
#include <complex>

namespace Gadgetron{

class EXPORTGADGETSMRICORE GiveSizeDataGadget :
  public Gadget2< ISMRMRD::AcquisitionHeader, hoNDArray< std::complex<float> > >
    {
    public:
      GADGET_DECLARE(GiveSizeDataGadget);
      
      GiveSizeDataGadget();
      virtual ~GiveSizeDataGadget();
      
      virtual int process_config(ACE_Message_Block* mb);
      virtual int process(GadgetContainerMessage< ISMRMRD::AcquisitionHeader >* m1,			  GadgetContainerMessage< hoNDArray< std::complex<float> > > * m2);
      
    protected:
          
    };
}
#endif /* GiveSizeDataGadget_H_ */
