#ifndef secrets_h
#define secrets_h

/*
 * COPY THIS FILE TO secrets.h
 * And modify as needed.
 * Make sure it's in your .gitignore, and 
 * don't accidentally publish it to GitHub etc!
 *
 */


// define one of these!
#define Network1
//#define Network2

#if defined(Network1)
const string SSID = "Network1";
const string SSIDPSK = "pass1";
const string MQTTSERVER = "192.168.1.1";
#elif defined(Network2)
const string SSID = "Network2";
const string SSIDPSK = "pass2";
const string MQTTSERVER = "192.168.2.1";

#endif

