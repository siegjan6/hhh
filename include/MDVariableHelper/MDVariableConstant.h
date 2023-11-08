#ifndef MDVARIABLECONSTANT_H
#define MDVARIABLECONSTANT_H

enum class MDVariableDataType
{
    null_type   = 0,

    n_int8      = 1,
    n_uint8  = 2,
    n_int16      = 3,
    n_uint16  = 4,
    n_int32      = 5,
    n_uint32  = 6,
    n_int64      = 7,
    n_uint64 = 8,
    n_float = 9,
    n_double  = 10,
    n_datetime = 11,
    n_string = 12
};

enum class MDVariableChangeType
{
    no_change   = 0,
    data_changed      = 1,
    quality_changed  = 2,
    both_changed      = 3
};

#endif // MDVARIABLECONSTANT_H
