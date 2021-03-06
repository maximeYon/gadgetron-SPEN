if (WIN32)
    add_definitions(-D__BUILD_GADGETRON_CORE__)
endif ()

add_subdirectory(readers)
add_subdirectory(writers)
add_subdirectory(parallel)
add_subdirectory(distributed)

add_library(gadgetron_core SHARED
        Types.h
        variant.hpp
        Channel.h
        Channel.hpp
        Channel.cpp
        Gadget.h
        Gadget.cpp
        Reader.h
        Writer.h
        LegacyACE.cpp
        Message.cpp
        Writer.hpp
        Response.h
        Response.cpp
        PureGadget.h
        io/primitives.h Types.hpp MessageID.h MPMCChannel.h io/from_string.h io/from_string.cpp io/ismrmrd_types.h io/adapt_struct.h TypeTraits.h ChannelAlgorithms.h io/iostream_operators.h )

target_link_libraries(gadgetron_core
        gadgetron_toolbox_cpucore
        boost)

target_include_directories(gadgetron_core PUBLIC
        .
        ${ISMRMRD_INCLUDE_DIR})

install(TARGETS gadgetron_core 
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
   RUNTIME DESTINATION bin
   COMPONENT main
)


install(FILES
        Channel.h
        Channel.hpp
        Message.h
        Message.hpp
        MPMCChannel.h
        Gadget.h
        Context.h
        Gadget.h
        Reader.h
        Types.h
        Types.hpp
        TypeTraits.h
        Writer.h
        Node.h
        LegacyACE.h
        PropertyMixin.h
        GadgetContainerMessage.h
        ChannelAlgorithms.h
        variant.hpp
        MessageID.h
        DESTINATION ${GADGETRON_INSTALL_INCLUDE_PATH} COMPONENT main)

install(FILES
        io/adapt_struct.h
        io/from_string.h
        io/ismrmrd_types.h
        io/primitives.h
        io/primitives.hpp
        DESTINATION ${GADGETRON_INSTALL_INCLUDE_PATH}/io COMPONENT main)
install(FILES
        config/distributed_default.xml
        config/distributed_generic_default.xml
        config/distributed_image_default.xml
        DESTINATION ${GADGETRON_INSTALL_CONFIG_PATH} COMPONENT main)
