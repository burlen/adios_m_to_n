#ifndef ADIOS_TT_H
#define ADIOS_TT_H

template<typename n_t>
class adios_tt;

#define DEFINE_ADIOS_TT(CT,AT)                              \
template<> class adios_tt<CT>                               \
{                                                           \
public:                                                     \
    static ADIOS_DATATYPES type(){ return AT; }             \
    static const char *name(){ return #AT; }                \
};

DEFINE_ADIOS_TT(char, adios_byte)
DEFINE_ADIOS_TT(int, adios_integer)
DEFINE_ADIOS_TT(long, adios_long)
#if defined(ADIOS_ISSUE_2)
DEFINE_ADIOS_TT(unsigned char, adios_unsigned_byte)
#else
DEFINE_ADIOS_TT(unsigned char, adios_byte)
#endif
DEFINE_ADIOS_TT(unsigned int, adios_unsigned_integer)
DEFINE_ADIOS_TT(unsigned long, adios_unsigned_long)
DEFINE_ADIOS_TT(float, adios_real)
DEFINE_ADIOS_TT(double, adios_double)

#endif
