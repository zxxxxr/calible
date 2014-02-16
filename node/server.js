var http = require('http');

http.createServer(function (req, res) {
	if(req.path == "/json"){
		res.writeHead(200, {'Content-Type': 'application/json'});
		res.end();
		return;
	}
	res.writeHead(200, {'Content-Type': 'text/html'});
	res.end('Web Server Inited\n');
}).listen(8080);

