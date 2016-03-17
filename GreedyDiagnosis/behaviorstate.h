#ifndef BEHAVIORSTATE_H
#define BEHAVIORSTATE_H

#include "astl.h"
#include "transition.h"
#include "nettransition.h"
#include "statedata.h"
#include "systransition.h"

/**
 * @brief The BehaviorState class represents state info in greedy diagnosis
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
     * @brief marked state processed
     */
    bool marked;
public:
    int n_comps, n_inputs, n_pts, n_isp;

    /**
     * @brief S tuple of component current states
     */
    vector<int>  S;
    /**
     * @brief E tuple of current input terminals content
     */
    vector<std::string> E;
    /**
     * @brief P tuple of pattern spaces current states
     */
    vector<int>  P;
    /**
     * @brief I tuple of current index reached in local nodes observations
     */
    vector<int>  I;

    /**
     * @brief candidate_diagnosis decorated info
     */
    std::set<std::set<std::string> > candidate_diagnosis;

    /**
     * @brief BehaviorState
     */
    BehaviorState();
    /**
     * @brief BehaviorState constructor from number of components, input terminals, pattern spaces and nodes
     * @param n
     * @param m
     * @param k
     * @param i
     */
    BehaviorState(int n, int m, int k, int i);
    ~BehaviorState();

    BehaviorState copy();
    /**
     * @brief set_E
     * @param terms
     */
    void set_E(vector<Terminal*> terms);
    /**
     * @brief empty_terminals checks if all input terminals are empty
     * @return true if all links empty, false otherwise
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
