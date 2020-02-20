/*
* RemoveNavigationDataGadget.cpp
*
*  Created on: Dec 5, 2011
*      Author: hansenms
*/

#include "RemoveNavigationDataGadget.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include "ismrmrd/xml.h"

namespace Gadgetron{

RemoveNavigationDataGadget::RemoveNavigationDataGadget() {
}

RemoveNavigationDataGadget::~RemoveNavigationDataGadget() {
}

int RemoveNavigationDataGadget::process_config(ACE_Message_Block *mb)
{
    return GADGET_OK;
}


int RemoveNavigationDataGadget::process(GadgetContainerMessage<ISMRMRD::AcquisitionHeader> *m1, GadgetContainerMessage<hoNDArray<std::complex<float> > > *m2)
{

    if(m1->getObjectPtr()->isFlagSet(ISMRMRD::ISMRMRD_ACQ_IS_NAVIGATION_DATA))
    {

        //std::vector<size_t> data_out_dims = *m2->getObjectPtr()->get_dimensions();
        //std::cout << data_out_dims[0] << " " << data_out_dims[1] << std::endl;

        m1->release();
    }
    else
    {
        if( this->next()->putq(m1) < 0 ){
            GDEBUG("Failed to put message on queue\n");
            return GADGET_FAIL;
        }
    }

    return GADGET_OK;
}

GADGET_FACTORY_DECLARE(RemoveNavigationDataGadget)
}
