#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int level = 0;  // Αρχικόσ όροφοσ του ανελκυςτήρα
int x = 0;      // Flag λογικήσ για έλεγχο του κύριου βρόχου

// Διακοπή για την ανίχνευςη πατήματοσ κουμπιών
ISR(PORTF_PORT_vect) {
	// Και τα δύο κουμπιά (Up και Down) πατήθηκαν ταυτόχρονα
	if ((PORTF.INTFLAGS & 0b01000000) != 0 && (PORTF.INTFLAGS & 0b00100000) != 0) {
		PORTD.OUT &= 0b11111110;  // Ανάβει το LED ςφάλματοσ (PIN0)
		_delay_ms(del);
		PORTD.OUT |= 0b00000001;  // Σβήνει το LED ςφάλματοσ (PIN0)
		_delay_ms(del);
	}
	
	// Αν πατηθεί το κουμπί κάτω (PIN6) και δεν είμαςτε ςτον 0ο όροφο
	if ((PORTF.INTFLAGS & 0b01000000) != 0 && level > 0) {
		level--;  // Κατεβαίνουμε όροφο
	}
	// Αν πατηθεί το κουμπί πάνω (PIN5) και δεν είμαςτε ςτον 2ο όροφο
	else if ((PORTF.INTFLAGS & 0b00100000) != 0 && level < 2) {
		level++;  // Ανεβαίνουμε όροφο
	}
	
	// Ενημέρωςη LEDs απευθείασ μέςα ςτην ISR
	PORTD.OUT |= 0b00000111;  // Σβήνουμε όλα τα LEDs
	if (level == 1) {
		PORTD.OUTCLR = 0b00000010;  // Ανάβει το LED για τον 1ο όροφο
	}
	else if (level == 2) {
		PORTD.OUTCLR = 0b00000110;  // Ανάβουν τα LED για τον 2ο όροφο
	}
	
	// Καθαριςμόσ των interrupt flags
	int y = PORTF.INTFLAGS;
	PORTF.INTFLAGS = y;
}

int main() {
	// Οριςμόσ των LED pins ωσ έξοδοι
	PORTD.DIR |= 0b00000111;  // PIN0 (Error), PIN1, PIN2 ωσ έξοδοι
	PORTD.OUT |= 0b00000111;  // Σβήνουμε όλα τα LEDs αρχικά
	
	// Ενεργοποίηςη pull-ups και ρύθμιςη διακοπών ςτα PIN5 και PIN6
	PORTF.PIN5CTRL = PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;
	PORTF.PIN6CTRL = PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;
	
	sei();  // Ενεργοποίηςη παγκόςμιων διακοπών
	
	// Κύριοσ βρόχοσ: περιμένει διακοπή
	while (x == 0) {
		// Δεν κάνει τίποτα
	}
	
	cli();  // Απενεργοποίηςη διακοπών
	return 0;
}
