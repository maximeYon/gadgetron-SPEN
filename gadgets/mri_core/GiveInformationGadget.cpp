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
