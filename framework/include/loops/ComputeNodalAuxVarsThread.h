//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef COMPUTENODALAUXVARSTHREAD_H
#define COMPUTENODALAUXVARSTHREAD_H

// MOOSE includes
#include "ThreadedNodeLoop.h"

#include "libmesh/node_range.h"

// Forward declarations
class AuxiliarySystem;
class FEProblemBase;
template <typename T>
class MooseObjectWarehouse;

template <typename AuxKernelType>
class ComputeNodalAuxVarsThread
  : public ThreadedNodeLoop<ConstNodeRange, ConstNodeRange::const_iterator>
{
public:
  ComputeNodalAuxVarsThread(FEProblemBase & fe_problem,
                            const MooseObjectWarehouse<AuxKernelType> & storage,
                            const std::vector<std::map<std::string, MooseVariableFEBase *>> & vars);
  // Splitting Constructor
  ComputeNodalAuxVarsThread(ComputeNodalAuxVarsThread & x, Threads::split split);

  void onNode(ConstNodeRange::const_iterator & nd) override;

  void join(const ComputeNodalAuxVarsThread & /*y*/);

  void subdomainChanged();

  void post() override;

protected:
  AuxiliarySystem & _aux_sys;

  /// Storage object containing active AuxKernel objects
  const MooseObjectWarehouse<AuxKernelType> & _storage;

  const std::vector<std::map<std::string, MooseVariableFEBase *>> _aux_vars;

  std::set<SubdomainID> _block_ids;
};

#endif // COMPUTENODALAUXVARSTHREAD_H
