/**
 * @file flipFlops.c
 * @author zenze (zenzesama@gmail.com)
 * @brief this file contains all the logic for each flipflop
 * @version 0.1
 * @date 2026-02-08
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "logic.h"

/**
 * @brief this initializes a new rsFlipFlop with the default value of q = low and ~q = high
 * 
 * @return rs_flip_flop this is the returned rs flip flop
 */
rs_flip_flop newRsFlipFlop(){
    rs_flip_flop ff = {LOW, HIGH};
    return ff;
}

/**
 * @brief this simuates a single tick / change of input on the flip flop
 * 
 * @param ff this is the pointer to the flip flop whose state is to be changed
 * @param r this is the new r value that is passed
 * @param s this is the new s value that is passed
 * 
 * q and q_bar represents the new candidate values computed
 * the loop iteratively updates the flip-flop state until it reaches equilibrium
 * simulating the propagation delay through the cross-coupled NOR gates
 * 
 * then in the if statement,
 * if the new calculated values match the old ones, we have reached equilibrium so exit loop
 * if not, then update the flip-flop's stored state and try again
 */
void rsFlipFlop(rs_flip_flop* ff, bit r, bit s){
    bit q, q_bar;
    do{
        q = norGate(r, ff->q_bar);
        q_bar = norGate(s, q);
        if(q == ff->q && q_bar == ff->q_bar) break;
        ff->q = q;
        ff->q_bar = q_bar;
    }while(1);
}

/**
 * @brief this initializes a new d type flip flop usind the previously defined rs flipflop with a defailt value of q = low and ~q = high
 * 
 * @return d_flip_flop this is the returned d - flip flop
 */
d_flip_flop newDFlipFlop(){
    d_flip_flop ff;
    ff.rs_ff = newRsFlipFlop();
    return ff;
}

/**
 * @brief this simuates a single tick / change of input on the flip flop
 * 
 * @param ff this is the pointer to the flip flop whose state is to be changed
 * @param data this is the new data input that is passed
 * @param clock this is the new clock input that is passed
 * 
 * this is implemented by extending the rs flip flop logic.
 * We pass the data and clock input to the to a and gate and make it the s of the rs flip flop.
 * We pass in  the clock and not of the data to a and gate and make it the r of the rs flip flop
 * then we return the new state of the d flip flop
 */
void dFlipFlop(d_flip_flop* ff, bit data, bit clock){
    bit s = andGate(clock, data);
    bit r = andGate(clock, notGate(data));

    rsFlipFlop(&ff->rs_ff, r, s);
}