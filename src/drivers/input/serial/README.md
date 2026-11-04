# Serial console input

Reads one-line commands from the serial monitor (115200 baud, send with Enter):

| Command | Action |
|---|---|
| `i` | Print the current status again (the console only shows changes, so use this after opening the monitor) |
| `p` | Play / pause |
| `s` | Stop the alarm |
| `z` | Snooze while the alarm rings, otherwise start the sleep timer |
| `n` / `b` | Next / previous station |
| `+` / `-` | Volume up / down by 5 |
| `v<0-100>` | Set the volume, e.g. `v40` |

Any other input prints this list. While the alarm is ringing the volume commands change the alarm volume.
