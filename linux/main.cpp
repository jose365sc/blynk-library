/**
 * @file       main.cpp
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Mar 2015
 * @brief
 */

#include <random>

//#define BLYNK_DEBUG
#define BLYNK_PRINT stdout
#ifdef RASPBERRY
  #include <BlynkApiWiringPi.h>
#else
  #include <BlynkApiLinux.h>
#endif
#include <BlynkSocket.h>
#include <BlynkOptionsParser.h>

static BlynkTransportSocket _blynkTransport;
BlynkSocket Blynk(_blynkTransport);

static const char *auth, *serv;
static uint16_t port;

#include <BlynkWidgets.h>

BLYNK_WRITE(V1)
{
    printf("Got a value: %s\n", param[0].asStr());
}


char lastValue[16][256];

void obtainValues()
{
#if 1
  char command[1024] = "";
  sprintf(command, "/home/pi/blynk/BLYNK_OBTAIN_VALUES");
  BLYNK_LOG("Value exec %s", command);

  FILE *fp;
  char buff[1024] = "";
  if((fp=popen(command,"r")) != NULL) {
    if(fgets(buff, 1023, fp) != NULL) strtok(buff, "\n\0");
  }
  pclose(fp);
  BLYNK_LOG("Value stdout = %s", buff);

  double v[12];
  sscanf(buff, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
	v+0, v+1, v+2, v+3, v+4, v+5,
        v+6, v+7, v+8, v+9, v+10, v+11);

//  for(int i = 0; i < 12; i++)
//      printf(":%f", v[i]);

  for(int i = 0; i < 12; i++)
      sprintf(lastValue[i], "%f", v[i]);

  BLYNK_LOG("Values = %s, %s, %s, %s, %s, %s",
      lastValue[0], lastValue[1], lastValue[2], lastValue[3], lastValue[4], lastValue[5]);
  BLYNK_LOG("Values = %s, %s, %s, %s, %s, %s",
      lastValue[6], lastValue[7], lastValue[8], lastValue[9], lastValue[10], lastValue[11]);

#else
  std::random_device rnd;
  sprintf(lastValue[0],  "%f", 0  * 5.0 + (rnd() % 100) / 10.0 + 10.0);
  sprintf(lastValue[1],  "%f", 1  * 5.0 + (rnd() % 100) / 10.0 + 10.0);
  sprintf(lastValue[2],  "%f", 2  * 5.0 + (rnd() % 100) / 10.0 + 10.0);
  sprintf(lastValue[3],  "%f", 3  * 5.0 + (rnd() % 100) / 10.0 + 10.0);
  sprintf(lastValue[4],  "%f", 4  * 5.0 + (rnd() % 100) / 10.0 + 10.0);
  sprintf(lastValue[5],  "%f", 5  * 5.0 + (rnd() % 100) / 10.0 + 10.0);
  sprintf(lastValue[6],  "%f", 6  * 5.0 + (rnd() % 100) / 10.0 + 10.0);
  sprintf(lastValue[7],  "%f", 7  * 5.0 + (rnd() % 100) / 10.0 + 10.0);
  sprintf(lastValue[8],  "%f", 8  * 5.0 + (rnd() % 100) / 10.0 + 10.0);
  sprintf(lastValue[9],  "%f", 9  * 5.0 + (rnd() % 100) / 10.0 + 10.0);
  sprintf(lastValue[10], "%f", 10 * 5.0 + (rnd() % 100) / 10.0 + 10.0);
  sprintf(lastValue[11], "%f", 11 * 5.0 + (rnd() % 100) / 10.0 + 10.0);
#endif
}

BLYNK_READ(V0)  { Blynk.virtualWrite(0,  lastValue[0]); };
BLYNK_READ(V1)  { Blynk.virtualWrite(1,  lastValue[1]); };
BLYNK_READ(V2)  { Blynk.virtualWrite(2,  lastValue[2]); };
BLYNK_READ(V3)  { Blynk.virtualWrite(3,  lastValue[3]); };
BLYNK_READ(V4)  { Blynk.virtualWrite(4,  lastValue[4]); };
BLYNK_READ(V5)  { Blynk.virtualWrite(5,  lastValue[5]); };
BLYNK_READ(V6)  { Blynk.virtualWrite(6,  lastValue[6]); };
BLYNK_READ(V7)  { Blynk.virtualWrite(7,  lastValue[7]); };
BLYNK_READ(V8)  { Blynk.virtualWrite(8,  lastValue[8]); };
BLYNK_READ(V9)  { Blynk.virtualWrite(9,  lastValue[9]); };
BLYNK_READ(V10) { Blynk.virtualWrite(10, lastValue[10]); };
BLYNK_READ(V11) { Blynk.virtualWrite(11, lastValue[11]); };

void push_vpins()
{
    Blynk.virtualWrite(0,  lastValue[0]);
    Blynk.virtualWrite(1,  lastValue[1]);
    Blynk.virtualWrite(2,  lastValue[2]);
    Blynk.virtualWrite(3,  lastValue[3]);
    Blynk.virtualWrite(4,  lastValue[4]);
    Blynk.virtualWrite(5,  lastValue[5]);
    Blynk.virtualWrite(6,  lastValue[6]);
    Blynk.virtualWrite(7,  lastValue[7]);
    Blynk.virtualWrite(8,  lastValue[8]);
    Blynk.virtualWrite(9,  lastValue[9]);
    Blynk.virtualWrite(10, lastValue[10]);
    Blynk.virtualWrite(11, lastValue[11]);
}

void setup()
{
    Blynk.begin(auth, serv, port);
}

void loop()
{
    Blynk.run();
}


int main(int argc, char* argv[])
{
    const int push_update_interval = 120;

    parse_options(argc, argv, auth, serv, port);

    obtainValues();
    setup();
    int count = 0;
    while(true) {
        loop();
	if (count > push_update_interval)
	{
	    obtainValues();
	    push_vpins();
	    count = 0;
	}
	else
 	    ++count;
	sleep(1);
    }

    return 0;
}

