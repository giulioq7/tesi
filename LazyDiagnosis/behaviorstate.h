#ifndef BEHAVIORSTATE_H
#define BEHAVIORSTATE_H

#include "astl.h"
#include "transition.h"
#include "nettransition.h"
#include "statedata.h"
#include "systransition.h"

/**
 * @brief The BehaviorState class represents state info of the behavior in lazy diagnosis
 * @date Febbraio 2016
 * @author Giulio Quarenghi
 */
class BehaviorState
{
    /**
     * @brief number
     */
    int number;
    /**
     * @brief marked
     */
    bool marked;
public:
    int n_comps, n_inputs, n_pts, n_intfcs;

    /**
     * @brief S tuple of current states of components
     */
    vector<int>  S;
    /**
     * @brief E current links content
     */
    vector<std::string> E;
    /**
     * @brief P current states of pattern spaces
     */
    vector<int>  P;
    /**
     * @brief I local observation consumption
     */
    int I;
    /**
     * @brief interfaces current states of interfaces involved
     */
    vector<int>  interfaces;

    /**
     * @brief candidate_diagnosis decorated info
     */
    std::set<std::set<std::string> > candidate_diagnosis;

    /**
     * @brief interface_delta interface diagnosis of final states
     */
    std::set<std::set<std::string> > interface_delta;

    /**
     * @brief BehaviorState
     */
    BehaviorState();
    /**
     * @brief BehaviorState
     * @param n
     * @param m
     * @param k
     * @param i
     */
    BehaviorState(int n, int m, int k, int i);
    ~BehaviorState();

    /**
     * @brief empty_terminals
     * @return
     */
    bool empty_terminals();
    /**
     * @brief is_marked
     * @return
     */
    bool is_marked() const;
    /**
     * @brief get_number
     * @return
     */
    int get_number() const{return number;}
    /**
     * @brief mark_state
     */
    void mark_state() { marked = true; }
    /**
     * @brief set_number
     * @param n
     */
    void set_number(int n) { number = n; }
};

std::ostream& operator<<(std::ostream& out, const BehaviorState& s);

#endif // BEHAVIORSTATE_H
