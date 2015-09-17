#!/usr/bin/env node
var fs = require('fs');
var semver = require('semver');
var https = require('https');

var electronUrl = "https://registry.npmjs.org/electron-prebuilt";

function getJson(url, callback) {
	https.get(url, function (res) {
		if(res.statusCode > 300 && res.statusCode <= 308) {
			getJson(dir, res.headers.location, callback);
		} else if(res.statusCode === 200) {
			var contents = '';
			res.on('data', function (d) { contents += d.toString('utf8'); });
			res.on('end', function () { callback(null, JSON.parse(contents)); });
		} else if(res.statusCode === 404) {
			callback(null, false);
		} else {
			callback(new Error({ statusCode: res.statusCode, headers: res.headers }));
		}
	})
}

fs.readFile('package.json', {encoding:'utf8'}, function (err, contents) {	
	var electronVersion = 'latest';
	if (!err) {
		var package = JSON.parse(contents);
		if (package.engines && package.engines.electron) {
			electronVersion = package.engines.electron;
			if (semver.valid(electronVersion)) {
				return console.log(electronVersion);
			}
		}
	}

	getJson(electronUrl + '/' + electronVersion, function (err, contents) {
		if (!err && contents && contents.version) {
			return console.log(contents.version);
		}

		if (process.env.ELECTRON_VERSION) {
			return console.log(process.env.ELECTRON_VERSION);
		}

		return console.log('latest'); // not specified, print out default.
	})
});