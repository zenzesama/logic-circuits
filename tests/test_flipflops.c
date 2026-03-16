/**
 * @file test_flipflops.c
 * @author zenzesama (zenzesama@gmail.com)
 * @brief Unit tests for RS, D, edge-triggered D, and edge-triggered D with preset/clear.
 * @version 0.1
 * @date 2026-03-15
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "../logic.h"
#include "test_framework.h"
 
static void test_rs_flip_flop(void) {
    rs_flip_flop ff = newRsFlipFlop();
 
    /* Initial state: Q=0, Q_bar=1 */
    ASSERT_BIT_EQ(LOW,  ff.q,     "RS init Q");
    ASSERT_BIT_EQ(HIGH, ff.q_bar, "RS init Q_bar");
 
    /* Set: S=1, R=0 -> Q=1 */
    rsFlipFlop(&ff, LOW, HIGH);
    ASSERT_BIT_EQ(HIGH, ff.q,     "RS set Q");
    ASSERT_BIT_EQ(LOW,  ff.q_bar, "RS set Q_bar");
 
    /* Hold: S=0, R=0 -> no change */
    rsFlipFlop(&ff, LOW, LOW);
    ASSERT_BIT_EQ(HIGH, ff.q,     "RS hold Q");
    ASSERT_BIT_EQ(LOW,  ff.q_bar, "RS hold Q_bar");
 
    /* Reset: R=1, S=0 -> Q=0 */
    rsFlipFlop(&ff, HIGH, LOW);
    ASSERT_BIT_EQ(LOW,  ff.q,     "RS reset Q");
    ASSERT_BIT_EQ(HIGH, ff.q_bar, "RS reset Q_bar");
}
 
static void test_d_flip_flop(void) {
    d_flip_flop ff = newDFlipFlop();
 
    /* Clock LOW: data should not be latched */
    dFlipFlop(&ff, HIGH, LOW);
    ASSERT_BIT_EQ(LOW, ff.rs_ff.q, "D latch clock-LOW no change");
 
    /* Clock HIGH, D=1: latch 1 */
    dFlipFlop(&ff, HIGH, HIGH);
    ASSERT_BIT_EQ(HIGH, ff.rs_ff.q, "D latch D=1");
 
    /* Clock HIGH, D=0: latch 0 */
    dFlipFlop(&ff, LOW, HIGH);
    ASSERT_BIT_EQ(LOW, ff.rs_ff.q, "D latch D=0");
}
 
static void test_edge_triggered_d_flip_flop(void) {
    edge_triggered_d_flip_flop ff = newEdgeTriggeredDFlipFlop();
 
    /* Present D=1, no clock edge yet: output unchanged */
    edgeTriggeredDFlipFlop(&ff, HIGH, LOW);
    ASSERT_BIT_EQ(LOW, ff.slave.rs_ff.q, "ET-D no edge Q stays 0");
 
    /* Rising edge with D=1: output captures 1 */
    edgeTriggeredDFlipFlop(&ff, HIGH, HIGH);
    ASSERT_BIT_EQ(HIGH, ff.slave.rs_ff.q, "ET-D rising edge D=1");
 
    /* Clock stays HIGH (no new edge): output unchanged even if D changes */
    edgeTriggeredDFlipFlop(&ff, LOW, HIGH);
    ASSERT_BIT_EQ(HIGH, ff.slave.rs_ff.q, "ET-D no edge on sustained clock");
 
    /* Clock goes LOW, then HIGH again (new rising edge) with D=0 */
    edgeTriggeredDFlipFlop(&ff, LOW, LOW);
    edgeTriggeredDFlipFlop(&ff, LOW, HIGH);
    ASSERT_BIT_EQ(LOW, ff.slave.rs_ff.q, "ET-D rising edge D=0");
}
 
static void test_edge_triggered_d_flip_flop_pc(void) {
    edge_triggered_d_flip_flop_pc ff = newEdgeTriggeredDFlipFlopPC();
 
    /* Preset: forces Q=1 regardless of D or clock */
    edgeTriggeredDFlipFlopPC(&ff, LOW, LOW, HIGH, LOW);
    ASSERT_BIT_EQ(HIGH, ff.q,     "PC preset Q=1");
    ASSERT_BIT_EQ(LOW,  ff.q_bar, "PC preset Q_bar=0");
 
    /* Clear: forces Q=0 regardless of D or clock */
    edgeTriggeredDFlipFlopPC(&ff, HIGH, HIGH, LOW, HIGH);
    ASSERT_BIT_EQ(LOW,  ff.q,     "PC clear Q=0");
    ASSERT_BIT_EQ(HIGH, ff.q_bar, "PC clear Q_bar=1");
 
    /* Normal rising edge with D=1 */
    edgeTriggeredDFlipFlopPC(&ff, HIGH, LOW,  LOW, LOW);
    edgeTriggeredDFlipFlopPC(&ff, HIGH, HIGH, LOW, LOW);
    ASSERT_BIT_EQ(HIGH, ff.q, "PC normal D=1 rising edge");
 
    /* Normal rising edge with D=0 */
    edgeTriggeredDFlipFlopPC(&ff, LOW, LOW,  LOW, LOW);
    edgeTriggeredDFlipFlopPC(&ff, LOW, HIGH, LOW, LOW);
    ASSERT_BIT_EQ(LOW, ff.q, "PC normal D=0 rising edge");
 
    /* Both preset and clear HIGH: state should be unchanged (illegal state, no-op) */
    bit q_before = ff.q;
    edgeTriggeredDFlipFlopPC(&ff, LOW, LOW, HIGH, HIGH);
    ASSERT_BIT_EQ(q_before, ff.q, "PC both preset+clear HIGH is no-op");
}
 
int main(void) {
    test_rs_flip_flop();
    test_d_flip_flop();
    test_edge_triggered_d_flip_flop();
    test_edge_triggered_d_flip_flop_pc();
    PRINT_SUMMARY("test_flipflops");
    return _tests_failed;
}