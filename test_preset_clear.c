#include "logic.h"
int printf(const char* format, ...);

int main(){
    printf("\n*** EDGE-TRIGGERED D FLIP-FLOP WITH PRESET AND CLEAR TEST ***\n");
    
    edge_triggered_d_flip_flop_pc ff = newEdgeTriggeredDFlipFlopPC();
    
    // Test 1: Preset functionality (Pre=1, Clr=0)
    printf("\n=== Test 1: Preset (should set Q=1) ===\n");
    edgeTriggeredDFlipFlopPC(&ff, LOW, LOW, HIGH, LOW);
    printf("Pre=1, Clr=0: Q=%d, Q_bar=%d\n", ff.q, ff.q_bar);
    
    // Test 2: Clear functionality (Pre=0, Clr=1)
    printf("\n=== Test 2: Clear (should set Q=0) ===\n");
    edgeTriggeredDFlipFlopPC(&ff, LOW, LOW, LOW, HIGH);
    printf("Pre=0, Clr=1: Q=%d, Q_bar=%d\n", ff.q, ff.q_bar);
    
    // Test 3: Normal operation - rising edge with D=1
    printf("\n=== Test 3: Normal operation (Pre=0, Clr=0, D=1, rising edge) ===\n");
    edgeTriggeredDFlipFlopPC(&ff, HIGH, LOW, LOW, LOW);  // Clock LOW
    printf("Before rising edge: Q=%d\n", ff.q);
    edgeTriggeredDFlipFlopPC(&ff, HIGH, HIGH, LOW, LOW); // Clock HIGH (rising edge)
    printf("After rising edge: Q=%d\n", ff.q);
    
    // Test 4: Normal operation - rising edge with D=0
    printf("\n=== Test 4: Normal operation (D=0, rising edge) ===\n");
    edgeTriggeredDFlipFlopPC(&ff, LOW, LOW, LOW, LOW);   // Clock LOW
    printf("Before rising edge: Q=%d\n", ff.q);
    edgeTriggeredDFlipFlopPC(&ff, LOW, HIGH, LOW, LOW);  // Clock HIGH (rising edge)
    printf("After rising edge: Q=%d\n", ff.q);
    
    // Test 5: Preset overrides data
    printf("\n=== Test 5: Preset overrides D input ===\n");
    edgeTriggeredDFlipFlopPC(&ff, LOW, HIGH, HIGH, LOW); // D=0, Clk=1, but Pre=1
    printf("Pre=1 with D=0: Q=%d (should be 1)\n", ff.q);
    
    // Test 6: Clear overrides data
    printf("\n=== Test 6: Clear overrides D input ===\n");
    edgeTriggeredDFlipFlopPC(&ff, HIGH, HIGH, LOW, HIGH); // D=1, Clk=1, but Clr=1
    printf("Clr=1 with D=1: Q=%d (should be 0)\n", ff.q);
    
    // Test 7: Timing diagram simulation
    printf("\n=== Test 7: Timing diagram (multiple clock cycles) ===\n");
    printf("Clk | Pre | Clr | D | Q\n");
    printf("----|-----|-----|---|---\n");
    
    // Reset with clear
    edgeTriggeredDFlipFlopPC(&ff, LOW, LOW, LOW, HIGH);
    printf(" -  |  0  |  1  | - | %d\n", ff.q);
    
    // Clock cycle 1: D=1
    edgeTriggeredDFlipFlopPC(&ff, HIGH, LOW, LOW, LOW);
    printf(" 0  |  0  |  0  | 1 | %d\n", ff.q);
    edgeTriggeredDFlipFlopPC(&ff, HIGH, HIGH, LOW, LOW);
    printf(" 1  |  0  |  0  | 1 | %d\n", ff.q);
    
    // Clock cycle 2: D=0
    edgeTriggeredDFlipFlopPC(&ff, LOW, LOW, LOW, LOW);
    printf(" 0  |  0  |  0  | 0 | %d\n", ff.q);
    edgeTriggeredDFlipFlopPC(&ff, LOW, HIGH, LOW, LOW);
    printf(" 1  |  0  |  0  | 0 | %d\n", ff.q);
    
    // Clock cycle 3: Preset
    edgeTriggeredDFlipFlopPC(&ff, LOW, LOW, HIGH, LOW);
    printf(" -  |  1  |  0  | - | %d\n", ff.q);
    
    printf("\n*** ALL TESTS COMPLETE ***\n\n");
    return 0;
}