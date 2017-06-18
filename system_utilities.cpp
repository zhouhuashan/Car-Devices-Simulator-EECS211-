#include <iostream>
#include <string>
#include "definitions.h"
#include "system_utilities.h"

using namespace std;
extern device* devices[MAX_DEVICES];
extern int numDevices;
extern int globalTime;
extern LIST* eventList;
extern int checkEngine;

int parseCommandLine(string line, string tklist[]) {
    
    char token_terminator = ' ';
    string* accumulator = new string;
    *accumulator = "";
    int i = 0; //the index of the current character we're looking at
    int numTokens = 0; //the number of tokens we've collected so far
    
    if (line.length() > MAX_CMD_LINE_LENGTH)
    {
        return 0;
    }
    
    while (i < line.size()) { //while we have more characters to look at
        if (numTokens > MAX_TOKENS_ON_A_LINE)
        {
            return 0;
        }
        
        if (line[i] == ' ')
        {
            //we've encountered a space, is it a terminating space?
            if (*accumulator != "" && token_terminator == ' ')
            {
                tklist[numTokens] = *accumulator;
                *accumulator = "";
                numTokens++;
            } else if (*accumulator != "") //if not, it might be inside of quotes
            {
                *accumulator += " ";
            }
            i++; //move forward
        }
        else if (line[i] == '\"')
        {
            //we've encountered a quote is it an ending quote?
            if (*accumulator != "")
            {
                tklist[numTokens] = *accumulator;
                *accumulator = "";
                numTokens++;
                token_terminator = ' ';
            }
            else //if not, it must be a starting quote
            {
                token_terminator = '\"';
            }
            i++;
        }
        else { //not a quote or space, so just add the char to accumulator
            *accumulator = *accumulator + line[i];
            i++;
        }
    }
    
    //the loop has finished, so if there's stuff left in the accumulator, add it
    if (*accumulator != "")
    {
        tklist[numTokens] = *accumulator;
        *accumulator = "";
        numTokens++;
    }
    
    delete accumulator;
    
    return numTokens;
}

void print_token_list(int num, string commands[])
{
    cout << "Number of tokens: " << num <<endl;
    for (int k = 0; k < num; k++)
    {
        cout << "token: *" << commands[k] << "*\t";
    }
    cout << endl;
}


command_element* system_commands[NUMBER_OF_COMMANDS];

void fillSystemCommandList() {
    system_commands[0] = new command_element;
    (*system_commands[0]).c	= "halt";
    (*system_commands[0]).cnum	= HALT;
    
    system_commands[1] = new command_element;
    (*system_commands[1]).c	= "status";
    (*system_commands[1]).cnum	= STATUS;
    
    system_commands[2] = new command_element;
    (*system_commands[2]).c	= "time_click";
    (*system_commands[2]).cnum	= TIME_CLICK;
    
    system_commands[3] = new command_element;
    (*system_commands[3]).c	= "create_device";
    (*system_commands[3]).cnum	= CREATE_DEVICE;
    
    system_commands[4] = new command_element;
    (*system_commands[4]).c	= "create_event";
    (*system_commands[4]).cnum	= CREATE_EVENT;
    
    system_commands[5] = new command_element;
    (*system_commands[5]).c	= "set_device_value";
    (*system_commands[5]).cnum	= SET_DEVICE_VALUE;
    
    system_commands[6] = new command_element;
    (*system_commands[6]).c	= "process_events";
    (*system_commands[6]).cnum	= PROCESS_EVENTS;
    
    system_commands[7] = new command_element;
    (*system_commands[7]).c = "malfunction";
    (*system_commands[7]).cnum = MALFUNCTION;
}

void freeSystemCommandList() {
    for (int i=0; i < NUMBER_OF_COMMANDS; i++)
    {
        delete system_commands[i];
    }
}

int getCommandNumber(string s) {
    for(int i=0; i<NUMBER_OF_COMMANDS; i++) {
        if (s == system_commands[i]->c)
            return system_commands[i]->cnum;
    }
    return UNDEFINED_COMMAND;
}


int convertStringToValue(string arg) {
    // Converts the string into an unsigned integer.  No
    // error checking is done. The string is assumed to
    // contain only valid decimal digits.
    int number = 0;
    
    for (int i = 0; i < arg.length(); i++)
    {
        number = number*10 + (arg[i]-'0');
    }
    
    return number;
}

int getDeviceTypeFromString(string dt) {
    if (dt == "digital_sensor")
        return DIGITAL_SENSOR;
    else if (dt == "analog_sensor")
        return ANALOG_SENSOR;
    else if (dt == "digital_controller")
        return DIGITAL_CONTROLLER;
    else if (dt == "analog_controller")
        return ANALOG_CONTROLLER;
    else
        return GENERIC;
}

