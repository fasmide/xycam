var request = require("request");
var MjpegConsumer = require("mjpeg-consumer");
var WebSocket = require('ws');
var SerialPort = require("serialport")




var serialPort = new SerialPort.SerialPort("/dev/ttyUSB0", {
  baudrate: 115200,
  parser: SerialPort.parsers.readline("\n")
});



//
// Image socket
//
//
var ws = new WebSocket('ws://localhost:3000/imgReceive');

var imgs = 0;
ws.on('open', function() {

	var consumer = new MjpegConsumer();


	consumer.on('data', function(image) {
		ws.send(image.toString('base64'));
		imgs++;
	});

	//request("http://10.0.0.1:60152/liveview.JPG?%211234%21http%2dget%3a%2a%3aimage%2fjpeg%3a%2a%21%21%21%21%21").pipe(consumer);

});

setInterval(function() {
	process.stdout.write("\rImg/sec: "+ imgs);
	imgs = 0;
}, 1000);


//
// Control socket
// for positioning and reporting
//
var controlWs = new WebSocket('ws://localhost:3000/control');

controlWs.on('open', function() {
	console.log("Control socket opened");

});

controlWs.on('message', function(data) {
	data = JSON.parse(data);

	if (data.type == 'jog') {
		serialPort.write("j"+data.data.x + " " + data.data.y + " ");
	}
	if (data.type == 'absolutePosition') {
		serialPort.write("g"+data.data.x + " " + data.data.y + " ");	
	}
});


//
// Stepper
// uses slutty protocol like:
//
// Jogging:
//		j10<space>10<space>
//		- Moves +10 x, y relative to current position
//
// Absolute position:
// 		g10<space>10<space>
//		- Moves to position 10x 10y
//

serialPort.on("open", function () {
  console.log('Serialport opened');
});

serialPort.on('data', function(str) {
	
	var data = str.split(' ');
	controlWs.send(JSON.stringify({ 
		type: 'position', 
		data: {
			currentX: parseInt(data[0], 10),
			currentY: parseInt(data[1], 10),
			targetX: parseInt(data[2], 10),
			targetY: parseInt(data[3], 10),
			distanceX: parseInt(data[4], 10),
			distanceY: parseInt(data[5], 10) 
		}
	}));
});