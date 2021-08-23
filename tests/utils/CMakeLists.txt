find_package(dlib REQUIRED)
find_package(blas REQUIRED)
ADD_CXXTEST(GenTableUTest)

TARGET_LINK_LIBRARIES(GenTableUTest
        ascombo
        data
        atomese
        dlib::dlib
        ${BLAS_LIBRARIES}
        ${COGUTIL_LIBRARY}
        )