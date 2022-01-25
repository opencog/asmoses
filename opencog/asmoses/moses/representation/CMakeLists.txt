#install header files
INSTALL(FILES

        field_set.h
        instance.h
        scored_instance.h
        instance_scorer.h
        instance_set.h
        build_knobs.h
        knob_mapper.h
        knobs.h
        representation.h
        representation_parameters.h
        AtomeseRepresentation.h
        BuildAtomeseKnobs.h
        KnobLink.h

        DESTINATION

        "include/opencog/${PROJECT_NAME}/moses/representation"
)
