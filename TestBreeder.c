/*
Pokemon Sword & Shield softlockHoster 

Based on the LUFA library's Low-Level Joystick Demo
	(C) Dean Camera
Based on the HORI's Pokken Tournament Pro Pad design
	(C) HORI

This project implements a modified version of HORI's Pokken Tournament Pro Pad
USB descriptors to allow for the creation of custom controllers for the
Nintendo Switch. This also works to a limited degree on the PS3.

Since System Update v3.0.0, the Nintendo Switch recognizes the Pokken
Tournament Pro Pad as a Pro Controller. Physical design limitations prevent
the Pokken Controller from functioning at the same level as the Pro
Controller. However, by default most of the descriptors are there, with the
exception of Home and Capture. Descriptor modification allows us to unlock
these buttons for our use.
*/

#include "Joystick.h"

// ------------instructions------------
// - Your cursor must be over the Pokemon Camp option in the menu
// - You must be standing in front of the den, facing it
// - Plug in board at Change Grip/Order menu
// - :zensuck:

static const Command softlockHoster[] = {	
	//----------Setup [0,5]----------
	// Connect controller in Change Grip/Order
	{NOTHING, 30},
	{TRIGGERS, 30},
	{A, 40},
	{B, 40},
	{HOME, 1},
	{NOTHING, 30},
	
	//  Go to Route 5 Nursery [6,13]
	{X, 30},
	{A, 100},
	{UP_RIGHT, 1}, {NOTHING, 1},
	{A, 20}, {NOTHING, 1},
	{A, 150}, {NOTHING, 1},
	
	// Take Egg from breeder [14,35]
	{UP_LEFT, 30}, {NOTHING, 15},
	{A, 5}, {NOTHING, 40},
	{A, 10}, {NOTHING, 150},
	{A, 5}, {NOTHING, 70},
	{A, 5}, {NOTHING, 50},
	{A, 5}, {NOTHING, 70},
	{DOWN, 5}, {NOTHING, 5},
	{A, 5}, {NOTHING, 100},
	{A, 5}, {NOTHING, 70}, 
	{A, 5}, {NOTHING, 5},
	{DOWN, 20}, {NOTHING, 2},

	// start riding [36,59]
	{PLUS, 5}, {NOTHING, 10},
	{RIGHT, 210},
	{RIGHT, 150}, {B, 5}, {RIGHT, 150}, 
	{LEFT, 150}, {B, 5}, {LEFT, 150},
	{RIGHT, 150}, {B, 5}, {RIGHT, 150}, 
	{LEFT, 150}, {B, 5}, {LEFT, 150},
	{RIGHT, 150}, {B, 5}, {RIGHT, 150}, 
	{LEFT, 150}, {B, 5}, {LEFT, 150},
	{RIGHT, 150}, {B, 5}, {RIGHT, 150}, 
	
	//Egg Hatched [60,68]
	{NOTHING, 15},
	{A, 5}, {NOTHING, 800},
	{A, 5}, {NOTHING, 150},
	{B, 5}, {NOTHING, 20},
	{PLUS, 5}, {NOTHING, 5},
};


// Main entry point.
int main(void) {
	// We'll start by performing hardware and peripheral setup.
	SetupHardware();
	// We'll then enable global interrupts for our use.
	GlobalInterruptEnable();
	// Once that's done, we'll enter an infinite loop.
	for (;;)
	{
		// We need to run our task to process and deliver data for our IN and OUT endpoints.
		HID_Task();
		// We also need to run the main USB management task.
		USB_USBTask();
	}
}

