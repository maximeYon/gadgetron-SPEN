#ifndef ChangeTagsGadget_H_
#define ChangeTagsGadget_H_

#include "Gadget.h"
#include "hoNDArray.h"
#include "gadgetron_mricore_export.h"

#include <ismrmrd/ismrmrd.h>
#include <complex>

namespace Gadgetron{

class EXPORTGADGETSMRICORE ChangeTagsGadget :
  public Gadget2< ISMRMRD::AcquisitionHeader, hoNDArray< std::complex<float> > >
    {
    public:
      GADGET_DECLARE(ChangeTagsGadget);
      
      ChangeTagsGadget();
      virtual ~ChangeTagsGadget();
      
      virtual int process_config(ACE_Message_Block* mb);
      virtual int process(GadgetContainerMessage< ISMRMRD::AcquisitionHeader >* m1,			  GadgetContainerMessage< hoNDArray< std::complex<float> > > * m2);
      
    protected:

       unsigned int number_of_sets;
        unsigned int number_of_repetition;
         unsigned int number_of_encoding_step_e1;
         unsigned int number_of_encoding_step_e2;
          
         unsigned int number_of_segment_EPI;

          std::vector<size_t> dimensions_;
    };
}
#endif /* ChangeTagsGadget_H_ */
