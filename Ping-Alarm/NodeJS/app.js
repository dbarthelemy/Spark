var https = require('https');
var util = require('util');
var exec = require('child_process').exec;

var Event = {
    None: 0,
    Named: 1,
};

https.get('https://api.spark.io/v1/devices/YOUR_DEVICE_ID/events?access_token=YOUR_ACCESS_TOKEN', function(res) 
{
	console.log("statusCode: ", res.statusCode);
	console.log("headers: ", res.headers);

	var state = Event.None;
	var eventName = '';
	var eventData = {};

	res.on('data', function(d) {
		var json = d.toString();
//    	console.log(json);

		if (state == Event.None) {
		    var matchStr = 'event: alarm';
		    var idx = json.search(matchStr);
		    if (idx == 0) {
		    	eventName = json.substr(matchStr.length, json.length-matchStr.length).trim();
		    	state = Event.Named;
		    }
		}
		if (state == Event.Named) {
		    var matchStr = 'data:';
		    var idx = json.search(matchStr);
		    if (idx >= 0) {
		    	var eventDataString = json.substr(idx+matchStr.length, json.length-matchStr.length-idx).trim();
	            eventData = JSON.parse(eventDataString);
	            var message = 'Detect something at ' + eventData.published_at;
	    		console.log(message);
		        
		        sendPushNotification(message);

				eventName = '';
				eventData = {};
				state = Event.None;
		    }
		}
	}).on('error', function(e) {
		console.error(e);
	});
});

/*
 *  Parse Helper function
 */
var sendPushNotification = function(message)
{
    var jsonData = '{"where": { "deviceType": "ios" }, "data":{"alert": "' + message + '", "badge": "Increment"}}';
               
    var curlCmd = 'curl -X POST -H "X-Parse-Application-Id: YOUR_PARSE_APP_ID" -H "X-Parse-REST-API-Key: YOUR_PARSE_REST_API_KEY" -H "Content-Type: application/json" -d \'' + jsonData + '\' -k https://api.parse.com/1/push';
    console.log('[' + new Date().toISOString() + '] (sendPushNotification) Execute: ' + curlCmd);
               
    exec(curlCmd, function (error, stdout, stderr) {
        if (error === null) {
            var response = JSON.parse(stdout);
            console.log('[' + new Date().toISOString() + '] (sendPushNotification) : ' + util.inspect(response, false, null));
        }
        else {
            console.log('[' + new Date().toISOString() + '] (sendPushNotification) Error from Curl: ' + error.message);
        }
    });
}
