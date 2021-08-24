ADD_CXXTEST(GenTableUTest)
find_library(dai dai)
find_library(gmp gmp)
find_library(gmpxx gmpxx)

MESSAGE("libdai found in ${dai}")
MESSAGE("libgmpxx found in ${gmpxx}")

TARGET_LINK_LIBRARIES(GenTableUTest
        ascombo
        data
        atomese
        ${dai}
        ${gmp}
        ${gmpxx}
        ${COGUTIL_LIBRARY}
        )