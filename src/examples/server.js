var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);

io.on('connection', function(socket){
  socket.on('chat', function(json){
    console.log('chat event raised:',json);
    io.emit('chat', json);
  });
  socket.on('message', function(msg){
    console.log('message received:',msg);
    io.send(msg);
  });
});

http.listen(3000, function(){
  console.log('listening on *:3000');
});
