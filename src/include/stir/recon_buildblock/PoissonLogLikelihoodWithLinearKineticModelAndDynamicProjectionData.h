//
//
/*
    Copyright (C) 2006- 2013, Hammersmith Imanet Ltd
    Copyright (C) 2018, University College London

    SPDX-License-Identifier: Apache-2.0

    See STIR/LICENSE.txt for details
*/
/*!
  \file
  \ingroup GeneralisedObjectiveFunction
  \ingroup modelling
  \brief Declaration of class stir::PoissonLogLikelihoodWithLinearKineticModelAndDynamicProjectionData

  \author Kris Thielemans
  \author Sanida Mustafovic
  \author Charalampos Tsoumpas

*/

#ifndef __stir_recon_buildblock_PoissonLogLikelihoodWithLinearKineticModelAndDynamicProjectionData_H__
#define __stir_recon_buildblock_PoissonLogLikelihoodWithLinearKineticModelAndDynamicProjectionData_H__
#include "stir/RegisteredParsingObject.h"
#include "stir/recon_buildblock/PoissonLogLikelihoodWithLinearModelForMeanAndProjData.h"
#include "stir/recon_buildblock/PoissonLogLikelihoodWithLinearModelForMean.h"
#include "stir/VectorWithOffset.h"
#include "stir/DynamicProjData.h"
#include "stir/DynamicDiscretisedDensity.h"
#include "stir/modelling/ParseAndCreateParametricDiscretisedDensityFrom.h"
#include "stir/modelling/ParametricDiscretisedDensity.h"
#include "stir/modelling/KineticParameters.h"
#include "stir/modelling/PatlakPlot.h"

START_NAMESPACE_STIR

/*!
  \ingroup GeneralisedObjectiveFunction
  \ingroup modelling
  \brief a base class for LogLikelihood of independent Poisson variables
  where the mean values are linear combinations of the kinetic parameters.

  \par Parameters for parsing

*/

