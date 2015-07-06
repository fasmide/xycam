var request = require("request");
var MjpegConsumer = require("mjpeg-consumer");
var WebSocket = require('ws');
var SerialPort = require("serialport")

var ws = new WebSocket('ws://localhost:3000/imgReceive');

var controlWs = new WebSocket('ws://localhost:3000/control');

var serialPort = new SerialPort.SerialPort("/dev/ttyUSB0", {
  baudrate: 115200,
  parser: SerialPort.parsers.readline("\n")
});


var imgs = 0;
ws.on('open', function() {

	var consumer = new MjpegConsumer();


	consumer.on('data', function(image) {
		ws.send(image.toString('base64'));
		imgs++;
	});

	//request("http://10.0.0.1:60152/liveview.JPG?%211234%21http%2dget%3a%2a%3aimage%2fjpeg%3a%2a%21%21%21%21%21").pipe(consumer);


});

controlWs.on('open', function() {
	console.log("Control socket opened");

});

serialPort.on("open", function () {
  console.log('Serialport opened');
  serialPort.write('?');
});

serialPort.on('data', function(str) {
	console.log('Has serial data:', str);
	if (str.match(/^Grbl/)) {
		console.log("grbl ready");
		setInterval(function() {
			serialPort.write('?');
		}, 250);
		return;
	}
	controlWs.send(str);
});


setInterval(function() {
	process.stdout.write("\rImg/sec: "+ imgs);
	imgs = 0;
}, 1000);