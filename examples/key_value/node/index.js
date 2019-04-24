const key_store_node = require('bindings')('key_store_node')
const messages = require('./generated/messages_pb')

function OnRequest(req, callback) {
    
}

function OnNotification(notification) {
    
}

function addValue(key, value) {
    var addValueRequest = new messages.AddValueRequest()
    addValueRequest.setKey(key)
    addValueRequest.setValue(value)
    var req = new messages.Request()
    req.setRequestType(messages.Request.ADD_VALUE)
    req.setRequestBody(addValueRequest.serializeBinary())
    ubinder.sendRequest(req.serializeBinary(), )
}

function getValue(key) {
    return new Promise((resolve, reject)=> {
        
        ubinder.sendRequest()
    })
}

ubinder.init(OnRequest, OnNotification);