// Configures hardware and peripherals, such as the USB peripherals.
void SetupHardware(void) {
	// We need to disable watchdog if enabled by bootloader/fuses.
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	// We need to disable clock division before initializing the USB hardware.
	//clock_prescale_set(clock_div_1);
	// We can then initialize our hardware and peripherals, including the USB stack.

	#ifdef ALERT_WHEN_DONE
	// Both PORTD and PORTB will be used for the optional LED flashing and buzzer.
	#warning LED and Buzzer functionality enabled. All pins on both PORTB and \
PORTD will toggle when printing is done.
	DDRD  = 0xFF; //Teensy uses PORTD
	PORTD =  0x0;
                  //We'll just flash all pins on both ports since the UNO R3
	DDRB  = 0xFF; //uses PORTB. Micro can use either or, but both give us 2 LEDs
	PORTB =  0x0; //The ATmega328P on the UNO will be resetting, so unplug it?
	#endif
	// The USB stack should be initialized last.
	USB_Init();
}

// Fired to indicate that the device is enumerating.
void EVENT_USB_Device_Connect(void) {
	// We can indicate that we're enumerating here (via status LEDs, sound, etc.).
}

// Fired to indicate that the device is no longer connected to a host.
void EVENT_USB_Device_Disconnect(void) {
	// We can indicate that our device is not ready (via status LEDs, sound, etc.).
}

// Fired when the host set the current configuration of the USB device after enumeration.
void EVENT_USB_Device_ConfigurationChanged(void) {
	bool ConfigSuccess = true;

	// We setup the HID report endpoints.
	ConfigSuccess &= Endpoint_ConfigureEndpoint(JOYSTICK_OUT_EPADDR, EP_TYPE_INTERRUPT, JOYSTICK_EPSIZE, 1);
	ConfigSuccess &= Endpoint_ConfigureEndpoint(JOYSTICK_IN_EPADDR, EP_TYPE_INTERRUPT, JOYSTICK_EPSIZE, 1);

	// We can read ConfigSuccess to indicate a success or failure at this point.
}

// Process control requests sent to the device from the USB host.
void EVENT_USB_Device_ControlRequest(void) {
	// We can handle two control requests: a GetReport and a SetReport.

	// Not used here, it looks like we don't receive control request from the Switch.
}

// Process and deliver data from IN and OUT endpoints.
void HID_Task(void) {
	// If the device isn't connected and properly configured, we can't do anything here.
	if (USB_DeviceState != DEVICE_STATE_Configured)
		return;

	// We'll start with the OUT endpoint.
	Endpoint_SelectEndpoint(JOYSTICK_OUT_EPADDR);
	// We'll check to see if we received something on the OUT endpoint.
	if (Endpoint_IsOUTReceived())
	{
		// If we did, and the packet has data, we'll react to it.
		if (Endpoint_IsReadWriteAllowed())
		{
			// We'll create a place to store our data received from the host.
			USB_JoystickReport_Output_t JoystickOutputData;
			// We'll then take in that data, setting it up in our storage.
			while(Endpoint_Read_Stream_LE(&JoystickOutputData, sizeof(JoystickOutputData), NULL) != ENDPOINT_RWSTREAM_NoError);
			// At this point, we can react to this data.

			// However, since we're not doing anything with this data, we abandon it.
		}
		// Regardless of whether we reacted to the data, we acknowledge an OUT packet on this endpoint.
		Endpoint_ClearOUT();
	}

	// We'll then move on to the IN endpoint.
	Endpoint_SelectEndpoint(JOYSTICK_IN_EPADDR);
	// We first check to see if the host is ready to accept data.
	if (Endpoint_IsINReady())
	{
		// We'll create an empty report.
		USB_JoystickReport_Input_t JoystickInputData;
		// We'll then populate this report with what we want to send to the host.
		GetNextReport(&JoystickInputData);
		// Once populated, we can output this data to the host. We do this by first writing the data to the control stream.
		while(Endpoint_Write_Stream_LE(&JoystickInputData, sizeof(JoystickInputData), NULL) != ENDPOINT_RWSTREAM_NoError);
		// We then send an IN packet on this endpoint.
		Endpoint_ClearIN();
	}
}

typedef enum {
	PROCESS,
	DONE
} State_t;
State_t state = PROCESS;

