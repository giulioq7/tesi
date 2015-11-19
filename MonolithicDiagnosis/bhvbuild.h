#ifndef BHVBUILD_H
#define BHVBUILD_H

#include <astl.h>

class BhvBuild
{
public:
    BhvBuild();

    template <class SIGMA, class TAG>
    typename astl::DFA_map<SIGMA,TAG>::state_type make_state();
};

template <class SIGMA, class TAG>
typename astl::DFA_map<SIGMA,TAG>::state_type BhvBuild::make_state()
{

}

#endif // BHVBUILD_H
