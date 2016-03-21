var path = require('path')
var http = require('http')

http.createServer(function(req, res) {
	console.log('========================================================')
	console.log('Client requested to server')
	respondToClient(req, res);

}).listen(4000)

function respondToClient(req, res) {
	var filePath = path.normalize(req.url)
	console.log('Requested file path : ', filePath)

	imageFilePath = readyForData(filePath)
	if (imageFilePath.length < 1) {
		console.log('Invalid path')
		return;
	}

	console.log('Valid path :' + imageFilePath) 
	readData(imageFilePath, res)
}


function readyForData(filePath) {
	// image name is fixed
	var imageName = '1.png'
	var basicFilePath = 'C:\\Users\\jmhan\\Desktop'

	var fullFileDirPath = basicFilePath + filePath
	if (!checkDirectory(fullFileDirPath)) {
		console.log("Directory is not exist : " + fullFileDirPath);
		return "";
	}

	var fullFileImagePath = fullFileDirPath + '\\' + imageName
	if (!checkFile(fullFileImagePath)) {
		console.log("File is not exist : " + fullFileImagePath);
		return "";
	}

	return fullFileImagePath;
}

function readData(imagepath, res) {
	var fileStat = fs.statSync(imagepath)
	var fileSize = fileStat['size']

	fs.open(imagepath, 'r', function(err, fd) {
		if (err) {
			throw err;
		}

		var readBuffer = new Buffer(fileSize),
			bufferOffset = 0,
			bufferLength = readBuffer.length,
			filePosition = 0;


		fs.read(fd, readBuffer, bufferOffset, bufferLength, filePosition, 
			function read(err, readBytes) {
				if (err) {
					throw err;
				}
				console.log('read ' + readBytes)
				if (readBytes > 0) {
					//console.log(readBuffer.slice(0, readBytes))
				}

				res.end(readBuffer);
			});
	});
}

/*
	Util Method
*/

var fs = require('fs')

function checkDirectory(filepath) {
	try {
		return fs.statSync(filepath).isDirectory();
	}
	catch (err) {
		return false;
	}
}

function checkFile(filepath) {
	try {
		return fs.statSync(filepath).isFile();
	}
	catch (err) {
		return false;
	}
}


