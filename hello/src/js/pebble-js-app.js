Pebble.addEventListener("ready",
function(e) {
  console.log("connect!" + e.ready);
  console.log(e.type);
});

// Set callback for appmessage events
Pebble.addEventListener("appmessage",
function(e) {
  console.log("message");
  if (!e.playload.hasNext) {
    console.log("Data ends");
    return;
  }

  if (e.payload.time_s) {
    console.log("time_s = " + e.payload.s);
  }
  if (e.payload.time_ms) {
    console.log("time_ms = " + e.payload.ms);
  }
  if (e.payload.x) {
    console.log("x = " + x);
  }
  if (e.payload.y) {
    console.log("y = " + y);
  }
  if (e.payload.z) {
    console.log("z = " + z);
  }
});

