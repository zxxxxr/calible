var http = require('http'), fs = require('fs');

http.createServer(function (req, res) {
	if(req.url == "/json"){
		fs.readFile('../data/good.json', function(err,data){ 
			if (err) throw err;
			res.writeHead(200, {'Content-Type': 'application/json'});
			res.end(data);
		});
		
		return;
	}
	if(req.method == 'POST' && req.url == "/upload"){
		var data = "";
		req.on('data', function(chunk) {
			data += chunk.toString("utf8");
		});
    	req.on('end', function() {
    		fs.writeFile('../data/good.json', data, function(err){ 
				if (err) throw err;
				res.writeHead(200, "OK", {'Content-Type': 'text/plain'});
				res.end("success");
			});
		});
		return;
	}
	fs.readFile('render.html', function(err,data){ 
		if (err) throw err;
		res.writeHead(200, {'Content-Type': 'text/html'});
		res.end(data);
	});
}).listen(8888);

