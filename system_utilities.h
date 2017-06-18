include <string>
#include "devices.h"
#include "events.h"

using namespace std;

int parseCommandLine(string line, string tklist[]);
void print_token_list(int num, string commands[]);

// Functions that parse and print commands from command line.
void fillSystemCommandList();
void freeSystemCommandList();
int getCommandNumber(string s);
int convertStringToValue(string arg);

int getDeviceTypeFromString(string dt);
device* makeDevice(string tokenList[]);

int findDevice(string d);
void processLeftTurnSignal(EVENT* ev);
void processRightTurnSignal(EVENT* ev);
void processBrake(EVENT* ev);
void processAccelerator(EVENT* ev);
void processLeftTurnLamp(EVENT* ev);
void processRightTurnLamp(EVENT* ev);
void processSpeedometer(EVENT* ev);
void processMotor(EVENT* ev);
void processEvents();

class command_element {
public:
    string c; //the command
    int	cnum; //the command number
};
