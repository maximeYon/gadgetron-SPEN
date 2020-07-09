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
        public Gadget2<ISMRMRD::ImageHeader, hoNDArray<std::complex<float>>>

     {

    public:
    GADGET_DECLARE(ExtractDiffGadget);

    ExtractDiffGadget();

    virtual ~ExtractDiffGadget();



    protected:
    GADGET_PROPERTY(extract_mask, int, "(DEPRECATED) Extract mask, bitmask MAG=1, REAL=2, IMAG=4, PHASE=8", 0);
    GADGET_PROPERTY(extract_magnitude, bool, "Extract absolute value", true);
    GADGET_PROPERTY(extract_real, bool, "Extract real components", false);
    GADGET_PROPERTY(extract_imag, bool, "Extract imaginary component", false);
    GADGET_PROPERTY(extract_phase, bool, "Extract phase", false);
    GADGET_PROPERTY(real_imag_offset, float, "Offset to add to real and imag images", 0.0f);

    virtual int process(GadgetContainerMessage <ISMRMRD::ImageHeader> *m1,
                        GadgetContainerMessage <hoNDArray<std::complex<float> >> *m2) override;


    virtual int process_config(ACE_Message_Block* mb) override;

    float minimum_component(const hoNDArray<std::complex<float>>&);

    std::vector<ISMRMRD::ISMRMRD_ImageTypes> image_types;
};
}

#endif /* ExtractDiffGadget_H_ */