device* makeDevice(string tokenList[]) {
    int t = getDeviceTypeFromString(tokenList[1]);
    device* dp = NULL;
    switch (t) {
        case DIGITAL_CONTROLLER: {
            dp = new digitalControllerDevice(tokenList[2], convertStringToValue(tokenList[3]));
            break;
        }
        case DIGITAL_SENSOR: {
            int v;
            if (tokenList[4] == "OFF")
                v = OFF;
            else
                v = ON;
            dp = new digitalSensorDevice(tokenList[2], convertStringToValue(tokenList[3]), v);
            break;
        }
        case ANALOG_CONTROLLER: {
            dp = new analogControllerDevice(tokenList[2],
                                            convertStringToValue(tokenList[3]),
                                            convertStringToValue(tokenList[4]),
                                            convertStringToValue(tokenList[5]),
                                            convertStringToValue(tokenList[6]));
            break;
        }
        case ANALOG_SENSOR: {
            dp = new analogSensorDevice(tokenList[2],
                                        convertStringToValue(tokenList[3]),
                                        convertStringToValue(tokenList[4]),
                                        convertStringToValue(tokenList[5]));
            break;
        }
    }
    return dp;
}

void processEvents() {
    EVENT* tmpEvent;
    
    cout << "Processing events at time " << globalTime << "." << endl;
    
    tmpEvent = eventList->getFirstEvent();
    while(tmpEvent!=NULL) {
        // First check if this event is later than the current time.
        if(tmpEvent->getProcessTime()>globalTime) break;
        
        // If not, then process it and move to the next event.
        cout << "Processing event:" << endl;
        tmpEvent->display();
        cout << endl;
        
        //add some new code here to call the appropriate "process" function
        if (tmpEvent->getDeviceName() == "Left Turn Signal"){
            processLeftTurnSignal(tmpEvent);
        }
        else if (tmpEvent->getDeviceName() == "Right Turn Signal"){
            processRightTurnSignal(tmpEvent);
        }
        else if (tmpEvent->getDeviceName() == "Brake"){
            processBrake(tmpEvent);
        }
        else if (tmpEvent->getDeviceName() == "Accelerator"){
            processAccelerator(tmpEvent);
        }
        else if (tmpEvent->getDeviceName() == "Left Turn Lamp"){
            processLeftTurnLamp(tmpEvent);
        }
        else if (tmpEvent->getDeviceName() == "Right Turn Lamp"){
            processRightTurnLamp(tmpEvent);
        }
        else if (tmpEvent->getDeviceName() == "Speedometer"){
            processSpeedometer(tmpEvent);
        }
        else if (tmpEvent->getDeviceName() == "Motor"){
            processMotor(tmpEvent);
        }
        
        eventList->removeFirstEvent();		// Remove the event just processed.
        tmpEvent = eventList->getFirstEvent();	// Get the new first event.
    }
}

int findDevice(string d){
    int index = 0;
    for (int i=0; i<MAX_DEVICES; i ++){
        if (devices[i]->amIThisDevice(d)){
            return index;
        }
        else{
            index++;
        }
    }
    return -1;
}

void processLeftTurnSignal(EVENT* ev){
    if (ev->getValue() == ON){
        devices[findDevice("Left Turn Lamp")] -> setValue(ON);
        EVENT* newEvent = new EVENT("Left Turn Lamp", OFF, globalTime+2);
        eventList -> insertEvent(newEvent);
    }
    else{
        eventList -> removeEventsNamed("Left Turn Lamp");
        devices[findDevice("Left Turn Lamp")] -> setValue(OFF);
    }
}

void processRightTurnSignal(EVENT* ev){
    if (ev->getValue() == ON){
        devices[findDevice("Right Turn Lamp")] -> setValue(ON);
        EVENT* newEvent = new EVENT("Right Turn Lamp", OFF, globalTime+2);
        eventList -> insertEvent(newEvent);
    }
    else{
        eventList -> removeEventsNamed("Right Turn Lamp");
        devices[findDevice("Right Turn Lamp")] -> setValue(OFF);
    }
}

void processBrake(EVENT* ev){
    if (ev -> getValue() == ON){
        devices[findDevice("Brake Lampes")]->setValue(ON);
    }
    else if (ev -> getValue() == OFF){
        devices[findDevice("Brake Lampes")]->setValue(OFF);
    }
}

void processAccelerator(EVENT* ev){
    devices[findDevice("Motor")]->setValue(ev->getValue());
}

void processLeftTurnLamp(EVENT* ev){
    devices[findDevice("Left Turn Lamp")]->setValue(ev->getValue());
    if (ev->getValue() == OFF){
        EVENT* newEventOn = new EVENT("Left Turn Lamp", ON, globalTime+2);
        eventList->insertEvent(newEventOn);
    }
    else if (ev->getValue() == ON){
        EVENT* newEventOff = new EVENT("Left Turn Lamp", OFF, globalTime+2);
        eventList->insertEvent(newEventOff);
    }
}

void processRightTurnLamp(EVENT* ev){
    devices[findDevice("Right Turn Lamp")]->setValue(ev->getValue());
    if (ev->getValue() == OFF){
        EVENT* newEventOn = new EVENT("Right Turn Lamp", ON, globalTime+2);
        eventList->insertEvent(newEventOn);
    }
    else if (ev->getValue() == ON){
        EVENT* newEventOff = new EVENT("Right Turn Lamp", OFF, globalTime+2);
        eventList->insertEvent(newEventOff);
    }
}

void processSpeedometer(EVENT* ev){}
void processMotor(EVENT* ev){}
