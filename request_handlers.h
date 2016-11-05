// ***************************************************************************
// Request handlers
// ***************************************************************************
void getArgs() {
  if (server.arg("rgb") != "") {
    uint32_t rgb = (uint32_t) strtol(server.arg("rgb").c_str(), NULL, 16);
    main_color.red = ((rgb >> 16) & 0xFF);
    main_color.green = ((rgb >> 8) & 0xFF);
    main_color.blue = ((rgb >> 0) & 0xFF);
  } else {
    main_color.red = server.arg("r").toInt();
    main_color.green = server.arg("g").toInt();
    main_color.blue = server.arg("b").toInt();
  }
  FPS = server.arg("d").toInt();

  if (main_color.red > 255) {
    main_color.red = 255;
  }
  if (main_color.green > 255) {
    main_color.green = 255;
  }
  if (main_color.blue > 255) {
    main_color.blue = 255;
  }

  if (main_color.red < 0) {
    main_color.red = 0;
  }
  if (main_color.green < 0) {
    main_color.green = 0;
  }
  if (main_color.blue < 0) {
    main_color.blue = 0;
  }

  if (server.arg("d") == "") {
    FPS = 40;
  }

  DBG_OUTPUT_PORT.print("Mode: ");
  DBG_OUTPUT_PORT.print(mode);
  DBG_OUTPUT_PORT.print(", Color: ");
  DBG_OUTPUT_PORT.print(main_color.red);
  DBG_OUTPUT_PORT.print(", ");
  DBG_OUTPUT_PORT.print(main_color.green);
  DBG_OUTPUT_PORT.print(", ");
  DBG_OUTPUT_PORT.print(main_color.blue);
  DBG_OUTPUT_PORT.print(", Delay:");
  DBG_OUTPUT_PORT.print(FPS);
  DBG_OUTPUT_PORT.print(", Brightness:");
  DBG_OUTPUT_PORT.println(brightness);
  DBG_OUTPUT_PORT.print(", show_length:");
  DBG_OUTPUT_PORT.println(show_length);
}

void handleMinimalUpload() {
  char temp[1500];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf ( temp, 1500,
       "<!DOCTYPE html>\
				<html>\
					<head>\
						<title>ESP8266 Upload</title>\
						<meta charset=\"utf-8\">\
						<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\
						<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
					</head>\
					<body>\
						<form action=\"/edit\" method=\"post\" enctype=\"multipart/form-data\">\
							<input type=\"file\" name=\"data\">\
							<input type=\"text\" name=\"path\" value=\"/\">\
							<button>Upload</button>\
						</form>\
					</body>\
				</html>",
             hr, min % 60, sec % 60
           );
  server.send ( 200, "text/html", temp );
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }
  server.send ( 404, "text/plain", message );
}

char* listStatusJSON() {
  char json[255];
  snprintf(json, sizeof(json), "{\"mode\":%d, \"FPS\":%d,\"show_length\":%d, \"brightness\":%d, \"color\":[%d, %d, %d]}", mode, FPS, show_length, brightness, main_color.red, main_color.green, main_color.blue);
  return json;
}


