/**
  ******************************************************************************
  * @file    srvr.h
  * @author  Waveshare Team
  * @version V1.0.0
  * @date    23-January-2018
  * @brief   ESP8266 WiFi server.
  *          This file provides firmware functions:
  *           + Sending web page of the tool to a client's browser
  *           + Uploading images from client part by part
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
//#include <ESP8266WiFi.h>// ESP8266 and WiFi classes
#include <WiFi.h>

#include "buff.h" // POST request data accumulator
#include "epd.h"  // e-Paper driver

#include "scripts.h" // JavaScript code
#include "css.h"     // Cascading Style Sheets
#include "html.h"    // HTML page of the tool

//#include "utility/Debug.h"
#include "DEV_Config.h"
#include "GUI_Paint.h"


/* SSID and password of your WiFi net ----------------------------------------*/
const char *ssid = "FRITZ!Box 7530 XF"; //"your ssid";
const char *password = "63650802977324380###";   //"your password";

/* Static IP address Settings ------------------------------------------------*/
IPAddress staticIP(192, 168, 178, 113);
IPAddress gateway(192, 168, 178, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8);

/* Server and IP address ------------------------------------------------------*/
WiFiServer server(80); // Wifi server exemplar using port 80
IPAddress myIP;        // IP address in your local wifi net

