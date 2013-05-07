var http = require("http");
var url = require("url");
var querystring = require("querystring");
var twilio = require("twilio");
var sys = require("sys");


var data = {

	"+19734876086": "In room, knock",
  "+19737380544": "pooping"
}

var users = {

	"robertf224": "+19734876086",
  "katief818": "+19737380544"

}




var express = require('express');
var app = express.createServer(express.logger());
app.use(express.bodyParser());


app.post('/respondToSMS', function(req, res) { 

	if (twilio.validateExpressRequest(req, '8b6089970e77555da09217697ba9e606')) {

  		var message = req.body.Body;
  		var from = req.body.From;

        console.log('message: ' + message);
        console.log('from: ' + from);

        data[from] = message;
   }
   else {
        res.send('You do not belong here');
        console.log('Someone tried to access without permission');
    }

});




app.get('/:username', function(req,res){
	if(users[req.params.username] == undefined) {
		res.send('not a valid user\n');
	}
	else {
		res.send(data[users[req.params.username]]);
	}
    
});

app.get('/', function(req, res) {
	// send method adds content-length header for you
  	res.send('Welcome to the messageserver\n');
});








var port = process.env.PORT || 5000;
app.listen(port, function() {
  console.log("Listening on " + port);
});
