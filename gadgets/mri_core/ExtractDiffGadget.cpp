/*
 *
 * ExtractMagnitudeGadget.cpp
 *
 *  Created on: Nov 8, 2011
 *      Author: Michael S. Hansen
 */

#include <bitset>
#include <unordered_map>
#include <cpu/math/hoNDArray_math.h>
/*#include "../mri_core/GadgetIsmrmrdReadWrite.h" */
#include "ExtractDiffGadget.h"
#include "../mri_core/mri_core_def.h"

#include <boost/math/constants/constants.hpp>

namespace Gadgetron {


ExtractDiffGadget::ExtractDiffGadget() {
}

ExtractDiffGadget::~ExtractDiffGadget() {
}

int ExtractDiffGadget::process_config(ACE_Message_Block *mb) {


    return GADGET_OK;
}



int ExtractDiffGadget::process(GadgetContainerMessage<ISMRMRD::ImageHeader> *m1,   GadgetContainerMessage<hoNDArray<float>> *m2) {



     Gadgetron::GadgetContainerMessage<ISMRMRD::MetaContainer>* m3 = new Gadgetron::GadgetContainerMessage<ISMRMRD::MetaContainer>();


   // std::cout << m1->getObjectPtr()->image_series_index << std::endl;

    if (m1->getObjectPtr()->image_series_index==6000)
    {
        m3->getObjectPtr()->set(GADGETRON_IMAGECOMMENT, "ADC");
        m3->getObjectPtr()->set(GADGETRON_SEQUENCEDESCRIPTION, "ADC");    
        m3->getObjectPtr()->set(GADGETRON_IMAGEPROCESSINGHISTORY, "GT");
        //cm3->getObjectPtr()->set(GADGETRON_IMAGE_COLORMAP,  "HotMetal.pal");
        //m3->getObjectPtr()->set(GADGETRON_IMAGE_COLORMAP,  "Perfusion.pal");
        m3->getObjectPtr()->set(GADGETRON_IMAGE_WINDOWCENTER, (long)(1500));
        m3->getObjectPtr()->set(GADGETRON_IMAGE_WINDOWWIDTH, (long)(3000));
	std::cout << "ADC map section" << std::endl;
    }

    else if (m1->getObjectPtr()->image_series_index==5000)
    {
	std::cout << "Trace image section" << std::endl;
        m3->getObjectPtr()->set(GADGETRON_IMAGECOMMENT, "TRACEWEIGHTED");
        m3->getObjectPtr()->set(GADGETRON_SEQUENCEDESCRIPTION, "TRACEWEIGHTED");
        m3->getObjectPtr()->set(GADGETRON_IMAGEPROCESSINGHISTORY, "GT");
    }
    else
    {       
        m3->getObjectPtr()->set(GADGETRON_IMAGECOMMENT, "MAGNITUDE");
        m3->getObjectPtr()->set(GADGETRON_SEQUENCEDESCRIPTION, "MAGNITUDE");
        m3->getObjectPtr()->set(GADGETRON_IMAGEPROCESSINGHISTORY, "GT");
	std::cout << "DWI image section" << std::endl;
    }

    //m1->getObjectPtr()->image_series_index += series_offset.at(image_type);

    m2->cont(m3);

    if (this->next()->putq(m1) == -1) {
        m1->release();
        GDEBUG("Unable to put extracted images on next gadgets queue");
        return GADGET_FAIL;
    }


    return GADGET_OK;
}


GADGET_FACTORY_DECLARE(ExtractDiffGadget)

}
