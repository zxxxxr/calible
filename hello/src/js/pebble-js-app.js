var points = [];
var lastTime = undefined;
var real_points = [{x:0,y:0,z:0}];

function webRequest(data, callback){
  var req = new XMLHttpRequest();
  req.open('POST', 'http://ec2.maimoe.net:8888/upload', true);
  req.setRequestHeader("Content-type","application/x-www-form-urlencoded");
  req.onload = function(e) {
    if (req.readyState == 4 && req.status == 200) {
      if(req.status == 200) {
        callback(false);
      } else {
        callback(true);
      }
    }
  }
  req.send("data=" + encodeURIComponent(JSON.stringify(data)));
};

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
    points = [];
    return;
  }
  var last_point = real_points[real_points.length - 1];
  var tdiff = -1, nowtime = e.payload.time_s + (e.payload.time_ms / 1000);
  if(lastTime != null){
  	tdiff = nowtime - lastTime;
  	var newPoint = {
  		x:last_point.x + e.payload.x * tdiff,
  		y:last_point.y + e.payload.y * tdiff
  		z:last_point.xz + e.payload.z * tdiff
  	};
  	console.log(newPoint);
  	real_points.push(newPoint);
  	webRequest({
 		data:real_points
	  }, function(e){
		if(!e)
		  console.log("Web data submitted!");
	  });
  }
  lastTime = e.payload.time_s + (e.payload.time_ms / 1000);
});


var getDistance = function (sorted) {
  var dis = 0;

  var px = 0;
  var py = 0;
  var pz = 0;

  sorted[0].time = sorted[0].time_s + sorted[0].time_ms / 1000;
  //startTime = sorted[0].time;

  for (var i = 1; i < sorted.length; i++) {
    sorted[i].time = (sorted[i].time_s) + (sorted[i].time_ms/1000);
    var dt = sorted[i].time - sorted[i-1].time;

    //console.log("dt = " + dt);
    //console.log("VX = " + ax);
    //console.log("VY = " + ay);
    //console.log("VZ = " + az);

    var dx = sorted[i].x * dt;
    var dy = sorted[i].y * dt;
    var dz = sorted[i].z * dt;

    px += dx;
    py += dy;
    pz += dz;
    
    real_points.push({
    	x: px,
    	y: py,
    	z: pz
    });

    var s  = Math.sqrt(dx*dx + dy*dy + dz*dz);
    
    console.log('package '+ i + " s : " + s + "  dt= " + dt);
    console.log(' ');
    dis += s;
  }
  console.log("DISTANCE = " + dis);
  console.log("NUM OF POINTS: " + real_points.length);
  webRequest({
  	data:real_points
  }, function(e){
    if(!e)
      console.log("Web data submitted!");
  });
};
