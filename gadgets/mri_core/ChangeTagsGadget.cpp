/*
* ChangeTagsGadget.cpp
*
*  Created on: Dec 5, 2011
*      Author: hansenms
*/

#include "ChangeTagsGadget.h"
#include "GadgetIsmrmrdReadWrite.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include "ismrmrd/xml.h"

namespace Gadgetron{

ChangeTagsGadget::ChangeTagsGadget() {
}

ChangeTagsGadget::~ChangeTagsGadget() {
}

int ChangeTagsGadget::process_config(ACE_Message_Block *mb)
{
    ISMRMRD::IsmrmrdHeader h;
    ISMRMRD::deserialize(mb->rd_ptr(), h);

    ISMRMRD::EncodingLimits e_limits = h.encoding[0].encodingLimits;

    ISMRMRD::EncodingSpace e_space = h.encoding[0].encodedSpace;
    dimensions_.push_back(e_space.matrixSize.x);
    dimensions_.push_back(e_space.matrixSize.y);
    dimensions_.push_back(e_space.matrixSize.z);

    // GDEBUG(" Base: %d, %d, %d\n", dimensions_[0], dimensions_[1], dimensions_[2]);

    number_of_sets  = e_limits.set->maximum +1 ;
    number_of_repetition= e_limits.repetition->maximum +1 ;   //e_limits.repetition? e_limits.repetition->maximum+1 : 1
    number_of_encoding_step_e1 =e_limits.kspace_encoding_step_1->maximum+1;
    number_of_encoding_step_e2 =e_limits.kspace_encoding_step_2->maximum+1;
   // to comment
    number_of_segment_EPI=dimensions_[1]/number_of_encoding_step_e1;  //number_of_sets/number_of_repetition;

    // GDEBUG(" number_of_repetition   %d \n" , number_of_repetition  );
     GDEBUG(" Number_of_sets   %d \n" , number_of_sets  );
    // GDEBUG(" number_of_encoding_step_e1   %d \n" ,  number_of_encoding_step_e1  );
    // GDEBUG(" number_of_encoding_step_e2   %d \n" ,  number_of_encoding_step_e2  );
     GDEBUG(" number_of_segment_EPI   %d \n" ,  number_of_segment_EPI  );

    return GADGET_OK;
}


int ChangeTagsGadget::process(GadgetContainerMessage<ISMRMRD::AcquisitionHeader> *m1, GadgetContainerMessage<hoNDArray<std::complex<float> > > *m2)
{

    unsigned int e1= m1->getObjectPtr()->idx.kspace_encode_step_1;
    unsigned int set= m1->getObjectPtr()->idx.set;
    unsigned int repetition= m1->getObjectPtr()->idx.repetition;

    // cas 1 necessite une modification de la parameter map (xml) pour que hdr.encoding.encolmilit.ksapce_1.maximum =hdr.encoding.encolmilit.ksapce_1.maximum * NSegments
    //unsigned int e1_new=e1+((set*number_of_encoding_step_e1)-(repetition*(number_of_encoding_step_e1*number_of_segment_EPI)));
    //m1->getObjectPtr()->idx.kspace_encode_step_1=e1_new;
    //m1->getObjectPtr()->idx.repetition= 0;
    //m1->getObjectPtr()->idx.set=repetition;


    //std::cout << " e1  " << e1 <<  " e1_new " <<  e1_new << "  set: " << set  << "  repetition: " << repetition << "new_segment_EPI : "  << new_segment_EPI <<  std::endl;
   // std::cout << " e1  " << e1 <<   "  set: " << set  << "  repetition: " << repetition << "new_segment_EPI : "  << new_segment_EPI <<  std::endl;

    // cas 2
    unsigned int new_segment_EPI= set%number_of_segment_EPI;
    //m1->getObjectPtr()->idx.set= new_segment_EPI;

    m1->getObjectPtr()->idx.kspace_encode_step_1=e1;
    m1->getObjectPtr()->idx.repetition= new_segment_EPI;  //segment EPI de 0 à N-1  typiquement 0 1 2
    m1->getObjectPtr()->idx.set=repetition;    // contrast bvalue typiquement 0 1 2 à 13

    if( this->next()->putq(m1) < 0 ){
        GDEBUG("Failed to put message on queue\n");
        return GADGET_FAIL;
    }

return GADGET_OK;
}

GADGET_FACTORY_DECLARE(ChangeTagsGadget)
}
