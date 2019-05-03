key_store_node = require('bindings')('key_store_node')
messages = require('./generated/messages_pb')

function OnRequest(req, callback) {
    
}

function OnNotification(notification) {
    
}

addValue = function(key, value) {
    var addVal = new messages.AddValueRequest();
    addVal.setKey(key);
    addVal.setValue(value);
    var req = new messages.Request();
    req.setRequestType(messages.RequestType.ADD_VALUE);
    req.setRequestBody(addVal.serializeBinary());
    return req;
}
/*
function getValue(key) {
    return new Promise((resolve, reject)=> {
        
        ubinder.sendRequest()
    })
}
  */
register = function () {
    key_store_node.registerLib(OnRequest, OnNotification);
}

sendAddValue = function() {
    key_store_node.registerLib(OnRequest, OnNotification);
    var msg = addValue("key", "value").serializeBinary()
    key_store_node.sendRequest(msg, function(data){console.log("incoming Data");console.log(data);});

}