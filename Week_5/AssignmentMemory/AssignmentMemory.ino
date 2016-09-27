volatile static char initchar = 'a';    // Initialized global var, should end up in data
volatile static char uninitchar = 0;    // Uninitialized global var, should end up in bss

void setup()
{
  Serial.begin(9600);

  // Local variable and dynamically allocated object for checking down the line.
  volatile char tmp2 = 'a';
  volatile char* tmp = new char('a');
  
  // Retrieve relevant values.
  char stack = 1;
  char* stack_start = &stack;  
  extern char* __data_start;
  extern char* __data_end;
  extern char* __bss_start;
  extern char* __bss_end;
  extern char* __heap_start;
  extern char* __heap_end;
  int heap_end  = (int)stack_start - (int)&__malloc_margin;

  // Print it all to the monitor.
  Serial.print("+----------------+  data_start  = ");  Serial.println((int) &__data_start, HEX);
  Serial.print("+----------------+  data_end    = ");  Serial.println((int) &__data_end, HEX);
  Serial.print("+----------------+  bss_start   = ");  Serial.println((int) &__bss_start, HEX);
  Serial.print("+----------------+  bss_end     = ");  Serial.println((int) &__bss_end, HEX);
  Serial.print("+----------------+  heap_start  = ");  Serial.println((int) &__heap_start, HEX);
  Serial.print("+----------------+  heap_end    = ");  Serial.println((int) heap_end, HEX);
  Serial.print("+----------------+  stack_start = ");  Serial.println((int) stack_start, HEX);
  Serial.print("+----------------+  stack_end   = ");  Serial.println(RAMEND, HEX);

  // Check whether initchar is in data.
  if (&initchar >= &__data_start && &initchar <= &__data_end)
    Serial.println("+----------------+  VERIFIED: initialized global var is in data");
  else
    Serial.println("+----------------+  ERROR: initialized global var is not in data");

  // Check whether uninitchar is in bss.
  if (&uninitchar >= &__bss_start && &uninitchar <= &__bss_end)
    Serial.println("+----------------+  VERIFIED: uninitialized global var is in bss");
  else
    Serial.println("+----------------+  ERROR: uninitialized global var is not in bss");

  // Ensure the dynamically allocated object is in heap.
  if (tmp >= &__heap_start && tmp <= heap_end)
    Serial.println("+----------------+  VERIFIED: dynamically allocated object is in heap");
  else
    Serial.println("+----------------+  ERROR: dynamically allocated object is not in heap");
  
  // Ensure the local variable is on the stack.
  if (&tmp2 >= stack_start && &tmp2 <= RAMEND)
    Serial.println("+----------------+  VERIFIED: local variable is in stack");
  else
    Serial.println("+----------------+  ERROR: local variable is not in stack");

  delete tmp; // Look at me, I'm being totally responsible with my memory management!
}

void loop() { /* Empty. */ }
