const key_store_node = require('bindings')('key_store_node')
const messages = require('./generated/messages_pb')

function OnRequest(req, callback) {
    
}

function OnNotification(notification) {
    
}

function addValue(key, value) {
    var addVal = new messages.AddValueRequest();
    addVal.setKey(key);
    addVal.setValue(value);
    var req = new messages.Request();
    req.setRequestType(messages.RequestType.ADD_VALUE);
    req.setRequestBody(messages.RequestType.ADD_VALUE);
}

function getValue(key) {
    return new Promise((resolve, reject)=> {
        
        ubinder.sendRequest()
    })
}

ubinder.init(OnRequest, OnNotification);