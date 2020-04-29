#ifndef IrrigationSerialProtocol_h
#define IrrigationSerialProtocol_h

// Payload example:
// ST10|CH01|CT1258|ET0325|RT0135

// Constraints
const int NUMBER_OF_CHANNELS = 9;
const int NUMBER_OF_SEGMENTS = 5;
const int PAYLOAD_LENGTH = 30;

const char* SEPARATOR = "|";

// ********* Markers ***********

const char M_STATE []           = "ST";
const char M_CHANNEL []         = "CH";
const char M_CURRENT_TIME []    = "CT";
const char M_ELAPSED_TIME []    = "ET";
const char M_REMAINING_TIME []  = "RT";


// ********* States ***********

String operationalStates[] = {};

const int ST_IDLE       = 10;
const int ST_INIT       = 11;
const int ST_STOPPED    = 12;
const int ST_STARTING   = 13;
const int ST_STARTED    = 14;
const int ST_STOPPING   = 15;


// Max length of state text is 9 characters
void initOperationalStates() {
  operationalStates[ST_IDLE]      = "Idle";
  operationalStates[ST_INIT]      = "Init";
  operationalStates[ST_STOPPED]   = "Stopped";
  operationalStates[ST_STARTING]  = "Starting";
  operationalStates[ST_STARTED]   = "Started";
  operationalStates[ST_STOPPING]  = "Stopping";
}

int parseChannel(String channel) {
  if (channel.length() == 2) {
    if (channel.startsWith("0")) {
      return channel.charAt(1)+0;
    }
  }
  return 0;
}

#endif
