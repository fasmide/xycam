var request = require("request");
var MjpegConsumer = require("mjpeg-consumer");
var FileOnWrite = require("file-on-write");

var WebSocket = require('ws');
var ws = new WebSocket('ws://localhost:3000/imgReceive');
var imgs = 0;
ws.on('open', function() {

	var consumer = new MjpegConsumer();


	consumer.on('data', function(image) {
		ws.send(image.toString('base64'));
		imgs++;
	});

	request("http://10.0.0.1:60152/liveview.JPG?%211234%21http%2dget%3a%2a%3aimage%2fjpeg%3a%2a%21%21%21%21%21").pipe(consumer);


});

setInterval(function() {
	console.log("\rImg/sec: %s", imgs);
	imgs = 0;
}, 1000);