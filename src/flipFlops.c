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

#include "../include/logic.h"

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
 * @brief this simulates a single tick / change of input on the flip flop
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
 * @brief this initializes a new d type flip flop using the previously defined rs flipflop with a default value of q = low and ~q = high
 * 
 * @return d_flip_flop this is the returned d - flip flop
 */
d_flip_flop newDFlipFlop(){
    d_flip_flop ff;
    ff.rs_ff = newRsFlipFlop();
    return ff;
}

/**
 * @brief this simulates a single tick / change of input on the flip flop
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

/**
 * @brief this initializes a new edge-triggered D flip-flop with default values
 * 
 * @return edge_triggered_d_flip_flop this is the returned edge-triggered D flip-flop
 * 
 * uses a master slave configuration with two D-flip flops
 * tracks the last clock state
 */
edge_triggered_d_flip_flop newEdgeTriggeredDFlipFlop(void){
    edge_triggered_d_flip_flop ff;
    ff.master = newDFlipFlop();
    ff.slave = newDFlipFlop();
    ff.last_clock = LOW;
    return ff;
}

/**
 * @brief simulates an edge-triggered D flip-flop
 * 
 * @param ff pointer to the edge-triggered flip-flop
 * @param data the data input
 * @param clock the clock signal
 * 
 * this implements true edge-triggered behavior by detecting rising edges
 * the flip-flop only updates its output when clock transitions from LOW to HIGH
 * the master-slave configuration ensures the output is stable
 */
void edgeTriggeredDFlipFlop(edge_triggered_d_flip_flop* ff, bit data, bit clock){
    // Detect rising edge (transition from LOW to HIGH)
    if(ff->last_clock == LOW && clock == HIGH){
        // On rising edge: capture data through master-slave chain
        // Master captures the input data
        dFlipFlop(&ff->master, data, HIGH);
        
        // Slave captures master's output
        bit master_output = ff->master.rs_ff.q;
        dFlipFlop(&ff->slave, master_output, HIGH);
    }
    
    // Update the last clock state for next edge detection
    ff->last_clock = clock;
}

/**
 * @brief initializes a new edge-triggered D flip-flop with preset and clear
 * 
 * @return edge_triggered_d_flip_flop_pc the initialized flip-flop
 * 
 * this creates a flip-flop with preset and clear functionality
 * preset sets Q to 1, clear sets Q to 0
 */
edge_triggered_d_flip_flop_pc newEdgeTriggeredDFlipFlopPC(void){
    edge_triggered_d_flip_flop_pc ff;
    ff.base_ff = newEdgeTriggeredDFlipFlop();
    ff.q = LOW;
    ff.q_bar = HIGH;
    return ff;
}

/**
 * @brief simulates an edge-triggered D flip-flop with preset and clear
 * 
 * @param ff pointer to the flip-flop
 * @param data the data input
 * @param clock the clock signal
 * @param preset when HIGH (1), forces Q to 1
 * @param clear when HIGH (1), forces Q to 0
 * 
 * Truth table from the book:
 * Pre | Clr | D | Clk | Q | Q_bar
 * ----|-----|---|-----|---|-------
 *  1  |  0  | X |  X  | 1 |   0
 *  0  |  1  | X |  X  | 0 |   1
 *  0  |  0  | 0 |  ↑  | 0 |   1
 *  0  |  0  | 1 |  ↑  | 1 |   0
 *  0  |  0  | X |  0  | Q |   Q_bar (no change)
 * 
 * Preset and Clear override the Clock and Data inputs
 * Preset and Clear should not both be 1 at the same time, we just ignore that state and keep the previous values
 * 
 * the q and q_bar fields are updated to reflect the current state
 */
void edgeTriggeredDFlipFlopPC(edge_triggered_d_flip_flop_pc* ff, bit data, bit clock, bit preset, bit clear){
    if(preset == HIGH && clear == LOW){
        // Force Q to 1, Q_bar to 0
        ff->base_ff.slave.rs_ff.q = HIGH;
        ff->base_ff.slave.rs_ff.q_bar = LOW;
        ff->base_ff.master.rs_ff.q = HIGH;
        ff->base_ff.master.rs_ff.q_bar = LOW;
        ff->q = HIGH;
        ff->q_bar = LOW;
    }
    else if(clear == HIGH && preset == LOW){
        // Force Q to 0, Q_bar to 1
        ff->base_ff.slave.rs_ff.q = LOW;
        ff->base_ff.slave.rs_ff.q_bar = HIGH;
        ff->base_ff.master.rs_ff.q = LOW;
        ff->base_ff.master.rs_ff.q_bar = HIGH;
        ff->q = LOW;
        ff->q_bar = HIGH;
    }
    else if(preset == LOW && clear == LOW){
        // Normal edge-triggered operation
        edgeTriggeredDFlipFlop(&ff->base_ff, data, clock);
        ff->q = ff->base_ff.slave.rs_ff.q;
        ff->q_bar = ff->base_ff.slave.rs_ff.q_bar;
    }
}