// Bilddaten
  UBYTE *BlackImage;
  /* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
  UWORD Imagesize = ((EPD_7IN5_V2_WIDTH % 8 == 0) ? (EPD_7IN5_V2_WIDTH / 8 ) : (EPD_7IN5_V2_WIDTH / 8 + 1)) * EPD_7IN5_V2_HEIGHT;

/* The 'index' page flag ------------------------------------------------------*/
bool isIndexPage = true; // true : GET  request, client needs 'index' page;
// false: POST request, server sends empty page.
/* Server initialization -------------------------------------------------------*/
void Srvr__setup()
{
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

	if (WiFi.config(staticIP, gateway, subnet, dns, dns) == false) {
		Serial.println("Configuration failed.");
	}

    // Applying SSID and password
    WiFi.begin(ssid, password);

    // Waiting the connection to a router
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    // Connection is complete
    Serial.println("");

    Serial.println("WiFi connected");

    // Start the server
    server.begin();
    Serial.println("Server started");

    // Show obtained IP address in local Wifi net
    Serial.println(myIP = WiFi.localIP());
    //Create a new image cache
  if ((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
    printf("Failed to apply for black memory...\r\n");
    while (1);
  }
  printf("Paint_NewImage\r\n");
  Paint_NewImage(BlackImage, EPD_7IN5_V2_WIDTH, EPD_7IN5_V2_HEIGHT, 0, WHITE);
}

/* Sending a script to the client's browser ------------------------------------*/
bool Srvr__file(WiFiClient client, int fileIndex, char *fileName)
{
    // Print log message: sending of script file
    Serial.print(fileName);

    // Sent to the 'client' the header describing the type of data.
    client.print(fileIndex == 0
                 ? "HTTP/1.1 200 OK\r\nContent-Type: text/css\r\n\r\n"
                 : "HTTP/1.1 200 OK\r\nContent-Type: text/javascript\r\n\r\n");

    // Choose the index of script
    // (ESP8266 can't to send all of code by one file
    // and needs split it on a few parts)
    switch (fileIndex) {
    case 0:
        sendCSS(client);
        break;
    case 1:
        sendJS_A(client);
        break;
    case 2:
        sendJS_B(client);
        break;
    case 3:
        sendJS_C(client);
        break;
    case 4:
        sendJS_D(client);
        break;
    }

    client.print("\r\n");
    delay(1);

    // Print log message: the end of request processing
    Serial.println(">>>");

    return true;
}

/* The server state observation loop -------------------------------------------*/
bool Srvr__loop()
{
    // Looking for a client trying to connect to the server
    WiFiClient client = server.available();

    // Exit if there is no any clients
    if (!client)
        return false;

    // Print log message: the start of request processing
    Serial.print("<<<");

    // Waiting the client is ready to send data
    while (!client.available())
        delay(1);

    // Set buffer's index to zero
    // It means the buffer is empty initially
    Buff__bufInd = 0;

    // While the stream of 'client' has some data do...
    while (client.available()) {
        // Read a character from 'client'
        int q = client.read();

        // Save it in the buffer and increment its index
        Buff__bufArr[Buff__bufInd++] = (byte)q;

        // If the carachter means the end of line, then...
        if ((q == 10) || (q == 13)) {
            // Clean the buffer
            Buff__bufInd = 0;
            continue;
        }

        // Requests of files
        if (Buff__bufInd >= 11) {
            if (Buff__signature(Buff__bufInd - 11, "/run.go")) {
              // zu tun
              //1.Select Image
  printf("SelectImage:BlackImage\r\n");
  Paint_SelectImage(BlackImage);
  Paint_Clear(WHITE);

  // 2.Drawing on the image
  printf("Drawing:BlackImage\r\n");
  Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
  Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
  Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
  Paint_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
  Paint_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
  Paint_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
  Paint_DrawRectangle(80, 70, 130, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
  Paint_DrawCircle(45, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
  Paint_DrawCircle(105, 95, 20, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
  Paint_DrawLine(85, 95, 125, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
  Paint_DrawLine(105, 75, 105, 115, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
  Paint_DrawString_EN(10, 0, "waveshare", &Font16, BLACK, WHITE);
  Paint_DrawString_EN(10, 20, "hello world", &Font12, WHITE, BLACK);
  Paint_DrawNum(10, 33, 123456789, &Font12, BLACK, WHITE);
  Paint_DrawNum(10, 50, 987654321, &Font16, WHITE, BLACK);
  Paint_DrawString_CN(130, 0, " 你好abc", &Font12CN, BLACK, WHITE);
  Paint_DrawString_CN(130, 20, "微雪电子", &Font24CN, WHITE, BLACK);

  printf("EPD_Display\r\n");
  EPD_7IN5_V2_Display(BlackImage);
  
                return true;
            }
            if (Buff__signature(Buff__bufInd - 11, "/styles.css"))
                return Srvr__file(client, 0, "styles.css");

            if (Buff__signature(Buff__bufInd - 11, "/scriptA.js"))
                return Srvr__file(client, 1, "scriptA.js");

            if (Buff__signature(Buff__bufInd - 11, "/scriptB.js"))
                return Srvr__file(client, 2, "scriptB.js");

            if (Buff__signature(Buff__bufInd - 11, "/scriptC.js"))
                return Srvr__file(client, 3, "scriptC.js");

            if (Buff__signature(Buff__bufInd - 11, "/scriptD.js"))
                return Srvr__file(client, 4, "scriptD.js");
        }

        // If the buffer's length is larger, than 4 (length of command's name), then...
        if (Buff__bufInd > 4) {
            // It is probably POST request, no need to send the 'index' page
            isIndexPage = false;

            // e-Paper driver initialization
            if (Buff__signature(Buff__bufInd - 4, "EPD")) {
                Serial.print("\r\nEPD\r\n");
                // Getting of e-Paper's type
                EPD_dispIndex = (int)Buff__bufArr[Buff__bufInd - 1] - (int)'a';
                if(EPD_dispIndex < 0)
                  EPD_dispIndex = (int)Buff__bufArr[Buff__bufInd - 1] - (int)'A' + 26;
                // Print log message: initialization of e-Paper (e-Paper's type)
                Serial.printf("EPD %s", EPD_dispMass[EPD_dispIndex].title);

                // Initialization
                EPD_dispInit();
                //client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
                break;
            }

            // Image loading
            if (Buff__signature(Buff__bufInd - 4, "LOAD")) {
                // Print log message: image loading
                Serial.print("LOAD");

                // Load data into the e-Paper
                // if there is loading function for current channel (black or red)
                if (EPD_dispLoad != 0)
                    EPD_dispLoad();
                //client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
                break;
            }

            // Select the next data channel
            if (Buff__signature(Buff__bufInd - 4, "NEXT")) {
                // Print log message: next data channel
                Serial.print("NEXT");

                // Instruction code for for writting data into
                // e-Paper's memory
                int code = EPD_dispMass[EPD_dispIndex].next;

                // If the instruction code isn't '-1', then...
                if (code != -1) {
                    // Print log message: instruction code
                    Serial.printf(" %d", code);

                    // Do the selection of the next data channel
                    EPD_SendCommand(code);
                    delay(2);
                }

                // Setup the function for loading choosen channel's data
                EPD_dispLoad = EPD_dispMass[EPD_dispIndex].chRd;
                //client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
                break;
            }

            // If the loading is complete, then...
            if (Buff__signature(Buff__bufInd - 4, "SHOW")) {
                // Show results and Sleep
                EPD_dispMass[EPD_dispIndex].show();

                //Print log message: show
                Serial.print("\r\nSHOW");
                //client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
                break;
            }

            // If the routine reaches this code,
            // it means the there is no known commands,
            // the server has to send the 'index' page
            isIndexPage = true;
        }
    }

    // Clear data stream of the 'client'
    client.flush();

    // Sent to the 'client' the header describing the type of data.
    // In this case 'Content-Type' is 'text/html'
    client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");

    // Send the 'index' page if it's needed
    if (isIndexPage)
        sendHtml(client, myIP);
    else
        client.print("Ok!");

    client.print("\r\n");
    delay(1);

    // Print log message: the end of request processing
    Serial.println(">>>");
    return true;
}