void getStatusJSON() {
  server.send ( 200, "application/json", listStatusJSON() );
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {
  switch (type) {
    case WStype_DISCONNECTED:
      DBG_OUTPUT_PORT.printf("WS: [%u] Disconnected!\n", num);
      break;

    case WStype_CONNECTED: {
        IPAddress ip = webSocket.remoteIP(num);
        DBG_OUTPUT_PORT.printf("WS: [%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // send message to client
        webSocket.sendTXT(num, "Connected");
      }
      break;

    case WStype_TEXT:
      DBG_OUTPUT_PORT.printf("WS: [%u] get Text: %s\n", num, payload);

      // # ==> Set main color
      if (payload[0] == '#') {
        // decode rgb data
        uint32_t rgb = (uint32_t) strtol((const char *) &payload[1], NULL, 16);
        main_color.red = ((rgb >> 16) & 0xFF);
        main_color.green = ((rgb >> 8) & 0xFF);
        main_color.blue = ((rgb >> 0) & 0xFF);
        DBG_OUTPUT_PORT.printf("Set main color to: [%u] [%u] [%u]\n", main_color.red, main_color.green, main_color.blue);
        webSocket.sendTXT(num, "OK");
      }

      // # ==> Set glitter color
      if (payload[0] == 'G') {
        // decode rgb data
        uint32_t rgb = (uint32_t) strtol((const char *) &payload[1], NULL, 16);
        glitter_color.red = ((rgb >> 16) & 0xFF);
        glitter_color.green = ((rgb >> 8) & 0xFF);
        glitter_color.blue = ((rgb >> 0) & 0xFF);
        DBG_OUTPUT_PORT.printf("Set glitter color to: [%u] [%u] [%u]\n", glitter_color.red, glitter_color.green, glitter_color.blue);
        webSocket.sendTXT(num, "OK");
      }      

      // # ==> Set delay
      if (payload[0] == '?') {
        // decode delay data
        uint8_t d = (uint8_t) strtol((const char *) &payload[1], NULL, 10);
        FPS = ((d >> 0) & 0xFF);
        DBG_OUTPUT_PORT.printf("WS: Set FPS: [%u]\n", FPS);
        webSocket.sendTXT(num, "OK");
      }

      // # ==> Set brightness
      if (payload[0] == '%') {
        uint8_t b = (uint8_t) strtol((const char *) &payload[1], NULL, 10);
        brightness = ((b >> 0) & 0xFF);
        DBG_OUTPUT_PORT.printf("WS: Set brightness to: [%u]\n", brightness);
        FastLED.setBrightness(brightness);
        webSocket.sendTXT(num, "OK");
      }

      // # ==> Set show_length
      if (payload[0] == '^') {
        uint8_t b = (uint8_t) strtol((const char *) &payload[1], NULL, 10);
        show_length = ((b >> 0) & 0xFF);
        DBG_OUTPUT_PORT.printf("WS: Set show_length to: [%u]\n", show_length);
        webSocket.sendTXT(num, "OK");
      }

      // # ==> Set fade to black speed
      if (payload[0] == '_') {
        uint8_t b = (uint8_t) strtol((const char *) &payload[1], NULL, 10);
        ftb_speed = ((b >> 0) & 0xFF);
        DBG_OUTPUT_PORT.printf("WS: Set fade to black speed to: [%u]\n", ftb_speed);
        webSocket.sendTXT(num, "OK");
      } 

      // # ==> Set fade glitter density
      if (payload[0] == '+') {
        uint8_t b = (uint8_t) strtol((const char *) &payload[1], NULL, 10);
        glitter_density = ((b >> 0) & 0xFF);
        DBG_OUTPUT_PORT.printf("WS: Set fade to glitter density to: [%u]\n", glitter_density);
        webSocket.sendTXT(num, "OK");
      }           


      // * ==> Set main color and light all LEDs (Shortcut)
      if (payload[0] == '*') {
        // decode rgb data
        uint32_t rgb = (uint32_t) strtol((const char *) &payload[1], NULL, 16);

        main_color.red = ((rgb >> 16) & 0xFF);
        main_color.green = ((rgb >> 8) & 0xFF);
        main_color.blue = ((rgb >> 0) & 0xFF);

        for (int i = 0; i < NUM_LEDS; i++) {
          leds[i] = CRGB(main_color.red, main_color.green, main_color.blue);
        }
        FastLED.show();
        DBG_OUTPUT_PORT.printf("WS: Set all leds to main color: [%u] [%u] [%u]\n", main_color.red, main_color.green, main_color.blue);
        exit_func = true;
        mode = ALL;
        webSocket.sendTXT(num, "OK");
      }

      // ! ==> Set single LED in given color
      if (payload[0] == '!') {
        // decode led index
        uint64_t rgb = (uint64_t) strtol((const char *) &payload[1], NULL, 16);

        uint8_t led =          ((rgb >> 24) & 0xFF);
        if (led < NUM_LEDS) {
          ledstates[led].red =   ((rgb >> 16) & 0xFF);
          ledstates[led].green = ((rgb >> 8)  & 0xFF);
          ledstates[led].blue =  ((rgb >> 0)  & 0xFF);
          DBG_OUTPUT_PORT.printf("WS: Set single led [%u] to [%u] [%u] [%u]!\n", led, ledstates[led].red, ledstates[led].green, ledstates[led].blue);

          for (uint8_t i = 0; i < NUM_LEDS; i++) {
            leds[i] = CRGB(ledstates[i].red, ledstates[i].green, ledstates[i].blue);
            
          }
          FastLED.show();
        }
        exit_func = true;
        mode = ALL;
        webSocket.sendTXT(num, "OK");
      }

      // ! ==> Activate mode
      if (payload[0] == '=') {
        // we get mode data
        String str_mode = String((char *) &payload[0]);

        exit_func = true;

        if (str_mode.startsWith("=off")) {
          mode = OFF;
        }
        if (str_mode.startsWith("=all")) {
          mode = ALL;
        }
        if (str_mode.startsWith("=mixedshow")) {
          mode = MIXEDSHOW;
        }    
        if (str_mode.startsWith("=rainbow")) {
          mode = RAINBOW;
        } 
        if (str_mode.startsWith("=confetti")) {
          mode = CONFETTI;
        } 
        if (str_mode.startsWith("=sinelon")) {
          mode = SINELON;
        } 
        if (str_mode.startsWith("=juggle")) {
          mode = JUGGLE;
        }          
        if (str_mode.startsWith("=bpm")) {
          mode = BPM;
        }  
        if (str_mode.startsWith("=palette_anims")) {
          mode = PALETTE_ANIMS;
        }   
        if (str_mode.startsWith("=ripple")) {
          mode = RIPPLE;
        }   
        if (str_mode.startsWith("=comet")) {
          mode = COMET;
        }     
        if (str_mode.startsWith("=theaterchase")) {
          mode = THEATERCHASE;
        }                   
        if (str_mode.startsWith("=add_glitter")) {
          GLITTER_ON = true;
        }
        if (str_mode.startsWith("=stop_glitter")) {
          GLITTER_ON = false;
        }                                                                                   
        DBG_OUTPUT_PORT.printf("Activated mode [%u]!\n", mode);
        webSocket.sendTXT(num, "OK");
      }

      // $ ==> Get status Info.
      if (payload[0] == '$') {
        DBG_OUTPUT_PORT.printf("Get status info.");
        
        String json = listStatusJSON();
        DBG_OUTPUT_PORT.println(json);
        webSocket.sendTXT(num, json);
      }
      break;
  }
}

void checkForRequests() {
  webSocket.loop();
  server.handleClient();
  }
