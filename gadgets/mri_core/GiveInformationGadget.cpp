/*
* GiveInformationGadget.cpp
*
*  Created on: Dec 5, 2011
*      Author: hansenms
*/

#include "GiveInformationGadget.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include "ismrmrd/xml.h"

namespace Gadgetron{

GiveInformationGadget::GiveInformationGadget() {
}

GiveInformationGadget::~GiveInformationGadget() {
}

int GiveInformationGadget::process_config(ACE_Message_Block *mb)
{

    ISMRMRD::IsmrmrdHeader h;
    ISMRMRD::deserialize(mb->rd_ptr(),h);

    if (h.encoding.size() != 1) {
      GDEBUG("This Gadget only supports one encoding space\n");
      return GADGET_FAIL;
    }

    // Get the encoding space and trajectory description
    ISMRMRD::EncodingSpace e_space = h.encoding[0].encodedSpace;
    ISMRMRD::EncodingSpace r_space = h.encoding[0].reconSpace;
    ISMRMRD::EncodingLimits e_limits = h.encoding[0].encodingLimits;


    std::cout <<"e1 " << e_limits.kspace_encoding_step_1->maximum<< std::endl;
     std::cout <<"e2 " << e_limits.kspace_encoding_step_2->maximum<< std::endl;
     std::cout <<"contrast " << e_limits.contrast->maximum << std::endl;
     std::cout <<"set " << e_limits.set->maximum << std::endl;
     std::cout <<"segment " << e_limits.segment->maximum << std::endl;
     std::cout << "slice " <<e_limits.slice->maximum << std::endl;

    return GADGET_OK;
}


int GiveInformationGadget::process(GadgetContainerMessage<ISMRMRD::AcquisitionHeader> *m1, GadgetContainerMessage<hoNDArray<std::complex<float> > > *m2)
{

   bool phase_corr= m1->getObjectPtr()->isFlagSet(ISMRMRD::ISMRMRD_ACQ_IS_PHASECORR_DATA);

   unsigned int  ky= m1->getObjectPtr()->idx.kspace_encode_step_1;
   unsigned int  kz= m1->getObjectPtr()->idx.kspace_encode_step_2;
   unsigned int  repetition= m1->getObjectPtr()->idx.repetition;
   unsigned int  set= m1->getObjectPtr()->idx.set;
   unsigned int  segment= m1->getObjectPtr()->idx.segment;
   unsigned int  slice= m1->getObjectPtr()->idx.slice;

   unsigned int scan_counter= m1->getObjectPtr()->scan_counter;

   GDEBUG_STREAM(" coucou scan_counter: "<< scan_counter<<  " ky: "<< ky<<" kz: " <<kz<<" slice: " <<slice<<" rep: " <<repetition<< " set: "<< set<< " segment: "<< segment<< " phase_corr: "<< phase_corr);

   if( this->next()->putq(m1) < 0 ){
       GDEBUG("Failed to put message on queue\n");
       return GADGET_FAIL;
   }

    return GADGET_OK;
}

GADGET_FACTORY_DECLARE(GiveInformationGadget)
}
