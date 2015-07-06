var request = require('request'),
	exec = require('child_process').exec,
	fs = require('fs');

request.post({url: "http://10.0.0.1:10000/sony/camera", json: true, body: 
{
 "method": "actTakePicture",
 "params": [],
 "id": 1,
 "version": "1.0"
}
}, function(err, headers, body) {
	var filename = "img" + Date.now().toString() + ".jpg";
	request.get(body.result.pop().pop()).pipe(fs.createWriteStream(filename));
	console.log(filename, "saved");
});

"http://10.0.0.1:60152/liveview.JPG?%211234%21http%2dget%3a%2a%3aimage%2fjpeg%3a%2a%21%21%21%21%21"