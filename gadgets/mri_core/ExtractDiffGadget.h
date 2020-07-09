#ifndef ExtractDiffGadget_H_
#define ExtractDiffGadget_H_

#include "Gadget.h"
#include "hoNDArray.h"
#include "../mri_core/GadgetMRIHeaders.h"
#include "../mri_core/gadgetron_mricore_export.h"

#include <ismrmrd/ismrmrd.h>
#include <complex>
#include <ismrmrd/meta.h>


namespace Gadgetron {



    class EXPORTGADGETSMRICORE ExtractDiffGadget :
        public Gadget2<ISMRMRD::ImageHeader, hoNDArray<float>>

     {

    public:
    GADGET_DECLARE(ExtractDiffGadget);

    ExtractDiffGadget();

    virtual ~ExtractDiffGadget();



    protected:
    
    virtual int process(GadgetContainerMessage <ISMRMRD::ImageHeader> *m1,
                        GadgetContainerMessage <hoNDArray<float> > *m2) override;


    virtual int process_config(ACE_Message_Block* mb) override;

   
};
}

#endif /* ExtractDiffGadget_H_ */
