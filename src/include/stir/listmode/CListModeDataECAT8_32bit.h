/*
    Copyright (C) 2013-2014 University College London
    This file is part of STIR.

    SPDX-License-Identifier: Apache-2.0

    See STIR/LICENSE.txt for details
*/
/*!
  \file
  \ingroup listmode
  \brief Declaration of class stir::ecat::CListModeDataECAT8_32bit

  \author Kris Thielemans
*/

#ifndef __stir_listmode_CListModeDataECAT8_32bit_H__
#define __stir_listmode_CListModeDataECAT8_32bit_H__

#include "stir/listmode/CListModeData.h"
#include "stir/listmode/CListRecordECAT8_32bit.h"
#include "stir/IO/InputStreamWithRecords.h"
#include "stir/shared_ptr.h"
#include "stir/IO/InterfileHeaderSiemens.h"
#include <iostream>
#include <vector>

START_NAMESPACE_STIR
namespace ecat
{

//! A class that reads the listmode data for Siemens scanners
/*!  \ingroup listmode
    This file format is currently used by the Siemens Biograph PET/CT and mMR scanners.
    There's an Interfile-like header and a binary file with the actual list mode data.
    The name of the binary file is given by the value of the "name of data file" keyword
    in the header.

    Currently, the class only supports the 32bit version of the list mode format, see
    http://usa.healthcare.siemens.com/siemens_hwem-hwem_ssxa_websites-context-root/wcm/idc/groups/public/@us/@imaging/@molecular/documents/download/mdax/mjky/~edisp/petlink_guideline_j1-00672485.pdf
*/
class CListModeDataECAT8_32bit : public CListModeData
{
public:
  //! Construct fron the filename of the Interfile header
  CListModeDataECAT8_32bit(const std::string& listmode_filename_prefix);

  std::string get_name() const override;

  shared_ptr<CListRecord> get_empty_record_sptr() const override;

  Succeeded get_next_record(CListRecord& record) const override;

  Succeeded reset() override;

  SavedPosition save_get_position() override;

  Succeeded set_get_position(const SavedPosition&) override;

  //! returns \c true, as ECAT listmode data stores delayed events (and prompts)
  /*! \todo this might depend on the acquisition parameters */
  bool has_delayeds() const override { return true; }

private:
  typedef CListRecordECAT8_32bit CListRecordT;
  std::string listmode_filename;
  shared_ptr<InputStreamWithRecords<CListRecordT, bool>> current_lm_data_ptr;

  InterfileListmodeHeaderSiemens interfile_parser;

  // std::vector<int> segment_table;

  Succeeded open_lm_file();
};

} // namespace ecat
END_NAMESPACE_STIR

#endif
