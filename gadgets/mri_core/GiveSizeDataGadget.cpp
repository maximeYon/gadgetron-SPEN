/*
* GiveSizeDataGadget.cpp
*
*  Created on: Dec 5, 2011
*      Author: hansenms
*/

#include "GiveSizeDataGadget.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include "ismrmrd/xml.h"

namespace Gadgetron{

GiveSizeDataGadget::GiveSizeDataGadget() {
}

GiveSizeDataGadget::~GiveSizeDataGadget() {
}

int GiveSizeDataGadget::process_config(ACE_Message_Block *mb)
{
    return GADGET_OK;
}


int GiveSizeDataGadget::process(GadgetContainerMessage<ISMRMRD::AcquisitionHeader> *m1, GadgetContainerMessage<hoNDArray<std::complex<float> > > *m2)
{

    std::vector<size_t> data_out_dims = *m2->getObjectPtr()->get_dimensions();

    std::cout << data_out_dims[0] << " " << data_out_dims[1] << std::endl;

    // Get the dimensions
    size_t nx = m2->getObjectPtr()->get_size(0);
    size_t nc = m2->getObjectPtr()->get_size(1);

    std::cout << nx << " " << nc << std::endl;


        if( this->next()->putq(m1) < 0 ){
            GDEBUG("Failed to put message on queue\n");
            return GADGET_FAIL;
        }


    return GADGET_OK;
}

GADGET_FACTORY_DECLARE(GiveSizeDataGadget)
}
