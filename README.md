# esp32-next-rocket-public

ESP32 based device that displays the next Space X rocket launch date. It also starts
a countdown at T-90 minutes.

Physical device that is using this code:
https://www.hackster.io/akossereg/spacex-launch-countdown-display-cde7c2

## Hardware Components

- MAX7219 - 8 digit x 7 segments display module: this is being used to display the date of the 
next rocket launch, eg "21-01-31"
- TM1637 - 88:88 clock module: to display the time of the launch, eg "13:02"
- TM1637 - 88:88 clock module: for the countdown

See `include/config.h` for the GPIO connections.

## HTTPS server certificate
`server_root_cert.pem` should contain the proper certificate. You can fetch the certificate with the following command:
```
# openssl s_client -connect www.your-relay-server.io:443 -showcerts
// using the first block of the output
```

## Configuration
- You need to update `main/include/networking.h`, set WIFI ssid and password here. The device will connect to your local wifi network to be able to pull launch data.
- You need to expose a HTTP endpoint, which reads data from nextrocket.space, then responds a payload like this: `SpaceX;33;5;18;21-10-31-02:00:00` which 
  tells the device the following parameters: <company>;<days..>;<hours...>;<minutes-to-launch>;<date-time-of-launch>. The reason I was not using nextrocket.space 
  directly is that I did not want to parse JSON payload (I am too lame to use a lib for that), so it was easier to relay the request, and transform it to a more ESP32-readable format.
- Exposed HTTP endpoint's HOST and URL should be set here: `main/include/config.h`, see REST_HOST and REST_ENDPOINT
- Generate server_root-cert.pem as explained above

## Behaviour

- on startup, the devices connects to the local wifi network, and makes a request to your relay server to fetch 
the next rocket launch's details
- every 60 minutes it makes the same request to synchronize the timers