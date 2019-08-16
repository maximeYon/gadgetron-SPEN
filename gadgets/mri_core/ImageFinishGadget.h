#pragma once

#include "Gadget.h"
#include "Node.h"
#include "gadgetron_mricore_export.h"

namespace Gadgetron {

    class EXPORTGADGETSMRICORE ImageFinishGadget : public Core::GenericChannelGadget {
    public:
        ImageFinishGadget(
                const Core::Context &context,
                const Core::GadgetProperties &properties
        ) : GenericChannelGadget(properties) {};

    protected:
        void process(Core::InputChannel& in,
                    Core::OutputChannel& out) override;

    };
}

