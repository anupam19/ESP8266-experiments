#define SSID "YOURSSID"
#define PASS "YOURPASSWORD"
#define SERVER "173.17.168.19"
#define PORT "8181"
#define PIRPIN 2
#define AUTH_TOKEN "YOUR_TOKEN_FROM_YOUR_ACCOUNT";

char payload [150];
char c;
int calibrationTime = 30;
int pir_val;

void setup () {
    Serial2.begin (115200);
    Serial2.setTimeout (5000);
    Serial.begin (115200);
    delay (500);
    Serial.println("MotionSense Not-e-FYI");

    sensor_calibration ();
    Serial2.flush();
    Serial.flush();

    check_WiFi_radio ();
    serial_find ();
    delay(500);
    Serial2.flush();

    reset_wifi ();
    serial_find ();
    delay(5000);
    Serial2.flush();

    set_wifi_mode ();
    serial_read();
    delay(10000);
    Serial2.flush();
    
    reset_wifi ();
    serial_find ();
    delay(5000);
    Serial2.flush();

    check_wifi_stat ();
    if(Serial2.find("WIFI GOT IP")) {
        Serial.println("Connected to Access Point");
    }
    else {
        connect_ssid ();
    }
    delay(500);
    Serial2.flush();

}

void loop () {
    pir_val = pirRead ();
    if (pir_val == 1) {
        server_connect ();
        serial_find();
        Serial2.flush();

        post ();
        serial_find();
        Serial2.flush();

        server_disconnect ();
        serial_read();
        Serial2.flush();
    }
    else {
         Serial.println("Everything fine | No intrusion");
    }
}

void sensor_calibration () {
    digitalWrite (PIRPIN, LOW);
    Serial.print("calibrating sensor ");
    for (int i = 0; i < calibrationTime; i++) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
}

void check_WiFi_radio () {
    Serial.println("\nChechking AT");
    Serial2.println("AT");
}

void serial_find () {
    if(Serial2.find("OK")) {
        Serial.println("command Successfully done");
    }
    else {
        Serial.println("command Unsuccessful");
    }
}

void reset_wifi () {
    Serial.println("\nChechking AT+RST");
    Serial2.println("AT+RST");
}

void set_wifi_mode () {
    Serial.println("\nSetting AT+CWMODE=1");
    Serial2.println("AT+CWMODE=1");
}

void serial_read() {
    while (Serial2.available () > 0) {
        c = Serial2.read();
        Serial.print(c);    
    }
}

void check_wifi_stat () {
    Serial.println("\nSetting AT+CWJAP?");
    Serial2.print("AT+CWJAP?\r\n");
}

void connect_ssid () {
        String connect = "AT+CWJAP=\"";
        connect += SSID;
        connect += "\",\"";
        connect += PASS;
        connect += "\"";
        Serial.print ("Connecting to SSID : ");
        Serial.println (SSID);
        Serial2.println(connect);
}

void server_connect () {
    Serial.println("\nConnecting to server");
    String client_conn = "AT+CIPSTART=\"TCP\",\"";
    client_conn += SERVER;
    client_conn += "\",";
    client_conn += PORT;
    
    Serial2.println(client_conn);
    Serial2.flush();
}

void server_disconnect () {
    Serial.println("Disconnecting from server");
    Serial2.println("AT+CIPCLOSE");
    serial_read();
}

void post () {
    memset(payload , '\0', 150);
    sprintf(payload, "{\"type\": \"note\", \"title\": \"Motion Activity\", \"body\": \"Status code = [%d] Intrusion detected . Take action Immediately !!! \"}", pir_val);
    String data = payload;

    String post = "POST /pushbullet/v2/pushes HTTP/1.1\r\n";
    post += "Host: ";
    post += SERVER;
    post += "\r\n"
    post += "Accept: application/json";
    post += "User-Agent: Arduino\r\n";
    post += "Content-Type: application/json\r\n";
    post += "Authorization: Bearer ";
    post += AUTH_TOKEN;
    post += "\r\n"
    post += "Content-Length: ";
    post += data.length();
    post += "\r\n";
    post += "Connection: close\r\n\r\n";
    post += data;
    post += "\r\n";

    Serial2.print("AT+CIPSEND=");
    Serial2.println(post.length());
    if(Serial2.find(">")) {
        Serial.println("sending data");
        Serial2.println(post);
        serial_find();
    }
    else {
        Serial.println ("Unable to send data");
    Serial2.flush();
    }
}

int pirRead () {
    int p = 0;
    int a = 0;
    int curr = 0;
    while (a<1) {
        curr = digitalRead(PIRPIN);
        p |= curr;
        a++;
    }
    if (p == 1){
        if (p == 0){
            p = 1;
        }
    }
    else {
        if (p == 1){
            p = 0;
        }
    }
    return p;
}
