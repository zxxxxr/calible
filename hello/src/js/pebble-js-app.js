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
  if (!e.playload.hasNext) {
    console.log("Data ends.");
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


var getDistance = function (sorted) {
  var vx = 0;
  var vy = 0;
  var vz = 0;
  var dis = 0;

  var ax0 = sorted[0].x;
  var ay0 = sorted[0].y;
  var az0 = sorted[0].z;

  for (var i = 1; i < sorted.length; i++) {
    var dt = sorted[i].time - sorted[i-1].time;
   
    var ax = sorted[i].x - ax0;
    var ay = sorted[i].y - ay0;
    var az = sorted[i].z - az0;

    var dx = vx + 0.5 * ax * dt * dt;
    var dy = vy + 0.5 * ay * dt * dt;
    var dz = vz + 0.5 * az * dt * dt;

    vx += ax * dt;
    vy += ay * dt;
    vz += az * dt;

    var s  = Math.sqrt(dx*dx + dy*dy + dz*dz);
    console.log('s = ' + s);
    dis += s;
  }
  console.log(dis);
};

/*
var point1 = {time: 1 , x:0 , y:2 , z:0};
var point2 = {time: 2 , x:1 , y:0 , z:0};
var point3 = {time: 3 , x:0 , y:0 , z:1};
getDistance([point1, point2, point3]);
*/
