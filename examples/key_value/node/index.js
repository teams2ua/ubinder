key_store_node = require('bindings')('key_store_node')
messages = require('./generated/messages_pb')

function OnRequest(req, callback) {
    
}

function OnNotification(notification) {
    
}

register = function () {
    key_store_node.registerLib(OnRequest, OnNotification);
}

createAddValue = function(key, value) {
    var addVal = new messages.AddValueRequest();
    addVal.setKey(key);
    addVal.setValue(value);
    var req = new messages.Request();
    req.setRequestType(messages.RequestType.ADD_VALUE);
    req.setRequestBody(addVal.serializeBinary());
    return req;
}

createGetValue = function (key) {
    var getVal = new messages.GetValueRequest();
    getVal.setKey(key);
    var req = new messages.Request();
    req.setRequestType(messages.RequestType.GET_VALUE);
    req.setRequestBody(getVal.serializeBinary());
    return req;
}

sendAddValue = function(key, value) {
    return new Promise((resolve, reject) => {
        var msg = createAddValue(key, value).serializeBinary()
        key_store_node.sendRequest(msg, 
            function(data){
                var resp = messages.AddValueResponse.deserializeBinary(data);
                if (resp.getError()=='') {
                    resolve();
                }else {
                    reject(new Error(resp.getError()));
                }
            });        
        });
}

sendGetValue = function(key) {
    return new Promise((resolve, reject) => {
        var msg = createGetValue(key).serializeBinary()
        key_store_node.sendRequest(msg, 
            function(data){
                var resp = messages.GetValueResponse.deserializeBinary(data);
                if (resp.getError()=='') {
                    resolve(resp.getValue());
                }else {
                    reject(new Error(resp.getError()));
                }
            });
    });
}


run_test = function() {
    register();
    sendAddValue("key1", "value1")
        .then(() => sendGetValue("key1"))
        .then((val) => console.log(val));
    sendAddValue("key2", "value2")
        .then(() => sendGetValue("key2"))
        .then((val) => console.log(val));
    sendAddValue("key3", "value3")
        .then(() => sendGetValue("key3"))
        .then((val) => console.log(val));
}