#define ECHOES 2
int echoes = 0;
USB_JoystickReport_Input_t last_report;

int xpos = 0;
int ypos = 0;
int portsval = 0;

int durationCount = 0;

// setup
int commandIndex = 0;
int m_endIndex = 5;

// Prepare the next report for the host.
void GetNextReport(USB_JoystickReport_Input_t* const ReportData) {

	// Prepare an empty report
	memset(ReportData, 0, sizeof(USB_JoystickReport_Input_t));
	ReportData->LX = STICK_CENTER;
	ReportData->LY = STICK_CENTER;
	ReportData->RX = STICK_CENTER;
	ReportData->RY = STICK_CENTER;
	ReportData->HAT = HAT_CENTER;

	// Repeat ECHOES times the last report
	if (echoes > 0)
	{
		memcpy(ReportData, &last_report, sizeof(USB_JoystickReport_Input_t));
		echoes--;
		return;
	}

	// States and moves management
	switch (state)
	{
		case PROCESS:
			if (commandIndex == -1)
			{
				commandIndex = 5;
				m_endIndex = 68;
			}
			
			switch (softlockHoster[commandIndex].button)
			{
				case UP:
					ReportData->LY = STICK_MIN;				
					break;

				case LEFT:
					ReportData->LX = STICK_MIN;				
					break;

				case DOWN:
					ReportData->LY = STICK_MAX;				
					break;

				case RIGHT:
					ReportData->LX = STICK_MAX;				
					break;

				case UP_LEFT:
					ReportData->LX = STICK_MIN;
					ReportData->LY = STICK_MIN;
					break;	
			
				case UP_RIGHT:
					ReportData->LX = STICK_MAX;
					ReportData->LY = STICK_MIN;
					break;	
					
				case B_RIGHT:
					ReportData->LY = STICK_MAX;
					ReportData->Button |= SWITCH_R;
					break;
					
				case B_LEFT:
					ReportData->LY = STICK_MIN;
					ReportData->Button |= SWITCH_R;
					break;
					
				case X:
					ReportData->Button |= SWITCH_X;
					break;

				case Y:
					ReportData->Button |= SWITCH_Y;
					break;

				case A:
					ReportData->Button |= SWITCH_A;
					break;

				case B:
					ReportData->Button |= SWITCH_B;
					break;

				case L:
					ReportData->Button |= SWITCH_L;
					break;

				case R:
					ReportData->Button |= SWITCH_R;
					break;

				case ZL:
					ReportData->Button |= SWITCH_ZL;
					break;

				case ZR:
					ReportData->Button |= SWITCH_ZR;
					break;

				case MINUS:
					ReportData->Button |= SWITCH_MINUS;
					break;

				case PLUS:
					ReportData->Button |= SWITCH_PLUS;
					break;

				case LCLICK:
					ReportData->Button |= SWITCH_LCLICK;
					break;

				case RCLICK:
					ReportData->Button |= SWITCH_RCLICK;
					break;

				case TRIGGERS:
					ReportData->Button |= SWITCH_L | SWITCH_R;
					break;

				case HOME:
					ReportData->Button |= SWITCH_HOME;
					break;

				case CAPTURE:
					ReportData->Button |= SWITCH_CAPTURE;
					break;
					
				case BACKUP:
					ReportData->LY = STICK_MIN;	
					ReportData->Button |= SWITCH_X | SWITCH_B;
					break;	

				default:
					break;
			}

			durationCount++;

			if (durationCount > softlockHoster[commandIndex].duration)
			{
				commandIndex++;
				durationCount = 0;		

				// We reached the end of a command sequence
				if (commandIndex > m_endIndex)
				{
					commandIndex = -1;
				}		
			}

			break;

		case DONE: return;
	}

	// Prepare to echo this report
	memcpy(&last_report, ReportData, sizeof(USB_JoystickReport_Input_t));
	echoes = ECHOES;
}