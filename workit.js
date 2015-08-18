var SerialPort = require("serialport")


var serialPort = new SerialPort.SerialPort("/dev/rfcomm0", {
  baudrate: 57600,
  parser: SerialPort.parsers.readline("\n")
});

var direction = true;

setInterval(function() {

	if (direction) {
		serialPort.write("g0 0 ");
	} else {
		serialPort.write("g3900 0 ");
	}
	direction = !direction;

}, 12000);

serialPort.on('data', function(str) {
	
	console.log(str.toString());
});