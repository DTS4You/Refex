//##################################################################
//### Stripe Define                                              ###
//##################################################################

#ifndef STRIPE_INIT_H
#define STRIPE_INIT_H
#endif

// LED Anordnung
#ifndef DEBUG_STRIPE

// 1. DDB-Modul 
#define STRIPE_01_DDB       0
#define STRIPE_01_START     0
#define STRIPE_01_NUM       100  

#else

// Debug Aufbau
#define STRIPE_01_DDB        0
#define STRIPE_01_START      0
#define STRIPE_01_NUMBER     16           // Test

#endif
