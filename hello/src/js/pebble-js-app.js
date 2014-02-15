var points = [];
var startTime = undefined;

Pebble.addEventListener("ready",
function(e) {
  console.log("connect!" + e.ready);
  console.log(e.type);
});

// Set callback for appmessage events
Pebble.addEventListener("appmessage",
function(e) {
  console.log("message");
  if (!e.payload.hasNext) {
    console.log("Data ends.");
    console.log("  ");
    getDistance(points);
    return;
  }
  if (e.payload.time_s) {
    console.log("time_s = " + e.payload.time_s);
  }
  if (e.payload.time_ms) {
    console.log("time_ms = " + e.payload.time_ms);
  }
  if (e.payload.x) {
    console.log("x = " + e.payload.x);
  }
  if (e.payload.y) {
    console.log("y = " + e.payload.y);
  }
  if (e.payload.z) {
    console.log("z = " + e.payload.z);
  }

  points.push(e.payload);
});


var getDistance = function (sorted) {
  console.log("CALLed getDistance");
  var vx = 0;
  var vy = 0;
  var vz = 0;
  var dis = 0;

  var ax0 = sorted[0].x;
  var ay0 = sorted[0].y;
  var az0 = sorted[0].z;

  sorted[0].time = sorted[0].time_s + sorted[0].time_ms / 1000;
  //startTime = sorted[0].time;

  for (var i = 1; i < sorted.length; i++) {
    sorted[i].time = (sorted[i].time_s) + (sorted[i].time_ms/1000);
    var dt = sorted[i].time - sorted[i-1].time;
   
    var ax = (sorted[i].x - ax0) / 100;
    var ay = (sorted[i].y - ay0) / 100;
    var az = (sorted[i].z - az0) / 100;

    var dx = vx + 0.5 * ax * dt * dt;
    var dy = vy + 0.5 * ay * dt * dt;
    var dz = vz + 0.5 * az * dt * dt;

    vx += ax * dt;
    vy += ay * dt;
    vz += az * dt;

    var s  = Math.sqrt(dx*dx + dy*dy + dz*dz);
    //console.log('s = ' + s);
    dis += s;
  }
  console.log("DISTANCE = " + dis);
};

/*
var point1 = {time: 1 , x:0 , y:2 , z:0};
var point2 = {time: 2 , x:1 , y:0 , z:0};
var point3 = {time: 3 , x:0 , y:0 , z:1};
getDistance([point1, point2, point3]);
*/
