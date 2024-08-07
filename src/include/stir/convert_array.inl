//
//
/*
    Copyright (C) 2006 - 2007, Hammersmith Imanet Ltd
    This file is part of STIR.

    SPDX-License-Identifier: Apache-2.0

    See STIR/LICENSE.txt for details
*/
/*!
  \file
  \ingroup Array
  \brief implementation of stir::convert_array

  \author Kris Thielemans



*/
#include "stir/NumericInfo.h"
#include "stir/Array.h"
#include "stir/convert_range.h"
START_NAMESPACE_STIR

template <int num_dimensions, class T1, class T2, class scaleT>
void
find_scale_factor(scaleT& scale_factor, const Array<num_dimensions, T1>& data_in, const NumericInfo<T2> info_for_out_type)
{
  find_scale_factor(scale_factor, data_in.begin_all(), data_in.end_all(), info_for_out_type);
}

template <int num_dimensions, class T1, class T2, class scaleT>
Array<num_dimensions, T2>
convert_array(scaleT& scale_factor, const Array<num_dimensions, T1>& data_in, const NumericInfo<T2> info_for_out_type)
{
  Array<num_dimensions, T2> data_out(data_in.get_index_range());

  convert_array(data_out, scale_factor, data_in);
  return data_out;
}

template <int num_dimensions, class T1, class T2, class scaleT>
void
convert_array(Array<num_dimensions, T2>& data_out, scaleT& scale_factor, const Array<num_dimensions, T1>& data_in)
{
  convert_range(data_out.begin_all(), scale_factor, data_in.begin_all(), data_in.end_all());
}

END_NAMESPACE_STIR
