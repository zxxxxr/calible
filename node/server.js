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
	fs.readFile('render.html', function(err,data){ 
		if (err) throw err;
		res.writeHead(200, {'Content-Type': 'text/html'});
		res.end(data);
	});
}).listen(8080);

