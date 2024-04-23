#include <LPC210x.H>  
#include <stdint.h>	

#include "planificador.h"
#include "test.h"
#include "watchdog.h"

//MAIN PR2B
  int main() {
	//test_all();
	//test_watchdog();
	
	planificador();
}