template <typename TargetT>
class PoissonLogLikelihoodWithLinearKineticModelAndDynamicProjectionData
    : public RegisteredParsingObject<PoissonLogLikelihoodWithLinearKineticModelAndDynamicProjectionData<TargetT>,
                                     GeneralisedObjectiveFunction<TargetT>,
                                     PoissonLogLikelihoodWithLinearModelForMean<TargetT>>
{
private:
  typedef RegisteredParsingObject<PoissonLogLikelihoodWithLinearKineticModelAndDynamicProjectionData<TargetT>,
                                  GeneralisedObjectiveFunction<TargetT>,
                                  PoissonLogLikelihoodWithLinearModelForMean<TargetT>>
      base_type;
  typedef PoissonLogLikelihoodWithLinearModelForMeanAndProjData<DiscretisedDensity<3, float>> SingleFrameObjFunc;
  VectorWithOffset<SingleFrameObjFunc> _single_frame_obj_funcs;

public:
  //! Name which will be used when parsing a GeneralisedObjectiveFunction object
  static const char* const registered_name;

  PoissonLogLikelihoodWithLinearKineticModelAndDynamicProjectionData();

  //! Returns a pointer to a newly allocated target object (with 0 data).
  /*! Dimensions etc are set from the \a dyn_proj_data_sptr and other information set by parsing,
    such as \c zoom, \c output_image_size_z etc.
  */
  TargetT* construct_target_ptr() const override;

  std::unique_ptr<ExamInfo> get_exam_info_uptr_for_target() const override;

protected:
  double actual_compute_objective_function_without_penalty(const TargetT& current_estimate, const int subset_num) override;

  Succeeded set_up_before_sensitivity(shared_ptr<const TargetT> const& target_sptr) override;

  //! Add subset sensitivity to existing data
  /*! \todo Current implementation does NOT add to the subset sensitivity, but overwrites
   */
  void add_subset_sensitivity(TargetT& sensitivity, const int subset_num) const override;

  Succeeded actual_add_multiplication_with_approximate_sub_Hessian_without_penalty(TargetT& output,
                                                                                   const TargetT& input,
                                                                                   const int subset_num) const override;

  Succeeded actual_accumulate_sub_Hessian_times_input_without_penalty(TargetT& output,
                                                                      const TargetT& current_image_estimate,
                                                                      const TargetT& input,
                                                                      const int subset_num) const override;

public:
  /*! \name Functions to get parameters
   \warning Be careful with changing shared pointers. If you modify the objects in
   one place, all objects that use the shared pointer will be affected.
  */
  //@{
  const DynamicProjData& get_dyn_proj_data() const;
  const shared_ptr<DynamicProjData>& get_dyn_proj_data_sptr() const;
  const int get_max_segment_num_to_process() const;
  const bool get_zero_seg0_end_planes() const;
  const DynamicProjData& get_additive_dyn_proj_data() const;
  const shared_ptr<DynamicProjData>& get_additive_dyn_proj_data_sptr() const;
  const ProjectorByBinPair& get_projector_pair() const;
  const shared_ptr<ProjectorByBinPair>& get_projector_pair_sptr() const;
  const BinNormalisation& get_normalisation() const;
  const shared_ptr<BinNormalisation>& get_normalisation_sptr() const;
  //@}

  /*! \name Functions to set parameters
    This can be used as alternative to the parsing mechanism.
   \warning After using any of these, you have to call set_up().
   \warning Be careful with setting shared pointers. If you modify the objects in
   one place, all objects that use the shared pointer will be affected.
  */
  //@{
  void set_recompute_sensitivity(const bool);
  void set_sensitivity_sptr(const shared_ptr<TargetT>&);
  int set_num_subsets(const int num_subsets) override;

  void set_normalisation_sptr(const shared_ptr<BinNormalisation>&) override;
  void set_additive_proj_data_sptr(const shared_ptr<ExamData>&) override;

  void set_input_data(const shared_ptr<ExamData>&) override;
  const DynamicProjData& get_input_data() const override;
  //@}
protected:
  //! Filename with input projection data
  std::string _input_filename;

  //! points to the object for the total input projection data
  shared_ptr<DynamicProjData> _dyn_proj_data_sptr;

  //! the maximum absolute ring difference number to use in the reconstruction
  /*! convention: if -1, use get_max_segment_num()*/
  int _max_segment_num_to_process;

  /**********************/
  ParseAndCreateFrom<TargetT, DynamicProjData> target_parameter_parser;

  /********************************/
  //! name of file in which additive projection data are stored
  std::string _additive_dyn_proj_data_filename;
  //! points to the additive projection data
  /*! the projection data in this file is bin-wise added to forward projection results*/
  shared_ptr<DynamicProjData> _additive_dyn_proj_data_sptr;
  /*! the normalisation or/and attenuation data */
  shared_ptr<BinNormalisation> _normalisation_sptr;
  //! Stores the projectors that are used for the computations
  shared_ptr<ProjectorByBinPair> _projector_pair_ptr;
  //! signals whether to zero the data in the end planes of the projection data
  bool _zero_seg0_end_planes;
  // Patlak Plot Parameters
  /*! the patlak plot pointer where all the parameters are stored */
  shared_ptr<PatlakPlot> _patlak_plot_sptr;
  //! dynamic image template
  DynamicDiscretisedDensity _dyn_image_template;

  bool actual_subsets_are_approximately_balanced(std::string& warning_message) const override;

  void actual_compute_subset_gradient_without_penalty(TargetT& gradient,
                                                      const TargetT& current_estimate,
                                                      const int subset_num,
                                                      const bool add_sensitivity) override;

  //! Sets defaults for parsing
  /*! Resets \c sensitivity_filename and \c sensitivity_sptr and
     \c recompute_sensitivity to \c false.
  */
  void set_defaults() override;
  void initialise_keymap() override;
  bool post_processing() override;
};

END_NAMESPACE_STIR

//#include "stir/recon_buildblock/PoissonLogLikelihoodWithLinearKineticModelAndDynamicProjectionData.inl"

#endif
